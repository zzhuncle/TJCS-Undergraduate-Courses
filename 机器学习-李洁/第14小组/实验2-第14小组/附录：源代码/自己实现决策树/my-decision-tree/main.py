# -*- coding: utf-8 -*-
"""
Created on Fri May  7 17:11:41 2021

@author Y.Nie/1851909/nie.yao.work@outlook.com 
"""

import time
import pandas as pd
from sklearn.decomposition import PCA
from math import ceil 
 
from sklearn.tree import DecisionTreeClassifier
from id3 import DecisionTreeID3
from c45 import create_tree, predict
from utils import load_CIFAR10


if __name__ == "__main__":
    # df = pd.read_csv('weather.csv')
    # X, y = df.iloc[:, :-1], df.iloc[:, -1]
    
    # X_train, y_train, X_test, y_test = load_CIFAR10('../data')
    # pca = PCA(n_components=10)
    # X_train, X_test = pca.fit_transform(X_train), pca.fit_transform(X_test)
    # X, y = pd.DataFrame(X_train).astype(int), pd.Series(y_train.tolist())
    # Xt, yt = pd.DataFrame(X_test).astype(int), pd.Series(y_test.tolist())
    # X_data = pd.concat([X, Xt])
    # for index, column in X_data.iteritems():
    #     left, right = min(column), max(column)
    #     delta = ceil((right - left) / 100)
    #     bins = [left + i*delta for i in range(101)]
    #     X_data[index] = pd.cut(column, bins)
    # X, Xt = X_data.iloc[0:50000, :], X_data.iloc[50000:60000, :]
    
    # tree = DecisionTreeID3(max_depth=100000, min_samples_split=1)
    # start = time.time()
    # tree.fit(X, y)
    # end = time.time()
    # print('Time: {} sec'.format(end - start))
    # y_pred = tree.predict(Xt)
    # print(y_pred)
    
    # feature_num = 10
    # X_train, y_train, X_test, y_test = load_CIFAR10('../data')
    # pca = PCA(n_components=feature_num)
    # X_train, X_test = pca.fit_transform(X_train), pca.fit_transform(X_test)
    # X_train, X_test = X_train.tolist(), X_test.tolist()
    # labels = ['feature' + str(i+1) for i in range(feature_num)]
    # start = time.time()
    # tree = create_tree(X_train, labels)
    # end = time.time()
    # print('Time: {} sec'.format(end - start))
    # y = predict(tree, data_set, labels)

    feature_num = 200
    X_train, y_train, X_test, y_test = load_CIFAR10('../data')
    pca = PCA(n_components=feature_num)
    X_train, X_test = pca.fit_transform(X_train), pca.fit_transform(X_test)
    dtc = DecisionTreeClassifier(
        # max_depth=None,
        # min_samples_split=5,
        # min_samples_leaf=2,
        # min_impurity_split=1e-7
        # 
        )
    start = time.time()
    dtc.fit(X_train, y_train)
    end = time.time()
    print('Time: {} sec'.format(end - start))
    y_pred = dtc.predict(X_train)
    rate = sum(y_pred == y_train) / len(y_pred)
    print('Train Rate: {} %'.format(rate*100))
    y_pred = dtc.predict(X_test)
    rate = sum(y_pred == y_test) / len(y_pred)
    print('Test Rate: {} %'.format(rate*100))