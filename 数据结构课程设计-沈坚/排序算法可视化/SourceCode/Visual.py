import numpy as np
import time
import cv2
from tkinter import *
from PIL import Image, ImageTk
import copy

COL_SIZE = 820
ROW_SIZE = 480

color_dict = {1 : (0, 215, 255), # 金黄色
        2 : (33, 145, 237), # 胡萝卜色
        3 : (240, 255, 240), # 蜜露橙
        4 : (0, 255, 255), # 黄色
        5 : (201, 230, 252), # 蛋壳色
        6 : (87, 207, 207), # 香蕉色
        7 : (212, 255, 127), # 碧绿色
        8 : (208, 224, 64), # 青绿
        9 : (64, 145, 61), # 钴绿
        10 : (201, 161, 51), # 孔雀蓝
        11 : (171, 89, 61), # 钴蓝
        12 : (255, 0, 0),  # 蓝色
        13 : (203, 192, 255),  # 粉红色
        14 : (230, 240, 250),  # 亚麻色
        15 : (192, 192, 192),  # 灰色
        16 : (240, 32, 160),  # 紫色
        17 : (214, 112, 218),  # 淡紫色
        18 : (221, 160, 221),  # 梅红色
        19 : (105, 118, 112),  # 石板灰色
        20 : (255, 255, 0),  # 青色
}

class DataSeq:
    WHITE = (255, 255, 255)
    RED = (0, 0, 255)
    BLACK = (0, 0, 0)
    YELLOW = (0, 127, 255)

    def __init__(self, root, data, time_interval):
        self.data = copy.copy(data)
        self.length = len(data)
        max_data = -1
        for i in range(self.length):
            max_data = max(max_data, copy.copy(data[i]))
        self.root = root
        self.color = [i + 1 for i in range(self.length)]
        self._unit_width = COL_SIZE / self.length * 3 / 5
        if (max_data == 0):
            self._unit_height = 30
        else:
            self._unit_height = ROW_SIZE / max_data
        self._interval_block = COL_SIZE / self.length * 2 / 5
        self.col_size = COL_SIZE
        self.row_size = ROW_SIZE + 24
        self.imgLabel = Label(self.root)

        self.time_interval = time_interval # 时间间隔
        self.Getfigure()
        self.InitTime()
        self.Visualize()

    def Getfigure(self):
        self.figure = np.full((self.row_size, self.col_size, 3), 0, dtype = np.uint8)
        for i in range(self.length):
            val = self.data[i]
            self._set_figure(i, val)

    def SetTimeInterval(self, time_interval):
        self.time_interval = time_interval

    def InitTime(self):
        self.start = time.time()
        self.time = 0
        self.StopTimer()

    def StartTimer(self):
        self.start_flag = True
        self.start = time.time()

    def StopTimer(self):
        self.start_flag = False

    def GetTime(self):
        if self.start_flag:
            self.time = time.time() - self.start

    def _set_figure(self, idx, val):
        min_col = int(idx * (self._interval_block + self._unit_width))
        max_col = int(min_col + self._unit_width)
        min_row = int(- 25 - val * self._unit_height)
        self.figure[ : , min_col : max_col] = self.BLACK
        self.figure[min_row : -24, min_col : max_col] = color_dict[self.color[idx]]

    def SetColor(self, img, marks, color):
        for idx in marks:
            min_col = int(idx * (self._interval_block + self._unit_width))
            max_col = int(min_col + self._unit_width)
            min_row = int(- 25 - self.data[idx] * self._unit_height)
            img[min_row : -24, min_col : max_col] = color

    def Mark(self, img, marks, color):
        self.SetColor(img, marks, color)

    def SetVal(self, idx, val1, val2):
        self.data[idx] = val1
        self.color[idx] = val2
        self._set_figure(idx, val1)
        self.Visualize((idx,))

    def Swap(self, idx1, idx2):
        self.data[idx1], self.data[idx2] = self.data[idx2], self.data[idx1]
        self.color[idx1], self.color[idx2] = self.color[idx2], self.color[idx1]
        self._set_figure(idx1, self.data[idx1])
        self._set_figure(idx2, self.data[idx2])
        self.Visualize((idx1, idx2))

    def Cmp(self, idx1, idx2):
        self.Visualize((idx1, idx2))

    def Show(self, img):
        imgCVRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img2 = Image.fromarray(imgCVRGB)
        self.imgTK = ImageTk.PhotoImage(image = img2)
        self.imgLabel.configure(image = self.imgTK)  # 图像显示
        self.root.update_idletasks()
        self.imgLabel.place(x = 30, y = 60)


    def Visualize(self, mark1 = None, mark2 = None):
        img = self.figure.copy()
        if mark1:
            self.Mark(img, mark1, self.RED)
        if mark2:
            self.Mark(img, mark2, self.YELLOW)
        self.GetTime()
        cv2.putText(img, " Time:%02.2fs" % self.time, (20, 20), cv2.FONT_HERSHEY_PLAIN, 1, self.RED, 2)
        for idx in range(self.length):
            cv2.putText(img, "%d" % self.data[idx], (int(idx * (self._interval_block + self._unit_width)), int(self.row_size - 10)), cv2.FONT_HERSHEY_PLAIN, 1, self.RED, 2)
        self.Show(img)
        cv2.waitKey(self.time_interval)