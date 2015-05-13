//
//  Shader.vsh
//  Framework
//
//  Created by David Steiner on 2/24/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;

varying vec4 colorVarying;

uniform float translate;

void main()
{
    gl_Position = position;
    gl_Position.y += sin(translate) / 2.0;

    colorVarying = color;
}
