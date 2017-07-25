#include "FluidSystem.h"

using namespace Flow;
using namespace std;

FluidSystem::FluidSystem()
{
	Particles = make_shared<vector<Particle>>(vector<Particle>());
	Width = 2;
	Height = 1;
	Grid = FluidGrid(Particles, 1/h, Width, Height);
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
			p.Position = Vector2f(0.01 + (0.5 / Num) * (x + 0.5 * (y % 2)), Height - (0.7 / Num) * y);
			p.ID = x * Num + y;

			Particles->push_back(p);
		}
	}
}

float FluidSystem::EvaluateDensity(Vector2f pos)
{
	float density = 0;
	auto neighbours = Grid.GetNeighbourIndices(pos.x, pos.y);

	for (auto index : neighbours)
	{
		auto r = pos - (*Particles)[index].Position;
		density += (*Particles)[index].Mass * DensityKernel(r);
	}
	return density;
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
		if (neighbours.size() == 0)
		{
			std::cout << "no neighbours";
		}
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

		if (p.ID == 1500)
		{
			p.Density++;
		}

		for (auto index : neighbours)
		{
			auto r = p.Position - (*Particles)[index].Position;

			//begin by calculating the pressure force
			pressure += PressureKernel(r) * p.Mass * -(p.Pressure + (*Particles)[index].Pressure) / (2 * (*Particles)[index].Density);
			if (isnan(pressure.x) || isnan(pressure.y))
			{
				std::cout << "NaN Pressure";
				continue;
			}

			//now calculate the viscosity
			viscosity += ((*Particles)[index].Velocity - p.Velocity) * p.Mass  / ((*Particles)[index].Density) * ViscosityKernel(r) * mu;
		}

		p.Acceleration = (pressure + viscosity + Vector2f(0, Gravity)) / p.Density;

		//std::cout << p.Acceleration.x << " " << p.Acceleration.y << endl;
	}
}

void FluidSystem::ResolveCollisions()
{
	//Boundary Conditions
	for (auto &p : *Particles)
	{
		if (p.Position.x > Width)
		{
			p.Position.x = Width;
			p.Velocity.x *= -BoundaryDamping;
		}
		if (p.Position.x < 0)
		{
			p.Position.x = 0;
			p.Velocity.x *= -BoundaryDamping;
		}
		if (p.Position.y > Height)
		{
			p.Position.y = Height;
			p.Velocity.y *= -BoundaryDamping;
		}
		if (p.Position.y < 0)
		{
			p.Position.y = 0;
			p.Velocity.y *= -BoundaryDamping;
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
		return r * (1 / l) *  -PressureKernelConst * pow(h - l, 2);
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
