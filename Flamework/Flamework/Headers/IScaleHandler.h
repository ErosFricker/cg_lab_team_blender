//
//  IScaleHandler.h
//  Framework
//
//  Created by David Steiner on 3/9/14.
//
//

#ifndef I_SCALE_HANDLER_H
#define I_SCALE_HANDLER_H

struct IScaleHandler
{
    virtual void onScaleBegan(float x, float y) = 0;
    virtual void onScaleMoved(float x, float y) = 0;
    virtual void onScaleEnded(float x, float yt) = 0;
};

#endif
