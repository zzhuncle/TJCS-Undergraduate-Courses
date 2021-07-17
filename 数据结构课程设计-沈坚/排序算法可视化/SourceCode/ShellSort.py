# 希尔排序
from Visual import DataSeq
from math import ceil

# 不稳定排序
def ShellSort(v, ck):
    if (ck == 1):
        v.StartTimer()
        n = v.length
        D = ceil(n / 2)
        while D > 0:
            i = 0
            while i < n:
                tmp = v.data[i]
                tmp_c = v.color[i]
                j = i
                while j >= 1 and v.data[j - D] > tmp:
                    v.SetVal(j, v.data[j - D], v.color[j - D])
                    j -= D
                v.SetVal(j, tmp, tmp_c)
                i += D
            D //= 2
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        n = v.length
        D = ceil(n / 2)
        while D > 0:
            i = 0
            while i < n:
                tmp = v.data[i]
                tmp_c = v.color[i]
                j = i
                while j >= 1 and v.data[j - D] < tmp:
                    v.SetVal(j, v.data[j - D], v.color[j - D])
                    j -= D
                v.SetVal(j, tmp, tmp_c)
                i += D
            D //= 2
        v.Visualize()
        v.StopTimer()


