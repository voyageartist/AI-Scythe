
# -*- coding: utf-8 -*-
# example.py : Scythe example of use
# author : Antoine Passemiers

from scythe.core import *

if __name__ == "__main__":

    # TREE CONFIGURATION
    config = TreeConfiguration()
    config.is_incremental = False
    config.min_threshold = 1e-06
    config.max_height = 50
    config.n_classes = 3
    config.max_nodes = 30
    config.partitioning = PERCENTILE_PARTITIONING
    config.nan_value = -1.0
    
    X_train = np.asarray(np.array([
        [0, 0, 0], # 0    1    5.6   6.65  0.5 0.0 0.5
        [0, 0, 1], # 0    0    7.8   7.8   1.0 0.0 0.0
        [1, 0, 0], # 1    1    4.2   4.2   0.0 1.0 0.0
        [2, 0, 0], # 1    1    3.5   3.5   0.0 1.0 0.0
        [2, 1, 0], # 2    1.5  9.8   7.9   0.0 0.5 0.5
        [2, 1, 1], # 0    0    5.4   5.4   1.0 0.0 0.0
        [1, 1, 1], # 1    1    2.1   2.1   0.0 1.0 0.0
        [0, 0, 0], # 2    1    7.7   6.65  0.5 0.0 0.5
        [0, 1, 0], # 2    2    8.8   8.8   0.0 0.0 1.0
        [2, 1, 0], # 1    1.5  6.0   7.9   0.0 0.5 0.5
        [0, 1, 1], # 1    1    5.7   5.7   0.0 1.0 0.0
        [1, 0, 1], # 1    1    7.0   7.0   0.0 1.0 0.0
        [1, 1, 0], # 1    1    6.9   6.9   0.0 1.0 0.0
        [2, 0, 1]  # 0    0    6.3   6.3   1.0 0.0 0.0
    ]), dtype = np.double)
    # X_train = np.random.rand(14, 3)

    y_train = np.array([0, 0, 1, 1, 2, 0, 1, 2, 2, 1, 1, 1, 1, 0])
    X_test = X_train

    # CLASSIFICATION TREE