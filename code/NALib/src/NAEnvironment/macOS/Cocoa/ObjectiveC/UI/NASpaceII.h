
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaSpace
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
  NACocoaSpace* cocoaSpace;
  NASpace* space = naAlloc(NASpace);
  space->alternatebackground = NA_FALSE;

  contentRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  cocoaSpace = [[NACocoaSpace alloc] initWithSpace:space frame:contentRect];  
  na_InitSpace(space, NA_COCOA_PTR_OBJC_TO_C(cocoaSpace));

  return space;
}



NA_DEF void na_DestructSpace(NASpace* space){
  na_ClearSpace(space);
}



NA_DEF void naSetSpaceRect(NASpace* space, NARect rect){
  naDefineCocoaObject(NACocoaSpace, cocoaSpace, space);
  NSRect frame = naMakeNSRectWithRect(rect);
  frame.origin = NSMakePoint(0, 0);
  [cocoaSpace setFrame: frame];
}



NA_DEF void naAddSpaceChild(NASpace* space, void* child, NAPos pos){
  naDefineCocoaObject(NACocoaSpace, cocoaSpace, space);
  naDefineCocoaObject(NSView, cocoaview, child);
  NACocoaRadio* cocoaRadio;
  NACocoaTextBox* cocoaTextBox;
  NSView* subview;
  NSRect frame;
  
  switch(naGetUIElementType(child)){
  case NA_UI_RADIO:
    cocoaRadio = (NACocoaRadio*)cocoaview;
    subview = [cocoaRadio getContainingView];
    break;
  case NA_UI_TEXTBOX:
    cocoaTextBox = (NACocoaTextBox*)cocoaview;
    subview = [cocoaTextBox getContainingView];
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
  NSRect contentrect;
  NARect windowrect;
  naDefineCocoaObject(NACocoaSpace, cocoaSpace, space);
  // Warning: does not work when frame unequal bounds.
  contentrect = [cocoaSpace frame];
  windowrect = na_GetWindowAbsoluteInnerRect((NA_UIElement*)naGetUIElementWindow(space));
  rect.pos.x = windowrect.pos.x + contentrect.origin.x;
  rect.pos.y = windowrect.pos.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate){
  naDefineCocoaObject(NACocoaSpace, cocoaSpace, space);
  space->alternatebackground = alternate;
  [cocoaSpace setNeedsDisplay:YES];
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
