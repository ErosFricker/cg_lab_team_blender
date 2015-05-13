//
//  FrameworkViewController.h
//  Framework
//
//  Created by David Steiner on 2/24/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#include "Framework_GL.h"

@interface FrameworkViewController : UIViewController
{
    EAGLContext *context;
    GLuint program;
    double timestamp;
    
    BOOL animating;
    NSInteger animationFrameInterval;
    CADisplayLink *__weak displayLink;
    
    int _touchIndex;
    int _cPosX;
    int _cPosY;
    int _lPosX;
    int _lPosY;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

- (void)startAnimation;
- (void)stopAnimation;



@end
