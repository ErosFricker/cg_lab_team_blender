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

#include <boost/lexical_cast.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

using boost::lexical_cast;

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f


DemoSceneManager::DemoSceneManager(Application *application)
    : SceneManager(application)
    , _time(12)
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

    loadModel("pipe_model.obj", true, true);
    loadModel("ship.obj", true, true);
//    loadModel("guy.obj", true, true);
//    loadSound("test.mp3");
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

void DemoSceneManager::drawModel(const std::string &name, GLenum mode)
{
    Model::GroupMap &groups = getModel(name)->getGroups();
    for (auto i = groups.begin(); i != groups.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader.get())
        {
            vmml::mat4f m = vmml::mat4f::ZERO;
            
            
            float fieldOfViewDegrees = 33.0f;
            const float aspect = 3.0f/4.0f;
            const float zNear = 0.1f;
            const float zFar = 1000.0f;
            const float minimumFOV = 33.0f;
            const float maximumFOV = 360.0f;
            
            if (fieldOfViewDegrees < minimumFOV) {
                fieldOfViewDegrees = minimumFOV;
            }
            if (fieldOfViewDegrees > maximumFOV) {
                fieldOfViewDegrees = maximumFOV;
            }
            
         
            const float fieldOfView = fieldOfViewDegrees*(M_PI_F/360);
            
            
            const float f = 1 / tanf(fieldOfView/2.0f);
            
            m[0][0] = f / aspect;
            m[1][1] = f;
            m[2][2] = (zFar + zNear)/(zNear - zFar);
            m[2][3] = (2*zFar*zNear)/(zNear - zFar);
            m[3][2] = -1.0f;
            
         
            shader->setUniform("ProjectionMatrix", m);
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
        }
        else
        {
            util::log("No shader available.", util::LM_WARNING);
        }
        geometry.draw(mode);
    }
}

int firstTime = 1;
void DemoSceneManager::draw(double deltaT)
{
    if (firstTime==1){
        _eyePos = vmml::vec3f(0, 0.0, 100);
        firstTime = 0;
    }else{
        _eyePos.z() = _eyePos.z() - 0.4;
    }
    if (_eyePos.z() < 0.0f) {
    
        printf("EYEPOS NEGATIVE");
    }
    
    _time += deltaT;
    float angle = _time * .1;   // .1 radians per second
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    
    Gyro *gyro = Gyro::getInstance();
    gyro->read();
    
    vmml::mat4f translation = vmml::create_translation(vmml::vec3f(-.01, -.007, 20));
    vmml::mat4f translationPlayer = vmml::create_translation(vmml::vec3f(_eyePos.x(), _eyePos.y()-0.05, _eyePos.z()-1));
    vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(.2f));
    vmml::mat4f scalingPlayer = vmml::create_scaling(vmml::vec3f(.01f));
    
    vmml::mat3f rotation = vmml::create_rotation(gyro->getRoll() * -M_PI_F - .3f, vmml::vec3f::UNIT_Y) *
    vmml::create_rotation(gyro->getPitch() * -M_PI_F + .3f, vmml::vec3f::UNIT_X);
    vmml::vec3f eyeUp = vmml::vec3f::UP;
    _viewMatrix = lookAt(_eyePos, vmml::vec3f::ZERO, eyeUp);

    _modelMatrix = translationPlayer*scalingPlayer;
    drawModel("ship");
    
    _modelMatrix = translation * scaling;
    
    drawModel("pipe_model");
    
    for (int i = 0; i < 10; i++) {
        translation = vmml::create_translation(vmml::vec3f(-.01, -.007, 20+i*8));
        _modelMatrix = translation * scaling;
        drawModel("pipe_model");
    }
    
    
    

    
    
}
