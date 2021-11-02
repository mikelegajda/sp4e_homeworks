# Homework 1: Generalized minimal residual method

## Tested Dependencies
1. Python v.3.8.3
2. C++ v.12.0.5

### Python packages
1. matplotlib v.3.4.2

## Instructions to compile the program

To compile the program perform in order the following steps:
1. Move inside the main folder of the exercise with `ls homeworks_2`.
2. Create a `build` folder inside the main folder with `mkdir build`.
3. Move to the `build` folder and run compilation with `cmake .. && make` 

## Instruction to use the program
To run the program type `./main` followed by the additional parameters in the following order:
1. Input series type.  `arithmetic` or `pi` or `integral`.
2. Specify number of maximum iterations allowed.

3. For `arithmetic` or `pi` series, you can print to screen or save to file by choosing between: `printToScreen` or `saveToFile`. If you are calculating the integral just type `*`
4. For `arithmetic` or `pi` series, define the file type to be one of the following: `cvs`, `psv`, `txt`.  If you are calculating the integral just type `*`.
5. Only required if you are calculating the integral. You first specify the value of `a` and then the value of `b`.

Example 1: `./main pi 100 saveToFile csv` will calculate the pi series with maximum 100 iteration and will save the result in a csv file.

Example 2: `./main integral 100 * * 0 1`

## Answers and comments

### Ex 2.1
We decided to divide the work by separating the Series classes from the Dumper classes. We developed the two parts and then worked on the following tasks.

### Ex. 5
1. Denoting with $n$ the number of terms on the sum, the overall complexity of the first and simpler solution ends up to be $n^2$.
4. After storing the `current_value ` and `current_index` at each call of the compute function the sum does not need to be re-calculated. By doing that the complexity is reduced to $n$.
5. Even if we sum the terms in the inverted order the complexity is not changing. The number of computations remains the same, therefore the best complexity obtainable is still $n$.

### Ex. 6.4
Yes, we get the value expected.
1. For $\int_{0}^{1}x^3 dx$ we obtained the value expected after *number* iterations
2. For $\int_{0}^{\pi}cos(x) dx$ we obtained the value expected after *number* iterations
3. For $\int_{0}^{\pi/2}sin(x) dx$ we obtained the value expected after *number* iterations
