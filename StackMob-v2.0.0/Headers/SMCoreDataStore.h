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

#import "SMDataStore.h"
#import "SMSyncedObject.h"

extern NSString *const SMSetCachePolicyNotification;
extern NSString *const SMDirtyQueueNotification;

extern BOOL SM_CACHE_ENABLED;

typedef enum {
    SMCachePolicyTryNetworkOnly = 0,
    SMCachePolicyTryCacheOnly  = 1,
    SMCachePolicyTryNetworkElseCache = 2,
    SMCachePolicyTryCacheElseNetwork = 3,
} SMCachePolicy;

typedef enum {
    SMClientObject = 0,
    SMServerObject = 1,
} SMMergeObjectKey;

typedef int (^SMMergePolicy)(NSDictionary *clientObject, NSDictionary *serverObject, NSDate *serverBaseLastModDate);
typedef void (^SMSyncCallback)(NSArray *objects);

extern SMMergePolicy const SMMergePolicyClientWins;
extern SMMergePolicy const SMMergePolicyLastModifiedWins;
extern SMMergePolicy const SMMergePolicyServerModifiedWins;

@class SMIncrementalStore;

/**
 The `SMCoreDataStore` class provides all the necessary properties and methods to interact with StackMob's Core Data integration.
 
 ## Using SMCoreDataStore ##
 
 With your `SMCoreDataStore` object you can retrieve a managed object context configured with a `SMIncrementalStore` as it's persistent store to allow communication to StackMob from Core Data.  Obtain a managed object context for your thread using <contextForCurrentThread>.  You can obtain the managed object context for the main thread at any time with <mainThreadContext>.
 
 When saving or fetching from the context, use methods from the <a href="http://stackmob.github.io/stackmob-ios-sdk/Categories/NSManagedObjectContext+Concurrency.html" target="_blank">NSManagedObjectContext+Concurrency category</a> to ensure proper asynchronous saving and fetching off of the main thread.
 
 If you want to do your own context creation, use the <persistentStoreCoordinator> property to ensure your objects are being saved to the StackMob server.
 
 The default Core Data merge policy set for all contexts created by this class is `NSMergeByPropertyObjectTrumpMergePolicy`.  Use <setDefaultMergePolicy:applyToMainThreadContextAndParent:> to change the default.
 
 ## Using the Cache and Offline Sync ##
 
 All the settings for turing on/off the cache, managing policies and sync callbacks, and initializing the sync process can be found in the <a href="https://developer.stackmob.com/ios-sdk/offline-sync-guide" target="_blank">Offline Sync Guide</a>.
 
 @note You should not have to initialize an instance of this class directly.  Instead, initialize an instance of <SMClient> and use the method `coreDataStoreWithManagedObjectModel:` to retrieve an instance completely configured and ready to communicate to StackMob.
 */
@interface SMCoreDataStore : SMDataStore

///-------------------------------
/// @name Properties
///-------------------------------

/**
 An instance of `NSPersistentStoreCoordinator` with the `SMIncrementalStore` class as it's persistent store type.
 
 Uses the `NSManagedObjectModel` passed to the `coreDataStoreWithManagedObjectModel:` method in <SMClient>.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property(nonatomic, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;

/**
 An instance of `NSManagedObjectContext` set to use on the main thread.
 
 This managed object context has a private queue parent context set to ensure proper parent/child asynchronous saving.  The persistent store coordinator is set on the parent context. Merge policy is set to NSMergeByPropertyObjectTrumpMergePolicy.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
@property (nonatomic, strong) NSManagedObjectContext *mainThreadContext;

/**
 An instance of `NSManagedObjectContext` set to use on the main thread.
 
 This property is deprecated. Use <contextForCurrentThread> to obtain a properly initialized managed object context.
 
 @since Available in iOS SDK 1.0.0 and later.
 
 @note Deprecated in version 1.2.0. Use <contextForCurrentThread>.
 */
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext __attribute__((deprecated));

