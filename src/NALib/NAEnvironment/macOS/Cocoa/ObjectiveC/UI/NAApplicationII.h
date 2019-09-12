
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


@implementation NANativeApplicationDelegate
- (id) initWithCoreApplication:(NACoreApplication*)newcoreapplication{
  self = [super init];
  coreapplication = newcoreapplication;
  return self;
}
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
  NA_UNUSED(sender);
  naStopApplication();
  return NSTerminateCancel;
}
- (void)applicationDidFinishLaunching:(NSNotification *)notification{
  [NSApp applicationDidFinishLaunching:notification];
}
@end



NA_DEF void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  // The ((id (*)(id, SEL)) part is a cast of the objc_msgSend function which
  // is requires since a later version of Objective-C
//  ( (id (*)(id, SEL)) objc_msgSend)(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));

  // Start the Core application and set the native ID of the application.
  [NSApplication sharedApplication];
  NAApplication* app = naNewApplication();

  // Put an autorelease pool in place for the startup sequence.
  #if !__has_feature(objc_arc)
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  #endif
    // Call prestartup if desired.
    if(prestartup){prestartup(arg);}

    // Set the preferred translator languages.
    naResetApplicationPreferredTranslatorLanguages();

    // Let the Macintosh System know that the app is ready to run.
    [NSApp finishLaunching];
    
    // Call poststartup if desired.
    if(poststartup){poststartup(arg);}
  #if !__has_feature(objc_arc)
    [pool drain];
  #endif

  [NSApp setDelegate:naGetUIElementNativeID(app)];
  
//  SEL selector = @selector(doSomethingWithObject:afterDelay:);
//  IMP newImp = (IMP)swizzledDoSometingWithObjectAfterDelay;
//  Method method = class_getClassMethod([MyClass class], selector);
//  const char * encoding = method_getTypeEncoding(method);
//  class_replaceMethod([MyClass class], selector, newIMP, encoding);

  // Start the event loop.
  NSDate* distantfuture = [NSDate distantFuture];
  while(naIsCoreApplicationRunning()){
    #if !__has_feature(objc_arc)
      pool = [[NSAutoreleasePool alloc] init];
    #endif
      NSEvent* curevent = [NSApp nextEventMatchingMask:NAEventMaskAny untilDate:distantfuture inMode:NSDefaultRunLoopMode dequeue:YES];
//      if([curevent type] == NSEventType)
      naCollectGarbage();
      if(!naInterceptKeyboardShortcut(curevent)){
        if(curevent){[NSApp sendEvent:curevent];}
      }
    #if !__has_feature(objc_arc)
      [pool drain];
    #endif
  }

  // When reaching here, the application had been stopped.
//  naClearCoreApplication();
//  naReleaseUIElement(app);
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



NA_DEF NAApplication* naNewApplication(void){
  NACoreApplication* coreapplication = naAlloc(NACoreApplication);

  NANativeApplicationDelegate* nativeappdelegate = [[NANativeApplicationDelegate alloc] initWithCoreApplication:coreapplication];

  naInitCoreApplication(coreapplication, (void*)NA_COCOA_RETAIN(nativeappdelegate));

  return (NAApplication*)coreapplication;
}


NA_DEF void naDestructApplication(NAApplication* application){
  NACoreApplication* coreapplication = (NACoreApplication*)application;
  naClearCoreApplication(coreapplication);
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  dispatch_time_t nexttime = dispatch_time(DISPATCH_TIME_NOW, (int64)(1000000000. * timediff));
  dispatch_queue_t queue = dispatch_get_main_queue();
  dispatch_after_f(nexttime, queue, arg, function);
}



NA_DEF void naOpenConsoleWindow(const NAUTF8Char* windowtitle){
  NA_UNUSED(windowtitle);
  // Does nothing on the Mac
}


#define NA_COCOA_BUNDLE_PLIST @"InfoPlist"
#define NA_COCOA_BUNDLE_APPLICATION_NAME @"CFBundleDisplayName"
#define NA_COCOA_BUNDLE_VERSION_SHORT_KEY @"CFBundleShortVersionString"
#define NA_COCOA_BUNDLE_VERSION_KEY @"CFBundleVersion"
#define NA_COCOA_BUNDLE_ICON_FILE_KEY @"CFBundleIconFile"

NAString* naNewBundleApplicationName(void){
  NSString* applicationname = [[NSBundle mainBundle] localizedStringForKey:NA_COCOA_BUNDLE_APPLICATION_NAME value:nil table:NA_COCOA_BUNDLE_PLIST];
  if(!applicationname){
    applicationname = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_APPLICATION_NAME];
  }
  NAString* retstring = naNewStringWithFormat("%s", [applicationname UTF8String]);
  return retstring;
}

NAString* naNewBundleVersionString(void){
  NSString* versionstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_SHORT_KEY];
  NAString* retstring = naNewStringWithFormat("%s", [versionstring UTF8String]);
  return retstring;
}

NAString* naNewBundleBuildString(void){
  NSString* buildstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_KEY];
  NAString* retstring = naNewStringWithFormat("%s", [buildstring UTF8String]);
  return retstring;
}

NAString* naNewBundleIconPath(void){
  NSString* iconfilename = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_ICON_FILE_KEY];
  NSString* iconbasename = [iconfilename stringByDeletingPathExtension];
  NSURL* url = [[NSBundle mainBundle] URLForResource:iconbasename withExtension:@"icns"];
  NAString* retstring = naNewStringWithFormat("%s", [[url path] UTF8String]);
  return retstring;
}

NAString* naNewBundleResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NSURL* url;
  if(dir){
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix] subdirectory:[NSString stringWithUTF8String:dir]];
  }else{
    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix]];
  }
  NAString* retstring = naNewStringWithFormat("%s", [[url path] UTF8String]);
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
