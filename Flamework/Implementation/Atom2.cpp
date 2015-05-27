//
//  Atom2.cpp
//  Flamework
//
//  Created by Bernard Schroffenegger on 25.05.15.
//
//

#include "Atom2.h"

float Atom2::_speedMin = 18.f;
float Atom2::_speedMax = 20.f;

float Atom2::_rotation1Max = 10.f;
float Atom2::_rotation1Min = 0.f;

float Atom2::_rotation2Max = 0.f;
float Atom2::_rotation2Min = 0.f;

float Atom2::_shakeAmplitudeMax = 0.f;
float Atom2::_shakeAmplitudeMin = 0.f;

float Atom2::_shakeRateMax = 50.f;
float Atom2::_shakeRateMin = 50.f;

float Atom2::_distance = 0.2f;

Atom2::Atom2() {
    
}

Atom2::Atom2(float time) {
    createRandom(time);
}

Atom2::~Atom2() {
    
}

Atom* Atom2::getAtoms() {
    return _atoms;
}

void Atom2::createRandom(float time) {
    
    // Random vertices
    vmml::vec3f vectors[ATOMS];
    vectors[0] = createRandomVector();
    vectors[1] = -vectors[0];
    
    // Scale Vectors down
    for (int i=0; i<ATOMS; ++i) vectors[i] = _distance * vectors[i];
    
    // Define randomly interdependant properties
    vmml::vec3f rotAxis2 = createRandomVector();
    vmml::vec3f dir = createRandomDirection();
    
    float speed = createSpeed();
    float rot2Speed = createRotation2();
    float shakeAmpl = createShakeAmplitude();
    float shakeRate = createShakeRate();
    
    for (int i=0; i<ATOMS; ++i)
    {
        _atoms[i].createRandom(time);
        _atoms[i].setRadius(vectors[i]);
        _atoms[i].setDirection(dir);
        
        _atoms[i].setShakeDirection(vectors[i]);
        _atoms[i].setShakeAmplitude(shakeAmpl);
        _atoms[i].setSakeRate(shakeRate);
        
        _atoms[i].setRot2(rotAxis2);
        _atoms[i].setRot2Speed(rot2Speed);
        
        _atoms[i].setSpeed(speed);
    }
}

vmml::vec3f Atom2::createRandomVector()
{
    float x = pow(-1, rand()%2) * (rand()%100);
    float y = pow(-1, rand()%2) * (rand()%100);
    float z = pow(-1, rand()%2) * (rand()%100);
    return vmml::normalize(vmml::vec3f(x,y,z));
}

vmml::vec3f Atom2::createRandomDirection()
{
    float x = pow(-1.f, rand()%2) * (rand() % 201) / 100.f;
    float y = pow(-1.f, rand()%2) * sqrt(4-(x*x));
    return vmml::normalize(vmml::vec3f(x, y, 80.f));
}

void Atom2::setSpeedLimits(float min, float max) {
    _speedMin = min;
    _speedMax = max;
}

void Atom2::setRotation1SpeedLimits(float min, float max) {
    _rotation1Max = max;
    _rotation1Min = min;
}

void Atom2::setRotation2SpeedLimits(float min, float max) {
    _rotation2Max = max;
    _rotation2Min = min;
}

void Atom2::setShakeAmplitudeLimits(float min, float max) {
    _shakeAmplitudeMax = max;
    _shakeAmplitudeMin = min;
}

void Atom2::setShakeRateLimits(float min, float max) {
    _shakeRateMax = max;
    _shakeRateMin = min;
}


// Creates a random-float between _speedMin and _speedMax
float Atom2::createSpeed()
{
    return _speedMin + (((float) rand()) / (float) RAND_MAX)*(_speedMax-_speedMin);
}

float Atom2::createRotation1()
{
    return _rotation1Min + (((float) rand()) / (float) RAND_MAX)*(_rotation1Max-_rotation1Min);
}

float Atom2::createRotation2()
{
    return _rotation2Min + (((float) rand()) / (float) RAND_MAX)*(_rotation2Max-_rotation2Min);
}

float Atom2::createShakeAmplitude()
{
    return _shakeAmplitudeMin + (((float) rand()) / (float) RAND_MAX)*(_shakeAmplitudeMax-_shakeAmplitudeMin);
}

float Atom2::createShakeRate() {
    return _shakeRateMin + (((float) rand()) / (float) RAND_MAX)*(_shakeRateMax-_shakeRateMin);
}


