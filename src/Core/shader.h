#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <Eigen/Core>
#include <Core/Log.h>

typedef Eigen::Matrix<float, 2, 1> vec2;
typedef Eigen::Matrix<float, 3, 1> vec3;
typedef Eigen::Matrix<float, 4, 1> vec4;
typedef Eigen::Matrix<float, 2, 2> mat2;
typedef Eigen::Matrix<float, 3, 3> mat3;
typedef Eigen::Matrix<float, 4, 4> mat4;

class Shader
{
public:
    Shader(const char *vertexShader,
           const char *fragmentShader,
           const char *geometryShader = nullptr,
           const char *tessellationControlShader = nullptr,
           const char *tessellationEvaluationShader = nullptr);
    virtual ~Shader();
    void use();
    // Utility uniform functions
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const vec2 &value) const;
    void SetVec3(const std::string &name, const vec3 &value) const;
    void SetVec4(const std::string &name, const vec4 &value) const;
    void SetMat2(const std::string &name, const mat2 &mat) const;
    void SetMat3(const std::string &name, const mat3 &mat) const;
    void SetMat4(const std::string &name, const mat4 &mat) const;

private:
    GLuint programID;
    std::string loadShaderFile(const char *filepath);
    void checkCompileErrors(GLuint shader, std::string type);
};
