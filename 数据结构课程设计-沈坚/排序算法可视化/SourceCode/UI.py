# 简单界面设计
from Visual import *
from BubbleSort import BubbleSort
from CountSort import CountSort
from CombSort import CombSort
from HeapSort import HeapSort
from InsertSort import InsertSort
from MergeSort import MergeSort
from QuickSort import QuickSort
from SelectSort import SelectSort
from ShellSort import ShellSort
from BInsertSort import BInsertSort
from tkinter import filedialog

init_data = [16, 7, 16, 8, 1, 11, 2, 10, 6, 16, 5, 3, 12, 4, 15, 13]
if __name__ == "__main__":
    root = Tk()
    root.title("排序算法可视化")
    root.geometry('900x686')
    root.minsize(900, 686)
    root.maxsize(900, 686)

    time_lay = StringVar()
    time_lay.set("1")

    r = StringVar()
    r.set("1")

    v = DataSeq(root, init_data, time_interval = int(time_lay.get()))

    # 重置界面
    def Reset():
        global v
        v.SetTimeInterval(int(time_lay.get()))
        v.data = copy.copy(init_data)
        v.length = len(v.data)
        n = v.length
        max_data = -1
        for i in range(n):
            max_data = max(max_data, copy.copy(v.data[i]))
        v._unit_width = COL_SIZE / n * 3 / 5
        if (max_data == 0):
            v._unit_height = 30
        else:
            v._unit_height = ROW_SIZE / max_data
        v._interval_block = COL_SIZE / n * 2 / 5
        v.color = [i + 1 for i in range(n)]

        for i in range(n):
            v._set_figure(i, v.data[i])
        v.Getfigure()
        v.InitTime()
        v.Visualize()

    # 打开文件
    def Openfile():
        root = Tk()
        root.withdraw()
        filepath = filedialog.askopenfilename()
        data = []
        with open(filepath, "r") as f:
            for line in f:
                wordlist = line.split()
                data.extend(wordlist)
        f.close()
        data = list(map(int, data))
        global init_data
        init_data = data
        Reset()

    width = 88
    col_gap = 170
    left_base = 50
    first_line = 590
    second_line = 630

    lay_label = Label(root, text = "延时设置(重置后生效):").place(x = left_base + 160, y = 20)
    lay_entry = Entry(root, width = width // 8, textvariable = time_lay).place(x = left_base + col_gap + 120, y = 22)
    # 多选框
    frame = Frame(root, width = width // 2)
    Radiobutton(frame, variable = r, value = '1', text = '正序').grid(row = 0, column = 0)
    Radiobutton(frame, variable = r, value = '2', text = '逆序').grid(row = 0, column = 1)
    frame.place(x = left_base + col_gap + 280, y = 20)

    file_label = Button(root, text = "获取数据(15~20个)", command = Openfile).place(x = left_base + col_gap + 290 + 200, y = 20)

    button0 = Button(root, text = "重置/初始化", command = Reset).place(x = left_base, y = 16, width = width)
    button1 = Button(root, text = "直接插入排序", command = lambda: InsertSort(v, int(r.get()))).place(x = left_base, y = first_line, width = width)
    button2 = Button(root, text = "折半插入排序", command = lambda: BInsertSort(v, int(r.get()))).place(x = col_gap + left_base, y = first_line, width = width)
    button3 = Button(root, text = "希尔排序", command = lambda: ShellSort(v, int(r.get()))).place(x = 2 * col_gap + left_base, y = first_line, width = width)
    button4 = Button(root, text = "冒泡排序", command = lambda: BubbleSort(v, int(r.get()))).place(x = 3 * col_gap + left_base, y = first_line, width = width)
    button5 = Button(root, text = "快速排序", command = lambda: QuickSort(v, int(r.get()))).place(x = 4 * col_gap + left_base, y = first_line, width = width)
    button6 = Button(root, text = "直接选择排序", command = lambda: SelectSort(v, int(r.get()))).place(x = left_base, y = second_line, width = width)
    button7 = Button(root, text = "堆排序", command = lambda: HeapSort(v, int(r.get()))).place(x = col_gap + left_base, y = second_line, width = width)
    button8 = Button(root, text = "二路归并排序", command = lambda: MergeSort(v, int(r.get()))).place(x = 2 * col_gap + left_base, y = second_line, width = width)
    button9 = Button(root, text = "梳排序", command = lambda: CombSort(v, int(r.get()))).place(x = 3 * col_gap + left_base, y = second_line, width = width)
    button10 = Button(root, text = "计数排序", command = lambda: CountSort(root, v, int(r.get()))).place(x = 4 * col_gap + left_base, y = second_line, width = width)
    root.mainloop()