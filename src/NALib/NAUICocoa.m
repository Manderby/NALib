
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file is a .m-File which is an implementation file in Objective-C.
// This is required on Macintosh systems to link against the Cocoa-Framework.
// If you do not have an Objective-C compiler such as in windows, this file
// will simply be ignored and hence nothing is linked. If this is not the case,
// you can simply delete the .m File from your source tree.

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#include <objc/message.h>

#include "NAUIHiddenAPI.h"
#if NA_SYSTEM == NA_SYSTEM_MAC_OS_X
// Now, we are sure, we compile with Objective-C and on MacOSX. The two
// #if directives will be closed at the very bottom of this file.

#include "NAMemory.h"


#include "NACoord.h"
#include "NAThreading.h"



struct NAApplication{
  NAUIElement uielement;
//  NSAutoreleasePool* autoreleasepool;
  NABool ismousevisible;
};

struct NAScreen{
  NAUIElement uielement;
};


struct NAWindow{
  NAUIElement uielement;
  NABool fullscreen;
  NARect windowedframe;
  NAUInt trackingcount;
  NSTrackingArea* trackingarea;
};


struct NAOpenGLView{
  NAUIElement uielement;
};









typedef struct NAUI NAUI;
extern NAUI* na_ui;



//@interface NAApplicationStartup : NSObject <NSApplicationDelegate>{
//  NAApplicationStartup* myself;
//  NAFunc startupfunction;
//  id olddelegate;
//}
//@end
//
//@implementation NAApplicationStartup
//- (void)applicationDidFinishLaunching:(NSNotification *)notification{
//
//  naInitBareUI();
//  NAApplication* app = naAlloc(NAApplication);
//  naInitUIElement(&(app->uielement), NA_NULL, NA_UI_APPLICATION, NSApp);
//
//  if(startupfunction){startupfunction();}
//  [NSApp setDelegate:olddelegate];
//  if(olddelegate){
//    [olddelegate applicationDidFinishLaunching:notification];
//  }
////  [self release];
//}
//- (void)setMyself:(NAApplicationStartup*)newmyself{myself = newmyself;}
//- (void)setStartupFunction:(NAFunc)newfunction{startupfunction = newfunction;}
//- (void)setOldDelegate:(id)newolddelegate{olddelegate = newolddelegate;}
//@end



NA_DEF void naCallFunctionInSeconds(NAFunc function, void* arg, double timediff){
  dispatch_time_t nexttime = dispatch_time(DISPATCH_TIME_NOW, 1000000000 * timediff);
  dispatch_queue_t queue = dispatch_get_current_queue();
  dispatch_after_f(nexttime, queue, arg, function);
}



NA_HDEF NABool naDispatchUIElementCommandCocoa(NSEvent* event){
//  NSEventType type = [event type];
  [NSApp sendEvent:event];
  return NA_TRUE;
}



NA_HDEF void naStartupNALibApplication(void){
  naInitBareUI();
  NAApplication* app = naAlloc(NAApplication);
  naInitUIElement(&(app->uielement), NA_NULL, NA_UI_APPLICATION, NSApp);
}


NA_DEF void naOpenConsoleWindow(){
  // Does nothing on the Mac
}


NA_DEF void naStartApplication(NAFunc prestartup, NAFunc poststartup, void* arg){
  // The ((id (*)(id, SEL)) part is a cast of the objc_msgSend function which
  // is requires since a later version of Objective-C
//  ( (id (*)(id, SEL)) objc_msgSend)(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));

  [NSApplication sharedApplication];

  naInitBareUI();
  NAApplication* app = naAlloc(NAApplication);
  naInitUIElement(&(app->uielement), NA_NULL, NA_UI_APPLICATION, NSApp);
  app->ismousevisible = NA_TRUE;

  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    if(prestartup){prestartup(arg);}
    [NSApp finishLaunching];
    if(poststartup){poststartup(arg);}
  [pool drain];

  NSDate* distantfuture = [NSDate distantFuture];
  while(1){
    pool = [[NSAutoreleasePool alloc] init];
      NSEvent* curevent = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:distantfuture inMode:NSDefaultRunLoopMode dequeue:YES];
      if(curevent){naDispatchUIElementCommandCocoa(curevent);}
    [pool drain];
  }
}




