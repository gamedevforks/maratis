//
//  exampleAppDelegate.m
//  example
//
//  Created by Anaël Seghezzi on 10/03/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import "exampleAppDelegate.h"
#import "EAGLView.h"

@implementation exampleAppDelegate

@synthesize window;
@synthesize glView;

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
	application.statusBarHidden = YES;
	[glView startAnimation];
}

- (void) applicationWillResignActive:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
	[glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) dealloc
{
	[window release];
	[glView release];
	
	[super dealloc];
}

@end
