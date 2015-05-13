//
//  IPhysical.h
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//

#ifndef FRAMEWORK_I_PHYSICAL_H
#define FRAMEWORK_I_PHYSICAL_H

#include "matrix.hpp"

class IPhysical
{
public:
    virtual vmml::mat4f getTransform()=0;
    
};

#endif
