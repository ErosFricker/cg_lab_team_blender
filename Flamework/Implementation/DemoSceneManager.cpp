//
//  DemoSceneManager.cpp
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#include "DemoSceneManager.h"
#include "addendum.hpp"

#include "Application.h"
#include "TextureData.h"
#include "ModelData.h"
#include "ShaderData.h"
#include "Framework_GL.h"

#include "Particle.h"
#include "CoreParticle.h"
#include "ParticleEngine.h"

#include "Util.h"
#include <string>
#include <cmath>
#include <list>

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f
#define WINDOW_WIDTH    768.0f // 768.f for iPad2, 1024. f for normal
float steeringDirection = 0.0f;



DemoSceneManager::DemoSceneManager(Application *application)
: SceneManager(application)
, _time(0)
, _scaling(1, 1)
, _scrolling(0, 0.25)
, _color(vmml::vec4f(0,0,0,1))
, _rotationValue(0)
, _firstCall(true)
{
    
}

DemoSceneManager::~DemoSceneManager() {
}

void DemoSceneManager::onTouchBegan(float x, float y)
{
    util::log("onTouchBegan");
    vmml::vec2f cScrollPos(x, y);
    _lScrollPos = cScrollPos;
    float scrolling = cScrollPos.x() - (WINDOW_WIDTH/2.0f);
    
    if (scrolling > 0.0f){
        util::log("RIGHT");
        steeringDirection = scrolling;
    }
    else {
        util::log("LEFT");
        steeringDirection = scrolling;
    }
    
    steeringDirection = -steeringDirection;
    // getSound("test")->play();*/
}

void DemoSceneManager::onTouchMoved(float x, float y)
{
    util::log("onTouchMoved");
    vmml::vec2f cScrollPos(x, y);
    vmml::vec2f scrollDelta(cScrollPos - _lScrollPos);
    _scrolling += scrollDelta * SCROLL_SPEED;
    _lScrollPos = cScrollPos;
    
    steeringDirection = -(cScrollPos.x() - (WINDOW_WIDTH/2.0f));
}

void DemoSceneManager::onTouchEnded(float x, float y, int tapCount)
{
    util::log("onTouchEnded");
    steeringDirection = 0.0;
}

void DemoSceneManager::onScaleBegan(float x, float y)
{
    util::log("onScaleBegan");
    vmml::vec2f cScale(-x, y);
    _lScale = cScale;
}

void DemoSceneManager::onScaleMoved(float x, float y)
{
    util::log("onScaleModev");
    vmml::vec2f cScale(-x, y);
    vmml::vec2f scaleDelta(cScale - _lScale);
    _scaling += scaleDelta * SCALE_SPEED;
    _lScale = cScale;
}

void DemoSceneManager::onScaleEnded(float x, float y)
{
    vmml::vec2f cScale(-x, y);
    vmml::vec2f scaleDelta(cScale - _lScale);
    _scaling += scaleDelta * SCALE_SPEED;
    _lScale = cScale;
}
ParticleEngine* particleEngine;

void DemoSceneManager::initialize(size_t width, size_t height)
{
    getApplication()->addTouchHandler(this);
    getApplication()->addScaleHandler(this);
    
    _modelMatrixStack.push(vmml::mat4f::IDENTITY);
    
    loadModel("accelerator.obj", true, true);
    loadModel("ship.obj", true, true);
    loadModel("core.obj", true, true);
    loadModel("electron.obj", true, true);
    loadModel("black_hole.obj", true, true);
    loadModel("halo.obj", true, true);
    loadModel("fire_particle.obj", true, true);
    
    loadModel("zero.obj", true, true);
    loadModel("one.obj", true, true);
    loadModel("two.obj", true, true);
    loadModel("three.obj", true, true);
    loadModel("four.obj", true, true);
    loadModel("five.obj", true, true);
    loadModel("six.obj", true, true);
    loadModel("seven.obj", true, true);
    loadModel("eight.obj", true, true);
    loadModel("nine.obj", true, true);
    loadModel("black_hole.obj", true, true);
    
    //CREATE PARTICLE ENGINE
    ModelPtr model = getModel("fire_particle");
    if (!model) {
        SceneManager* sm = this;
        std::make_shared<ParticleEngine>(sm);
    }
    particleEngine = new ParticleEngine(this);
    
    
    
    
}

