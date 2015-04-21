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

#include <stack>

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
    
    void drawModel(const std::string &name, GLenum mode = GL_TRIANGLES);

    void pushModelMatrix();
    void popModelMatrix();
    void transformModelMatrix(const vmml::mat4f &t);
    
private:
    double _time;

    vmml::vec2f _scrolling;
    vmml::vec2f _lScrollPos;
    vmml::vec2f _scaling;
    vmml::vec2f _lScale;
    vmml::vec4f _eyePos;
    
    MatrixStack _modelMatrixStack;
    vmml::mat4f _modelMatrix;
    vmml::mat4f _viewMatrix;
};


#endif
