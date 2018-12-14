
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








// ///////////////////////////////////
// APPLICATION
// ///////////////////////////////////


NA_DEF void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  // The ((id (*)(id, SEL)) part is a cast of the objc_msgSend function which
  // is requires since a later version of Objective-C
//  ( (id (*)(id, SEL)) objc_msgSend)(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));

  // Start the Cocoa application and set the native ID of the application.
  [NSApplication sharedApplication];
  naStartCoreApplication(sizeof(NACocoaApplication), NSApp);

  // Put an autorelease pool in place for the startup sequence.
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    // Call prestartup if desired.
    if(prestartup){prestartup(arg);}
    // Let the Macintosh System know that the app is ready to run.
    [NSApp finishLaunching];
    // Call poststartup if desired.
    if(poststartup){poststartup(arg);}
  [pool drain];

  // Start the event loop.
  NSDate* distantfuture = [NSDate distantFuture];
  while(naIsCoreApplicationRunning()){
    pool = [[NSAutoreleasePool alloc] init];
      NSEvent* curevent = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:distantfuture inMode:NSDefaultRunLoopMode dequeue:YES];
      naCollectGarbage();
      if(curevent){[NSApp sendEvent:curevent];}
    [pool drain];
  }

  // When reaching here, the application had been stopped.
  naClearCoreApplication();
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  dispatch_time_t nexttime = dispatch_time(DISPATCH_TIME_NOW, 1000000000 * timediff);
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
  [((NSView*)naGetUIElementNativeID(uielement)) setNeedsDisplay:YES];
}






















NA_HDEF NARect naGetApplicationAbsoluteRect(){
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
  frame = [(NSScreen*)(naGetUIElementNativeID((NAUIElement*)screen)) frame];
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
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
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
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  windowframe = [nativewindow frame];
  rect.pos.x = windowframe.origin.x;
  rect.pos.y = windowframe.origin.y;
  rect.size.width = windowframe.size.width;
  rect.size.height = windowframe.size.height;
  return rect;
}



NA_HDEF NARect naGetViewAbsoluteInnerRect(NACoreUIElement* view){
  NARect rect;
  NSRect contentrect;
  NARect windowrect;
  // Warning: does not work when frame unequal bounds.
  contentrect = [(NSView*)(naGetUIElementNativeID((NAUIElement*)view)) frame];
  windowrect = naGetWindowAbsoluteInnerRect((NACoreUIElement*)naGetUIElementWindow((NAUIElement*)view));
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
        naError("naGetUIElementRect", "The relative element is invalid for the given uielement, which seems to be the application.");
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









@implementation NANativeView
- (void)drawRect:(NSRect)dirtyRect{
  NA_UNUSED(dirtyRect);
}
@end





@implementation NANativeWindow
- (id) initWithCocoaWindow:(NACocoaWindow*)nawindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  cocoawindow = nawindow;
  return self;
}
- (NACocoaWindow*) cocoawindow{
  return cocoawindow;
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
  cocoawindow->trackingcount = 0;
  cocoawindow->trackingarea = nil;

  NSRect contentRect = naMakeNSRectWithRect(cocoawindow->corewindow.windowedframe);
  void* nativeID = [[NANativeWindow alloc] initWithCocoaWindow:cocoawindow contentRect:contentRect styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask backing:NSBackingStoreBuffered defer:NO screen:[NSScreen mainScreen]];
  NANativeView* nativeView = [[[NANativeView alloc] initWithFrame:[[(NANativeWindow*)nativeID contentView] frame]] autorelease];
  [(NANativeWindow*)nativeID setDelegate:(NANativeWindow*)nativeID];
  [(NANativeWindow*)nativeID setContentView:nativeView];
  [(NANativeWindow*)nativeID setInitialFirstResponder:[(NANativeWindow*)nativeID contentView]];
  naRegisterCoreUIElement(&(cocoawindow->corewindow.uielement), (NACoreUIElement*)naGetApplication(), NA_UI_WINDOW, nativeID);
  return (NAWindow*)cocoawindow;
}



NA_HDEF void naRenewWindowMouseTracking(NACocoaWindow* cocoawindow){
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  cocoawindow->trackingarea = [[NSTrackingArea alloc] initWithRect:[[nativewindow contentView] bounds]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
      owner:nativewindow userInfo:nil];
  [[nativewindow contentView] addTrackingArea:cocoawindow->trackingarea];
}



NA_HDEF void naClearWindowMouseTracking(NACocoaWindow* cocoawindow){
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  [[nativewindow contentView] removeTrackingArea:cocoawindow->trackingarea];
  [cocoawindow->trackingarea release];
  cocoawindow->trackingarea = nil;
}



NA_HDEF void naRetainWindowMouseTracking(NAWindow* window){
  NACocoaWindow* cocoawindow = (NACocoaWindow*)window;
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  cocoawindow->trackingcount++;
  if(cocoawindow->trackingcount == 1){
    [nativewindow setAcceptsMouseMovedEvents:YES];
    naRenewWindowMouseTracking(cocoawindow);
  }
}



NA_HDEF void naReleaseWindowMouseTracking(NACocoaWindow* cocoawindow){
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  cocoawindow->trackingcount--;
  if(cocoawindow->trackingcount == 0){
    [nativewindow setAcceptsMouseMovedEvents:NO];
    naClearWindowMouseTracking(cocoawindow);
  }
}



NA_DEF void naClearWindow(NAWindow* window){
  NA_UNUSED(window);
}



