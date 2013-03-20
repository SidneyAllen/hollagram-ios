//
//  NoWhisperViewController.m
//  base-project
//
//  Created by Sidney Maestre on 1/16/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import "NoWhisperViewController.h"
#import "AppDelegate.h"
#import "StackMob.h"

@interface NoWhisperViewController ()

@end

@implementation NoWhisperViewController

@synthesize client = _client;
@synthesize whipserArray = _whipserArray;
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
    
    self.client = [SMClient defaultClient];
    self.managedObjectContext = [[self.appDelegate coreDataStore] contextForCurrentThread];
}


-(void) viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    [[NSNotificationCenter defaultCenter]
        addObserver:self
        selector:@selector(clearWhisperArray)
        name:@"logoutSuccess"
        object:nil];
    
    if([self.client isLoggedIn]) {
        if(self.whipserArray == nil)
        {
            [self loadTableData];
        }
    }
}

-(void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    if([self.client isLoggedIn]) {
        NSLog(@"Is logged in");
        [self.myView setHidden:YES];
        [self.myTableView setHidden:NO];
        
    } else {
        NSLog(@"Not logged in");
        [self.myView setHidden:NO];
        [self.myTableView setHidden:YES];
    }
    
    if([self.client isLoggedIn]) {
            
        UIBarButtonItem *logoutButton = [[UIBarButtonItem alloc] initWithTitle:@"Logout" style:UIBarButtonItemStylePlain target:self action:@selector(submitLogout:)];
        self.navigationItem.rightBarButtonItem = logoutButton;
        UIBarButtonItem *refreshButton = [[UIBarButtonItem alloc] initWithTitle:@"Refresh" style:UIBarButtonItemStylePlain target:self action:@selector(submitRefresh:)];
        self.navigationItem.leftBarButtonItem = refreshButton;
    } else {
        UIBarButtonItem *loginButton = [[UIBarButtonItem alloc] initWithTitle:@"Login" style:UIBarButtonItemStylePlain target:self action:@selector(openLogin:)];
        self.navigationItem.rightBarButtonItem = loginButton;
    }
}

-(IBAction)submitRefresh:(id)sender {
    [self loadTableData];
}

-(IBAction)openLogin:(id)sender {
    self.whipserArray = nil;
    
    UIStoryboard *mainStoryboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
    UIViewController *vc = [mainStoryboard instantiateViewControllerWithIdentifier:@"LoginViewController"];
    [self presentModalViewController:vc animated:YES];    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewDidUnload {

    [super viewDidUnload];
    [self setMyTableView:nil];
    [self setMyView:nil];
}


-(IBAction)submitLogout:(id)sender {
    [self.client logoutOnSuccess:^(NSDictionary *result) {
        
        self.navigationItem.leftBarButtonItem = nil;
        [self.myView setHidden:NO];
        [self.myTableView setHidden:YES];
        [self clearWhisperArray];
        
        UIBarButtonItem *loginButton = [[UIBarButtonItem alloc] initWithTitle:@"Login" style:UIBarButtonItemStylePlain target:self action:@selector(openLogin:)];
        self.navigationItem.rightBarButtonItem = loginButton;

        
    } onFailure:^(NSError *error) {
        NSLog(@"Logout Fail: %@",error);
    }];
}

- (void) clearWhisperArray {
    self.navigationItem.leftBarButtonItem = nil;
    self.whipserArray = nil;
}

- (void) loadTableData {
    
    [[self.appDelegate coreDataStore] purgeCacheOfObjectsWithEntityName:@"Whisper"];
    
    sleep(1);
    
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Whisper" inManagedObjectContext:self.managedObjectContext];
    [fetchRequest setEntity:entity];
    
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"createddate" ascending:NO];
    NSArray *sortDescriptors = [NSArray arrayWithObjects:sortDescriptor, nil];
    
    [fetchRequest setSortDescriptors:sortDescriptors];
    
    [self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
         NSLog(@"Fetch all whispers!");
        self.whipserArray = results;
        [self.myTableView reloadData];
    } onFailure:^(NSError *error) {
        NSLog(@"Error on Whisper Fetch");
    }];
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return  [self.whipserArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    NSManagedObject *object = [self.whipserArray objectAtIndex:indexPath.row];
    
    cell.textLabel.text = [object valueForKey:@"desc"];
    
    return cell;
}

@end
