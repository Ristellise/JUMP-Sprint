#pragma once
#include "State.h"

class StateHangar : public GState
{
public:
	StateHangar();
	~StateHangar();

	void OnEnter();
	void OnExit();
	void OnUpdate(double dt);
	void OnRender();
	void OnCam(int X, int Y, float XChange, float YChange);
};