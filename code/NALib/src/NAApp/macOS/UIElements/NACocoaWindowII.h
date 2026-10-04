
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../NAPreferences.h"



struct NACocoaWindow{
  NAWindow window;
};
NA_HAPI void na_DestructCocoaWindow(NACocoaWindow* cocoaWindow);
NA_RUNTIME_TYPE(NACocoaWindow, na_DestructCocoaWindow, NA_FALSE);




@implementation NACocoaNativeWindow

- (id) initWithWindow:(NACocoaWindow*)newCocoaWindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
  cocoaWindow = newCocoaWindow;
  [self setReleasedWhenClosed:NO];
  [self setHasShadow:YES];
  return self;
}

- (NACocoaWindow*) window{
  return cocoaWindow;
}

- (BOOL)windowShouldClose:(id)sender{
  NABool shouldClose;
  NA_UNUSED(sender);
  naSetFlagu32(&cocoaWindow->window.coreFlags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE, NA_TRUE);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_CLOSES)) {
    // no super method to be called.
  }
  shouldClose = !naGetFlagu32(cocoaWindow->window.coreFlags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING);
  naSetFlagu32(&cocoaWindow->window.coreFlags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE | NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, NA_FALSE);
  return (BOOL)shouldClose;
}

- (NARect)getContentRect{
  NSRect contentRect = [NSWindow contentRectForFrameRect:[super frame] styleMask:[self styleMask]];
  return naMakeRectWithNSRect(contentRect);
}

- (void)setContentRect:(NARect)rect{
  NSRect frame = [NSWindow frameRectForContentRect:naMakeNSRectWithRect(rect) styleMask:[self styleMask]];
  if(!NSEqualRects([self frame], frame)) {
    [self setFrame: frame];
  }
}

- (void)setFrame:(NSRect)frame {
  [super setFrame:frame display:YES];
  // mouse tracking will be updated in windowDidResize
}

- (void)setWindowTitle:(const NAUTF8Char*) title{
  [self setTitle:[NSString stringWithUTF8String:title]];
}

- (void)markAsChanged:(NABool) changed{
  [self setDocumentEdited:changed ? YES : NO];
}

- (void)setKeepOnTop:(NABool) keepOnTop{
  if(keepOnTop) {
    [self setLevel:NSFloatingWindowLevel];
  }else{
    [self setLevel:NSNormalWindowLevel];
  }
}

- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  NAMouseStatus* mouseStatus = na_GetApplicationMouseStatus(naGetApplication());
  na_SetMouseEnteredAtPos(
    mouseStatus,
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_MOUSE_ENTERED)) {
    [super mouseEntered:event];
  }
}

- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  NAMouseStatus* mouseStatus = na_GetApplicationMouseStatus(naGetApplication());
  na_SetMouseExitedAtPos(
    mouseStatus,
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_MOUSE_EXITED)) {
    [super mouseExited:event];
  }
}

- (void)keyDown:(NSEvent*)event{
  NA_UNUSED(event);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_KEY_DOWN)) {
    [super keyDown:event];
  }
}

- (void)keyUp:(NSEvent*)event{
  NA_UNUSED(event);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_KEY_UP)) {
    [super keyUp:event];
  }
}

- (BOOL)performKeyEquivalent:(NSEvent*)event{
  NA_UNUSED(event);
  NABool handeled = na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_KEY_DOWN);
  if(!handeled) {
    [super keyUp:event];
  }
  return handeled ? YES : NO;
}

- (void)windowDidResize:(NSNotification *)notification{
  NA_UNUSED(notification);
  // asdf

  na_UpdateWindowScreen((NAWindow*)cocoaWindow, na_GetApplicationScreenWithNativeScreenPtr((NA_COCOA_BRIDGE void*)[self screen]));
  na_UpdateMouseTracking(&cocoaWindow->window.uiElement);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_RESHAPE)) {
    // no super method to be called.
  }
}

- (void)windowDidMove:(NSNotification *)notification{
  // Note that this method will also be called after a window is created with
  // naNewWindow when the rect is outside of the main screen.
  NA_UNUSED(notification);
  // asdf

  na_UpdateWindowScreen((NAWindow*)cocoaWindow, na_GetApplicationScreenWithNativeScreenPtr((NA_COCOA_BRIDGE void*)[self screen]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_RESHAPE)) {
    // no super method to be called.
  }
}

