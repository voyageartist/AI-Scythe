import random

try:
    import tkinter as tk
except ImportError:
    import Tkinter as tk


class Iterator:
    def __init__(self, N, M, kc):
        self.N = N
        self.M = M
        self.kc = kc
        self.sc = self.M 