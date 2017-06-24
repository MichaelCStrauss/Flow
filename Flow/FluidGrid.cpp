#include "FluidGrid.h"
#include <math.h>

using namespace Flow;
using namespace std;

Flow::FluidGrid::FluidGrid()
{
}

FluidGrid::FluidGrid(shared_ptr<ParticleVector> particles, int x, int y)
{
	Particles = particles;
	Width = x;
	Height = y;

	Grid = vector<vector<int>>();
	Grid.reserve(Width * Height);

	for (int i = 0; i < Width * Height; i++)
	{
		Grid.push_back(vector<int>());
	}
}


FluidGrid::~FluidGrid()
{
}

FLOW_API void FluidGrid::Update()
{
	//clear the old grid
	for (auto &cell : Grid)
	{
		cell = vector<int>();
		cell.reserve(CellReservation);
	}
	for (int i = 0; i < Particles->size(); i++)
	{
		auto p = (*Particles)[i];
		int x = fmaxf(fminf(p.Position.x * Width, Width - 1), 0); //make sure the index is bounded 
		int y = fmaxf(fminf(p.Position.y * Width, Width - 1), 0);
		Grid[y * Height + x].push_back(i);
	}
}

FLOW_API std::vector<int> FluidGrid::GetNeighbourIndices(Particle particle)
{
	int x = particle.Position.x * Width;
	int y = particle.Position.y * Height;

	auto neighbours = vector<int>();

	//this could be tuned for better performance
	neighbours.reserve(CellReservation);

	int i, j;

	for (i = fmaxf(0, x - 1); i <= fminf(Width - 1, x + 1); i++)
	{
		for (j = fmaxf(0, y - 1); j <= fminf(Height - 1, y + 1); j++)
		{
			neighbours.insert(neighbours.end(), Grid[j * Height + i].begin(), Grid[j * Height + i].end());
		}
	}

	return neighbours;
}
