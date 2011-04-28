//
//  EAGLView.m
//  example
//
//  Created by Anaël Seghezzi on 10/03/11.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#include <MEngine.h>

#include <MContexts/MES1Context.h>
#include <MContexts/MALContext.h>
#include <MContexts/MBulletContext.h>
#include <MLoaders/MiOSImageLoader.h>
//#include <MLoaders/MSndFileLoader.h>
//#include <MLoaders/MFreetypeLoader.h>
//#include <MLoaders/MBinFontLoader.h>

#include <MFileManager/MLevelLoad.h>
#include <MBehaviors/MBLookAt.h>
#include <MBehaviors/MBFollow.h>
#include <MScript/MScript.h>
#include <MInput/MInput.h>
#include <MFileManager/MMeshLoad.h>
#include <MFileManager/MLevelLoad.h>
#include <MProject/MProject.h>
//#include <MRenderers/MStandardRenderer.h>
#include <MRenderers/MFixedRenderer.h>

MRenderingContext * render;
MInputContext *	input;
MLevel * level;
MGame * game;
MRenderer * mrenderer;
MSystemContext * msystem;
MPhysicsContext * physics;
MScriptContext * script;
MSoundContext * soundContext;

class MiOSContext : public MSystemContext
{
public:
	
	// destructor
	~MiOSContext(void){}
	
	// screen
	void getScreenSize(unsigned int * width, unsigned int * height)
	{
        // Get real screen size
        CGRect screenBounds = [[UIScreen mainScreen] bounds];
		if (width) (*width) = CGRectGetWidth(screenBounds);
		if (height) (*height) = CGRectGetHeight(screenBounds);
	}
	
	// cursor
	void setCursorPosition(int x, int y){}
	
	// working directory
	const char * getWorkingDirectory(void)
	{
		static char currentDirectory[256] = "";
		//NSString * resourcePath = [[NSBundle mainBundle] resourcePath];
		//strcpy(currentDirectory, [resourcePath cStringUsingEncoding:NSUTF8StringEncoding]);
		return currentDirectory;
	}	
	
	// system tick
	unsigned long getSystemTick(void)
	{
		static NSTimeInterval startTime = [NSDate timeIntervalSinceReferenceDate];
		return (unsigned long)(([NSDate timeIntervalSinceReferenceDate] - startTime ) * 1000.0);
	}
};

void loadProject(const char * filename)
{
	MEngine * engine = MEngine::getInstance();
	
	if(! filename)
		return;
	
	// load project file
	MProject proj;
	if(proj.loadXML(filename))
	{
		//loadGamePlugin();
		
		// renderer
		//changeRenderer(proj.renderer.c_str());
		
		// load start level
		engine->loadLevel(proj.startLevel.c_str());
	}
}





#import "EAGLView.h"

#import "ES1Renderer.h"
#import "ES2Renderer.h"

@implementation EAGLView

@synthesize animating;
@dynamic animationFrameInterval;

// You must implement this method
+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

// The GL view is initialized by hand, so we need to init the rendering engine
- (id) initWithFrame:(CGRect)frame
{
    if ((self = [super initWithFrame:frame]))
	{
        if (![self initRenderingContext])
        {
            [self release];
            return nil;
        }
    }
    
    return self;
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id) initWithCoder:(NSCoder*)coder
{    
    if ((self = [super initWithCoder:coder]))
	{
        if (![self initRenderingContext])
        {
            [self release];
            return nil;
        }
    }
	
    return self;
}

