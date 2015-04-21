//
//  GL.h
//  Framework
//
//  Created by David Steiner on 3/4/13.
//
//

#ifndef FRAMEWORK_GL_H
#define FRAMEWORK_GL_H

#define USE_GL_ES2

#ifdef USE_GL_ES2
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif

#endif
