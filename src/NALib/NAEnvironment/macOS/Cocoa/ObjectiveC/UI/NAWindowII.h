
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!




@implementation NANativeWindow
- (id) initWithCocoaWindow:(NACocoaWindow*)newcocoawindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  cocoawindow = newcocoawindow;
  trackingcount = 0;
  trackingarea = nil;
  self.releasedWhenClosed = NO;
  return self;
}
- (NACocoaWindow*) cocoawindow{
  return cocoawindow;
}
//- (BOOL)performMnemonic:(NSString *)string{
//  int asdf= 1234;
//  return NO;
//}
//- (BOOL)performKeyEquivalent:(NSEvent *)event{
//  NA_UNUSED(event);
////  int asdf= 1234;
////  NSString* chars = [event charactersIgnoringModifiers];
////  NSString* charsMod = [event characters];
////  NSEventModifierFlags flags = [event modifierFlags] & NSEventModifierFlagDeviceIndependentFlagsMask;
//  return NO;
//}
- (NSTrackingArea*) trackingarea{
  return trackingarea;
}
- (NAUInt) trackingcount{
  return trackingcount;
}
- (BOOL)windowShouldClose:(id)sender{
  NA_UNUSED(sender);
  NABool shouldClose = NA_TRUE;
  naDispatchUIElementCommand((NACoreUIElement*)cocoawindow, NA_UI_COMMAND_CLOSES, &shouldClose);
  return (BOOL)shouldClose;
}
- (void)setContentRect:(NARect)rect{
  NSRect frame = [NSWindow frameRectForContentRect:naMakeNSRectWithRect(rect) styleMask:[self styleMask]];
  [super setFrame:frame display:YES];
  naDispatchUIElementCommand((NACoreUIElement*)cocoawindow, NA_UI_COMMAND_RESHAPE, &rect);
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
- (void)keyUp:(NSEvent*)event{
  NAUIKeyCode keyCode = [event keyCode];
  naDispatchUIElementCommand((NACoreUIElement*)cocoawindow, NA_UI_COMMAND_KEYUP, &keyCode);
}
@end



NA_DEF NAWindow* naNewWindow(const NAUTF8Char* title, NARect rect, NABool resizeable){
  NACocoaWindow* cocoawindow = naAlloc(NACocoaWindow);
  cocoawindow->corewindow.windowedframe = rect;
  cocoawindow->corewindow.fullscreen = NA_FALSE;

  NSRect contentRect = naMakeNSRectWithRect(cocoawindow->corewindow.windowedframe);
  NSUInteger styleMask = NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable;
  if(resizeable){styleMask |= NAWindowStyleMaskResizable;}
  NANativeWindow* nativewindow = [[NANativeWindow alloc] initWithCocoaWindow:cocoawindow contentRect:contentRect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO screen:[NSScreen mainScreen]];
  
  [nativewindow setDelegate:nativewindow];
  [nativewindow setTitle:[NSString stringWithUTF8String:title]];
  [nativewindow setInitialFirstResponder:[nativewindow contentView]];
  naRegisterCoreUIElement(&(cocoawindow->corewindow.uielement), NA_UI_WINDOW, (void*)NA_COCOA_RETAIN(nativewindow));

  NASpace* space = naNewSpace(naMakeRectWithNSRect([[nativewindow contentView] frame]));
  naSetWindowContentSpace(cocoawindow, space);

  naSetUIElementParent(cocoawindow, naGetApplication());

  return (NAWindow*)cocoawindow;
}



NA_DEF void naDestructWindow(NAWindow* window){
  NACocoaWindow* cocoawindow = (NACocoaWindow*)window;
  [((NA_COCOA_BRIDGE NANativeWindow*)naGetUIElementNativeID(window)) close];
  NA_COCOA_RELEASE(naUnregisterCoreUIElement(&(cocoawindow->corewindow.uielement)));
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title){
  [((NA_COCOA_BRIDGE NANativeWindow*)naGetUIElementNativeID(window)) setWindowTitle:title];
}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop){
  [((NA_COCOA_BRIDGE NANativeWindow*)naGetUIElementNativeID(window)) setKeepOnTop:keepOnTop];
}



