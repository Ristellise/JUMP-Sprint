#include "State.h"

class Stateinit : public GState
{
public:
    Stateinit();
    ~Stateinit();
    
    void OnEnter();
    void OnExit();
    void OnUpdate();
    void OnRender();
};