//
//  Gyro.cpp
//  Framework
//
//  Created by Rahul Mukhi on 12/12/12.
//  Copyright (c) 2012 Rahul Mukhi. All rights reserved.
//  Reworked by David Steiner
//

#include "Gyro.h"

#include <iostream>
#include <memory>
#import <CoreMotion/CoreMotion.h>
#import <UIKit/UIKit.h>

static Gyro *instance = NULL;

@interface GyroImpl:CMMotionManager
{
    CMMotionManager* _motionManager;
    NSTimer* _timer;
    CMGyroData *_gyroData;
    
    double _pitch, _yaw, _roll;
}

- (double)getPitch;
- (double)getYaw;
- (double)getRoll;

@end

@implementation GyroImpl

-(id) init
{
    _motionManager = [[CMMotionManager alloc] init];
    _motionManager.deviceMotionUpdateInterval = 1.0f/60.0f;
    [_motionManager startDeviceMotionUpdates];
    _timer = [NSTimer scheduledTimerWithTimeInterval:(1.0f/60.0f) target:self selector:@selector(read) userInfo:nil repeats:YES];
    
    if([_motionManager isGyroAvailable]) {
        if(![_motionManager isGyroActive]) {
            [_motionManager setGyroUpdateInterval:0.1f];
            [_motionManager startGyroUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMGyroData *gyroData, NSError *error) {
                
            }];
        }
    }
    
    return self;
}

- (void)read
{
    CMAttitude *attitude;
    CMDeviceMotion *motion = _motionManager.deviceMotion;
    attitude = motion.attitude;
    
    _pitch = attitude.pitch;
    _roll = attitude.roll;
    _yaw = attitude.yaw;
    
}

- (double)getPitch
{
    return _pitch;
}

- (double)getYaw
{
    return _yaw;
}

- (double)getRoll
{
    return _roll;
}

@end

struct Gyro::Impl
{
    GyroImpl *wrapped;
};

Gyro *Gyro::getInstance()
{
    if (!instance)
    {
        instance = new Gyro();
    }
    return instance;
}

Gyro::Gyro()
: _impl(new Impl)
{
    _impl->wrapped = [[GyroImpl alloc] init];
}

Gyro::~Gyro()
{
    delete _impl;
}

void Gyro::read()
{
    [_impl->wrapped read];
}

float Gyro::getPitch()
{
    return [_impl->wrapped getPitch];
}

float Gyro::getYaw()
{
    return [_impl->wrapped getYaw];
}

float Gyro::getRoll()
{
    return [_impl->wrapped getRoll];
}


