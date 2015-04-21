
//
//  SceneManager.cpp
//  Framework
//
//  Created by Rahul Mukhi on 12/12/12.
//  Reworked by David Steiner
//

#include "SceneManager.h"
#include "addendum.hpp"

#include "Application.h"
#include "TextureData.h"
#include "ModelData.h"
#include "ShaderData.h"
#include "Framework_GL.h"
#include "Util.h"

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f

static const std::string DEFAULT_MATERIAL_NAME = "default";


SceneManager::SceneManager(Application *application)
    : _application(application)
{
    
}

SceneManager::~SceneManager() {}

Application *SceneManager::getApplication()
{
    return _application;
}

std::string getRawName(const std::string &fileName, std::string *ext = nullptr)
{
    int index = fileName.find_last_of(".");
    if (index != std::string::npos)
    {
        if (ext) *ext = fileName.substr(index + 1, std::string::npos);
        return fileName.substr(0, index);
    }
    return fileName;
}

ModelPtr SceneManager::loadModel(const std::string &fileName, bool flipT, bool flipZ)
{
    std::string name = getRawName(fileName);
    ModelData modelData(fileName, flipT, flipZ);
    return createModel(name, modelData);
}

ModelPtr SceneManager::createModel()
{
    ModelPtr model(NULL);
    return model;
}

TexturePtr SceneManager::loadTexture(const std::string &fileName)
{
    std::string name = getRawName(fileName);
    TextureData textureData(fileName);
    return createTexture(name, textureData);
}

TexturePtr SceneManager::createTexture()
{
    TexturePtr texture(new Texture());
    return texture;
}

GeometryPtr SceneManager::createGeometry()
{
    GeometryPtr geometry(new Geometry());
    return geometry;
}

ShaderPtr SceneManager::loadShader(const std::string &shaderName)
{
    std::string name = getRawName(shaderName);
    
    ShaderData shaderData(shaderName);
    ShaderPtr shader = createShader(name, shaderData);
    if (shader) return shader;
    
    shader = loadShader(DEFAULT_MATERIAL_NAME);
    if (shader) return shader;
    
    util::log("Couldn't load shader '" + name + "'.", util::LM_INFO);
    return nullptr;
}

SoundPtr SceneManager::loadSound(const std::string &fileName)
{
    std::string name = getRawName(fileName);
    SoundPtr &sound = _sounds[name];
    if (sound) return sound;
    
    sound = SoundPtr(new Sound(fileName));
    return sound;
}

ModelPtr SceneManager::createModel(const std::string &name, const ModelData &modelData)
{
    ModelPtr &model = _models[name];
    if (model) return model;
    
    model = ModelPtr(new Model(this, modelData));
    return model;
}

ShaderPtr SceneManager::createShader(const std::string &name, const ShaderData &shaderData)
{
    ShaderPtr &shader = _shaders[name];
    if (shader) return shader;
    
    if (shaderData.isValid())
    {
        util::log("Created shader '" + name + "'.", util::LM_INFO);
        shader = ShaderPtr(new Shader(shaderData));
        shader->registerAttrib("Position", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
        shader->registerAttrib("Normal", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
         shader->registerAttrib("Tangent", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent));
//         shader->registerAttrib("Bitangent", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, bitangent));
        shader->registerAttrib("TexCoord", 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
        return shader;
    }
    
    return nullptr;
}

TexturePtr SceneManager::createTexture(const std::string &name, const TextureData &textureData)
{
    TexturePtr &texture = _textures[name];
    if (texture) return texture;
    
    texture = TexturePtr(new Texture(textureData));
    
    return texture;
}

MaterialPtr SceneManager::createMaterial(const std::string &name, const MaterialData &materialData)
{
    MaterialPtr &material = _materials[name];
    if (material) return material;
    
    material = MaterialPtr(new Material);
    material->initialize(this, materialData, loadShader(name));
    return material;
}

ModelPtr SceneManager::getModel(const std::string &name)
{
    return _models[name];
}

ShaderPtr SceneManager::getShader(const std::string &name)
{
    return _shaders[name];
}

TexturePtr SceneManager::getTexture(const std::string &name)
{
    return _textures[name];
}

MaterialPtr SceneManager::getMaterial(const std::string &name)
{
    return _materials[name];
}

SoundPtr SceneManager::getSound(const std::string &name)
{
    return _sounds[name];
}
