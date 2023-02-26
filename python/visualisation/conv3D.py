import random

try:
    import tkinter as tk
except ImportError:
    import Tkinter as tk


class Iterator:
    def __init__(self, N, M, P, O, kc, kr, kp):
        self.N = N
        self.M = M
        self.P = P
        self.O = O
        self.kc = kc
        self.kr = kr
        self.kp = kp
        self.sc = self.P - self.kc + 1
        self.sr = self.M - self.kr + 1
        self.sp = self.O - self.kp + 1

    def _iterator_begin(self, j):
        self.i = 0
        self.q = 0
        self.x = self.P * (j