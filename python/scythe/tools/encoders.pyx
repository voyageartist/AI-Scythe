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
    cdef cnp.int64_t seed
    cdef bint use_32bits_hashing

    def __init__(self, dtype = np.int32, seed = 0):
        """
        Fowler/Noll/Vo hash algorithm, that produces either 32 bits keys
        or 64 bits keys

        Parameters
        ----------
        dtype: np.dtype
            Data type of the output arrays (either np.int32 or np.int64)
        seed: int
            Initial value of the key passed to the hash functions

        References
        ----------
        1) http://www.isthe.com/chongo/src/fnv/hash_32.c
        2) http://www.isthe.com/chongo/src/fnv/hash_64.c
        """
        assert(dtype in [np.int32, np.int64])
        self.use_32bits_hashing = (dtype == np.int32)
        self.seed = int(round(seed))

    cdef cnp.ndarray encode_32bits(self, object[:] data_buf):
        """
        Vectorized Fowler/Noll/Vo hash algorithm, applied on a buffer

        Parameters
        ----------
        data_buf: object[:]
            Buffer containing the referen