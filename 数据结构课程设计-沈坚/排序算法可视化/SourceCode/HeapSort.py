# 堆排序
from Visual import DataSeq

# 不稳定排序
def HeapAdjust(v, s, m, ck):
    if (ck == 1):
        rc = v.data[s] # 保存r[s]
        rc_c = v.color[s]
        j = 2 * s + 1
        while j <= m:
            if j < m and v.data[j] < v.data[j + 1]:
                j += 1
            if rc >= v.data[j]:
                break
            v.SetVal(s, v.data[j], v.color[j])
            s = j
            j = 2 * j + 1
        v.SetVal(s, rc, rc_c)
    else:
        rc = v.data[s] # 保存r[s]
        rc_c = v.color[s]
        j = 2 * s + 1
        while j <= m:
            if j < m and v.data[j + 1] < v.data[j]:
                j += 1
            if rc <= v.data[j]:
                break
            v.SetVal(s, v.data[j], v.color[j])
            s = j
            j = 2 * j + 1
        v.SetVal(s, rc, rc_c)

def HeapSort(v, ck):
    if (ck == 1):
        v.StartTimer()
        for i in range(v.length // 2 - 1, -1, -1):
            HeapAdjust(v, i, v.length - 1, ck)
        for i in range(v.length - 1, 0, -1):
            v.Swap(0, i)
            HeapAdjust(v, 0, i - 1, ck)
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        for i in range(v.length // 2 - 1, -1, -1):
            HeapAdjust(v, i, v.length - 1, ck)
        for i in range(v.length - 1, 0, -1):
            v.Swap(0, i)
            HeapAdjust(v, 0, i - 1, ck)
        v.Visualize()
        v.StopTimer()