- (void)windowDidChangeBackingProperties:(NSNotification *)notification{    
  // This method is called for every window by itself but is always called
  // after handleApplicationDidChangeScreenParameters which in turn already
  // created a list of new NAScreen entries and attached each window to a
  // proper NAScreen entry and repositioned it properly.
  
  NA_UNUSED(notification);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_RESHAPE)) {
    // no super method to be called.
  }
}

- (BOOL)canBecomeMainWindow{
  return !naGetFlagu32(cocoaWindow->window.flags, NA_WINDOW_AUXILIARY);
}

- (BOOL)canBecomeKeyWindow{
  return (BOOL)naGetFlagu32(cocoaWindow->window.coreFlags, NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS);
}

- (void)windowDidBecomeMain:(NSNotification *)notification{
  NA_UNUSED(notification);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaWindow, NA_UI_COMMAND_PRESSED)) {
    // no super method to be called.
  }
}

@end



NA_DEF NAWindow* naNewWindow(
  const NAUTF8Char* title,
  NARect rect,
  uint32 flags)
{
  NACocoaWindow* cocoaWindow = naNew(NACocoaWindow);

  NABool resizeable = naGetFlagu32(flags, NA_WINDOW_RESIZEABLE);
  NABool titleless = naGetFlagu32(flags, NA_WINDOW_TITLELESS);
  NABool noncloseable = naGetFlagu32(flags, NA_WINDOW_NON_CLOSEABLE);
  NABool nonminiaturizeable = naGetFlagu32(flags, NA_WINDOW_NON_MINIATURIZEABLE);
  NABool auxiliary = naGetFlagu32(flags, NA_WINDOW_AUXILIARY);
  
  NSUInteger styleMask = 0;
  if(resizeable) {
    styleMask |= NAWindowStyleMaskResizable;
  }
  if(!titleless) {
    styleMask |= NAWindowStyleMaskTitled;
  }
  if(!noncloseable) {
    styleMask |= NAWindowStyleMaskClosable;
  }
  if(!nonminiaturizeable) {
    styleMask |= NAWindowStyleMaskMiniaturizable;
  }
  // Commented out because in newer macOS versions, there is no more distinction
  // between auxiliary and normal windows. And the non-activating property
  // doesn't really work well together with other properties. This will probably
  // never be reintroduced again, since further below, some properties of
  // auxiliary windows are set manually, but this remains here for reference.
//  if(auxiliary) {
//    styleMask |= NAWindowStyleMaskNonactivatingPanel | NAWindowStyleMaskUtilityWindow;
//  }
  
  // Correct the rect such that the titlebar is at least partially on a screen.
  naCorrectApplicationWindowRect(&rect, titleless);
  
  const NAScreen* screen = naGetApplicationScreenWithPos(naGetRectCenter(rect));
  if(screen) {
    NARect screenRect = na_GetScreenRect(&screen->uiElement);
    rect.pos.x -= screenRect.pos.x;
    rect.pos.y -= screenRect.pos.y;
  }
  
  naDefineUIElementNativeCocoaObjConst(NSScreen, nativeScreenObj, screen);

  NACocoaNativeWindow* nativeObj = [[NACocoaNativeWindow alloc]
    initWithWindow:cocoaWindow
    contentRect:naMakeNSRectWithRect(rect)
    styleMask:styleMask
    backing:NSBackingStoreBuffered
    defer:NO
    screen:nativeScreenObj];
  
  // asdf
  NAScreen* actualScreen = na_GetApplicationScreenWithNativeScreenPtr((NA_COCOA_BRIDGE void*)[nativeObj screen]);
  
  if(auxiliary) {
    [nativeObj setKeepOnTop:YES];
    [nativeObj setHidesOnDeactivate:YES];
    [nativeObj setCollectionBehavior:NSWindowCollectionBehaviorTransient | NAWindowCollectionBehaviorFullScreenAuxiliary];
    [nativeObj setExcludedFromWindowsMenu:YES];
  }
  
  [nativeObj setDelegate:nativeObj];
  [nativeObj setTitle:[NSString stringWithUTF8String:title]];
  [nativeObj setInitialFirstResponder:[nativeObj contentView]];
  na_InitCoreWindow(
    (NAWindow*)cocoaWindow,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    actualScreen,
    NA_NULL,
    NA_FALSE,
    resizeable,
    rect);

  cocoaWindow->window.flags = flags;

  NASpace* space = naNewSpace(rect.size);
  if(titleless) {
    naSetSpaceDragsWindow(space, NA_TRUE);
  }

  naSetWindowContentSpace((NAWindow*)cocoaWindow, space);

  return (NAWindow*)cocoaWindow;
}



