//
//  PhysicsManager.h
//  Framework
/*
    Implementation of application physics and AI (with or without Bullet physics)
*/
//  Created by Rahul Mukhi on 12/12/12.
//

#ifndef FRAMEWORK_PHYSICS_MANAGER_H
#define FRAMEWORK_PHYSICS_MANAGER_H

#include <iostream>
#include <memory>
//#include "btBulletDynamicsCommon.h"

class Application;

class PhysicsManager
{
    
public:
    PhysicsManager(Application *application);
    ~PhysicsManager();
    
    void initialize();
    void updateSimulation(double deltaT);

private:
    Application *_application;
    
};

typedef std::shared_ptr<PhysicsManager> PhysicsManagerPtr;

#endif