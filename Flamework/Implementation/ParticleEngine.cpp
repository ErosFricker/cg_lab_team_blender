//
//  ParticleEngine.cpp
//  Flamework
//
//  Created by Eros Fricker on 20.05.15.
//
//

#include "ParticleEngine.h"
#include "ModelData.h"
#include "TextureData.h"
#include "SceneManager.h"
#include "Util.h"
#define NUM_PARTICLES 1000

EmitterParticle particles[NUM_PARTICLES];
GLuint textureId;
GLuint vertexBuffer;
GLuint indexBuffer;

float timeUntilNextStep;
float colorTime;
float angle;
const float STEP_TIME = 0.002f;
float PARTICLE_SIZE = 0.15f;
float GRAVITY = 0.15f;
vmml::vec3f STARTING_POSITION;

int DemoSceneManager::getScore(){
    return _score;
}


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

vmml::vec3f adjParticlePos(vmml::vec3f pos) {
    return rotateParticle(pos, vmml::vec3f(0, 0, 0), 0.1);
}
bool compareParticles(Vertex* particle1, Vertex* particle2) {
    return adjParticlePos(particle1->position.z) <
    adjParticlePos(particle2->position.z);
}

vmml::vec3f ParticleEngine::curColor() {
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

vmml::vec3f ParticleEngine::curVelocity() {
    float xVelocity = fmodf(randomFloat(), 0.08f);
    if (fmodf(randomFloat(), 2.0) == 0) {
        xVelocity = -xVelocity;
    }
    float zVelocity = randomFloat();
    // float yVelocity = fmodf(randomFloat(), -0.6f);
    return vmml::vec3f(xVelocity, -.3f, zVelocity);
}


void ParticleEngine::createParticle(EmitterParticle* p) {
    //p->pos = vmml::vec3f(0.0, -.77, 99);
    p->pos = STARTING_POSITION;
    //p->pos.y() -= fmodf(randomFloat(), 0.1);
    
    p->velocity = curVelocity() + vmml::vec3f(0.1f * randomFloat() - 0.025f,
                                              -0.05f,
                                              0.5f * randomFloat() - 0.25f);
    p->color = curColor();
    p->timeAlive = 0;
    p->lifespan = randomFloat();
}
void ParticleEngine::step() {
    colorTime += STEP_TIME / 10;
    while (colorTime >= 1) {
        colorTime -= 1;
    }
    EmitterParticle* particle;
    for(int i = 0; i < NUM_PARTICLES; i++) {
        
        particle = particles + i;
        particle->pos += particle->velocity*STEP_TIME;
        
        if (particle->timeAlive > particle->lifespan/2.0f) {
            particle->pos.x()=fmodf(particle->pos.x(), 0.0001f);
        }
        
        particle->velocity += vmml::vec3f(0.0f, -GRAVITY * STEP_TIME, 0.0f);
        particle->timeAlive += STEP_TIME;
        if (particle->timeAlive > particle->lifespan) {
            createParticle(particle);
        }
    }
}


void ParticleEngine::advance(float dt) {
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

ParticleEngine::ParticleEngine(SceneManager* sceneManager, vmml::vec3f startPosition) : Model(sceneManager, ModelData()) {
    Model::GroupMap &groups = getGroups();
    Geometry &geometry = groups["geom"];
    STARTING_POSITION = startPosition;
    
    //TODO: Change vertices for quads!
    GeometryData::VboVertices vertices;
    GeometryData::VboIndices indices;
    EmitterParticle* p;
    float zPos = 99.0;
    Vector3 normal = {0.0, 0.0, 1.0f};
    Vector3 bitangent = {1.0, 0.0, 0.0};
    float size = PARTICLE_SIZE / 2.0f;
    float lifeTime;
    for (int i = 0; i < NUM_PARTICLES; i++) {
        
        p = particles + i;
        createParticle(p);
        lifeTime = (1-p->timeAlive/p->lifespan);
        
        TexCoord coord = {1.0, 0.0};
        Vertex v = {{p->pos[0]-size, p->pos[1]-size, p->pos[2]}, normal,{lifeTime, 1.0, 0.0}, bitangent, coord};
        
        TexCoord coord2 = {1.0, 1.0};
        Vertex v2 = {{p->pos[0]-size, p->pos[1]+size, p->pos[2]}, normal,{lifeTime, 1.0, 0.0}, bitangent, coord2};
        
        
        
        TexCoord coord3 = {0.0, 1.0};
        Vertex v3 = {{p->pos[0]+size, p->pos[1]+size, p->pos[2]}, normal,{lifeTime, 1.0, 0.0}, bitangent, coord3 };
        
        TexCoord coord4 = {0.0, 0.0};
        Vertex v4 = {{p->pos[0]+size, p->pos[1]-size, p->pos[2]}, normal, {lifeTime, 1.0, 0.0}, bitangent, coord4};
        
        
        if (i%4 == 0) {
            indices.push_back(i);
            indices.push_back(i+1);
            indices.push_back(i+2);
            indices.push_back(i);
            indices.push_back(i+2);
            indices.push_back(i+3);
        }
        
        vertices.push_back(v);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v4);
        
    }
    for (int i = 0; i < 5/STEP_TIME; i++) {
        step();
    }
    
    geometry.copyVertexData(vertices);
    geometry.copyIndexData(indices);
    
    geometry.initializeVertexBuffer();
    MaterialData md;
    md.textures["DiffuseMap"] = "fire_particle.png";
    MaterialPtr material = sceneManager->createMaterial("fire_particle", md);
    
    setMaterial(material);
    ShaderPtr shader = getMaterial()->getShader();
    shader->registerAttrib("TexCoord", 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
    shader->registerAttrib("Tangent", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent));
    
}


ParticleEngine::~ParticleEngine(){
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
    
}



void ParticleEngine::draw(GLenum mode, float deltaT, bool boost) {
    if (boost) {
        GRAVITY = 3.0f;
        PARTICLE_SIZE = 0.25f;
    }else{
        GRAVITY = 0.15f;
        PARTICLE_SIZE = 0.15f;
    }
    advance(0.1);
    Geometry &geometry = getGroups()["geom"];
    Geometry::VertexDataPtr ptr = geometry.getVertexData();
    EmitterParticle* p;
    float lifeTime;
    float size = PARTICLE_SIZE/2.0f;
    for (int i = 0 ; i < NUM_PARTICLES; i+=4) {
        p = particles + i;
        lifeTime =(1-p->timeAlive/p->lifespan);
        Vertex* v = (ptr.get() + i);
        v->position.x = p->pos.x()-size;
        v->position.y = p->pos.y()-size;
        v->position.z = p->pos.z();
        v->tangent.x = lifeTime;
        v = (ptr.get() + i + 1);
        v->position.x = p->pos.x()-size;
        v->position.y = p->pos.y()+size;
        v->position.z = p->pos.z();
        v->tangent.x = lifeTime;
        v = (ptr.get() + i + 2);
        v->position.x = p->pos.x()+size;
        v->position.y = p->pos.y()+size;
        v->position.z = p->pos.z();
        v->tangent.x = lifeTime;

        
        v = (ptr.get() + i + 3);
        v->position.x = p->pos.x()+size;
        v->position.y = p->pos.y()-size;
        v->position.z = p->pos.z();
        v->tangent.x = lifeTime;
    }
    
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);
    Model::draw(GL_TRIANGLES);
    glDisable(GL_BLEND);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
}
