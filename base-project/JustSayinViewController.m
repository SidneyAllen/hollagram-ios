//
//  JustSayinViewController.m
//  base-project
//
//  Created by Sidney Maestre on 1/17/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import "JustSayinViewController.h"
#import "AppDelegate.h"
#import "Whisper.h"
#import "User.h"
#import "StackMob.h"

@interface JustSayinViewController ()

@end

@implementation JustSayinViewController

@synthesize client = _client;
@synthesize managedObjectContext = _managedObjectContext;

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
    
    self.messageField.delegate = self;
    self.urlField.delegate = self;
    self.usernameField.delegate = self;
    self.client = [SMClient defaultClient];
}


-(void) viewDidAppear:(BOOL)animated {
    if([self.client isLoggedIn]) {
        
        UIBarButtonItem *logoutButton = [[UIBarButtonItem alloc] initWithTitle:@"Logout" style:UIBarButtonItemStylePlain target:self action:@selector(submitLogout:)];
        self.navigationItem.rightBarButtonItem = logoutButton;
    } else {
        UIBarButtonItem *loginButton = [[UIBarButtonItem alloc] initWithTitle:@"Login" style:UIBarButtonItemStylePlain target:self action:@selector(openLogin:)];
        self.navigationItem.rightBarButtonItem = loginButton;
    }
}

-(IBAction)submitLogout:(id)sender {
    [self.client logoutOnSuccess:^(NSDictionary *result) {
        
        [[NSNotificationCenter defaultCenter] postNotificationName:@"logoutSuccess" object:self userInfo:nil];
        
        UIBarButtonItem *loginButton = [[UIBarButtonItem alloc] initWithTitle:@"Login" style:UIBarButtonItemStylePlain target:self action:@selector(openLogin:)];
        self.navigationItem.rightBarButtonItem = loginButton;
        
    } onFailure:^(NSError *error) {
        NSLog(@"Logout Fail: %@",error);
    }];
}

-(IBAction)openLogin:(id)sender {
    UIStoryboard *mainStoryboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
    UIViewController *vc = [mainStoryboard instantiateViewControllerWithIdentifier:@"LoginViewController"];
    [self presentModalViewController:vc animated:YES];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (BOOL) textView: (UITextView*) textView shouldChangeTextInRange: (NSRange) range
  replacementText: (NSString*) text
{
    if ([text isEqualToString:@"\n"]) {
        [textView resignFirstResponder];
        return NO;
    }
    return YES;
}

- (IBAction)postMessage:(id)sender {
    
    if([self.client isLoggedIn]) {
        
        int msgLength = [self.messageField.text length];
        
        if(!msgLength){
            
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Message"
                                                            message:@"You must enter a message."
                                                           delegate:nil
                                                  cancelButtonTitle:@"OK"
                                                  otherButtonTitles:nil];
            [alert show];
        } else {
            
            int textLength = [self.usernameField.text length];
            
            if(textLength){
                
                // GET CURRENT LOGGED IN USERNAME
                [self.client  getLoggedInUserOnSuccess:^(NSDictionary *result) {
                    
                    NSString *currentUsername = [result objectForKey:@"username"];
                    
                    // CREATE MY NEW WHISPER
                    Whisper *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Whisper" inManagedObjectContext:self.managedObjectContext];
                
                    [newManagedObject setValue:self.messageField.text forKey:@"desc"];
                    [newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
                    
                    // FETCH THE CURRENTLY Logged in USER
                    NSError *error = nil;
                    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];

                    NSEntityDescription *entity = [NSEntityDescription entityForName:@"User" inManagedObjectContext:self.managedObjectContext];
                    [fetchRequest setEntity:entity];
                    
                    NSPredicate *equalPredicate =[NSPredicate predicateWithFormat:@"username == %@", currentUsername];
                    [fetchRequest setPredicate:equalPredicate];
                    
                    error = nil;
                    NSArray *users = [self.managedObjectContext executeFetchRequest:fetchRequest error:&error];
             
                    User *loggedInUserManagedObject = [users objectAtIndex:0];
                    
                    // ADD RELATIONSHIP to Logged in User
                    [newManagedObject addUserObject:loggedInUserManagedObject];
                   
                    
                    // FETCH the USER you are whispering too.
                    NSFetchRequest *fetchRequest2 = [[NSFetchRequest alloc] init];
                    NSEntityDescription *entity2 = [NSEntityDescription entityForName:@"User" inManagedObjectContext:self.managedObjectContext];
                    [fetchRequest2 setEntity:entity2];
                    
                                NSPredicate *equalPredicate2 =[NSPredicate predicateWithFormat:@"username == %@", self.usernameField.text];
                    [fetchRequest2 setPredicate:equalPredicate2];
                    
                    
                    error = nil;
                    NSArray *users2 = [self.managedObjectContext executeFetchRequest:fetchRequest2 error:&error];
                    
                    // IF YOU FIND A USERNAME MATCH
                    if([users2 count] > 0) {
                        User *userManagedObject2 = [users2 objectAtIndex:0];
                        
                        // ADD RELATIONSHIP to USER you are whispering too
                        [newManagedObject addUserObject:userManagedObject2];
                    }
                    
                    // SAVE WHISPER and RELATIONSHIPS
                    [self.managedObjectContext saveOnSuccess:^{
                        [self.messageField setText:@""];
                        [self.urlField setText:@""];
                        [self.usernameField setText:@""];
                    } onFailure:^(NSError *error) {
                        NSLog(@"There was an error! %@", error);
                    }];
                    
                } onFailure:^(NSError *error) {
                    NSLog(@"No user found");
                }];
                
            } else {
                
                NSManagedObject *newManagedObject = [NSEntityDescription insertNewObjectForEntityForName:@"Shout" inManagedObjectContext:self.managedObjectContext];
                
                [newManagedObject setValue:self.messageField.text forKey:@"desc"];
                [newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
                
                [self.managedObjectContext saveOnSuccess:^{

                    [self.messageField setText:@""];
                    [self.urlField setText:@""];
                    [self.usernameField setText:@""];
                } onFailure:^(NSError *error) {
                    NSLog(@"There was an error! %@", error);

                }];
            }
        }
    
    } else {
        UIStoryboard *mainStoryboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
        UIViewController *vc = [mainStoryboard instantiateViewControllerWithIdentifier:@"LoginViewController"];
        [self presentModalViewController:vc animated:YES];
    }
}

- (void)viewDidUnload {
    [self setMessageField:nil];
    [self setUrlField:nil];
    [self setUsernameField:nil];
    [super viewDidUnload];
}
@end
