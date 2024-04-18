#pragma once

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Standard Headers
#include <map>
#include <memory>
#include <vector>
#include "slib.hpp"
#include "Shader.hpp"

// Define Namespace
namespace sage
{

class Mesh {
public:
    // mesh data
    std::vector<slib::vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<slib::texture> textures;

    Mesh(const std::vector<slib::vertex>& vertices, 
         const std::vector<unsigned int>& indices, 
         const std::vector<slib::texture>& textures);
    void Draw(Shader &shader);
private:
    //  render data
    unsigned int VAO{}, VBO{}, EBO{};

    void setupMesh();
};
};
