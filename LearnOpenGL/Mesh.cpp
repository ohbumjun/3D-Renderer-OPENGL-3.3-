#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;

        // Texture 이름 규칙을 정해둔 상태
        // ex) texture_diffuse1, texture_diffuse2, texture_specular1, texture_specular2
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setFloat(("material." + name + number).c_str(), i);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO); // VAO 를 생성한다.
    glGenBuffers(1, &VBO);          // Vertex Buffer Object 를 생성한다. (Vertex Data)
    glGenBuffers(1, &EBO);      // Element Buffer Object 를 생성한다. (Index Data)

    glBindVertexArray(VAO);     // VAO Bind 

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO 메모리 생성
    glBufferData(GL_ARRAY_BUFFER,           // vertices  변수 데이터를 그대로 넘겨준다.
                 vertices.size() * sizeof(Vertex),
                 &vertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // EBO 메모리 생성
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int), // indices 변수 데이터를 그대로 넘겨준다.
                 &indices[0],
                 GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal)); // Vertex 구조체의 Normal 변수의 offset

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void *)offsetof(Vertex, TexCoords)); // Vertex 구조체의 TexCoords 변수의 offset

    glBindVertexArray(0);
}
