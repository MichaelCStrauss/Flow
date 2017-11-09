#pragma once
#include "Renderer\FluidRenderer.h"

namespace Flow
{
	class BasicRenderer :
		public FluidRenderer
	{
	public:
		BasicRenderer(std::shared_ptr<FluidSystem> system);
		~BasicRenderer();
	};
}

