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

		FLOW_API float EvaluateDensity(Vector2f position);

		shared_ptr<ParticleVector> Particles;
		//start using a getter for the particles so that we can use mutexes in the future
		FLOW_API shared_ptr<ParticleVector> getParticles();

		FLOW_API vector<int> GetNearbyParticles(float x, float y);

		float Width, Height;
		int Frame = 0;

	private:
		FLOW_API void CalculateDensity();
		FLOW_API void CalculateForces();
		FLOW_API void ResolveCollisions();

		FLOW_API float DensityKernel(Vector2f r);
		FLOW_API Vector2f PressureKernel(Vector2f r);
		FLOW_API float ViscosityKernel(Vector2f r);

		FluidGrid Grid;

		int Num = 50;

		//timing information
		double gridTime_, densityTime_, forcesTime_, remainderTime_;


		//Fluid Constants
		float TimeStep = 0.30f;
		float Gravity = -10000;
		float ParticleMass = 0.018;
		float RestDensity = 1000;
		float k = 2.8;
		float mu = 4;
		float h = 0.035;
		float h2;

		float BoundaryDamping = 0.2;


		float DensityKernelConst;
		float PressureKernelConst;
	};
}

