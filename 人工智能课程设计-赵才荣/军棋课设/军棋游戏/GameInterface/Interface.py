import pygame

from common import *


class Interface:
    """
    陆军棋棋盘类
    """
    MODE = {  # size:(width, height)
        'big': {
            'screen_size': (615, 800),
            'bg_size': (615, 800),
            'chess_size': (56, 32),
            'start_x': 40,
            'start_y': 45,
            'delta_x': 120,
            'delta_y': 52,
            'extra_y': 26,
            'font_size': 80
        },
        'small': {
            'screen_size': (461, 615),
            'bg_size': (461, 615),
            'chess_size': (42, 24),
            'start_x': 31,
            'start_y': 36,
            'delta_x': 89,
            'delta_y': 40,
            'extra_y': 20,
            'font_size': 60
        }
    }

    def __init__(self, mode='small', caption='陆军棋', img_path='./img/'):
        # 设置参数
        pygame.init()
        self.mode = mode
        self.screen = pygame.display.set_mode(Interface.MODE[mode]['screen_size'])
        pygame.display.set_caption(caption)
        self.bg_size = Interface.MODE[mode]['bg_size']
        self.chess_size = Interface.MODE[mode]['chess_size']
        self.font_size = Interface.MODE[mode]['font_size']

        # 加载图片
        self.img_board = pygame.image.load(img_path + "board.png").convert_alpha()
        self.img_board = pygame.transform.scale(self.img_board, self.bg_size)
        self.img_chess = {}
        for i in [*CHESS, Chess('RED', 0, 0), Chess('BLACK', 0, 0)]:
            if i.name == 'BLANK': continue
            self.img_chess[i.name] = pygame.transform.scale(pygame.image.load(img_path + i.name + ".png"),
                                                            self.chess_size)
        self.img_act = pygame.transform.scale(
            pygame.image.load(img_path + "activate.png"),
            self.chess_size
        )

        self.start_x = Interface.MODE[mode]['start_x']
        self.start_y = Interface.MODE[mode]['start_y']
        self.delta_x = Interface.MODE[mode]['delta_x']
        self.delta_y = Interface.MODE[mode]['delta_y']
        self.extra_y = Interface.MODE[mode]['extra_y']

        self.fpsCLOCK = pygame.time.Clock()
        self.FPS = 40

    def show(self, chess, activate=None):
        """
        :param chess: 棋盘矩阵
        :param activate: 激活的坐标(row, col)或坐标列表[()]，如果是列表，则全部激活，如果是当个坐标，空白处无法激活。
        :return:
        """
        self.screen.blit(self.img_board, (0, 0))  # 加载背景
        for x in range(5):
            for y in range(13):
                pos = [self.start_x + x * self.delta_x, self.start_y + y * self.delta_y]
                if y == 6:
                    pos[1] += self.extra_y
                elif y > 6:
                    pos[1] += 2 * self.extra_y
                if chess[y, x] == BLANK:
                    if type(activate) == list and (y, x) in activate:
                        self.screen.blit(self.img_act, pos)
                    continue
                self.screen.blit(self.img_chess['RED' if chess[y, x] < 0 else 'BLACK'], pos)
                self.screen.blit(self.img_chess[CHESS[abs(chess[y, x])].name], pos)
                if (y, x) == activate or (type(activate) == list and (y, x) in activate):
                    self.screen.blit(self.img_act, pos)
        pygame.display.flip()
        self.fpsCLOCK.tick(self.FPS)

    def xy_to_rc(self, mouse_x, mouse_y):
        """
        将鼠标的点击位置映射成棋盘位置
        :param mouse_x: 鼠标点击的x坐标
        :param mouse_y: 鼠标点击的y坐标
        :return: 返回触发的棋盘位置(row, col)，点击无效区域返回None
        """
        for x in range(5):
            for y in range(13):
                pos = [self.start_x + x * self.delta_x, self.start_y + y * self.delta_y]  # (x,y)
                if y == 6:
                    pos[1] += self.extra_y
                elif y > 6:
                    pos[1] += 2 * self.extra_y
                if pos[0] <= mouse_x < pos[0] + self.chess_size[0] and \
                        pos[1] <= mouse_y < pos[1] + self.chess_size[1]:
                    return y, x
        return None

    def end(self, winner):
        font = pygame.font.SysFont('microsoft Yahei', self.font_size, bold=True)
        info = font.render(winner + ' Win!', False, (0, 0, 255))
        self.screen.blit(info,
                         (self.bg_size[0] / 2 - 3 * self.font_size / 2, self.bg_size[1] / 2 - 0.5 * self.font_size / 2))
        pygame.display.flip()
