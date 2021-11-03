# Homework 2: C++ Classes

Authors: Chenyang Wang & Mikele Gajda

## Tested Dependencies
1. Python v.3.8.3
2. matplotlib v.3.4.2

## Instructions to compile the program

To compile the program perform in order the following steps:
1. Move inside the main folder of the exercise with `ls homeworks_2`.
2. Create a `build` folder inside the main folder with `mkdir build`.
3. Move to the `build` folder and run compilation with `cmake .. && make` 

## Instructions to use the program
To run the program type `./main` followed by the additional parameters in the following order:
1. Input series type.  `arithmetic` or `pi` or `integral`.
2. Specify number of maximum iterations allowed.
3. For `arithmetic` or `pi` series, you can print to screen or save to file by choosing between: `printToScreen` or `saveAllToFile`. If you are calculating the integral, you have to specify `printOnlyResult` in order to visualize the final result.
4. For `arithmetic` or `pi` series, if you choose `saveAllToFile`, define the file type to be one of the following: `cvs`, `psv`, `txt`. If you choose `printToScreen` you can proceed.\
In case you are calculating the integral, type `_`, then choose one of the possible functions (`cubic`, `cos`, or `sin`). Afterwards, you just need to specify the value of `a` and the value of `b`, separated as usual by a blank space.

Example 1: `./main pi 100 saveAllToFile csv` will calculate the pi series with maximum 100 iteration and will save the result in a csv file.

Example 2: `./main integral 100 printOnlyResult _ cubic 0 1` will calculate the integral from 0 to 1 of the cubic function in 100 iteration and then will print the final result to the terminal.

## Instructions to plot the result

If you decided to save the result of each iteration to file by choosing `saveAllToFile`, you can call the python plotting program `PlotSeries.py` to visualize the convergence of the series compared to its analytical calculated value. To do so move to the `/src` folder and run the script as follows:\
`python3 PlotSeries ../build/results.csv`\
Where `../build/results.csv` is the file containing the results if you choose the `csv` format.

## Answers and comments

### Ex 2.1
We decided to divide the work by separating the Series classes from the Dumper classes. We decided to first design the header files of the respective parts so that each one of us could be able to work independently without worrying about the exact implementations of the other one's cc files.

### Ex. 5
1. Denoting with $n$ the number of terms on the sum, the overall complexity of the first and simpler solution ends up to be $n^2$.
4. After storing the `current_value ` and `current_index` at each call of the compute function the sum does not need to be re-calculated. By doing that the complexity is reduced to $n$.
5. Even if we sum the terms in the inverted order the complexity is not changing. The number of computations remains the same, therefore the best complexity obtainable is still $n$.

### Ex. 6.4
Yes, we get the value expected.
1. For $\int_{0}^{1}x^3 dx$ we obtained the value expected after *112* iterations
2. For $\int_{0}^{\pi}cos(x) dx$ we obtained the value expected after *630* iterations
3. For $\int_{0}^{\pi/2}sin(x) dx$ we obtained the value expected after *160* iterations
