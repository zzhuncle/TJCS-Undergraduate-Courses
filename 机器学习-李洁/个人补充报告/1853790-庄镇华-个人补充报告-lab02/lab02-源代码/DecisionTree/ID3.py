import numpy as np
import pandas as pd

class TreeNode(object):
    def __init__(self, ids=None, children=[], entropy=0, depth=0):
        self.ids = ids               # index of data in this node
        self.entropy = entropy       # entropy, will fill later
        self.depth = depth           # distance to root node
        self.split_attribute = None  # which attribute is chosen, it non-leaf
        self.children = children     # list of its child nodes
        self.order = None            # order of values of split_attribute in children
        self.label = None            # label of node if it is a leaf

    def set_properties(self, split_attribute, order):
        self.split_attribute = split_attribute
        self.order = order

    def set_label(self, label):
        self.label = label

class DecisionTreeID3(object):
    def __init__(self, max_depth=10, min_samples_split=2, min_gain=1e-4):
        self.root = None
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.Ntrain = 0
        self.min_gain = min_gain

    def fit(self, data, target):
        self.Ntrain = data.count()[0]
        self.data = data
        self.attributes = list(data)
        self.target = target
        self.labels = target.unique()

        ids = range(self.Ntrain)
        self.root = TreeNode(ids=ids, entropy=self._entropy(ids), depth=0)
        queue = [self.root]
        while queue:
            node = queue.pop()
            if node.depth < self.max_depth:
                node.children = self._split(node)
                if not node.children:  # leaf node
                    self._set_label(node)
                queue += node.children
            else:
                self._set_label(node)

    def _entropy(self, ids):
        if len(ids) == 0:
            return 0
        freq = np.array(self.target[ids].value_counts())
        freq_0 = freq[freq.nonzero()[0]]  # remove prob 0
        prob_0 = freq_0 / float(freq_0.sum())
        ent = -np.sum(prob_0*np.log2(prob_0))
        return ent

    def _set_label(self, node):
        node.set_label(self.target[node.ids].mode()[0])  # most frequent label

    def _split(self, node):
        ids = node.ids
        best_gain = 0
        best_splits = []
        best_attribute = None
        order = None
        sub_data = self.data.iloc[ids, :]
        for i, attr in enumerate(self.attributes):
            values = sub_data[attr].unique().tolist()
            if len(values) == 1:
                continue  # entropy = 0
            splits = []
            for val in values:
                splits.append(sub_data.index[sub_data[attr] == val].tolist())
            # don't split if a node has too small number of points
            if min(map(len, splits)) < self.min_samples_split:
                continue
            # information gain
            HxS = 0
            for split in splits:
                HxS += len(split)/len(ids) * self._entropy(split)
            gain = node.entropy - HxS
            if gain < self.min_gain:
                continue  # stop if small gain
            if gain > best_gain:
                best_gain = gain
                best_splits = splits
                best_attribute = attr
                order = values
        node.set_properties(best_attribute, order)
        child_nodes = [TreeNode(ids=split, entropy=self._entropy(split), depth=node.depth+1)
                       for split in best_splits]
        return child_nodes

    def predict(self, new_data):
        npoints = new_data.count()[0]
        labels = [None] * npoints
        for n in range(npoints):
            x = new_data.iloc[n, :]  # one point
            # start from root and recursively travel if not meet a leaf
            node = self.root
            while node.children:
                node = node.children[node.order.index(x[node.split_attribute])]
            labels[n] = node.label

        return labels
