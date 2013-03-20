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
#import "SMResponseBlocks.h"
#import "AFHTTPClient.h"

@class SMRequestOptions;
@class SMDataStore;
@class SMUserSession;
@class SMCoreDataStore;

#define DEFAULT_API_HOST @"api.stackmob.com"
#define DEFAULT_USER_SCHEMA @"user"
#define DEFAULT_PRIMARY_KEY_FIELD_NAME @"username"
#define DEFAULT_PASSWORD_FIELD_NAME @"password"

/**
 `SMClient` provides a high level interface to interacting with StackMob. A new client must be given, at the very least, an API version and public key in order to communicate with your StackMob application.
 
 When initialized, an instance of `SMClient` sets default values for other configuration settings which may be set as necessary by your application.
 
 A <defaultClient> is can be used for applications which use a globally available client to share configuration settings.
 
 ## Initialize ##
 
 Wherever you plan to use StackMob, add `#import "StackMob.h"` to the header file.
 
 Create a variable of type `SMClient`, most likely in your AppDelegate file where you initialize other application wide variables, and initialize it like this:
 
    // Assuming your variable is declared SMClient *client;
    client = [[SMClient alloc] initWithAPIVersion:@"YOUR_API_VERSION" publicKey:@"YOUR_PUBLIC_KEY"];
 
 For YOUR_API_VERSION, pass @"0" for Development, @"1" or higher for the corresponding version in Production.
 
 If you haven't found your public key yet, check out **Manage App Info** under the **App Settings** sidebar on the [Dashboard page](https://dashboard.stackmob.com).
 
 **Important:** The default schema to use for authentication is **user**, with **username** and **password** fields. If you plan on using a different user object schema or different field names, check out the **User Authentication** section below.
 
 <br/>
 
 ### Default Client ###
 
 The class method <defaultClient> gives you a shared instance of your configured `SMClient` instance. 
 
 It is always set to the first `SMClient` instance that is initialized, and can be set to a different instance with the <setDefaultClient:> method.
 
 
 ## Core Data Integration ##
 
 In order to use the Core Data integration, you must initialize an `SMClient` as well as a `NSManagedObjectModel`, then pass the `NSManagedObjectModel` instance to the `SMClient` instance method <coreDataStoreWithManagedObjectModel:> which returns an instance of <SMCoreDataStore>.  You can then retrieve an instance of `NSManagedObjectContext` using the contextForCurrentThread method:
 
    SMClient *client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"];
    SMCoreDataStore *coreDataStore = [client coreDataStoreWithManagedObjectModel:self.managedObjectModel];
    self.managedObjectContext = [coreDataStore contextForCurrentThread];
 
 It is important only to instantiate one `SMCoreDataStore` instance throughout the duration of your application.  This ensures that you use the same copy of the persistent store coordinator.
 
 Check out the [SMCoreDataStore class](http://stackmob.github.com/stackmob-ios-sdk/Classes/SMCoreDataStore.html) for a list of all methods and functionality for interacting with the Core Data integration.
 
 For the full list of Core Data functionality that is supported by our integration, as well as work arounds for that which is not, check out the [StackMob <--> Core Data Support Specifications](http://stackmob.github.com/stackmob-ios-sdk/CoreDataSupportSpecs.html).
 
 Last but not least, make sure to adhere to the [StackMob <--> Core Data Coding Practices](http://stackmob.github.com/stackmob-ios-sdk/index.html\#coding\_practices)!
 
 ## The User Schema ##
 
 When you create an application on StackMob, a **user** schema is automatically generated, with **username** as it's primary key field as well as a **password** field. This is the default schema for user objects.
 
 When you initialize an `SMClient` with <initWithAPIVersion:publicKey:>, the following defaults are set:
 
 * apiHost = @"api.stackmob.com";
 * userSchema = @"user";
 * userPrimaryKeyField = @"username";
 * userPasswordField = @"password";
 
 To change the defaults so they match your schemas and fields on StackMob:
 
 * With your instance of `SMClient`, you can directly set the properties listed above using the dot notation or setters.  For example:
 
 
        client.userSchema = @"teacher";
        [client setUserPrimaryKeyField:@"email"];
 
 
 * Alternatively, you can set all the properties at once using <initWithAPIVersion:apiHost:publicKey:userSchema:userPrimaryKeyField:userPasswordField:>.
 
 **Important:** Don't forget to check the **Create as a User Object** box when <a href="https://developer.stackmob.com/api/schemas/create" target="_blank">creating a new schema</a> for user objects.
 
 
 ## User Authentication ##
 
 When a client is instantiated, an instance of `SMUserSession` is initialized and configured with the provided settings.  This is where the user's OAuth2 credentials and token information is located, and is used by the internal `SMDataStore` instance to authenticate requests.
 
 `SMClient` provides a set of methods for logging in and out, checking state, and resetting passwords. They are listed below in **Tasks** starting with the **Basic Authentication** section.
 
 ## Facebook Authentication ##
 
 The Facebook token used in the methods to create, link or login users comes from the Facebook SDK.  You must register your application with the Facebook developer website, import the SDK, and follow the tutorial to get the SDK set up in your application.  When you successfully login via Facebook, a method in your application is called and provided with the token you'll need to pass to the Facebook methods listed below.  This token does not changed once assigned but is used to verify that you have a currently authenticated session by the Facebook SDK.
 
 */
