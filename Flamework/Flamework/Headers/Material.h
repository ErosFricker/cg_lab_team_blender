//
//  Material.h
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#ifndef FRAMEWORK_MATERIAL_H
#define FRAMEWORK_MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include "MaterialData.h"

class SceneManager;

class Material
{
public:
    typedef std::unordered_map<std::string, TexturePtr>     Textures;
    typedef std::unordered_map<std::string, vmml::vec3f>    Vectors;
    typedef std::unordered_map<std::string, float>          Scalars;
    
    void initialize(SceneManager *sceneManager, const MaterialData &materialData, ShaderPtr shader);
    
    void bind();
    
    const Textures  &getTextures()                      { return _textures;         }
    TexturePtr      getTexture(const std::string &name) { return _textures[name];   }
    
    const Vectors   &getVectors()                       { return _vectors;          }
    vmml::vec3f     getVector(const std::string &name)  { return _vectors[name];    }
    
    const Scalars   &getScalars()                       { return _scalars;          }
    float           getScalar(const std::string &name)  { return _scalars[name];    }

    void            setTextures(const Textures &arg)                            { _textures = arg;          }
    void            setTexture(const std::string &name, TexturePtr arg)         { _textures[name] = arg;    }
    
    void            setVectors(const Vectors& arg)                              { _vectors = arg;           }
    void            setVector(const std::string &name, const vmml::vec3f &arg)  { _vectors[name] = arg;     }
    
    void            setScalars(const Scalars &arg)                              { _scalars = arg;           }
    void            setScalar(const std::string &name, float arg)               { _scalars[name] = arg;     }
    
#ifdef USE_GL_ES2
    ShaderPtr   getShader()                 { return _shader;       }
    void        setShader(ShaderPtr arg)    { _shader       = arg;  }
#endif
    
    std::string getName()                       { return _name; }
    void        setName(const std::string &arg) { _name = arg;  }
    
private:
    Textures    _textures;
    Vectors     _vectors;
    Scalars     _scalars;
    
#ifdef USE_GL_ES2
    ShaderPtr   _shader;
#endif
    
    std::string _name;
};

typedef std::shared_ptr< Material > MaterialPtr;

#endif