NA_DEF void cubFlushGarbageMemory(){
  #ifndef NDEBUG
    if(!NSApp)
      {naCrash("naInitUI", "You need to have an active NSApplication. See API for more information.");return;}
  #endif

}




















//NA_DEF NAUInt naRunUI(){
//  #ifndef NDEBUG
//    if(!na_ui)
//      {naCrash("naRunUI", "UI not initialized.");return 0;}
//    if(!NSApp)
//      {naCrash("naRunUI", "You need to have an active NSApplication. See API for more information.");return 0;}
//    if(CFRunLoopGetCurrent() != CFRunLoopGetMain())
//      naError("naRunUI", "The UI should run in the main loop");
//  #endif
//  
////  CFRunLoopRef testref = CFRunLoopGetCurrent();
////  CFRunLoopRef testref2 = CFRunLoopGetMain();
//  
////  NAApplication* app = naGetApplication();
//  
////  naGetApplication()->autoreleasepool = [[NSAutoreleasePool alloc] init];
////  [NSApp run];
////  NSAutoreleasePool* pool;
//
//  NSDate* distantfuture = [NSDate distantFuture];
//  while(1){
//    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
//    NSEvent* curevent = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:distantfuture inMode:NSDefaultRunLoopMode dequeue:YES];
//    if(curevent){
//      naDispatchUIElementCommandCocoa(curevent);
//    }
////    [curevent release];
////  cubFlushGarbageMemory();
//    [pool drain];
////  [NSAutoreleasePool showPools];
////    cubFlushGarbageMemory();
//  }
//  return 0;
//
//}













NA_HDEF void naRefreshUIElementNow(void* uielement){
  [((NSView*)naGetUINativeID(uielement)) setNeedsDisplay:YES];
}