@interface SMClient : NSObject

#pragma mark properties
///-------------------------------
/// @name Properties
///-------------------------------

/**
 The API version of your StackMob application which this client instance should use.
 
 Pass @"0" for Development, @"1" or higher for the corresponding version in Production.
 */
@property(nonatomic, copy) NSString *appAPIVersion;

/**
 The host to connect to for API requests. 
 
 Default is `api.stackmob.com`.
 */
@property(nonatomic, copy) NSString *apiHost;

/**
 Your StackMob application's OAuth2 public key.
 */
@property(nonatomic, readonly, copy) NSString *publicKey;

/**
 The StackMob schema that has been marked as a user object. 
 
 Default is `@"user"`.
 */
@property(nonatomic, copy) NSString *userSchema;

/**
 The StackMob primary key field name for the user object schema. 
 
 Default is `@"username"`.
 */
@property(nonatomic, copy) NSString *userPrimaryKeyField;

/**
 The StackMob field name for the password. 
 
 Default is `@"password"`.
 */
@property(nonatomic, copy) NSString *userPasswordField;

/**
 An instance of `SMUserSession` which contains the necessary credentials to make StackMob requests.
 */
@property(nonatomic, readonly, strong) SMUserSession * session;


#pragma mark init
///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Override the default client.
 
 @param client The client to set.
 */
+ (void)setDefaultClient:(SMClient *)client;

/**
 A shared `SMClient` instance, for convenience. This will be the first `SMClient` object created, unless overridden
 via <setDefaultClient:>.
 */
+ (SMClient *)defaultClient;

/**
 Initialize specifying all parameters.
 
 @param appAPIVersion The API version of your StackMob application which this client instance should use.
 @param apiHost The host to connect to for API requests. Default is `api.stackmob.com`.
 @param publicKey Your StackMob application's OAuth2 public key.
 @param userSchema The StackMob schema that has been marked as a user object. Default is `@"user"`.
 @param userPrimaryKeyField The StackMob primary key field name for the user object schema. Default is `@"username"`.
 @param userPasswordField The StackMob field name for the password. Default is `@"password"`.
 
 @return An instance of `SMClient`.
 */
- (id)initWithAPIVersion:(NSString *)appAPIVersion
                 apiHost:(NSString *)apiHost 
               publicKey:(NSString *)publicKey 
              userSchema:(NSString *)userSchema
              userPrimaryKeyField:(NSString *)userPrimaryKeyField
       userPasswordField:(NSString *)userPasswordField;

/**
 Initialize with only the most basic parameters and defaults for the rest.
 
 @param appAPIVersion The API version of your StackMob application which this client instance should use.
 @param publicKey Your StackMob application's OAuth2 public key.
 
 @return An instance of `SMClient`.
 */
