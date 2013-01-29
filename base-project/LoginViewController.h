//
//  LoginViewController.h
//  base-project
//
//  Created by Sidney Maestre on 1/17/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SMClient;

@interface LoginViewController : UIViewController

@property (strong, nonatomic) SMClient *client;
@property (weak, nonatomic) IBOutlet UITextField *usernameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;
@property (weak, nonatomic) IBOutlet UIButton *loginButton;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *spinner;

- (IBAction)cancelLogin:(id)sender;
- (IBAction)signupModal:(id)sender;
- (IBAction)loginUser:(id)sender;

@end
