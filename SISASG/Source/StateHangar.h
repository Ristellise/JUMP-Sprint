#pragma once
#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "spaceship.h"

#include "MouseHandler.h"
#include "Uniforms.h"
#include "StateManager.h"
#include <math.h>
#include "State.h"

class StateHangar : public GState
{
	enum GEOMETRY_TYPE
	{
		GEO_LIGHT,
		GEO_STAR,
		GEO_SIDE,

		// Ships
		GEO_SHIP1,
		GEO_SHIP2,
		NUM_GEOMETRY, // This should be at the end of the Enum, else nothing renders.
	};

	struct STARS
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float scale = 0;
		int stime = 0;
	};

public:
	GLFWwindow* l_window;
	std::vector<STARS> stars;
	std::vector<Light> lights;

	float rotateAngle;
	
	int NumberOfShips = 3;
	bool selectingShips = true;
	int NumberOfPlanets = 4;
	
	int starsnumber;
	float Shift = 0;
	bool lit = true;
	bool shiftmovement = false;
	int Dir = 0; // x Direction on UI
	float Delay = 0; // Frame delay

	STARS coord;

	StateHangar();
	~StateHangar();

	void RenderShips();
	void RenderUI();
	void Stars();

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);
};