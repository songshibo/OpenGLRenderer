#include "object.h"

Object::Object(string const &path)
{
    position = float3::Zero();
    scale = float3::Ones();
    rotation = quaternion::Identity();
}

void Object::Draw(Shader &drawShader)
{
    if (shader != nullptr)
        DEBUG_INFO("Object:{} has changed shader!", name);
    shader = &drawShader;

    // Generate Model Matrix

    for (uint32_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(*shader);
    }
}

void Object::Draw()
{
    if (shader == nullptr)
    {
        DEBUG_ERROR("Object:{} has no shader for drawing!", name);
        return;
    }

    // Generate Model Matrix

    for (uint32_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(*shader);
    }
}

float4x4 Object::model()
{
    using namespace Eigen;
    float4x4 model = float4x4::Identity();
    model.block<3, 3>(0, 0) = rotation.toRotationMatrix() * Scaling(scale);
    model.col(3) << position, 1.0f;
}

void Object::loadModel(string const &path)
{
}
void Object::processNode()
{
}
Mesh Object::processMesh()
{
}