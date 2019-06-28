
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is a .m-File which is an implementation file in Objective-C.
// This is required on Macintosh systems to link against the Cocoa-Framework.
// If you do not have an Objective-C compiler such as in windows, this file
// will simply be ignored and hence nothing is linked. If this is not the case,
// you can simply delete the .m File from your source tree.


#include "NAUICocoa.h"


#if (NA_CONFIG_COMPILE_GUI == 1)
#ifdef __OBJC__
#if NA_OS == NA_OS_MAC_OS_X
// Now, we are sure, we compile with Objective-C and on MacOSX. The two
// #if directives will be closed at the very bottom of this file.



#include "NAMemory.h"
#include "NACoord.h"
#include "NAThreading.h"
#include "NATranslator.h"


// Mapping of deprecated entities
#if !defined __MAC_10_12
  #define NAEventMaskAny                  NSAnyEventMask
  #define NAWindowStyleMaskTitled         NSTitledWindowMask
  #define NAWindowStyleMaskClosable       NSClosableWindowMask
  #define NAWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
  #define NAWindowStyleMaskResizable      NSResizableWindowMask
  #define NAWindowStyleMaskBorderless     NSBorderlessWindowMask
  #define NAEventModifierFlagShift        NSShiftKeyMask
  #define NAEventModifierFlagOption       NSAlternateKeyMask
  #define NAEventModifierFlagControl      NSControlKeyMask
  #define NAEventModifierFlagCommand      NSCommandKeyMask
#else
  #define NAEventMaskAny                  NSEventMaskAny
  #define NAWindowStyleMaskTitled         NSWindowStyleMaskTitled
  #define NAWindowStyleMaskClosable       NSWindowStyleMaskClosable
  #define NAWindowStyleMaskMiniaturizable NSWindowStyleMaskMiniaturizable
  #define NAWindowStyleMaskResizable      NSWindowStyleMaskResizable
  #define NAWindowStyleMaskBorderless     NSWindowStyleMaskBorderless
  #define NAEventModifierFlagShift        NSEventModifierFlagShift
  #define NAEventModifierFlagOption       NSEventModifierFlagOption
  #define NAEventModifierFlagControl      NSEventModifierFlagControl
  #define NAEventModifierFlagCommand      NSEventModifierFlagCommand
#endif


NA_HAPI NARect naGetApplicationAbsoluteRect(void);
NA_HAPI NARect naGetScreenAbsoluteRect(NACoreUIElement* screen);
NA_HAPI NARect naGetWindowAbsoluteOuterRect(NACoreUIElement* window);
NA_HAPI NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space);
NA_HAPI NARect naGetButtonAbsoluteInnerRect(NACoreUIElement* space);

NA_HAPI void naRenewWindowMouseTracking(NACocoaWindow* cocoawindow);
NA_HAPI void naClearWindowMouseTracking(NACocoaWindow* cocoawindow);


// ///////////////////////////////////
// APPLICATION
// ///////////////////////////////////

NA_DEF void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  // The ((id (*)(id, SEL)) part is a cast of the objc_msgSend function which
  // is requires since a later version of Objective-C
//  ( (id (*)(id, SEL)) objc_msgSend)(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));

  // Start the Cocoa application and set the native ID of the application.
  [NSApplication sharedApplication];
  
//  NSMenu* myMenu = [[NSMenu alloc] initWithTitle:@"Wurst"];
//  NSMenuItem* mainMenuItem = [[NSMenuItem alloc] initWithTitle:@"Superwurst" action:NA_NULL keyEquivalent:@"A"];
//  NSMenu* subMenu = [[NSMenu alloc] initWithTitle:@"SubWurst"];
////  [mainMenuItem setMenu:subMenu];
//  [myMenu addItem:mainMenuItem];  
//  [NSApp setMainMenu:myMenu];

//  NSImage* myIcon = [[NSImage alloc] initWithContentsOfFile:@"~/Desktop/nalibtitle.png"];
//  [NSApp setApplicationIconImage:myIcon];

//  NSBundle* bundle = NA_NULL;
  NSBundle* bundle = [NSBundle mainBundle];
  NSDictionary<NSString *, id>* plist = [bundle infoDictionary];
  
