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
        self.x = self.P * (j % self.kc) + (j / self.kr)

    def _iterator_inc(self):
        self.i += 1
        if (self.i == self.sc):
            self.q += self.M
            self.i = 0
            if (self.q == self.sr * self.M):
                self.q = 0
                self.x += self.M * self.P

    def _iterator_deref(self):
        return self.x + self.i + self.q


class Grid:
    def __init__(self):
        self.master = tk.Tk()

        self.squares = list()
        pw = 20
        self.cv = tk.Canvas(self.master, wid