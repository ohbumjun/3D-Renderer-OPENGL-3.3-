#pragma once

#include <string>
#include "Shader.h"
#include "stb_image.h"
#include <vector>

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
    std::string type; // Texture 이름
    std::string path;      // store path of texture to compare with other textures
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

    // render data
    unsigned int VAO, VBO, EBO;
private:
    void setupMesh();
};
