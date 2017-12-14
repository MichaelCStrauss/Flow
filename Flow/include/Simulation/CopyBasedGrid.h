#pragma once
#include "stdafx.h"
#include "FluidSystem.h"

using namespace std;

namespace Flow 
{
	class CopyBasedGrid
	{
	public:
		FLOW_API CopyBasedGrid();
		FLOW_API CopyBasedGrid(shared_ptr<FluidSystem> system, int density);
		FLOW_API ~CopyBasedGrid();

		FLOW_API void Update();

		FLOW_API vector<Vector2f> getPositions(float x, float y);

		shared_ptr<FluidSystem> system_;
		
	private:

		FLOW_API inline int Index(int x, int y);

		int Density, Width, Height, Frame;

		//amount of ints to reserve for in each cell.
		int CellReservation = 40;

		//cache invalidation time in frames
		vector< vector<Vector2f> > Grid;
	};
}
