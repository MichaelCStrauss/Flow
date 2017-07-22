#include "FluidGrid.h"
#include <math.h>

using namespace Flow;
using namespace std;

Flow::FluidGrid::FluidGrid()
{
}

FluidGrid::FluidGrid(shared_ptr<ParticleVector> particles, int density, int sim_w, int sim_h)
{
	Particles = particles;
	Density = density;
	Width = sim_w * Density;
	Height = sim_h * Density;
	Frame = 0;

	Grid = vector<vector<int>>();
	Grid.reserve(Width * Height);
	Cache = vector<CellCache>();
	Cache.reserve(Width * Height);

	for (int i = 0; i < Width * Height; i++)
	{
		Grid.push_back(vector<int>());
		Cache.push_back(CellCache());
	}
}


FluidGrid::~FluidGrid()
{
}

FLOW_API void FluidGrid::Update()
{
	Frame++;
	//clear the old grid
	for (auto &cell : Grid)
	{
		cell = vector<int>();
		cell.reserve(CellReservation);
	}

	//sort the particles
	for (int i = 0; i < Particles->size(); i++)
	{
		auto p = (*Particles)[i];
		int x = fmaxf(fminf(p.Position.x * Density, Width - 1), 0); //make sure the index is bounded 
		int y = fmaxf(fminf(p.Position.y * Density, Height - 1), 0);
		Grid[y * Width + x].push_back(i);
	}
}

FLOW_API std::vector<int> FluidGrid::GetNeighbourIndices(float x, float y)
{
	x *= Density;
	y *= Density;
	auto index = Index(x, y);

	//Check if this has been cached
	if (Cache[index].FrameCalculated > Frame - CacheInvalidationTime && Frame != 1)
		return Cache[index].Indices;

	auto neighbours = vector<int>();

	//this could be tuned for better performance
	neighbours.reserve(CellReservation);

	int i, j;

	for (i = fmaxf(0, x - 1); i <= fminf(Width - 1, x + 1); i++)
	{
		for (j = fmaxf(0, y - 1); j <= fminf(Height - 1, y + 1); j++)
		{
			neighbours.insert(neighbours.end(), Grid[j * Width + i].begin(), Grid[j * Width + i].end());
		}
	}

	Cache[index].FrameCalculated = Frame;
	Cache[index].Indices = neighbours;

	return neighbours;
}

vector<int> FluidGrid::GetNeighbourIndices(Particle particle)
{
	return GetNeighbourIndices(particle.Position.x, particle.Position.y);
}

inline int FluidGrid::Index(int x, int y)
{
	//sanity check the index
	x = fmaxf(fminf(x, Width - 1), 0); //make sure the index is bounded 
	y = fmaxf(fminf(y, Height - 1), 0);
	return y * Width + x;
}
