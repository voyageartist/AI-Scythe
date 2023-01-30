
# -*- coding: utf-8 -*-
# distutils: language=c++
# cython: boundscheck=False
# cython: wraparound=False
# cython: initializedcheck=True

import numpy as np
cimport numpy as cnp
cnp.import_array()

from libc.stdlib cimport *
from libc.stdio cimport *
from libc.string cimport *
from cython cimport view

from libcpp.limits cimport numeric_limits

from scythe.utils import *


cdef Dataset to_dataset(cnp.ndarray X):
    cdef Dataset dataset
    dataset.n_rows = X.shape[0]
    dataset.n_cols = X.shape[1]
    dataset.data = <void*>X.data
    dataset.dtype = np.dtype(X.dtype).num
    return dataset

cdef MDDataset to_md_dataset(cnp.ndarray X):
    cdef MDDataset dataset
    dataset.dtype = np.dtype(X.dtype).num
    dataset.data = <void*>X.data
    dataset.n_dims = X.ndim
    for i in range(7):
        dataset.dims[i] = X.shape[i] if i < dataset.n_dims else 0
    return dataset

cdef Labels to_labels(cnp.ndarray y):
    cdef Labels labels
    labels.n_rows = y.shape[0]
    labels.data = <target_t*>y.data
    return labels

cdef cnp.ndarray ptr_to_cls_predictions(float* predictions, size_t n_rows, size_t n_classes):
    cdef float[:, ::1] mview = <float[:n_rows, :n_classes:1]>predictions
    return np.asarray(mview)

cdef cnp.ndarray ptr_to_reg_predictions(cy_data_np* predictions, size_t n_rows):
    cdef cy_data_np[::1] mview = <cy_data_np[:n_rows:1]>predictions
    return np.asarray(mview)


cdef class TreeConfiguration:
    cdef TreeConfig config

    def __init__(self):
        self.config.task = CLASSIFICATION_TASK
        self.config.is_incremental = False
        self.config.min_threshold = 1e-06
        self.config.max_height = 200
        self.config.max_n_features = 9999
        self.config.n_classes = 2
        self.config.max_nodes = <size_t>1e+15
        self.config.partitioning = PERCENTILE_PARTITIONING
        self.config.nan_value = <data_t>np.nan
        self.config.is_complete_random = False
        self.config.ordered_queue = False
        self.config.class_weights = NULL

    cdef TreeConfig get_c_config(self):
        return self.config

    property task:
        def __get__(self): return self.config.task
        def __set__(self, value): self.config.task = value
    property is_incremental:
        def __get__(self): return self.config.is_incremental
        def __set__(self, value): self.config.is_incremental = value
    property min_threshold:
        def __get__(self): return self.config.min_threshold
        def __set__(self, value): self.config.min_threshold = value
    property max_height:
        def __get__(self): return self.config.max_height
        def __set__(self, value): self.config.max_height = value
    property max_n_features:
        def __get__(self): return self.config.max_n_features
        def __set__(self, value): self.config.max_n_features = value
    property n_classes:
        def __get__(self): return self.config.n_classes
        def __set__(self, value): self.config.n_classes = value
    property max_nodes:
        def __get__(self): return self.config.max_nodes
        def __set__(self, value): self.config.max_nodes = value
    property partitioning:
        def __get__(self): return self.config.partitioning
        def __set__(self, value): self.config.partitioning = value
    property nan_value:
        def __get__(self): return self.config.nan_value
        def __set__(self, value): self.config.nan_value = value
    property is_complete_random:
        def __get__(self): return self.config.is_complete_random
        def __set__(self, value): self.config.is_complete_random = value
    property ordered_queue:
        def __get__(self): return self.config.ordered_queue
        def __set__(self, value): self.config.ordered_queue = value
    property class_weights:
        def __get__(self):
            return np.asarray(<float[:self.config.n_classes:1]>self.config.class_weights)
        def __set__(self, cnp.ndarray value):
            assert(value.dtype == np.float)
            self.config.n_classes = len(value)
            self.config.class_weights = <float*>value.data


