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
		FLOW_API FluidGrid();
		FLOW_API FluidGrid(shared_ptr<ParticleVector> particles, int density, float sim_w, float sim_h);
		FLOW_API ~FluidGrid();

		FLOW_API void Update();
		FLOW_API vector<int> GetNeighbourIndices(Particle particle);
		FLOW_API vector<int> GetNeighbourIndices(float x, float y);

		shared_ptr<ParticleVector> Particles;
		
	private:

		FLOW_API inline int Index(int x, int y);

		int Density, Width, Height, Frame;

		//amount of ints to reserve for in each cell.
		int CellReservation = 40;

		//cache invalidation time in frames
		int CacheInvalidationTime = 1;

		vector< vector<int> > Grid;
		vector<CellCache> Cache;
	};
}
