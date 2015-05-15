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
, _currentPosistion(vmml::vec3f(0.f))
, lifeTime(0.f)
{
    
}

Particle::~Particle() {}

void Particle::setTranslation( vmml::vec3f vector ) {
    _translationVector = vector;
}

void Particle::generateRandomParticle(double seed) {
    // Random seed
    srand((unsigned int) seed);
    // Random floats between -1 and 1
    float delta_x = (rand() % 101) / 100.f * pow(-1.f, rand()%2);
    float delta_y = (rand() % 101) / 100.f * pow(-1.f, rand()%2);
    setTranslation(vmml::vec3f(delta_x, delta_y, 40.f));
    lifeTime = 0.f;
}

vmml::mat4f Particle::getModelMatrix( float deltaT, float scaling ) {
    vmml::mat4f translation_particle =
        vmml::create_translation(
            vmml::vec3f(
                _translationVector.x()*lifeTime,
                _translationVector.y()*lifeTime,
                _translationVector.z()*lifeTime
            )
        );
    lifeTime += deltaT;
    _modelMatrix = translation_particle * vmml::create_scaling(scaling);
    return _modelMatrix;
}

bool Particle::passed() {
    if (_translationVector.z() * lifeTime > 100) return true;
    else return false;
}

float Particle::getLifetime() { return lifeTime; }

