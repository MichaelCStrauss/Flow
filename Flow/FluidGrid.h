#pragma once
#include "stdafx.h"
#include "Particle.h"

using namespace std;

namespace Flow 
{
	struct CellCache
	{
		vector<int> Indices;
		int FrameCalculated;
	};

	class FluidGrid
	{
	public:
		FluidGrid();
		FluidGrid(shared_ptr<ParticleVector> particles, int density, int sim_w, int sim_h);
		~FluidGrid();

		FLOW_API void Update();
		FLOW_API vector<int> GetNeighbourIndices(Particle particle);

		shared_ptr<ParticleVector> Particles;
		
	private:

		FLOW_API inline int Index(int x, int y);

		int Density, Width, Height, Frame;

		//amount of ints to reserve for in each cell.
		int CellReservation = 40;

		//cache invalidation time in frames
		int CacheInvalidationTime = 1;

		vector<vector<int>> Grid;
		vector<CellCache> Cache;
	};
}
