
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NANativeSpace
- (id) initWithCocoaSpace:(NACocoaSpace*)newcocoaspace frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  trackingarea = [[NSTrackingArea alloc] initWithRect:[self bounds]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow
      owner:self userInfo:nil];
  [self addTrackingArea:trackingarea];
  [self setWantsLayer:YES];

  cocoaspace = newcocoaspace;
  return self;
}
- (void)drawRect:(NSRect)dirtyRect{
  [super drawRect:dirtyRect];
  if(cocoaspace->corespace.alternatebackground){
    [[[NSColor controlTextColor] colorWithAlphaComponent:.075] setFill];
    NSRectFill(dirtyRect);
  }
}
- (void)mouseMoved:(NSEvent*)event{
  NA_UNUSED(event);
  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, NA_UI_COMMAND_MOUSE_MOVED, NA_NULL);
}
- (void)mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, NA_UI_COMMAND_MOUSE_ENTERED, NA_NULL);
}
- (void)mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, NA_UI_COMMAND_MOUSE_EXITED, NA_NULL);
}
//- (void)keyDown:(NSEvent*)event{
//  NAUIKeyCode keyCode = [event keyCode];
//  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, NA_UI_COMMAND_KEYDOWN, &keyCode);
//}
//- (void)flagsChanged:(NSEvent*)event{
//  NAUIKeyCode keyCode = [event keyCode];
//  NABool shift   = ([event modifierFlags] & NAEventModifierFlagShift)    ?NA_TRUE:NA_FALSE;
//  NABool alt     = ([event modifierFlags] & NAEventModifierFlagOption)   ?NA_TRUE:NA_FALSE;
//  NABool control = ([event modifierFlags] & NAEventModifierFlagControl)  ?NA_TRUE:NA_FALSE;
//  NABool command = ([event modifierFlags] & NAEventModifierFlagCommand)  ?NA_TRUE:NA_FALSE;

//  keyCode = NA_KEYCODE_SHIFT;
//  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, (shift?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//  keyCode = NA_KEYCODE_OPTION;
//  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, (alt?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//  keyCode = NA_KEYCODE_CONTROL;
//  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, (control?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//  keyCode = NA_KEYCODE_LEFT_COMMAND;
//  naDispatchUIElementCommand((NACoreUIElement*)cocoaspace, (command?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//}
@end



NA_DEF NASpace* naNewSpace(NARect rect){
  NACocoaSpace* cocoaspace = naAlloc(NACocoaSpace);
  cocoaspace->corespace.alternatebackground = NA_FALSE;

  NSRect contentRect = naMakeNSRectWithRect(rect);
  NANativeSpace* nativeSpace = [[NANativeSpace alloc] initWithCocoaSpace:cocoaspace frame:contentRect];  
  naRegisterCoreUIElement(&(cocoaspace->corespace.uielement), NA_UI_SPACE, (void*)NA_COCOA_RETAIN(nativeSpace));
  
  return (NASpace*)cocoaspace;
}



NA_DEF void naDestructSpace(NASpace* space){
  NACocoaSpace* cocoaspace = (NACocoaSpace*)space;
  NA_COCOA_RELEASE(naUnregisterCoreUIElement(&(cocoaspace->corespace.uielement)));
}



NA_DEF void naAddSpaceChild(NASpace* space, NAUIElement* child){
  NANativeSpace* nativespace = (NA_COCOA_BRIDGE NANativeSpace*)(naGetUIElementNativeID(space));
  NANativeRadio* nativeradio;
  NANativeTextBox* nativetextbox;
  switch(naGetUIElementType(child)){
  case NA_UI_RADIO:
    nativeradio = (NA_COCOA_BRIDGE NANativeRadio*)(naGetUIElementNativeID(child));
    [nativespace addSubview:[nativeradio getContainingView]];
    break;
  case NA_UI_TEXTBOX:
    nativetextbox = (NA_COCOA_BRIDGE NANativeTextBox*)(naGetUIElementNativeID(child));
    [nativespace addSubview:[nativetextbox getContainingView]];
    break;
  default:
    [nativespace addSubview:(NA_COCOA_BRIDGE NSView*)naGetUIElementNativeID(child)];
    break;
  }
  naSetUIElementParent(child, space);
}



NA_HDEF NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space){
  NARect rect;
  NSRect contentrect;
  NARect windowrect;
  // Warning: does not work when frame unequal bounds.
  contentrect = [(NA_COCOA_BRIDGE NSView*)(naGetUIElementNativeID((NAUIElement*)space)) frame];
  windowrect = naGetWindowAbsoluteInnerRect((NACoreUIElement*)naGetUIElementWindow((NAUIElement*)space));
  rect.pos.x = windowrect.pos.x + contentrect.origin.x;
  rect.pos.y = windowrect.pos.y + contentrect.origin.y;
  rect.size.width = contentrect.size.width;
  rect.size.height = contentrect.size.height;
  return rect;
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate){
  NACocoaSpace* cocoaspace = (NACocoaSpace*)space;
  cocoaspace->corespace.alternatebackground = alternate;
  NANativeSpace* nativespace = (NA_COCOA_BRIDGE NANativeSpace*)(naGetUIElementNativeID(space));
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
