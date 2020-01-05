
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


@implementation NACocoaApplicationDelegate
- (id) initWithCocoaApplication:(NACocoaApplication*)newcocoaapplication{
  self = [super init];
  cocoaapplication = newcocoaapplication;
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



NA_DEF void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  // The ((id (*)(id, SEL)) part is a cast of the objc_msgSend function which
  // is requires since a later version of Objective-C
//  ( (id (*)(id, SEL)) objc_msgSend)(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));

  NAApplication* app;
  NSDate* distantfuture;
  #if !NA_MACOS_USES_ARC
    NSAutoreleasePool* pool;
  #endif
  
  // Start the Core application if not started already and set the native ID
  // of the application.
  [NSApplication sharedApplication];
  app = naNewApplication();

  // Put an autorelease pool in place for the startup sequence.
  #if !NA_MACOS_USES_ARC
    pool = [[NSAutoreleasePool alloc] init];
  #endif
    // Call prestartup if desired.
    if(prestartup){prestartup(arg);}

    // Set the preferred translator languages.
    naResetApplicationPreferredTranslatorLanguages();

    // Let the Macintosh System know that the app is ready to run.
    [NSApp finishLaunching];
    
    // Call poststartup if desired.
    if(poststartup){poststartup(arg);}
  #if !NA_MACOS_USES_ARC
    [pool drain];
  #endif

  NA_UNUSED(app);
  [NSApp setDelegate:(NA_COCOA_BRIDGE id)naGetUIElementNativeID(app)];
  
//  SEL selector = @selector(doSomethingWithObject:afterDelay:);
//  IMP newImp = (IMP)swizzledDoSometingWithObjectAfterDelay;
//  Method method = class_getClassMethod([MyClass class], selector);
//  const char * encoding = method_getTypeEncoding(method);
//  class_replaceMethod([MyClass class], selector, newIMP, encoding);

  // Start the event loop.
  distantfuture = [NSDate distantFuture];
  while(naIsCoreApplicationRunning()){
    NSEvent* curevent;
    #if !NA_MACOS_USES_ARC
      pool = [[NSAutoreleasePool alloc] init];
    #endif
      curevent = [NSApp nextEventMatchingMask:NAEventMaskAny untilDate:distantfuture inMode:NSDefaultRunLoopMode dequeue:YES];
//      if([curevent type] == NSEventType)
      naCollectGarbage();
      if(!naInterceptKeyboardShortcut(curevent)){
        if(curevent){[NSApp sendEvent:curevent];}
      }
    #if !NA_MACOS_USES_ARC
      [pool drain];
    #endif
  }

  // When reaching here, the application had been stopped.
  naReleaseUIElement(app);
}




NA_DEF void naResetApplicationPreferredTranslatorLanguages(void){
  NAInt lang = (NAInt)[[NSLocale preferredLanguages] count] - 1;
  while(lang >= 0){
    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)lang];
    NALanguageCode3 langcode = naGetLanguageCode([language UTF8String]);
    naSetTranslatorLanguagePreference(langcode);
    lang--;
  }
}



NA_HDEF NAApplication* naNewApplication(void){
  NACocoaApplication* cocoaapplication = naAlloc(NACocoaApplication);

  NACocoaApplicationDelegate* cocoaappdelegate = [[NACocoaApplicationDelegate alloc] initWithCocoaApplication:cocoaapplication];

  naInitCoreApplication(&(cocoaapplication->coreapplication), NA_COCOA_PTR_OBJC_TO_C(cocoaappdelegate));

  return (NAApplication*)cocoaapplication;
}



NA_DEF void naDestructApplication(NAApplication* application){
  NACocoaApplication* cocoaapplication = (NACocoaApplication*)application;
  // Do not clear the core application. It will only call this method again.
  naClearCoreApplication(&(cocoaapplication->coreapplication));
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  dispatch_time_t nexttime = dispatch_time(DISPATCH_TIME_NOW, (int64)(1000000000. * timediff));
  dispatch_queue_t queue = dispatch_get_main_queue();
  dispatch_after_f(nexttime, queue, arg, function);
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
  if(app->coreapplication.name){
    return naNewStringWithFormat("%s", app->coreapplication.name);
  }else{
    NSString* applicationname = [[NSBundle mainBundle] localizedStringForKey:NA_COCOA_BUNDLE_APPLICATION_NAME value:nil table:NA_COCOA_BUNDLE_PLIST];
    if(!applicationname){
      applicationname = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_APPLICATION_NAME];
    }
    return naNewStringWithFormat("%s", [applicationname UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationCompanyName(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->coreapplication.companyName){
    return naNewStringWithFormat("%s", app->coreapplication.companyName);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationVersionString(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->coreapplication.versionString){
    return naNewStringWithFormat("%s", app->coreapplication.versionString);
  }else{
    NSString* versionstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_SHORT_KEY];
    return naNewStringWithFormat("%s", [versionstring UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationBuildString(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->coreapplication.buildString){
    return naNewStringWithFormat("%s", app->coreapplication.buildString);
  }else{
    NSString* buildstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_KEY];
    return naNewStringWithFormat("%s", [buildstring UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationIconPath(void){
  NACocoaApplication* app = (NACocoaApplication*)naGetApplication();
  if(app->coreapplication.iconPath){
    return naNewStringWithFormat("%s", app->coreapplication.iconPath);
  }else{
    NSString* iconfilename = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_ICON_FILE_KEY];
    NSString* iconbasename = [iconfilename stringByDeletingPathExtension];
    NSURL* url = [[NSBundle mainBundle] URLForResource:iconbasename withExtension:@"icns"];
    return naNewStringWithFormat("%s", [[url path] UTF8String]);
  }
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NSURL* url;
  NAString* retstring;
  if(dir){
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix] subdirectory:[NSString stringWithUTF8String:dir]];
  }else{
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix]];
  }
  retstring = naNewStringWithFormat("%s", [[url path] UTF8String]);
  return retstring;
}



// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