// Generic metho to init the rendering context for the view
// This is called from initWithFrame and initWithCoder
- (BOOL) initRenderingContext
{
    // Enable multitouch on this view
    self.multipleTouchEnabled = YES;
    
    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    UIScreen* mainScreen = [UIScreen mainScreen];
    if ([mainScreen respondsToSelector:@selector(scale)])
    {
        eaglLayer.contentsScale = mainScreen.scale;
        NSLog(@"Using scale factor %f for device", mainScreen.scale);
    }
    
    eaglLayer.opaque = TRUE;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
	
    
    // set ressource path as current directory
    NSString * resourcePath = [[NSBundle mainBundle] resourcePath];
    chdir([resourcePath cStringUsingEncoding:NSUTF8StringEncoding]);
    
    /*
     renderer = [[ES2Renderer alloc] init];
     
     if (!renderer)
     {
     renderer = [[ES1Renderer alloc] init];
     
     if (!renderer)
     {
     [self release];
     return nil;
     }
     }*/
    
    
    // ES1
    renderer = [[ES1Renderer alloc] init];
    
    if (!renderer)
    {
        return NO;
    }
    
    
    // Maratis
    {
        // get engine
        MEngine * engine = MEngine::getInstance();
        
        // create virtual contexts
        soundContext = new MALContext();
        render = new MES1Context();
        physics = new MBulletContext();
        script = new MScript();
        input = new MInput();
        msystem = new MiOSContext();
        
        // create default Level and Game
        level = new MLevel();
        game = new MGame();
        
        // init MEngine
        engine->setSoundContext(soundContext); // sound context
        engine->setRenderingContext(render); // rendering context
        engine->setPhysicsContext(physics); // physics context
        engine->setScriptContext(script); // script context
        engine->setInputContext(input); // input context
        engine->setSystemContext(msystem); // system context
        
        engine->getImageLoader()->addLoader(M_loadImage); // image loader
                                                          //engine->getSoundLoader()->addLoader(M_loadSound); // sound loader
        engine->getLevelLoader()->addLoader(xmlLevelLoad); // level loader
                                                           //engine->getFontLoader()->addLoader(M_loadBinFont); // bin font loader
        
        // add some default "Maratis" behaviors : uncomment if wanted or add custom
        engine->getBehaviorManager()->addBehavior(MBLookAt::getStaticName(), M_OBJECT3D_CAMERA, MBLookAt::getNew);
        engine->getBehaviorManager()->addBehavior(MBFollow::getStaticName(), M_OBJECT3D, MBFollow::getNew);
        
        // add renderers
        //engine->getRendererManager()->addRenderer(MStandardRenderer::getStaticName(), MStandardRenderer::getNew);
        engine->getRendererManager()->addRenderer(MFixedRenderer::getStaticName(), MFixedRenderer::getNew);
        
        // mesh loader
        engine->getMeshLoader()->addLoader(xmlMeshLoad);
        engine->getArmatureAnimLoader()->addLoader(xmlArmatureAnimLoad);
        engine->getTexturesAnimLoader()->addLoader(xmlTextureAnimLoad);
        engine->getMaterialsAnimLoader()->addLoader(xmlMaterialAnimLoad);
        
        // set level
        engine->setLevel(level);
        
        // set game
        engine->setGame(game);
        
        // set renderer
        mrenderer = engine->getRendererManager()->getRendererByName("FixedRenderer")->getNewRenderer();
        engine->setRenderer(mrenderer);
        
        // configure accelerometer support
        [self configureMobileInput];
        
        char filename[256];
        getGlobalFilename(filename, msystem->getWorkingDirectory(), "Demos.mproj");
        loadProject(filename);
        
        // begin game
        game->begin();
    }
    
    animating = FALSE;
    displayLinkSupported = FALSE;
    animationFrameInterval = 1;
    displayLink = nil;
    animationTimer = nil;
    
    // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
    // class is used as fallback when it isn't available.
    NSString *reqSysVer = @"3.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
    {
        displayLinkSupported = TRUE;
    }
    
    return YES;
}

- (void) configureMobileInput
{
    // Create space for the touches dictionary
    for (int i = 0; i < 5; i++)
    {
        touchPointAvailable[i] = true;
    }
    
    touchPoints = CFDictionaryCreateMutable(kCFAllocatorDefault, 10, NULL, NULL);
    
    // Create three axis to handle the accelerometer data
    input->createAxis("ACCEL_X");
    input->createAxis("ACCEL_Y");
    input->createAxis("ACCEL_Z");
    
    // Setup the accelerometer object
    UIAccelerometer* accelerometer = [UIAccelerometer sharedAccelerometer];
    accelerometer.updateInterval = 1/50.0;
    accelerometer.delegate = self;
}

- (void) drawView:(id)sender
{
    [renderer render];
}

