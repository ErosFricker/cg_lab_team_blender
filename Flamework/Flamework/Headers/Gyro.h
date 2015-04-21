//
//  Gyro.h
//  Framework
/*
    Initialize Gyro, get the pitch, roll and yaw when Ipad is rotated
*/
//  Created by Rahul Mukhi on 12/12/12.
//  Reworked by David Steiner
//

#ifndef FRAMEWORK_GYRO_H
#define FRAMEWORK_GYRO_H

#include <memory>

class Gyro
{
    
public:
    
    static Gyro *getInstance();
    
    ~Gyro();
    
    void    read();

    float   getPitch();
    float   getYaw();
    float   getRoll();

private:
    
    Gyro();

    struct Impl;
    Impl *_impl;
};

typedef std::shared_ptr<Gyro> GyroPtr;

#endif