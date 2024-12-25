#pragma once

#include "Shader.h"
#include <string>
#include "Mesh.h"
#include <vector>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

unsigned int TextureFromFile(const char *path,
                             const std::string &directory,
                             bool gamma = false);

class Model
{
public:
    Model(char *path) // file path
    {
        loadModel(path); // assimp import
    }
    void Draw(Shader &shader);

private:
    // model data : �������� Mesh �� �̷���� �ִ�.
    std::vector<Mesh> meshes;
    // file path ��ε� ����
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                         aiTextureType type,
                                              std::string typeName);

    // Mesh �� �ϳ��� texture �� ���� ������ ������ �� �ִ�.
    // - ���� �̹� load �� texture �� �����صΰ� �ѹ����� load �� �� �ֵ��� �Ѵ�.
    std::vector<Texture> textures_loaded;
};