- (void) layoutSubviews
{
	[renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (NSInteger) animationFrameInterval
{
	return animationFrameInterval;
}

- (void) setAnimationFrameInterval:(NSInteger)frameInterval
{
	// Frame interval defines how many display frames must pass between each time the
	// display link fires. The display link will only fire 30 times a second when the
	// frame internal is two on a display that refreshes 60 times a second. The default
	// frame interval setting of one will fire 60 times a second when the display refreshes
	// at 60 times a second. A frame interval setting of less than one results in undefined
	// behavior.
	if (frameInterval >= 1)
	{
		animationFrameInterval = frameInterval;
		
		if (animating)
		{
			[self stopAnimation];
			[self startAnimation];
		}
	}
}

- (void) startAnimation
{
	if (!animating)
	{
		if (displayLinkSupported)
		{
			// CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
			// if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
			// not be called in system versions earlier than 3.1.

			displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
			[displayLink setFrameInterval:animationFrameInterval];
			[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		}
		else
        {
			animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];
		}
        
		animating = TRUE;
	}
}

- (void)stopAnimation
{
	if (animating)
	{
		if (displayLinkSupported)
		{
			[displayLink invalidate];
			displayLink = nil;
		}
		else
		{
			[animationTimer invalidate];
			animationTimer = nil;
		}
		
		animating = FALSE;
	}
}

- (void) accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
    // To provide some standard support on non-mobile devices
    input->setAxis("MOUSE_X", acceleration.x);
    input->setAxis("MOUSE_Y", acceleration.y);
    
    // Send data to accelerometer-specific axis
    input->setAxis("ACCEL_X", acceleration.x);
    input->setAxis("ACCEL_Y", acceleration.y);
    input->setAxis("ACCEL_Z", acceleration.z);
}

- (int) getAvailableTouchID
{
    for (int i = 0; i < 5; i++)
    {
        if (touchPointAvailable[i])
        {
            touchPointAvailable[i] = false;
            return i;
        }
    }
    
    return -1;
}

- (void) setTouchIDAvailable:(int)touchID
{
    touchPointAvailable[touchID] = true;
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Traverse all touches and create a new touch for each one
    for (UITouch* touch in [touches allObjects])
    {
        // Create a new TouchData object
        int* touchID = (int *)malloc(sizeof(int));
        *touchID = [self getAvailableTouchID];
        CFDictionarySetValue(touchPoints, touch, touchID);
        
        // Get the touch location
        CGPoint location = [touch locationInView:self];
        
        // Send the data to input
        input->beginTouch(*touchID, MVector2(location.x, location.y));
        
        NSLog(@"Began Touch %d at position %2.f, %2.f.", *touchID, location.x, location.y);
    }
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Traverse all touches and update each one
    for (UITouch* touch in [touches allObjects])
    {
        CGPoint location = [touch locationInView:self];
        int* touchID = (int *)CFDictionaryGetValue(touchPoints, touch);
        
        if (touchID != NULL)
        {
            // Send an updated touch to input
            input->updateTouch(*touchID, MVector2(location.x, location.y));
        
            NSLog(@"Updated Touch %d at position %2.f, %2.f.", *touchID, location.x, location.y);
        }
    }
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Traverse all touches and end them
    for (UITouch* touch in [touches allObjects])
    {
        CGPoint location = [touch locationInView:self];
        int* touchID = (int *)CFDictionaryGetValue(touchPoints, touch);
        
        if (touchID != NULL)
        {
            // Send the end touch to input
            input->endTouch(*touchID, MVector2(location.x, location.y));
        
            NSLog(@"Ended Touch %d at position %2.f, %2.f.", *touchID, location.x, location.y);
        
            // After updating input, set the touch index as available and release the memory
            [self setTouchIDAvailable:*touchID];
            CFDictionaryRemoveValue(touchPoints, touch);
        }
    }
}

- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Traverse all touches and cancell them
    for (UITouch* touch in [touches allObjects])
    {
        CGPoint location = [touch locationInView:self];
        int* touchID = (int *)CFDictionaryGetValue(touchPoints, touch);
        
        if (touchID != NULL)
        {
            // Send the end touch to input
            input->cancelTouch(*touchID, MVector2(location.x, location.y));
            
            NSLog(@"Cancelled Touch %d at position %2.f, %2.f.", *touchID, location.x, location.y);
            
            // After updating input, set the touch index as available and release the memory
            [self setTouchIDAvailable:*touchID];
            CFDictionaryRemoveValue(touchPoints, touch);
        }
    }
}

- (void) dealloc
{
	// Maratis
	{
		game->end();
		mrenderer->destroy();
		
		SAFE_DELETE(game);
		SAFE_DELETE(level);
		
		SAFE_DELETE(render);
		SAFE_DELETE(soundContext);
		SAFE_DELETE(physics);
		SAFE_DELETE(script);
		SAFE_DELETE(input);
		SAFE_DELETE(msystem);
	}
    
    // Release the touches dictionary
    CFRelease(touchPoints);
	
    [renderer release];
	
    [super dealloc];
}

@end