//  NSAppearance* appearance = [[NSAppearance alloc] initWithAppearanceNamed:NSAppearanceNameVibrantDark bundle:bundle];
//  [NSApp setAppearance:appearance];
//  [NSApp setPresentationOptions:NSApplicationPresentationHideDock];


  [plist setValue:@"ch.manderc.testapp" forKey:[NSString stringWithUTF8String:CFStringGetCStringPtr(kCFBundleIdentifierKey, kCFStringEncodingUTF8)]];
  
//    [plist setValue:@"Knackwurst" forKey:[NSString stringWithUTF8String:CFStringGetCStringPtr(kCFBundleNameKey, kCFStringEncodingUTF8)]];

//  [NSApp finishLaunching];
  

  
  #if __has_feature(objc_arc)
    naStartCoreApplication(sizeof(NACocoaApplication), (NANativeID)CFBridgingRetain(NSApp));
  #else
    naStartCoreApplication(sizeof(NACocoaApplication), (NANativeID)NSApp);
  #endif


  // Put an autorelease pool in place for the startup sequence.
  #if !__has_feature(objc_arc)
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  #endif
    // Call prestartup if desired.
    if(prestartup){prestartup(arg);}

    // Set the preferred translator languages.
    NAInt lang = (NAInt)[[NSLocale preferredLanguages] count] - 1;
    while(lang >= 0){
      NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)lang];
      NALanguageCode3 langcode = naGetLanguageCode([language UTF8String]);
      naSetTranslatorLanguagePreference(langcode);
      lang--;
    }

    // Let the Macintosh System know that the app is ready to run.
    [NSApp finishLaunching];
    
    // Call poststartup if desired.
    if(poststartup){poststartup(arg);}
  #if !__has_feature(objc_arc)
    [pool drain];
  #endif

  // Start the event loop.
  NSDate* distantfuture = [NSDate distantFuture];
  while(naIsCoreApplicationRunning()){
    #if !__has_feature(objc_arc)
      pool = [[NSAutoreleasePool alloc] init];
    #endif
      NSEvent* curevent = [NSApp nextEventMatchingMask:NAEventMaskAny untilDate:distantfuture inMode:NSDefaultRunLoopMode dequeue:YES];
      naCollectGarbage();
      if(curevent){[NSApp sendEvent:curevent];}
    #if !__has_feature(objc_arc)
      [pool drain];
    #endif
  }

  // When reaching here, the application had been stopped.
  naClearCoreApplication();
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  dispatch_time_t nexttime = dispatch_time(DISPATCH_TIME_NOW, (int64)(1000000000. * timediff));
  dispatch_queue_t queue = dispatch_get_main_queue();
  dispatch_after_f(nexttime, queue, arg, function);
}



NA_DEF void naOpenConsoleWindow(const char* windowtitle){
  NA_UNUSED(windowtitle);
  // Does nothing on the Mac
}





// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////


NA_DEF void naRefreshUIElementNow(NAUIElement* uielement){
  #if __has_feature(objc_arc)
  [((NSView*)CFBridgingRelease(naGetUIElementNativeID(uielement))) setNeedsDisplay:YES];
  #else
    [((NSView*)naGetUIElementNativeID(uielement)) setNeedsDisplay:YES];
  #endif
}


NA_DEF void naSetUIElementParent (NAUIElement* uielement, NAUIElement* parent){
  NACoreUIElement* coreelement = (NACoreUIElement*)uielement;
  // todo: remove from old parent
  coreelement->parent = parent;
}




















NA_HDEF NARect naGetApplicationAbsoluteRect(void){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetScreenAbsoluteRect(NACoreUIElement* screen){
  NARect rect;
  NSRect frame;
  NSRect mainframe;
  mainframe = [[NSScreen mainScreen] frame];
  #if __has_feature(objc_arc)
    frame = [(NSScreen*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)screen)) frame];
  #else
    frame = [(NSScreen*)(naGetUIElementNativeID((NAUIElement*)screen)) frame];
  #endif
  rect.pos.x = frame.origin.x;
  rect.pos.y = mainframe.size.height - frame.size.height - frame.origin.y;
  rect.size.width = frame.size.width;
  rect.size.height = frame.size.height;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window){
  NARect rect;
  NSRect contentrect;
  NSRect windowframe;
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)window));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  #endif
  contentrect = [[nativewindow contentView] frame];
  windowframe = [nativewindow frame];
  rect.pos.x = windowframe.origin.x + contentrect.origin.x;
  rect.pos.y = windowframe.origin.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteOuterRect(NACoreUIElement* window){
  NARect rect;
  NSRect windowframe;
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)window));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  #endif
  windowframe = [nativewindow frame];
  rect.pos.x = windowframe.origin.x;
  rect.pos.y = windowframe.origin.y;
  rect.size.width = windowframe.size.width;
  rect.size.height = windowframe.size.height;
  return rect;
}



