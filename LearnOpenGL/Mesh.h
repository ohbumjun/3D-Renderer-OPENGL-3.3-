#pragma once

#include <fstream>
#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "Shader.h"
#include <vector>

// math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Vertex
{
    /*
    * 각각의 vertex attribute 를 저장
    */
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


struct Texture
{
    unsigned int id;
    std::string type;
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures);
    void Draw(Shader &shader);

private:
    // render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};
