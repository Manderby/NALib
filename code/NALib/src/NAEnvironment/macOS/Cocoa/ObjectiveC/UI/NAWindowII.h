
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "NAPreferences.h"


@implementation NANativeWindow
- (id) initWithCoreWindow:(NACoreWindow*)newcorewindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  corewindow = newcorewindow;
  trackingcount = 0;
  trackingarea = nil;
  self.releasedWhenClosed = NO;
  return self;
}
- (NACoreWindow*) corewindow{
  return corewindow;
}
- (NSTrackingArea*) trackingarea{
  return trackingarea;
}
- (NAUInt) trackingcount{
  return trackingcount;
}
- (BOOL)windowShouldClose:(id)sender{
  NABool shouldClose;
  NA_UNUSED(sender);
  naSetFlagi(&(corewindow->flags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE, NA_TRUE);
  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_CLOSES);
  shouldClose = !naGetFlagi(corewindow->flags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING);
  naSetFlagi(&(corewindow->flags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE | NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, NA_FALSE);
  return (BOOL)shouldClose;
}
- (void)setContentRect:(NARect)rect{
  NSRect frame = [NSWindow frameRectForContentRect:naMakeNSRectWithRect(rect) styleMask:[self styleMask]];
  [super setFrame:frame display:YES];
  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_RESHAPE);
}
- (void)setWindowTitle:(const NAUTF8Char*) title{
  [self setTitle:[NSString stringWithUTF8String:title]];
}
- (void)setKeepOnTop:(NABool) keepOnTop{
  if(keepOnTop){
    [self setLevel:NSFloatingWindowLevel];
  }else{
    [self setLevel:NSNormalWindowLevel];
  }
}
- (void)renewMouseTracking{
  trackingarea = [[NSTrackingArea alloc] initWithRect:[[self contentView] bounds]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
      owner:self userInfo:nil];
  [[self contentView] addTrackingArea:trackingarea];
}
- (void)clearMouseTracking{
  [[self contentView] removeTrackingArea:trackingarea];
  NA_COCOA_DISPOSE(trackingarea);
  trackingarea = nil;
}
- (void)retainMouseTracking{
  trackingcount++;
  if(trackingcount == 1){
    [self setAcceptsMouseMovedEvents:YES];
    naRenewWindowMouseTracking(corewindow);
  }
}
- (void)releaseMouseTracking{
  trackingcount--;
  if(trackingcount == 0){
    [self setAcceptsMouseMovedEvents:NO];
    naClearWindowMouseTracking(corewindow);
  }
}
- (void)mouseMoved:(NSEvent*)event{
//  double deltaX = [event deltaX];
//  double deltaY = [event deltaY];
  naSetMouseMovedByDiff([event deltaX], [event deltaY]);
  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_MOUSE_MOVED);
//  [NSEvent setMouseCoalescingEnabled:NO];
}
- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  naSetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_MOUSE_ENTERED);
}
- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  naSetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_MOUSE_EXITED);
}
- (void)keyUp:(NSEvent*)event{
  NA_UNUSED(event);
  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_KEYUP);
}
- (void)windowDidResize:(NSNotification *)notification{
  NA_UNUSED(notification);
  naRememberWindowPosition(corewindow);
}
- (void)windowDidMove:(NSNotification *)notification{
  NA_UNUSED(notification);
  naRememberWindowPosition(corewindow);
}
@end



NA_DEF NAWindow* naNewWindow(const NAUTF8Char* title, NARect rect, NABool resizeable, NAInt storageTag){
  NSRect contentRect;
  NSUInteger styleMask;
  NANativeWindow* nativewindow;
  NASpace* space;
  NACoreWindow* corewindow = naAlloc(NACoreWindow);

  rect = naSetWindowStorageTag(corewindow, storageTag, rect, resizeable);

  contentRect = naMakeNSRectWithRect(rect);
  styleMask = NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable;
  if(resizeable){styleMask |= NAWindowStyleMaskResizable;}
  nativewindow = [[NANativeWindow alloc] initWithCoreWindow:corewindow contentRect:contentRect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO screen:nil];
  
  [nativewindow setDelegate:nativewindow];
  [nativewindow setTitle:[NSString stringWithUTF8String:title]];
  [nativewindow setInitialFirstResponder:[nativewindow contentView]];
  naInitCoreWindow(corewindow, NA_COCOA_PTR_OBJC_TO_C(nativewindow), NA_NULL, NA_FALSE, resizeable, rect);

  space = naNewSpace(rect.size);
  naSetWindowContentSpace(corewindow, space);

  naSetUIElementParent(corewindow, naGetApplication());

  return (NAWindow*)corewindow;
}



