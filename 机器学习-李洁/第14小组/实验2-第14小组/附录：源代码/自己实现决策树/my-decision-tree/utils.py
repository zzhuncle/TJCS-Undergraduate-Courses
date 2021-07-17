# -*- coding: utf-8 -*-
"""
Created on Sat May  8 15:17:08 2021

@author Y.Nie/1851909/nie.yao.work@outlook.com 
"""

import pickle
import os
import numpy as np


def load_CIFAR_batch(filename):
    """Load single batch of cifar.
    """
    with open(filename, 'rb') as f:
        datadict = pickle.load(f, encoding='latin1')
        X, y = datadict['data'], datadict['labels']
        # X = X.reshape(10000, 3, 32, 32).transpose(0, 2, 3, 1).astype("float")
        y = np.array(y)
        return X, y


def load_CIFAR10(ROOT):
    """Load all of cifar.
    """
    xs, ys = [], []

    # Train batch 1～5.
    for b in range(1, 6):
        f = os.path.join(ROOT, 'data_batch_%d' % (b, ))
        X, y = load_CIFAR_batch(f)
        xs.append(X)
        ys.append(y)
    X_train, y_train = np.concatenate(xs), np.concatenate(ys)

    # Test batch.
    X_test, y_test = load_CIFAR_batch(os.path.join(ROOT, 'test_batch'))

    return X_train, y_train, X_test, y_test




