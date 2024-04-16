//
// Created by Steve Wheeler on 07/02/2024.
//

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace sage
{

struct Shader
{
    std::string vertexShader;
    std::string fragShader;
    
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    : vertexShader(ReadShaderFile(vertexShaderPath)), fragShader(ReadShaderFile(fragmentShaderPath))
    {}
    
private:

    std::string ReadShaderFile(const std::string& path)
    {
        std::stringstream ss;
        std::ifstream file;
        file.open(path);
        if (!file)
        {
            std::cout << "ERROR: ReadShaderFile: Shader file failed to load \n";
        }
        ss << file.rdbuf();
        file.close(); // for good luck
        return ss.str();;
    }

};



}
