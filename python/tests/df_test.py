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

    fconfig = Fores