- (id)initWithAPIVersion:(NSString *)appAPIVersion publicKey:(NSString *)publicKey;

#pragma mark Datastore
///-------------------------------
/// @name Retrieving a Datastore
///-------------------------------

/**
 With the instance of <SMCoreDataStore> returned by this method you can call the <code>- (NSManagedObjectContext)managedObjectContext</code> method to retrieve an instance of `NSManagedObjectContext` that has been configured to StackMob.  It includes an `NSPersistentStoreCoordinator` of type <SMIncrementalStore> which has been initialized with the `NSManagedObjectModel` provided to this method.
 
 @param managedObjectModel An instance of `NSManagedObjectModel` set to the data model to be replicated on StackMob.
 
 @return An instance of `SMCoreDataStore`.
 */
- (SMCoreDataStore *)coreDataStoreWithManagedObjectModel:(NSManagedObjectModel *)managedObjectModel;

/**
 A `dataStore` instance should be used to make direct REST calls to StackMob.  See <SMDataStore> for basic CRUD methods.
 
 @return An `SMDataStore` instance using this client's configurations.
 */
- (SMDataStore *)dataStore;

#pragma mark auth
///-------------------------------
/// @name Basic Authentication
///-------------------------------

/**
 Login a user to your app with a username/password. 
 
 The credentials should match an existing user object.
 
 @param username The username to log in with.
 @param password The password to log in with.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure. If the error code is `SMErrorTemporaryPasswordResetRequired`, you should prompt the user supply a new password and call <loginWithUsername:temporaryPassword:settingNewPassword:onSuccess:onFailure:>.
 */
- (void)loginWithUsername:(NSString *)username
                 password:(NSString *)password
                onSuccess:(SMResultSuccessBlock)successBlock
                onFailure:(SMFailureBlock)failureBlock;

/**
 Login a user to your app with a username/password. 
 
 The credentials should match an existing user object.
 
 @param username The username to log in with.
 @param password The password to log in with.
 @param options An options object contains headers and other configuration for this request.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure. If the error code is `SMErrorTemporaryPasswordResetRequired`, you should prompt the user supply a new password and call <loginWithUsername:temporaryPassword:settingNewPassword:onSuccess:onFailure:>.
 */
- (void)loginWithUsername:(NSString *)username
                 password:(NSString *)password
              options:(SMRequestOptions *)options
                onSuccess:(SMResultSuccessBlock)successBlock
                onFailure:(SMFailureBlock)failureBlock;

