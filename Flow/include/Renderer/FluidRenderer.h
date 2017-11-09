#pragma once
#include "stdafx.h"
#include "Simulation/Particle.h"
#include "Simulation/FluidSystem.h"

namespace Flow
{
	class FluidRenderer
	{
	public:
		//Initialise any resources needed by the renderer
		FluidRenderer(std::shared_ptr<FluidSystem> system);
		~FluidRenderer();

		FLOW_API void Draw();

	protected:
		std::shared_ptr<FluidSystem> system_;
	};
}
