//
//  JustSayinViewController.h
//  base-project
//
//  Created by Sidney Maestre on 1/17/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SMClient;

@interface JustSayinViewController : UIViewController <UITextFieldDelegate, UITextViewDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (strong, nonatomic) SMClient *client;
@property (weak, nonatomic) IBOutlet UITextView *messageField;
@property (weak, nonatomic) IBOutlet UITextField *urlField;
@property (weak, nonatomic) IBOutlet UITextField *usernameField;

- (IBAction)postMessage:(id)sender;

@end