/**
 The cache policy to adhere by during fetch requests.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
@property (nonatomic) SMCachePolicy cachePolicy;

/**
 The queue used to execute sync callbacks (success and failure).
 
 Default is main thread.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic) dispatch_queue_t syncCallbackQueue;

/**
 Boolean indicating whether a sync with the server is in progress.
 
 YES if a sync is in progress, otherwise NO.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic) BOOL syncInProgress;

/**
 Boolean indicating whether to send the `createddate` and `lastmoddate` keys and values in a request payload (during sync only).
 
 By default, the `createddate` and `lastmoddate` fields are set by the server. If you want these values to reflect the last local save, regardless of network connection, set this property to YES and the client will send them in request payloads during sync requests.
 
 @note This flag is only taken under consideration during a sync, where objects modified offline may have been assigned timestamps.  Saves when online go directly to the server, so the server sets the fields.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic) BOOL sendLocalTimestamps;

/**
 During sync, the global merge policy used to fix conflicts.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong) SMMergePolicy defaultSMMergePolicy;

/**
 When syncing dirty inserted objects, the merge policy used to fix conflicts.
 
 Set this property only if you want a different merge policy other than the default to use for insert conflicts.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong) SMMergePolicy insertsSMMergePolicy;

/**
 When syncing dirty updated objects, the merge policy used to fix conflicts.
 
 Set this property only if you want a different merge policy other than the default to use for update conflicts.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong) SMMergePolicy updatesSMMergePolicy;

/**
 When syncing dirty deletes objects, the merge policy used to fix conflicts.
 
 Set this property only if you want a different merge policy other than the default to use for delete conflicts.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong) SMMergePolicy deletesSMMergePolicy;

/**
 Property which holds a callback executed when inserts on the server fail during a sync.
 
 Set using <setSyncCallbackForFailedInserts:>.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong, setter = setSyncCallbackForFailedInserts:) SMSyncCallback syncCallbackForFailedInserts;

/**
 Property which holds a callback executed when updates on the server fail during a sync.
 
 Set using <setSyncCallbackForFailedUpdates:>.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong, setter = setSyncCallbackForFailedUpdates:) SMSyncCallback syncCallbackForFailedUpdates;

/**
 Property which holds a callback executed when deletes on the server fail during a sync.
 
 Set using <setSyncCallbackForFailedDeletes:>.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong, setter = setSyncCallbackForFailedDeletes:) SMSyncCallback syncCallbackForFailedDeletes;

/**
 Property which holds the callback executed when sync completes.
 
 Set using <setSyncCompletionCallback:>.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
@property (nonatomic, strong, setter = setSyncCompletionCallback:) SMSyncCallback syncCompletionCallback;

/**
 An instance of SMRequestOptions that will be used as the default for all save and fetch calls.
 
 When an instance of this class is initialized, this property is assigned a value of [SMRequestOptions options].  Set this property to customize default options.
 
 **Note:** Not all options provided by the SMRequestOptions class are taken into account during save/fetch requests.  The following options are currently safe to customize:
 
 * isSecure property (HTTPS)
 
 Customizing other options can result in unexpected requests, which can lead to save/fetch failures.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
@property (nonatomic, strong) SMRequestOptions *globalRequestOptions;


///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Initializes an `SMCoreDataStore`.
 
 @param apiVersion The API version of your StackMob application.
 @param session The session containing the credentials to use for requests made to StackMob by Core Data.
 @param managedObjectModel The managed object model to set to the persistent store coordinator.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initWithAPIVersion:(NSString *)apiVersion session:(SMUserSession *)session managedObjectModel:(NSManagedObjectModel *)managedObjectModel;

///-------------------------------
/// @name Obtaining a Managed Object Context
///-------------------------------

/**
 Returns an initialized context for the current thread.
 
 Merge policy is set to NSMergeByPropertyObjectTrumpMergePolicy.
 
 If the current thread is the main thread, returns a context initialized with a NSMainQueueConcurrencyType.  Otherwise, returns a context initialized with a NSPrivateQueueConcurrencyType, with the mainThreadContext as its parent.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (NSManagedObjectContext *)contextForCurrentThread;

/**
 Sets the merge policy that is set by default to any context returned from <contextForCurrentThread>.
 
 If apply is YES, sets the merge policy of mainThreadContext and its private parent context to mergePolicy.
 
 @param mergePolicy The default merge policy to use going forward.
 @param apply Whether or not to set mergePolicy as the merge policy for the existing mainThreadContext and its private parent context.
 
 @since Available in iOS SDK 1.2.0 and later.
 
 @note Deprecated in version 2.0.0. Use <setDefaultCoreDataMergePolicy:applyToMainThreadContextAndParent:>.
 */
