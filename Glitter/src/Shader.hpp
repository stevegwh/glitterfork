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
    void Compile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    unsigned int ID{};
public:
    void Use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    unsigned int getID() const;
    
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();
    // Enforce shader being passed as a pointer, to avoid calling the destructor and invalidating OpenGL's shader ID.
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(Shader&&) = delete;
};



}
