#pragma once

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "slib.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

#include <string>
#include <vector>
#include <memory>


namespace sage {
class Model
{
public:
    std::vector<slib::texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection{};
    std::unique_ptr<Shader> shader;

    Model(std::string const &path, std::unique_ptr<Shader> _shader);
    void Draw();

private:

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<slib::texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
} // sage