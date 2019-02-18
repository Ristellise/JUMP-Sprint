#include "entity.h"
#include "Application.h"
#include "Mtx44.h"

entity::entity()
{
	Reset();
	accl = 0.f;
}

entity::~entity()
{
}

void entity::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void entity::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void entity::Update(double dt)
{
	static const float ENTITY_SPEED = 20.f;
	if (Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
	{
		position.x -= (float)(ENTITY_SPEED * 0.2 * dt);
	}
	if (Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
	{
		position.x += (float)(ENTITY_SPEED * 0.2 * dt);
	}
	if (Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
	{
		position.y += (float)(ENTITY_SPEED * 0.2 * dt);
	}
	if (Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
	{
		position.y -= (float)(ENTITY_SPEED * 0.2 * dt);
	}
}

void entity::UpdateBBox()
{

    Vector3 hotFront = view * (float)this->Boxsize.BBForward;
    Vector3 hotRight = right * (float)this->Boxsize.BBSide;
    Vector3 hotTop = up * (float)this->Boxsize.BBTop;
    Vector3 pos = this->position;

    //get front positions
    this->HBox.frontLeftUp = pos + hotFront - hotRight - hotTop;
    this->HBox.frontLeftDown = pos + hotFront - hotRight + hotTop;
    this->HBox.frontRightUp = pos + hotFront + hotRight + hotTop;
    this->HBox.frontRightDown = pos + hotFront + hotRight - hotTop;

    this->HBox.backLeftUp = pos - hotFront - hotRight + hotTop;
    this->HBox.backLeftDown = pos - hotFront - hotRight - hotTop;
    this->HBox.backRightUp = pos - hotFront + hotRight + hotTop;
    this->HBox.backLeftDown = pos - hotFront + hotRight - hotTop;
}

// void entity::Update(double dt, float topSpeed, float fwdaccl, float bwdaccl, float & accl)
// {
// }