NA_HDEF NARect naGetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetScreenAbsoluteRect(NAUIElement* screen){
  NARect rect;
  NSRect frame;
  NSRect mainframe;
  mainframe = [[NSScreen mainScreen] frame];
  frame = [(NSScreen*)(naGetUINativeID(screen)) frame];
  rect.pos.x = frame.origin.x;
  rect.pos.y = mainframe.size.height - frame.size.height - frame.origin.y;
  rect.size.width = frame.size.width;
  rect.size.height = frame.size.height;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteInnerRect(NAUIElement* window){
  NARect rect;
  NSRect contentrect;
  NSRect windowframe;
  contentrect = [[(NSWindow*)(naGetUINativeID(window)) contentView] frame];
  windowframe = [(NSWindow*)(naGetUINativeID(window)) frame];
  rect.pos.x = windowframe.origin.x + contentrect.origin.x;
  rect.pos.y = windowframe.origin.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteOuterRect(NAUIElement* window){
  NARect rect;
  NSRect windowframe;
  windowframe = [(NSWindow*)(naGetUINativeID(window)) frame];
  rect.pos.x = windowframe.origin.x;
  rect.pos.y = windowframe.origin.y;
  rect.size.width = windowframe.size.width;
  rect.size.height = windowframe.size.height;
  return rect;
}



NA_HDEF NARect naGetViewAbsoluteInnerRect(NAUIElement* view){
  NARect rect;
  NSRect contentrect;
  NARect windowrect;
  // Warning: does not work when frame unequal bounds.
  contentrect = [(NSView*)(naGetUINativeID(view)) frame];
  windowrect = naGetWindowAbsoluteInnerRect((NAUIElement*)naGetUIElementWindow(view));
  rect.pos.x = windowrect.pos.x + contentrect.origin.x;
  rect.pos.y = windowrect.pos.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_DEF NARect naGetUIElementRect(void* uielement, void* relativeelement, NABool includebounds){
  NARect rect;
  NARect relrect;
  NAUIElement* element;
  NAUIElement* relelement;
  
  element = (NAUIElement*)uielement;
  relelement = (NAUIElement*)relativeelement;
  NAApplication* app = naGetApplication();
  
  // First, let's handle the root case: Returning the application rect.
  if(element == (NAUIElement*)app){
    #ifndef NDEBUG
      if(relativeelement && (relativeelement != app))
        naError("naGetUIElementRect", "The relative element is invalid for the given uielement, which seems to be the application.");
    #endif
    return naGetApplicationAbsoluteRect();
  }
  
  // Now, we find the appropriate relative element.
  if(!relelement){relelement = naGetUIElementParent(element);}
  
  switch(element->elementtype){
  case NA_UI_APPLICATION: rect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = naGetScreenAbsoluteRect(element); break;
  case NA_UI_WINDOW:
    if(includebounds){
      rect = naGetWindowAbsoluteOuterRect(element);
    }else{
      rect = naGetWindowAbsoluteInnerRect(element);
    }
    break;
  case NA_UI_OPENGLVIEW:  rect = naGetViewAbsoluteInnerRect(element); break;
  }
  
  switch(relelement->elementtype){
  case NA_UI_APPLICATION: relrect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      relrect = naGetScreenAbsoluteRect(relelement); break;
  case NA_UI_WINDOW:      relrect = naGetWindowAbsoluteInnerRect(relelement); break;
  case NA_UI_OPENGLVIEW:  relrect = naGetViewAbsoluteInnerRect(relelement); break;
  }
  
  rect.pos.x = rect.pos.x - relrect.pos.x;
  rect.pos.y = rect.pos.y - relrect.pos.y;
  rect.size.width = rect.size.width;
  rect.size.height = rect.size.height;
  
  // Convert the rect into absolute coordinates.
  
  return rect;
}








@interface NANativeView : NSView
@end

@implementation NANativeView
- (void)drawRect:(NSRect)dirtyRect{
  NA_UNUSED(dirtyRect);
}
@end




@interface NANativeWindow : NSWindow <NSWindowDelegate>{
  NAWindow* nalibwindow;
}
@end

@implementation NANativeWindow
- (id) initWithNALibWindow:(NAWindow*)nawindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  nalibwindow = nawindow;
  return self;
}
- (NAWindow*) nalibwindow{return nalibwindow;}
- (void)mouseMoved:(NSEvent *)theEvent{
//  double deltaX = [theEvent deltaX];
//  double deltaY = [theEvent deltaY];
  naSetMouseMovedByDiff([theEvent deltaX], [theEvent deltaY]);
  naDispatchUIElementCommand(nalibwindow, NA_UI_COMMAND_MOUSE_MOVED, NA_NULL);
//  [NSEvent setMouseCoalescingEnabled:NO];
}
- (void)mouseEntered:(NSEvent *)theEvent{
  NA_UNUSED(theEvent);
  naSetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  naDispatchUIElementCommand(nalibwindow, NA_UI_COMMAND_MOUSE_ENTERED, NA_NULL);
}
- (void)mouseExited:(NSEvent *)theEvent{
  NA_UNUSED(theEvent);
  naSetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  naDispatchUIElementCommand(nalibwindow, NA_UI_COMMAND_MOUSE_EXITED, NA_NULL);
}
@end




NA_DEF NAWindow* naNewWindow(const char* title, double posx, double posy, double width, double height, NABool resizeable){
  NA_UNUSED(title);
  NA_UNUSED(resizeable);
  NAWindow* window = naAlloc(NAWindow);
  window->windowedframe = naMakeRect(naMakePos(posx, posy), naMakeSize(width, height));
  window->fullscreen = NA_FALSE;
  window->trackingcount = 0;
  window->trackingarea = nil;
  
  NSRect contentRect = naMakeNSRectWithRect(window->windowedframe);
  void* nativeID = [[NANativeWindow alloc] initWithNALibWindow:window contentRect:contentRect styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask backing:NSBackingStoreBuffered defer:NO screen:[NSScreen mainScreen]];
  NANativeView* nativeView = [[[NANativeView alloc] initWithFrame:[[(NANativeWindow*)nativeID contentView] frame]] autorelease];
  [(NANativeWindow*)nativeID setDelegate:(NANativeWindow*)nativeID];
  [(NANativeWindow*)nativeID setContentView:nativeView];
  [(NANativeWindow*)nativeID setInitialFirstResponder:[(NANativeWindow*)nativeID contentView]];
  naInitUIElement(&(window->uielement), (NAUIElement*)naGetApplication(), NA_UI_WINDOW, nativeID);
  return window;
}



NA_HDEF void naRenewWindowMouseTracking(NAWindow* window){
  window->trackingarea = [[NSTrackingArea alloc] initWithRect:[[(NSWindow*)naGetUINativeID(window) contentView] bounds]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
      owner:(NSWindow*)naGetUINativeID(window) userInfo:nil];
  [[(NSWindow*)naGetUINativeID(window) contentView] addTrackingArea:window->trackingarea];
}



NA_HDEF void naClearWindowMouseTracking(NAWindow* window){
  [[(NSWindow*)naGetUINativeID(window) contentView] removeTrackingArea:window->trackingarea];
  [window->trackingarea release];
  window->trackingarea = nil;
}



NA_HDEF void naRetainWindowMouseTracking(NAWindow* window){
  window->trackingcount++;
  if(window->trackingcount == 1){
    [(NSWindow*)naGetUINativeID(window) setAcceptsMouseMovedEvents:YES];
    naRenewWindowMouseTracking(window);
  }
}



NA_HDEF void naReleaseWindowMouseTracking(NAWindow* window){
  window->trackingcount--;
  if(window->trackingcount == 0){
    [(NSWindow*)naGetUINativeID(window) setAcceptsMouseMovedEvents:NO];
    naClearWindowMouseTracking(window);
  }
}



NA_DEF void naClearWindow(NAWindow* window){
  NA_UNUSED(window);
}



NA_DEF void naShowWindow(NAWindow* window){
  [(NANativeWindow*)(naGetUINativeID(&(window->uielement))) makeKeyAndOrderFront:NA_NULL];
}



NA_DEF void naSetWindowContentView(NAWindow* window, void* uielement){
  if(window->trackingarea){naClearWindowMouseTracking(window);}
  NAUIElement* element = (NAUIElement*)uielement;
  [(NANativeWindow*)(naGetUINativeID(&(window->uielement))) setContentView:element->nativeID];
  [(NSWindow*)naGetUINativeID(window) setInitialFirstResponder:[(NSWindow*)naGetUINativeID(window) contentView]];
  if(window->trackingcount){naRenewWindowMouseTracking(window);}
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  if(fullscreen == window->fullscreen){return;}
  if(fullscreen){
    window->windowedframe = naMakeRectWithNSRect([(NSWindow*)(window->uielement.nativeID) frame]);
    [(NSWindow*)(naGetUINativeID(&(window->uielement))) setStyleMask:NSBorderlessWindowMask];
    [(NSWindow*)(naGetUINativeID(&(window->uielement))) setFrame:[[NSScreen mainScreen] frame] display:YES];
    [(NSWindow*)(naGetUINativeID(&(window->uielement))) setLevel:kCGScreenSaverWindowLevel];
  }else{
    [(NSWindow*)(naGetUINativeID(&(window->uielement))) setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];
    [(NSWindow*)(naGetUINativeID(&(window->uielement))) setFrame:naMakeNSRectWithRect(window->windowedframe) display:YES];
    [(NSWindow*)(naGetUINativeID(&(window->uielement))) setLevel:NSNormalWindowLevel];
  }
  window->fullscreen = fullscreen;
  // Setting the first responder again is necessary as otherwise the first
  // responder is lost.
  [(NSWindow*)(naGetUINativeID(&(window->uielement))) makeFirstResponder:[(NSWindow*)(naGetUINativeID(&(window->uielement))) contentView]];
}



NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  return window->fullscreen;
}





@interface NANativeOpenGLView : NSOpenGLView{
  NAOpenGLView* nalibopenglview;
}
@end

@implementation NANativeOpenGLView
- (id)initWithNALibView:(NAOpenGLView*)naopenglview frame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)pixelformat{
  self = [super initWithFrame:frameRect pixelFormat:pixelformat];
  nalibopenglview = naopenglview;
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
}
- (void)drawRect:(NSRect)dirtyRect{
//    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  [[self openGLContext] makeCurrentContext];
  naDispatchUIElementCommand(nalibopenglview, NA_UI_COMMAND_REDRAW, &dirtyRect);
//    [pool release];
}
- (void)reshape{
  [[self openGLContext] update];
  NARect bounds = naMakeRectWithNSRect([self bounds]);
  naDispatchUIElementCommand(nalibopenglview, NA_UI_COMMAND_RESHAPE, &bounds);
}
- (void)keyDown:(NSEvent *)theEvent{
  NAUIKeyCode keyCode = [theEvent keyCode];
  naDispatchUIElementCommand(nalibopenglview, NA_UI_COMMAND_KEYDOWN, &keyCode);
}
- (void)keyUp:(NSEvent *)theEvent{
  NAUIKeyCode keyCode = [theEvent keyCode];
  naDispatchUIElementCommand(nalibopenglview, NA_UI_COMMAND_KEYUP, &keyCode);
}
- (void)flagsChanged:(NSEvent *)theEvent{
  NAUIKeyCode keyCode;
  NABool shift   = ([theEvent modifierFlags] & NSShiftKeyMask)    ?NA_TRUE:NA_FALSE;
  NABool alt     = ([theEvent modifierFlags] & NSAlternateKeyMask)?NA_TRUE:NA_FALSE;
  NABool control = ([theEvent modifierFlags] & NSControlKeyMask)  ?NA_TRUE:NA_FALSE;
  NABool command = ([theEvent modifierFlags] & NSCommandKeyMask)  ?NA_TRUE:NA_FALSE;
  
  keyCode = NA_KEYCODE_SHIFT;
  naDispatchUIElementCommand(nalibopenglview, (shift?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
  keyCode = NA_KEYCODE_OPTION;
  naDispatchUIElementCommand(nalibopenglview, (alt?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
  keyCode = NA_KEYCODE_CONTROL;
  naDispatchUIElementCommand(nalibopenglview, (control?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
  keyCode = NA_KEYCODE_LEFT_COMMAND;
  naDispatchUIElementCommand(nalibopenglview, (command?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
}
@end




NA_DEF NAOpenGLView* naNewOpenGLView(NAWindow* window, double width, double height){
  NAOpenGLView* openglview = naAlloc(NAOpenGLView);

  // Configure the OpenGL Context and initialize this object.
  NSOpenGLPixelFormatAttribute attr[] = {
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 24,
//		NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFADepthSize, 64,
    NSOpenGLPFAAllowOfflineRenderers, // lets OpenGL know this context is offline renderer aware
		0 };
	NSOpenGLPixelFormat *pixelformat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];
  NSRect frameRect = NSMakeRect(0.f, 0.f, (CGFloat)width, (CGFloat)height);
  void* nativeID = [[[NANativeOpenGLView alloc] initWithNALibView:openglview frame:frameRect pixelFormat:pixelformat] autorelease];
  
//  if([self respondsToSelector:@selector(setWantsBestResolutionOpenGLSurface:)]){
//    #if defined __MAC_10_7
//      [self setWantsBestResolutionOpenGLSurface:YES];
//    #endif
//  }

  naInitUIElement(&(openglview->uielement), (NAUIElement*)window, NA_UI_OPENGLVIEW, nativeID);
  return openglview;
}


NA_DEF void naSwapOpenGLBuffer(NAOpenGLView* openglview){
  [[(NANativeOpenGLView*)(openglview->uielement.nativeID) openGLContext] flushBuffer];
}

NA_DEF void naSetOpenGLInnerRect(NAOpenGLView* openglview, NARect bounds){
  NA_UNUSED(openglview);
  NA_UNUSED(bounds);
}






NA_DEF void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage){
  NA_UNUSED(sendmovemessage);
  NARect viewrect;
  NSRect screenframe;
  CGPoint centerpos;
  viewrect = naGetUIElementRect(uielement, naGetApplication(), includebounds);
  screenframe = [[NSScreen mainScreen] frame];
//  centerpos.x = viewrect.pos.x + viewrect.size.width * .5f;
//  centerpos.y = viewrect.pos.y + viewrect.size.height * .5f;
  centerpos.x = (CGFloat)viewrect.pos.x + (CGFloat)viewrect.size.width * .5f;
  centerpos.y = (CGFloat)screenframe.size.height - (CGFloat)(viewrect.pos.y + viewrect.size.height * .5f);
  
  CGWarpMouseCursorPosition(centerpos);
//  CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, centerpos);
}



NA_DEF void naShowMouse(){
  NAApplication* app = naGetApplication();
  if(!app->ismousevisible){
    CGDisplayShowCursor(kCGDirectMainDisplay);
    app->ismousevisible = NA_TRUE;
  }
}


NA_DEF void naHideMouse(){
  NAApplication* app = naGetApplication();
  if(app->ismousevisible){
    CGDisplayHideCursor(kCGDirectMainDisplay);
    app->ismousevisible = NA_FALSE;
  }
}




#endif // NA_SYSTEM == NA_SYSTEM_MAC_OS_X
#endif // __OBJC__

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
