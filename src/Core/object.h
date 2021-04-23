#pragma once

#include <glad/glad.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Core/Log.h>
#include <Core/shader.h>
#include <Core/mesh.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

typedef Eigen::Matrix<float, 2, 1> float2;
typedef Eigen::Matrix<float, 3, 1> float3;
typedef Eigen::Matrix<float, 4, 4> float4x4;
typedef Eigen::Quaternionf quaternion;

class Object
{
public:
    vector<Mesh> meshes;
    string name;
    Shader *shader;
    float3 position;
    quaternion rotation;
    float3 scale;

    Object(string const &path);
    void Draw();
    void Draw(Shader &drawShader);

private:
    void loadModel(string const &path);
    void processNode();
    Mesh processMesh();
    // Assemble model matrix
    float4x4 model();
};
