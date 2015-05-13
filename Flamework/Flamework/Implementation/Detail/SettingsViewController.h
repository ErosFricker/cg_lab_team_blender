//
//  SettingsViewController.h
//  Flamework
//
//  Created by Eros Fricker on 28.04.15.
//
//

#import <UIKit/UIKit.h>


@protocol MusicDelegate
@required - (void) musicSliderValueChanged: (float) value;
@required - (void) sfxSliderValueChanged: (float) value;


@end
@interface SettingsViewController : UIViewController

@property(nonatomic, strong) id<MusicDelegate> musicDelegate;
@property (weak, nonatomic) IBOutlet UISlider *backgroundMusicSlider;
@property (weak, nonatomic) IBOutlet UISlider *sfxSlider;

@end