NA_DEF void naSetWindowRect(NAWindow* window, NARect rect){
  [((NA_COCOA_BRIDGE NANativeWindow*)naGetUIElementNativeID(window)) setContentRect:rect];
}



NA_DEF float naGetWindowUIScaleFactor(NAWindow* window){
  NANativeWindow* nativeWindow = naGetUIElementNativeID(window);
  CGFloat res = 1.;
  if(NSAppKitVersionNumber >= NSAppKitVersionNumber10_7){
    #ifdef __MAC_10_7
      res = [nativeWindow backingScaleFactor];
    #endif
  }else{
    #ifndef __MAC_10_7
      res = [nativeWindow userSpaceScaleFactor];
    #endif
  }
  return (res == 1.) ? NA_UIIMAGE_RESOLUTION_1x : NA_UIIMAGE_RESOLUTION_2x;
}


NA_HDEF void naSetWindowFirstTabElement(NAWindow* window, NAUIElement* nextelem){
  [((NA_COCOA_BRIDGE NANativeWindow*)naGetUIElementNativeID(window)) setInitialFirstResponder:naGetUIElementNativeID(nextelem)];
}

NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window){
  NARect rect;
  NSRect contentrect;
  NSRect windowframe;
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
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
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  windowframe = [nativewindow frame];
  rect.pos.x = windowframe.origin.x;
  rect.pos.y = windowframe.origin.y;
  rect.size.width = windowframe.size.width;
  rect.size.height = windowframe.size.height;
  return rect;
}



NA_HDEF void naRenewWindowMouseTracking(NACocoaWindow* cocoawindow){
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  [nativewindow renewMouseTracking];
}



NA_HDEF void naClearWindowMouseTracking(NACocoaWindow* cocoawindow){
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  [nativewindow clearMouseTracking];
}



NA_HDEF void naRetainWindowMouseTracking(NAWindow* window){
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  [nativewindow retainMouseTracking];
}



NA_HDEF void naReleaseWindowMouseTracking(NACocoaWindow* cocoawindow){
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)cocoawindow));
  [nativewindow releaseMouseTracking];
}



NA_DEF void naClearWindow(NAWindow* window){
  NA_UNUSED(window);
}



NA_DEF void naShowWindow(NAWindow* window){
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  [nativewindow makeKeyAndOrderFront:NA_NULL];
}



NA_DEF void naCloseWindow(NAWindow* window){
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  [nativewindow performClose:NA_NULL];
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, NAUIElement* uielement){
  NACocoaWindow* cocoawindow = (NACocoaWindow*)window;
  NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
  if([nativewindow trackingarea]){naClearWindowMouseTracking(cocoawindow);}
  NACoreUIElement* element = (NACoreUIElement*)uielement;
  [nativewindow setContentView:(NA_COCOA_BRIDGE NSView*)element->nativeID];
  [(NA_COCOA_BRIDGE NSWindow*)naGetUIElementNativeID((NAUIElement*)window) setInitialFirstResponder:[nativewindow contentView]];
  
  cocoawindow->corewindow.contentspace = (NACoreSpace*)uielement;
  naSetUIElementParent(uielement, window);
  
  if([nativewindow trackingcount]){naRenewWindowMouseTracking(cocoawindow);}
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  if(fullscreen != corewindow->fullscreen){
    NANativeWindow* nativewindow = (NA_COCOA_BRIDGE NANativeWindow*)(naGetUIElementNativeID((NAUIElement*)window));
    if(fullscreen){
      corewindow->windowedframe = naMakeRectWithNSRect([(NA_COCOA_BRIDGE NSWindow*)(corewindow->uielement.nativeID) frame]);
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
  return corewindow->contentspace;
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
