#ifndef GAME_H
#define GAME_H

#include "State.h"
#include "spaceship.h"
#include "PlanetRange.h"
#include "Hoops.h"
#include "AudioHandler.h"

class StateGame : public GState
{
    struct STARS
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float scale = 0;
        int stime = 0;
    };

public:
    StateGame();
    ~StateGame();

    Mtx44 cubeMatrix;
    Mtx44 cubeMultR;
    Mtx44 cubeMult1;
    Mtx44 cubeMult2;
    Mtx44 cubeMult3;

    float rotateAngle;
    bool meshisnull;

    std::string dtimestring;
    std::string dgamestring;

    STARS coord;
    std::vector<STARS> stars;
    int starsnumber;
    void Stars();

    PlanetRange planetrange1;
    Hoops hoop;
    
    SoundContainer audiosrc;

	Bullet* bullet;

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);
	void hoopGenerate();
	void hoopChecker();
	void hoopRender();

private:
    // test for hoops currently
    float x = 20.f, y = 0.f, z = 180.f, rad = 14.f; // main coords starts at venus, inner radius of ring is 9
    int points = 0; // counts the points (fairly bugged)
    int totalHoops = 0;

    struct Hooplah
    {
        float offset_x = 400, offset_y = 0, offset_z = 1000, rotation = 0; // stores positions of all rings (increase array size accordingly)
        bool passed;
    };
    Hooplah ok;
    std::vector<Hooplah> hoopPos;
    
    double elapsedTime;
};

#endif