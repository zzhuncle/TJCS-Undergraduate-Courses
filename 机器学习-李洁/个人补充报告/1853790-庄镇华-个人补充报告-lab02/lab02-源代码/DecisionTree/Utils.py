import pickle
import os
import numpy as np

def load_CIFAR_batch(filename):
    with open(filename, 'rb') as f:
        datadict = pickle.load(f, encoding='latin1')
        X, y = datadict['data'], datadict['labels']
        y = np.array(y)
        return X, y

def load_CIFAR10(ROOT):
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




