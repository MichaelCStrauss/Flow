#include "FluidSystem.h"

using namespace Flow;
using namespace std;

FluidSystem::FluidSystem()
{
	Particles = make_shared<vector<Particle>>(vector<Particle>());
	int n = 1 / h;
	Grid = FluidGrid(Particles, n, n);
	h2 = h * h;

	DensityKernelConst = 315.f / (64 * 3.1416f * pow(h, 9));
	PressureKernelConst = 45.f / (3.1416f * pow(h, 6));
}


FluidSystem::~FluidSystem()
{
}

void FluidSystem::Init()
{
	InitParticles();
}

void FluidSystem::InitParticles()
{
	Particles->reserve(Num * Num);

	for (int x = 0; x < Num; x++)
	{
		for (int y = 0; y < Num; y++)
		{
			auto p = Particle();
			p.Mass = ParticleMass;
			p.Position = Vector2f(0.1 + (0.5 / Num) * x, 0.9 - (0.5 / Num) * y);

			Particles->push_back(p);
		}
	}
}

void FluidSystem::Update(float deltaTime)
{
	Grid.Update();

	CalculateDensity();
	CalculateForces();

	//integrate
	for (auto &p : *Particles)
	{
		p.Velocity += p.Acceleration * deltaTime * TimeStep;
		p.Position += p.Velocity * deltaTime * TimeStep;
	}

	ResolveCollisions();

	Frame++;
}


void FluidSystem::CalculateDensity()
{
	for (auto &p : *Particles)
	{
		auto neighbours = Grid.GetNeighbourIndices(p);
		p.Density = 0;

		for (auto index : neighbours)
		{
			auto r = p.Position - (*Particles)[index].Position;
			p.Density += p.Mass * DensityKernel(r);
		}
		//std::cout << p.Density << std::endl;
		p.Pressure = k * (p.Density - RestDensity);
	}
}

void FluidSystem::CalculateForces()
{
	for (auto &p : *Particles)
	{
		auto neighbours = Grid.GetNeighbourIndices(p);

		auto pressure = Vector2f();
		auto viscosity = Vector2f();

		for (auto index : neighbours)
		{
			auto r = p.Position - (*Particles)[index].Position;

			//begin by calculating the pressure force
			pressure += PressureKernel(r) * p.Mass * -(p.Pressure + (*Particles)[index].Pressure) / (2 * (*Particles)[index].Density);
			if (isnan(pressure.x) || isnan(pressure.y))
			{
				std::cout << "oh no";
				return;
			}

			//now use the viscosity
			viscosity += ((*Particles)[index].Velocity - p.Velocity) * p.Mass  / ((*Particles)[index].Density) * ViscosityKernel(r) * mu;
		}

		p.Acceleration = pressure + viscosity + Vector2f(0, Gravity);

		//std::cout << p.Acceleration.x << " " << p.Acceleration.y << endl;
	}
}

void FluidSystem::ResolveCollisions()
{
	//Boundary Conditions
	for (auto &p : *Particles)
	{
		if (p.Position.x > 1)
		{
			p.Position.x = 1;
			p.Velocity.x *= -0.5;
		}
		if (p.Position.x < 0)
		{
			p.Position.x = 0;
			p.Velocity.x *= -0.5;
		}
		if (p.Position.y > 1)
		{
			p.Position.y = 1;
			p.Velocity.y *= -0.5;
		}
		if (p.Position.y < 0)
		{
			p.Position.y = 0;
			p.Velocity.y *= -0.5;
		}
	}
}

float FluidSystem::DensityKernel(Vector2f r)
{
	auto r2 = r.lengthSq();
	
	if (r2 > h2)
		return 0;
	else
		return DensityKernelConst * pow(h2 - r2, 3);
}

Vector2f FluidSystem::PressureKernel(Vector2f r)
{
	auto r2 = r.lengthSq();
	
	if (r2 > h2)
		return Vector2f();
	else if (r2 == 0)
		return Vector2f();
	else
	{
		auto l = r.length();
		auto ret =  r * (1 / l) *  -PressureKernelConst * pow(h - l, 2);
		return ret;
	}
}

float FluidSystem::ViscosityKernel(Vector2f r)
{
	auto r2 = r.lengthSq();
	
	if (r2 > h2)
		return 0;
	else if (r2 == 0)
		return 0;
	else
		return PressureKernelConst * (h - r.length());
}
