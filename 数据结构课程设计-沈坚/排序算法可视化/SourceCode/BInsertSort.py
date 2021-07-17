# 折半插入排序
from Visual import DataSeq

# 稳定排序
def BInsertSort(v, ck):
    if (ck == 1):
        v.StartTimer()
        n = v.length
        for i in range(1, n, 1):
            tmp = v.data[i]
            tmp_c = v.color[i]
            low = 0
            high = i - 1
            while low <= high:
                m = (low + high) // 2
                v.Cmp(i, m)
                if tmp < v.data[m]:
                    high = m - 1
                else:
                    low = m + 1
            for j in range(i - 1, high, -1):
                v.SetVal(j + 1, v.data[j], v.color[j])
            v.SetVal(high + 1, tmp, tmp_c)
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        n = v.length
        for i in range(1, n, 1):
            tmp = v.data[i]
            tmp_c = v.color[i]
            low = 0
            high = i - 1
            while low <= high:
                m = (low + high) // 2
                v.Cmp(i, m)
                if tmp > v.data[m]:
                    high = m - 1
                else:
                    low = m + 1
            for j in range(i - 1, high, -1):
                v.SetVal(j + 1, v.data[j], v.color[j])
            v.SetVal(high + 1, tmp, tmp_c)
        v.Visualize()
        v.StopTimer()