/**
 Login a user to your app with a username and temporary password, changing the users's password to the supplied new password.
 
 This call is meant to be used as part of the forgot password flow. After the user receives an email with their temporary password, they should be taken to a login screen with an extra field for a new password, and that should hook up to this API. Your app can detect this situation via <loginWithUsername:password:onSuccess:onFailure:> returning the error `SMErrorTemporaryPasswordResetRequired` to the failure block.
 
 @param username The username to log in with.
 @param tempPassword The temporary password received via email.
 @param newPassword The new password to be set, invalidating the old and temporary passwords.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithUsername:(NSString *)username
        temporaryPassword:(NSString *)tempPassword
       settingNewPassword:(NSString *)newPassword      
                onSuccess:(SMResultSuccessBlock)successBlock
                onFailure:(SMFailureBlock)failureBlock;

/**
 Login a user to your app with a username and temporary password, changing the users's password to the supplied new password. 
 
 This call is meant to be used as part of the forgot password flow. After the user receives an email with their temporary password, they should be taken to a login screen with an extra field for a new password, and that should hook up to this API. Your app can detect this situation via <loginWithUsername:password:onSuccess:onFailure:> returning the error `SMErrorTemporaryPasswordResetRequired`.
 
 @param username The username to log in with.
 @param tempPassword The temporary password received via email.
 @param newPassword The new password to be set, invalidating the old and temporary passwords.
 @param options An options object contains headers and other configuration for this request.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithUsername:(NSString *)username
        temporaryPassword:(NSString *)tempPassword
       settingNewPassword:(NSString *)newPassword
              options:(SMRequestOptions *)options
                onSuccess:(SMResultSuccessBlock)successBlock
                onFailure:(SMFailureBlock)failureBlock;

/**
 Refresh the current login. 
 
 Only use this method if you plan to manually manage your session. Logins expire after an hour and needs to be refreshed. This is handled automatically when you make a request, but for highly concurrent systems you may want to call this manually.
 
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)refreshLoginWithOnSuccess:(SMResultSuccessBlock)successBlock
                        onFailure:(SMFailureBlock)failureBlock;

#pragma mark Retrieve User
///-------------------------------
/// @name Retrieve the Logged In User
///-------------------------------

/**
 Return the full object associated with the logged in user.
 
 Useful on app startup to replace login when the user is already logged in.
 
 @param options An options object contains headers and other configuration for this request.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)getLoggedInUserWithOptions:(SMRequestOptions *)options
                         onSuccess:(SMResultSuccessBlock)successBlock
                         onFailure:(SMFailureBlock)failureBlock;

/**
 Return the full object associated with the logged in user.
 
 Useful on app startup to replace login when the user is already logged in.
 
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)getLoggedInUserOnSuccess:(SMResultSuccessBlock)successBlock
                       onFailure:(SMFailureBlock)failureBlock;

/**
 Check whether the current user is logged in.
 
 This method first checks if a refresh token exists, and if that fails checks to see if the expiration date on the access token is later than the current time.  The reason we return `YES` to this method if a refresh token exists is because automatic refresh of a session using the refresh token is initiated if a request comes back unauthorized or the current access token has expired.  The developer does not have to worry about refreshing their own user sessions. 
 
 @return `YES` if the current user is logged in, otherwise `NO`.
 */
- (BOOL)isLoggedIn;

/**
 Check whether the user is logged out by returning the negation of <isLoggedIn>.
 
 @return `YES` if the current user is logged out, otherwise `NO`.
 */
- (BOOL)isLoggedOut;

#pragma mark Logout
///-------------------------------
/// @name Logout
///-------------------------------

