//
// Created by Steve Wheeler on 16/04/2024.
//

#include "RendererGl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace sage
{


unsigned int RendererGl::LoadTexture(const std::string& path, GLenum textureNumber, GLenum format)
{

    // load textures
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(textureNumber);
    glBindTexture(GL_TEXTURE_2D, texture);
// set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: stb_image: Failed to load texture. Wrong path?" << std::endl;
    }
    stbi_image_free(data);
    // ----
    return texture;
}

void RendererGl::Render()
{
    
}

void RendererGl::cleanup()
{
    
}

RendererGl::RendererGl()
{
}

RendererGl::~RendererGl()
{
    cleanup();
}
} // sage