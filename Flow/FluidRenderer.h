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

		float EvaluateField(vector<int> &indices, float x, float y);

		Vector2f ScreenCoordsToSim(Vector2f screen);

		void AddVertices(int Configuration, float ScreenX, float ScreenY, int i, int j);
		inline void AddVert(float x, float y);
		inline void AddVert(Vector2f pos);

		shared_ptr<FluidSystem> System;
		
		vector<float> cells;
		vector<float> vertices;

		GLuint meshVBO, meshVAO, transVAO;
		GLuint basicProgram, transProgram;

		//parameters
		int Resolution = 150;
		int CellsX, CellsY;
		float ParticleRadius = 0.01; //this in sim terms
		float DensityThreshold = 600;
		float CellW, CellH;
	};
}
