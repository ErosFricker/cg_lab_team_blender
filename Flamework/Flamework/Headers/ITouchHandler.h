//
//  ITouchHandler.h
//  Framework
//
//  Created by David Steiner on 3/5/13.
//
//

#ifndef I_TOUCH_HANDLER_H
#define I_TOUCH_HANDLER_H

struct ITouchHandler
{
    virtual void onTouchBegan(float x, float y) = 0;
    virtual void onTouchMoved(float x, float y) = 0;
    virtual void onTouchEnded(float x, float y, int tapCount) = 0;
};

#endif
