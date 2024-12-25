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
    // model data : 여러개의 Mesh 로 이루어져 있다.
    std::vector<Mesh> meshes;
    // file path 경로도 저장
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                         aiTextureType type,
                                              std::string typeName);

    // Mesh 는 하나의 texture 를 여러 군데에 재사용할 수 있다.
    // - 따라서 이미 load 된 texture 를 저장해두고 한번씩만 load 할 수 있도록 한다.
    std::vector<Texture> textures_loaded;
};