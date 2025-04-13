
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



struct NACocoaSpace{
  NASpace space;
};
NA_HAPI void na_DestructCocoaSpace(NACocoaSpace* _Nonnull cocoaSpace);
NA_RUNTIME_TYPE(NACocoaSpace, na_DestructCocoaSpace, NA_FALSE);



@implementation NACocoaNativeSpace

- (id _Nonnull) initWithSpace:(NACocoaSpace* _Nonnull)newCocoaSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  [self setWantsLayer:YES];
  [self resetDrag];

  cocoaSpace = newCocoaSpace;
  return self;
}

- (BOOL)acceptsFirstResponder{
  return YES; // This is required to get keyboard input.
}

- (void)drawRect:(NSRect)dirtyRect{
  [super drawRect:dirtyRect];

  dirtyRect = [self frame];
  dirtyRect.origin = NSMakePoint(0., 0.);

  NAColor bgColor;
  naFillSpaceBackgroundColor(&bgColor, &cocoaSpace->space);
  NSColor* curBgColor = naAllocUIColor(&bgColor, NA_NULL);
  [curBgColor setFill];

  NSRectFill(dirtyRect);

  naDeallocUIColor(curBgColor);
}

- (void)mouseMoved:(NSEvent* _Nonnull)event{
  NA_UNUSED(event);
  NAMouseStatus* mouseStatus = na_GetApplicationMouseStatus(naGetApplication());
  na_SetMouseMovedTo(
    mouseStatus,
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
    [super mouseMoved:event];
  }
}

- (void)mouseDown:(NSEvent* _Nonnull)event{
  NAMouseStatus* mouseStatus = na_GetApplicationMouseStatus(naGetApplication());
  na_SetMouseEnteredAtPos(
    mouseStatus,
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  na_SetMouseButtonPressed(
    mouseStatus,
    NA_MOUSE_BUTTON_LEFT,
    ([NSEvent pressedMouseButtons] & (1 << 0)) != 0);

  if(cocoaSpace->space.dragsWindow) {
    isMoving = NA_TRUE;
    originMousePos = naMakePosWithNSPoint([event locationInWindow]);
  }else{
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_DOWN)) {
      [super mouseDown:event];
    }
  }
}

- (void)mouseUp:(NSEvent* _Nonnull)event{
  NAMouseStatus* mouseStatus = na_GetApplicationMouseStatus(naGetApplication());
  na_SetMouseExitedAtPos(
    mouseStatus,
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  na_SetMouseButtonPressed(
    mouseStatus,
    NA_MOUSE_BUTTON_LEFT,
    ([NSEvent pressedMouseButtons] & (1 << 0)) != 0);

  if(cocoaSpace->space.dragsWindow) {
    [self resetDrag];
  }else{
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_UP)) {
      [super mouseUp:event];
    }
  }
}

- (void)mouseDragged:(NSEvent* _Nonnull)event{
  if(cocoaSpace->space.dragsWindow && isMoving) {
    NAPos curMousePos = naMakePosWithNSPoint([event locationInWindow]);
    NSRect frame = [[self window] frame];
    frame.origin.x += curMousePos.x - originMousePos.x;
    frame.origin.y += curMousePos.y - originMousePos.y;

    [[self window] setFrame:frame display:YES];
  }else{
    [super mouseDragged:event];
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
      // don't know what to do.
    }
  }
}

- (void)mouseEntered:(NSEvent* _Nonnull)event{
  NA_UNUSED(event);
  NAMouseStatus* mouseStatus = na_GetApplicationMouseStatus(naGetApplication());
  na_SetMouseEnteredAtPos(
    mouseStatus,
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_ENTERED)) {
    [super mouseEntered:event];
  }
}

- (void)mouseExited:(NSEvent* _Nonnull)event{
  NA_UNUSED(event);
  NAMouseStatus* mouseStatus = na_GetApplicationMouseStatus(naGetApplication());
  na_SetMouseExitedAtPos(
    mouseStatus,
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_MOUSE_EXITED)) {
    [super mouseExited:event];
  }
}

- (void)keyDown:(NSEvent* _Nonnull)event{
  na_CaptureKeyboardStatus(event);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_KEY_DOWN)) {
    [super keyDown:event];
  }
}

- (void)keyUp:(NSEvent* _Nonnull)event{
  na_CaptureKeyboardStatus(event);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaSpace, NA_UI_COMMAND_KEY_UP)) {
    [super keyUp:event];
  }
}

- (void)resetDrag{
  isMoving = NA_FALSE;
  originMousePos = naMakePos(0, 0);
}

- (BOOL)acceptsFirstMouse:(NSEvent* _Nullable)event{
  NA_UNUSED(event);
  return YES;
}

- (void)cancelOperation:(nullable id)sender{
  [self resetDrag];
}

- (void)setFrame:(NSRect)frame {
  [super setFrame:frame];
//  na_UpdateMouseTracking(&cocoaSpace->space.uiElement);
}

@end



