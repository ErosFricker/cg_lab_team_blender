//
//  StartScreenViewController.h
//  Flamework
//
//  Created by Eros Fricker on 22.04.15.
//
//
#import <AVFoundation/AVFoundation.h>

#import <UIKit/UIKit.h>

@interface StartScreenViewController : UIViewController <AVAudioPlayerDelegate>
@property(nonatomic) bool userReturnedToView;

@end
