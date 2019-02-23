#include "State.h"
#include "spaceship.h"

class StateStat : public GState
{
public:
    StateStat();
    ~StateStat();
    std::string dtimestring;

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);

    std::string Teststring = "";
};