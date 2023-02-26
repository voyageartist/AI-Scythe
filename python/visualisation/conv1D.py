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
        self.sc = self.M - self.kc + 1

    def _iterator_begin(self, j):
        self.i = 0
        self.x = j

    def _iterator_inc(self):
        self.i += 1
        if (self.i == self.sc):
            self.q += self.M
            self.i = 0
            if (self.q == self.sr * self.M):
                self.q = 0
                self.x += self.M * self.P

    def _iterator_deref(self):
        return self.x + self.i


class Grid:
    def __init__(self):
        self.master = tk.Tk()

        self.squares = list()
        pw, k = 20, 0
        self.cv = tk.Canvas(self.master, width = 850, height = 400)
        for i in range(10):
            for j in range(20):
                self.squares.append(
                    self.