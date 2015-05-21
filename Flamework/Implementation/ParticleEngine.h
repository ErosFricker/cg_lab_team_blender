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
#include "globals.h"
#include "EmitterParticle.h"

class ParticleEngine : public Model {
    
    
private:


    explicit ParticleEngine(SceneManager *sceneManager);

    
    
    //Returns the current color of particles produced by the fountain.
    vmml::vec3f curColor();
    //Returns the average velocity of particles produced by the fountain.
    vmml::vec3f curVelocity();
    
    //Alters p to be a particle newly produced by the fountain.
    void createParticle(EmitterParticle* p);
    
    //Advances the particle fountain by STEP_TIME seconds.
    void step();
    
    
public:
  /*  ParticleEngine(GLuint textureId1) {
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
    }*/
    
    //Advances the particle fountain by the specified amount of time.
    void advance(float dt);
    
    //Draws the particle fountain.
    void draw();
};

typedef std::shared_ptr<ParticleEngine> ParticleEnginePtr;




#endif /* defined(__Flamework__ParticleEngine__) */
