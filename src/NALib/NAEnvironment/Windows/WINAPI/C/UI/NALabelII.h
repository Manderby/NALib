
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPILabel NAWINAPILabel;
struct NAWINAPILabel {
  NACoreLabel corelabel;
  NABool enabled;
};


NAWINAPICallbackInfo naLabelWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SETFONT:
  case WM_STYLECHANGING:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_STYLECHANGED:
  case WM_WINDOWPOSCHANGED:
  case WM_SETTEXT:
  case WM_PAINT:
  case WM_NCPAINT:
    break;

  case WM_ERASEBKGND:
    info.hasbeenhandeled = NA_TRUE;
    info.result = 1;
    break;

  default:
    //printf("Uncaught Label message\n");
    break;
  }
  
  return info;
}




//LRESULT CALLBACK LabelWindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(hWnd);
//  return naLabelWINAPIProc(uielement, message, wParam, lParam);
//}



//@implementation NANativeLabel
//- (id) initWithCoreLabel:(NACoreLabel*)newcorelabel frame:(NSRect)frame{
//  self = [super initWithFrame:frame];
////  [self setCell:[[MDVerticallyCenteredTextFieldCell alloc] initTextCell:@"Wurst"]];
//  [self setSelectable:YES];
//  [self setEditable:NO];
//  [self setBordered:NO];
//  [self setBackgroundColor:[NSColor colorWithCalibratedRed:0. green:0. blue:1. alpha:.1]];
//  [self setDrawsBackground:NO];
//  [self setLineBreakMode:NSLineBreakByWordWrapping];
//  [self setFont:[NSFont labelFontOfSize:[NSFont systemFontSize]]];
//  corelabel = newcorelabel;
//  return self;
//}
//- (void) setLink:(const NAUTF8Char*)url{
//  [self setAllowsEditingTextAttributes: YES];
//  [self setSelectable: YES];
//
//  NSMutableAttributedString* attrString = NA_COCOA_AUTORELEASE([[NSMutableAttributedString alloc] initWithString: [self stringValue]]);
//  NSRange range = NSMakeRange(0, [attrString length]);
//
//  NSURL* nsurl = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
//  [attrString beginEditing];
//  [attrString addAttribute:NSLinkAttributeName value:[nsurl absoluteString] range:range];
//  NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
//  [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
//  paragraphStyle.alignment = [self alignment];
//  [attrString addAttribute:NSParagraphStyleAttributeName value:paragraphStyle range:range];
//  [attrString addAttribute:NSUnderlineStyleAttributeName value:[NSNumber numberWithInt:NSUnderlineStyleSingle] range:range];
//  [attrString addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:range];
//  [attrString endEditing];
//  
//  [self setAttributedStringValue: attrString];
//}
//- (void) setLabelEnabled:(NABool)enabled{
//  [self setAlphaValue:enabled ? 1. : .35];
//}
//@end



NA_DEF NALabel* naNewLabel(const NAUTF8Char* text, NASize size){
  HWND hWnd;
  DWORD style;
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPILabel* winapilabel = naAlloc(NAWINAPILabel);

  // We need a read only edit control here, otherwise on windows, the user is not able to select text.
  style = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_MULTILINE;

  TCHAR* systemtext = naAllocSystemStringWithUTF8String(text, 0);

	hWnd = CreateWindow(
		TEXT("EDIT"), systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  naFree(systemtext);

  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldLabelWindowProc){app->oldLabelWindowProc = oldproc;}

  naInitCoreLabel(&(winapilabel->corelabel), hWnd);

  winapilabel->enabled = NA_TRUE;
  SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NALabel*)winapilabel;
}



NA_DEF void naDestructLabel(NALabel* label){
  NAWINAPILabel* winapilabel = (NAWINAPILabel*)label;
  naClearCoreLabel(&(winapilabel->corelabel));
}



NA_DEF void naSetLabelText(NALabel* label, const NAUTF8Char* text){
  TCHAR* systemtext = naAllocSystemStringWithUTF8String(text, 0);
  SendMessage(naGetUIElementNativeID(label), WM_SETTEXT, 0, (LPARAM)systemtext);
  naFree(systemtext);
}



NA_DEF void naSetLabelLink(NALabel* label, const NAUTF8Char* url){
//  naDefineNativeCocoaObject(NANativeLabel, nativelabel, label);
//  [nativelabel setLink: url];
}



NA_DEF NABool naIsLabelEnabled(NALabel* label){
  NAWINAPILabel* winapilabel = (NAWINAPILabel*)label;
  return winapilabel->enabled;
}



NA_DEF void naSetLabelEnabled(NALabel* label, NABool enabled){
  NAWINAPILabel* winapilabel = (NAWINAPILabel*)label;
  winapilabel->enabled = enabled;
  naRefreshUIElement(label, 0);
}



NA_DEF void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment){
  long style = GetWindowLongPtr(naGetUIElementNativeID(label), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativeID(label), GWL_STYLE, style);
}



NA_DEF void naSetLabelFontKind(NALabel* label, NAFontKind kind){
  SendMessage(naGetUIElementNativeID(label), WM_SETFONT, (WPARAM)getFontWithKind(kind), MAKELPARAM(TRUE, 0));
}



NA_HDEF NARect naGetLabelAbsoluteInnerRect(NACoreUIElement* label){
  NA_UNUSED(label);
  return naMakeRectS(20, 40, 100, 50);
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
