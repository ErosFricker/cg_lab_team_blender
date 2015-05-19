//
//  DemoSceneManager.h
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#ifndef FRAMEWORK_DEMO_SCENE_MANAGER_H
#define FRAMEWORK_DEMO_SCENE_MANAGER_H

#include "SceneManager.h"
#include "ITouchHandler.h"
#include "IScaleHandler.h"

#include "Particle.h"

#include <stack>
#include <list>

const int NUMBER_OF_ORTHOGONALSYSTEMS = 100;

const vmml::vec4f RED = vmml::vec4f(1,0,0,1);
const vmml::vec4f YELLOW = vmml::vec4f(1,1,0,1);
const vmml::vec4f GREEN = vmml::vec4f(0,1,0,1);
const vmml::vec4f AZUR = vmml::vec4f(0,1,1,1);
const vmml::vec4f BLUE = vmml::vec4f(0,0,1,1);

const vmml::vec4f WHITE = vmml::vec4f(1,1,1,1);
const vmml::vec4f BLACK = vmml::vec4f(0,0,0,1);

class Application;

class DemoSceneManager: public SceneManager, public ITouchHandler, public IScaleHandler
{
public:
    typedef std::stack< vmml::mat4f >   MatrixStack;

    DemoSceneManager(Application *application);
    virtual ~DemoSceneManager();
    
    virtual void onTouchBegan(float x, float y);
    virtual void onTouchMoved(float x, float y);
    virtual void onTouchEnded(float x, float y, int tapCount);
    
    virtual void onScaleBegan(float x, float y);
    virtual void onScaleMoved(float x, float y);
    virtual void onScaleEnded(float x, float y);
    
    virtual void initialize(size_t width, size_t height);
    virtual void draw(double deltaT);
    
    void drawModel(float deltaT, const std::string &name, GLenum mode = GL_TRIANGLES);

    void pushModelMatrix();
    void popModelMatrix();
    void transformModelMatrix(const vmml::mat4f &t);
    
    void createOrthonormalSystems();
    
private:
    std::list<Particle> _activeParticles;
    
    double _time;
    float _rotationValue;
    bool _firstCall;
    bool _collision;
    int _particlesPassed;
    float _speed;

    vmml::vec2f _scrolling;
    vmml::vec2f _lScrollPos;
    vmml::vec2f _scaling;
    vmml::vec2f _lScale;
    vmml::vec4f _eyePos;
    
    MatrixStack _modelMatrixStack;
    vmml::mat4f _modelMatrix;
    vmml::mat4f _viewMatrix;
    vmml::mat4f _projectionMatrix;
    
    vmml::mat4f _modelMatrixAccelerator;
    vmml::mat4f _modelMatrixShip;
    
    vmml::vec4f _color;
    vmml::mat4f _acceleratorRotation;
    
    vmml::mat3f _orthonormalBases[NUMBER_OF_ORTHOGONALSYSTEMS];
};


#endif
