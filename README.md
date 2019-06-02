# Flow: A simple library for fluid simulation

This small library implements the Smoothed Particle Hydrodynamics model of fluid dynamics described by [Matthias Muller et al (2003)](http://matthias-mueller-fischer.ch/publications/sca03.pdf).

Computation of the fluid is performed in a single thread on the CPU, while rendering uses Marching Squares to generate mesh on the GPU. Evaluation of the field in order to render the fluid is performed using multiple CPU threads.

## Demo
A short video demonstrating the fluid, boundary conditions, and rendering using marching cubes or the debug render (density):

### [High Quality on Streamable](https://streamable.com/daxli)

![demo gif](https://thumbs.gfycat.com/VapidNippyIsabellineshrike-size_restricted.gif)

## Features
- Written in modern C++: use of RAII, smart pointers, platform independent threads and mutexes
- OpenGL renderer using geometry shader for mesh generation
- 2,500 particles at 200 FPS on a Core i7

## To do
- Implement the simulation on the GPU
- Add ability to interact with the fluid
- Static mesh collision