//
//  StartScreenViewController.m
//  Flamework
//
//  Created by Eros Fricker on 22.04.15.
//
//

#import <AVFoundation/AVFoundation.h>
#import "StartScreenViewController.h"

@interface StartScreenViewController ()
@property (weak, nonatomic) IBOutlet UILabel *TitleTextLabel;
@property (weak, nonatomic) IBOutlet UIButton *startGameButton;
@property (weak, nonatomic) IBOutlet UIButton *settingsButton;
@property (weak, nonatomic) IBOutlet UIImageView *backgroundImageView;
@property(nonatomic, strong) AVAudioPlayer* player;

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
    _player = [[AVAudioPlayer alloc] initWithData:data error:&error];
    _player.numberOfLoops = -1;
    _player.delegate = self;
    [_player play];
    
    // Do any additional setup after loading the view.
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
