#ifndef GAME_H
#define GAME_H

#include "State.h"
#include "testCube.h"
#include "PlanetRange.h"
#include "Hoops.h"

class StateGame : public GState
{
public:
	StateGame();
	~StateGame();

	Mtx44 cubeMatrix;
	Mtx44 cubeMultR;
	Mtx44 cubeMult1;
	Mtx44 cubeMult2;
	Mtx44 cubeMult3;

	std::string dtimestring;

	PlanetRange planetrange1;
	Hoops hoop;

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);
};

#endif