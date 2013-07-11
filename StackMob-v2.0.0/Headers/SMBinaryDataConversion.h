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

/**
 `SMBinaryDataConversion` offers a class method <stringForBinaryData:name:contentType:> to decode binary data into a string.  This is then used to send to StackMob as the value for a field with type Binary Data.  The contents of the string will be parsed and the content will be stored on s3.  StackMob will then store the public url as the value.  A call to `refreshObject:mergeChanges:` on the managed object context will update the in-memory value to the url in the persistent store.
 
 ## Example s3 Integration ##
 
 Once you have your content in `NSData` form, pass it to the <stringForBinaryData:name:contentType:> method.
 
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString* pathToImageFile = [bundle pathForResource:@"coolPic" ofType:@"jpg"];
    NSData *theData = [NSData dataWithContentsOfFile:pathToImageFile];
 
    NSString *picData = [SMBinaryDataConversion stringForBinaryData:theData name:@"whateverNameYouWant" contentType:@"image/jpg"];
 
 You can now set the string value, save the managed object context, and refresh your in-memory copy of the object to grab the url pointing to your data.
 
    [newManagedObject setValue:picData forKey:@"pic"];
 
    NSManagedObjectContext *context = [[[SMClient defaultClient] coreDataStore] contextForCurrentThread];
    [context saveOnSuccess:^{
        [context refreshObject:newManagedObject mergeChanges:YES];
    } onFailure:^(NSError *error) {
        // Error
    }];
 
 `[newManagedObject valueForKey:@"pic"]` now returns the s3 url for the data.
 
 ## Saving Binary Data Offline ##
 
 When you save an object with binary data while the device is offline, the value of the attribute will contain a data representation, ready to be saved to StackMob. In order to properly read it at that point, the data must be extracted from the string and decoded.
 
 In order to check if the attribute value is a url or not, use the <stringContainsURL:> method.  To convert the string value back to data, use the <dataForString:> method.
 
    NSString *picString = [newManagedObject valueForKey:@"pic"];
    if ([SMBinaryDataConversion stringContainsURL:picString]) {
        NSURL *urlForPic = [NSURL URLForString:picString];
        // Set image from url
        
    } else {
        UIImage *image = [UIImage imageWithData:[SMBinaryDataConversion dataForString:picString]];
        // Set image directly
    }
 
 @note Binary Data fields are not inferred. You must edit the schema on the StackMob website and add a new field of type Binary Data that has the same name as the string attribute in your Xcode data model.  This must be done before you persist any data to avoid inferring a field with type string.
 
 */
@interface SMBinaryDataConversion : NSObject

/**
 Returns a string version of binary data used by StackMob to upload content to s3.
 
 @param data The actual content in `NSData` form.
 @param name A name for the content.  This can be any arbitrary name.
 @param contentType The content type of the data. See [Internet Media Type](http://en.wikipedia.org/wiki/Internet%5fmedia%5ftype) for a full list.
 
 @return A string formatted for StackMob to persist data to S3.
 @since Available in iOS SDK 1.0.0 and later.
 */
+ (NSString *)stringForBinaryData:(NSData *)data name:(NSString *)name contentType:(NSString *)contentType;

/**
 Returns the data representation of a string created with <stringForBinaryData:name:contentType:>.
 
 Use this method when pulling the attribute value when the object has not yet been synced with the server, i.e. does not yet contain a proper S3 URL.
 
 @param string The string to convert to data.
 
 @return The data representation of the string.
 
 @since Available in iOS SDK 2.0.0 and later.
 
 */
+ (NSData *)dataForString:(NSString *)string;

/**
 Given a string attribute which maps to a Binary field on StackMob, returns whether the string contains an S3 URL.
 
 If the object has already been saved and persisted to StackMob, the string will contain the S3 URL which maps to the data.  Otherwise the object will contain a data representation, ready to be saved to StackMob. In order to properly read it the data must be extracted from the string and decoded using the <dataForString:> method.
 
 @param value The string to evaluate.
 
 @return YES if the string contains an S3 URL, otherwise NO.
 
 @since Available in iOS SDK 2.0.0 and later.
 
 */
+ (BOOL)stringContainsURL:(NSString *)value;



@end
