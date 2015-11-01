
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

#include "NAUI.h"
#if NA_SYSTEM == NA_SYSTEM_MAC_OS_X
// Now, we are sure, we compile with Objective-C and on MacOSX. The two
// #if directives will be closed at the very bottom of this file.

#include "NAMemory.h"


// The following are just comments to see what the original types should be:
// typedef unsigned short NAUIKeyCode;
// typedef NSResponder* NANativeUIID;


#include "NACoord.h"
#include "NAThreading.h"






struct NAWindow{
  NAUIElement uielement;
  NABool fullscreen;
  NARect windowedframe;
//  HDC hDC;    // Device context
//  HGLRC hRC;  // Rendering context
//  NAList elements;
//  Rect previouswinrect;
//  TCHAR fullscreendevicename[CCHDEVICENAME];
//  Rect fullscreenrect;
//  MBController* controller;
//  Rect winrect;
//  Rect viewrect;
//  MBBool erasebackground;
//  MBBool quitonclose;
//  void show();
//  void close();
//  void enableContext();
//  virtual void prepareOpenGL(); // loads all needed stuff for best OpenGL experience
//  virtual void startOpenGLDrawing(); // opens the context
//  virtual void endOpenGLDrawing();  // swaps the buffer
//  virtual void draw();  // default implementation does nothing
//  virtual void resize(); // default implementation stores the position and size
//  virtual void keyDown(int32 key);  // default  does nothing.
//  void setPos(int posx, int posy);
//
//  void setFullscreenRect(WCHAR szDevice[CCHDEVICENAME], Rect rect);
//
//  void addElement(MBUIElement* newelement);
//  void removeElement(MBUIElement* oldelement);
//  MBUIElement* getUIElement(HWND handle);
//  
//  // The background of a window is usually filled with the default windows
//  // background color. Set it to false, if you want to prevent that.
//  MBBool eraseBackground();
//  void setEraseBackground(MBBool erase);
//
//  // The background of a window is usually filled with the default windows
//  // background color. Set it to false, if you want to prevent that.
//  MBBool quitOnClose();
//  void setQuitOnClose(MBBool quit);
};

struct NAOpenGLView{
  NAUIElement uielement;
};















NA_DEF void naStartDefaultApplication(void){
  // The ((id (*)(id, SEL)) part is a cast of the objc_msgSend function which
  // is requires since a later version of Objective-C
  ( (id (*)(id, SEL)) objc_msgSend)(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));
}





NA_DEF void naInitUI(void){
  naInitBareUI();

  #ifndef NDEBUG
    if(!NSApp)
      {naCrash("naInitUI", "You need to have an active NSApplication. See API for more information.");return;}
  #endif

  NAApplication* app = naAlloc(NAApplication);
  naInitUIElement(&(app->uielement), NA_NULL, NA_UI_APPLICATION, NSApp);
  
}




NA_HDEF NABool naDispatchUIElementCommandCocoa(NSEvent* event){
//  printf("fire.\n");
////  NSEventType type = [event type];
//  [NSApp sendEvent:event];
}









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
@end




NA_DEF NAUInt naRunUI(){
  #ifndef NDEBUG
    if(CFRunLoopGetCurrent() != CFRunLoopGetMain())
      naError("naRunUI", "The UI should run in the main loop");
  #endif
  
  while(1){
    NSEvent* curevent = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantFuture] inMode:NSDefaultRunLoopMode dequeue:YES];
    if(curevent){
      NABool eventfinished = naDispatchUIElementCommandCocoa(curevent);
    }
  }

}













NA_HDEF void naRefreshUIElementCallback(NANativeUIID nativeID){
  [((NSView*)nativeID) setNeedsDisplay:YES];
}



NA_DEF void naRefreshUIElement(void* uielement, double timediff){
  naCallUIElementInSeconds(naRefreshUIElementCallback, naGetNativeID(uielement), timediff);
}








