
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NANativeSpace
- (id) initWithCoreSpace:(NACoreSpace*)newcorespace frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  // todo: make this dependent on whether tracking is needed or not.
  trackingarea = [[NSTrackingArea alloc] initWithRect:[self bounds]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow
      owner:self userInfo:nil];
  [self addTrackingArea:trackingarea];
  [self setWantsLayer:YES];

  corespace = newcorespace;
  return self;
}
- (void)drawRect:(NSRect)dirtyRect{
  [super drawRect:dirtyRect];
  if(corespace->alternatebackground){
    [[[NSColor controlTextColor] colorWithAlphaComponent:.075] setFill];
    NSRectFill(dirtyRect);
  }
}
- (void)mouseMoved:(NSEvent*)event{
  NA_UNUSED(event);
  naDispatchUIElementCommand((NACoreUIElement*)corespace, NA_UI_COMMAND_MOUSE_MOVED);
}
- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  naDispatchUIElementCommand((NACoreUIElement*)corespace, NA_UI_COMMAND_MOUSE_ENTERED);
}
- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  naDispatchUIElementCommand((NACoreUIElement*)corespace, NA_UI_COMMAND_MOUSE_EXITED);
}
@end



NA_DEF NASpace* naNewSpace(NASize size){
  NACoreSpace* corespace = naAlloc(NACoreSpace);
  corespace->alternatebackground = NA_FALSE;

  NSRect contentRect = NSMakeRect(0., 0., size.width, size.height);
  NANativeSpace* nativeSpace = [[NANativeSpace alloc] initWithCoreSpace:corespace frame:contentRect];  
  naInitCoreSpace(corespace, NA_COCOA_TAKE_OWNERSHIP(nativeSpace));
  
  return (NASpace*)corespace;
}



NA_DEF void naDestructSpace(NASpace* space){
  NACoreSpace* corespace = (NACoreSpace*)space;
  naClearCoreSpace(corespace);
}



NA_DEF void naAddSpaceChild(NASpace* space, NAUIElement* child, NAPos pos){
  naDefineNativeCocoaObject(NANativeSpace, nativespace, space);
  naDefineNativeCocoaObject(NSView, nativeview, child);
  NANativeRadio* nativeradio;
  NANativeTextBox* nativetextbox;
  NSView* subview;
  
  switch(naGetUIElementType(child)){
  case NA_UI_RADIO:
    nativeradio = (NANativeRadio*)nativeview;
    subview = [nativeradio getContainingView];
    break;
  case NA_UI_TEXTBOX:
    nativetextbox = (NANativeTextBox*)nativeview;
    subview = [nativetextbox getContainingView];
    break;
  default:
    subview = nativeview;
    break;
  }
  [nativespace addSubview:subview];
  NSRect frame = [subview frame];
  frame.origin = NSMakePoint(pos.x, pos.y);
  [subview setFrame: frame];
  naSetUIElementParent(child, space);
}



NA_HDEF NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space){
  NARect rect;
  NSRect contentrect;
  NARect windowrect;
  naDefineNativeCocoaObject(NANativeSpace, nativespace, space);
  // Warning: does not work when frame unequal bounds.
  contentrect = [nativespace frame];
  windowrect = naGetWindowAbsoluteInnerRect((NACoreUIElement*)naGetUIElementWindow((NAUIElement*)space));
  rect.pos.x = windowrect.pos.x + contentrect.origin.x;
  rect.pos.y = windowrect.pos.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate){
  NACoreSpace* corespace = (NACoreSpace*)space;
  corespace->alternatebackground = alternate;
  naDefineNativeCocoaObject(NANativeSpace, nativespace, space);
  [nativespace setNeedsDisplay:YES];
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
