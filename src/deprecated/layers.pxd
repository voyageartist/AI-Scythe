# -*- coding: utf-8 -*-
# distutils: language=c++

import numpy as np
cimport numpy as cnp
cnp.import_array()

from scythe.core cimport *
from scythe.core import *


cdef extern from "../../src/deep_learning/layers/layer.hpp" namespace "scythe":
    struct LayerConfig:
        ForestConfig fconfig
        size_t       n_forests
        int          forest_type

cdef extern from "../../src/deep_scythe.hpp":
    size_t c_create_deep_forest(int task)
    void c_fit_deep_forest(MDDataset dataset, Labels* labels, size_t for