# -*- coding: utf-8 -*-
# distutils: language=c++
# cython: boundscheck=False
# cython: wraparound=False
# cython: initializedcheck=True

import numpy as np
cimport numpy as cnp
cnp.import_array()

from libc.stdlib cimport malloc, free
from libc.stdio cimport *
from libc.string cimport strcmp

from cpython.unicode cimport *


cdef cnp.int32_t FNV_32_PRIME = 16777619
cdef cnp.int64_t FNV_64_PRIME = 1099511628211


cdef inline cnp.int32