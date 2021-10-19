from numpy.ma.core import dot
from scipy.optimize import minimize
from scipy.sparse.linalg import lgmres
import numpy as np
import matplotlib.pyplot as plt

tol = 1.e-10

#Defining the function to minimize
def min_S(x):
    x_t = np.transpose(x)
    A = np.array([[8.0, 1.0],[1.0, 3.0]])
    b = np.array([[2.0], [4.0]])
    x_tA = (np.dot(x_t, A))
    f = 0.5*(np.dot(x_tA, x)) - np.dot(x_t, b)
    return f

def create_plot():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    X, Y = np.mgrid[-3:3:50j, -3:3:50j]
    X_Y = np.stack([X, Y], axis=0).reshape((2,-1)) # (2,-1)
    Z = min_S(X_Y).diagonal()
    Z = Z.reshape(X.shape)
    ax.plot_surface(X, Y, Z, cmap="autumn_r", lw=0, rstride=1, cstride=1, alpha=0.5)
    ax.contour(X, Y, Z, 10, lw=3, colors="k", linestyles="solid")
    return ax

# create the plot
ax = create_plot()

# global variables for drawing
x_k_prev = np.array([0.0, 0.0])
y_k_prev = min_S(x_k_prev)[0]
def plot_callback(xk):
    """callback function to draw the optimized vector for each iteration

    Args:
        xk ([type]): xk is the current parameter vector
    """
    print(xk)
    global x_k_prev
    global y_k_prev
    ax.plot([x_k_prev[0],xk[0]],[x_k_prev[1],xk[1]],[y_k_prev, min_S(xk)[0]], 'bo', linewidth=1, ls ='--' )
    x_k_prev = xk
    y_k_prev = min_S(xk)[0]

# applying minimization method
res = minimize(min_S, (0.,0.), method="BFGS", tol=tol, callback=plot_callback)
print(res)
plt.show()


# global variables for drawing
x_k_prev = np.array([0.0, 0.0])
y_k_prev = min_S(x_k_prev)[0]
# recreate plot
ax = create_plot() # TODO: maybe should draw a two plot in one figure, and the global variable seems to mess up the second drawing
A = np.array([[8.0, 1.0],[1.0, 3.0]])
b = np.array([[2.0], [4.0]])
x, exitCode = lgmres(A, b,x0=x_k_prev,tol=tol, callback=plot_callback)
print(x)
plt.show()


x = np.array([0.08695651, 1.30434782])
x_t = np.transpose(x)
x_tA = (np.dot(x_t, A))
f = 0.5*(np.dot(x_tA, x)) - np.dot(x_t, b)
print(f)