from numpy.ma.core import dot
from scipy.optimize import minimize
from scipy.sparse.linalg import lgmres
import numpy as np

tol = 1.e-10
#Defining the function to minimize
def min_S(x):
    x_t = np.transpose(x)
    A = np.array([[8.0, 1.0],[1.0, 3.0]])
    b = np.array([[2.0], [4.0]])
    x_tA = (np.dot(x_t, A))
    f = 0.5*(np.dot(x_tA, x)) - np.dot(x_t, b)
    return f

# applying minimization method
res = minimize(min_S, (0.,0.), tol=1.e-10)

print (res)

A = np.array([[8.0, 1.0],[1.0, 3.0]])
b = np.array([[2.0], [4.0]])

x, exitCode = lgmres(A, b, atol=tol)
print (x)


x = np.array([0.08695651, 1.30434782])
x_t = np.transpose(x)
x_tA = (np.dot(x_t, A))
f = 0.5*(np.dot(x_tA, x)) - np.dot(x_t, b)
print(f)