- (void)setDefaultMergePolicy:(id)mergePolicy applyToMainThreadContextAndParent:(BOOL)apply __deprecated;

/**
 Sets the merge policy that is set by default to any context returned from <contextForCurrentThread>.
 
 If apply is YES, sets the merge policy of mainThreadContext and its private parent context to mergePolicy.
 
 @param mergePolicy The default merge policy to use going forward.
 @param apply Whether or not to set mergePolicy as the merge policy for the existing mainThreadContext and its private parent context.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)setDefaultCoreDataMergePolicy:(id)mergePolicy applyToMainThreadContextAndParent:(BOOL)apply;

///-------------------------------
/// @name Manually Purging the Cache
///-------------------------------

/**
 Removes the cache entry for the provided NSManagedObjectID.
 
 @param objectID The managed object ID of the object to remove from the cache, if an entry exists.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfManagedObjectID:(NSManagedObjectID *)objectID;

/**
 Removes the cache entries for the provided array of NSManagedObject instances.
 
 @param managedObjects An array of managed objects to remove from the cache, if entries exist.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfManagedObjects:(NSArray *)managedObjects;

/**
 Removes the cache entries for the provided array of NSManagedObjectID instances.
 
 @param managedObjectIDs An array of managed object IDs whose objects should be removed from the cache, if entries exist.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfManagedObjectsIDs:(NSArray *)managedObjectIDs;

/**
 Removes all the cache entries whose entity matches the provided entity name.
 
 @param entityName The name of the entity.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfObjectsWithEntityName:(NSString *)entityName;

/**
 Clears the cache of all entries.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)resetCache;

///-------------------------------
/// @name Sync
///-------------------------------

/**
 Initiates the sync process in the background.
 
 If a sync is already in progress, this method does nothing.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)syncWithServer;

/**
 Returns a Boolean indicating whether an object is potentially out of sync with its server representation.
 
 An object is marked dirty when it is modified and saved while the device is offline.
 
 @param objectID The NSManagedObjectID of the object.
 
 @return YES if the object is dirty, otherwise NO.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (BOOL)isDirtyObject:(NSManagedObjectID *)objectID;

/**
 Removes the "dirty" tag from an object. Optionally purges the object from the cache as well.
 
 Takes an entry from the array passed to a sync error callback.
 
 @param object The object to mark as synced.
 @param purge A boolean value representing whether the object should be purged from the cache.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)markFailedObjectAsSynced:(NSDictionary *)object purgeFromCache:(BOOL)purge;

/**
 Removes the "dirty" tag from all objects in an array. Optionally purges the objects from the cache as well.
 
 Takes an array of entries from the array passed to a sync error callback.
 
 @param objects The objects to mark as synced.
 @param purge A boolean value representing whether the objects should be purged from the cache.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)markArrayOfFailedObjectsAsSynced:(NSArray *)objects purgeFromCache:(BOOL)purge;

/**
 Use to set a callback executed when inserts on the server fail during a sync.
 
 @param block The block to execute when there are failed inserts.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)setSyncCallbackForFailedInserts:(void (^)(NSArray *objects))block;

/**
 Use to set a callback executed when updates on the server fail during a sync.
 
 @param block The block to execute when there are failed updates.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)setSyncCallbackForFailedUpdates:(void (^)(NSArray *objects))block;

/**
 Use to set a callback executed when deletes on the server fail during a sync.
 
 @param block The block to execute when there are failed deletes.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)setSyncCallbackForFailedDeletes:(void (^)(NSArray *objects))block;

/**
 Use to set a callback executed when a sync completes.
 
 @param block The block to execute when a sync with the server completes.
 
 @since Available in iOS SDK 2.0.0 and later.
 */
- (void)setSyncCompletionCallback:(void (^)(NSArray *objects))block;

@end
