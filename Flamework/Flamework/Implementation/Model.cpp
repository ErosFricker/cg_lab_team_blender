//
//  Model.cpp
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//  Copyright (c) 2013 Rahul Mukhi. All rights reserved.
//  Reworked by David Steiner
//

#include "Model.h"
#include "ModelData.h"
#include "TextureData.h"
#include "SceneManager.h"
#include "Util.h"

#include "Framework_GL.h"


// TODO: clean up
Model::Model(SceneManager *sceneManager, const ModelData &modelData)
: _sceneManager(sceneManager)
{
    ModelData::GroupMap data = modelData.getData();

    for (auto i = data.begin(); i != data.end(); ++i)
    {
        Geometry &g = _groups[i->first];
        GeometryDataPtr gData = i->second;
        MaterialPtr material = _sceneManager->createMaterial(gData->materialData.name, gData->materialData);
        g.initialize(gData);
        g.setMaterial(material);
    }
}

Model::~Model()
{}

void Model::draw(GLenum mode)
{
    for (auto i = _groups.begin(); i != _groups.end(); ++i)
    {
        i->second.draw(mode);
    }
}

void Model::draw(const std::string &groupName, GLenum mode)
{
    _groups[groupName].draw(mode);
}
