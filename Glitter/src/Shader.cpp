//
// Created by Steve Wheeler on 16/04/2024.
//
#include "Shader.hpp"

#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <iostream>

namespace sage
{

std::string Shader::ReadShaderFile(const std::string &path)
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

void Shader::Compile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    std::string vertexShader = ReadShaderFile(vertexShaderPath);
    std::string fragShader = ReadShaderFile(fragmentShaderPath);
    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];


// vertex Shader
    const char* vShader = vertexShader.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShader, NULL);
    glCompileShader(vertex);
// print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

// fragment Shader
    const char* fShader = fragShader.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShader, NULL);
    glCompileShader(fragment);
// print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

// shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
// print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

// delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

unsigned int Shader::getID() const
{
    return ID;
}

void Shader::Use() const
{
    glUseProgram(ID);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    Compile(vertexShaderPath, fragmentShaderPath);
}

} //sage


