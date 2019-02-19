#include "State.h"
#include "testCube.h"
#include "Bullet.h"

class Stateinit : public GState
{
public:
    Stateinit();
    ~Stateinit();
    std::string dtimestring;

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
	void RenderBullet();
    void OnCam(int X, int Y, float XChange, float YChange);
	Bullet* bullet;
    std::string Teststring = "";
};