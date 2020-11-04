
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaNativeSpace
- (id) initWithSpace:(NASpace*)newSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  // todo: make this dependent on whether tracking is needed or not.
  trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow
      owner:self userInfo:nil];
  [self addTrackingArea:trackingArea];
  [self setWantsLayer:YES];

  space = newSpace;
  return self;
}
- (void)drawRect:(NSRect)dirtyRect{
  [super drawRect:dirtyRect];
  if(space->alternatebackground){
    [[[NSColor controlTextColor] colorWithAlphaComponent:(CGFloat).075] setFill];
    NSRectFill(dirtyRect);
  }
}
- (void)mouseMoved:(NSEvent*)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)space, NA_UI_COMMAND_MOUSE_MOVED);
}
- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)space, NA_UI_COMMAND_MOUSE_ENTERED);
}
- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)space, NA_UI_COMMAND_MOUSE_EXITED);
}
@end



NA_DEF NASpace* naNewSpace(NASize size){
  NSRect contentRect;
  NACocoaNativeSpace* nativePtr;
  NASpace* space = naAlloc(NASpace);
  space->alternatebackground = NA_FALSE;

  contentRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  nativePtr = [[NACocoaNativeSpace alloc] initWithSpace:space frame:contentRect];  
  na_InitSpace(space, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  return space;
}



NA_DEF void na_DestructSpace(NASpace* space){
  na_ClearSpace(space);
}



NA_DEF void naSetSpaceRect(NASpace* space, NARect rect){
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  NSRect frame = naMakeNSRectWithRect(rect);
  frame.origin = NSMakePoint(0, 0);
  [nativePtr setFrame: frame];
}



NA_DEF void naAddSpaceChild(NASpace* space, void* child, NAPos pos){
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  naDefineCocoaObject(NSView, cocoaview, child);
  NACocoaNativeRadio* nativeRadioPtr;
  NACocoaNativeTextBox* nativeTextBoxPtr;
  NSView* subview;
  NSRect frame;
  
  switch(naGetUIElementType(child)){
  case NA_UI_RADIO:
    nativePtr = (NACocoaNativeRadio*)cocoaview;
    subview = [nativeRadioPtr getContainingView];
    break;
  case NA_UI_TEXTBOX:
    cocoaTextBox = (NACocoaNativeTextBox*)cocoaview;
    subview = [nativeTextBoxPtr getContainingView];
    break;
  default:
    subview = cocoaview;
    break;
  }
  [cocoaSpace addSubview:subview];
  frame = [subview frame];
  frame.origin = NSMakePoint((CGFloat)pos.x, (CGFloat)pos.y);
  [subview setFrame: frame];
  na_SetUIElementParent(child, space);
}



NA_HDEF NARect na_GetSpaceAbsoluteInnerRect(NA_UIElement* space){
  NARect rect;
  NSRect contentRect;
  NARect windowrect;
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  // Warning: does not work when frame unequal bounds.
  contentRect = [nativePtr frame];
  windowrect = na_GetWindowAbsoluteInnerRect((NA_UIElement*)naGetUIElementWindow(space));
  rect.pos.x = windowrect.pos.x + contentRect.origin.x;
  rect.pos.y = windowrect.pos.y + contentRect.origin.y;
  rect.size.width = contentRect.size.width;
  rect.size.height = contentRect.size.height;
  return rect;
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate){
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  space->alternatebackground = alternate;
  [nativePtr setNeedsDisplay:YES];
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
