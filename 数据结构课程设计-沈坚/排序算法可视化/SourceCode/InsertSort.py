# 直接插入排序
from Visual import DataSeq

# 稳定排序
def InsertSort(v, ck):
    if (ck == 1):
        v.StartTimer()
        n = v.length
        for i in range(n):
            tmp = v.data[i]
            tmp_c = v.color[i]
            j = i
            while j >= 1 and v.data[j - 1] > tmp:
                v.SetVal(j, v.data[j - 1], v.color[j - 1])
                j -= 1
            v.SetVal(j, tmp, tmp_c)
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        n = v.length
        for i in range(n):
            tmp = v.data[i]
            tmp_c = v.color[i]
            j = i
            while j >= 1 and v.data[j - 1] < tmp:
                v.SetVal(j, v.data[j - 1], v.color[j - 1])
                j -= 1
            v.SetVal(j, tmp, tmp_c)
        v.Visualize()
        v.StopTimer()