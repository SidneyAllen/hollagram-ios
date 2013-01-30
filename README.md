hollagram-ios
=============

The next gen social app destined to replace Twitter ;-)

# Prerequisites
* If you don't have a StackMob account, signup for your free account at <a href="http://www.stackmob.com">www.stackmob.com</a>
* Amazon S3 Account

# Setup
* From the StackMob Dashbaord <a href="https://developer.stackmob.com/start?newapp">create a new app</a>.
* Go through the Getting Started to Download the StackMob SDK and make your first API call
 
# Add Binary field to the User Schema
Every new StackMob has a User schema created by default.  You'll want to add a new field to the User Schema.  The field should be called "pic" and have a type of "binary"
In order to use the binary type field, you'll need to configure Amazon S3.  Here is a <a href="https://developer.stackmob.com/tutorials/dashboard/Adding-a-Binary-Field-to-Schemas">step by step guide</a> to do it.

# Add API Key to  AppDelegate.m
The last thing you'll need to do is get your <a href="https://dashboard.stackmob.com/settings">Public Development API key</a> copy and paste it into your AppDelegate.

    self.client = [[SMClient alloc] initWithAPIVersion:@"0"  publicKey:@"YOUR_PUBLIC_API_KEY"];
