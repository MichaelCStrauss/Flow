# Flow: A simple library for fluid simulation

This small library implements the Smoothed Particle Hydrodynamics model of fluid dynamics described by [Matthias Muller et al (2003)](http://matthias-mueller-fischer.ch/publications/sca03.pdf).

Computation of the fluid is performed in a single thread on the CPU, while rendering uses Marching Squares to generate mesh on the GPU. Evaluation of the field in order to render the fluid is performed using multiple CPU threads.

## Demo:
A short video demonstrating the fluid, boundary conditions, and rendering using marching cubes or the debug render (density):

### [High Quality on Streamable](https://streamable.com/daxli)

![demo gif](https://thumbs.gfycat.com/VapidNippyIsabellineshrike-size_restricted.gif)

## Features
