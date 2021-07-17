# 冒泡排序
from Visual import DataSeq

# 稳定排序
def BubbleSort(v, ck):
    if (ck == 1):
        v.StartTimer()
        n = v.length
        for i in range(n - 1, -1, -1):
            for j in range(0, i, 1):
                if v.data[j] > v.data[j + 1]:
                    v.Swap(j, j + 1)
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        n = v.length
        for i in range(n - 1, -1, -1):
            for j in range(0, i, 1):
                if v.data[j] < v.data[j + 1]:
                    v.Swap(j, j + 1)
        v.Visualize()
        v.StopTimer()