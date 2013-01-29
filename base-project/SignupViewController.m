//
//  SignupViewController.m
//  base-project
//
//  Created by Sidney Maestre on 1/18/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import "SignupViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import "User.h"

@interface SignupViewController ()

@end

@implementation SignupViewController

@synthesize managedObjectContext = _managedObjectContext;
@synthesize usernameField = _usernameField;
@synthesize passwordField = _passwordField;
@synthesize client = _client;
@synthesize picData = _picData;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

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
    
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
    self.client = [SMClient defaultClient];
    self.usernameField.delegate = self;
    self.passwordField.delegate = self;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (IBAction)cancelLogin:(id)sender {
    [self dismissModalViewControllerAnimated:YES];
}

- (IBAction)createUser:(id)sender {
    
    NSManagedObjectContext *context = [[self.appDelegate coreDataStore] contextForCurrentThread];
    
    User *newUser = [[User alloc] initIntoManagedObjectContext:context];
     
    [newUser setValue:self.picData forKey:@"pic"];
    [newUser setValue:self.usernameField.text forKey:[newUser primaryKeyField]];
    [newUser setPassword:self.passwordField.text];
    
    [self.managedObjectContext saveOnSuccess:^{
        [self loginUser];

    } onFailure:^(NSError *error) {
        [self.managedObjectContext deleteObject:newUser];
        [newUser removePassword];
        NSLog(@"There was an error! %@", error);
    }];
    
}

- (IBAction)uploadPhoto:(id)sender {
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc]init];
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
        
        [imagePicker setSourceType:UIImagePickerControllerSourceTypeCamera];
    } else {
        
        [imagePicker setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
    }
    
    [imagePicker setDelegate:self];
    [self presentViewController:imagePicker animated:YES completion:nil];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    
    NSData *imageData = UIImageJPEGRepresentation(image, 0.7);
    
    // Convert the binary data to string to save on Amazon S3
    self.picData = [SMBinaryDataConversion stringForBinaryData:imageData name:@"someImage.jpg" contentType:@"image/jpg"];
    [self.profileImage setImage:image];
    
    [self dismissViewControllerAnimated:YES completion:nil];
}


- (void)loginUser {
    [self.client loginWithUsername:self.usernameField.text password:self.passwordField.text onSuccess:^(NSDictionary *results) {
    
        [self.usernameField setText:@""];
        [self.passwordField setText:@""];
        
        [self dismissModalViewControllerAnimated:YES];
    } onFailure:^(NSError *error) {
        NSLog(@"Login Fail: %@",error);
    }];
}

- (void)viewDidUnload {
    [self setUsernameField:nil];
    [self setPasswordField:nil];
    [self setProfileImage:nil];
    [super viewDidUnload];
}
@end
