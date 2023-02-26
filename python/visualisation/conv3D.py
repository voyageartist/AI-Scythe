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
       