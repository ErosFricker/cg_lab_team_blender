//
//  Particle.cpp
//  Flamework
//
//  Created by Bernard Schroffenegger on 15.05.15.
//
//

#include "Particle.h"
#include "addendum.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DELTA_X     -1.f
#define DELTA_Y     -1.f
#define DELTA_Z     40.f


Particle::Particle()
: _modelMatrix(vmml::mat4f::IDENTITY)
, _translationVector(vmml::vec3f(0.f))
, _rotationMatrix(vmml::mat4f::IDENTITY)
, _currentPosistion(vmml::vec3f(0.f))
, lifeTime(0.f)
{
    
}

Particle::~Particle() {}

void Particle::setTranslation( vmml::vec3f vector ) {
    _translationVector = vector;
}
void Particle::setRotation(vmml::mat4f rotation){
    _rotationMatrix = rotation;
    
}

void Particle::generateRandomParticle(double seed) {
    srand((unsigned int) seed);
    float delta_x = (rand() % 201) / 100.f * pow(-1.f, rand()%2);
    float delta_y = sqrt(4-(delta_x*delta_x)) * pow(-1.f, rand()%2);
    setTranslation(vmml::vec3f(delta_x, delta_y, 80.f));
    _electrons = rand()%4;
    _system = rand()%100;
    lifeTime = 0.f;
    _random1 = rand() %101 / 101.f;
    _random2 = rand() %101 / 101.f;
}

vmml::mat4f Particle::getModelMatrix( float deltaT, float scaling) {
    lifeTime += deltaT;
    _currentPosistion = vmml::vec3f(
                            _translationVector.x()*lifeTime,
                            _translationVector.y()*lifeTime,
                            _translationVector.z()*lifeTime);
    vmml::mat4f translation_particle = vmml::create_translation(vmml::vec3f(_currentPosistion));
    _scaling = 1/100.f * _translationVector.z()*lifeTime*scaling;
    _modelMatrix = translation_particle * vmml::create_scaling(_scaling);
    _modelMatrix = _rotationMatrix*_modelMatrix;
    return _modelMatrix;
}

bool Particle::passed() {
    if (_translationVector.z() * lifeTime > 100) return true;
    else return false;
}

float Particle::getLifetime() { return lifeTime; }
unsigned int Particle::getNumberOfElectrons() { return _electrons; }
unsigned int Particle::getOrthonormalSystem() { return _system; }
vmml::vec3f Particle::getCurrentPosition() { return _currentPosistion; }
float Particle::getCurrentScalingFactor() { return _scaling; }
float Particle::getRandom1() { return _random1; }
float Particle::getRandom2() { return _random2; }

