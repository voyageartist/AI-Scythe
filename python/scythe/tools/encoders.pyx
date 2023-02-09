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


cdef inline cnp.int32_t fnv_32bits(unsigned char* element, Py_ssize_t nbytes):
    """
    Fowler/Noll/Vo hash algorithm that produces 32 bits keys

    Parameters
    ----------
    element: unsigned char*
        Pointer to the string to be hashed
    nbytes: Py_ssize_t
        Number of bytes contained in the string
    
    Return
    ------
    h: cnp.int32_t
        Output key / hashed value
    """
    cdef unsigned char* s = <unsigned char*>element
    cdef cnp.int32_t h = 0
    cdef Py_ssize_t i
    for i in range(nbytes):
        h += (h << 1) + (h << 4) + (h << 7) + (h << 8) + (h << 24)
        # or use this in place of previous line: h *= FNV_32_PRIME
        h ^= s[i]
    return h

cdef inline cnp.int64_t fnv_64bits(unsigned char* element, Py_ssize_t nbytes):
    """
    Fowler/Noll/Vo hash algorithm that produces 64 bits keys

    Parameters
    ----------
    element: unsigned char*
        Pointer to the string to be hashed
    nbytes: Py_ssize_t
        Number of bytes contained in the string
    
    Return
    ------
    h: cnp.int64_t
        Output key / hashed value
    """
    cdef unsigned char* s = <unsigned char*>element
    cdef cnp.int64_t h = 0
    cdef Py_ssize_t i
    for i in range(nbytes):
        h += (h << 1) + (h << 4) + (h << 5) + (h << 7) + (h << 8) + (h << 40)
        # or use this in place of previous line: h *= FNV_64_PRIME
        h ^= s[i]
    return h


cdef class HashEncoder:
