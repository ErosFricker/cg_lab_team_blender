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
    return rotateParticle(pos, vmml::vec3f(0, 0, 0), 0.1);
}
bool compareParticles(EmitterParticle* particle1, EmitterParticle* particle2) {
    return adjParticlePos(particle1->pos)[2] <
    adjParticlePos(particle2->pos)[2];
}
EmitterParticle particles[NUM_PARTICLES];
GLuint textureId;
//The amount of time until the next call to step().
float timeUntilNextStep;
//The color of particles that the fountain is currently shooting.  0
//indicates red, and when it reaches 1, it starts over at red again.  It
//always lies between 0 and 1.
float colorTime;

//The angle at which the fountain is shooting particles, in radians.
float angle;

const float STEP_TIME = 0.01f;
//The length of the sides of the quadrilateral drawn for each particle.
const float PARTICLE_SIZE = 0.2f;

const float GRAVITY = 3.0f;
vmml::vec3f STARTING_POSITION;

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
    float xVelocity = fmodf(randomFloat(), 0.4);
    if (fmodf(randomFloat(), 2.0) == 0) {
        xVelocity = -xVelocity;
    }
    float zVelocity = randomFloat();
    float yVelocity = fmodf(randomFloat(), -0.6f);
    return vmml::vec3f(xVelocity, -1.0f, zVelocity);
}


void ParticleEngine::createParticle(EmitterParticle* p) {
    //p->pos = vmml::vec3f(0.0, -.77, 99);
    p->pos = STARTING_POSITION;
    p->velocity = curVelocity() + vmml::vec3f(0.5f * randomFloat() - 0.25f,
                                              -0.1f,
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
    
  /*  angle += 0.1f * STEP_TIME;
    while (angle > 2 * M_PI_F) {
        angle -= 2 * M_PI_F;
    }*/
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
    /*v -1.000000 0.000000 1.000000
     v 1.000000 0.000000 1.000000
     v -1.000000 0.000000 -1.000000
     v 1.000000 0.000000 -1.000000
     vt 0.999900 0.000100
     vt 0.999900 0.999900
     vt 0.000100 0.999900
     vt 0.000100 0.000100*/
    
    for (int i = 0; i < NUM_PARTICLES; i++) {
        createParticle(particles + i);
    }
    for (int i = 0; i < 5/STEP_TIME; i++) {
        step();
    }
    //TODO: Add particles to array and to geometry
    for (int i = 0; i < NUM_PARTICLES; i++) {
        EmitterParticle p = particles[i];
        float zPos = 99.0;
        float size = PARTICLE_SIZE / 2.0f;
        
        Point3 pos = {p.pos[0]-size, p.pos[1]-size, zPos};
        Vector3 normal = {0.0, 0.0, 1.0f};
        
        float lifeTime = (1-p.timeAlive/p.lifespan);
        Vector3 tangent = {lifeTime, 1.0, 0.0};
        Vector3 bitangent = {1.0, 0.0, 0.0};
        TexCoord coord = {1.0, 0.0};
        Vertex v1 = {pos, normal, tangent, bitangent, coord};
        vertices.push_back(v1);
        
        Point3 pos2 = {p.pos[0]-size, p.pos[1]+size, p.pos[2]};
        TexCoord coord2 = {1.0, 1.0};
        Vertex v2 = {pos2, normal, tangent, bitangent, coord2};
        vertices.push_back(v2);
        
        
        Point3 pos3 = {p.pos[0]+size, p.pos[1]+size, zPos};
        TexCoord coord3 = {0.0, 1.0};
        Vertex v3 = {pos3, normal, tangent, bitangent, coord3};
        vertices.push_back(v3);
        Point3 pos4 = {p.pos[0]+size, p.pos[1]-size, zPos};
        TexCoord coord4 = {0.0, 0.0};
        Vertex v4 = {pos4, normal, tangent, bitangent, coord4};
        vertices.push_back(v4);
    }
    
    for (int i = 0; i < NUM_PARTICLES; i++) {
        if (i%4 == 0) {
            indices.push_back(i);
            indices.push_back(i+1);
            indices.push_back(i+2);
            indices.push_back(i);
            indices.push_back(i+2);
            indices.push_back(i+3);
        }
    }

    geometry.copyVertexData(vertices);
    geometry.copyIndexData(indices);
    
    geometry.initializeVertexBuffer();
    MaterialData md;
    md.textures["DiffuseMap"] = "fire_particle_3.png";
    MaterialPtr material = sceneManager->createMaterial("fire_particle", md);

    setMaterial(material);
    ShaderPtr shader = getMaterial()->getShader();
    shader->registerAttrib("TexCoord", 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
    shader->registerAttrib("Tangent", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent));


    //sceneManager->createModel("fire_particle", ModelData("fire_particle.obj"));
    
}

GLuint vertexBuffer;
GLuint indexBuffer;

ParticleEngine::~ParticleEngine(){
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);

}



void ParticleEngine::draw(GLenum mode, float deltaT) {
    advance(0.1);
    Geometry &geometry = getGroups()["geom"];
    std::vector<EmitterParticle*> ps;
    for (int i = 0;  i< NUM_PARTICLES; i++) {
        ps.push_back(particles+i);
    }
    std::sort(ps.begin(), ps.end(), compareParticles);
    
    GeometryData::VboVertices vertices;
    GeometryData::VboIndices indices;
    ShaderPtr shader = getMaterial()->getShader();
    for (int i = 0; i < NUM_PARTICLES; i++) {
        EmitterParticle p = particles[i];
        float zPos = 99.0;
        float size = PARTICLE_SIZE / 2.0f;
        
        Point3 pos = {p.pos[0]-size, p.pos[1]-size, zPos};
        Vector3 normal = {0.0, 0.0, 1.0f};
        float lifeTime = (1-p.timeAlive/p.lifespan);
        Vector3 tangent = {lifeTime, 1.0, 0.0};
        Vector3 bitangent = {1.0, 0.0, 0.0};
        TexCoord coord = {1.0, 0.0};
        Vertex v1 = {pos, normal, tangent, bitangent, coord};
        vertices.push_back(v1);
        
        Point3 pos2 = {p.pos[0]-size, p.pos[1]+size, p.pos[2]};
        TexCoord coord2 = {1.0, 1.0};
        Vertex v2 = {pos2, normal, tangent, bitangent, coord2};
        vertices.push_back(v2);
        
        
        Point3 pos3 = {p.pos[0]+size, p.pos[1]+size, zPos};
        TexCoord coord3 = {0.0, 1.0};
        Vertex v3 = {pos3, normal, tangent, bitangent, coord3};
        vertices.push_back(v3);
        Point3 pos4 = {p.pos[0]+size, p.pos[1]-size, zPos};
        TexCoord coord4 = {0.0, 0.0};
        Vertex v4 = {pos4, normal, tangent, bitangent, coord4};
        vertices.push_back(v4);
    }
    
    
    
    for (int i = 0; i < NUM_PARTICLES; i++) {
        if (i%4 == 0) {
            indices.push_back(i);
            indices.push_back(i+1);
            indices.push_back(i+2);
            indices.push_back(i);
            indices.push_back(i+2);
            indices.push_back(i+3);
        }
    }
    
    geometry.copyVertexData(vertices);
    geometry.copyIndexData(indices);
    vertexBuffer = geometry.getVertexBuffer();
    indexBuffer = geometry.getIndexBuffer();

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), geometry.getVertexData().get(), GL_STATIC_DRAW);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    Model::draw(mode);
    glDisable(GL_BLEND);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
 }
