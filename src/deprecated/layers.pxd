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
