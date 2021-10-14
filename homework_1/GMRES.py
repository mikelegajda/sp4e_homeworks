#!/usr/bin/env python

import numpy as np
from scipy.sparse import csc_matrix
from scipy.sparse.linalg import gmres as gmres_benchmark


def gmres(A: np.ndarray, b: np.ndarray, max_iterations: int, threshold: float, callback=None):
    m, n = A.shape
    assert m == n, "A should be a square matrix"
    
    # give initial x
    x = np.random.rand(m,1)

    r = b - np.dot(A, x)

    return None


def arnoldi(A, Q, k):
    return None


if __name__ == "__main__":
    A_test = np.array([[8,1],[1,3]])
    b_test = np.array([2,4])
    # benchmark result form scipy
    x_test, exitCode = gmres_benchmark(A_test, b_test)
    print("Benchmark exitCode: ", exitCode)
    print("Benchmark result: ", x_test)

    x = gmres(A_test, b_test, 1000, 0.0001)
    print("Our result:", x)



