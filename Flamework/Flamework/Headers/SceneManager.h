//
//  SceneManager.h
//  Framework
/*
    Manages the graphics part of the applcation.
    Initializes and uses models, camera, shader, textures, geometry and FBO's(todo)
*/
//  Created by Rahul Mukhi on 12/12/12.
//  Reworked by David Steiner
//

#ifndef FRAMEWORK_SCENE_MANAGER_H
#define FRAMEWORK_SCENE_MANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include "vector.hpp"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Geometry.h"
#include "Sound.h"

class Application;

class SceneManager
{
public:
    typedef std::unordered_map< std::string, ShaderPtr >    Shaders;
    typedef std::unordered_map< std::string, TexturePtr >   Textures;
    typedef std::unordered_map< std::string, MaterialPtr >  Materials;
    typedef std::unordered_map< std::string, ModelPtr >     Models;
    typedef std::unordered_map< std::string, SoundPtr >     Sounds;
    
    SceneManager(Application *application);
    virtual ~SceneManager();
    
    virtual void initialize(size_t width, size_t height) = 0;
    virtual void draw(double deltaT) = 0;
    
    // ToDo : createFrameBufferObject
    virtual ModelPtr    loadModel(const std::string &fileName, bool flipT = false, bool flipZ = false);
    virtual ShaderPtr   loadShader(const std::string &shaderName);
    virtual TexturePtr  loadTexture(const std::string &fileName);
    virtual SoundPtr    loadSound(const std::string &fileName);
    
    virtual ModelPtr    createModel(const std::string &name,    const ModelData &modelData);
    virtual ShaderPtr   createShader(const std::string &name,   const ShaderData &shaderData);
    virtual TexturePtr  createTexture(const std::string &name,  const TextureData &textureData);
    virtual MaterialPtr createMaterial(const std::string &name, const MaterialData &materialData);
    
    virtual ModelPtr    createModel();
    virtual TexturePtr  createTexture();
    virtual GeometryPtr createGeometry();
    
    virtual ModelPtr    getModel(const std::string &name);
    virtual ShaderPtr   getShader(const std::string &name);
    virtual TexturePtr  getTexture(const std::string &name);
    virtual MaterialPtr getMaterial(const std::string &name);
    virtual SoundPtr    getSound(const std::string &name);
    
    virtual Application *getApplication();
    
private:
    Shaders     _shaders;
    Textures    _textures;
    Materials   _materials;
    Models      _models;
    Sounds      _sounds;
    
    Application *_application;
    
    vmml::vec2f _scrolling;
    vmml::vec2f _lScrollPos;
    
    vmml::vec2f _scaling;
    vmml::vec2f _lScale;
};

#endif
