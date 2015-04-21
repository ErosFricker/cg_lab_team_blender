//
//  Sound.cpp
//  Framework
//
//  Created by Rahul Mukhi on 12/12/12.
//  Reworked by David Steiner
//

#include "Sound.h"
#include "Util.h"

#import <AVFoundation/AVFoundation.h>

struct Sound::Impl    { AVAudioPlayer *player; };

Sound::Sound(const std::string &soundFileName)
: _impl(new Sound::Impl)
{
    NSString *path = [NSString stringWithCString:util::getFilePath(soundFileName).c_str()
                                       encoding:[NSString defaultCStringEncoding]];
    NSURL *url = [NSURL URLWithString:path];    // FIXME: suboptimal
    
    NSError *error;
    _impl->player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];

    [_impl->player prepareToPlay];
}

Sound::~Sound()
{
    delete _impl;
}

void Sound::play()
{
    [_impl->player play];
}

void Sound::stop()
{
    [_impl->player stop];
}

void Sound::pause()
{
    [_impl->player pause];
}

void Sound::setVolume(float volume)
{
    if(volume>1.0f)
        volume = 1.0f;
    
    if(volume<0.0f)
        volume = 0.0f;
    
    _impl->player.volume = volume;
}