#pragma once

#include <glad/glad.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    Object(string const &path, float3 worldPos = float3::Zero(), quaternion localRot = quaternion::Identity(), float3 localScale = float3::Ones());
    void Draw(GLenum drawMode = GL_TRIANGLES);
    void Draw(Shader &drawShader, GLenum drawMode = GL_TRIANGLES);
    // Assemble model matrix
    float4x4 model();

private:
    void loadModel(string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};
