# 快速排序
from Visual import DataSeq

# 不稳定排序
def Partition(v, low, high, ck):
    if (ck == 1):
        pivotkey = v.data[low]
        while low < high:
            while low < high and v.data[high] >= pivotkey:
                high -= 1
            v.Swap(low, high)
            while low < high and v.data[low] <= pivotkey:
                low += 1
            v.Swap(low, high)
        return low
    else:
        pivotkey = v.data[low]
        while low < high:
            while low < high and v.data[high] <= pivotkey:
                high -= 1
            v.Swap(low, high)
            while low < high and v.data[low] >= pivotkey:
                low += 1
            v.Swap(low, high)
        return low


def Qsort(v, low, high, ck):
    if low < high:
        pivotloc = Partition(v, low, high, ck)
        Qsort(v, low, pivotloc - 1, ck)
        Qsort(v, pivotloc + 1, high, ck)

def QuickSort(v, ck):
    v.StartTimer()
    n = v.length
    Qsort(v, 0, n - 1, ck)
    v.Visualize()
    v.StopTimer()
