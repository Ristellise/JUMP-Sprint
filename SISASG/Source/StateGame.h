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

private:
	// test for hoops currently
	int x = 250, y = 0, z = -250, rad = 9; // main coords starts at venus, inner radius of ring is 9
	int points = 0; // counts the points (fairly bugged)
	float offset_x[20], offset_y[20], offset_z[20]; // stores positions of all rings (increase array size accordingly)

};

#endif