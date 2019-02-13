#ifndef SCENE_HANGAR_H
#define SCENE_HANGAR_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "testCube.h"

#include "MouseHandler.h"
#include "Uniforms.h"
#include "StateManager.h"

class SceneHangar : public Scene
{

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_LIGHT,
		GEO_TEXT, // To move to vector solution

		// Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_FRONT,
		GEO_BACK,
		GEO_TOP,
		GEO_BOTTOM,
		// Test Cube
		GEO_TESTCUBE,
		GEO_SHIP1,
		NUM_GEOMETRY, // This should be at the end of the Enum, else nothing renders.
	};

public:
	SceneHangar();
	~SceneHangar();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
	void RenderSkybox();
	virtual void Exit();

private:
	GLFWwindow* l_window;
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	std::vector<Mesh*> meshListVector;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;
	MS modelStack, viewStack, projectionStack;
	Light lights[8];
	FontLoader FLInstance;

	std::string dtimestring = "";
	void RenderMesh(Mesh *mesh, bool enableLight);
	double lastkeypress = 0.0;
	unsigned int selector = 0;
	int NumberOfShips = 3;
	float Delay = 0;
	float Shift = 0;
	bool lit = true;
	bool shiftmovement = false;
	testCube testCube1;
};

#endif