
# -*- coding: utf-8 -*-

import os, sys
from distutils.extension import Extension
from distutils.command.build_ext import build_ext

import numpy as np
from numpy.distutils.misc_util import Configuration
from numpy.distutils.core import setup as np_setup


LANGUAGE = "c++"
CPP_SRC_FOLDER = "../../src"

cpp_src_files = [
    (
        "MNIST.py", [], "MNIST"
    ),
    (
        "utils.py", [], "utils"
    ),
    (
        "plot.py", [], "plot"
    ),
    (
        "tools/encoders.cpp", [], "encoders",
    ),
    (
        "core.cpp",
        [
            "densities/continuous.cpp",
            "forest/classification_rf.cpp",
            "forest/forest.cpp",
            "misc/bagging.cpp",
            "misc/sets.cpp",
            "tree/cart.cpp",
            "tree/heuristics.cpp",
            "tree/pruning.cpp",
            "scythe.cpp",
            "deep_learning/layers/layer.cpp",
            "deep_learning/layers/concatenation_layer.cpp",
            "deep_learning/layers/scanner2D.cpp",
            "deep_learning/deep_forest.cpp",
            "deep_scythe.cpp"
        ],
        "core"
    ),
]

source_files = list()
for module in cpp_src_files:
    cpp_src_filepaths = [module[0]]
    for filename in module[1]:
        cpp_src_filepaths.append(os.path.join(CPP_SRC_FOLDER, filename))
    source_files.append((cpp_src_filepaths, module[2]))


source_folder = "scythe"
sub_packages = []


COMPILER = "mingw"

if COMPILER == "mingw":
    extra_compile_args = [
        "-std=c++14", 
        "-ftree-loop-optimize",
        "-ftree-vectorize",
        "-funroll-loops",
        "-ftree-vectorizer-verbose=1",
        "-g",
        # "-fopenmp",
        # "-fopenmp-simd"
        "-Iinclude",
        "-O3"
    ]
elif COMPILER == "msvc":
    # https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx
    extra_compile_args = [
        "/O2",
        "/arch",
        "/GA",
        "/Wall"
    ]
extra_link_args = []


libraries = ["m"] if os.name == "posix" else list()
include_dirs = [np.get_include()]

config = Configuration(source_folder, "", "")
for sub_package in sub_packages:
    config.add_subpackage(sub_package)
for sources, extension_name in source_files:
    sources = [os.path.join(source_folder, source) for source in sources]
    print(extension_name, sources)
    config.add_extension(
        extension_name, 
        language = LANGUAGE,
        sources = sources,
        include_dirs = include_dirs + [os.curdir],
        libraries = libraries,
        extra_compile_args = extra_compile_args,
        extra_link_args = extra_link_args
    )

np_setup(**config.todict())