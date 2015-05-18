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
#include "Util.h"
#include "Particle.h"
#include <cmath>
#include <list>

#include <boost/lexical_cast.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

using boost::lexical_cast;

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f
#define WINDOW_WIDTH    1024.0f
int steeringDirection = 0;

DemoSceneManager::DemoSceneManager(Application *application)
    : SceneManager(application)
    , _time(0)
    , _scaling(1, 1)
    , _scrolling(0, 0.25)
{
    
}

DemoSceneManager::~DemoSceneManager() {}

void DemoSceneManager::onTouchBegan(float x, float y)
{
    util::log("onTouchBegan");
    vmml::vec2f cScrollPos(x, y);
    _lScrollPos = cScrollPos;
    
    if (cScrollPos.x() > (WINDOW_WIDTH/2.0f)){
        
        util::log("RIGHT");
        steeringDirection = 2;
        
    }
    else{
        util::log("LEFT");
        steeringDirection = 1;
    }
    
    
//    getSound("test")->play();*/
}

void DemoSceneManager::onTouchMoved(float x, float y)
{
    util::log("onTouchMoved");
    vmml::vec2f cScrollPos(x, y);
    vmml::vec2f scrollDelta(cScrollPos - _lScrollPos);
    _scrolling += scrollDelta * SCROLL_SPEED;
    _lScrollPos = cScrollPos;
}

void DemoSceneManager::onTouchEnded(float x, float y, int tapCount)
{
    util::log("onTouchEnded");
    steeringDirection = 0;
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

void DemoSceneManager::initialize(size_t width, size_t height)
{
    getApplication()->addTouchHandler(this);
    getApplication()->addScaleHandler(this);

    _modelMatrixStack.push(vmml::mat4f::IDENTITY);

    loadModel("accelerator.obj", true, true);
    loadModel("ship.obj", true, true);
    loadModel("core.obj", true, true);
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
            
            shader->setUniform("LightPos", vmml::vec4f(0.01, 0.01, -4, 1.f));
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

int firstTime = 1;
std::list<Particle> activeParticles;
Particle p;
float rotationValue = 0.0f;

void DemoSceneManager::draw(double deltaT)
{
    
    // Update timereference
    _time += deltaT;
    
    // Catch the first function call
    if (firstTime==1){
        
        // Random seed
        srand((unsigned int) _time);
        
        // Define viewmatrix
        _eyePos = vmml::vec3f(0, 0, 100);
        vmml::vec3f eyeUp = vmml::vec3f::UP;
        _viewMatrix = lookAt(_eyePos, vmml::vec3f::ZERO, eyeUp);
        
        // Define projectionmatrix
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
        
        // Define timereferences
        deltaT = 0; // !!!
        firstTime = 0;
        _time = 0;
        
        // Set default color
        _color = vmml::vec4f(0,0,0,1);
        
        
        
        // Setup modelmatrices
        _modelMatrixAccelerator =
            vmml::create_rotation(-M_PI_F/2, vmml::vec3f(1,0,0))
            * vmml::create_scaling(6.f);
        
        _modelMatrixShip =
            vmml::create_translation(vmml::vec3f(0, -2, 80))
            * vmml::create_rotation(M_PI_F, vmml::vec3f(0,1,0))
            * vmml::create_scaling(.2f);
        
        
        
    }
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    
    if (steeringDirection == 1) {
        rotationValue +=0.1;
    }else if (steeringDirection == 2){
        rotationValue -=0.1;
    }
        _acceleratorRotation = (vmml::mat4f)vmml::create_rotation(rotationValue*-M_PI_F*-.5f, vmml::vec3f::UNIT_Z);
        _modelMatrixAccelerator = _acceleratorRotation*_modelMatrixAccelerator;
    
    
    
    
    /*
    // Userinput - tablet rotation
    Gyro *gyro = Gyro::getInstance();
    gyro->read();
    
    vmml::mat3f rotation =
        vmml::create_rotation(gyro->getRoll() * -M_PI_F - .3f, vmml::vec3f::UNIT_Y)
        * vmml::create_rotation(gyro->getPitch() * -M_PI_F + .3f, vmml::vec3f::UNIT_X);
    */
    
    // Define the "speed"
    float time = .05f * _time;
    
    // Accelerator
    _modelMatrix = _modelMatrixAccelerator;
    drawModel(time, "accelerator");

    // Ship
    _modelMatrix =  vmml::create_translation(vmml::vec3f(0.015 * sin(80*time + M_PI_F/2.f), 0.03*sin(40*time), 0)) * _modelMatrixShip;
    drawModel(0, "ship");
    
    // Particles
    
    // Generate new particles
    // If particlelist < x, generate new particle with probability p
    if (rand()%101 < 5 && activeParticles.size() < 10) {
        // Generate new seed
        Particle p;
        p.generateRandomParticle(_time + (double) rand());
        activeParticles.push_back(p);
    }
    
    // Draw active particles
    for (std::list<Particle>::iterator it=activeParticles.begin(); it != activeParticles.end(); ++it) {
        //if ((*it).passed()) continue;
        (*it).setRotation(_acceleratorRotation);
        _modelMatrix = (*it).getModelMatrix(0.4f * deltaT, .2f);
        drawModel(0, "core");
    }
    
    // Remove inactive particles
    while ((*activeParticles.begin()).passed()) activeParticles.pop_front();
    
}
