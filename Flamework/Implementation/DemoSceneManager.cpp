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
#include "globals.h"
#include "ParticleEngine.h"

#include "Util.h"
#include <string>
#include <cmath>
#include <list>

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f
#define WINDOW_WIDTH    1024.0f // 768.f for iPad2
float steeringDirection = 0.0f;

ParticleEngine* engine;


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
                        * vmml::create_rotation(M_PI_F, vmml::vec3f(0,1,0))
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

vmml::mat4f DemoSceneManager::getScoreModelMatrix(vmml::vec4f position, int place, float scale) {
    return  vmml::create_translation(
                vmml::vec3f(position.x()-place*position.w(),position.y(),position.z())
            )
            * vmml::create_rotation(-M_PI_F/2.f, vmml::vec3f(1,0,0))
            * vmml::create_scaling(scale);
}

void DemoSceneManager::draw(double deltaT)
{
    _time += deltaT;
    
    // GAME CONFIGURATION
    if (_firstCall) {
        
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
        _particleSize = 0.4f;           // 0.2
        _particleSpawnProbability = 5;  // 20
        _maxParticleNumber = 15;        // 25
        _particleSpeedIncrement = 15;
        
        // Navigation
        steeringDirection = 0;
        _steeringSpeed = .0003f;
        _rotationValue = 0;
        _steeringRotation = 0.f;
        _gyroSpeed = 0.1;
        
        // Accelerator
        _textureSpeed = 0;
        _textureSpeedExp = 1.5f;
        _textureSpeedFac = 0.2f;
        
        // Ship
        _amplitudeVertical = 0.03f;
        _amplitudeHorizontal = 0.015f;
        _shakingfactorLeft = 24.f;
        _shakingfactorRight = 4.f;
        
        // Matrices
        createViewMatrix();
        createProjectionMatrix();
        createModelmatrixAccelerator(6.f); // scalingfactor
        createModelmatrixShip(vmml::vec3f(0, -2, 80), 0.2f); // position, scaling
        _gyroRotationMatrix = vmml::mat4f::IDENTITY;
        
        // Others
        createOrthonormalSystems();
    }
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    
    // UPDATE PARAMETERS
    
    // Tabletrotation (works but sucks)
    Gyro *gyro = Gyro::getInstance();
    gyro->read();
    _gyroRotationMatrix *= vmml::create_rotation(_gyroSpeed*gyro->getRoll(), vmml::vec3f::UNIT_Z);
    
    // Touch/Scroll
    _rotationValue += steeringDirection;
    
    // Current systemrotation
    _steeringRotation = /*_gyroRotationMatrix*/vmml::create_rotation(_rotationValue*_steeringSpeed,vmml::vec3f::UNIT_Z);
    
    // Other Parameters
    
    // Accelerator
    
    float eros_acceleration = 0.1f * _time;
    float accelerator_speed = fmodf((0.1*_time),0.496f);
    
    _modelMatrix = _steeringRotation * _modelMatrixAccelerator;
    _textureSpeed = fmodf(_textureSpeedFac*pow(_time,_textureSpeedExp),0.496f);
    drawModel(eros_acceleration, "accelerator");
    
    // Ship
    _shipModifierMatrix = getShipShakingMatrix(_time, _amplitudeVertical, _amplitudeHorizontal, _shakingfactorLeft, _shakingfactorRight);
    _modelMatrix =  _shipModifierMatrix * _modelMatrixShip;
    drawModel(0, "ship");
    
    
    // Generate new coreparticles
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
        _modelMatrix = _steeringRotation * (*it).getModelMatrix(_time);
        drawModel(0, "core");
        
        // Collisiondetection
        vmml::vec3f absolutePosition = _steeringRotation*(it->getPosition(_time)).getPosition();
        if(hasCollided(absolutePosition, _positionShip)) _collision = true;
        
        // Draw Halo
        vmml::mat4f tmp = _modelMatrix;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        float halo_scal = fabs(sin(it->getRandom1()*25.f*_time + it->getRandom2()));
        _modelMatrix *= vmml::create_rotation(M_PI_F/2.f, vmml::vec3f(1,0,0))
                        * vmml::create_scaling(1.3f+halo_scal);
        drawModel(0, "halo");
        glDisable(GL_BLEND);
        _modelMatrix = tmp;
    }
    
    // Remove passed particles
    std::list<CoreParticle>::const_iterator iterator;
    while (!(*(--_particleList.end())).getPosition(_time).isValid())
    {
        _particleList.pop_back();
        ++_particlesPassed;
        ++_score;
    }
    
    // TEST, draw black hole (blue sphere)
    if (_collision)
    {
        _modelMatrix = _modelMatrixShip * vmml::create_scaling(3.f);
        _color = vmml::vec4f(0,0,0.2,1);
        drawModel(0, "black_hole");
    }
    
    if(_particlesPassed >= 50)
    {
        _particleSpeedIncrement += _particleSpeedIncrement;
        _particlesPassed = 0;
        _particleSpawnProbability += 0;
        _maxParticleNumber += 0;
    }
    
    // Draw Points
    // Analize points
    /*
    std::string str = std::to_string(_points);
    std::string::iterator iter = str.begin();
    while (iter!=str.end()) {
        std::cout << (int) *(iter++);
    }std::cout << std::endl << _points << std::endl;
    */
    
    glDisable(GL_DEPTH_TEST);
    int length = std::to_string(_score).length();
    for ( int j=0; j<length; ++j) {
        int digit = (int) (_score % (unsigned int) pow(10, j+1) / pow(10, j));
        _modelMatrix = getScoreModelMatrix(vmml::vec4f(_x,_y,_z,_d), j, _s);
        switch (digit) {
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
        std::cout << digit;
    } std::cout << std::endl;
    glEnable(GL_DEPTH_TEST);
    
    
    
    
    //engine->draw();

    
    /*
    //Preparing Frame BUffer
    GLuint fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLuint colorTexture = 0;
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    GLsizei fboWidth = 768.0;
    GLsizei fboHeight = 1024.0;
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
    
    GLuint depthRBO = 0;
    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, fboWidth, fboHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        std::fprintf(stderr, "INCOMPLETE FRAMEBUFFER");
        std::abort();
        
    }
    
    /*
    ShaderPtr vertexShader = loadShader("blur.vert");
    ShaderPtr fragmentShader = loadShader("blur.frag");
    */
    
    
    /*
    //Binding framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fboWidth, fboHeight);
    
    GLuint program;
    //ATTACHING SHADERS
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &blurVS, 0);
    glCompileShader(vertexShader);
    glAttachShader(program, vertexShader);
    
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &blurFS, 0);
    glCompileShader(fragmentShader);
    glAttachShader(program, vertexShader);
    glLinkProgram(program);
    
   //SET UNIFORMS:  glUniform4fv(<#GLint location#>, <#GLsizei count#>, <#const GLfloat *v#>)
    
    glUseProgram(program);
    
    
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_WIDTH/2.0f);
    
    //RENDER HERE AGAIN FOR AFFECTING THE SCREEN
    */
    
    overallGameScore += 1;
}
