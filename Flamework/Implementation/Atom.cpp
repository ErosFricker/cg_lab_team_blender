//
//  Atom.cpp
//  Flamework
//
//  Created by Bernard Schroffenegger on 24.05.15.
//
//

#include "Atom.h"

const unsigned int Atom::NUMBER_OF_TYPES = 15;

const std::string Atom::TYPE[] =
{
    "water_particle",   // 1
    "fire",
    "silver_particle",
    "gold_particle",
    "forest_particle",
    "stone1_particle",
    "stone2_particle",
    "stone3_particle",
    "storm_particle",
    "wood_particle",
    "blood_particle",
    "cloud_particle",
    "marble_particle",
    "marmor_particle",
    "metal_particle",
};

/*
 loadModel("stone1_particle.obj", true, true);
 loadModel("stone2_particle.obj", true, true);
 loadModel("stone3_particle.obj", true, true);
 loadModel("storm_particle.obj", true, true);
 loadModel("wood_particle.obj", true, true);
 loadModel("blood_particle.obj", true, true);
 loadModel("cloud_particle.obj", true, true);
 loadModel("diamond_particle.obj", true, true);
 loadModel("marble_particle.obj", true, true);
 loadModel("marmor_particle.obj", true, true);
 loadModel("metal_particle.obj", true, true);
 */


float Atom::_speedMin = 18.f;
float Atom::_speedMax = 20.f;

float Atom::_rotation1Max = 10.f;
float Atom::_rotation1Min = 0.f;

float Atom::_rotation2Max = 0.f;
float Atom::_rotation2Min = 0.f;

float Atom::_shakeAmplitudeMax = 0.f;
float Atom::_shakeAmplitudeMin = 0.f;

float Atom::_shakeRateMax = 50.f;
float Atom::_shakeRateMin = 50.f;

Atom::Atom() {
}

Atom::Atom(float time) {
    createRandom(time);
}

Atom::~Atom() {
}

void Atom::createRandom(float time) {
    _particleType = TYPE[rand()%NUMBER_OF_TYPES];
    _birthTime = time;
    _center = vmml::vec3f(0,0,0);
    _radius = vmml::vec3f(0,0,0);
    _shakeDirection = createRandomVector();
    _rot1 = createRandomVector();
    _rot2 = vmml::vec3f(0,0,0);
    _direction = createRandomDirection();
    _originalSize = 0.3f;
    _speed = createSpeed();
    _rot1speed = createRotation1();
    _rot2speed = createRotation2();
    _shakeAmplitude = createShakeAmplitude();
    _shakeRate = createShakeRate();
    _glimmerPhase = 0.f;
    _glimmerRate = 0.f;
}

vmml::mat4f Atom::getModelMatrix(float time) {
    vmml::vec3f position = getRelativePosition(time);
    return vmml::create_translation(position)
            * vmml::create_rotation(_rot1speed, _rot1)
            * vmml::create_scaling(1.f/100.f * position.z() * _originalSize);
}

vmml::mat4f Atom::getModelMatrix(float time, vmml::mat4f steeringMatrix) {
    return steeringMatrix * getModelMatrix(time);
}

vmml::mat4f Atom::getModelMatrixWoRot(float time) {
    vmml::vec3f position = getRelativePosition(time);
    return vmml::create_translation(position)
    *vmml::create_rotation(2.f, vmml::normalize(vmml::vec3f(1.f,1.f,1.f)))
    * vmml::create_scaling(1.f/100.f * position.z() * _originalSize);
}

vmml::mat4f Atom::getModelMatrixWoRot(float time, vmml::mat4f steeringMatrix) {
    return steeringMatrix * getModelMatrixWoRot(time);
}

vmml::vec3f Atom::getRelativePosition(float time) {
    return vmml::create_translation(_direction*(time-_birthTime)*_speed)
            //* vmml::create_translation(_shakeDirection*_shakeAmplitude*sin(time*_shakeRate))
            * vmml::create_translation(_center)
            * vmml::create_scaling(1.f/80.f*(_direction*(time-_birthTime)*_speed).z())
            * vmml::create_rotation(time*_rot2speed, _rot2)
            * vmml::create_translation(_shakeDirection*_shakeAmplitude*sin(time*_shakeRate))
            * vmml::create_translation(-_center)
            * (_center+_radius);
}

vmml::vec3f Atom::getAbsoultPosition(float time, vmml::mat4f steeringMatrix) {
    return steeringMatrix * getRelativePosition(time);
}

