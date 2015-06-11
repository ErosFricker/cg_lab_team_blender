//
//  Atom4.h
//  Flamework
//
//  Created by Bernard Schroffenegger on 25.05.15.
//
//

#ifndef __Flamework__Atom4__
#define __Flamework__Atom4__

#include "Atom.h"
#include <stdio.h>

class Atom4 {
    
public:
    
    Atom4();
    Atom4(float time);
    ~Atom4();
    
    Atom* getAtoms();
    
    static void setSpeedLimits(float min, float max);
    static void setRotation1SpeedLimits(float min, float max);
    static void setRotation2SpeedLimits(float min, float max);
    static void setShakeAmplitudeLimits(float min, float max);
    static void setShakeRateLimits(float min, float max);
    
private:
    
    const static unsigned int ATOMS = 4;
    Atom _atoms[ATOMS];
    
    void createRandom(float time);
    vmml::vec3f getRandomNormal(vmml::vec3f v);
    vmml::vec3f createRandomVector();
    vmml::vec3f createRandomDirection();
    
    float createSpeed();
    float createRotation1();
    float createRotation2();
    float createShakeAmplitude();
    float createShakeRate();
    
    static float _distance;
    
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
};

#endif
