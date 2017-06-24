#pragma once
#include "stdafx.h"
#include "Particle.h"
#include "FluidGrid.h"

using namespace std;

namespace Flow
{

	class FluidSystem
	{
	public:
		FLOW_API FluidSystem();
		FLOW_API ~FluidSystem();

		//Perform all the basic initialization
		FLOW_API void Init();

		FLOW_API void Update(float deltaTime);

		FLOW_API void InitParticles();

		shared_ptr<ParticleVector> Particles;

	private:
		FLOW_API void CalculateDensity();
		FLOW_API void CalculateForces();
		FLOW_API void ResolveCollisions();

		FLOW_API float DensityKernel(Vector2f r);
		FLOW_API Vector2f PressureKernel(Vector2f r);
		FLOW_API float ViscosityKernel(Vector2f r);

		FluidGrid Grid;

		int Num = 20;

		//Fluid Constants
		float TimeStep = 0.30f;
		float Gravity = -10;
		float ParticleMass = 0.025;
		float RestDensity = 1000;
		float k = 0.01;
		float mu = 0.008;
		float h = 0.05;
		float h2;

		int Frame = 0;

		float DensityKernelConst;
		float PressureKernelConst;
	};
}

