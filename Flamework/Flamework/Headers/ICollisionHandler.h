//
//  ICollisionHandler.h
//  Framework
/*
    Interface between physics engine and physical object
*/
//  Created by Rahul Mukhi on 1/16/13.
//

#ifndef FRAMEWORK_I_COLLISION_HANDLER_H
#define FRAMEWORK_I_COLLISION_HANDLER_H

class ICollisionHandler
{
    
public:
    
    virtual void onCollide() = 0;
    
};

#endif
