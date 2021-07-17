import time
import pandas as pd
from sklearn.decomposition import PCA
from math import ceil 
 
from sklearn.tree import DecisionTreeClassifier
from ID3 import DecisionTreeID3
from C45 import create_tree, predict
from Utils import load_CIFAR10

if __name__ == "__main__":
    feature_num = 100
    X_train, y_train, X_test, y_test = load_CIFAR10('./data')
    pca = PCA(n_components=feature_num)
    X_train, X_test = pca.fit_transform(X_train), pca.fit_transform(X_test)
    dtc = DecisionTreeClassifier()
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