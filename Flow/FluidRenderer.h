#pragma once
#include "stdafx.h"
#include "Particle.h"
#include "FluidSystem.h"
#include <fstream>

using namespace std;

namespace Flow
{
	class FluidRenderer
	{
	public:
		FLOW_API FluidRenderer();
		FLOW_API ~FluidRenderer();

		FLOW_API void Init(shared_ptr<FluidSystem> system);
		FLOW_API void Draw();
		FLOW_API void End();

	private:

		void InitShaders();
		void InitGeometry();
		
		void UpdateGeometry();

		Vector2f ScreenCoordsToSim(Vector2f screen);

		void AddVertices(int Configuration, float ScreenX, float ScreenY, int i, int j);
		inline void AddVert(float x, float y);
		inline void AddVert(Vector2f pos);

		shared_ptr<FluidSystem> System;
		
		vector<float> cells;
		vector<float> vertices;

		GLuint vbo, vao;

		//parameters
		int Resolution = 150;
		float DensityThreshold = 600;
		float CellW, CellH;
	};
}
