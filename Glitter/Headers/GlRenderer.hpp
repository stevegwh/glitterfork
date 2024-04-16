//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include <string>

namespace sage
{

class glRenderer
{
    void initGLFW();
public:
    
    void LoadTexture(const std::string& path);

};

} // sage
