from math import log
import operator
import numpy as np

class Node(object):
    def __init__(self, children=[], best_feature=-1, best_feature_label='', value=0, depth=0, leaf=False, _class=None):
        self.children = children
        self.depth = depth
        self.best_feature = best_feature
        self.best_feature_label = best_feature_label
        self.split_value = value
        self.leaf = leaf
        self._class = _class

def calculate_shannon_entropy(data_set):
    label_counts = {}
    for sample in data_set:
        current_label = sample[-1]
        if current_label not in label_counts.keys():
            label_counts[current_label] = 0
        label_counts[current_label] += 1
    
    shannon_ent = 0.0
    for label in label_counts.keys():
        prob = float(label_counts[label]) / len(data_set)
        shannon_ent -= prob * log(prob, 2)
    return shannon_ent

def majority_count(class_list):
    class_count = {}
    # [['否'], ['是'], ['是']] -> ['否', '是', '是']
    class_list= np.mat(class_list).flatten().A.tolist()[0]  
    for vote in class_list:
        if vote not in class_count.keys():
            class_count[vote] = 0
        class_count[vote] += 1
    
    sorted_class_count = sorted(class_count.items(), key=operator.itemgetter(1), reverse=True)
    return sorted_class_count[0][0]

def split_data_set(data_set, axis, value):
    sub_data_set = []
    for sample in data_set:
        if sample[axis] == value:
            reduced_sample = sample[:axis]
            reduced_sample.extend(sample[axis+1:])
            sub_data_set.append(reduced_sample)
            
    return sub_data_set

def split_continuous_data_set(data_set, axis, value, direction):
    sub_data_set = []
    for sample in data_set:
        if direction == 0:
            if sample[axis] > value:  # 按照大于(>)该值进行划分
                reduce_data = sample[:axis]
                reduce_data.extend(sample[axis + 1:])
                sub_data_set.append(reduce_data)
        if direction == 1:
            if sample[axis] <= value:  # 按照小于等于(<=)该值进行划分
                reduce_data = sample[:axis]
                reduce_data.extend(sample[axis + 1:])
                sub_data_set.append(reduce_data)
    return sub_data_set

def choose_best_feature_to_split(data_set, labels):
    base_entropy = calculate_shannon_entropy(data_set)
    base_gain_ratio = 0.0
    best_feature = -1
    num_features = len(data_set[0]) - 1
    best_split_dict = {}  # 用来存储每一个连续型特征所对应最佳切分点的具体值
    for i in range(num_features):
        # 获取第 i 个特征的所有值
        feature_values = [sample[i] for sample in data_set]
        # 如果第 i 个特征是连续型数据
        if type(feature_values[0]).__name__ == 'float' or type(
                feature_values[0]).__name__ == 'int':
            # 将该特征的所有值按从小到大顺序排序
            sorted_feature_values = sorted(feature_values)
            # 取相邻两值的平均数做划分点
            split_list = []
            for j in range(len(feature_values) - 1):
                split_list.append(
                    (sorted_feature_values[j] + sorted_feature_values[j + 1]) / 2.0)
            # 遍历每个划分点
            for j in range(len(split_list)):
                # 计算该划分方式的条件信息熵
                new_entropy = 0.0
                value = split_list[j]
                greater_sub_data_set = split_continuous_data_set(data_set, i, value, 0)
                smaller_sub_data_set = split_continuous_data_set(data_set, i, value, 1)
                prob0 = len(greater_sub_data_set) / len(data_set)
                prob1 = len(smaller_sub_data_set) / len(data_set)
                new_entropy += prob0 * calculate_shannon_entropy(greater_sub_data_set)
                new_entropy += prob1 * calculate_shannon_entropy(smaller_sub_data_set)
                # 计算该划分方式的分裂信息
                split_info = 0.0
                split_info -= prob0 * log(prob0, 2)
                split_info -= prob1 * log(prob1, 2)
                # 计算信息增益率 = 信息增益 / 该划分方式的分裂信息
                gain_ratio = float(base_entropy - new_entropy) / split_info
                if gain_ratio > base_gain_ratio:
                    base_gain_ratio = gain_ratio
                    best_split = j
                    best_feature = i
            best_split_dict[labels[i]] = split_list[best_split]  # 最佳切分点
        # 如果第 i 个特征是离散型数据
        else:  
            unique_values = set(feature_values)
            # 计算该划分方式的条件信息熵和分裂信息
            new_entropy = 0.0
            split_info = 0.0
            for value in unique_values:
                sub_data_set = split_data_set(data_set, i, value)
                prob = len(sub_data_set) / float(len(data_set))
                new_entropy += prob * calculate_shannon_entropy(sub_data_set)
                split_info -= prob * log(prob, 2)
            # 若该特征的特征值都相同，即信息增益和分裂信息都为0，则跳过该特征
            if split_info == 0.0:
                continue
            # 计算信息增益率 = 信息增益 / 该划分方式的分裂信息
            gain_ratio = float(base_entropy - new_entropy) / split_info
            if gain_ratio > base_gain_ratio:
                best_feature = i
                base_gain_ratio = gain_ratio
                
    # 如果最佳切分特征是连续型，则最佳切分点为切分值
    if type(data_set[0][best_feature]).__name__ == 'float' or type(
            data_set[0][best_feature]).__name__ == 'int':
        best_feature_value = best_split_dict[labels[best_feature]]
        
    # 如果最佳切分特征是离散型，则最佳切分点为切分特征名称
    if type(data_set[0][best_feature]).__name__ == 'str':
        best_feature_value = labels[best_feature]
    
    return best_feature, best_feature_value

