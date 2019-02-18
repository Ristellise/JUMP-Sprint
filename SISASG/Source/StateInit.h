#include "State.h"
#include "testCube.h"

class Stateinit : public GState
{
public:
    Stateinit();
    ~Stateinit();
    std::string dtimestring;

	Mtx44 cubeMatrix;
	Mtx44 cubeMultR;
	Mtx44 cubeMult1;
	Mtx44 cubeMult2;
	Mtx44 cubeMult3;

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);

    std::string Teststring = "";
};