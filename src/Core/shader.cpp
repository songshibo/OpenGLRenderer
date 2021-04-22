#include "shader.h"
#include "Config.h"
#include "Eigen/OpenGLSupport"

Shader::Shader(const char *vertexShader,
               const char *fragmentShader,
               const char *geometryShader,
               const char *tessellationControlShader,
               const char *tessellationEvaluationShader)
    : programID(0)
{
    using namespace std;
    GLuint vertex, fragment, geometry, tessControl, tessEval;

    // Vertex Shader
    string vertexStr = loadShaderFile(vertexShader);
    const char *vertexCode = vertexStr.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    // Fragment Shader
    string fragmentStr = loadShaderFile(fragmentShader);
    const char *fragmentCode = fragmentStr.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);

    if (geometryShader != nullptr)
    {
        string geometryStr = loadShaderFile(geometryShader);
        const char *geometryCode = geometryStr.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryCode, NULL);
        glCompileShader(geometry);
    }
    if (tessellationControlShader != nullptr)
    {
        string tessControlStr = loadShaderFile(tessellationControlShader);
        const char *tessControlCode = tessControlStr.c_str();
        tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessControl, 1, &tessControlCode, NULL);
        glCompileShader(tessControl);
    }
    if (tessellationEvaluationShader != nullptr)
    {
        string tessEvalStr = loadShaderFile(tessellationEvaluationShader);
        const char *tessEvalCode = tessEvalStr.c_str();
        tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessEval, 1, &tessEvalCode, NULL);
        glCompileShader(tessEval);
    }

    programID = glCreateProgram();
    if (programID == 0)
    {
        DEBUG_FATAL("Failed to create shader program object.");
        return;
    }

    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    if (geometryShader != nullptr)
        glAttachShader(programID, geometry);
    if (tessellationControlShader != nullptr)
        glAttachShader(programID, tessControl);
    if (tessellationEvaluationShader != nullptr)
        glAttachShader(programID, tessEval);

    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");
    // delete shader
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryShader != nullptr)
        glDeleteShader(geometry);
    if (tessellationControlShader != nullptr)
        glDeleteShader(tessControl);
    if (tessellationEvaluationShader != nullptr)
        glDeleteShader(tessEval);
}

Shader::~Shader()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
        programID = 0;
    }
}

void Shader::use()
{
    glUseProgram(programID);
}

//Utility functions
void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name, const vec2 &value) const
{
    glUniform(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, const vec3 &value) const
{
    Eigen::glUniform(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetVec4(const std::string &name, const vec4 &value) const
{
    Eigen::glUniform(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetMat2(const std::string &name, const mat2 &mat) const
{
    Eigen::glUniform(glGetUniformLocation(programID, name.c_str()), mat);
}

void Shader::SetMat3(const std::string &name, const mat3 &mat) const
{
    Eigen::glUniform(glGetUniformLocation(programID, name.c_str()), mat);
}

void Shader::SetMat4(const std::string &name, const mat4 &mat) const
{
    Eigen::glUniform(glGetUniformLocation(programID, name.c_str()), mat);
}

std::string Shader::loadShaderFile(const char *filepath)
{
    using namespace std;
    string strpath(filepath);
    ifstream shaderFile(SOURCE_DIR + strpath);
    string content;
    while (shaderFile.good())
    {
        string line;
        getline(shaderFile, line);
        content.append(line + "\n");
    }
    return content;
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            DEBUG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {}\n{}", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            DEBUG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}", type, infoLog);
        }
    }
}