cdef class ForestConfiguration:
    cdef ForestConfig config

    def __init__(self):
        self.config.type = RANDOM_FOREST
        self.config.task = CLASSIFICATION_TASK
        self.config.n_classes = 2
        self.config.score_metric = MLOG_LOSS
        self.config.n_iter = 100
        self.config.max_n_trees = 100
        self.config.max_n_nodes = <size_t>1e+15
        self.config.max_n_features = 9999
        self.config.n_leaves = <size_t>1e+15
        self.config.n_jobs = 1
        self.config.n_samples_per_leaf = 1
        self.config.regularization = REG_L1
        self.config.bagging_fraction = 1.0
        self.config.early_stopping_round = 300
        self.config.boosting_method = GRADIENT_BOOSTING
        self.config.max_depth = <size_t>1e+15
        self.config.l1_lambda = 0.1
        self.config.l2_lambda = 0.1
        self.config.seed = 4.0
        self.config.verbose = True
        self.config.nan_value = numeric_limits[data_t].quiet_NaN()
        self.config.min_threshold = 1e-06
        self.config.ordered_queue = False
        self.config.partitioning = 100

    cdef ForestConfig get_c_config(self):
        return self.config
    cdef void set_c_config(self, ForestConfig fconfig):
        self.config = fconfig

    property type:
        def __get__(self): return self.config.type
        def __set__(self, value): self.config.type = value
    property task:
        def __get__(self): return self.config.task
        def __set__(self, value): self.config.task = value
    property n_classes:
        def __get__(self): return self.config.n_classes
        def __set__(self, value): self.config.n_classes = value
    property score_metric:
        def __get__(self): return self.config.score_metric
        def __set__(self, value): self.config.score_metric = value
    property max_n_trees:
        def __get__(self): return self.config.max_n_trees
        def __set__(self, value): self.config.max_n_trees = self.config.n_iter = value
    property max_n_nodes:
        def __get__(self): return self.config.max_n_nodes
        def __set__(self, value): self.config.max_n_nodes = value
    property max_n_features:
        def __get__(self): return self.config.max_n_features
        def __set__(self, value): self.config.max_n_features = value
    property n_leaves:
        def __get__(self): return self.config.n_leaves
        def __set__(self, value): self.config.n_leaves = value
    property n_jobs:
        def __get__(self): return self.config.n_jobs
        def __set__(self, value): self.config.n_jobs = value
    property n_samples_per_leaf:
        def __get__(self): return self.config.n_samples_per_leaf
        def __set__(self, value): self.config.n_samples_per_leaf = value
    property regularization:
        def __get__(self): return self.config.regularization
        def __set__(self, value): self.config.regularization = value
    property bagging_fraction:
        def __get__(self): return self.config.bagging_fraction
        def __set__(self, value): self.config.bagging_fraction = value
    property early_stopping_round:
        def __get__(self): return self.config.early_stopping_round
        def __set__(self, value): self.config.early_stopping_round = value
    property boosting_method:
        def __get__(self): return self.config.boosting_method
        def __set__(self, value): self.config.boosting_method = value
    property max_depth:
        def __get__(self): return self.config.max_depth
        def __set__(self, value): self.config.max_depth = value
    property l1_lambda:
        def __get__(self): return self.config.l1_lambda
        def __set__(self, value): self.config.l1_lambda = value
    property l2_lambda:
        def __get__(self): return self.config.l2_lambda
        def __set__(self, value): self.config.l2_lambda = value
    property seed:
        def __get__(self): return self.config.seed
        def __set__(self, value): self.config.seed = value
    property verbose:
        def __get__(self): return self.config.verbose
        def __set__(self, value): self.config.verbose = value
    property nan_value:
        def __get__(self): return self.config.nan_value
        def __set__(self, value): self.config.nan_value = value
    property min_threshold:
        def __get__(self): return self.config.min_threshold
        def __set__(self, value): self.config.min_threshold = value
    property ordered_queue:
        def __get__(self): return self.config.ordered_queue
        def __set__(self, value): self.config.ordered_queue = value
    property partitioning:
        def __get__(self): return self.config.partitioning
        def __set__(self, value): self.config.partitioning = value


cdef class Tree:
    cdef TreeConfig config
    cdef void* predictor_p
    cdef size_t n_features

    def __init__(self, TreeConfiguration cy_config, task):
        cy_config.task = TASKS[task]
        self.config = cy_config.get_c_config()
    def fit(self, X, y):
        cdef cnp.ndarray cX = np.ascontiguousarray(X, dtype = data_np)
        cdef cnp.ndarray cy = np.ascontiguousarray(y, dtype = target_np)
        cdef Dataset dataset = to_dataset(cX)
        cdef Labels labels = to_labels(cy)
        if self.config.task == REGRESSION_TASK:
            self.predictor_p = fit_regression_tree(
                &dataset, &labels, &self.config)
        else:
            self.predictor_p = fit_classification_tree(
                &dataset, &labels, &self.config)
        self.n_features = cX.shape[1]