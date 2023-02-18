# -*- coding: utf-8 -*-
# df_test.py: Tests on the deep forest
# author : Antoine Passemiers

import os, sys

import matplotlib.pyplot as plt

from scythe.core import *
from scythe.layers import *

def minimal_test():
    n_forests_per_layer = 2
    kc, kr = 2, 2

    fconfig = ForestConfiguration()
    # fconfig.bag_size       = 60000
    fconfig.n_classes      = 2
    fconfig.max_n_trees    = 2
    fconfig.max_n_features = 20
    fconfig.max_depth      = 20
    lconfig = LayerConfiguration(fconfig, n_forests_per_layer, COMPLETE_RANDOM_FOREST)

    print("Create gcForest")
    graph = DeepForest(task = "classification", n_classes = 2)
    scanner = MultiGrainedScanner2D(lconfig, (kc, kr))
    graph.add(scanner)
    cas