vmml::mat4f lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up)
{
    vmml::vec3f zaxis = vmml::normalize(eye - target);
    vmml::vec3f xaxis = vmml::normalize(vmml::cross<3>(up, zaxis));
    vmml::vec3f yaxis = vmml::cross<3>(zaxis, xaxis);
    
    vmml::mat4f view;
    view.set_row(0, vmml::vec4f(xaxis.x(), xaxis.y(), xaxis.z(), -vmml::dot(xaxis, eye)));
    view.set_row(1, vmml::vec4f(yaxis.x(), yaxis.y(), yaxis.z(), -vmml::dot(yaxis, eye)));
    view.set_row(2, vmml::vec4f(zaxis.x(), zaxis.y(), zaxis.z(), -vmml::dot(zaxis, eye)));
    view.set_row(3, vmml::vec4f(0, 0, 0, 1.0));
    
    return view;
}

void DemoSceneManager::drawModel(float deltaT, const std::string &name, GLenum mode)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get())
        {
            shader->setUniform("ProjectionMatrix", _projectionMatrix);
            shader->setUniform("ViewMatrix", _viewMatrix);
            shader->setUniform("ModelMatrix", _modelMatrix);
            
            vmml::mat3f normalMatrix;
            vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrix)), normalMatrix);
            shader->setUniform("NormalMatrix", normalMatrix);
            
            shader->setUniform("EyePos", _eyePos);
            
            shader->setUniform("LightPos", vmml::vec4f(0.01, 2.01, 75., 1.f));
            shader->setUniform("Light1", _lights[0]);
            shader->setUniform("Light2", _lights[1]);
            shader->setUniform("Light3", _lights[2]);
            shader->setUniform("Light4", _lights[3]);
            shader->setUniform("Light5", _lights[4]);
            shader->setUniform("Ia", vmml::vec3f(1.f));
            shader->setUniform("Id", vmml::vec3f(1.f));
            shader->setUniform("Is", vmml::vec3f(1.f));
            shader->setUniform("deltaT", deltaT);
            
            shader->setUniform("ColorVector", _color);
        }
        else
        {
            util::log("No shader available.", util::LM_WARNING);
        }
        geometry.draw(mode);
    }
}

void DemoSceneManager::createOrthonormalSystems()
{
    // Initialize a radom seed
    srand((unsigned int) time(NULL));
    
    // Compute orthonormal systems
    for (int i=0; i<NUMBER_OF_ORTHOGONALSYSTEMS; ++i)
    {
        // Create 3 random vectors
        int random[9];
        for (int i=0; i<9; ++i) random[i] = (int) pow(-1.f,rand()%2) * rand()%100;
        vmml::vec3f v1(random[0], random[1], random[2]),
        v2(random[3], random[4], random[5]),
        v3(random[6], random[7], random[8]);
        
        // Orthonormalize this vectors by gram-schmidt
        vmml::vec3f x = vmml::normalize(v1);
        vmml::vec3f y = vmml::normalize(v2 - vmml::dot(v2, x) * x);
        vmml::vec3f z = vmml::normalize(v3 - vmml::dot(v3, x) * x - vmml::dot(v3, y) * y);
        
        // Save the basis vectors as rowvectors of a a 3x3-Matrix
        vmml::mat3f orthonormalMatrix;
        orthonormalMatrix.set_row(0, x);
        orthonormalMatrix.set_row(1, y);
        orthonormalMatrix.set_row(2, z);
        
        // Save the computed orthogonalmatrix
        _orthonormalBases[i] = orthonormalMatrix;
    }
}

