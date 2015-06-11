//
//  PositionManager.h
//  Flamework
//
//  Created by Bernard Schroffenegger on 20.05.15.
//
//

#ifndef Flamework_Position_h
#define Flamework_Position_h

#include "addendum.hpp"

class Position {
    
public:
    
    Position();
    Position(vmml::vec3f position);
    
    ~Position();
    
    void setPosition(vmml::vec3f position);
    vmml::vec3f getPosition();
    vmml::mat4f getModelMatrix(float originalObjectSize);
    
    float x();
    float y();
    float z();
    
    bool isValid();
    
private:
    vmml::vec3f _position;
    
};

#endif

