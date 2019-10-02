
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPISpace NAWINAPISpace;
struct NAWINAPISpace {
  NACoreSpace corespace;
};



NABool naSpaceWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NABool hasbeenhandeled = NA_FALSE;

  switch(message){
  default:
    //printf("Uncaught Space message\n");
    break;
  }
  
  return hasbeenhandeled;
}



//@implementation NANativeSpace
//- (id) initWithCoreSpace:(NACoreSpace*)newcorespace frame:(NSRect)frame{
//  self = [super initWithFrame:frame];
//
//  trackingarea = [[NSTrackingArea alloc] initWithRect:[self bounds]
//      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow
//      owner:self userInfo:nil];
//  [self addTrackingArea:trackingarea];
//  [self setWantsLayer:YES];
//
//  corespace = newcorespace;
//  return self;
//}
//- (void)drawRect:(NSRect)dirtyRect{
//  [super drawRect:dirtyRect];
//  if(corespace->alternatebackground){
//    [[[NSColor controlTextColor] colorWithAlphaComponent:.075] setFill];
//    NSRectFill(dirtyRect);
//  }
//}
//- (void)mouseMoved:(NSEvent*)event{
//  NA_UNUSED(event);
//  naDispatchUIElementCommand((NACoreUIElement*)corespace, NA_UI_COMMAND_MOUSE_MOVED);
//}
//- (void)mouseEntered:(NSEvent*)event{
//  NA_UNUSED(event);
//  naDispatchUIElementCommand((NACoreUIElement*)corespace, NA_UI_COMMAND_MOUSE_ENTERED);
//}
//- (void)mouseExited:(NSEvent*)event{
//  NA_UNUSED(event);
//  naDispatchUIElementCommand((NACoreUIElement*)corespace, NA_UI_COMMAND_MOUSE_EXITED);
//}
//@end



NA_DEF NASpace* naNewSpace(NARect rect){
  HWND hWnd;
  DWORD exStyle;
  DWORD style;

  NAWINAPISpace* winapispace = naAlloc(NAWINAPISpace);

  exStyle = 0;
  style = WS_CHILD | WS_VISIBLE;

	hWnd = CreateWindow(
		TEXT("NASpace"), "Space", style,
		0, 0, (int)rect.size.width, (int)rect.size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  DWORD lasterror = GetLastError();

  naInitCoreSpace(&(winapispace->corespace), hWnd);
  winapispace->corespace.alternatebackground = NA_FALSE;

  return (NASpace*)winapispace;
}



NA_DEF void naDestructSpace(NASpace* space){
//  NACoreSpace* corespace = (NACoreSpace*)space;
//  naClearCoreSpace(corespace);
}



NA_DEF void naAddSpaceChild(NASpace* space, NAUIElement* child){
//  naDefineNativeCocoaObject(NANativeSpace, nativespace, space);
//  naDefineNativeCocoaObject(NSView, nativeview, child);
//  NANativeRadio* nativeradio;
//  NANativeTextBox* nativetextbox;
//  switch(naGetUIElementType(child)){
//  case NA_UI_RADIO:
//    nativeradio = (NANativeRadio*)nativeview;
//    [nativespace addSubview:[nativeradio getContainingView]];
//    break;
//  case NA_UI_TEXTBOX:
//    nativetextbox = (NANativeTextBox*)nativeview;
//    [nativespace addSubview:[nativetextbox getContainingView]];
//    break;
//  default:
//    [nativespace addSubview:nativeview];
//    break;
//  }
  naSetUIElementParent(child, space);
}



NA_HDEF NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space){
  NARect rect;
  NARect screenrect;
  RECT contentrect;
  POINT testpoint = {0, 0};
  NACoreUIElement* corespace;

  corespace = (NACoreUIElement*)space;

  GetClientRect(corespace->nativeID, &contentrect);
  ClientToScreen(corespace->nativeID, &testpoint);
  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = screenrect.size.height - ((double)testpoint.y + (contentrect.bottom - contentrect.top));
  rect.size.width = (double)contentrect.right - (double)contentrect.left;
  rect.size.height = (double)contentrect.bottom - (double)contentrect.top;
  return rect;
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate){
//  NACoreSpace* corespace = (NACoreSpace*)space;
//  corespace->alternatebackground = alternate;
//  naDefineNativeCocoaObject(NANativeSpace, nativespace, space);
//  [nativespace setNeedsDisplay:YES];
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
