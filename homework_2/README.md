# Homework 1: Generalized minimal residual method

## Tested Dependencies
1. Python 3.8.3
2. C++ 12.0.5

### Python packages
1. matplotlib==3.4.2

## Instructions to compile the program

To compile the program perform in order the following steps:
1. Move inside the main folder of the exercize with `ls homeworks_`.
2. Create a `build` folder inside the main folder with `mkdir build`.
3. Move to the `build` folder and run compilation with `cmake .. && make` 

## Instruction to use the program
1. To run the program type `./main` followed by the additional parameters as expressed below
[...]

## Answers and comments

### Ex 2.1
We decided to divide the work by separating the Series classes from the Dumper classes. We developed the two parts and then worked on the following tasks.

### Ex. 5
1. Denoting with $n$ the number of terms on the sum, the overall complexity of the first and simpler solution ends up to be $n^2$.