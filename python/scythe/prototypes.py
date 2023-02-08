
# -*- coding: utf-8 -*-

import numpy as np
import random
from copy import deepcopy

try:
	from queue import Queue
except:
	from Queue import Queue


LEFT  = 1
RIGHT = 2
NONE  = 3

class Node:
	def __init__(self, node_id):
		self.node_id      = node_id
		self.left_child   = None
		self.right_child  = None
		self.hyperplane   = None
		self.mu           = None
		self.threshold    = None
	def whichChild(self, x):
		if self.left_child is None or self.right_child is None:
			return NONE
		elif self.hyperplane is None:
			return LEFT if (np.linalg.norm(x - self.mu) <= self.threshold) else RIGHT
		else:
			return LEFT if (np.dot(x, self.hyperplane) <= self.threshold) else RIGHT


class APDTree:
	def __init__(self, n_iterations = 4, min_n_samples = 150, n_principal_components = 5):
		self.n_features = 0
		self.densities = None
		self.n_iterations = n_iterations
		self.discretization = 100
		self.min_n_samples = min_n_samples
		self.n_principal_components = n_principal_components
		self.root = None
		self.n_nodes = 0
		self.sample_mask = None

	@staticmethod
	def computeCost(samples, decoded):
		return np.mean(np.linalg.norm(samples - decoded, axis = 1))

	def computeSphericity(self, samples):
		mu = np.mean(samples, axis = 0)
		var = np.sum(np.linalg.norm(samples - mu, axis = 1))
		return np.sqrt(2.0 * var / len(samples))

	def APDSplittingRule(self, samples):
		p = (np.random.rand(samples.shape[1]) - 0.5) * 2.0
		for i in range(self.n_iterations):
			print(X.shape, p.shape, X.shape)
			print(np.dot(X, p).shape)
			print(np.dot(np.dot(X, p).T, X).shape)
			q = np.sum(np.dot(np.dot(X, p), X))
			p = q / np.linalg.norm(q)
			print(p.shape)
		return p

	def splitNode(self, X):
		if True: # TODO : if has outliers
			mu = np.mean(X, axis = 0)
			D  = np.linalg.norm(X - mu, axis = 1)
			is_left = (D <= np.median(D))
			return is_left, mu, None, np.median(D)
		else:
			p  = self.APDSplittingRule(X)
			print(X.shape, p.shape)
			P  = np.dot(X, p)
			is_left = (P <= np.median(P))
			return is_left, None, p, np.median(P)

	def fitPCA(self, samples):
		cov = np.cov(samples.T)
		eigenvalues, eigenvectors = np.linalg.eig(cov)

		# Sort eigenvalues in decreasing order
		indices = np.argsort(eigenvalues)[::-1]
		eigenvectors = eigenvectors[:, indices]
		eigenvectors = eigenvectors[:, :self.n_principal_components]

		"""
		encoded = np.dot(samples, eigenvectors)
		decoded = np.dot(encoded, eigenvectors.T)
		"""
		
		cost = self.computeSphericity(samples)
		return cost, eigenvectors

	def preprocessDensities(self, X):
		self.n_features = X.shape[1]
		self.densities = list()
		for f in range(self.n_features):
			bins = np.arange(self.discretization)
			self.densities.append(np.digitize(X[:, f], bins))