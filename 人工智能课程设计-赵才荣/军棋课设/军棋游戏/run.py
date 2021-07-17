import argparse
import logging
import subprocess
import sys
import threading
import time

import util
from GameEngine import *
from GameEngine.LBChess import end
from GameInterface.Interface import *

lock = threading.Lock()
event_end = threading.Event()


class ChessEngineError(RuntimeError):

    def __str__(self) -> str:
        return "AI Engine Error"


class EngineWrapper:

    def __init__(self, cmd: str) -> None:
        self.cmd = cmd
        self.process = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        while not self.process.stdout.readable():
            time.sleep(1)
        line = self.process.stdout.readline().decode().strip()
        if "ready" not in line:
            raise ChessEngineError
        self.result = None

    def _inference(self, cmd):
        self.process.stdin.write(cmd.encode())
        self.process.stdin.write("\n".encode())
        self.process.stdin.flush()
        line = self.process.stdout.readline()
        logging.info("decision made by " + repr(self) + ":" + repr(line))
        self.result = line.decode().strip()

    def make_decision(self, cmd: str):
        self.result = None
        t = threading.Thread(target=self._inference, args=(cmd,))
        t.start()
        while self.result is None:
            for e in pygame.event.get():
                if e.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
        t.join()
        return self.result

    def init_board(self, cmd: str):
        cmd = "init " + cmd
        self.process.stdin.write(cmd.encode())
        self.process.stdin.write("\n".encode())
        self.process.stdin.flush()

    def end(self):
        self.process.stdin.write("end\n".encode())
        self.process.stdin.flush()

    def __repr__(self) -> str:
        return "<" + self.cmd + ">"


class HumanInputWrapper:

    def __init__(self, role, ui, game) -> None:
        self.role = role
        self.ui = ui
        self.game = game
        self.status = "WAIT"
        self.activate = None

    def init_board(self, cmd: str):
        pass

    def end(self):
        pass

    def make_decision(self, cmd: str) -> str:
        lock.acquire()
        flag_end = False
        ret = None
        while True:
            if flag_end:
                break
            for e in pygame.event.get():

                # 按下叉号则关闭
                if e.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                # 玩家点击
                if e.type == pygame.MOUSEBUTTONDOWN and flag_end is False:  # 触发
                    mx, my = pygame.mouse.get_pos()
                    pos = self.ui.xy_to_rc(mx, my)

                    if pos is not None:  # 有效且合法
                        print(pos)
                        if self.status == 'WAIT':  # 如果未激活则进行激活
                            if self.game.data[pos[0], pos[1]] * self.role > 0:  # 成功激活的条件是，激活的棋子不能是空且与当前轮次相符合
                                self.status = 'ACT'
                                self.activate = pos[:]
                        else:  # 移动
                            if pos in self.game.get(self.activate):
                                self.status = 'WAIT'
                                ret = [self.activate, pos]
                                self.activate = None
                                print(ret)
                                flag_end = True
                            else:  # 移动发生错误，则取消激活
                                self.status = 'WAIT'
                                self.activate = None
                    else:  # 点击无效处则取消激活状态
                        self.status = 'WAIT'
                        self.activate = None

                    active_path = None
                    if self.activate is not None:
                        active_path = [self.activate] + self.game.get(self.activate)
                    self.ui.show(self.game.data, activate=active_path)
        lock.release()
        return util.move_to_ncn(ret)

    def __repr__(self) -> str:
        return "<manual, role" + str(self.role) + ">"


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    # 获取命令行参数
    parser = argparse.ArgumentParser(description='A land battle chess visualization.')
    parser.add_argument("-b", "--black", dest='black', help="Setting of the Black", type=str)
    parser.add_argument("-r", "--red", dest='red', help="Setting of the Red", type=str)
    parser.add_argument("-i", "--init_board", dest="init_ncn",
                        help="Initialize chessboard with an given NCN(Naive Chessboard Notation.)", type=str,
                        default="ppdfd/plldz/z1g1y/yt1ty/s1g1s/jvgva/5/JVGVA/S1G1S/YT1TY/Z1G1Y/PLLDZ/PPDFD")
    parser.add_argument("-l", "--large", dest="ui_mode", default=False, action="store_true",
                        help="Use large GUI.")
    parser.add_argument("-c", "--continue", dest="cnt_total", type=int, default=0, help="Resume from a given round.")
    parser = parser.parse_args()
    logging.info("args: " + repr(parser))

    # 实例化游戏引擎和游戏界面
    game_model = LBChess.LBChess(util.parse_ncn(parser.init_ncn)[0])
    UI = Interface(mode='small' if not parser.ui_mode else "big", img_path='./GameInterface/img/')

    # 显示游戏
    UI.show(game_model.data)

    # 初始化红黑双方
    op_0 = EngineWrapper(parser.red) if parser.red else HumanInputWrapper(-1, UI, game_model)
    op_1 = EngineWrapper(parser.black) if parser.black else HumanInputWrapper(1, UI, game_model)

    # 初始化状态变量
    cnt = 0
    cnt_total = parser.cnt_total
    flag_inited = False
    remain = -1

    while True:
        cur = op_0 if cnt_total % 2 == 0 else op_1
        turn = -1 if cnt_total % 2 == 0 else 1
        remain_cur = len(game_model.data.nonzero()[0])
        if remain == -1 or remain_cur < remain:
            remain = remain_cur
            cnt = 0
        logging.info("current round: " + repr(cur) + ", current turn: " + repr(turn) + ", round count: " + repr(
            cnt) + ", total round: " + repr(cnt_total))
        logging.info("current board: " + repr(util.board_to_ncn(game_model.data)))
        winner = end(game_model.data)
        if winner is not None:
            UI.end(winner='The Black' if winner == B else 'The Red')
            while 1:
                for event in pygame.event.get():
                    # 按下叉号则关闭
                    if event.type == pygame.QUIT:
                        pygame.quit()
                        sys.exit()

        if cur is not None:
            if not flag_inited:
                to_init = op_1 if cnt_total % 2 == 0 else op_0
                to_init.init_board(util.board_to_ncn(game_model.data) + " " + str(cnt) + " " + str(cnt_total))
                flag_inited = True
            raw_decision = cur.make_decision(util.board_to_ncn(game_model.data) + " " + str(cnt) + " " + str(cnt_total))
            decision = util.parse_ncn_move(raw_decision)
            logging.info("decision(parsed): " + repr(decision))
            game_model.move(*decision)
            UI.show(game_model.data, activate=decision)
            cnt += 1
            cnt_total += 1