NA_HDEF NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space){
  NARect rect;
  NSRect contentrect;
  NARect windowrect;
  // Warning: does not work when frame unequal bounds.
  #if __has_feature(objc_arc)
    contentrect = [(NSView*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)space)) frame];
  #else
    contentrect = [(NSView*)(naGetUIElementNativeID((NAUIElement*)space)) frame];
  #endif
  windowrect = naGetWindowAbsoluteInnerRect((NACoreUIElement*)naGetUIElementWindow((NAUIElement*)space));
  rect.pos.x = windowrect.pos.x + contentrect.origin.x;
  rect.pos.y = windowrect.pos.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_DEF NARect naGetUIElementRect(NAUIElement* uielement, NAUIElement* relativeuielement, NABool includeborder){
  NARect rect;
  NARect relrect;
  NACoreUIElement* element;
  NACoreUIElement* relelement;

  element = (NACoreUIElement*)uielement;
  relelement = (NACoreUIElement*)relativeuielement;
  NAApplication* app = naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(element == (NACoreUIElement*)app){
    #ifndef NDEBUG
      if(relelement && (relelement != (NACoreUIElement*)app))
        naError("The relative element is invalid for the given uielement, which seems to be the application.");
    #endif
    return naGetApplicationAbsoluteRect();
  }

  // Now, we find the appropriate relative element.
  if(!relelement){relelement = (NACoreUIElement*)naGetUIElementParent((NAUIElement*)element);}

  switch(element->elementtype){
  case NA_UI_APPLICATION: rect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = naGetScreenAbsoluteRect(element); break;
  case NA_UI_WINDOW:
    if(includeborder){
      rect = naGetWindowAbsoluteOuterRect(element);
    }else{
      rect = naGetWindowAbsoluteInnerRect(element);
    }
    break;
  case NA_UI_VIEW :       rect = naGetSpaceAbsoluteInnerRect(element); break;
  case NA_UI_OPENGLVIEW:  rect = naGetSpaceAbsoluteInnerRect(element); break;
  case NA_UI_BUTTON :     rect = naGetButtonAbsoluteInnerRect(element); break;
  }

  switch(relelement->elementtype){
  case NA_UI_APPLICATION: relrect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      relrect = naGetScreenAbsoluteRect(relelement); break;
  case NA_UI_WINDOW:      relrect = naGetWindowAbsoluteInnerRect(relelement); break;
  case NA_UI_VIEW:        relrect = naGetSpaceAbsoluteInnerRect(relelement); break;
  case NA_UI_OPENGLVIEW:  relrect = naGetSpaceAbsoluteInnerRect(relelement); break;
  case NA_UI_BUTTON:      relrect = naGetButtonAbsoluteInnerRect(relelement); break;
  }

  rect.pos.x = rect.pos.x - relrect.pos.x;
  rect.pos.y = rect.pos.y - relrect.pos.y;
  rect.size.width = rect.size.width;
  rect.size.height = rect.size.height;

  // Convert the rect into absolute coordinates.

  return rect;
}









// ////////////////////////////
// Window

