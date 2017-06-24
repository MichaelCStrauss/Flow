#pragma once
#include "stdafx.h"
#include "Particle.h"

using namespace std;

namespace Flow 
{
	class FluidGrid
	{
	public:
		FluidGrid();
		FluidGrid(shared_ptr<ParticleVector> particles, int x, int y);
		~FluidGrid();

		FLOW_API void Update();
		FLOW_API vector<int> GetNeighbourIndices(Particle particle);

		shared_ptr<ParticleVector> Particles;
		
	private:
		int Width, Height;

		//amount of ints to reserve for in each cell.
		int CellReservation = 100;

		vector<vector<int>> Grid;
	};
}
