#pragma once
#include "stdafx.h"
#include "Particle.h"
#include "FluidSystem.h"
#include <fstream>

using namespace std;

namespace Flow
{
	class FluidRendererOld
	{
	public:
		FLOW_API FluidRendererOld();
		FLOW_API ~FluidRendererOld();

		FLOW_API void Init(shared_ptr<FluidSystem> system);
		FLOW_API void Draw();
		FLOW_API void End();

	private:

		void InitShaders();
		void InitBasicShader(); //the shader to render the mesh

		void InitTransformFeedback();
		void InitTransformShader(); //the shader to evaluate the field

		void InitGeometry();

		void UpdateGeometry();
		void CalculateGridValues();

		float EvaluateField(vector<int> &indices, float x, float y);

		Vector2f ScreenCoordsToSim(Vector2f screen);

		void AddVertices(int Configuration, float ScreenX, float ScreenY, int i, int j);
		inline void AddVert(float x, float y);
		inline void AddVert(Vector2f pos);

		shared_ptr<FluidSystem> System;
		
		vector<float> cells;
		vector<float> vertices;

		GLuint meshVBO, meshVAO, transVBO, transVAO, transTBO;
		GLuint basicProgram, transProgram;

		//parameters
		int Resolution = 175;
		int CellsX, CellsY;
		float ParticleRadius = 0.015; //this in sim terms
		float DensityThreshold = 200;
		float CellW, CellH;
	};
}