@implementation NANativeWindow
- (id) initWithCocoaWindow:(NACocoaWindow*)newcocoawindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  cocoawindow = newcocoawindow;
  trackingcount = 0;
  trackingarea = nil;
  return self;
}
- (NACocoaWindow*) cocoawindow{
  return cocoawindow;
}
- (NSTrackingArea*) trackingarea{
  return trackingarea;
}
- (NAUInt) trackingcount{
  return trackingcount;
}
- (void)renewMouseTracking{
  trackingarea = [[NSTrackingArea alloc] initWithRect:[[self contentView] bounds]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
      owner:self userInfo:nil];
  [[self contentView] addTrackingArea:trackingarea];
}
- (void)clearMouseTracking{
  [[self contentView] removeTrackingArea:trackingarea];
  #if __has_feature(objc_arc)
    // TrackingArea will be released automatically when ARC is turned on.
  #else
    [trackingarea release];
  #endif
  trackingarea = nil;
}
- (void)retainMouseTracking{
  trackingcount++;
  if(trackingcount == 1){
    [self setAcceptsMouseMovedEvents:YES];
    naRenewWindowMouseTracking(cocoawindow);
  }
}
- (void)releaseMouseTracking{
  trackingcount--;
  if(trackingcount == 0){
    [self setAcceptsMouseMovedEvents:NO];
    naClearWindowMouseTracking(cocoawindow);
  }
}
- (void)mouseMoved:(NSEvent*)event{
//  double deltaX = [event deltaX];
//  double deltaY = [event deltaY];
  naSetMouseMovedByDiff([event deltaX], [event deltaY]);
  naDispatchUIElementCommand((NACoreUIElement*)cocoawindow, NA_UI_COMMAND_MOUSE_MOVED, NA_NULL);
//  [NSEvent setMouseCoalescingEnabled:NO];
}
- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  naSetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  naDispatchUIElementCommand((NACoreUIElement*)cocoawindow, NA_UI_COMMAND_MOUSE_ENTERED, NA_NULL);
}
- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  naSetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  naDispatchUIElementCommand((NACoreUIElement*)cocoawindow, NA_UI_COMMAND_MOUSE_EXITED, NA_NULL);
}
@end




NA_DEF NAWindow* naNewWindow(const char* title, NARect rect, NABool resizeable){
  NA_UNUSED(title);
  NA_UNUSED(resizeable);
  NACocoaWindow* cocoawindow = naAlloc(NACocoaWindow);
  cocoawindow->corewindow.windowedframe = rect;
  cocoawindow->corewindow.fullscreen = NA_FALSE;

  NSRect contentRect = naMakeNSRectWithRect(cocoawindow->corewindow.windowedframe);
  NANativeWindow* nativewindow = [[NANativeWindow alloc] initWithCocoaWindow:cocoawindow contentRect:contentRect styleMask:NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable | NAWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:NO screen:[NSScreen mainScreen]];
  
  [nativewindow setDelegate:nativewindow];
  [nativewindow setInitialFirstResponder:[nativewindow contentView]];
  #if __has_feature(objc_arc)
    naRegisterCoreUIElement(&(cocoawindow->corewindow.uielement), NA_UI_WINDOW, (void*)CFBridgingRetain(nativewindow));
  #else
    naRegisterCoreUIElement(&(cocoawindow->corewindow.uielement), NA_UI_WINDOW, (void*)nativewindow);
  #endif

  NASpace* space = naNewSpace(naMakeRectWithNSRect([[nativewindow contentView] frame]));
  naSetWindowContentSpace(cocoawindow, space);

  return (NAWindow*)cocoawindow;
}



NA_HDEF void naRenewWindowMouseTracking(NACocoaWindow* cocoawindow){
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  #endif
  [nativewindow renewMouseTracking];
}



NA_HDEF void naClearWindowMouseTracking(NACocoaWindow* cocoawindow){
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  #endif
  [nativewindow clearMouseTracking];
}



NA_HDEF void naRetainWindowMouseTracking(NAWindow* window){
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)window));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  #endif
  [nativewindow retainMouseTracking];
}



NA_HDEF void naReleaseWindowMouseTracking(NACocoaWindow* cocoawindow){
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  #endif
  [nativewindow releaseMouseTracking];
}



NA_DEF void naClearWindow(NAWindow* window){
  NA_UNUSED(window);
}



