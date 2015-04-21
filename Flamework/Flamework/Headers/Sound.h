//
//  Sound.h
//  Framework
/*
    Initialize, play and stop sound file.
*/
//  Created by Rahul Mukhi on 12/12/12.
//  Reworked by David Steiner
//

#ifndef FRAMEWORK_SOUND_H
#define FRAMEWORK_SOUND_H

#include <string>

class Sound
{
public:
    
    Sound(const std::string &soundFileName);
    ~Sound();
    
    void play();
    void stop();
    void pause();
    void setVolume(float volume);
    
private:
    struct Impl;
    Impl *_impl;
};

typedef std::shared_ptr<Sound> SoundPtr;

#endif