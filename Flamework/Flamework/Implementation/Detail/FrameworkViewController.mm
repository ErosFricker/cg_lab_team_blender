//
//  FrameworkViewController.mm
//  Framework
//
//  Created by David Steiner on 2/24/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "FrameworkViewController.h"
#import "EAGLView.h"
#include "Application.h"
#include "Framework_GL.h"
#include "Util.h"

// Uniform index.
enum
{
    UNIFORM_TRANSLATE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};

@interface FrameworkViewController () {
    // Application instance
    Application _application;
}
@property (nonatomic, strong) EAGLContext *context;
@property (nonatomic, weak) CADisplayLink *displayLink;

@end

@implementation FrameworkViewController

@synthesize animating, context, displayLink;

- (void)awakeFromNib
{
#ifdef USE_GL_ES2
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
#else
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
#endif
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
    
    animating = FALSE;
    animationFrameInterval = 1;
    timestamp = 0.0;
    self.displayLink = nil;
    
    
    // initiailize the application
    
    size_t width = [[UIScreen mainScreen] bounds].size.width;
    size_t height = [[UIScreen mainScreen] bounds].size.height;
    _application.initialize(width, height);
}

- (void)dealloc
{
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self.view];
    
    _lPosX = touchLocation.x;
    _lPosY = touchLocation.y;
    _application.onTouchBegan(touchLocation.x, touchLocation.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self.view];
    
    _lPosX = touchLocation.x;
    _lPosY = touchLocation.y;
    _application.onTouchEnded(touchLocation.x, touchLocation.y, 1);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePanGesture:)];
    // resizeGesture.minimumNumberOfTouches = 2;
    panGesture.delaysTouchesEnded = FALSE;
    [self.view addGestureRecognizer:panGesture];
    _cPosX = 0;
    _cPosY = 0;
    _lPosX = 0;
    _lPosY = 0;
    _touchIndex = 0;
}

-(IBAction)handlePanGesture:(UIPanGestureRecognizer *)recognizer
{
    int nTouches = [recognizer numberOfTouches];
    CGPoint touchLocation = [recognizer locationInView:self.view];
    // std::cout << "nTouches: " << nTouches << std::endl;
    switch (recognizer.state)
    {
        case UIGestureRecognizerStateBegan:
            if (nTouches == 1)
            {
                _lPosX = touchLocation.x;
                _lPosY = touchLocation.y;
                _application.onTouchBegan(touchLocation.x, touchLocation.y);
            }
            // _application.onScaleBegan(0, 0);
            break;
        case UIGestureRecognizerStateChanged:
            if (nTouches == 1)
            {
                if (_touchIndex == 0)
                {
                    _lPosX = touchLocation.x;
                    _lPosY = touchLocation.y;
                    _application.onTouchMoved(touchLocation.x, touchLocation.y);
                }
            }
            else if (nTouches == 2)
            {
                if (_touchIndex == 0)
                {
                    _cPosX = touchLocation.x;
                    _cPosY = touchLocation.y;
                }
                _lPosX = touchLocation.x;
                _lPosY = touchLocation.y;
                _application.onScaleMoved(_cPosX - _lPosX, _cPosY - _lPosY);
                ++_touchIndex;
            }
            break;
        case UIGestureRecognizerStateEnded:
            if (nTouches < 2)
            {
                _cPosX = touchLocation.x;
                _cPosY = touchLocation.y;
                // _application.onScaleEnded(_cPosX - _lPosX, _cPosY - _lPosY);
                _application.onTouchEnded(touchLocation.x, touchLocation.y, nTouches);
            }
            _touchIndex = 0;
            break;
            
        default:
            break;
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;
        
        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(update)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)update
{
    [(EAGLView *)self.view setFramebuffer];
    
    double currentTime = [displayLink timestamp];
	double deltaT = currentTime - timestamp;
	timestamp = currentTime;
    
    _application.update(deltaT);
    _application.draw(deltaT);
    
    [(EAGLView *)self.view presentFramebuffer];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotate
{
    return NO;
}

@end
