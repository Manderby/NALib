
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "NAPreferences.h"


@implementation NACocoaWindow
- (id) initWithCoreWindow:(NACoreWindow*)newcorewindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  corewindow = newcorewindow;
  trackingcount = 0;
  trackingarea = nil;
  [self setReleasedWhenClosed:NO];
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
  naSetMouseMovedByDiff([event deltaX], -[event deltaY]);
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
  NACocoaWindow* cocoawindow;
  NASpace* space;
  NACoreWindow* corewindow = naAlloc(NACoreWindow);

  rect = naSetWindowStorageTag(corewindow, storageTag, rect, resizeable);

  contentRect = naMakeNSRectWithRect(rect);
  styleMask = NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable;
  if(resizeable){styleMask |= NAWindowStyleMaskResizable;}
  cocoawindow = [[NACocoaWindow alloc] initWithCoreWindow:corewindow contentRect:contentRect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO screen:nil];
  
  [cocoawindow setDelegate:cocoawindow];
  [cocoawindow setTitle:[NSString stringWithUTF8String:title]];
  [cocoawindow setInitialFirstResponder:[cocoawindow contentView]];
  naInitCoreWindow(corewindow, NA_COCOA_PTR_OBJC_TO_C(cocoawindow), NA_NULL, NA_FALSE, resizeable, rect);

  space = naNewSpace(rect.size);
  naSetWindowContentSpace(corewindow, space);

  naSetUIElementParent(corewindow, naGetApplication());

  return (NAWindow*)corewindow;
}



NA_DEF void naDestructWindow(NAWindow* window){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  [cocoawindow close];
  naReleaseUIElement(corewindow->contentspace);
  naClearCoreWindow(corewindow);
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  [cocoawindow setWindowTitle:title];
}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  [cocoawindow setKeepOnTop:keepOnTop];
}



NA_DEF void naSetWindowRect(NAWindow* window, NARect rect){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  NARect currect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
  if(!naEqualRect(currect, rect)){
    [cocoawindow setContentRect:rect];
  }
}



NA_DEF NAUIImageResolution naGetWindowUIResolution(NAWindow* window){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  CGFloat res = naGetWindowBackingScaleFactor(cocoawindow);

  return (res == 1.) ? NA_UIIMAGE_RESOLUTION_1x : NA_UIIMAGE_RESOLUTION_2x;
}



NA_HDEF void naSetWindowFirstTabElement(NAWindow* window, NAUIElement* firstTabElem){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  naDefineCocoaObject(NSView, cocoaFirstTab, firstTabElem);
  [cocoawindow setInitialFirstResponder:cocoaFirstTab];
}



NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window){
  NARect rect;
  NSRect contentrect;
  NSRect windowframe;
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  contentrect = [[cocoawindow contentView] frame];
  windowframe = [cocoawindow frame];
  rect.pos.x = windowframe.origin.x + contentrect.origin.x;
  rect.pos.y = windowframe.origin.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteOuterRect(NACoreUIElement* window){
  NARect rect;
  NSRect windowframe;
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  windowframe = [cocoawindow frame];
  rect.pos.x = windowframe.origin.x;
  rect.pos.y = windowframe.origin.y;
  rect.size.width = windowframe.size.width;
  rect.size.height = windowframe.size.height;
  return rect;
}



NA_HDEF void naRenewWindowMouseTracking(NACoreWindow* corewindow){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, corewindow);
  [cocoawindow renewMouseTracking];
}



NA_HDEF void naClearWindowMouseTracking(NACoreWindow* corewindow){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, corewindow);
  [cocoawindow clearMouseTracking];
}



//NA_HDEF void* naAllocMouseTracking(NAWindow* window){
//  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
//  [cocoawindow retainMouseTracking];
//}
//
//
//
//NA_HDEF void naDeallocMouseTracking(NACoreWindow* corewindow){
//  naDefineCocoaObject(NACocoaWindow, cocoawindow, corewindow);
//  [cocoawindow releaseMouseTracking];
//}



NA_DEF void naClearWindow(NAWindow* window){
  NA_UNUSED(window);
}



NA_DEF void naShowWindow(NAWindow* window){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  [cocoawindow makeKeyAndOrderFront:NA_NULL];
}



NA_DEF void naCloseWindow(NAWindow* window){
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  [cocoawindow performClose:NA_NULL];
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, NAUIElement* uielement){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  naDefineCocoaObject(NSView, cocoaelem, uielement);
  if([cocoawindow trackingarea]){naClearWindowMouseTracking(corewindow);}
  [cocoawindow setContentView:cocoaelem];
  [cocoawindow setInitialFirstResponder:[cocoawindow contentView]];
  
  corewindow->contentspace = (NACoreSpace*)uielement;
  naSetUIElementParent(uielement, window);
  
  if([cocoawindow trackingcount]){naRenewWindowMouseTracking(corewindow);}
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  naDefineCocoaObject(NACocoaWindow, cocoawindow, window);
  if(fullscreen != naIsWindowFullscreen(window)){
    if(fullscreen){
      corewindow->windowedframe = naMakeRectWithNSRect([cocoawindow frame]);
      [cocoawindow setStyleMask:NAWindowStyleMaskBorderless];
      [cocoawindow setFrame:[[NSScreen mainScreen] frame] display:YES];
      [cocoawindow setLevel:kCGScreenSaverWindowLevel];
    }else{
      [cocoawindow setStyleMask:NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable | NAWindowStyleMaskResizable];
      [cocoawindow setFrame:naMakeNSRectWithRect(corewindow->windowedframe) display:YES];
      [cocoawindow setLevel:NSNormalWindowLevel];
    }
    naSetFlagi(&(corewindow->flags), NA_CORE_WINDOW_FLAG_FULLSCREEN, fullscreen);
    // Setting the first responder again is necessary as otherwise the first
    // responder is lost.
    [cocoawindow makeFirstResponder:[cocoawindow contentView]];
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