void DemoSceneManager::createModelmatrixShip(vmml::vec3f position, float scaling)
{
    _modelMatrixShip = vmml::create_translation(position)
    * vmml::create_rotation(M_PI_F, vmml::vec3f(0.,1.,0.2))
    * vmml::create_scaling(scaling);
    
    _positionShip = position;
}

void DemoSceneManager::createModelmatrixAccelerator(float scaling)
{
    _modelMatrixAccelerator = vmml::create_rotation(-M_PI_F/2, vmml::vec3f(1,0,0))
    * vmml::create_scaling(scaling);
}

void DemoSceneManager::createProjectionMatrix()
{
    _projectionMatrix = vmml::mat4f::ZERO;
    
    float fieldOfViewDegrees = 33.0f;
    const float aspect = 3.0f/4.0f;
    const float zNear = 0.1f;
    const float zFar = 1000.0f;
    const float minimumFOV = 33.0f;
    const float maximumFOV = 360.0f;
    
    if (fieldOfViewDegrees < minimumFOV) fieldOfViewDegrees = minimumFOV;
    if (fieldOfViewDegrees > maximumFOV) fieldOfViewDegrees = maximumFOV;
    
    const float fieldOfView = fieldOfViewDegrees*(M_PI_F/360);
    const float f = 1 / tanf(fieldOfView/2.0f);
    
    _projectionMatrix[0][0] = f / aspect;
    _projectionMatrix[1][1] = f;
    _projectionMatrix[2][2] = (zFar + zNear)/(zNear - zFar);
    _projectionMatrix[2][3] = (2*zFar*zNear)/(zNear - zFar);
    _projectionMatrix[3][2] = -1.0f;
}

void DemoSceneManager::createViewMatrix()
{
    _eyePos = vmml::vec3f(0, 0, 100);
    _viewMatrix = lookAt(_eyePos, vmml::vec3f::ZERO, vmml::vec3f::UP);
}

vmml::mat4f DemoSceneManager::getShipShakingMatrix(float time, float amplitudeLeft, float amplituderight,float shakingfactorLeft, float shakingfactorRight)
{
    return  vmml::create_translation
    (
     vmml::vec3f
     (
      amplituderight * sin(shakingfactorRight*time + M_PI_F/2.f),
      amplitudeLeft*sin(shakingfactorLeft*time),
      0
      )
     );
}

bool DemoSceneManager::hasCollided(vmml::vec3f vec1, vmml::vec3f vec2) {
    
    vmml::vec3f distanceVector = -vec1 + vec2;
    
    if (-0.6 <= distanceVector.x() && distanceVector.x() <= 0.6 &&
        -0.1 <= distanceVector.y() && distanceVector.y() <= 0.1 &&
        -0.0 <= distanceVector.z() && distanceVector.z() <= 0.3)
    {
        return true;
    }
    
    else return false;
}

vmml::mat4f DemoSceneManager::orientToViewer(vmml::vec3f eye, vmml::vec3f lookAt, vmml::vec3f point)
{
    vmml::vec3f v1 = -eye+lookAt;
    vmml::vec3f v2 = -eye+point;
    
    float alpha = std::acos(vmml::dot(v1,v2)/sqrt(vmml::dot(v1,v1))/sqrt(vmml::dot(v2,v2)));
    
    return vmml::create_rotation(alpha, v1.cross(v2));
}

vmml::mat4f DemoSceneManager::fakeScaling(vmml::vec3f position, float originalSize)
{
    return vmml::create_scaling(1/100.f * position.z() * originalSize);
}

vmml::mat4f DemoSceneManager::haloMatrix(vmml::vec3f eye, vmml::vec3f lookAt, vmml::vec3f point, float originalSize) {
    
    return vmml::create_translation(point)
    * orientToViewer(eye, lookAt, point)
    * fakeScaling(point, originalSize);
}

