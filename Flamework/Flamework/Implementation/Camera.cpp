//
//  Camera.cpp
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#include "Camera.h"
#include <cassert>

Camera::Camera()
{
    
}

Camera::~Camera()
{
    
}

void Camera::lookAt(const vmml::vec3f &point)
{
    
}

void Camera::moveCamera(const vmml::vec3f &translation)
{
    assert(!"Not implemented.");
}

void Camera::rotateCamera(const vmml::vec3f &axis, float rotation)
{
    assert(!"Not implemented.");
}
