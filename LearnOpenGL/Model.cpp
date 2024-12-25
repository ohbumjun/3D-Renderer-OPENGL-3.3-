#include "Model.h"

using namespace std;

unsigned int TextureFromFile(const char *path,
                             const string &directory,
                             bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);//

    int width, height, nrComponents;
    unsigned char *data =
        stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format,
                     width,
                     height,
                     0,
                     format,
                     GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
void Model::Draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
    // model 을 Assimp 의 scene object 로 import 한다.
    Assimp::Importer importer;

    // aiProcess_Triangulate :
    // - 모든 mesh 를 triangle 로 변환한다. model 이 triangle 로 모두
    // 이루어지지 않은 경우, model 의 primitive shape 를 trinalg e 로 변경
    // - aiProcess_FlipUVs flips the texture coordinates on the y-axis where necessary
    // during processing (you may remember from the Textures chapter that most images in OpenGL were
    // reversed around the y-axis
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of("/"));

    // root node 를 인자로 넘겨준다.
    processNode(scene->mRootNode, scene);
}

// 각 node 는 children 을 가지고 있다. 해당 children 에 대한 재귀 호출
void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node’s meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        // 1) pos
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // 2) normal
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        vertices.push_back(vertex);

        // 3) texture cooord
        // mesh->mTextureCoords : array that can hold up to 
        //      8 different sets of texture coordinates per vertex.
        if (mesh->mTextureCoords
                [0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // 현재 'i' 번째 vertex 의 texture coordinate 를 가져온다.
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }

    // process indices
    // Assimp 에서 mesh 는 여러개의 face 를 가지고 있다.
    // 이때 face 는 하나의 single primitive 를 가리킨다.
    // 우리는 맨 처음 옵션에서 aiProcess_Triangulate 를 사용했기 때문에
    // 모든 primitive 는 triangle 로 이루어져 있다.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // mNumIndices : face 가 가지고 있는 index 의 개수 (어떤 순서로 그려야 하는가)
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    // - mesh 는 material object 에 대한 index 만을 가지고 있다.
    if (mesh->mMaterialIndex >= 0)
    {
        // material 을 가져온다.
        // - material 내부는 각 texture type 에 대한 texture location array 를 지닌다.
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse
        vector<Texture> diffuseMaps =
            loadMaterialTextures(material,
                                 aiTextureType_DIFFUSE,
                                 "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // 2. specular
        vector<Texture> specularMaps =
            loadMaterialTextures(material,
                                 aiTextureType_SPECULAR,
                                 "texture_specular");
        textures.insert(textures.end(),
                        specularMaps.begin(),
                        specularMaps.end());

        // 3. normal maps
        std::vector<Texture> normalMaps =
            loadMaterialTextures(material,
                                 aiTextureType_HEIGHT,
                                 "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        // 4. height maps
        std::vector<Texture> heightMaps =
            loadMaterialTextures(material,
                                 aiTextureType_AMBIENT,
                                 "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

}

// material 로부터 texture 를 가져오기
std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName)
{
    vector<Texture> textures;

    // material 내 모든 texture 를 가져온다.
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        { // if texture hasn’t been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}
