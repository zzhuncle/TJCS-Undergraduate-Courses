# 直接选择排序
from Visual import DataSeq

# 不稳定排序
def SelectSort(v, ck):
    if (ck == 1):
        v.StartTimer()
        n = v.length
        for i in range(n):
            for j in range(i, n):
                if v.data[j] < v.data[i]:
                    v.Swap(i, j)
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        n = v.length
        for i in range(n):
            for j in range(i, n):
                if v.data[j] > v.data[i]:
                    v.Swap(i, j)
        v.Visualize()
        v.StopTimer()