//
//  ShoutViewController.m
//  base-project
//
//  Created by Sidney Maestre on 1/16/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import "ShoutViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"
#import "User.h"

@interface ShoutViewController ()

@end

@implementation ShoutViewController

@synthesize client = _client;
@synthesize shoutArray = _shoutArray;
@synthesize managedObjectContext = _managedObjectContext;

- (AppDelegate *)appDelegate {
    return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.client = [SMClient defaultClient];
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
}

-(void) viewDidAppear:(BOOL)animated {
    if([self.client isLoggedIn]) {
    
        UIBarButtonItem *logoutButton = [[UIBarButtonItem alloc] initWithTitle:@"Logout" style:UIBarButtonItemStylePlain target:self action:@selector(submitLogout:)];
        self.navigationItem.rightBarButtonItem = logoutButton;
    } else {
        UIBarButtonItem *loginButton = [[UIBarButtonItem alloc] initWithTitle:@"Login" style:UIBarButtonItemStylePlain target:self action:@selector(openLogin:)];
        self.navigationItem.rightBarButtonItem = loginButton;
    }
    
    UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithTitle:@"Refresh" style:UIBarButtonItemStylePlain target:self action:@selector(submitRefresh:)];
    self.navigationItem.leftBarButtonItem = refreshButton;
}

-(void) viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    if(self.shoutArray == nil)
    {
        [self loadTableData];
    }
}

-(IBAction)submitRefresh:(id)sender {
    [[self.appDelegate coreDataStore] purgeCacheOfObjectsWithEntityName:@"Shout"];
    sleep(1);
    [self loadTableData];
}

-(IBAction)submitLogout:(id)sender {
    [self.client logoutOnSuccess:^(NSDictionary *result) {
        self.navigationItem.rightBarButtonItem = nil;

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


- (void) loadTableData {
    
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Shout" inManagedObjectContext:self.managedObjectContext];
    
    [fetchRequest setEntity:entity];
    
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"createddate" ascending:NO];
    NSArray *sortDescriptors = [NSArray arrayWithObjects:sortDescriptor, nil];
    
    [fetchRequest setSortDescriptors:sortDescriptors];
 
    [self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
        self.shoutArray = results;
        [self.tableView reloadData];
    } onFailure:^(NSError *error) {
        NSLog(@"Error on Shout Fetch");
    }];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return  [self.shoutArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    __block UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier  forIndexPath:indexPath];
    
    
    NSManagedObject *object = [self.shoutArray objectAtIndex:indexPath.row];
    
    NSString *userOwner = [object valueForKey:@"sm_owner"];
    
    NSArray *myArray = [userOwner componentsSeparatedByString:@"/"];
    NSString *username = [myArray objectAtIndex:1];
    
    NSError *error = nil;
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"User" inManagedObjectContext:self.managedObjectContext];
    [fetchRequest setEntity:entity];
    
    NSPredicate *equalPredicate =[NSPredicate predicateWithFormat:@"username == %@", username];
    [fetchRequest setPredicate:equalPredicate];
    
    
    NSArray *users = [self.managedObjectContext executeFetchRequest:fetchRequest error:&error];
    
    User *userManagedObject = [users objectAtIndex:0];
    
    
    NSString *usernameLabel = [[NSString alloc]initWithFormat:@"@%@",username];
    cell.detailTextLabel.text = usernameLabel;
    cell.textLabel.text = [object valueForKey:@"desc"];
    cell.imageView.image = [UIImage imageNamed:@"Default-568h@2x.png"];

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
         
            NSURL *url = [NSURL URLWithString:userManagedObject.pic];
            NSData *imageData = [NSData dataWithContentsOfURL:url];
   
            UIImage *image  = nil;
            if (imageData)
                image = [[UIImage alloc] initWithData:imageData];
            
            if (image)
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
                    if (cell)
                    {
                        cell.textLabel.frame = CGRectMake(90, 10, 300, 30);
                        cell.detailTextLabel.frame = CGRectMake(90, 40, 300, 50);

                        cell.imageView.contentMode = UIViewContentModeScaleAspectFit;
                        [cell.imageView setFrame:CGRectMake(0,0,80,80)];
                        cell.imageView.image = image;
                    
                    }
                    
                });
            }

    });
    
    return cell;
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Navigation logic may go here. Create and push another view controller.
    /*
     ; *detailViewController = [[; alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
     [self.navigationController pushViewController:detailViewController animated:YES];
     */
}

@end
