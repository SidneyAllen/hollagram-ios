//
//  User.h
//  hollagram
//
//  Created by Sidney Maestre on 1/24/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "StackMob.h"

@class Whisper;

@interface User : SMUserManagedObject

@property (nonatomic, retain) NSString * pic;
@property (nonatomic, retain) NSString * username;
@property (nonatomic, retain) NSSet *whispers;
- (id)initIntoManagedObjectContext:(NSManagedObjectContext *)context;

@end

@interface User (CoreDataGeneratedAccessors)

- (void)addWhispersObject:(Whisper *)value;
- (void)removeWhispersObject:(Whisper *)value;
- (void)addWhispers:(NSSet *)values;
- (void)removeWhispers:(NSSet *)values;


@end