vmml::mat4f DemoSceneManager::getHaloModelMatrix(float size, float scaling) {
    float s = 3.5*size + scaling;
    return vmml::create_rotation(M_PI_F/2.f, vmml::vec3f(1,0,0))
    * vmml::create_scaling(s);
}

vmml::mat4f DemoSceneManager::getScoreModelMatrix(vmml::vec4f position, int place, float scale) {
    return  vmml::create_translation(
                                     vmml::vec3f(position.x()-place*position.w(),position.y(),position.z())
                                     )
    * vmml::create_rotation(-M_PI_F/2.f, vmml::vec3f(1,0,0))
    * vmml::create_scaling(scale);
}

void DemoSceneManager::checkLights(vmml::vec3f position){
    int delta = sqrt(position.x()*position.x() + position.y()*position.y() + position.z()*position.z());
    for(int i = 0; i < 5; i++){
        if (delta < _lightDistance[4]){
            
            int tmpDist = _lightDistance[4];
            vmml::vec4f tmpPos = _lights[4];
            _lightDistance[4] = delta;
            _lights[4] = vmml::vec4f(position, 1.f);
            
            for(int k = 3; k >= 0; k--){
                int tmpDist2 = _lightDistance[k];
                vmml::vec4f tmpPos2 = _lights[k];
                _lightDistance[k] = tmpDist;
                _lights[k] = tmpPos;
                tmpDist = tmpDist2;
                tmpPos = tmpPos2;
            }
        }
        else{
            for(int j = 0; j < 4; j++){
                
                if (delta < _lightDistance[j] && delta > _lightDistance[j+1]){
                    //_put light there and push back
                    int tmpDist = _lightDistance[j];
                    vmml::vec4f tmpPos = _lights[j];
                    _lightDistance[j] = delta;
                    _lights[j] = vmml::vec4f(position, 1.f);
                    
                    for(int k = 3; k >= 0; k--){
                        int tmpDist2 = _lightDistance[k];
                        vmml::vec4f tmpPos2 = _lights[k];
                        _lightDistance[k] = tmpDist;
                        _lights[k] = tmpPos;
                        tmpDist = tmpDist2;
                        tmpPos = tmpPos2;
                    }
                }
            }
        }
    }

}


float _explosionAnimationTimer;
vmml::mat4f _explosionmatrix;

float _particleAnimationTimer = 0.0f;

