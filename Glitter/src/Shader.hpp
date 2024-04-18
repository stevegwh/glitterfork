//
// Created by Steve Wheeler on 07/02/2024.
//

#pragma once

#include <string>

namespace sage
{

class Shader
{
    static std::string ReadShaderFile(const std::string& path);
public:
    std::string vertexShader;
    std::string fragShader;
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();
    Shader(Shader const &) = delete;
    Shader & operator=(Shader const &) = delete;
    unsigned int ID;
    void Compile();
    void Use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

};



}
