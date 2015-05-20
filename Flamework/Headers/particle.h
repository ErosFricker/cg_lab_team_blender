//
//  Particle.h
//  Flamework
//
//  Created by Bernard Schroffenegger on 15.05.15.
//
//

#ifndef Flamework_Particle_h
#define Flamework_Particle_h

#include "addendum.hpp"


class Particle {
    
public:
    Particle();
    ~Particle();
    void setTranslation( vmml::vec3f vector );
    void setRotation(vmml::mat4f vector);
    void generateRandomParticle( double seed );
    vmml::mat4f getModelMatrix( float deltaT, float scaling );
    bool passed();
    
    float getLifetime();
    unsigned int getNumberOfElectrons();
    unsigned int getOrthonormalSystem();
    vmml::vec3f getCurrentPosition();
    float getCurrentScalingFactor();
    float getRandom1();
    float getRandom2();
    
private:
    float lifeTime;
    float _scaling;
    vmml::mat4f _modelMatrix;
    vmml::vec3f _translationVector;
    vmml::vec3f _currentPosistion;
    vmml::mat4f _rotationMatrix;
    
    unsigned int _electrons;
    unsigned int _system;
    
    float _random1;
    float _random2;
};

#endif
