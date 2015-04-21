//
//  ShaderData.cpp
//  Framework
//
//  Created by David Steiner on 4/19/13.
//
//

#include <string>
#include <fstream>
#include <iostream>
#include "ShaderData.h"
#include "Util.h"

ShaderData::ShaderData(const std::string &shaderFile)
:   _valid(true)
{
    load(shaderFile);
}

ShaderData::ShaderData(const std::string &vertShaderFileName, const std::string &fragShaderFileName)
:   _valid(false)
{
    load(vertShaderFileName, fragShaderFileName);
}

ShaderData::ShaderData()
:   _valid(false)
{}

ShaderData &ShaderData::load(const std::string &shaderFile)
{
    return load(shaderFile + ".vert", shaderFile + ".frag");
}

ShaderData &ShaderData::load(const std::string &vertShaderFileName, const std::string &fragShaderFileName)
{
    _vertShaderSrc = loadSrc(vertShaderFileName);
    _fragShaderSrc = loadSrc(fragShaderFileName);
    
    return *this;
}

std::string ShaderData::loadSrc(const std::string &fileName)
{
    std::cout << "Trying to load shader file " << fileName << " . . ." << std::endl;
    
    _valid = false;
    
    if (!util::fileExists(fileName))
    {
        std::cout << ". . . doesn't exist" << std::endl;
        return std::string();
    }
    
    std::ifstream file(util::getFilePath(fileName), std::ifstream::in);
    
    std::string ret;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.length() > 0)
        {
            _valid = true;
        }
        ret += line + "\n";
    }
    
    return ret;
}
