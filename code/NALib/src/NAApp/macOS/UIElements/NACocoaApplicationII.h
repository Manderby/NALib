
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!

#include "../../../NAUtility/NANotifier.h"
#include "../../../NAUtility/NAString.h"



struct NACocoaApplication {
  NAApplication application;
};
NA_HAPI void na_DestructCocoaApplication(NACocoaApplication* cocoaApplication);
NA_RUNTIME_TYPE(NACocoaApplication, na_DestructCocoaApplication, NA_FALSE);




@interface NA_ApplicationHelper : NSObject {
  NAMutator postStartupFunction;
  NAMutator cleanupFunction;
  void* arg;
}
@end

@implementation NA_ApplicationHelper

- (void)setPostStartupFunction:(NAMutator)postUpdate {
  postStartupFunction = postUpdate;
}

- (void)setCleanupFunction:(NAMutator)cleanup {
  cleanupFunction = cleanup;
}

- (void)setPostStartupArg:(void*)newArg {
  arg = newArg;
}

- (void)handleDidFinishLaunching:(NSNotification *)notification {
  [self
    performSelector:@selector(handleDidFinishLaunchingAfterAll:)
    withObject:notification
    afterDelay:0];

  // We only needed this for the startup.
  [[NSNotificationCenter defaultCenter] removeObserver:self name:NSApplicationDidFinishLaunchingNotification object:nil]; 
}

- (void)handleDidFinishLaunchingAfterAll:(NSNotification *)notification {
  // Reaching here, we can be sure, all notifications of the DidFinishLaunching
  // type have been successfully processed and now it is time to initialize
  // the UI.

  // If this is a bare application without XIB
  if(![NSApp delegate]) {
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

- (void)handleDidBecomeActive:(NSNotification *)notification {
  // At this point, we get notified that the application has become active.
  // This means, after this notification has been fully delivered by the
  // notification center, we can call postStartup. But we need to wait for
  // all notifications to be handeled and therefore, we post another
  // notification which will then be processed right in the next cycle.
  
  [self
    performSelector:@selector(handleDidBecomeActiveAfterAll:)
    withObject:notification
    afterDelay:0];

  // We only needed this for the startup.
  [[NSNotificationCenter defaultCenter] removeObserver:self name:NSApplicationDidBecomeActiveNotification object:nil]; 
}

- (void)handleDidBecomeActiveAfterAll:(NSNotification *)notification {
  // Reaching here, we can be sure, all notifications of the DidBecomeActive
  // type have been successfully processed and now it is time to call the
  // postUpdate function.

  if(postStartupFunction) {
    postStartupFunction(arg);
  }
    
  // Make the application active
  // If this is a bare application without XIB
  if(![NSApp delegate]) {
    [NSApp activateIgnoringOtherApps:YES];
  }
}

- (void)handleApplicationWillTerminate:(NSNotification *)notification {
  na_TerminateApplication(cleanupFunction, arg);
}

- (void)handleApplicationDidChangeScreenParameters:(NSNotification *)notification {
  na_RenewApplicationScreens();
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
  if(!naIsRuntimeRunning()) {
    naStartRuntime();
  }

  // Start the shared application if not started already.
  [NSApplication sharedApplication];
  
  // After the following call, the Translator and Notifier will be ready.
  na_NewApplication();

  // Put an autorelease pool in place for the startup sequence.
  #if !NA_MACOS_USES_ARC
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  #endif
    // Call preStartup if desired.
    if(preStartup) {
      preStartup(arg);
    }

    // Set the preferred translator languages.
    naResetApplicationPreferredTranslatorLanguages();

    // Let the Macintosh System know that the app is ready to run.
    [NSApp finishLaunching];
    
  #if !NA_MACOS_USES_ARC
    [pool drain]; // also releases the pool. No separate release necessary.
  #endif

  // We want to get notified about the DidFinishLaunching and DidBecomeActive
  // notification because that is the point when the UI needs to be activated
  // and is ready to be built.
  NA_ApplicationHelper* applicationHelper = [[NA_ApplicationHelper alloc] init];
  [applicationHelper setPostStartupFunction: postStartup];
  [applicationHelper setCleanupFunction: cleanup];
  [applicationHelper setPostStartupArg: arg];

  [[NSNotificationCenter defaultCenter] 
    addObserver:applicationHelper
    selector:@selector(handleDidFinishLaunching:)
    name:NSApplicationDidFinishLaunchingNotification
    object:nil];
  [[NSNotificationCenter defaultCenter] 
    addObserver:applicationHelper
    selector:@selector(handleDidBecomeActive:)
    name:NSApplicationDidBecomeActiveNotification
    object:nil];
  [[NSNotificationCenter defaultCenter] 
    addObserver:applicationHelper
    selector:@selector(handleApplicationWillTerminate:)
    name:NSApplicationWillTerminateNotification
    object:nil];
  [[NSNotificationCenter defaultCenter] 
    addObserver:applicationHelper
    selector:@selector(handleApplicationDidChangeScreenParameters:)
    name:NSApplicationDidChangeScreenParametersNotification
    object:nil];
    
  // Start the event loop.
  NSDate* distantFuture = [NSDate distantFuture];
  while(na_IsApplicationRunning()) {
    #if !NA_MACOS_USES_ARC
      pool = [[NSAutoreleasePool alloc] init];
    #endif

      naCollectGarbage();

      NSEvent* curEvent = [NSApp nextEventMatchingMask:NAEventMaskAny untilDate:distantFuture inMode:NSDefaultRunLoopMode dequeue:YES];
//      NSEventType type = [curEvent type];
//      [curevent description]
//      NSEventSubtype subtype = [curEvent subtype];
//      printf("type %d, desc %s\n", (int)type, [[curEvent description] UTF8String]);

//      NSEventType type = [curEvent type];

      if(!na_InterceptKeyboardShortcut(curEvent)) {
        if(curEvent) {
          [NSApp sendEvent:curEvent];
        }
      }

      naRunNotifier();

    #if !NA_MACOS_USES_ARC
      [pool drain]; // also releases the pool. No separate release necessary.
    #endif
  }

  na_TerminateApplication(cleanup, arg);
}



NA_DEF void naResetApplicationPreferredTranslatorLanguages(void) {
  int32 langIndex = (int32)[[NSLocale preferredLanguages] count] - 1;
  while(langIndex >= 0) {
    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)langIndex];
    NALanguageCode3 langCode = naGetLanguageCode([language UTF8String]);
    naSetTranslatorLanguagePreference(langCode);
    langIndex--;
  }
}



NA_HDEF NAApplication* na_NewApplication(void) {
  NACocoaApplication* cocoaApplication = naNew(NACocoaApplication);

  na_InitApplication(&cocoaApplication->application, NA_COCOA_PTR_OBJC_TO_C(NSApp));

  return (NAApplication*)cocoaApplication;
}



NA_DEF void na_DestructCocoaApplication(NACocoaApplication* cocoaApplication) {
  na_ClearApplication((NAApplication*)cocoaApplication);
}



#if NA_DEBUG
  NA_DEF void naDebugUIElement(const void* elem) { NA_UNUSED(elem); }
  NA_HDEF void na_UndebugUIElement(const NA_UIElement* elem) { NA_UNUSED(elem); }
  NA_HDEF NABool na_IsUIElementBeingDebugged(const NA_UIElement* elem) { NA_UNUSED(elem); return NA_FALSE;}
#else
  NA_DEF void naDebugUIElement(const void* elem) { NA_UNUSED(elem); }
  NA_HDEF void na_UndebugUIElement(const NA_UIElement* elem) { NA_UNUSED(elem); }
  NA_HDEF NABool na_IsUIElementBeingDebugged(const NA_UIElement* elem) { NA_UNUSED(elem); return NA_FALSE;}
#endif



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff) {
  dispatch_time_t nextTime = dispatch_time(DISPATCH_TIME_NOW, naCastDoubleToi64(1000000000. * timediff));
  dispatch_queue_t queue = dispatch_get_main_queue();
  dispatch_after_f(nextTime, queue, arg, function);
}



NA_DEF void naOpenConsoleWindow(void) {
  // Does nothing on the Mac
}


#define NA_COCOA_BUNDLE_PLIST @"InfoPlist"
#define NA_COCOA_BUNDLE_APPLICATION_NAME @"CFBundleDisplayName"
#define NA_COCOA_BUNDLE_VERSION_SHORT_KEY @"CFBundleShortVersionString"
#define NA_COCOA_BUNDLE_VERSION_KEY @"CFBundleVersion"
#define NA_COCOA_BUNDLE_ICON_FILE_KEY @"CFBundleIconFile"

NA_DEF NAString* naNewApplicationName(void) {
  NAApplication* app = naGetApplication();
  if(app->appName) {
    return naNewStringExtraction(app->appName, 0, -1);
  }else{
    NSString* applicationName = [[NSBundle mainBundle] localizedStringForKey:NA_COCOA_BUNDLE_APPLICATION_NAME value:nil table:NA_COCOA_BUNDLE_PLIST];
    if(!applicationName) {
      applicationName = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_APPLICATION_NAME];
    }
    return naNewStringWithFormat("%s", [applicationName UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationCompanyName(void) {
  NAApplication* app = naGetApplication();
  if(app->companyName) {
    return naNewStringExtraction(app->companyName, 0, -1);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationVersionString(void) {
  NAApplication* app = naGetApplication();
  if(app->versionString) {
    return naNewStringExtraction(app->versionString, 0, -1);
  }else{
    NSString* versionString = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_SHORT_KEY];
    return naNewStringWithFormat("%s", [versionString UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationBuildString(void) {
  NAApplication* app = naGetApplication();
  if(app->buildString) {
    return naNewStringExtraction(app->buildString, 0, -1);
  }else{
    NSString* buildString = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_KEY];
    return naNewStringWithFormat("%s", [buildString UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationResourceBasePath(void) {
  NAApplication* app = naGetApplication();
  if(app->resourceBasePath) {
    return naNewStringExtraction(app->resourceBasePath, 0, -1);
  }else{
    return naNewExecutablePath();
  }
}

NA_DEF NAString* naNewApplicationIconPath(void) {
  NAApplication* app = naGetApplication();
  if(app->iconPath) {
    return naNewStringExtraction(app->iconPath, 0, -1);
  }else{
    NSString* iconFilename = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_ICON_FILE_KEY];
    NSString* iconBaseName = [iconFilename stringByDeletingPathExtension];
    NSURL* url = [[NSBundle mainBundle] URLForResource:iconBaseName withExtension:@"icns"];
    return naNewStringWithFormat("%s", [[url path] UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* path, const NAUTF8Char* baseBame, const NAUTF8Char* suffix) {
  NSURL* url;
  if(path && *path) {
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:baseBame] withExtension:[NSString stringWithUTF8String:suffix] subdirectory:[NSString stringWithUTF8String:path]];
  }else{
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:baseBame] withExtension:[NSString stringWithUTF8String:suffix]];
  }
  return naNewStringWithFormat("%s", [[url path] UTF8String]);
}

NA_HDEF void na_UpdateApplicationIconPath() {
  // nothing to be done on macOS
}

NA_DEF NAString* naNewExecutablePath(void) {
  return naNewStringWithFormat("%s", [[[NSBundle mainBundle] bundlePath] UTF8String]);
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
