#pragma once
#include "State.h"

class StateMenus : public GState
{
public:
	StateMenus();
	~StateMenus();

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);
};