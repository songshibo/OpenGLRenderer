#pragma once

#include <glad/glad.h>
#include <Eigen/Core>

#include <string>
#include <vector>
#include <Core/shader.h>

typedef Eigen::Matrix<float, 2, 1> float2;
typedef Eigen::Matrix<float, 3, 1> float3;
typedef Eigen::Matrix<float, 4, 1> float4;
typedef Eigen::Matrix<float, 4, 4> float4x4;

using namespace std;

struct Vertex
{
    float3 position;
    float3 normal;
    float2 texcoords;
    float3 tangent;
    float3 bitangent;
};

struct Texture
{
    GLuint id;
    string type;
    string path;
};

class Mesh
{
public:
    vector<Vertex> vertices;
    vector<uint32_t> indices;
    GLuint VAO;
    Mesh(vector<Vertex> verts, vector<uint32_t> faces);
    virtual ~Mesh();
    void Draw(Shader &shader, GLenum drawMode = GL_TRIANGLES);

private:
    GLuint VBO, EBO;
    void SetUp();
};
