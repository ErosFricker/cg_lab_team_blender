//
//  FrameworkAppDelegate.h
//  Framework
//
//  Created by David Steiner on 2/24/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class FrameworkViewController;

@interface FrameworkAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    FrameworkViewController *viewController;
}

@property (nonatomic, strong) IBOutlet UIWindow *window;
@property (nonatomic, strong) IBOutlet FrameworkViewController *viewController;

@end

