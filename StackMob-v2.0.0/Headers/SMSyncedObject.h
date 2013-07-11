/*
 * Copyright 2012-2013 StackMob
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <CoreData/CoreData.h>

typedef enum {
    SMSyncActionInsertedOnServer = 0,
    SMSyncActionUpdatedOnServer = 1,
    SMSyncActionDeletedFromServer = 2,
    SMSyncActionUpdatedCache = 3,
} SMSyncAction;

/**
 When a sync with the server completes, the `SMCoreDataStore` `syncCompletionCallback` block is executed, and passed an array of successfully synced objects.  All objects will be instances of `SMSyncedObject`.
 
 An `SMSyncedObject` instance contains the object ID, as well as information about how the object was merged. See the <actionTaken> property for more information.
 */
@interface SMSyncedObject : NSObject

///-------------------------------
/// Properties
///-------------------------------

/**
 The object ID of the synced object.  If the object was inserted or updated, this will be an instance of `NSManagedObjectID`.  If it was deleted, this will be an instance of `NSString`.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic) id objectID;

/**
 The action taken when synced.  Property is of type `SMSyncAction`, defined as:
 
    typedef enum {
        SMSyncActionInsertedOnServer = 0,
        SMSyncActionUpdatedOnServer = 1,
        SMSyncActionDeletedFromServer = 2,
        SMSyncActionUpdatedCache = 3,
    } SMSyncAction;
 
 @since Available in iOS SDK 2.0.0 and later.
 
 */
@property (nonatomic) SMSyncAction actionTaken;

///-------------------------------
/// Initialize
///-------------------------------

/**
 Initialize a new instance of `SMSyncedObject`.  
 
 You should not need to instantiate an instance of this class, as it is used internally during the sync with server process.
 
 @param objectID The object ID.
 @param actionTaken The action taken for this object during the sync.
 
 @return An instance of `SMSyncedObject`.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (id)initWithObjectID:(id)objectID actionTaken:(SMSyncAction)actionTaken;

@end
