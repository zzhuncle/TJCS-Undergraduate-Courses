import re

import numpy as np

n2b_mapper = {"F": 1, "P": 5, "V": 9, "D": 2, "L": 6, "S": 10, "Z": 3, "Y": 7, "J": 11, "G": 4, "T": 8, "A": 12,
              "f": -1, "p": -5, "v": -9, "d": -2, "l": -6, "s": -10, "z": -3, "y": -7, "j": -11, "g": -4, "t": -8,
              "a": -12}

b2n_mapper = {1: 'F', 5: 'P', 9: 'V', 2: 'D', 6: 'L', 10: 'S', 3: 'Z', 7: 'Y', 11: 'J', 4: 'G', 8: 'T', 12: 'A',
              -1: 'f', -5: 'p', -9: 'v', -2: 'd', -6: 'l', -10: 's', -3: 'z', -7: 'y', -11: 'j', -4: 'g', -8: 't',
              -12: 'a'}


class ChessNotationError(RuntimeError):
    def __str__(self) -> str:
        return "Chessboard Notation Error"


def check(ncn):
    if re.match("^\w{1,5}(/[\w]{1,5}){12}(\s[0-9]+\s[0-9]+){0,1}$", ncn) is not None:
        return True
    else:
        raise ChessNotationError


def parse_ncn(ncn: str) -> (np.ndarray, int, int):
    check(ncn)
    splitted = ncn.split()    # 字符串分割
    if len(splitted) == 3:
        cnt = int(splitted[1])
        total_cnt = int(splitted[2])
        board_notation = splitted[0]
    else:
        cnt = None
        total_cnt = None
        board_notation = splitted[0]
    board = np.zeros((13, 5), np.int)
    for r, row in enumerate(board_notation.split("/")):
        c_i = 0
        for col in row:
            if col.isalpha():# 字符串只含字母，而且为空
                board[r, c_i] = n2b_mapper[col]# 自己定义的字典
                c_i += 1
            elif col.isnumeric():
                c_i += int(col)
            else:
                raise ChessNotationError
    return board, cnt, total_cnt


def board_to_ncn(board: np.ndarray) -> str:
    assert board.shape == (13, 5)
    ncn = ""
    for row in range(13):
        if row is not 0:
            ncn += "/"
        zero_cnt = 0
        for col in range(5):
            element = board[row, col]
            if element:
                if zero_cnt is not 0:
                    ncn += str(zero_cnt)
                    zero_cnt = 0
                ncn += b2n_mapper[element]
            else:
                zero_cnt += 1
        if zero_cnt is not 0:
            ncn += str(zero_cnt)
    return ncn


def parse_ncn_move(move: str) -> list:
    assert len(move) == 4
    st = (ord("m") - ord(move[0]), int(move[1]))
    ed = (ord("m") - ord(move[2]), int(move[3]))
    return [st, ed]


def move_to_ncn(move: list) -> str:
    return chr(ord("m") - move[0][0]) + str(move[0][1]) + chr(ord("m") - move[1][0]) + str(move[1][1])
