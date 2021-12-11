# Homework 3: External libraries: application to a heat equation solver

Authors: Chenyang Wang & Mikele Gajda

## Tested Dependencies
1. Python v.3.8.3

## Ex. 1
The additional classes available for the project are here described:

- **MaterialPoint:** This class derives form `Particle` class, as shown in `material_point.hh`, and adds properties for the heat rate and temperature;

- **Matrix:** It is a class template added to provide manipulation functions for 2D data; 

- **MaterialPointFactory:** It is factory class derived using `particles_factory_interface.hh` that provides a simulation of a system composed by material points.

- **FFT:** It is a class needed to compute the 2D discrete Fourier Transformation. 

