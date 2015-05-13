//
//  PhysicsManager.cpp
//  Framework
//
//  Created by Rahul Mukhi on 12/12/12.
//

#include "PhysicsManager.h"
#include "Util.h"

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;


PhysicsManager::PhysicsManager(Application *application)
    : _application(application)
{}

PhysicsManager::~PhysicsManager()
{
    
}

void PhysicsManager::initialize()
{
    
}

void PhysicsManager::updateSimulation(double deltaT)
{
    // util::log("updateSimulation, deltaT: " + lexical_cast< std::string >(deltaT));
}
