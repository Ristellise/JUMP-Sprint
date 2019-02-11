#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    this->pos = defaultPosition = pos;
    this->target = defaultTarget = target;
    Vector3 view = (target - pos).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt)
{
    Vector3 view = (target - pos).Normalized();
    Vector3 right = view.Cross(up) * 0.5f;

    static const float CAMERA_SPEED = 100.f;

    if (
        (pos.x >= -100) &&
        (pos.x <= 100) &&
        (pos.z >= -100) &&
        (pos.z <= 100) &&
        (pos.y >= -100) &&
        (pos.y <= 100)
        )
    {
        if (Application::IsKeyPressed('A'))
        {
            pos = pos - right;
            target = pos + view;
        }
        if (Application::IsKeyPressed('D'))
        {
            pos = pos + right;
            target = pos + view;
        }
        if (Application::IsKeyPressed('W'))
        {
            Vector3 view = (target - pos).Normalized();
            pos = pos + view;
            target = target + view;
        }
        if (Application::IsKeyPressed('S'))
        {
            Vector3 view = (target - pos).Normalized();
            pos -= view * (float)(40.f * dt);
            target = target + view;
        }

        if (Application::IsKeyPressed(VK_LEFT))
        {
            float yaw = (float)(CAMERA_SPEED * dt);
            Mtx44 rotation;
            rotation.SetToRotation(yaw, 0, 1, 0);
            view = rotation * view;
            target = pos + view;
            up = rotation * up;
        }

        if (Application::IsKeyPressed(VK_RIGHT))
        {
            float yaw = (float)(-CAMERA_SPEED * dt);
            Mtx44 rotation;
            rotation.SetToRotation(yaw, 0, 1, 0);
            view = rotation * view;
            target = pos + view;
            up = rotation * up;
        }

        if (Application::IsKeyPressed(VK_UP))
        {
            float pitch = (float)(CAMERA_SPEED * dt);
            Vector3 view = (target - pos).Normalized();
            Vector3 right = view.Cross(up);
            right.y = 0;
            right.Normalize();
            up = right.Cross(view).Normalized();
            Mtx44 rotation;
            rotation.SetToRotation(pitch, right.x, right.y, right.z);
            view = rotation * view;
            target = pos + view;
            up = rotation * up;
        }

        if (Application::IsKeyPressed(VK_DOWN))
        {
            float pitch = (float)(-CAMERA_SPEED * dt);
            Vector3 view = (target - pos).Normalized();
            Vector3 right = view.Cross(up);
            right.y = 0;
            right.Normalize();
            up = right.Cross(view).Normalized();
            Mtx44 rotation;
            rotation.SetToRotation(pitch, right.x, right.y, right.z);
            view = rotation * view;
            target = pos + view;
            up = rotation * up;
        }

        if (Application::IsKeyPressed('Q'))
        {
            pos = pos + up;
            target = pos + view;
        }

        if (Application::IsKeyPressed('E'))
        {
            pos = pos - up;
            target = pos + view;
        }
    }

    if (
        (pos.x < -100) ||
        (pos.x > 100) ||
        (pos.y < -100) ||
        (pos.y > 100) ||
        (pos.z < -100) ||
        (pos.z > 100)
        )
    {
        Reset();
    }

    if (Application::IsKeyPressed('R'))
    {
        Reset();
    }
}

void Camera3::Reset()
{
    pos = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}