vmml::vec3f Atom::createRandomDirection() {
    float x = pow(-1.f, rand()%2) * (rand() % 201) / 100.f;
    float y = pow(-1.f, rand()%2) * sqrt(4-(x*x));
    return vmml::normalize(vmml::vec3f(x, y, 80.f));
}

vmml::vec3f Atom::createRandomVector() {
    float x = pow(-1, rand()%2) * (rand()%100);
    float y = pow(-1, rand()%2) * (rand()%100);
    float z = pow(-1, rand()%2) * (rand()%100);
    return vmml::normalize(vmml::vec3f(x,y,z));
}

void Atom::setSpeedLimits(float min, float max) {
    _speedMin = min;
    _speedMax = max;
}

void Atom::setRotation1SpeedLimits(float min, float max) {
    _rotation1Max = max;
    _rotation1Min = min;
}

void Atom::setRotation2SpeedLimits(float min, float max) {
    _rotation2Max = max;
    _rotation2Min = min;
}

void Atom::setShakeAmplitudeLimits(float min, float max) {
    _shakeAmplitudeMax = max;
    _shakeAmplitudeMin = min;
}

void Atom::setShakeRateLimits(float min, float max) {
    _shakeRateMax = max;
    _shakeRateMin = min;
}

void Atom::setParticleType(std::string type) { _particleType = type; }
std::string Atom::getParticleType() { return _particleType; }

void Atom::setBirthTime(float time) { _birthTime = time; }
float Atom::getBirthTime() { return _birthTime; }

void Atom::setCenter(vmml::vec3f center) { _center = center; }
vmml::vec3f Atom::getCenter() { return _center; }

void Atom::setRadius(vmml::vec3f radius) { _radius = radius; }
vmml::vec3f Atom::getRadius() { return _radius; }

void Atom::setRot1(vmml::vec3f rot1) { _rot1 = rot1; }
vmml::vec3f Atom::getRot1() { return _rot1; }

void Atom::setRot2(vmml::vec3f rot2) { _rot2 = rot2; }
vmml::vec3f Atom::getRot2() { return _rot2; }

void Atom::setShakeDirection(vmml::vec3f shakeDirection) { _shakeDirection = shakeDirection; }
vmml::vec3f Atom::getShakeDirection() { return _shakeDirection; }

void Atom::setDirection(vmml::vec3f direction) { _direction = direction; }
vmml::vec3f Atom::getDirection() { return _direction; }

void Atom::setOriginalSize(float originalSize) { _originalSize = originalSize; }
float Atom::getOriginalSize() { return _originalSize; }

void Atom::setSpeed(float speed) { _speed = speed; }
float Atom::getSpeed() { return _speed; }

void Atom::setRot1Speed(float rot1Speed) { _rot1speed = rot1Speed; }
float Atom::getRot1Soeed() { return _rot1speed; }

void Atom::setRot2Speed(float rot2Speed) { _rot2speed = rot2Speed; }
float Atom::getRot2Speed() { return _rot2speed; }

void Atom::setSakeRate(float shakeRate) { _shakeRate = shakeRate; }
float Atom::getShakeRate() { return _shakeRate; }

void Atom::setShakeAmplitude(float shakeAmlitude) { _shakeAmplitude = shakeAmlitude; }
float Atom::getShakeAmplitude() { return _shakeAmplitude; }

void Atom::setGlimmerPhase(float glimmerPhase) { _glimmerPhase = glimmerPhase; }
float Atom::getGlimmerPhase() { return _glimmerPhase; }

void Atom::setGlimmerRate(float glimmerRate) { _glimmerRate = glimmerRate; }
float Atom::getGlimmerRate() { return _glimmerRate; }

float Atom::createSpeed()
{
    return _speedMin + (((float) rand()) / (float) RAND_MAX)*(_speedMax-_speedMin);
}

float Atom::createRotation1()
{
    return _rotation1Min + (((float) rand()) / (float) RAND_MAX)*(_rotation1Max-_rotation1Min);
}

float Atom::createRotation2()
{
    return _rotation2Min + (((float) rand()) / (float) RAND_MAX)*(_rotation2Max-_rotation2Min);
}

float Atom::createShakeAmplitude()
{
    return _shakeAmplitudeMin + (((float) rand()) / (float) RAND_MAX)*(_shakeAmplitudeMax-_shakeAmplitudeMin);
}

float Atom::createShakeRate() {
    return _shakeRateMin + (((float) rand()) / (float) RAND_MAX)*(_shakeRateMax-_shakeRateMin);
}
float Atom::getHaloSize(float time) {
    vmml::vec3f position = getRelativePosition(time);
    return 1.f/150.f * position.z() * _originalSize;
; }
