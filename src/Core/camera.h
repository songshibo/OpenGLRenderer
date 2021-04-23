#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Core/Log.h>

enum FPS_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 60.0f;
const float ZNEAR = 0.1f;
const float ZFAR = 100.0f;

typedef Eigen::Matrix<float, 3, 1> float3;
typedef Eigen::Matrix<float, 4, 1> float4;
typedef Eigen::Matrix<float, 4, 4> float4x4;

static inline float deg2rad(float deg)
{
    return deg * static_cast<float>(M_PI) / 180.0f;
}

class FpsCamera
{
public:
    // Attributes
    float3 position;
    float3 worldUp;
    float3 front, right, up;
    // FPS camera parameters
    float fov, zNear, zFar;
    FpsCamera(float3 cameraPos = float3(0.0f, 0.0f, 0.0f), float3 cameraFront = float3(0.0f, 0.0f, -1.0f), float3 upVector = float3(0.0f, 1.0f, 0.0f), float cameraFov = FOV);
    virtual ~FpsCamera();
    float4x4 perspective(float aspect);
    float4x4 view();
    void ProcessKeyboard(FPS_Movement direction, const float &deltaTime);
    void ProcessMouseInput(const float &xoffset, const float &yoffset, bool constranPitch = true);

private:
    float yaw, pitch;
    void updateCameraVector();
};