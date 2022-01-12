# HOMEWORK 4: Eigen and Pybind

Authors: Mikele Gajda & Chenyang Wang

## Dependencies
1. Python v.3.8.3

## Quitck Start

1. clone the repository and run:
```
git pull --recurse-submodules
```
2. build
```
cd {PROJECT_FOLDER}/homework_4
mkdir build && cd build 
cmake .. -DUSE_PYTHON=ON
make
```
3. Run google test
```
cd build
./src/test_temperature
``` 
4. Test main.py
```
# generate input file
cd src
python3 generate_input.py 128 ../build/src/input.csv
python3 generate_materialpoint_input.py 256 ../build/src/m_input.csv

# run test
cd build/src
mkdir dumps
python3 main.py 1000 5 input.csv planet 0.01
or 
python3 main.py 1000 5 input.csv ping_pong 0.01
or
python3 main.py 1000 5 m_input.csv material_point 0.01
```

# Exercise Answer
1. Ex.3 Question 2:

    The createComputes callable or functor is designed to setup some simulation parameters, e.g., mass and conductivity, and customise the process of simulation by adding different compute class, e.g. ComputeGravity.

2. Ex.4 Question 2:

    In order to correctly manage references to Compute objects, I will create a function for each type of Compute object to return the smart shared pointer of it. Afterwards, it can be bound with python init function.
    
    E.g.,
    ```
    std::shared_ptr<ComputeTemperature> get_shared_ptr_computeTemperature(){
        return std::make_shared<ComputeTemperature>;
    }

    # for pybind
    m.def(py::init(&get_shared_ptr_computeTemperature))
    ```


# Troubleshooting
1. Install pybind
```
sudo apt install python3-pybind11
```