NA_DEF void naDestructWindow(NAWindow* window){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  [nativewindow close];
  naReleaseUIElement(corewindow->contentspace);
  naClearCoreWindow(corewindow);
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  [nativewindow setWindowTitle:title];
}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  [nativewindow setKeepOnTop:keepOnTop];
}



NA_DEF void naSetWindowRect(NAWindow* window, NARect rect){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  NARect currect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
  if(!naEqualRect(currect, rect)){
    [nativewindow setContentRect:rect];
  }
}



NA_DEF NAUIImageResolution naGetWindowUIResolution(NAWindow* window){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  CGFloat res = 1.;
  NA_MACOS_LEGACY_EXECUTE(7,
    {res = [nativewindow userSpaceScaleFactor];},
    {res = [nativewindow backingScaleFactor];})
  return (res == 1.) ? NA_UIIMAGE_RESOLUTION_1x : NA_UIIMAGE_RESOLUTION_2x;
}



NA_HDEF void naSetWindowFirstTabElement(NAWindow* window, NAUIElement* firstTabElem){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  naDefineNativeCocoaObject(NSView, nativeFirstTab, firstTabElem);
  [nativewindow setInitialFirstResponder:nativeFirstTab];
}



NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window){
  NARect rect;
  NSRect contentrect;
  NSRect windowframe;
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
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
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  windowframe = [nativewindow frame];
  rect.pos.x = windowframe.origin.x;
  rect.pos.y = windowframe.origin.y;
  rect.size.width = windowframe.size.width;
  rect.size.height = windowframe.size.height;
  return rect;
}



NA_HDEF void naRenewWindowMouseTracking(NACoreWindow* corewindow){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, corewindow);
  [nativewindow renewMouseTracking];
}



NA_HDEF void naClearWindowMouseTracking(NACoreWindow* corewindow){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, corewindow);
  [nativewindow clearMouseTracking];
}



//NA_HDEF void* naAllocMouseTracking(NAWindow* window){
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  [nativewindow retainMouseTracking];
//}
//
//
//
//NA_HDEF void naDeallocMouseTracking(NACoreWindow* corewindow){
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, corewindow);
//  [nativewindow releaseMouseTracking];
//}



NA_DEF void naClearWindow(NAWindow* window){
  NA_UNUSED(window);
}



NA_DEF void naShowWindow(NAWindow* window){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  [nativewindow makeKeyAndOrderFront:NA_NULL];
}



NA_DEF void naCloseWindow(NAWindow* window){
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  [nativewindow performClose:NA_NULL];
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, NAUIElement* uielement){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  naDefineNativeCocoaObject(NSView, nativeelem, uielement);
  if([nativewindow trackingarea]){naClearWindowMouseTracking(corewindow);}
  [nativewindow setContentView:nativeelem];
  [nativewindow setInitialFirstResponder:[nativewindow contentView]];
  
  corewindow->contentspace = (NACoreSpace*)uielement;
  naSetUIElementParent(uielement, window);
  
  if([nativewindow trackingcount]){naRenewWindowMouseTracking(corewindow);}
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
  if(fullscreen != naIsWindowFullscreen(window)){
    if(fullscreen){
      corewindow->windowedframe = naMakeRectWithNSRect([nativewindow frame]);
      [nativewindow setStyleMask:NAWindowStyleMaskBorderless];
      [nativewindow setFrame:[[NSScreen mainScreen] frame] display:YES];
      [nativewindow setLevel:kCGScreenSaverWindowLevel];
    }else{
      [nativewindow setStyleMask:NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable | NAWindowStyleMaskResizable];
      [nativewindow setFrame:naMakeNSRectWithRect(corewindow->windowedframe) display:YES];
      [nativewindow setLevel:NSNormalWindowLevel];
    }
    naSetFlagi(&(corewindow->flags), NA_CORE_WINDOW_FLAG_FULLSCREEN, fullscreen);
    // Setting the first responder again is necessary as otherwise the first
    // responder is lost.
    [nativewindow makeFirstResponder:[nativewindow contentView]];
  }
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
