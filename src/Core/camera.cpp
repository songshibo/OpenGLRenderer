#include "camera.h"

FpsCamera::FpsCamera(float3 cameraPos, float3 cameraFront, float3 upVector, float cameraFov)
{
    position = cameraPos;
    worldUp = upVector;
    yaw = YAW;
    pitch = PITCH;
    front = cameraFront;
    fov = cameraFov;
    zNear = ZNEAR;
    zFar = ZFAR;
    DEBUG_INFO("FPS Camera initialized.");
    updateCameraVector();
}

FpsCamera::~FpsCamera()
{
}

float4x4 FpsCamera::perspective(float aspect)
{
    float4x4 projection = float4x4::Zero();
    if (aspect <= 0)
    {
        DEBUG_ERROR("Camera aspect <= 0!");
        return projection;
    }
    if (zFar <= zNear)
    {
        DEBUG_ERROR("Camera far clip plane <= near clip plane!");
        return projection;
    }

    float theta = deg2rad(fov * 0.5f);
    float range = zFar - zNear;
    float invtan = 1.0f / tan(theta);
    projection(0, 0) = invtan / aspect;
    projection(1, 1) = invtan;
    projection(2, 2) = -(zFar + zNear) / range;
    projection(3, 2) = -1;
    projection(2, 3) = -2 * zNear * zFar / range;
    return projection;
}

float4x4 FpsCamera::view()
{
    // eye position + front -> look point
    float3 f = front.normalized();
    float3 u = up.normalized();
    float3 s = f.cross(u).normalized();
    u = s.cross(f);

    float4x4 view = float4x4::Zero();
    view << s.x(), s.y(), s.z(), -s.dot(position),
        u.x(), u.y(), u.z(), -u.dot(position),
        -f.x(), -f.y(), -f.z(), f.dot(position),
        0, 0, 0, 1;
    return view;
}

void FpsCamera::ProcessKeyboard(FPS_Movement direction, const float &deltaTime)
{
    float velocity = SPEED * deltaTime;
    switch (direction)
    {
    case FORWARD:
        position += front * velocity;
        break;
    case BACKWARD:
        position -= front * velocity;
        break;
    case RIGHT:
        position -= right * velocity;
        break;
    case LEFT:
        position += right * velocity;
        break;
    default:
        DEBUG_ERROR("Invalid keyboard input for FPS camera");
        break;
    }
}

void FpsCamera::ProcessMouseInput(const float &xoffset, const float &yoffset, bool constranPitch)
{
    yaw += xoffset * SENSITIVITY;
    pitch += yoffset * SENSITIVITY;
    if (constranPitch)
    {
        pitch = pitch > 89.9f ? 89.9f : pitch;
        pitch = pitch < -89.9f ? -89.9f : pitch;
    }
    updateCameraVector();
}

void FpsCamera::updateCameraVector()
{
    float3 f;
    float rYaw = deg2rad(yaw);
    float rPitch = deg2rad(pitch);

    f << cos(rYaw) * cos(rPitch), sin(rPitch), sin(rYaw) * cos(rPitch);
    front = f.normalized();
    right = front.cross(worldUp).normalized();
    up = right.cross(front).normalized();
}