//void MBUIElement::setEnabled(MBBool enabled){
//  SendMessage(hWnd, WM_ENABLE, (WPARAM)enabled, 0);
//}
//
//
//
//void MBUIElement::leftMouseUp(int32 x, int32 y, int32 modifierkeys){
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseMove(int32 x, int32 y, int32 modifierkeys){
//  if(mousex == -1){
//    TRACKMOUSEEVENT tme;
//    tme.cbSize = sizeof(TRACKMOUSEEVENT);
//    tme.dwFlags = TME_LEAVE;
//    tme.hwndTrack = hWnd;
//    tme.dwHoverTime = HOVER_DEFAULT;
//    TrackMouseEvent(&tme);
//  }
//
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseLeave(){
//  mousex = -1;
//  mousey = -1;
//
//  TRACKMOUSEEVENT tme;
//  tme.cbSize = sizeof(TRACKMOUSEEVENT);
//  tme.dwFlags = TME_CANCEL;
//  tme.hwndTrack = hWnd;
//  tme.dwHoverTime = HOVER_DEFAULT;
//  TrackMouseEvent(&tme);
//}





@interface NANativeView : NSView
@end

@implementation NANativeView
- (void)drawRect:(NSRect)dirtyRect{
  printf("draw\n");
}
@end


NA_DEF NAWindow* naNewWindow(const char* title, double posx, double posy, double width, double height, NABool resizeable){
  NAWindow* window = naAlloc(NAWindow);
  window->windowedframe = naMakeRect(naMakePos(posx, posy), naMakeSize(width, height));
  window->fullscreen = NA_FALSE;
  NSRect contentRect = naMakeNSRectWithRect(window->windowedframe);
  NANativeUIID nativeID = [[NANativeWindow alloc] initWithNALibWindow:window contentRect:contentRect styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask backing:NSBackingStoreBuffered defer:NO screen:[NSScreen mainScreen]];
  NANativeView* nativeView = [[NANativeView alloc] initWithFrame:[[(NANativeWindow*)nativeID contentView] frame]];
  [(NANativeWindow*)nativeID setContentView:nativeView];
  [(NANativeWindow*)nativeID setDelegate:(NANativeWindow*)nativeID];
  [(NANativeWindow*)nativeID setInitialFirstResponder:[(NANativeWindow*)nativeID contentView]];
  [(NANativeWindow*)nativeID setAcceptsMouseMovedEvents:YES];
  naInitUIElement(&(window->uielement), (NAUIElement*)naGetApplication(), NA_UI_WINDOW, nativeID);
  return window;
}



NA_DEF void naClearWindow(NAWindow* window){
}



NA_DEF void naShowWindow(NAWindow* window){
  [(NANativeWindow*)(naGetNativeID(&(window->uielement))) makeKeyAndOrderFront:NA_NULL];
//  ShowWindow(hWnd, SW_SHOW);
}



NA_DEF void naSetWindowContentView(NAWindow* window, void* uielement){
  NAUIElement* element = (NAUIElement*)uielement;
  [(NANativeWindow*)(naGetNativeID(&(window->uielement))) setContentView:element->nativeID];
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  if(fullscreen == window->fullscreen){return;}
  if(fullscreen){
    window->windowedframe = naMakeRectWithNSRect([(NSWindow*)(window->uielement.nativeID) frame]);
    [(NSWindow*)(naGetNativeID(&(window->uielement))) setStyleMask:NSBorderlessWindowMask];
    [(NSWindow*)(naGetNativeID(&(window->uielement))) setFrame:[[NSScreen mainScreen] frame] display:YES];
    [(NSWindow*)(naGetNativeID(&(window->uielement))) setLevel:kCGScreenSaverWindowLevel];
  }else{
    [(NSWindow*)(naGetNativeID(&(window->uielement))) setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];
    [(NSWindow*)(naGetNativeID(&(window->uielement))) setFrame:naMakeNSRectWithRect(window->windowedframe) display:YES];
    [(NSWindow*)(naGetNativeID(&(window->uielement))) setLevel:NSNormalWindowLevel];
  }
  window->fullscreen = fullscreen;
  // Setting the first responder again is necessary as otherwise the first
  // responder is lost.
  [(NSWindow*)(naGetNativeID(&(window->uielement))) makeFirstResponder:[(NSWindow*)(naGetNativeID(&(window->uielement))) contentView]];
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
  naDispatchUIElementCommand(nalibopenglview, NA_UI_COMMAND_REDRAW, &dirtyRect);
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
  keyCode = NA_KEYCODE_COMMAND;
  naDispatchUIElementCommand(nalibopenglview, (command?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
}
- (void)mouseMoved:(NSEvent *)theEvent{
  NASize delta = naMakeSizeE([theEvent deltaX], [theEvent deltaY]);
  naDispatchUIElementCommand(nalibopenglview, NA_UI_COMMAND_MOUSEMOVED, &delta);
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
  NSRect frameRect = NSMakeRect(0, 0, width, height);
  NANativeUIID nativeID = [[NANativeOpenGLView alloc] initWithNALibView:openglview frame:frameRect pixelFormat:pixelformat];
  
//  if([self respondsToSelector:@selector(setWantsBestResolutionOpenGLSurface:)]){
//    #if defined __MAC_10_7
//      [self setWantsBestResolutionOpenGLSurface:YES];
//    #endif
//  }

  naInitUIElement(&(openglview->uielement), (NAUIElement*)window, NA_UI_OPENGLVIEW, nativeID);
  return openglview;
}


NA_DEF void naStartOpenGLDrawing(NAOpenGLView* openglview){
  [[(NANativeOpenGLView*)(openglview->uielement.nativeID) openGLContext] makeCurrentContext];
}

NA_DEF void naEndOpenGLDrawing(NAOpenGLView* openglview){
  [[(NANativeOpenGLView*)(openglview->uielement.nativeID) openGLContext] flushBuffer];
}



NA_DEF void naCenterMouseInView(NAOpenGLView* openglview){
  NSRect windowframe = [(NANativeWindow*)(naGetNativeID(&(naGetUIElementWindow((NAUIElement*)openglview)->uielement))) frame];
  CGPoint centerpos = {windowframe.origin.x + windowframe.size.width * .5f, windowframe.origin.y + windowframe.size.height * .5f};

  //NSRect viewrect = [self convertRect:[self bounds] toView:nil];
//  NSRect screenrect = [[self window] convertRectToScreen:viewrect];
//  CGPoint centerpos = {screenrect.origin.x + screenrect.size.width * .5f, screenrect.origin.y + screenrect.size.height * .5f};
//  centerpos.y = [[self window] screen].frame.size.height - centerpos.y;
//  
  //  // deprecated method for Snow leopard:
  //  NSPoint centerpos = [[self window] convertBaseToScreen:NSMakePoint(400, 400)];
  //  centerpos.y = [[self window] screen].frame.size.height - centerpos.y;
  
  CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, centerpos);
}



NA_DEF void naHideMouse(){
  CGDisplayHideCursor(kCGDirectMainDisplay);
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
