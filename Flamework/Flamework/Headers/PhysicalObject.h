//
//  PhysicalObject.h
//  Framework
/*
    Implementation of collision detection with the physical object
*/
//  Created by Rahul Mukhi on 1/16/13.
//

#ifndef FRAMEWORK_PHYSICAL_OBJECT_H
#define FRAMEWORK_PHYSICAL_OBJECT_H

#include <iostream>
#include "IPhysical.h"
#include "ICollisionHandler.h"

class PhysicalObject: public IPhysical
{
public:
    void setCollisionHandler(ICollisionHandler);
};

#endif