def create_tree(data_set, labels):
    class_split = [sample[-1] for sample in data_set]
    # 如果类别完全相同，则停止继续划分
    if class_split.count(class_split[0]) == len(class_split):
        return class_split[0]
        # return Node(_class=class_split[0], depth=0, leaf=True)
    # 遍历完所有特征时返回出现次数最多的类别
    if len(data_set[0]) == 1:
        return majority_count(class_split)
        # return Node(_class=majority_count(class_split), depth=0, leaf=True)
    best_feature, best_feature_value = choose_best_feature_to_split(data_set, labels)
    # 如果无法选出最优分类特征，返回出现次数最多的类别
    if best_feature == -1:  
        return majority_count(class_split)
        # return Node(_class=majority_count(class_split), depth=0, leaf=True)
    best_feature_label = labels[best_feature]
    tree = {best_feature_label: {}}
    # tree = Node(best_feature=best_feature, best_feature_label=best_feature_label, depth=0)
    sub_labels = labels[:best_feature] + labels[best_feature + 1:]
    
    # 如果最佳切分特征是离散型
    if type(data_set[0][best_feature]).__name__ == 'str':
        feature_values = [sample[best_feature] for sample in data_set]
        unique_values = set(feature_values)
        for value in unique_values:
            sub_data_set = split_data_set(data_set, best_feature, value)
            tree[best_feature_label][value] = create_tree(sub_data_set, sub_labels)
            # tree.children.append(create_tree(sub_data_set, sub_labels))
    
    # 如果最佳切分特征是连续型
    if type(data_set[0][best_feature]).__name__ == 'int' or type(
            data_set[0][best_feature]).__name__ == 'float':
        # 将数据集划分为两个子集，针对每个子集分别建树
        value = best_feature_value
        greater_sub_data_set = split_continuous_data_set(data_set, best_feature, value, 0)
        smaller_sub_data_set = split_continuous_data_set(data_set, best_feature, value, 1)
        # 针对连续型特征，在生成决策的模块，修改划分点的标签，如“> x.xxx”，"<= x.xxx"
        tree[best_feature_label]['>' + str(value)] = create_tree(greater_sub_data_set, sub_labels)
        tree[best_feature_label]['<=' + str(value)] = create_tree(smaller_sub_data_set, sub_labels)
    return tree

def predict(tree, X_test, labels):
    y = []
    for X in X_test:
        node = tree
        while True:
            # 如果该节点不是叶子节点
            if type(node) == dict:
                for parent, children in node.items():
                    children = list(children.items())
                    # 如果是连续型
                    if children[0][0][0] == '>':
                        value_split = float(children[0][0][1:])
                        if X[labels.index(parent)] > value_split:
                            node = children[0][1]
                        else:
                            node = children[1][1]
                    # 如果是离散型
                    else:
                        for child in children:
                            if X[labels.index(parent)] == child[0]:
                                node = child[1]
                                break
            # 如果该节点是叶子节点
            else:
                y.append(node)
                break
    return y