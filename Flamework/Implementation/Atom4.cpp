//
//  Atom4.cpp
//  Flamework
//
//  Created by Bernard Schroffenegger on 25.05.15.
//
//

#include "Atom4.h"
#include <cmath>

float Atom4::_distance = .28f;

float Atom4::_speedMin = 15.f;
float Atom4::_speedMax = 20.f;

float Atom4::_rotation1Max = 10.f;
float Atom4::_rotation1Min = 0.f;

float Atom4::_rotation2Max = 10.f;
float Atom4::_rotation2Min = 0.f;

float Atom4::_shakeAmplitudeMax = 0.2f;
float Atom4::_shakeAmplitudeMin = 0.f;

float Atom4::_shakeRateMax = 20.f;
float Atom4::_shakeRateMin = 0.f;

Atom4::Atom4()
{
}

Atom4::Atom4(float time)
{
    createRandom(time);
}

Atom4::~Atom4()
{
}

Atom* Atom4::getAtoms()
{
    return _atoms;
}

void Atom4::createRandom(float time)
{
    // Random vertices of the tetraeder
    vmml::vec3f vectors[ATOMS];
    vectors[0] = createRandomVector();
    vectors[1] = vmml::create_rotation(acosf(-1.f/3.f), getRandomNormal(vectors[0])) * vectors[0];
    vectors[2] = vmml::create_rotation(2.f*M_PI_F / 3.f, vectors[0]) * vectors[1];
    vectors[3] = vmml::create_rotation(4.f*M_PI_F / 3.f, vectors[0]) * vectors[1];
    
    // Scale Vectors down
    for (int i=0; i<4; ++i) vectors[i] = _distance * vectors[i];
    
    // Define randomly interdependant properties
    vmml::vec3f rotAxis2 = createRandomVector();
    vmml::vec3f dir = createRandomDirection();
    
    float speed = createSpeed();
    float rot2Speed = createRotation2();
    float shakeAplitude = createShakeAmplitude();
    float shakeRate = createShakeRate();
    
    // Assign properties
    for (int i=0; i<ATOMS; ++i)
    {
        _atoms[i].createRandom(time);
        _atoms[i].setRadius(vectors[i]);
        _atoms[i].setShakeDirection(vectors[i]);
        _atoms[i].setShakeAmplitude(shakeAplitude);
        _atoms[i].setSakeRate(shakeRate);
        _atoms[i].setDirection(dir);
        _atoms[i].setSpeed(speed);
        _atoms[i].setRot2(rotAxis2);
        _atoms[i].setRot2Speed(rot2Speed);
    }
}

vmml::vec3f Atom4::getRandomNormal(vmml::vec3f v)
{
    v = vmml::normalize(vmml::normalize(v));
    vmml::vec3f n = vmml::normalize(createRandomVector());
    return vmml::normalize(n - vmml::dot(n, v)/vmml::dot(v,v)*v);
}

vmml::vec3f Atom4::createRandomVector()
{
    float x = pow(-1, rand()%2) * (rand()%100);
    float y = pow(-1, rand()%2) * (rand()%100);
    float z = pow(-1, rand()%2) * (rand()%100);
    return vmml::normalize(vmml::vec3f(x,y,z));
}

vmml::vec3f Atom4::createRandomDirection()
{
    float x = pow(-1.f, rand()%2) * (rand() % 201) / 100.f;
    float y = pow(-1.f, rand()%2) * sqrt(4-(x*x));
    return vmml::normalize(vmml::vec3f(x, y, 80.f));
}

void Atom4::setSpeedLimits(float min, float max) {
    _speedMin = min;
    _speedMax = max;
}

void Atom4::setRotation1SpeedLimits(float min, float max) {
    _rotation1Max = max;
    _rotation1Min = min;
}

void Atom4::setRotation2SpeedLimits(float min, float max) {
    _rotation2Max = max;
    _rotation2Min = min;
}

void Atom4::setShakeAmplitudeLimits(float min, float max) {
    _shakeAmplitudeMax = max;
    _shakeAmplitudeMin = min;
}

void Atom4::setShakeRateLimits(float min, float max) {
    _shakeRateMax = max;
    _shakeRateMin = min;
}

// Random-float between _speedMin and _speedMax
float Atom4::createSpeed()
{
    return _speedMin + (((float) rand()) / (float) RAND_MAX)*(_speedMax-_speedMin);
}

float Atom4::createRotation1()
{
    return _rotation1Min + (((float) rand()) / (float) RAND_MAX)*(_rotation1Max-_rotation1Min);
}

float Atom4::createRotation2()
{
    return _rotation2Min + (((float) rand()) / (float) RAND_MAX)*(_rotation2Max-_rotation2Min);
}

float Atom4::createShakeAmplitude()
{
    return _shakeAmplitudeMin + (((float) rand()) / (float) RAND_MAX)*(_shakeAmplitudeMax-_shakeAmplitudeMin);
}

float Atom4::createShakeRate() {
    return _shakeRateMin + (((float) rand()) / (float) RAND_MAX)*(_shakeRateMax-_shakeRateMin);
}
