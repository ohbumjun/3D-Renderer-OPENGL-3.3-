#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

// math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    
     // �߿� : shader ������ uniform �� ã�� ���� �� shader �� ����� ���İ�
    // �ƴϾ �ȴ�. ��, �ش� uniform �� ���� �����ϴ� ���� shader ��� ����
    // utility uniform functions
    void setBool(const std::string& uniformName, bool value) const
    {
        unsigned int uniform = glGetUniformLocation(ID, uniformName.c_str());

        // ��ã�� ��� -1 ����
        // assert(uniform != -1);

        glUniform1i(uniform, (int)value);
    }
    void setInt(const std::string &uniformName, int value) const
    {
        unsigned int uniform = glGetUniformLocation(ID, uniformName.c_str());

        // ��ã�� ��� -1 ����
        // assert(uniform != -1);

        glUniform1i(uniform, value);
    }
    void setFloat(const std::string &uniformName, float value) const
    {
        unsigned int uniform = glGetUniformLocation(ID, uniformName.c_str());
        // ��ã�� ��� -1 ����
        // assert(uniform != -1);

        glUniform1f(uniform, value);
    }
    void setVec3f(const std::string &uniformName, const glm::vec3& value) const
    {
        unsigned int uniform = glGetUniformLocation(ID, uniformName.c_str());
        // ��ã�� ��� -1 ����
        // assert(uniform != -1);
        glUniform3f(uniform, value.x, value.y, value.z);
    }

    // Matrix4f
    void setMat4(const std::string& uniformName, const glm::mat4& value) const
    {
        unsigned int uniform = glGetUniformLocation(ID, uniformName.c_str());
        // assert(uniform != -1);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
	}
};
#endif