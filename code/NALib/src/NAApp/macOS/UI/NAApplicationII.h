
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!

#include "../../../NAUtility/NANotifier.h"



@implementation NACocoaNativeApplicationDelegate

- (id) initWithCocoaApplication:(NACocoaApplication*)newCocoaApplication{
  self = [super init];
  cocoaApplication = newCocoaApplication;
  oldDelegate = NA_NULL;
  postStartupFunction = NA_NULL;
  postStartupArg = NA_NULL;
  return self;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
  NA_UNUSED(sender);
  naStopApplication();
  return NSTerminateCancel;
}

- (void)setOldDelegate:(NSObject <NSApplicationDelegate>*)delegate {
  oldDelegate = delegate;
}

- (void)setPostStartupFunction:(NAMutator)postUpdate {
  postStartupFunction = postUpdate;
}

- (void)setPostStartupArg:(void*)arg {
  postStartupArg = arg;
}

- (void)applicationWillFinishLaunching:(NSNotification *)notification{
  // forward the notification to the oldDelegate
  if(oldDelegate && oldDelegate != self
    && [oldDelegate respondsToSelector:@selector(applicationWillFinishLaunching:)]){
    [oldDelegate applicationWillFinishLaunching:notification];
  }
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  // forward the notification to the oldDelegate
  if(oldDelegate && oldDelegate != self
    && [oldDelegate respondsToSelector:@selector(applicationDidFinishLaunching:)]){
    [oldDelegate applicationDidFinishLaunching:notification];
  }
  
  // If this is a bare application without XIB
  if(!oldDelegate){
    // Show with a dock icon:
    [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
    // Without this event, the applicationDidBecomeActive will not be called.
    NSEvent* activationEvent = [NSEvent
      otherEventWithType: NSEventTypeAppKitDefined
      location: NSMakePoint(0, 0)
      modifierFlags: 0
      timestamp: 0
      windowNumber: 0
      context: nil
      subtype: NSEventSubtypeApplicationActivated
      data1: 0
      data2: 0];
    [NSApp sendEvent:activationEvent];
  }
}

- (void)applicationWillBecomeActive:(NSNotification *)notification{
  // forward the notification to the oldDelegate
  if(oldDelegate && oldDelegate != self
    && [oldDelegate respondsToSelector:@selector(applicationWillBecomeActive:)]){
    [oldDelegate applicationWillBecomeActive:notification];
  }
}

- (void)applicationDidBecomeActive:(NSNotification *)notification{
  // forward the notification to the oldDelegate
  if(oldDelegate && oldDelegate != self
    && [oldDelegate respondsToSelector:@selector(applicationDidBecomeActive:)]){
    [oldDelegate applicationDidBecomeActive:notification];
  }

  // Call postStartup the first time if desired.
  if(postStartupFunction){
    postStartupFunction(postStartupArg);
    
    // If this is a bare application without XIB
    if(!oldDelegate){
      // Make the application active
      [NSApp activateIgnoringOtherApps:YES];
    }
  }
  
  // Give up this delegate and return it to the previous delegate.
  [NSApp setDelegate:oldDelegate];
}

@end


// Interesting read:
// https://lapcatsoftware.com/blog/2007/03/10/everything-you-always-wanted-to-know-about-nsapplication/
NA_DEF void naStartApplication(
  NAMutator preStartup,
  NAMutator postStartup,
  NAMutator cleanup,
  void* arg)
{  
  // Start the shared application if not started already and set the nativePtr
  // of the application.
  [NSApplication sharedApplication];
  NAApplication* app = na_NewApplication();

  // Put an autorelease pool in place for the startup sequence.
  #if !NA_MACOS_USES_ARC
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  #endif
    // Call preStartup if desired.
    if(preStartup){
      preStartup(arg);
    }

    // Set the preferred translator languages.
    naResetApplicationPreferredTranslatorLanguages();

    // Let the Macintosh System know that the app is ready to run.
    [NSApp finishLaunching];
    
  #if !NA_MACOS_USES_ARC
    [pool drain]; // also releases the pool. No separate release necessary.
  #endif

  NACocoaNativeApplicationDelegate* nativeApp = (NA_COCOA_BRIDGE NACocoaNativeApplicationDelegate*)naGetUIElementNativePtr(app);

  // Hijack the delegate during startup without losing the old delegate
  // which might have already been set by the user or in a XIB file.
  NSObject <NSApplicationDelegate>* oldDelegate = [NSApp delegate];
  [nativeApp setOldDelegate:oldDelegate];
  [nativeApp setPostStartupFunction: postStartup];
  [nativeApp setPostStartupArg: arg];
  [NSApp setDelegate:nativeApp];

  NANotifier* notifier = naAllocNotifier();
  naSetCurrentNotifier(notifier);

  // Start the event loop.
  NSDate* distantFuture = [NSDate distantFuture];
  while(na_IsApplicationRunning()){
    #if !NA_MACOS_USES_ARC
      pool = [[NSAutoreleasePool alloc] init];
    #endif
      NSEvent* curEvent = [NSApp nextEventMatchingMask:NAEventMaskAny untilDate:distantFuture inMode:NSDefaultRunLoopMode dequeue:YES];
//      NSEventType type = [curEvent type];
//      [curevent description]
//      NSEventSubtype subtype = [curEvent subtype];
//      printf("type %d, desc %s\n", (int)type, [[curEvent description] UTF8String]);
//      NSEventSubtypeWindowMoved

      naCollectGarbage();
      if(!na_InterceptKeyboardShortcut(curEvent)){
        if(curEvent){
          [NSApp sendEvent:curEvent];
        }
      }

      naRunNotifier();

    #if !NA_MACOS_USES_ARC
      [pool drain]; // also releases the pool. No separate release necessary.
    #endif
  }

  naDeallocNotifier(notifier);

  // Before deleting the application, we cleanup whatever the user needs to
  // clean up.
  if(cleanup)
    cleanup(arg);

  // When reaching here, the application had been stopped.
  naDelete(app);
}



NA_DEF void naResetApplicationPreferredTranslatorLanguages(void){
  NAInt langIndex = (NAInt)[[NSLocale preferredLanguages] count] - 1;
  while(langIndex >= 0){
    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)langIndex];
    NALanguageCode3 langCode = naGetLanguageCode([language UTF8String]);
    naSetTranslatorLanguagePreference(langCode);
    langIndex--;
  }
}



