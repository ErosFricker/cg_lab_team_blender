//
//  StartScreenViewController.m
//  Flamework
//
//  Created by Eros Fricker on 22.04.15.
//
//

#import <AVFoundation/AVFoundation.h>
#import "StartScreenViewController.h"
#import "SettingsViewController.h"

@interface StartScreenViewController () <MusicDelegate>
@property (weak, nonatomic) IBOutlet UILabel *TitleTextLabel;
@property (weak, nonatomic) IBOutlet UIButton *startGameButton;
@property (weak, nonatomic) IBOutlet UIButton *settingsButton;
@property (weak, nonatomic) IBOutlet UIImageView *backgroundImageView;
@property(nonatomic, strong) AVAudioPlayer* backgroundMusicPlayer;

@end

@implementation StartScreenViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIFont* titleFont = [UIFont fontWithName:@"Alexis Bullet Italic" size:100.0f];
    UIFont* buttonFont = [UIFont fontWithName:@"Alexis Laser Italic" size:50.0f];
    
    [_TitleTextLabel setFont:titleFont];
    [_TitleTextLabel setTextColor:[UIColor orangeColor]];
    [_startGameButton.titleLabel setFont:buttonFont];
    [_startGameButton.titleLabel setTextColor:[UIColor whiteColor]];
    [_settingsButton.titleLabel setFont:buttonFont];
    [_settingsButton.titleLabel setTextColor:[UIColor whiteColor]];
    
    NSString* pathToSoundFile = [[NSBundle mainBundle] pathForResource:@"pampam" ofType:@"mp3"];
    NSError* error;
    NSData* data = [[NSData alloc] initWithContentsOfFile:pathToSoundFile];
    _backgroundMusicPlayer = [[AVAudioPlayer alloc] initWithData:data error:&error];
    _backgroundMusicPlayer.numberOfLoops = -1;
    _backgroundMusicPlayer.delegate = self;
    [_backgroundMusicPlayer play];
    
    // Do any additional setup after loading the view.
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if ([segue.identifier isEqualToString:@"settings"]) {
        SettingsViewController* vc = [segue destinationViewController];
        vc.musicDelegate = self;
        vc.backgroundMusicSlider.value = _backgroundMusicPlayer.volume;
    }
}

-(void)musicSliderValueChanged:(float)value{
    
    [_backgroundMusicPlayer setVolume:value];
    
}
-(void)sfxSliderValueChanged:(float)value{
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
