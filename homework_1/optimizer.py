from typing import Callable
from numpy.ma.core import dot
from scipy.optimize import minimize
from scipy.sparse.linalg import lgmres
import numpy as np
import matplotlib.pyplot as plt

# some global variables to be used
TOL = 1.e-10
A = np.array([[8.0, 1.0],[1.0, 3.0]])
b = np.array([[2.0], [4.0]])

def S(x):
    """The function to minimize

    Args:
        x ([type]): [description]

    Returns:
        [float]: the result
    """
    x_t = np.transpose(x)
    x_tA = (np.dot(x_t, A))
    f = 0.5*(np.dot(x_tA, x)) - np.dot(x_t, b)
    return f

def draw_plot(bfgs_solution_history, gmres_solution_history):
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

    ax_1 = fig.add_subplot(121, projection="3d")
    ax_2 = fig.add_subplot(122, projection="3d")

    ax_1.plot_surface(X, Y, Z, cmap="autumn_r", lw=0, rstride=1, cstride=1, alpha=0.5)
    ax_1.contour(X, Y, Z, 10, lw=3, colors="k", linestyles="solid")
    ax_1.set_title("BFGS")

    ax_2.plot_surface(X, Y, Z, cmap="autumn_r", lw=0, rstride=1, cstride=1, alpha=0.5)
    ax_2.contour(X, Y, Z, 10, lw=3, colors="k", linestyles="solid")
    ax_2.set_title("GMRES")

    # draw solution trajectory
    ax_1.plot([item[0] for item in bfgs_solution_history],[item[1] for item in bfgs_solution_history],[S(item)[0] for item in bfgs_solution_history], 'bo', linewidth=1, ls ='--' )
    ax_2.plot([item[0] for item in gmres_solution_history],[item[1] for item in gmres_solution_history],[S(item)[0] for item in gmres_solution_history], 'bo', linewidth=1, ls ='--' )

    plt.show()
    

solution_history = [] # global variable to store the solution trajectory
def trajectory_save_callback(xk):
    """callback function to save the optimized vector for each iteration

    Args:
        xk ([type]): xk is the current parameter vector
    """
    solution_history.append(xk)

def minimizer(func: Callable):
    """A function which takes S(x) as a input argument in form of a python functor and returns the minimizer

    Args:
        func (Callable): The S(x) function

    Returns:
        two minimizers from the respective algorithm
    """
    ## routine: BFGS
    # applying minimization method
    initial_solution = np.array([0.0,0.0])
    solution_history.append(initial_solution)
    res = minimize(S, initial_solution, method="BFGS", tol=TOL, callback=trajectory_save_callback)
    solution_history_bfgs = solution_history.copy()
    solution_history.clear()
    sol_bfgs = res.x
    print("The result of BFGS is as follows: \n", sol_bfgs)


    ## routine: lgmres
    solution_history.append(initial_solution)
    sol_gmres, exitCode = lgmres(A, b, x0=initial_solution,tol=TOL, callback=trajectory_save_callback)
    solution_history_gmres = solution_history.copy()
    print("The result of GMRES is as follows: \n", sol_gmres)

    draw_plot(solution_history_bfgs, solution_history_gmres)
    return sol_bfgs, sol_gmres

if __name__ == "__main__":
    minimizer(S)
    print("Done")