NA_DEF void naShowWindow(NAWindow* window){
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)window));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  #endif
  [nativewindow makeKeyAndOrderFront:NA_NULL];
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, NAUIElement* uielement){
  NACocoaWindow* cocoawindow = (NACocoaWindow*)window;
  #if __has_feature(objc_arc)
    NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)window));
  #else
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  #endif
  if([nativewindow trackingarea]){naClearWindowMouseTracking(cocoawindow);}
  NACoreUIElement* element = (NACoreUIElement*)uielement;
  #if __has_feature(objc_arc)
    [nativewindow setContentView:(NSView*)CFBridgingRelease(element->nativeID)];
    [(NSWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)window)) setInitialFirstResponder:[nativewindow contentView]];
  #else
    [nativewindow setContentView:(NSView*)element->nativeID];
    [(NSWindow*)naGetUIElementNativeID((NAUIElement*)window) setInitialFirstResponder:[nativewindow contentView]];
  #endif
  
  naAddListLastMutable(&(cocoawindow->corewindow.uielement.childs), uielement);
  naSetUIElementParent(uielement, window);
  
  if([nativewindow trackingcount]){naRenewWindowMouseTracking(cocoawindow);}
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  if(fullscreen != corewindow->fullscreen){
    #if __has_feature(objc_arc)
      NANativeWindow* nativewindow = (NANativeWindow*)CFBridgingRelease(naGetUIElementNativeID((NAUIElement*)window));
    #else
      NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
    #endif
    if(fullscreen){
      #if __has_feature(objc_arc)
        corewindow->windowedframe = naMakeRectWithNSRect([(NSWindow*)CFBridgingRelease(corewindow->uielement.nativeID) frame]);
      #else
        corewindow->windowedframe = naMakeRectWithNSRect([(NSWindow*)(corewindow->uielement.nativeID) frame]);
      #endif
      [nativewindow setStyleMask:NAWindowStyleMaskBorderless];
      [nativewindow setFrame:[[NSScreen mainScreen] frame] display:YES];
      [nativewindow setLevel:kCGScreenSaverWindowLevel];
    }else{
      [nativewindow setStyleMask:NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable | NAWindowStyleMaskResizable];
      [nativewindow setFrame:naMakeNSRectWithRect(corewindow->windowedframe) display:YES];
      [nativewindow setLevel:NSNormalWindowLevel];
    }
    corewindow->fullscreen = fullscreen;
    // Setting the first responder again is necessary as otherwise the first
    // responder is lost.
    [nativewindow makeFirstResponder:[nativewindow contentView]];
  }
}



NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  return corewindow->fullscreen;
}


NA_DEF NASpace* naGetWindowContentSpace(NAWindow* window){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  return naGetListFirstMutable(&(corewindow->uielement.childs));
}





// ////////////////////////////
// Space

@implementation NANativeSpace
- (id) initWithCocoaSpace:(NACocoaSpace*)newcocoaspace frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  cocoaspace = newcocoaspace;
  return self;
}
- (void)drawRect:(NSRect)dirtyRect{
  NA_UNUSED(dirtyRect);
  dirtyRect.origin.x += 100;
  dirtyRect.origin.y += 50;
  dirtyRect.size.width /= 2.;
  dirtyRect.size.height /= 2.;
  [[NSColor blueColor] setFill];
    NSRectFill(dirtyRect);
    [super drawRect:dirtyRect];
}
@end



NA_DEF NASpace* naNewSpace(NARect rect){
  NACocoaSpace* cocoaspace = naAlloc(NACocoaSpace);

  NSRect contentRect = naMakeNSRectWithRect(rect);
  NANativeSpace* nativeSpace = [[NANativeSpace alloc] initWithCocoaSpace:cocoaspace frame:contentRect];
  #if __has_feature(objc_arc)
    // Space will be released automatically when ARC is turned on.
  #else
    [nativeSpace autorelease];
  #endif
  
  #if __has_feature(objc_arc)
    naRegisterCoreUIElement(&(cocoaspace->corespace.uielement), NA_UI_VIEW, (void*)CFBridgingRetain(nativeSpace));
  #else
    naRegisterCoreUIElement(&(cocoaspace->corespace.uielement), NA_UI_VIEW, (void*)nativeSpace);
  #endif
  return (NASpace*)cocoaspace;
}



void naAddSpaceChild(NASpace* space, NAUIElement* child){
  NANativeSpace* nativespace = CFBridgingRelease(naGetUIElementNativeID(space));
  NANativeID nativechild = naGetUIElementNativeID(child);
  [nativespace addSubview:CFBridgingRelease(nativechild)];
}



// ////////////////////////////
// OpenGL Space