NA_DEF NASpace* _Nonnull naNewSpace(NASize size) {
  NACocoaSpace* cocoaSpace = naNew(NACocoaSpace);

  NACocoaNativeSpace* nativePtr = [[NACocoaNativeSpace alloc]
    initWithSpace:cocoaSpace
    frame:naMakeNSRectWithSize(size)];  
  na_InitSpace((NASpace*)cocoaSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  cocoaSpace->space.alternateBackground = NA_FALSE;
  cocoaSpace->space.dragsWindow = NA_FALSE;

  return (NASpace*)cocoaSpace;
}



NA_DEF void na_DestructCocoaSpace(NACocoaSpace* _Nonnull cocoaSpace) {
  na_ClearSpace((NASpace*)cocoaSpace);
}



NA_DEF void naSetSpaceRect(NASpace* _Nonnull space, NARect rect) {
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  NSRect frame = naMakeNSRectWithRect(rect);
  [nativePtr setFrame: frame];
}



NA_DEF void naSetSpaceVisible(NASpace* _Nonnull space, NABool visible) {
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  [nativePtr setHidden:visible ? NO : YES];
}



NA_DEF void naSetSpaceDragsWindow(NASpace* _Nonnull space, NABool isDraggable) {
  space->dragsWindow = isDraggable;
}



@class NACocoaNativeRadio;
@class NACocoaNativeTextBox;


NA_HDEF NSView* _Nonnull na_getNSViewOfChild(void* _Nonnull child) {
  naDefineCocoaObjectConst(NSView<NACocoaNativeEncapsulatedElement>, cocoaView, child);

  NSView* childView;  
  switch(naGetUIElementType(child)) {
  case NA_UI_RADIO:
    childView = [cocoaView getEncapsulatingView];
    break;
  case NA_UI_TEXTBOX:
    childView = [cocoaView getEncapsulatingView];
    break;
  default:
    childView = cocoaView;
    break;
  }
  
  return childView;
}



NA_DEF void naAddSpaceChild(NASpace* _Nonnull space, void* _Nonnull child, NAPos pos) {
  naDefineCocoaObject(NACocoaNativeSpace, nativeSpacePtr, space);

  NSView* childView = na_getNSViewOfChild(child);  
  
  double offsetX = na_GetUIElementXOffset(child);
  double offsetY = na_GetUIElementYOffset(child);
   
  [nativeSpacePtr addSubview:childView];
  NSRect frame = [childView frame];
  frame.origin = NSMakePoint((CGFloat)pos.x + offsetX, (CGFloat)pos.y + offsetY);
  [childView setFrame: frame];
  
  na_AddSpaceChild(space, child);
}



NA_DEF void naRemoveSpaceChild(NASpace* _Nonnull space, void* _Nonnull child) {
  NAListIterator iter = naMakeListModifier(&space->childs);
  NABool found = naLocateListData(&iter, child);
  naClearListIterator(&iter);
  if(found) {
    na_RemoveSpaceChild(space, child);
    [(NA_COCOA_BRIDGE NSView*)(naGetUIElementNativePtr(child)) removeFromSuperview];
  }else{
    #if NA_DEBUG
      naError("Child UI element not found in given space.");
    #endif
  }
}



NA_DEF void naRemoveAllSpaceChilds(NASpace* _Nonnull space) {
  while(!naIsListEmpty(&space->childs)) {
    void* child = naGetListFirstMutable(&space->childs);
    na_RemoveSpaceChild(space, child);
    [(NA_COCOA_BRIDGE NSView*)(naGetUIElementNativePtr(child)) removeFromSuperview];
  }
}



NA_DEF void naShiftSpaceChilds(NASpace* _Nonnull space, NAPos shift) {
  NAListIterator childIt = naMakeListMutator(&space->childs);
  while(naIterateList(&childIt)) {
    void* child = naGetListCurMutable(&childIt);
    NSView* childView = na_getNSViewOfChild(child);  
    NSRect frame = [childView frame];
    frame.origin = NSMakePoint(frame.origin.x + shift.x, frame.origin.y + shift.y);
    [childView setFrame: frame];
  }
  naClearListIterator(&childIt);
}



NA_DEF void naSetSpaceBackgroundColor(NASpace* _Nonnull space, const NAColor* _Nullable color) {
  na_SetSpaceBackgroundColor(space, color);
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  [nativePtr setNeedsDisplay:YES];

  NAListIterator iter = naMakeListModifier(&space->childs);
  while(naIterateList(&iter)) {
    NA_UIElement* elem = naGetListCurMutable(&iter);
    if(elem->elementType == NA_UI_SPACE) {
      naRefreshUIElement(elem, 0);
    }
  }
  naClearListIterator(&iter);
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* _Nonnull space, NABool alternate) {
  space->alternateBackground = alternate;
  naRefreshUIElement(space, 0.);
}



NA_HDEF NARect na_GetSpaceRect(const NA_UIElement* _Nullable space) {
  naDefineCocoaObjectConst(NACocoaNativeSpace, nativePtr, space);
  return naMakeRectWithNSRect([nativePtr frame]);
}

NA_HDEF void na_SetSpaceRect(NA_UIElement* _Nullable space, NARect rect) {
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  [nativePtr setFrame:naMakeNSRectWithRect(rect)];
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