NA_DEF void naShowWindow(NAWindow* window){
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  [nativewindow makeKeyAndOrderFront:NA_NULL];
}



NA_DEF void naSetWindowContentView(NAWindow* window, NAUIElement* uielement){
  NACocoaWindow* cocoawindow = (NACocoaWindow*)window;
  if(cocoawindow->trackingarea){naClearWindowMouseTracking(cocoawindow);}
  NACoreUIElement* element = (NACoreUIElement*)uielement;
  NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  [nativewindow setContentView:element->nativeID];
  [(NSWindow*)naGetUIElementNativeID((NAUIElement*)window) setInitialFirstResponder:[nativewindow contentView]];
  if(cocoawindow->trackingcount){naRenewWindowMouseTracking(cocoawindow);}
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  if(fullscreen != corewindow->fullscreen){
    NANativeWindow* nativewindow = (NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
    if(fullscreen){
      corewindow->windowedframe = naMakeRectWithNSRect([(NSWindow*)(corewindow->uielement.nativeID) frame]);
      [nativewindow setStyleMask:NSBorderlessWindowMask];
      [nativewindow setFrame:[[NSScreen mainScreen] frame] display:YES];
      [nativewindow setLevel:kCGScreenSaverWindowLevel];
    }else{
      [nativewindow setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];
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






@implementation NANativeOpenGLView
- (id)initWithNALibView:(NAOpenGLView*)naopenglview frame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)pixelformat initFunc:(NAMutator)newinitFunc initData:(void*)newinitData{
  self = [super initWithFrame:frameRect pixelFormat:pixelformat];
  nalibopenglview = naopenglview;
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
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, NA_UI_COMMAND_REDRAW, &dirtyRect);
}
- (void)reshape{
  [[self openGLContext] update];
  NARect bounds = naMakeRectWithNSRect([self bounds]);
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, NA_UI_COMMAND_RESHAPE, &bounds);
}
- (void)keyDown:(NSEvent*)event{
  NAUIKeyCode keyCode = [event keyCode];
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, NA_UI_COMMAND_KEYDOWN, &keyCode);
}
- (void)keyUp:(NSEvent*)event{
  NAUIKeyCode keyCode = [event keyCode];
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, NA_UI_COMMAND_KEYUP, &keyCode);
}
- (void)flagsChanged:(NSEvent*)event{
  NAUIKeyCode keyCode;
  NABool shift   = ([event modifierFlags] & NSShiftKeyMask)    ?NA_TRUE:NA_FALSE;
  NABool alt     = ([event modifierFlags] & NSAlternateKeyMask)?NA_TRUE:NA_FALSE;
  NABool control = ([event modifierFlags] & NSControlKeyMask)  ?NA_TRUE:NA_FALSE;
  NABool command = ([event modifierFlags] & NSCommandKeyMask)  ?NA_TRUE:NA_FALSE;

  keyCode = NA_KEYCODE_SHIFT;
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, (shift?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
  keyCode = NA_KEYCODE_OPTION;
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, (alt?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
  keyCode = NA_KEYCODE_CONTROL;
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, (control?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
  keyCode = NA_KEYCODE_LEFT_COMMAND;
  naDispatchUIElementCommand((NACoreUIElement*)nalibopenglview, (command?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
}
@end




NA_DEF NAOpenGLView* naNewOpenGLView(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
  NACoreOpenGLView* coreopenglview = naAlloc(NACoreOpenGLView);

  // Configure the OpenGL Context and initialize this object.
  NSOpenGLPixelFormatAttribute attr[] = {
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 24,
//		NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFADepthSize, 64,
    NSOpenGLPFAAllowOfflineRenderers, // lets OpenGL know this context is offline renderer aware
		0 };
	NSOpenGLPixelFormat *pixelformat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];
  NSRect frameRect = NSMakeRect(0.f, 0.f, (CGFloat)size.width, (CGFloat)size.height);
  void* nativeID = [[[NANativeOpenGLView alloc] initWithNALibView:coreopenglview frame:frameRect pixelFormat:pixelformat initFunc:initfunc initData:initdata] autorelease];

//  if([self respondsToSelector:@selector(setWantsBestResolutionOpenGLSurface:)]){
//    #if defined __MAC_10_7
//      [self setWantsBestResolutionOpenGLSurface:YES];
//    #endif
//  }

  naRegisterCoreUIElement(&(coreopenglview->uielement), (NACoreUIElement*)window, NA_UI_OPENGLVIEW, nativeID);
  return coreopenglview;
}


NA_DEF void naSwapOpenGLBuffer(NAOpenGLView* openglview){
  NACoreOpenGLView* coreopenglview = (NACoreOpenGLView*)openglview;
  [[(NANativeOpenGLView*)(coreopenglview->uielement.nativeID) openGLContext] flushBuffer];
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
  viewrect = naGetUIElementRect(uielement, (NAUIElement*)naGetApplication(), includebounds);
  screenframe = [[NSScreen mainScreen] frame];
//  centerpos.x = viewrect.pos.x + viewrect.size.width * .5f;
//  centerpos.y = viewrect.pos.y + viewrect.size.height * .5f;
  centerpos.x = (CGFloat)viewrect.pos.x + (CGFloat)viewrect.size.width * .5f;
  centerpos.y = (CGFloat)screenframe.size.height - (CGFloat)(viewrect.pos.y + viewrect.size.height * .5f);

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
