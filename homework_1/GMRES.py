#!/usr/bin/env python

import numpy as np
from scipy.sparse.linalg import lgmres as gmres_benchmark
import matplotlib.pyplot as plt
from optimizer import S

#Global variables
TOL = 1.e-10

def vector_norm(v):
    return np.sqrt(np.einsum('k,k->', v, v))

def mat_vec_product(m, v):
    return np.einsum('ik,k->i', m, v)

def vector_dot_product(u, v):
    return np.einsum('i,i->', u, v)

def test_einsum():
    # vector norm
    m = np.array([[8,1],[1,3]])
    v = np.array([2,4])
    u = np.array([1,1])
    assert np.linalg.norm(v) == vector_norm(v)
    assert np.array_equal(np.dot(m, v), mat_vec_product(m, v))
    assert np.dot(v, u) == vector_dot_product(v, u)
    print("test eimsum: pass")

solution_history = [] # global variable to store the solution trajectory
def trajectory_save_callback(xk):
    """callback function to save the optimized vector for each iteration

    Args:
        xk ([type]): xk is the current parameter vector
    """
    solution_history.append(xk)

def gmres(A: np.ndarray, b: np.ndarray, max_iterations: int, threshold: float, callback=None):
    # Ref: https://en.wikipedia.org/wiki/Generalized_minimal_residual_method#Solving_the_least_squares_problem
    m, n = A.shape
    assert m == n, "A should be a square matrix"
    
    # give initial x
    # x = np.random.rand(m,1)
    x = np.zeros((m,))

    r = b - mat_vec_product(A, x)
    b_norm = vector_norm(b)
    error = vector_norm(r) / b_norm

    # initialize the 1D vectors
    sn = np.zeros((max_iterations, 1))
    cs = np.zeros((max_iterations, 1))
    e1 = np.zeros((max_iterations + 1, 1))
    e1[0] = 1
    error_list = [error]
    r_norm = vector_norm(r)

    # initialize Q
    Q = np.zeros((m, max_iterations + 1))
    Q[:, 0] = r / r_norm

    # initialize H
    H = np.zeros((max_iterations + 1, max_iterations))
    beta = r_norm * e1

    for k in range(max_iterations):
        # run arnoldi
        H[0:k+2, k], Q[:,k+1] = arnoldi(A, Q, k)

        # eliminate the last element in H ith row and update the rotation matrix
        H[0:k+2, k], cs[k], sn[k] = apply_given_rotation(H[0:k+2, k], cs, sn, k)

        # update the residual vector
        # TODO: use matrix operation?
        beta[k+1] = -sn[k] * beta[k]
        beta[k] = cs[k] * beta[k]
        error = np.abs(beta[k+ 1]) / b_norm

        # save the error
        error_list.append(error[0])

        # compute the current solution
        y_k,_,_,_ = np.linalg.lstsq(H[0:k+1, 0:k+1], beta[0:k+1], rcond=None)
        y_k = y_k.reshape(y_k.shape[0],)
        x_k = x + mat_vec_product(Q[:, 0:k+1], y_k)
        if callback is not None:
            callback(x_k)

        if error <= threshold:
            break

    # calculate the result
    y,_,_,_ = np.linalg.lstsq(H[0:k+1, 0:k+1], beta[0:k+1], rcond=None)
    y = y.reshape(y.shape[0],)
    x = x + mat_vec_product(Q[:, 0:k+1], y)

    return x, error_list


def arnoldi(A, Q, k):
    q = mat_vec_product(A, Q[:, k]) # Krylov Vector
    h = np.zeros((k+2, ))
    for i in range(k+1):
        h[i] = vector_dot_product(q, Q[:, i])
        q = q - h[i] * Q[:, i]
    h[k+1] = vector_norm(q)
    q = q / h[k+1]
    return h, q

def apply_given_rotation(h, cs, sn, k):
    # apply for the ith column
    # TODO: use matrix operation?
    for i in range(k):
        temp = cs[i] * h[i] + sn[i] * h[i+1]
        h[i+1] = -sn[i] * h[i] + cs[i] * h[i+1]
        h[i] = temp
    
    # update the next sin cos values for rotation
    cs_k, sn_k = update_rotation(h[k], h[k+1])

    # eliminate H(i + 1, i)
    h[k] = cs_k * h[k] + sn_k * h[k+1]
    h[k+1] = 0.0

    return h, cs_k, sn_k

def update_rotation(v1, v2):
    # Calculate the Given rotation matrix
    t = np.sqrt(v1*v1 + v2*v2)
    cs = v1 / t
    sn = v2 / t
    return cs, sn

def draw_single_plot(solution_history, title_1):
    """Draw the plots given the solution histories

    Args:
        bfgs_solution_history ([type]): a list of solution from BFGS
        gmres_solution_history ([type]): a list of solution vector from GMRES
    """
    # draw the surface and contours
    fig = plt.figure()
    X, Y = np.mgrid[-3:3:50j, -3:3:50j]
    X_Y = np.stack([X, Y], axis=0).reshape((2,-1)) # (2,-1)
    Z = S(X_Y).diagonal()
    Z = Z.reshape(X.shape)

    ax_1 = fig.add_subplot(111, projection="3d")

    ax_1.plot_surface(X, Y, Z, cmap="autumn_r", rstride=1, cstride=1, alpha=0.5)
    ax_1.contour(X, Y, Z, 10, colors="k", linestyles="solid")
    ax_1.set_title(title_1)
    # draw solution trajectory
    ax_1.plot([item[0] for item in solution_history],[item[1] for item in solution_history],[S(item)[0] for item in solution_history], 'bo', linewidth=1, ls ='--' )

    plt.show()
    

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--draw', action='store_true', 
                        help='set the flag to draw the plot')
    parser.add_argument('--method', type=str, required=True,
                    help='choose method between GMRES and ScipyLGMRES')
    parser.add_argument('--A', type=float, required=True, nargs='+',
                        help='input matrix A in a flaten vector format')
    parser.add_argument('--b', type=float, required=True, nargs='+',
                        help='input vector b')
    
    args = parser.parse_args()
    A_test = np.array(args.A, dtype=np.float32)
    dim = np.ceil(np.sqrt(A_test.shape[0])).astype(int)
    A_test = A_test.reshape((dim, dim))
    b_test = np.array(args.b, dtype=np.float32)
    draw = args.draw
    method = args.method

    print("Input A:", A_test)
    print("Input b:", b_test)

    # test einsum implementation of matrix operation
    # test_einsum()

    # A_test = np.array([[8,1],[1,3]])
    # b_test = np.array([2,4])
    # A_test = np.array([[3,2,-1],[2,-2,4],[-1,0.5,-1]])
    # b_test = np.array([1, -2, 0])
    # benchmark result form scipy
    initial_solution = np.array([0.0,0.0])
    solution_history.append(initial_solution)
    if method == "ScipyLGMRES":
        x_test, exitCode = gmres_benchmark(A_test, b_test, callback=trajectory_save_callback, atol=TOL)
        print("ScipyLGMRES exitCode: ", exitCode)
        print("ScipyLGMRES result: ", x_test)
    elif method == "GMRES":
        sol_gmres, err_list = gmres(A_test, b_test, 5000, TOL, callback=trajectory_save_callback)
        print("GMRES result: ", sol_gmres)
    else:
        print("Unknown method specified, please choose between ScipyLGMRES and GMRES")
        exit(-1)

    if draw:
        draw_single_plot(solution_history, method)