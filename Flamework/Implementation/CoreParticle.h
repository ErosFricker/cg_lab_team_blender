//
//  CoreParticle.h
//  Flamework
//
//  Created by Bernard Schroffenegger on 21.05.15.
//
//

#ifndef __Flamework__CoreParticle__
#define __Flamework__CoreParticle__

#include "addendum.hpp"
#include "Position.h"

class CoreParticle {
    
public:
    CoreParticle();
    ~CoreParticle();
    
    CoreParticle(Position position, vmml::vec3f travelDirection, float birthTime, float speed);
    
    void create(float time, float speed, float size, float randomSeed);
    void setSpeed(float speed);
    Position getPosition(float time);
    vmml::mat4f getModelMatrix(float time);
    
    float getRandom1();
    float getRandom2();
    vmml::vec3f getRotAxis();
    float getRotSpeed();
    
private:
    float _birthTime;
    float _speed;
    float _size;
    
    float _random1;
    float _random2;
    
    vmml::vec3f rotAxis;
    float rotSpeed;
    
    vmml::vec3f _travelDirection;
};

#endif
