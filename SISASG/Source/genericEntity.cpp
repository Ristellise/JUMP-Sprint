#include "genericEntity.h"
#include "Application.h"


genericEntity::genericEntity()
{
}


genericEntity::~genericEntity()
{
}

void genericEntity::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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
void genericEntity::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const Vector3& size)
{
    this->Init(pos, target, up);
    this->size = size;
}

void genericEntity::Reset()
{
    position.Set(1, 0, 0);
    target.Set(0, 0, 0);
    up.Set(0, 1, 0);
}

void genericEntity::Update(double dt)
{
    static const float ENTITY_SPEED = 20.f;
    position = position + view * (float)(velocity * dt);
    if (this->velocity > 0)
    {
        this->velocity -= 1.0f * dt;
    }
}