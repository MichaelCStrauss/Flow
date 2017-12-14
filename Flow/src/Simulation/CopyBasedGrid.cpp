#include <Simulation/CopyBasedGrid.h>
#include <math.h>

using namespace Flow;
using namespace std;

Flow::CopyBasedGrid::CopyBasedGrid()
{
}

CopyBasedGrid::CopyBasedGrid(shared_ptr<FluidSystem> system, int density)
{
	system_ = system;
	Density = density;
	Width = system_->Width * Density;
	Height = system_->Height * Density;
	Frame = 0;

	Grid = vector< vector<Vector2f> >();
	Grid.reserve(Width * Height);

	for (int i = 0; i < Width * Height; i++)
	{
		Grid.push_back(vector<Vector2f>());
	}
}


CopyBasedGrid::~CopyBasedGrid()
{
}

FLOW_API void CopyBasedGrid::Update()
{
	Frame++;
	//clear the old grid
	for (auto &cell : Grid)
	{
		cell.clear();
	}

	//sort the particles
	for (unsigned int i = 0; i < system_->getParticles()->size(); i++) {
		auto p = (*system_->getParticles())[i];
		int x = fmaxf(fminf(p.Position.x * (float)Density, Width - 1), 0); //make sure the index is bounded 
		int y = fmaxf(fminf(p.Position.y * (float)Density, Height - 1), 0);

		for (int i = fmaxf(0, x - 1); i <= fminf(Width - 1, x + 1); i++)
		{
			for (int j = fmaxf(0, y - 1); j <= fminf(Height - 1, y + 1); j++)
			{
				Grid[j * Width + i].push_back(p.Position);
			}
		}
	}
}

vector<Vector2f> CopyBasedGrid::getPositions(float x, float y)
{
	x *= Density;
	y *= Density;
	auto index = Index(x, y);
	return Grid[index];
}

inline int CopyBasedGrid::Index(int x, int y)
{
	//sanity check the index
	x = fmaxf(fminf(x, Width - 1), 0); //make sure the index is bounded 
	y = fmaxf(fminf(y, Height - 1), 0);
	return y * Width + x;
}

