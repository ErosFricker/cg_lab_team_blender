//
//  Atom.h
//  Flamework
//
//  Created by Bernard Schroffenegger on 24.05.15.
//
//

#ifndef __Flamework__Atom__
#define __Flamework__Atom__

#include <stdio.h>
#include "addendum.hpp"
#include <string>

class Atom {

public:
    
    Atom();
    Atom(float time);
    ~Atom();
    
    void createRandom(float time);
    
    vmml::mat4f getModelMatrix(float time);
    vmml::mat4f getModelMatrix(float time, vmml::mat4f steeringMatrix);
    vmml::vec3f getRelativePosition(float time);
    vmml::vec3f getAbsoultPosition(float time, vmml::mat4f steeringMatrix);
    
    vmml::vec3f createRandomDirection();
    vmml::vec3f createRandomVector();
    
    void setParticleType(std::string type);
    std::string getParticleType();
    void setBirthTime(float time);
    float getBirthTime();
    void setCenter(vmml::vec3f center);
    vmml::vec3f getCenter();
    void setRadius(vmml::vec3f radius);
    vmml::vec3f getRadius();
    void setRot1(vmml::vec3f rot1);
    vmml::vec3f getRot1();
    void setRot2(vmml::vec3f rot2);
    vmml::vec3f getRot2();
    void setDirection(vmml::vec3f direciton);
    vmml::vec3f getDirection();
    void setShakeDirection(vmml::vec3f shakeDirection);
    vmml::vec3f getShakeDirection();
    void setOriginalSize(float originalSize);
    float getOriginalSize();
    void setSpeed(float speed);
    float getSpeed();
    void setRot1Speed(float rot1Speed);
    float getRot1Soeed();
    void setRot2Speed(float rot2Speed);
    float getRot2Speed();
    void setSakeRate(float shakeRate);
    float getShakeRate();
    void setShakeAmplitude(float shakeAmplitude);
    float getShakeAmplitude();
    void setGlimmerPhase(float glimmerPhase);
    float getGlimmerPhase();
    void setGlimmerRate(float glimmerRate);
    float getGlimmerRate();
        
    static void setSpeedLimits(float min, float max);
    static void setRotation1SpeedLimits(float min, float max);
    static void setRotation2SpeedLimits(float min, float max);
    static void setShakeAmplitudeLimits(float min, float max);
    static void setShakeRateLimits(float min, float max);
    

private:
    std::string _particleType;
    float _birthTime;
    
    vmml::vec3f _center;
    vmml::vec3f _radius;
    vmml::vec3f _rot1;
    vmml::vec3f _rot2;
    vmml::vec3f _shakeDirection;
    
    vmml::vec3f _direction;
    
    float _originalSize;
    float _speed;
    float _rot1speed;
    float _rot2speed;
    
    float _shakeRate;
    float _shakeAmplitude;
    float _glimmerPhase;
    float _glimmerRate;
    
    static float _speedMax;
    static float _speedMin;
    
    static float _rotation1Max;
    static float _rotation1Min;
    
    static float _rotation2Max;
    static float _rotation2Min;
    
    static float _shakeAmplitudeMax;
    static float _shakeAmplitudeMin;
    
    static float _shakeRateMax;
    static float _shakeRateMin;
    
    float createSpeed();
    float createRotation1();
    float createRotation2();
    float createShakeAmplitude();
    float createShakeRate();
    
    static const unsigned int NUMBER_OF_TYPES;
    static const std::string TYPE[];
};

#endif