NA_DEF void na_DestructCocoaWindow(NACocoaWindow* cocoaWindow) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, cocoaWindow);
  [nativeObj close];
  
  na_ClearCoreWindow((NAWindow*)cocoaWindow);
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, window);
  [nativeObj setWindowTitle:title];
}



NA_DEF void naSetWindowSkin(NAWindow* window, NASkin skin) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, window);
  switch(skin) {
  case NA_SKIN_DARK:
     [nativeObj setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantDark]];
    break;
  case NA_SKIN_LIGHT:
     [nativeObj setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantLight]];
    break;
  case NA_SKIN_PLAIN:
  case NA_SKIN_SYSTEM:
    // Setting the appearance to nil resets it to whatever effectiveAppearance
    // of the Application is.
    [nativeObj setAppearance:nil];
    break;
  }
}



NA_DEF void naSetWindowDocumentUrl(NAWindow* window, const NAUTF8Char* url) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, window);
  if(url) {
    NSURL* nsurl = [[NSURL alloc] initFileURLWithPath:[NSString stringWithFormat:@"%s", url]];
    [nativeObj setRepresentedURL:nsurl];
  }else{
    [nativeObj setRepresentedURL:nil];
  }
}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, window);
  [nativeObj setKeepOnTop:keepOnTop];
}



NA_DEF void naSetWindowFirstTabElement(NAWindow* window, const void* firstTabElem) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeWindowObj, window);
  naDefineUIElementNativeCocoaObjConst(NSView, cocoaFirstTabObj, firstTabElem);
  [nativeWindowObj setInitialFirstResponder:cocoaFirstTabObj];
}



NA_HDEF NARect na_GetNativeWindowAbsoluteInnerRect(const NSWindow* window) {
  NARect rect;
  NSRect contentRect;
  NSRect windowFrame;
  contentRect = [[window contentView] frame];
  windowFrame = [window frame];
  rect.pos.x = windowFrame.origin.x + contentRect.origin.x;
  rect.pos.y = windowFrame.origin.y + contentRect.origin.y;
  rect.size.width = contentRect.size.width;
  rect.size.height = contentRect.size.height;
  return rect;
}



NA_HDEF NARect na_GetWindowAbsoluteInnerRect(const NA_UIElement* window) {
  naDefineUIElementNativeCocoaObjConst(NSWindow, nativeObj, window);

  NARect rect;
  NSRect contentRect;
  NSRect windowFrame;
  contentRect = [[nativeObj contentView] frame];
  windowFrame = [nativeObj frame];
  rect.pos.x = windowFrame.origin.x + contentRect.origin.x;
  rect.pos.y = windowFrame.origin.y + contentRect.origin.y;
  rect.size.width = contentRect.size.width;
  rect.size.height = contentRect.size.height;
  return rect;
}



NA_DEF void naShowWindow(const NAWindow* window) {
  // Unfortunately, macOS automatically changes the position of the window if
  // it does not fit properly on the screen. We do not want that, therefore,
  // we need to manually readjust the intended rect after the call to
  // makeKeyAndOrderFront. Which requires a mutable pointer. Not great but
  // that's what you get if the os is trying to do things automatically which
  // are not supposed to.
  NAWindow* mutableWindow = (NAWindow*)window;
  
  na_UpdateUIElementUIScale(mutableWindow);

  naDefineUIElementNativeCocoaObjConst(NACocoaNativeWindow, nativeObj, window);
  NARect rect = na_GetWindowRect(&window->uiElement);
  [nativeObj makeKeyAndOrderFront:NA_NULL];
  na_SetWindowRect(&mutableWindow->uiElement, rect);
}

NA_DEF void naShowWindowModal(NAWindow* window, NAWindow* parentWindow) {
  NA_UNUSED(parentWindow);
  naDefineUIElementNativeCocoaObjConst(NACocoaNativeWindow, nativeObj, window);
  [NSApp runModalForWindow: nativeObj];
}

