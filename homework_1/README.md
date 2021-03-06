# Homework 1: Generalized minimal residual method

## Tested Dependencies
Python 3.8.3

### Packages
1. numpy==1.20.3
2. scipy==1.6.3
3. matplotlib==3.4.2

## Instructions to launch the program

To launch the program, open a terminal window in the file's location.

### Ex 1
To run the solution to the first part of the exercise type:
```
python3 optimizer.py 
```
This will output the optimal solution and the required plots.

### Ex 2
1. To run the GMRES of our implementation:
    ```
    python3 GMRES.py --A 8 1 1 3 --b 2 4 --method GMRES --draw
    ```

2. To run the LGMRES from Scipy:
    ```
    python3 GMRES.py --A 8 1 1 3 --b 2 4 --method ScipyLGMRES --draw
    ```


