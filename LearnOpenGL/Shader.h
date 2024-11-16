#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <iostream>
#include <sstream>
#include <string>
class Shader
{
public:
    // the program ID
    unsigned int ID;
    // constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath);
    // use/activate the shader
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    void setBool(const std::string& uniformName, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), (int)value);
    }
    void setInt(const std::string &uniformName, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), value);
    }
    void setFloat(const std::string &uniformName, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, uniformName.c_str()), value);
    }
};
#endif