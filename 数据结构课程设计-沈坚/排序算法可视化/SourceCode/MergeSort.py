# 二路归并排序
import copy
from Visual import DataSeq

# 稳定排序
# 递归方式实现
def Merge(v, L, M1, R, ck):
    if (ck == 1):
        tmp = copy.copy(v.data)
        tmp_c = copy.copy(v.color)
        M = M1 - 1
        i, j, k = L, M1, L
        while i <= M and j <= R:
            if tmp[i] <= tmp[j]:
                v.SetVal(k, tmp[i], tmp_c[i])
                i += 1
            else:
                v.SetVal(k, tmp[j], tmp_c[j])
                j += 1
            k += 1
        while i <= M:
            v.SetVal(k, tmp[i], tmp_c[i])
            k, i = k + 1, i + 1
        while j <= R:
            v.SetVal(k, tmp[j], tmp_c[j])
            k, j = k + 1, j + 1
    else:
        tmp = copy.copy(v.data)
        tmp_c = copy.copy(v.color)
        M = M1 - 1
        i, j, k = L, M1, L
        while i <= M and j <= R:
            if tmp[i] >= tmp[j]:
                v.SetVal(k, tmp[i], tmp_c[i])
                i += 1
            else:
                v.SetVal(k, tmp[j], tmp_c[j])
                j += 1
            k += 1
        while i <= M:
            v.SetVal(k, tmp[i], tmp_c[i])
            k, i = k + 1, i + 1
        while j <= R:
            v.SetVal(k, tmp[j], tmp_c[j])
            k, j = k + 1, j + 1

# 左闭右闭
def Sort(v, L, R, ck):
    if R > L:
        M = (L + R) // 2
        Sort(v, L, M, ck)
        Sort(v, M + 1, R, ck)
        Merge(v, L, M + 1, R, ck)

def MergeSort(v, ck):
    v.StartTimer()
    n = v.length
    Sort(v, 0, n - 1, ck)
    v.Visualize()
    v.StopTimer()
