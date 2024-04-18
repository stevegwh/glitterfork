#pragma once
#include <utility>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

namespace slib
{
#define MAX_BONE_INFLUENCE 4
struct material;

struct texture
{
    int w, h;
    std::vector<unsigned char> data;
    unsigned int bpp;
    unsigned int id;
    std::string type;
    std::string path;
};

struct material
{
    float Ns{};
    std::array<float,3> Ka{};
    std::array<float,3> Kd{};
    std::array<float,3> Ks{};
    std::array<float,3> Ke{};
    float Ni{};
    float d{};
    int illum{};
    texture map_Kd;
    texture map_Ks;
    texture map_Ns;
};

struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct tri
{
    slib::vertex v1;
    slib::vertex v2;
    slib::vertex v3;
    std::string material;
};

struct Color
{
    int r, g, b;
};

glm::mat4 fpsviewGl( const glm::vec3& eye, float pitch, float yaw );

}
