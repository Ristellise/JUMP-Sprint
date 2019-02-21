#pragma once
#include "State.h"

class StateShop : public GState
{
public:
	StateShop();
	~StateShop();

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);
};