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
#include "CoreParticle.h"

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
    
    void createModelmatrixShip(vmml::vec3f position, float scaling);
    vmml::mat4f getShipShakingMatrix(float time, float amplitudeLeft, float amplituderight, float shakingfactorLeft, float shakingfactorRight);
    void createModelmatrixAccelerator(float scaling);
    void createProjectionMatrix();
    void createViewMatrix();

    vmml::mat4f getScoreModelMatrix(vmml::vec4f position, int place, float scale);
    
    vmml::vec3f distanceBetween(vmml::vec3f vec1, vmml::vec3f vec2);
    vmml::mat4f orientToViewer(vmml::vec3f eye, vmml::vec3f lookAt, vmml::vec3f point);
    vmml::mat4f haloMatrix(vmml::vec3f eye, vmml::vec3f lookAt, vmml::vec3f point, float originalSize);
    vmml::mat4f fakeScaling(vmml::vec3f position, float originalSize);
    bool hasCollided(vmml::vec3f vec1, vmml::vec3f vec2);
    
private:
    std::list<Particle> _activeParticles;
    std::list<CoreParticle> _particleList;
    
    double _time;
    float _rotationValue;
    bool _firstCall;
    bool _collision;
    int _particlesPassed;
    //float _speed;
    int _score;
    float _x, _y, _z, _s, _d;
    
    float _steeringSpeed;
    float _textureSpeed;
    float _textureSpeedExp;
    float _textureSpeedFac;
    
    vmml::mat4f _steeringRotation;
    vmml::mat4f _shipModifierMatrix;
    vmml::mat4f _gyroRotationMatrix;
    
    float _gyroSpeed;
    float _amplitudeVertical;
    float _amplitudeHorizontal;
    float _shakingfactorLeft;
    float _shakingfactorRight;
    
    float _particleSpeed;
    float _particleSize;
    float _particleSpawnProbability;
    float _maxParticleNumber;
    float _particleSpeedIncrement;

    vmml::vec2f _scrolling;
    vmml::vec2f _lScrollPos;
    vmml::vec2f _scaling;
    vmml::vec2f _lScale;
    vmml::vec4f _eyePos;
    
    vmml::mat4f _modelMatrix;
    vmml::mat4f _viewMatrix;
    vmml::mat4f _projectionMatrix;
    
    vmml::mat4f _modelMatrixAccelerator;
    vmml::mat4f _modelMatrixShip;
    vmml::vec3f _positionShip;
    
    vmml::vec4f _color;
    vmml::mat4f _acceleratorRotation;
    
    MatrixStack _modelMatrixStack;
    vmml::mat3f _orthonormalBases[NUMBER_OF_ORTHOGONALSYSTEMS];
};


#endif
