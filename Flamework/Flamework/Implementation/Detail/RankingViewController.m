//
//  RankingViewController.m
//  Flamework
//
//  Created by Eros Fricker on 24.05.15.
//
//

#import "RankingViewController.h"
#include "StartScreenViewController.h"

@interface RankingViewController ()

@end

@implementation RankingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([segue.identifier isEqualToString:@"returnToMainMenu"]) {
        StartScreenViewController *vc = [segue destinationViewController];
        vc.userReturnedToView = true;
        
    }
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}


@end
