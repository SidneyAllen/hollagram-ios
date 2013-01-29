//
//  Shout.h
//  hollagram
//
//  Created by Sidney Maestre on 1/28/13.
//  Copyright (c) 2013 StackMob. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>


@interface Shout : NSManagedObject

@property (nonatomic, retain) NSDate * createddate;
@property (nonatomic, retain) NSString * desc;
@property (nonatomic, retain) NSString * shoutId;
@property (nonatomic, retain) NSString * sm_owner;
@property (nonatomic, retain) NSString * url;
@property (nonatomic, retain) NSString * username;

@end
