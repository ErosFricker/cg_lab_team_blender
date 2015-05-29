//
//  ParticleEngine.h
//  Flamework
//
//  Created by Eros Fricker on 20.05.15.
//
//

#ifndef __Flamework__ParticleEngine__
#define __Flamework__ParticleEngine__

#include <stdio.h>
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
#include "EmitterParticle.h"

class ParticleEngine : public Model {

    
private:

    //Returns the current color of particles produced by the fountain.
    vmml::vec3f curColor();
    //Returns the average velocity of particles produced by the fountain.
    vmml::vec3f curVelocity();
    
    //Alters p to be a particle newly produced by the fountain.
    void createParticle(EmitterParticle* p);
    
    //Advances the particle fountain by STEP_TIME seconds.
    void step();
    
    
public:
    explicit ParticleEngine(SceneManager *sceneManager, vmml::vec3f startPosition);
    ~ParticleEngine();
    
    void advance(float dt);
    
    virtual void draw(GLenum mode = GL_TRIANGLES, float deltaT = 0.0, bool boost = false);
};

typedef std::shared_ptr<ParticleEngine> ParticleEnginePtr;




#endif /* defined(__Flamework__ParticleEngine__) */
