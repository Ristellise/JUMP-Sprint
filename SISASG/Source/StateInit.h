#include "State.h"
#include "spaceship.h"

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
    void OnCam(int X, int Y, float XChange, float YChange);

    std::string Teststring = "";
};