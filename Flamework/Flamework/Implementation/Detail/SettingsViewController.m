//
//  SettingsViewController.m
//  Flamework
//
//  Created by Eros Fricker on 28.04.15.
//
//

#import "SettingsViewController.h"




@interface SettingsViewController ()
@property (weak, nonatomic) IBOutlet UILabel *titleLabel;
@property (weak, nonatomic) IBOutlet UILabel *backgroundMusicLabel;
@property (weak, nonatomic) IBOutlet UILabel *SFXLabel;

@property (weak, nonatomic) IBOutlet UIButton *saveButton;

@end

@implementation SettingsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIFont* titleFont = [UIFont fontWithName:@"Alexis Bullet Italic" size:100.0f];
    UIFont* buttonFont = [UIFont fontWithName:@"Alexis Laser Italic" size:50.0f];
    
    [_titleLabel setFont:titleFont];
    [_titleLabel setTextColor:[UIColor whiteColor]];
    [_backgroundMusicLabel setFont:buttonFont];
    [_backgroundMusicLabel setTextColor:[UIColor whiteColor]];
    [_SFXLabel setFont:buttonFont];
    [_SFXLabel setTextColor:[UIColor whiteColor]];
    [_sfxSlider setTintColor:[UIColor orangeColor]];
    [_backgroundMusicSlider setTintColor:[UIColor orangeColor]];
    
    [_saveButton.titleLabel setFont:buttonFont];
    [_saveButton setTintColor:[UIColor orangeColor]];
    
    
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)saveButtonPressed:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (IBAction)backgroundMusicSliderValueChanged:(id)sender {
    
    UISlider* slider = (UISlider*) sender;
    
    [self.musicDelegate musicSliderValueChanged:slider.value];
    
    
}
- (IBAction)sfxSliderValueChanged:(id)sender {
    UISlider* slider = (UISlider*) sender;
    
    [self.musicDelegate sfxSliderValueChanged:slider.value];
    
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
