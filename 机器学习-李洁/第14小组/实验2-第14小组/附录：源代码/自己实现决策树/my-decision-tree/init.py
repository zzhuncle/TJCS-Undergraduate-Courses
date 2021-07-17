# -*- coding: utf-8 -*-
"""
Created on Mon May 10 13:49:10 2021

@author: nieya
"""

# Things required to unpack the CIFAR-10 library
import os
# import h5py
import six
from six.moves import range, cPickle
import tarfile

# Main Library for Matrices manipulation
import numpy as np

# To draw the images
import matplotlib.pyplot as plt

import pickle

def pydump(obj, filename):
    with open(filename, 'wb') as f:
        pickle.dump(obj, f)

def pyload(filename):
    with open(filename, 'rb') as f:
        return pickle.load(f)


def cifar_10():
    # LOAD TRAINING DATA
    tar_file = tarfile.open("../data/cifar-10-python.tar.gz", 'r:gz')
    train_batches = []
    for batch in range(1, 6):
        file = tar_file.extractfile(
            'cifar-10-batches-py/data_batch_%d' % batch)
        try:
            if six.PY3:
                array = cPickle.load(file, encoding='latin1')
            else:
                array = cPickle.load(file)
            train_batches.append(array)
        finally:
            file.close()

    train_features = np.concatenate(
        [batch['data'].reshape(batch['data'].shape[0], 3, 32, 32)
            for batch in train_batches])
    train_labels = np.concatenate(
        [np.array(batch['labels'], dtype=np.uint8)
            for batch in train_batches])
    train_labels = np.expand_dims(train_labels, 1)


    # LOAD TEST DATA
    file = tar_file.extractfile('cifar-10-batches-py/test_batch')
    try:
        if six.PY3:
            test = cPickle.load(file, encoding='latin1')
        else:
            test = cPickle.load(file)
    finally:
        file.close()

    test_features = test['data'].reshape(test['data'].shape[0],
                                         3, 32, 32)
    test_labels = np.array(test['labels'], dtype=np.uint8)
    test_labels = np.expand_dims(test_labels, 1)

    return train_features, train_labels, test_features, test_labels

train_features, train_labels, test_features, test_labels = cifar_10()
# train100_labels, train100_labels, test100_features, test100_labels = load_cifar(100)

X = train_features.reshape(50000, 3*32*32)
Xt = test_features.reshape(10000, 3*32*32)
y = train_labels.flatten()
yt = test_labels.flatten()

# X100 = train100_features.reshape(50000, 3*32*32)
# Xt100 = test100_features.reshape(10000, 3*32*32)
# y100 = train100_labels.flatten()
# yt100 = test100_labels.flatten()

# Helps with writing functions!
msg = lambda x: print("%s ... " % x, end="")
done = lambda: print("done.")