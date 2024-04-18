#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "slib.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

#include <string>
#include <vector>


namespace sage {
class Model
{
public:
    glm::vec3 position = { 1.0f, 1.0f, 1.0f };
    std::vector<slib::texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection{};

    Model(std::string const &path, bool gamma = false);
    void Draw(Shader &shader);

private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<slib::texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
} // sage