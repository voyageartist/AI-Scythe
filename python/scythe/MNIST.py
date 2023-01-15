# -*- coding: utf-8 -*-
# MNIST.py : Load the MNIST dataset
# author : Antoine Passemiers

import os, sys, struct
import numpy as np


def loadMNISTTrainingSet(location = "."):

    y_filepath = os.path.join(location, "train-labels-idx1-ubyte")
    with open(y_filepath, "rb") as y_file:
        _, _ = struct.unpack(">II", y_file.read(8))
        