/**
 Logout, clearing token validity locally and on the server.
 
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)logoutOnSuccess:(SMResultSuccessBlock)successBlock
              onFailure:(SMFailureBlock)failureBlock;

#pragma mark Resetting Password
///-------------------------------
/// @name Resetting Password
///-------------------------------

/**
 Kick off the "Forgot Password" process.
 
 This should be hooked up to a button on the login screen. An email will be sent to the user with a temporary 
 password. They can then use that temporary password to login with <loginWithUsername:temporaryPassword:settingNewPassword:onSuccess:onFailure:>.
 
 @param username The user to send the email to.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)sendForgotPaswordEmailForUser:(NSString *)username
                            onSuccess:(SMResultSuccessBlock)successBlock
                            onFailure:(SMFailureBlock)failureBlock;

/**
 Reset a user's password securely.
 
 This would be hooked up to a password reset form. Changing a password via the regular Datastore API 
 will result in an error. This API requires the user to be logged in as well as to supply their old password.
 
 @param oldPassword The user's current password.
 @param newPassword The new password for the user.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)changeLoggedInUserPasswordFrom:(NSString *)oldPassword
                                    to:(NSString *)newPassword
                             onSuccess:(SMResultSuccessBlock)successBlock
                             onFailure:(SMFailureBlock)failureBlock;


#pragma mark Facebook
///-------------------------------
/// @name Facebook Authentication
///-------------------------------

/**
 Create a user linked with a Facebook account.
 
 The username for this method is extracted from the Facebook account.
 @param fbToken A Facebook access token obtained from Facebook.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)createUserWithFacebookToken:(NSString *)fbToken
                          onSuccess:(SMResultSuccessBlock)successBlock
                          onFailure:(SMFailureBlock)failureBlock;

/**
 Create a user linked with a Facebook account
 
 @param fbToken A Facebook access token obtained from Facebook
 @param username The username to user, rather than getting one from Facebook.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)createUserWithFacebookToken:(NSString *)fbToken
                           username:(NSString *)username
                          onSuccess:(SMResultSuccessBlock)successBlock
                          onFailure:(SMFailureBlock)failureBlock;

/**
 Link the logged in user with a Facebook account.
 
 @param fbToken A Facebook access token obtained from Facebook.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)linkLoggedInUserWithFacebookToken:(NSString *)fbToken
                                onSuccess:(SMResultSuccessBlock)successBlock
                                onFailure:(SMFailureBlock)failureBlock;

/**
 Login a user to your app with a Facebook token.
 
 The credentials should match a existing user object that has a linked Facebook account, via either <createUserWithFacebookToken:onSuccess:onFailure:>, or <linkLoggedInUserWithFacebookToken:onSuccess:onFailure:>.
 
 @param fbToken A Facebook access token obtained from Facebook.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithFacebookToken:(NSString *)fbToken
                     onSuccess:(SMResultSuccessBlock)successBlock
                     onFailure:(SMFailureBlock)failureBlock;

/**
 Login a user to your app with a Facebook token.
 
 The credentials should match a existing user object that has a linked Facebook account, via either <createUserWithFacebookToken:onSuccess:onFailure:>, or <linkLoggedInUserWithFacebookToken:onSuccess:onFailure:>.
 
 @param fbToken A Facebook access token obtained from Facebook.
 @param options An options object contains headers and other configuration for this request.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithFacebookToken:(NSString *)fbToken
                   options:(SMRequestOptions *)options
                     onSuccess:(SMResultSuccessBlock)successBlock
                     onFailure:(SMFailureBlock)failureBlock;

/**
 Update the logged in users's Facebook status.
 
 The logged in user must have a linked Facebook account, via either 
 <createUserWithFacebookToken:onSuccess:onFailure:>, or <linkLoggedInUserWithFacebookToken:onSuccess:onFailure:>.
 
 @param message The message to post.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)updateFacebookStatusWithMessage:(NSString *)message
                          onSuccess:(SMResultSuccessBlock)successBlock
                          onFailure:(SMFailureBlock)failureBlock;

/**
 Get Facebook info for the logged in users.
 
 The logged in user must have a linked Facebook account, via either 
 <createUserWithFacebookToken:onSuccess:onFailure:>, or <linkLoggedInUserWithFacebookToken:onSuccess:onFailure:>.
 
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)getLoggedInUserFacebookInfoWithOnSuccess:(SMResultSuccessBlock)successBlock
                                       onFailure:(SMFailureBlock)failureBlock;


#pragma mark twitter
///-------------------------------
/// @name Twitter Authentication
///-------------------------------

/**
 Create a user linked with a Twitter account
 
 The username is extracted from the Twitter account.
 
 @param twitterToken A Twitter token obtained from Twitter.
 @param twitterSecret A Twitter secret obtained from Twitter.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)createUserWithTwitterToken:(NSString *)twitterToken
                     twitterSecret:(NSString *)twitterSecret
                         onSuccess:(SMResultSuccessBlock)successBlock
                         onFailure:(SMFailureBlock)failureBlock;

/**
 Create a user linked with a Twitter account.
 
 @param twitterToken A Twitter token obtained from Twitter.
 @param twitterSecret A Twitter secret obtained from Twitter.
 @param username The username to user, rather than getting one from Twitter.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)createUserWithTwitterToken:(NSString *)twitterToken
                     twitterSecret:(NSString *)twitterSecret
                          username:(NSString *)username
                         onSuccess:(SMResultSuccessBlock)successBlock
                         onFailure:(SMFailureBlock)failureBlock;

/**
 Link the logged in user with a Twitter account.
 
 @param twitterToken A Twitter token obtained from Twitter.
 @param twitterSecret A Twitter secret obtained from Twitter.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)linkLoggedInUserWithTwitterToken:(NSString *)twitterToken
                           twitterSecret:(NSString *)twitterSecret
                               onSuccess:(SMResultSuccessBlock)successBlock
                               onFailure:(SMFailureBlock)failureBlock;

/**
 Login a user to your app with twitter credentials.
 
 The credentials should match a existing user object that has a linked Twitter account, via either <createUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>, or <linkLoggedInUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>.
 
 @param twitterToken A Twitter token obtained from Twitter.
 @param twitterSecret A Twitter secret obtained from Twitter.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithTwitterToken:(NSString *)twitterToken
                twitterSecret:(NSString *)twitterSecret
                    onSuccess:(SMResultSuccessBlock)successBlock
                    onFailure:(SMFailureBlock)failureBlock;

/**
 Login a user to your app with twitter credentials.
 
 The credentials should match a existing user object that has a linked Twitter account, via either 
 <createUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>, or <linkLoggedInUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>.
 
 @param twitterToken A Twitter token obtained from Twitter.
 @param twitterSecret A Twitter secret obtained from Twitter.
 @param options An options object contains headers and other configuration for this request.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon successful login with the user object for the logged in user.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithTwitterToken:(NSString *)twitterToken
                twitterSecret:(NSString *)twitterSecret
                  options:(SMRequestOptions *)options
                    onSuccess:(SMResultSuccessBlock)successBlock
                    onFailure:(SMFailureBlock)failureBlock;

/**
 Update the logged in users's Twitter status.
 
 The logged in user must have a linked Twitter account, via either 
 <createUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>, or <linkLoggedInUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>.
 
 @param message The message to post.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)updateTwitterStatusWithMessage:(NSString *)message
                             onSuccess:(SMResultSuccessBlock)successBlock
                             onFailure:(SMFailureBlock)failureBlock;

/**
 Get Twitter info for the logged in users.
 
 The logged in user must have a linked Twitter account, via either
 <createUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>, or <linkLoggedInUserWithTwitterToken:twitterSecret:onSuccess:onFailure:>.
 
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)getLoggedInUserTwitterInfoOnSuccess:(SMResultSuccessBlock)successBlock
                                      onFailure:(SMFailureBlock)failureBlock;


#pragma mark - Gigya
///-------------------------------
/// @name Gigya Authentication
///-------------------------------

/**
 Login with Gigya, providing the m_pDict property of the GSObject user parameter provided by the gsLoginUIDidLogin:user:context: delegate method.
 
 If a user does not exist that has been previously linked with the provided information from Gigya, one will be created.
 
 @param gsUser The dictionary returned by user.m_pDict from the gsLoginUIDidLogin:user:context method.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithGigyaUserDictionary:(NSDictionary *)gsUser
                           onSuccess:(SMResultSuccessBlock)successBlock
                           onFailure:(SMFailureBlock)failureBlock;

/**
 Login with Gigya, providing the values of the `UID`, `UIDSignature` and `timestampSignature` keys from the GSObject user parameter provided by the gsLoginUIDidLogin:user:context: method.
 
 If a user does not exist that has been previously linked with the provided information from Gigya, one will be created.
 
 @param uid The value of the `UID` key from the GSObject user parameter.
 @param uidSignature The value of the `UIDSignature` key from the GSObject user parameter.
 @param signatureTimestamp The value of the `signatureTimestamp` key from the GSObject user parameter.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithGigyaUID:(NSString *)uid
                 uidSignature:(NSString *)uidSignature
           signatureTimestamp:(NSString *)signatureTimestamp
                    onSuccess:(SMResultSuccessBlock)successBlock
                    onFailure:(SMFailureBlock)failureBlock;

/**
 Login with Gigya with options parameter.
 
 @param uid The value of the `UID` key from the GSObject user parameter.
 @param uidSignature The value of the `UIDSignature` key from the GSObject user parameter.
 @param signatureTimestamp The value of the `signatureTimestamp` key from the GSObject user parameter.
 @param options The request options.
 @param successBlock <i>typedef void (^SMResultSuccessBlock)(NSDictionary *result)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 */
- (void)loginWithGigyaUID:(NSString *)uid
             uidSignature:(NSString *)uidSignature
       signatureTimestamp:(NSString *)signatureTimestamp
                  options:(SMRequestOptions *)options
                onSuccess:(SMResultSuccessBlock)successBlock
                onFailure:(SMFailureBlock)failureBlock;

@end
