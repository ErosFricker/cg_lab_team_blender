//
//  Camera.h
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//

#ifndef FRAMEWORK_CAMERA_H
#define FRAMEWORK_CAMERA_H

#include <memory>
#include <iostream>	
#include "vector.hpp"
#include "matrix.hpp"

class Camera
{
public:
    
    Camera();
    ~Camera();
    
    void lookAt(const vmml::vec3f &point);
    void moveCamera(const vmml::vec3f &translation);
    void rotateCamera(const vmml::vec3f &axis, float rotation);
    
private:
    
    vmml::vec3f _position, _view;
    float _speed;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif
