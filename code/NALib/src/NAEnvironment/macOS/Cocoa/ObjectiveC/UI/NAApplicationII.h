
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NACocoaApplication NACocoaApplication;
struct NACocoaApplication {
  NAApplication application;
};

NA_HAPI void na_DestructCocoaApplication(NACocoaApplication* cocoaApplication);
NA_RUNTIME_TYPE(NACocoaApplication, na_DestructCocoaApplication, NA_FALSE);



@interface NACocoaNativeApplicationDelegate : NSObject <NSApplicationDelegate>{
  NACocoaApplication* cocoaApplication;
}
@end



@implementation NACocoaNativeApplicationDelegate

- (id) initWithCocoaApplication:(NACocoaApplication*)newCocoaApplication{
  self = [super init];
  cocoaApplication = newCocoaApplication;
  return self;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
  NA_UNUSED(sender);
  naStopApplication();
  return NSTerminateCancel;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  if([NSApp delegate] && [NSApp delegate] != self){
    [[NSApp delegate] applicationDidFinishLaunching:notification];
  }
}

@end



NA_DEF void naStartApplication(NAMutator preStartup, NAMutator postStartup, void* arg){  
  // Start the shared application if not started already and set the nativePtr
  // of the application.
  [NSApplication sharedApplication];
  NAApplication* app = na_NewApplication();

  // Put an autorelease pool in place for the startup sequence.
  #if !NA_MACOS_USES_ARC
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  #endif
    // Call preStartup if desired.
    if(preStartup){preStartup(arg);}

    // Set the preferred translator languages.
    naResetApplicationPreferredTranslatorLanguages();

    // Let the Macintosh System know that the app is ready to run.
    [NSApp finishLaunching];
    
    // Call postStartup if desired.
    if(postStartup){postStartup(arg);}
  #if !NA_MACOS_USES_ARC
    [pool drain]; // also releases the pool. No separate release necessary.
  #endif

//  [NSApp setDelegate:naGetUIElementNativePtr(app)];

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
        if(curEvent){[NSApp sendEvent:curEvent];}
      }
    #if !NA_MACOS_USES_ARC
      [pool drain]; // also releases the pool. No separate release necessary.
    #endif
  }

  // When reaching here, the application had been stopped.
  naDelete(app);
}



NA_DEF void naResetApplicationPreferredTranslatorLanguages(void){
  NAInt lang = (NAInt)[[NSLocale preferredLanguages] count] - 1;
  while(lang >= 0){
    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)lang];
    NALanguageCode3 langCode = naGetLanguageCode([language UTF8String]);
    naSetTranslatorLanguagePreference(langCode);
    lang--;
  }
}



NA_HDEF NAApplication* na_NewApplication(void){
  NACocoaApplication* cocoaApplication = naNew(NACocoaApplication);

  NACocoaNativeApplicationDelegate* nativePtr = [[NACocoaNativeApplicationDelegate alloc]
    initWithCocoaApplication:cocoaApplication];
  na_InitApplication(&(cocoaApplication->application), NA_COCOA_PTR_OBJC_TO_C(nativePtr));

//  na_InitApplication(&(cocoaApplication->application), NA_COCOA_PTR_OBJC_TO_C(NSApp));

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
