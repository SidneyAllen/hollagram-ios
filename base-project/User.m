//
//  User.m
//  hollagram
//
//  Created by Sidney Maestre on 1/24/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import "User.h"
#import "Whisper.h"


@implementation User

@dynamic pic;
@dynamic username;
@dynamic whispers;

- (id)initIntoManagedObjectContext:(NSManagedObjectContext *)context {
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"User" inManagedObjectContext:context];
    self = [super initWithEntity:entity insertIntoManagedObjectContext:context];
    
    if (self) {
        // assign local variables and do other init stuff here
    }
    
    return self;
}

@end
