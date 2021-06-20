
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NACocoaSpace NACocoaSpace;
struct NACocoaSpace{
  NASpace space;
};

NA_HAPI void na_DestructCocoaSpace(NACocoaSpace* cocoaSpace);
NA_RUNTIME_TYPE(NACocoaSpace, na_DestructCocoaSpace, NA_FALSE);

@interface NACocoaNativeSpace : NSView{
  NACocoaSpace*   cocaSpace;
  NSTrackingArea* trackingArea;
}
@end



@implementation NACocoaNativeSpace

- (id) initWithSpace:(NACocoaSpace*)newCocoaSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  // todo: make this dependent on whether tracking is needed or not.
  trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
      options:(NSTrackingAreaOptions)(NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow)
      owner:self userInfo:nil];
  [self addTrackingArea:trackingArea];
  [self setWantsLayer:YES];

  cocaSpace = newCocoaSpace;
  return self;
}

- (void)dealloc{
  NA_COCOA_RELEASE(trackingArea);
  NA_COCOA_SUPER_DEALLOC();
}

- (void)drawRect:(NSRect)dirtyRect{
  [super drawRect:dirtyRect];
  if(cocaSpace->space.alternatebackground){
    [[[NSColor controlTextColor] colorWithAlphaComponent:(CGFloat).075] setFill];
    NSRectFill(dirtyRect);
  }
}

- (void)mouseMoved:(NSEvent*)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)cocaSpace, NA_UI_COMMAND_MOUSE_MOVED);
}

- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)cocaSpace, NA_UI_COMMAND_MOUSE_ENTERED);
}

- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  na_DispatchUIElementCommand((NA_UIElement*)cocaSpace, NA_UI_COMMAND_MOUSE_EXITED);
}

@end



NA_DEF NASpace* naNewSpace(NASize size){
  NACocoaSpace* cocoaSpace = naNew(NACocoaSpace);

  NACocoaNativeSpace* nativePtr = [[NACocoaNativeSpace alloc]
    initWithSpace:cocoaSpace
    frame:naMakeNSRectWithSize(size)];  
  na_InitSpace((NASpace*)cocoaSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  cocoaSpace->space.alternatebackground = NA_FALSE;

  return (NASpace*)cocoaSpace;
}



NA_DEF void na_DestructCocoaSpace(NACocoaSpace* cocoaSpace){
  na_ClearSpace((NASpace*)cocoaSpace);
}



NA_DEF void naSetSpaceRect(NASpace* space, NARect rect){
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  NSRect frame = naMakeNSRectWithRect(rect);
  frame.origin = NSMakePoint(0, 0);
  [nativePtr setFrame: frame];
}



@class NACocoaNativeRadio;
@class NACocoaNativeTextBox;

NA_DEF void naAddSpaceChild(NASpace* space, void* child, NAPos pos){
  naDefineCocoaObject(NACocoaNativeSpace, nativeSpacePtr, space);
  naDefineCocoaObject(NSView<NACocoaNativeEncapsulatedElement>, cocoaView, child);

  NSView* subview;  
  switch(naGetUIElementType(child)){
  case NA_UI_RADIO:
    subview = [cocoaView getEncapsulatingView];
    break;
  case NA_UI_TEXTBOX:
    subview = [cocoaView getEncapsulatingView];
    break;
  default:
    subview = cocoaView;
    break;
  }
  
  [nativeSpacePtr addSubview:subview];
  NSRect frame = [subview frame];
  frame.origin = NSMakePoint((CGFloat)pos.x, (CGFloat)pos.y);
  [subview setFrame: frame];
  
  na_AddSpaceChild(space, child);
}



NA_HDEF NARect na_GetSpaceAbsoluteInnerRect(NA_UIElement* space){
  naDefineCocoaObject(NACocoaNativeSpace, nativePtr, space);
  // Warning: does not work when frame unequal bounds.
  NSRect contentRect = [nativePtr frame];
  
  NARect windowRect;
  NAWindow* window = naGetUIElementWindow(space);
  if(window){
    windowRect = na_GetNativeWindowAbsoluteInnerRect(naGetUIElementNativePtr(window));
  }else{
    #ifndef NDEBUG
      naError("Given element has no NAWindow as parent. Using native window parent.");
    #endif
    windowRect = naMakeRectWithNSRect([[nativePtr window] frame]);
  }
  
  NARect rect = naMakeRectS(
    windowRect.pos.x + contentRect.origin.x,
    windowRect.pos.y + contentRect.origin.y,
    contentRect.size.width,
    contentRect.size.height);
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
