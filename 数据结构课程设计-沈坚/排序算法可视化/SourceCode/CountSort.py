import copy
from Visual import *

# 稳定排序
def Reset(v, data):
    v.data = copy.copy(data)
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

def CountSort(root, v, ck):
    if (ck == 1):
        v.StartTimer()
        n = v.length

        data = copy.copy(v.data)
        data1 = copy.copy(v.data)

        # 求得最大的元素，便于计算色块高度，便于显示
        max_data = -1
        for i in range(n):
            max_data = max(max_data, copy.copy(data[i]))

        data.sort()
        map = {}
        count = 0
        for i in range(n):
            if data[i] not in map.keys():
                map[data[i]] = count
                count += 1

        # 展示计数时的动态变化，此时色块个数为原始数据中不重复的元素个数
        n1 = len(map)
        v.data = [0 for _ in range(n1)]
        _data = [0 for _ in range(max_data)]
        Reset(v, v.data)
        for i in range(n):
            v.SetVal(map[data1[i]], v.data[map[data1[i]]] + 1, i + 1)
            _data[data1[i] - 1] += 1

        # 展示原始数据排序的动态变化
        Reset(v, [max_data for _ in range(n)])
        j = 0
        for i in range(max_data):
            tmp = _data[i]
            while tmp > 0:
                v.SetVal(j, i + 1, j + 1)
                tmp -= 1
                j += 1
        v.Visualize()
        v.StopTimer()
    else:
        v.StartTimer()
        n = v.length

        data = copy.copy(v.data)
        data1 = copy.copy(v.data)

        # 求得最大的元素，便于计算色块高度，便于显示
        max_data = -1
        for i in range(n):
            max_data = max(max_data, copy.copy(data[i]))

        data.sort()
        map = {}
        count = 0
        for i in range(n):
            if data[i] not in map.keys():
                map[data[i]] = count
                count += 1

        # 展示计数时的动态变化，此时色块个数为原始数据中不重复的元素个数
        n1 = len(map)
        v.data = [0 for _ in range(n1)]
        _data = [0 for _ in range(max_data)]
        Reset(v, v.data)
        for i in range(n):
            v.SetVal(map[data1[i]], v.data[map[data1[i]]] + 1, i + 1)
            _data[data1[i] - 1] += 1

        # 展示原始数据排序的动态变化
        Reset(v, [max_data for _ in range(n)])
        j = 0
        for i in range(max_data - 1, -1, -1):
            tmp = _data[i]
            while tmp > 0:
                v.SetVal(j, i + 1, j + 1)
                tmp -= 1
                j += 1
        v.Visualize()
        v.StopTimer()