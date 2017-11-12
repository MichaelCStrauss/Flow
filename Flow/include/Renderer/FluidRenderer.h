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
		FLOW_API FluidRenderer(std::shared_ptr<FluidSystem> system);
		FLOW_API ~FluidRenderer();

		FLOW_API void Draw();

	protected:
		std::shared_ptr<FluidSystem> system_;
	};
}
