//
//  Material.cpp
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#include "Material.h"
#include "SceneManager.h"

void Material::initialize(SceneManager *sceneManager, const MaterialData &materialData, ShaderPtr shader)
{
    for (auto i = materialData.textures.cbegin(); i != materialData.textures.cend(); ++i)
    {
        const std::string &texName = i->first;
        const std::string &texFileName = i->second;
        TexturePtr texture = sceneManager->loadTexture(texFileName);
        setTexture(texName, texture);
    }
    setVectors(materialData.vectors);
    setScalars(materialData.scalars);
    setName(materialData.name);
    setShader(shader);
}

void Material::bind()
{
    _shader->bind();
    _shader->setUniforms(_textures);
    _shader->setUniforms(_vectors);
    _shader->setUniforms(_scalars);
}
