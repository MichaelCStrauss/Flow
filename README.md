# Flow: A simple library for fluid simulation

This small library implements the Smoothed Particle Hydrodynamics model of fluid dynamics described by [Matthias Muller et al (2003)](http://matthias-mueller-fischer.ch/publications/sca03.pdf). Fluids are simulated in real-time, suitable for interactive applications such as video games. 

## Demo
A short video demonstrating the fluid, boundary conditions, and rendering using marching squares or the debug render (density):

### [High Quality on Streamable](https://streamable.com/daxli)

![demo gif](https://thumbs.gfycat.com/VapidNippyIsabellineshrike-size_restricted.gif)



## Introduction

Smoothed Particle Hydrodynamics is a Lagrangian method for simulating the movement and interactions of fluids. Unlike Eulerian grid-based approaches, SPH discretises a fluid into distinct particles.

SPH determines a quantity of the underlying field, such as density or pressure, by summing the contributions of all particles weighted by a smoothing function, $W(\mathbf{\overrightarrow{r}},h)$. Naively, to calculate the density of a particular particle would require summing against all other particles, an $\mathcal{O}(n^2)$ operation on all particles. This time complexity is prohibitive for real time applications using thousands of particles.

The smoothing function is only non zero when the particle is within the smoothing radius $h$ of other particles. This lends itself well to optimisation. I have integrated a cache efficient grid to ensure that only particles within $2h$ are considered.

Particles in SPH are discrete. In order to draw a contiguous surface, a mesh must be generated. I have implemented a marching squares algorithm, which produces a mesh by evaluating the density field of the fluid.

## Features
- Written in modern C++: use of RAII, smart pointers, platform independent threads and mutexes
- OpenGL renderer using geometry shader for mesh generation
- 2,500 particles at 200 FPS on a Core i7