#if (NA_CONFIG_COMPILE_OPENGL == 1)
  @implementation NANativeOpenGLSpace
  - (id)initWithCocoaOpenGLSpace:(NAOpenGLSpace*)newcocoaopenglspace frame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)pixelformat initFunc:(NAMutator)newinitFunc initData:(void*)newinitData{
    self = [super initWithFrame:frameRect pixelFormat:pixelformat];
    cocoaopenglspace = newcocoaopenglspace;
    initFunc = newinitFunc;
    initData = newinitData;
    return self;
  }
  - (BOOL)acceptsFirstResponder{
    return YES; // This is required to get keyboard input.
  }
  - (void)prepareOpenGL{
    // When entering this function, the opengl context is set.
    [super prepareOpenGL];
    // Make sure OpenGL always swaps the buffers of the default framebuffer. If
    // this is not done, sometimes, the double buffer will not work properly.
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

    // Now the OpenGL context is created and current. We can initialize it
    // if necessary.
    if(initFunc){
      initFunc(initData);
    }
  }
  - (void)drawRect:(NSRect)dirtyRect{
    [[self openGLContext] makeCurrentContext];
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, NA_UI_COMMAND_REDRAW, &dirtyRect);
  }
  - (void)reshape{
    [super reshape];
    [[self openGLContext] update];
    NARect bounds = naMakeRectWithNSRect([self bounds]);
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, NA_UI_COMMAND_RESHAPE, &bounds);
  }
  - (void)keyDown:(NSEvent*)event{
    NAUIKeyCode keyCode = [event keyCode];
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, NA_UI_COMMAND_KEYDOWN, &keyCode);
  }
  - (void)keyUp:(NSEvent*)event{
    NAUIKeyCode keyCode = [event keyCode];
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, NA_UI_COMMAND_KEYUP, &keyCode);
  }
  - (void)flagsChanged:(NSEvent*)event{
    NAUIKeyCode keyCode;
    NABool shift   = ([event modifierFlags] & NAEventModifierFlagShift)    ?NA_TRUE:NA_FALSE;
    NABool alt     = ([event modifierFlags] & NAEventModifierFlagOption)   ?NA_TRUE:NA_FALSE;
    NABool control = ([event modifierFlags] & NAEventModifierFlagControl)  ?NA_TRUE:NA_FALSE;
    NABool command = ([event modifierFlags] & NAEventModifierFlagCommand)  ?NA_TRUE:NA_FALSE;

    keyCode = NA_KEYCODE_SHIFT;
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, (shift?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
    keyCode = NA_KEYCODE_OPTION;
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, (alt?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
    keyCode = NA_KEYCODE_CONTROL;
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, (control?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
    keyCode = NA_KEYCODE_LEFT_COMMAND;
    naDispatchUIElementCommand((NACoreUIElement*)cocoaopenglspace, (command?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
  }
  @end



  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
    NACoreOpenGLSpace* coreopenglspace = naAlloc(NACoreOpenGLSpace);

    // Configure the OpenGL Context and initialize this object.
    NSOpenGLPixelFormatAttribute attr[] = {
      NSOpenGLPFADoubleBuffer,
      NSOpenGLPFAColorSize, 24,
  //		NSOpenGLPFAAlphaSize, 8,
      NSOpenGLPFADepthSize, 64,
      NSOpenGLPFAAllowOfflineRenderers, // lets OpenGL know this context is offline renderer aware
      0 };
    NSOpenGLPixelFormat *pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
    #if __has_feature(objc_arc)
      // Space will be released automatically when ARC is turned on.
    #else
      [pixelformat autorelease];
    #endif
    NSRect frameRect = NSMakeRect(0.f, 0.f, (CGFloat)size.width, (CGFloat)size.height);
    NANativeOpenGLSpace* nativeSpace = [[NANativeOpenGLSpace alloc] initWithCocoaOpenGLSpace:coreopenglspace frame:frameRect pixelFormat:pixelformat initFunc:initfunc initData:initdata];
    #if __has_feature(objc_arc)
      // Space will be released automatically when ARC is turned on.
    #else
      [nativeSpace autorelease];
    #endif

  //  if([self respondsToSelector:@selector(setWantsBestResolutionOpenGLSurface:)]){
  //    #if defined __MAC_10_7
  //      [self setWantsBestResolutionOpenGLSurface:YES];
  //    #endif
  //  }

    #if __has_feature(objc_arc)
      naRegisterCoreUIElement(&(coreopenglspace->uielement), (NACoreUIElement*)window, NA_UI_OPENGLVIEW, (void*) CFBridgingRetain(nativeSpace));
    #else
      naRegisterCoreUIElement(&(coreopenglspace->uielement), (NACoreUIElement*)window, NA_UI_OPENGLVIEW, (void*) nativeSpace);
    #endif
    return coreopenglspace;
  }


  NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openglspace){
    NACoreOpenGLSpace* coreopenglspace = (NACoreOpenGLSpace*)openglspace;
    #if __has_feature(objc_arc)
      [[(NANativeOpenGLSpace*)CFBridgingRelease(coreopenglspace->uielement.nativeID) openGLContext] flushBuffer];
    #else
      [[(NANativeOpenGLSpace*)(coreopenglspace->uielement.nativeID) openGLContext] flushBuffer];
    #endif
  }

  NA_DEF void naSetOpenGLInnerRect(NAOpenGLSpace* openglspace, NARect bounds){
    NA_UNUSED(openglspace);
    NA_UNUSED(bounds);
  }

#endif  // NA_CONFIG_COMPILE_OPENGL




// ////////////////////////////
// Button

@implementation NANativeButton
- (id) initWithCocoaButton:(NACocoaButton*)newcocoabutton frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  [self setTitle:@"TheBöttn"];
  [self setButtonType:NSButtonTypeMomentaryPushIn];
  [self setBezelStyle:NSBezelStyleShadowlessSquare];
  [self setBordered:YES];
  cocoabutton = newcocoabutton;
  return self;
}
//- (void)drawRect:(NSRect)dirtyRect{
//  NA_UNUSED(dirtyRect);
//  dirtyRect.origin.x += 100;
//  dirtyRect.origin.y += 50;
//  dirtyRect.size.width /= 2.;
//  dirtyRect.size.height /= 2.;
//  [[NSColor blueColor] setFill];
//    NSRectFill(dirtyRect);
//    [super drawRect:dirtyRect];
//}
@end



NA_DEF NAButton* naNewButton(void){
  NACocoaButton* cocoabutton = naAlloc(NACocoaButton);

  NSRect contentRect = NSMakeRect(40, 70, 100, 50);
  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCocoaButton:cocoabutton frame:contentRect];
  #if __has_feature(objc_arc)
    // Button will be released automatically when ARC is turned on.
  #else
    [nativeButton autorelease];
  #endif
  
  #if __has_feature(objc_arc)
    naRegisterCoreUIElement(&(cocoabutton->corebutton.uielement), NA_UI_BUTTON, (void*)CFBridgingRetain(nativeButton));
  #else
    naRegisterCoreUIElement(&(cocoabutton->corebutton.uielement), NA_UI_BUTTON, (void*)nativeButton);
  #endif
  return (NAButton*)cocoabutton;
}


NA_HDEF NARect naGetButtonAbsoluteInnerRect(NACoreUIElement* space){
  return naMakeRectS(20, 40, 100, 50);
}




// ///////////////////


NA_DEF void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage){
  NA_UNUSED(sendmovemessage);
  NARect spacerect;
  NSRect screenframe;
  CGPoint centerpos;
  spacerect = naGetUIElementRect(uielement, (NAUIElement*)naGetApplication(), includebounds);
  screenframe = [[NSScreen mainScreen] frame];
//  centerpos.x = spacerect.pos.x + spacerect.size.width * .5f;
//  centerpos.y = spacerect.pos.y + spacerect.size.height * .5f;
  centerpos.x = (CGFloat)spacerect.pos.x + (CGFloat)spacerect.size.width * .5f;
  centerpos.y = (CGFloat)screenframe.size.height - (CGFloat)(spacerect.pos.y + spacerect.size.height * .5f);

  CGWarpMouseCursorPosition(centerpos);
//  CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, centerpos);
}



NA_DEF void naShowMouse(){
  NACoreApplication* coreapp = (NACoreApplication*)naGetApplication();
  if(!(coreapp->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE)){
    CGDisplayShowCursor(kCGDirectMainDisplay);
    coreapp->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}


NA_DEF void naHideMouse(){
  NACoreApplication* coreapp = (NACoreApplication*)naGetApplication();
  if(coreapp->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE){
    CGDisplayHideCursor(kCGDirectMainDisplay);
    coreapp->flags &= ~NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}




#endif // NA_OS == NA_OS_MAC_OS_X
#endif // __OBJC__
#endif // (NA_CONFIG_COMPILE_GUI == 1)

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