void DemoSceneManager::draw(double deltaT)
{
    _time += deltaT;
    
    
    
    
    // GAME CONFIGURATION
    // -----------------------------------------------------------------------
    if (_firstCall) {
        _explosionAnimationTimer = 1.0f;
        _particleAnimationTimer = 0.0f;
        // Time
        srand((unsigned int) _time);
        _firstCall = false;
        deltaT = 0;
        _time = 0;
        
        // Score
        _score = 0;
        _x = 5.f;  // (x,y,z): position of the first digit (lowerleft)
        _y = 6.4f;
        _z = 50.f;
        _s = .3f;   // scale for digits
        _d = .6f;   // distance between digits
        
        // Particles
        _collision = false;
        _particleSpeed = 30.f;
        _particleSize = 0.3f;           // 0.2
        _particleSpawnProbability = 10;  // 20
        _maxParticleNumber = 20;        // 25
        _particleSpeedIncrement = 10;
        
        for(int i = 0; i < 5; i++){
            _lights[i] = vmml::vec4f(0., 0., 0., 1.f);
            _lightDistance[i] = 1000;
        }
        
        // Navigation
        steeringDirection = 0;
        _steeringSpeed = .0003f;
        _rotationValue = 0;
        _gyroSpeed = 0.1;
        
        // Accelerator
        _textureSpeed = 0;
        _textureSpeedExp = 1.5f;
        _textureSpeedFac = 0.2f;
        
        // Ship
        _amplitudeVertical = 0.03f;
        _amplitudeHorizontal = 0.015f;
        _shakingVertical = 24.f;
        _shakingHorizontal = 4.f;
        
        // Matrices
        createViewMatrix();
        createProjectionMatrix();
        createModelmatrixAccelerator(6.f); // scalingfactor
        createModelmatrixShip(vmml::vec3f(0, -2, 80), 0.2f); // position, scaling
        _gyroMatrix = vmml::mat4f::IDENTITY;
        _explosionmatrix = vmml::mat4f::IDENTITY;
        // Others
        createOrthonormalSystems();
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    
    
    
    
    
    
    // STEERING
    
    /* Gyro, sucks
     Gyro *gyro = Gyro::getInstance();
     gyro->read();
     _gyroMatrix *= vmml::create_rotation(_gyroSpeed*gyro->getRoll(), vmml::vec3f::UNIT_Z);
     */
    
    

    
    
    // Touch/Scroll
    _rotationValue += steeringDirection;
    // Current systemrotation
    
    // Steering
    _steeringMatrix = vmml::create_rotation(_rotationValue*_steeringSpeed,vmml::vec3f::UNIT_Z);
    /*_gyroRotationMatrix;*/
    
    
    // ACCELERATOR
    
    // Texturespeed
    // float epileptically = 0.1f * _time;
    // float constant = fmodf((0.1*_time),0.496f);
    // float accelerated = fmodf(_textureSpeedFac*pow(_time,_textureSpeedExp),0.496f);
    
    _modelMatrix = _steeringMatrix * _modelMatrixAccelerator;
    if (_explosionAnimationTimer > 0.0f && _explosionAnimationTimer < 1.0f && _collision == true) {
        
        
        _modelMatrix= _modelMatrix*vmml::create_scaling(_explosionAnimationTimer);
        float rotationValue = (-M_PI)*_explosionAnimationTimer;
        _modelMatrix = _modelMatrix*vmml::create_rotation(rotationValue, vmml::vec3f::UNIT_Y);
        _modelMatrix = _modelMatrix*vmml::create_translation(vmml::vec3f(0.0, 0.0, _explosionAnimationTimer));
        _explosionmatrix = _modelMatrix;
        
        _explosionAnimationTimer -=0.1;
        
        
    }
    if (_collision && _explosionAnimationTimer < 0.0001) {
        
        loadSound("you_lose.mp3")->play();
        shouldStop = true;
        
    }
    drawModel(0.1f*_time, "accelerator");
    
    // PARTICLES
    
    // Particle generator
    if (rand()%101<_particleSpawnProbability && _particleList.size()<_maxParticleNumber)
    {
        CoreParticle p;
        p.create(_time, _particleSpeed, _particleSize, rand());
        _particleList.push_front(p);
    }
    
    // Draw Particles
    std::list<CoreParticle>::iterator it;
    for (it = _particleList.begin(); it != _particleList.end(); ++it)
    {
        // Draw Core
        _modelMatrix = _steeringMatrix * (*it).getModelMatrix(_time);
        if (_explosionAnimationTimer > 0.0f && _explosionAnimationTimer < 1.0f && _collision == true) {
            
            _modelMatrix= _modelMatrix*vmml::create_scaling(_explosionAnimationTimer);
            float rotationValue = (-M_PI)*_explosionAnimationTimer;
            _modelMatrix = _modelMatrix*vmml::create_rotation(rotationValue, vmml::vec3f::UNIT_Y);
            _explosionmatrix = _modelMatrix;
            
            _explosionAnimationTimer -=0.001;
            
            
        }
        if (_collision && _explosionAnimationTimer < 0.0001) {
            shouldStop = true;
        }
        
        //Check if the particle is one of the 5 closest for lighting
        vmml::vec3f position = it->getPosition(_time).getPosition();
        checkLights(position);
        
        drawModel(0, "core");
        
        // Collisiondetection
        vmml::vec3f absolutePosition = _steeringMatrix*(it->getPosition(_time)).getPosition();
        if(hasCollided(absolutePosition, _positionShip)) _collision = true;
        
        
        
        // Draw Halo
        vmml::mat4f tmp = _modelMatrix;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // transparancy
        float halo_scal = fabs(sin(it->getRandom1()*25.f*_time + it->getRandom2()));
        _modelMatrix *= getHaloModelMatrix(_particleSize, halo_scal);
        drawModel(0, "halo");
        glDisable(GL_BLEND);
        _modelMatrix = tmp;
    }
    
    // Remove passed particles, increase the speed where appropriate, and update the score
    std::list<CoreParticle>::const_iterator iterator;
    while (!(*(--_particleList.end())).getPosition(_time).isValid())
    {
        _particleList.pop_back();
        ++_particlesPassed;
        ++_score;
    }
    
    // SHIP
    
    // The Ship
    _shipModifierMatrix = getShipShakingMatrix(_time, _amplitudeVertical, _amplitudeHorizontal, _shakingVertical, _shakingHorizontal);
    
    _modelMatrix =  _shipModifierMatrix * _modelMatrixShip;
    
    if (_explosionAnimationTimer > 0.0f && _explosionAnimationTimer < 1.0f && _collision == true) {
        
        _modelMatrix= _modelMatrix*vmml::create_scaling(_explosionAnimationTimer);
        float rotationValue = (-M_PI)*_explosionAnimationTimer;
        _modelMatrix = _modelMatrix*vmml::create_rotation(rotationValue, vmml::vec3f::UNIT_Y);
        _explosionmatrix = _modelMatrix;
        
        _explosionAnimationTimer -=0.001;
        
        
    }
    if (_collision && _explosionAnimationTimer < 0.0001) {
        shouldStop = true;
    }
    
    
    if (steeringDirection<0.0f) {
        _modelMatrix*=vmml::create_rotation(0.4f, vmml::vec3f::UNIT_Z);
    }else if (steeringDirection > 0.0f){
        _modelMatrix*=vmml::create_rotation(-0.4f, vmml::vec3f::UNIT_Z);
    }
    
    drawModel(0, "ship");

    
    
    
    
    // BLACKHOLE (TEST, blue sphere)
    // -----------------------------------------------------------------------
    glDisable(GL_DEPTH_TEST);
    if (_collision)
    {
        loadSound("explosion.mp3")->play();
        //_modelMatrix = _modelMatrixShip * vmml::create_scaling(3.f);
        if(_explosionAnimationTimer ==1.0f){
            _explosionAnimationTimer = 0.95;
        }
        _color = vmml::vec4f(0,0,0.2,1);
        drawModel(0, "black_hole");
        _time = 1000.0f;
    }
    glEnable(GL_DEPTH_TEST);
    
    
    // DRAW THE SCORE
    // -----------------------------------------------------------------------
    glDisable(GL_DEPTH_TEST); // Score is always in front of everything else
    glEnable(GL_BLEND); // Transparency
    glBlendFunc(GL_ONE, GL_ONE);
    for (int j=0; j<std::to_string(_score).length(); ++j)
    {
        int digit = (int) (_score % (unsigned int) pow(10, j+1) / pow(10, j));
        _modelMatrix = getScoreModelMatrix(vmml::vec4f(_x,_y,_z,_d), j, _s);
        switch (digit)
        {
            case 0: drawModel(0, "zero"); break;
            case 1: drawModel(0, "one"); break;
            case 2: drawModel(0, "two"); break;
            case 3: drawModel(0, "three"); break;
            case 4: drawModel(0, "four"); break;
            case 5: drawModel(0, "five"); break;
            case 6: drawModel(0, "six"); break;
            case 7: drawModel(0, "seven"); break;
            case 8: drawModel(0, "eight"); break;
            case 9: drawModel(0, "nine"); break;
            default: break;
        }
    }
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
    
    // UPDATE PARAMETERS
    // -----------------------------------------------------------------------
    if(_particlesPassed >= 50)
    {
        loadSound("swoosh.mp3")->play();
        _particleAnimationTimer = 10.0f;

        _particleSpeed += _particleSpeedIncrement;
        _particlesPassed = 0;
        _particleSpawnProbability += 0;
        _maxParticleNumber += 0;
    }
    
    if (_particleAnimationTimer >0.0f) {
        
        
        // Motion Fire
        vmml::mat4f tmp;
        vmml::mat4f fireparticleMatrix = vmml::create_rotation(-M_PI_F/2.f, vmml::vec3f(1,0,0))
        * vmml::create_scaling(0.1f);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE); // transparancy
        float sign;
        for (int g=0; g<2; ++g)
        {
            if (g==0) sign = 1.f;
            else sign = -1.f;
            _modelMatrix = vmml::create_translation(vmml::vec3f(sign*.56f,-2.4f,81.f)) * fireparticleMatrix;
            tmp = _modelMatrix;
            
            for (int h=0; h<10; ++h)
            {
                for (int i=0; i<10; ++i)
                {
                    int j = 0;
                    float x = pow(-1.f, rand()%2) * (rand()%(50+h*10)) /100.f;
                    float y = pow(-1.f, rand()%2) * (rand()%(50+h*10)) /100.f;
                    _modelMatrix *= vmml::create_translation(vmml::vec3f(x+sign*0.1*h,j,y-0.4*h));;
                    drawModel(0, "fire_particle");
                    j += 0.01;
                    _modelMatrix = tmp;
                }
            }
        }
        glDisable(GL_BLEND);
        _particleAnimationTimer -=.5f;
    }
    
    
    
    
    /* //particle engine drawing code...doesn't show up in view !!!!!
     glEnable(GL_DEPTH_TEST);
     glDisable(GL_CULL_FACE);
     //glCullFace(GL_BACK);
     //glEnable(GL_CULL_FACE);
     //glDisable(GL_CULL_FACE);
     Gyro *gyro = Gyro::getInstance();
     gyro->read();
     vmml::mat4f translation = vmml::create_translation(vmml::vec3f(_scrolling.x()
     , -_scrolling.y(), 0));
     vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(.2f));
     vmml::mat3f rotation = vmml::create_rotation(gyro->getRoll() * -M_PI_F - .3f,
     vmml::vec3f::UNIT_Y) *
     vmml::create_rotation(gyro->getPitch() * -M_PI_F + .3f, vmml::vec3f::UNIT_X);
     _eyePos = rotation * vmml::vec3f(0, 0, 0.25);
     vmml::vec3f eyeUp = vmml::vec3f::UP;
     _viewMatrix = lookAt(_eyePos, vmml::vec3f::ZERO, rotation * eyeUp);
     _modelMatrix = translation * scaling;
     ShaderPtr shader = particleEngine->getMaterial()->getShader();
     shader->setUniform("ProjectionMatrix", vmml::mat4f::IDENTITY);
     shader->setUniform("ViewMatrix", _viewMatrix);
     shader->setUniform("ModelMatrix", _modelMatrix);
     vmml::mat3f normalMatrix;
     vmml::compute_inverse(vmml::transpose(vmml::mat3f(_modelMatrix)),
     normalMatrix);
     shader->setUniform("NormalMatrix", normalMatrix);
     shader->setUniform("EyePos", _eyePos);
     shader->setUniform("LightPos", vmml::vec4f(0.f, 0.f, 90.f, 1.f));
     shader->setUniform("Ia", vmml::vec3f(1.f));
     shader->setUniform("Id", vmml::vec3f(1.f));
     shader->setUniform("Is", vmml::vec3f(1.f));
     particleEngine->draw(GL_TRIANGLE_STRIP);
     // glEnable(GL_CULL_FACE);*/
    
}
