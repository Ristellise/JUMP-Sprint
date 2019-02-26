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

void entity::InitSound(SoundContainer * Src, double *dt)
{
    this->SoundSrc = Src;
    this->SoundSrc->setDTptr(dt);
    this->SoundSrc->updatePos(&this->position);
}

void entity::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    this->position = pos;
    this->target = target;
    view = (target - position).Normalized();
    right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    this->up = right.Cross(view).Normalized();
    this->size = Vector3(1.0f, 1.0f, 1.0f);
    this->Boxsize = BBoxDimensions::toBBox(this->size);
}
void entity::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const Vector3& size)
{
    this->Init(pos, target, up);
    this->size = size;
}

void entity::Reset()
{
    position.Set(1, 0, 0);
    target.Set(0, 0, 0);
    up.Set(0, 1, 0);
}

void entity::OnHit(entity* Ent)
{
}

void entity::Update(double dt)
{
    position = position + view * (float)(velocity * dt);
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
    this->HBox.backRightDown = pos - hotFront + hotRight - hotTop;
}

BBoxDimensions::BBoxDimensions()
{
}

BBoxDimensions::~BBoxDimensions()
{
}

void BBoxDimensions::Set(float forward, float side, float top)
{
    this->BBForward = forward;
    this->BBSide = side;
    this->BBTop = top;
}

BBoxDimensions BBoxDimensions::toBBox(const Vector3 vect)
{
    return BBoxDimensions(vect.x, vect.y, vect.z);
}
