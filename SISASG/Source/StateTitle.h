#ifndef TITLE_H
#define TITLE_H

#include "State.h"

class StateTitle : public GState
{
public:
	StateTitle();
	~StateTitle();

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);
};

#endif