NA_HDEF NAApplication* na_NewApplication(void){
  NACocoaApplication* cocoaApplication = naNew(NACocoaApplication);

  NACocoaNativeApplicationDelegate* nativePtr = [[NACocoaNativeApplicationDelegate alloc]
    initWithCocoaApplication:cocoaApplication];
  na_InitApplication(&(cocoaApplication->application), NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  return (NAApplication*)cocoaApplication;
}



NA_DEF void na_DestructCocoaApplication(NACocoaApplication* cocoaApplication){
  na_ClearApplication((NAApplication*)cocoaApplication);
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  dispatch_time_t nextTime = dispatch_time(DISPATCH_TIME_NOW, naMakei64WithDouble(1000000000. * timediff));
  dispatch_queue_t queue = dispatch_get_main_queue();
  dispatch_after_f(nextTime, queue, arg, function);
}



NA_DEF void naOpenConsoleWindow(void){
  // Does nothing on the Mac
}


#define NA_COCOA_BUNDLE_PLIST @"InfoPlist"
#define NA_COCOA_BUNDLE_APPLICATION_NAME @"CFBundleDisplayName"
#define NA_COCOA_BUNDLE_VERSION_SHORT_KEY @"CFBundleShortVersionString"
#define NA_COCOA_BUNDLE_VERSION_KEY @"CFBundleVersion"
#define NA_COCOA_BUNDLE_ICON_FILE_KEY @"CFBundleIconFile"

NA_DEF NAString* naNewApplicationName(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->application.name){
    return naNewStringWithFormat("%s", app->application.name);
  }else{
    NSString* applicationName = [[NSBundle mainBundle] localizedStringForKey:NA_COCOA_BUNDLE_APPLICATION_NAME value:nil table:NA_COCOA_BUNDLE_PLIST];
    if(!applicationName){
      applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_APPLICATION_NAME];
    }
    return naNewStringWithFormat("%s", [applicationName UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationCompanyName(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->application.companyName){
    return naNewStringWithFormat("%s", app->application.companyName);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationVersionString(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->application.versionString){
    return naNewStringWithFormat("%s", app->application.versionString);
  }else{
    NSString* versionString = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_SHORT_KEY];
    return naNewStringWithFormat("%s", [versionString UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationBuildString(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->application.buildString){
    return naNewStringWithFormat("%s", app->application.buildString);
  }else{
    NSString* buildString = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_KEY];
    return naNewStringWithFormat("%s", [buildString UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationIconPath(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->application.iconPath){
    return naNewStringWithFormat("%s", app->application.iconPath);
  }else{
    NSString* iconFilename = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_ICON_FILE_KEY];
    NSString* iconBasename = [iconFilename stringByDeletingPathExtension];
    NSURL* url = [[NSBundle mainBundle] URLForResource:iconBasename withExtension:@"icns"];
    return naNewStringWithFormat("%s", [[url path] UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NSURL* url;
  if(dir){
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix] subdirectory:[NSString stringWithUTF8String:dir]];
  }else{
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix]];
  }
  return naNewStringWithFormat("%s", [[url path] UTF8String]);
}

// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