NA_DEF void naCloseWindowModal(NAWindow* window) {
  naDefineUIElementNativeCocoaObjConst(NACocoaNativeWindow, nativeObj, window);
  [NSApp endSheet: nativeObj];
}



NA_DEF void naCloseWindow(const NAWindow* window) {
  naDefineUIElementNativeCocoaObjConst(NACocoaNativeWindow, nativeObj, window);
  NABool titleless = naGetFlagu32(window->flags, NA_WINDOW_TITLELESS);
  NABool noncloseable = naGetFlagu32(window->flags, NA_WINDOW_NON_CLOSEABLE);
  if(titleless || noncloseable) {
    [nativeObj close];
  }else{
    [nativeObj performClose:nil];
  }
}



NA_DEF void naMarkWindowChanged(NAWindow* window, NABool changed) {
  naDefineUIElementNativeCocoaObjConst(NACocoaNativeWindow, nativeObj, window);
  [nativeObj markAsChanged:changed];
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, void* space) {
  #if NA_DEBUG
    if(!window)
      naError("window is nullptr");
    if(!space)
      naError("space is nullptr");
    if((naGetUIElementType(space) != NA_UI_SPACE) &&
      (naGetUIElementType(space) != NA_UI_IMAGE_SPACE) &&
      (naGetUIElementType(space) != NA_UI_OPENGL_SPACE) &&
      (naGetUIElementType(space) != NA_UI_METAL_SPACE))
      naError("Require a space, not any arbitrary ui element.");
  #endif

  if(space == window->contentSpace) {
    #if NA_DEBUG
      naError("Setting the same content space again. Nothing will happen");
    #endif
    return;
  }

  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeWindowObj, window);
  naDefineUIElementNativeCocoaObjConst(NSView, nativeUIElementObj, space);

  [nativeWindowObj setContentView:nativeUIElementObj];
  [nativeWindowObj setInitialFirstResponder:[nativeWindowObj contentView]];
    
  if(window->contentSpace) { naDelete(window->contentSpace); }
  window->contentSpace = space;
  na_SetUIElementParent(space, window);
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullScreen) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, window);
  if(fullScreen != naIsWindowFullscreen(window)) {
    if(fullScreen) {
      window->windowedFrame = naMakeRectWithNSRect([nativeObj frame]);
      [nativeObj setStyleMask:NAWindowStyleMaskBorderless];
      [nativeObj setFrame:[[NSScreen mainScreen] frame]];
      [nativeObj setLevel:kCGScreenSaverWindowLevel];
    }else{
      [nativeObj setStyleMask:NAWindowStyleMaskTitled | NAWindowStyleMaskClosable | NAWindowStyleMaskMiniaturizable | NAWindowStyleMaskResizable];
      [nativeObj setFrame:naMakeNSRectWithRect(window->windowedFrame)];
      [nativeObj setLevel:NSNormalWindowLevel];
    }
    naSetFlagu32(&window->coreFlags, NA_CORE_WINDOW_FLAG_FULLSCREEN, fullScreen);
    // Setting the first responder again is necessary as otherwise the first
    // responder is lost.
    [nativeObj makeFirstResponder:[nativeObj contentView]];
  }
}



NA_DEF void naSetWindowAcceptsKeyboardReactions(NAWindow* window, NABool accepts) {
  naSetFlagu32(&window->coreFlags, NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS, accepts);
}


NA_HDEF NARect na_GetWindowRect(const NA_UIElement* window) {
  naDefineUIElementNativeCocoaObjConst(NACocoaNativeWindow, nativeObj, window);
  return [nativeObj getContentRect];
}

NA_HDEF void na_SetWindowRect(NA_UIElement* window, NARect rect) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, window);
  [nativeObj setContentRect:rect];
}

NA_DEF NARect naGetWindowOuterRect(const NAWindow* window) {
  naDefineUIElementNativeCocoaObjConst(NACocoaNativeWindow, nativeObj, window);
  return naMakeRectWithNSRect([nativeObj frame]);
}

NA_DEF void naSetWindowOuterRect(NAWindow* window, NARect rect) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeWindow, nativeObj, window);
  [nativeObj setFrame:naMakeNSRectWithRect(rect)];
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
