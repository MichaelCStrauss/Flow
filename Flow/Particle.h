#pragma once

#include "stdafx.h"

namespace Flow
{
	struct Particle
	{
		Vector2f Position;
		Vector2f PreviousPosition;
		Vector2f Velocity;
		Vector2f Acceleration;

		float Density;
		float Pressure;
		float Mass;

		int ID;
	};

	typedef std::vector<Particle> ParticleVector;
}