
# -*- coding: utf-8 -*-

import os, sys, subprocess
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import numpy as np
from numpy.distutils.misc_util import Configuration
from numpy.distutils.core import setup as np_setup
from numpy.distutils.numpy_distribution import NumpyDistribution

try:
    from Cython.Build import cythonize
    USE_CYTHON = True
except ImportError:
    USE_CYTHON = False
    

source_folder = "scythe"
source_files = [
    "core.pyx",
    "tools/encoders.pyx"
]

extra_compiler_args = [
    "-std=c++14",
    "-Iinclude",
    "-fPIC",
    "-Wpedantic",
    "-Wall",
    "-Wextra",
    "-Wconversion",
    "-Weffc++",
    "-Wstrict-null-sentinel",
    "-Wold-style-cast",
    "-Wnoexcept",
    "-Wctor-dtor-privacy",
    "-Woverloaded-virtual",
    "-Wsign-promo",
    "-Wzero-as-null-pointer-constant",
    "-Wconversion",
    "-pthread",
    "-O3",
    "-ftree-loop-optimize",
    "-ftree-vectorize",
    "-funroll-loops",
    "-ftree-vectorizer-verbose=1",
    "-g",
    "-fopenmp",
    "-fopenmp-simd"
]
extra_compiler_args += [
    "/O2",
    "/Ox",
    "/arch",
    "/GA",
    "/openmp",
]

def configuration(parent_package = str(), top_path = None):
    config = Configuration(None, parent_package, top_path)
    config.set_options(ignore_setup_xxx_py = True,
                       assume_default_configuration = True,
                       delegate_options_to_subpackages = True,
                       quiet = True)
    config.add_subpackage("scythe")

    return config

setup_args = {
    "name" : "Scythe",
    "version" : "1.0.0",
    "description" : "Deep learning library based on random forests",
    "long_description" : str(), # TODO
    "author" : "Antoine Passemiers",
    "classifiers" : [
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Operating System :: Microsoft :: Windows',
        'Programming Language :: C++',
    ],
    "configuration" : configuration
}

extensions = list()
for source_file in source_files:
    source_filepath = os.path.join(source_folder, source_file)
    sources = [source_filepath]
    extension_name = ".".join(["scythe", source_file])
    print(extension_name, sources)
    extensions.append(
        Extension(extension_name,
                  sources,
                  language = "c++",
                  extra_compiler_args = extra_compiler_args,
                  include_dirs = [np.get_include()]
        )
    )

GOT_BUILD_CMD = "install" in sys.argv or "build" in sys.argv
GOT_TEST_CMD = "test" in sys.argv

if USE_CYTHON and GOT_BUILD_CMD:
    # Setting "scythe" as the root package
    # This is to prevent cython from generating inappropriate variable names
    # (because it is based on a relative path)
    init_path = os.path.join(os.path.realpath(__file__), "../__init__.py")
    if os.path.isfile(init_path):
        os.remove(init_path)
        print("__init__.py file removed")
    # Generates the C files, but does not compile them
    extensions = cythonize(
        extensions,
        language = "c++"
    )

if GOT_BUILD_CMD:
    np_setup(**setup_args)

"""
if GOT_TEST_CMD:
    test_path = os.path.join(os.path.realpath(__file__), "../scythe/tests")
    for file in os.listdir(test_path):
        if file.endswith(".py"):
            subprocess.call(["nosetests", os.path.join(test_path, file)])
"""