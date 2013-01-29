//
//  SignupViewController.h
//  base-project
//
//  Created by Sidney Maestre on 1/18/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SMClient;

@interface SignupViewController : UIViewController <UITextFieldDelegate,UIImagePickerControllerDelegate,UINavigationControllerDelegate> {
       NSString* picData;
}

@property (strong, nonatomic) SMClient *client;
@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (weak, nonatomic) IBOutlet UITextField *usernameField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;

@property (retain, nonatomic) NSString *picData;

- (IBAction)cancelLogin:(id)sender;
- (IBAction)createUser:(id)sender;
- (IBAction)uploadPhoto:(id)sender;
@property (weak, nonatomic) IBOutlet UIImageView *profileImage;

@end
