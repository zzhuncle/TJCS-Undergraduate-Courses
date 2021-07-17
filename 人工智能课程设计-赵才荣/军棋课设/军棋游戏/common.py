class Chess:
    def __init__(self, name, num, n):
        self.num = num
        self.N = n
        self.name = name


BLANK = 0  # 空白
FLAG = 1  # 军旗
DI = 2  # 地雷
ZHA = 3  # 炸弹
GONG = 4  # 工兵
PAI = 5  # 排长
LIAN = 6  # 连长
YING = 7  # 营长
TUAN = 8  # 团长
LV = 9  # 旅长
SHI = 10  # 师长
JUN = 11  # 军长
SI = 12  # 司令
CHESS = [
    Chess('BLANK', 0, -1),
    Chess('FLAG', 1, 1),
    Chess('DI', 2, 3),
    Chess('ZHA', 3, 2),
    Chess('GONG', 4, 3),
    Chess('PAI', 5, 3),
    Chess('LIAN', 6, 3),
    Chess('YING', 7, 2),
    Chess('TUAN', 8, 2),
    Chess('LV', 9, 2),
    Chess('SHI', 10, 2),
    Chess('JUN', 11, 1),
    Chess('SI', 12, 1)
]

# [0,13]
B = 1 # 对方黑方大写字母
R = -1 # 本方红方小写字母

# 例：黑方排长的编码值 = B * pai， 红方军长的编码值 = R * jun

XINGYING = [(2, 1), (2, 3), (3, 2), (4, 1), (4, 3), (8, 1), (8, 3), (9, 2), (10, 1), (10, 3)]  # 行营坐标
SHANJIE = [(6, 1), (6, 3)]  # 山界坐标
DABENYING = [(0, 1), (0, 3), (12, 1), (12, 3)]  # 大本营坐标

CMD_QUIT = "end"
CMD_INIT = "init"
