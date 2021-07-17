# 梳排序
from Visual import DataSeq

# 不稳定排序
def CombSort(v, ck):
    if (ck == 1):
        v.StartTimer()
        n = v.length
        D = int(n / 1.3)
        while D:
            for i in range(n):
                for j in range(i + D, n, D):
                    if v.data[j - D] > v.data[j]:
                        v.Swap(j - D, j)
            D = int(D / 1.3)
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        n = v.length
        D = int(n / 1.3)
        while D:
            for i in range(n):
                for j in range(i + D, n, D):
                    if v.data[j - D] < v.data[j]:
                        v.Swap(j - D, j)
            D = int(D / 1.3)
        v.Visualize()
        v.StopTimer()