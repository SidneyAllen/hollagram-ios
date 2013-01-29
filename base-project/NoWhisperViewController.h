//
//  NoWhisperViewController.h
//  base-project
//
//  Created by Sidney Maestre on 1/16/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@class SMClient;

@interface NoWhisperViewController : UIViewController <UITableViewDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

@property (strong, nonatomic) SMClient *client;
@property (strong, nonatomic) UITableView  *myTableView;
@property (strong, nonatomic) UIView  *myView;

@property (strong, nonatomic) NSArray  *whipserArray;

@end
