//
//  LoginViewController.m
//  base-project
//
//  Created by Sidney Maestre on 1/17/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import "LoginViewController.h"
#import "StackMob.h"

@interface LoginViewController ()

@end

@implementation LoginViewController

@synthesize client = _client;
@synthesize spinner = _spinner;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.usernameField setReturnKeyType:UIReturnKeyDone];
    [self.usernameField addTarget:self
                           action:@selector(textFieldFinished:)
                 forControlEvents:UIControlEventEditingDidEndOnExit];
    
    [self.passwordField setReturnKeyType:UIReturnKeyGo];
    [self.passwordField addTarget:self
                       action:@selector(textFieldFinished:)
             forControlEvents:UIControlEventEditingDidEndOnExit];
    
    [super viewDidLoad];
    
    self.client = [SMClient defaultClient];
}

- (void) viewDidAppear:(BOOL)animated {
    if([self.client isLoggedIn]) {
        [self dismissModalViewControllerAnimated:YES];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    if(textField == self.passwordField)
    {
        [self loginUser:nil];
    }
    [textField resignFirstResponder];
    return YES;
}

- (IBAction)cancelLogin:(id)sender {
    [self dismissModalViewControllerAnimated:YES];
}

- (IBAction)signupModal:(id)sender {
    UIStoryboard *mainStoryboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
    UIViewController *vc = [mainStoryboard instantiateViewControllerWithIdentifier:@"SignupViewController"];
    [self presentModalViewController:vc animated:YES];

}

- (IBAction)loginUser:(id)sender {
    
    [self.usernameField resignFirstResponder];
    [self.passwordField resignFirstResponder];
    
    [self.client loginWithUsername:self.usernameField.text password:self.passwordField.text
                         onSuccess:^(NSDictionary *results) {
        
                             [self.usernameField setText:@""];
                             [self.passwordField setText:@""];
                             [self dismissModalViewControllerAnimated:YES];
                         } onFailure:^(NSError *error) {
                             NSLog(@"Login Fail: %@",error);
                         }
     ];
}
- (void)viewDidUnload {
    [self setUsernameField:nil];
    [self setPasswordField:nil];
    [self setLoginButton:nil];
    [super viewDidUnload];
}
@end
