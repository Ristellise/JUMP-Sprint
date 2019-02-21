#ifndef GAME_H
#define GAME_H

#include "State.h"
#include "spaceship.h"
#include "PlanetRange.h"
#include "Hoops.h"
#include "AudioHandler.h"

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

    SoundContainer audiosrc;

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);

private:
	// test for hoops currently
	float x = 0.f, y = 0.f, z = 30.f, rad = 14.f; // main coords starts at venus, inner radius of ring is 9
	int points = 0; // counts the points (fairly bugged)
	float offset_x[5], offset_y[5], offset_z[5]; // stores positions of all rings (increase array size accordingly)

};

#endif