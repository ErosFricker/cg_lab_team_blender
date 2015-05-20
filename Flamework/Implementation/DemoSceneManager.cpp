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
#include "globals.h"
#include "ParticleEngine.h"

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

#define SCROLL_SPEED    0.002f
#define SCALE_SPEED     0.008f
#define WINDOW_WIDTH    1024.0f
float steeringDirection = 0.0f;

const float GRAVITY = 3.0f;
const int NUM_PARTICLES = 1000;
//The interval of time, in seconds, by which the particle engine periodically
//steps.
const float STEP_TIME = 0.01f;
//The length of the sides of the quadrilateral drawn for each particle.
const float PARTICLE_SIZE = 0.05f;

vmml::vec3f rotateParticle(vmml::vec3f v, vmml::vec3f axis, float degrees) {
    axis = axis.normalize();
    float radians = degrees * M_PI_F / 180;
    float s = sin(radians);
    float c = cos(radians);
    return v * c + axis * axis.dot(v) * (1 - c) + v.cross(axis) * s;
}

float randomFloat(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

//Returns the position of the particle, after rotating the camera.
vmml::vec3f adjParticlePos(vmml::vec3f pos) {
    return rotateParticle(pos, vmml::vec3f(1, 0, 0), -30);
}
bool compareParticles(EmitterParticle* particle1, EmitterParticle* particle2) {
    return adjParticlePos(particle1->pos)[2] <
    adjParticlePos(particle2->pos)[2];
}

class ParticleEngine {
    
    
private:
    //Rotates the vector by the indicated number of degrees about the specified axis
    
    GLuint textureId;
    EmitterParticle particles[NUM_PARTICLES];
    //The amount of time until the next call to step().
    float timeUntilNextStep;
    //The color of particles that the fountain is currently shooting.  0
    //indicates red, and when it reaches 1, it starts over at red again.  It
    //always lies between 0 and 1.
    float colorTime;
    
    //The angle at which the fountain is shooting particles, in radians.
    float angle;
    
    //Returns the current color of particles produced by the fountain.
    vmml::vec3f curColor() {
        vmml::vec3f color;
        if (colorTime < 0.166667f) {
            color = vmml::vec3f(1.0f, colorTime * 6, 0.0f);
        }
        else if (colorTime < 0.333333f) {
            color = vmml::vec3f((0.333333f - colorTime) * 6, 1.0f, 0.0f);
        }
        else if (colorTime < 0.5f) {
            color = vmml::vec3f(0.0f, 1.0f, (colorTime - 0.333333f) * 6);
        }
        else if (colorTime < 0.666667f) {
            color = vmml::vec3f(0.0f, (0.666667f - colorTime) * 6, 1.0f);
        }
        else if (colorTime < 0.833333f) {
            color = vmml::vec3f((colorTime - 0.666667f) * 6, 0.0f, 1.0f);
        }
        else {
            color = vmml::vec3f(1.0f, 0.0f, (1.0f - colorTime) * 6);
        }
        //Make sure each of the color's components range from 0 to 1
        for(int i = 0; i < 3; i++) {
            if (color[i] < 0) {
                color[i] = 0;
            }
            else if (color[i] > 1) {
                color[i] = 1;
            }
        }
        
        return color;
    }
    //Returns the average velocity of particles produced by the fountain.
    vmml::vec3f curVelocity() {
        return vmml::vec3f(2 * cos(angle), 2.0f, 2 * sin(angle));
    }
    
    //Alters p to be a particle newly produced by the fountain.
    void createParticle(EmitterParticle* p) {
        p->pos = vmml::vec3f(0, 0, 0);
        p->velocity = curVelocity() + vmml::vec3f(0.5f * randomFloat() - 0.25f,
                                                  0.5f * randomFloat() - 0.25f,
                                                  0.5f * randomFloat() - 0.25f);
        p->color = curColor();
        p->timeAlive = 0;
        p->lifespan = randomFloat() + 1;
    }
    
    //Advances the particle fountain by STEP_TIME seconds.
    void step() {
        colorTime += STEP_TIME / 10;
        while (colorTime >= 1) {
            colorTime -= 1;
        }
        
        angle += 0.5f * STEP_TIME;
        while (angle > 2 * M_PI_F) {
            angle -= 2 * M_PI_F;
        }
        for(int i = 0; i < NUM_PARTICLES; i++) {
            EmitterParticle* p = particles + i;
            
            p->pos += p->velocity * STEP_TIME;
            p->velocity += vmml::vec3f(0.0f, -GRAVITY * STEP_TIME, 0.0f);
            p->timeAlive += STEP_TIME;
            if (p->timeAlive > p->lifespan) {
                createParticle(p);
            }
        }
    }
    
public:
    ParticleEngine(GLuint textureId1) {
        textureId = textureId1;
        timeUntilNextStep = 0;
        colorTime = 0;
        angle = 0;
        for(int i = 0; i < NUM_PARTICLES; i++) {
            createParticle(particles + i);
        }
        for(int i = 0; i < 5 / STEP_TIME; i++) {
            step();
        }
    }
    
    //Advances the particle fountain by the specified amount of time.
    void advance(float dt) {
        while (dt > 0) {
            if (timeUntilNextStep < dt) {
                dt -= timeUntilNextStep;
                step();
                timeUntilNextStep = STEP_TIME;
            }
            else {
                timeUntilNextStep -= dt;
                dt = 0;
            }
        }
    }
    
    //Draws the particle fountain.
    void draw() {
        std::vector<EmitterParticle*> ps;
        for(int i = 0; i < NUM_PARTICLES; i++) {
            ps.push_back(particles + i);
        }
        sort(ps.begin(), ps.end(), compareParticles);
        
        
        
        /* GLfloat vertices[ps.size()*3*3];
         for(unsigned int i = 0; i < ps.size(); i++) {
         EmitterParticle* p = ps[i];
         glColorMask(p->color[0], p->color[1], p->color[2],
         (1 - p->timeAlive / p->lifespan));
         float size = PARTICLE_SIZE / 2;
         
         vmml::vec3f pos = adjParticlePos(p->pos);
         vertices[i] = pos[0]-size;
         vertices[i+1] = pos[1] - size;
         vertices[i+2] = pos[2];
         vertices[i+3] = pos[0]-size;
         vertices[i+4] = pos[1] +size;
         vertices[i+5] = pos[2];
         vertices[i+6] = pos[0]+size;
         vertices[i+7] = pos[1]+size;
         vertices[i+8] = pos[2];
         GLuint vertexBuffer;
         glGenBuffers(1, &vertexBuffer);
         glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
         
         
         
         }*/
        GLfloat vertices[9];
        vertices[0] = 20;
        vertices[1] = 0;
        vertices[2] = 90;
        vertices[3] = 0;
        vertices[4] = 20;
        vertices[5] = 90;
        vertices[6] = 20;
        vertices[7] = 20;
        vertices[8] = 90;
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        
        
        
        
        
        //DRAWING CODE!!!
        
        /*glBegin(GL_QUADS);
         for(unsigned int i = 0; i < ps.size(); i++) {
         Particle* p = ps[i];
         glColor4f(p->color[0], p->color[1], p->color[2],
         (1 - p->timeAlive / p->lifespan));
         float size = PARTICLE_SIZE / 2;
         
         vmml::vec3f pos = adjParticlePos(p->pos);
         
         glTexCoord2f(0, 0);
         glVertex3f(pos[0] - size, pos[1] - size, pos[2]);
         glTexCoord2f(0, 1);
         glVertex3f(pos[0] - size, pos[1] + size, pos[2]);
         glTexCoord2f(1, 1);
         glVertex3f(pos[0] + size, pos[1] + size, pos[2]);
         glTexCoord2f(1, 0);
         glVertex3f(pos[0] + size, pos[1] - size, pos[2]);
         }
         glEnd();*/
    }
};



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
    else{
        util::log("LEFT");
        steeringDirection = scrolling;
    }
    steeringDirection = -steeringDirection;
    
    
    //    getSound("test")->play();*/
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

ParticleEngine* engine;
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
    engine = new ParticleEngine(15);
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

void DemoSceneManager::createOrthonormalSystems()
{
    // Initialize a radom seed
    srand((unsigned int) time(NULL));
    
    // Compute orthonormal systems
    for (int i=0; i<NUMBER_OF_ORTHOGONALSYSTEMS; ++i) {
        
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


void DemoSceneManager::drawParticles(std::list<Particle> particleList, float particle_speed) {
    
}




vmml::vec3f position_ship(vmml::vec3f(0, -2, 80));
float steeringSpeed = -0.00009f;
void DemoSceneManager::draw(double deltaT)
{
    // Catch the first function call
    if (_firstCall)
    {
        _firstCall = false;
        _particlesPassed = 0;
        _collision = false;
        _speed = 1;
        
        // Set random seed
        srand((unsigned int) _time);
        createOrthonormalSystems();
        
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
        _time = 0;
        
        // Setup default modelmatrices
        _modelMatrixAccelerator =
        vmml::create_rotation(-M_PI_F/2, vmml::vec3f(1,0,0))
        * vmml::create_scaling(6.f);
        
        _modelMatrixShip =
        vmml::create_translation(position_ship)
        * vmml::create_rotation(M_PI_F, vmml::vec3f(0,1,0))
        * vmml::create_scaling(.2f);
    }
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    
    // Update timereference
    _time += deltaT;
    
    // Userinput (Steering onTouch): Update default accelerator-modelmatrix
    float acceleratorRotation = 0;
    if (steeringDirection < 0.0) {
        _rotationValue += steeringDirection;
        acceleratorRotation +=steeringDirection;
    }
    else if (steeringDirection > 0.0){
        _rotationValue += steeringDirection;
        acceleratorRotation += steeringDirection;
    }
    _acceleratorRotation = (vmml::mat4f) vmml::create_rotation(
                                                               acceleratorRotation*-M_PI_F*steeringSpeed, vmml::vec3f::UNIT_Z);
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
    float particle_speed = 0.3f*deltaT*_speed;
    float accelerator_speed = fmodf((0.1*_time),0.496f);
    
    // Accelerator
    _modelMatrix = _modelMatrixAccelerator;
    drawModel(accelerator_speed, "accelerator");
    
    // Ship
    _modelMatrix =  vmml::create_translation(vmml::vec3f(0.015 * sin(240*time + M_PI_F/2.f), 0.03*sin(40*time), 0)) * _modelMatrixShip;
    drawModel(0, "ship");
    
    // Particles
    
    // Generate new particles
    // If particlelist < x, generate new particle with probability p
    if (rand()%101 < 20 && _activeParticles.size() < 25) {
        Particle p;
        p.generateRandomParticle(_time + (double) rand());
        _activeParticles.push_front(p);
    }
    
    std::list<Particle> tmp;
    
    // Draw active particles
    std::list<Particle>::iterator it;
    for (it=_activeParticles.begin(); it != _activeParticles.end(); ++it) {
        // Draw the core
        _modelMatrix = vmml::create_rotation(_rotationValue*-M_PI_F*steeringSpeed, vmml::vec3f::UNIT_Z)
        * (*it).getModelMatrix(particle_speed, .2f);
        
        //COLLISION DETECTION
        vmml::vec3f distance = vmml::vec3f(_modelMatrix.at(0,3),_modelMatrix.at(1,3),_modelMatrix.at(2,3)) - position_ship;
        
        if( -0.6 <= distance.x() && distance.x() <= 0.6 &&
           -0.1 <= distance.y() && distance.y() <= 0.1 &&
           0 <= distance.z() && distance.z() <= 0.3) {
            //std::cout << "delta = " << delta << std::endl;
            std::cout << "x = " << distance.x() << std::endl;
            std::cout << "y = " << distance.y() << std::endl;
            std::cout << "z = " << distance.z() << std::endl;
            _collision = true;
        }
        
        else drawModel(0, "core");
        
        // draw halo effects
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        float halo_scal = sin(it->getRandom1()*25.f*_time + it->getRandom2());
        _modelMatrix *= vmml::create_rotation(M_PI_F/2.f, vmml::vec3f(1,0,0))*vmml::create_scaling(2.f+0.25f*halo_scal);
        drawModel(0, "halo");
        glDisable(GL_BLEND);
        
        // Draw electrons
        vmml::mat4f modelMatrix = _modelMatrix;
        vmml::mat4f _modelMatrixElectrons;
        for (int i=0; i<(*it).getNumberOfElectrons(); ++i) {
            float scal = (*it).getCurrentScalingFactor();
            int sys = (*it).getOrthonormalSystem();
            _modelMatrixElectrons =
            vmml::create_rotation(_rotationValue*-M_PI_F*steeringSpeed, vmml::vec3f::UNIT_Z)
            * vmml::create_translation((*it).getCurrentPosition())
            * vmml::create_translation(vmml::create_scaling(1.2f*scal)*vmml::create_rotation(30*time, _orthonormalBases[sys].get_row(i))
                                       * _orthonormalBases[sys].get_row((i+1)%3))
            * vmml::create_scaling(.5f*scal);
            std::cout << "SCALING " << 20*time << std::endl << std::endl;
            _modelMatrix = _modelMatrixElectrons;
            _color = YELLOW;
            drawModel(0, "electron");
        }
        _modelMatrix = modelMatrix;
    }
    
    // Remove inactive particles
    while ((*(--_activeParticles.end())).passed()){
        _activeParticles.pop_back();
        _particlesPassed++;
    }
    
    
    // TEST, draw black hole
    if (_collision) {
        _modelMatrix = _modelMatrixShip * vmml::create_scaling(5.f);
        _color = vmml::vec4f(0,0,0.2,1);
        drawModel(0, "black_hole");
    }
    
    if(_particlesPassed >= 50){
        _speed = _speed + 0.1;
        _particlesPassed = 0;
    }
    
    engine->draw();
    overallGameScore += 1;
}
