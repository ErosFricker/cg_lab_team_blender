//
//  Application.h
//  Framework
/*
    Main Application class.
    Implements gyro and sound class. (Touch to be done)
    Initializes physics and graphics manager class.
*/
//  Created by Rahul Mukhi on 12/5/12.
//  Reworked by David Steiner
//

#ifndef FRAMEWORK_APPLICATION_H
#define FRAMEWORK_APPLICATION_H

#include <iostream>
#include <memory>
#include <set>
#include <map>
#include "ITouchHandler.h"
#include "IScaleHandler.h"
#include "Sound.h"
#include "Gyro.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "vector.hpp"

class Application: public ITouchHandler
{
public:
    Application();
    virtual ~Application();
        
    void initialize(size_t width, size_t height);
    void update(double deltaT);
    void draw(double deltaT);
    void destroy();
    
    void addTouchHandler(ITouchHandler *arg);
    void addScaleHandler(IScaleHandler *arg);
    
    void onTouchBegan(float x, float y);
    void onTouchMoved(float x, float y);
    void onTouchEnded(float x, float y, int tapCount);
    
    void onScaleBegan(float x, float y);
    void onScaleMoved(float x, float y);
    void onScaleEnded(float x, float y);
    
private:
    SceneManager     *_sceneManager;
    PhysicsManager      *_physicsManager;
    
    std::map< std::string, SoundPtr >   _soundMap;
    std::set< ITouchHandler* >          _touchHandlers;
    std::set< IScaleHandler* >          _scaleHandlers;
    
    SoundPtr createSound(const std::string &soundFileName, const std::string &soundFileType);
};

#endif