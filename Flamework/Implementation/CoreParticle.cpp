//
//  CoreParticle.cpp
//  Flamework
//
//  Created by Bernard Schroffenegger on 21.05.15.
//
//

#include "CoreParticle.h"
#include <cmath>

CoreParticle::CoreParticle() {
}

CoreParticle::~CoreParticle() {
}

CoreParticle::CoreParticle(Position position, vmml::vec3f travelDirection, float birthTime, float speed) {
    _travelDirection = travelDirection;
    _birthTime = birthTime;
    _speed = speed;
}

void CoreParticle::create(float time, float speed, float size, float randomSeed) {
    srand((unsigned int) randomSeed);
    float delta_x = (rand() % 201) / 100.f * pow(-1.f, rand()%2);
    float delta_y = sqrt(4-(delta_x*delta_x)) * pow(-1.f, rand()%2);
    _travelDirection = vmml::normalize(vmml::vec3f(delta_x, delta_y, 80.f));
    _birthTime = time;
    _speed = speed;
    _size = size;
    _random1 = rand() %101 / 10.f+50.f;
    _random2 = rand() %101 / 100.f;
    int a = pow(-1, rand()%2);
    int b = pow(-1, rand()%2);
    rotAxis = vmml::vec3f(rand()%20, a*rand()%20, b*rand()%20);
    rotSpeed = rand()%20;
}

void CoreParticle::setSpeed(float speed) {
    _speed = speed;
}

Position CoreParticle::getPosition(float time) {
    return _travelDirection*(time-_birthTime)*_speed;
}

vmml::mat4f CoreParticle::getModelMatrix(float time) {
    return getPosition(time).getModelMatrix(_size);
}

vmml::vec3f CoreParticle::getRotAxis() { return rotAxis; }

float CoreParticle::getRotSpeed() { return rotSpeed; }

float CoreParticle::getRandom1() { return _random1; }

float CoreParticle::getRandom2() { return _random2; }
