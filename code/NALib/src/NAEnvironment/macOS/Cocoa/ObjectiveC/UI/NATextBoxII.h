
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NANativeTextBox
- (id) initWithCoreTextBox:(NACoreTextBox*)newcoretextbox frame:(NSRect)frame{
  NSRect clipRect;
  NSClipView* clipView;
  NSRect documentrect = NSMakeRect(0, 0, frame.size.width, frame.size.height);
  self = [super initWithFrame:documentrect];

  scrollview = [[NSScrollView alloc] initWithFrame:frame];
  [scrollview setHasHorizontalScroller:NO];
  [scrollview setHasVerticalScroller:YES];
  [scrollview setAutohidesScrollers:YES];

  clipRect = NSMakeRect(0, 0, frame.size.width, frame.size.height);
  clipView = [[NSClipView alloc] initWithFrame:clipRect];
  [scrollview setContentView:clipView];
  [scrollview setDocumentView:self];

  #if NA_MACOS_AVAILABILITY_10_10
    [scrollview setAutomaticallyAdjustsContentInsets:YES];
    [[scrollview contentView] setAutomaticallyAdjustsContentInsets:YES];
  #endif

  coretextbox = newcoretextbox;
  return self;
}
- (void) setText:(const NAUTF8Char*)text{
  [self setString:[NSString stringWithUTF8String:text]];
}
- (void) setTextAlignment:(NATextAlignment) alignment{
  [self setAlignment:getNSTextAlignmentWithAlignment(alignment)];
}
- (void) setFontKind:(NAFontKind)kind{
  [self setFont:NA_COCOA_PTR_C_TO_OBJC(getFontWithKind(kind))];
}
- (void) setReadOnly:(NABool)readonly{
  [self setEditable:!readonly];
}
- (NSView*) getContainingView{
  return scrollview;
}
@end



NA_DEF NATextBox* naNewTextBox(NASize size){
  NACoreTextBox* coretextbox = naAlloc(NACoreTextBox);
  
  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NANativeTextBox* nativeTextBox = [[NANativeTextBox alloc] initWithCoreTextBox:coretextbox frame:frameRect];
  naInitCoreTextBox(coretextbox, NA_COCOA_PTR_OBJC_TO_C(nativeTextBox));
  
  return (NATextBox*)coretextbox;
}



NA_DEF void naDestructTextBox(NATextBox* textbox){
  NACoreTextBox* coretextbox = (NACoreTextBox*)textbox;
  naClearCoreTextBox(coretextbox);
}



NA_DEF void naSetTextBoxText(NATextBox* textbox, const NAUTF8Char* text){
  naDefineNativeCocoaObject(NANativeTextBox, nativetextbox, textbox);
  [nativetextbox setText:text];
}



NA_DEF void naSetTextBoxTextAlignment(NATextBox* textbox, NATextAlignment alignment){
  naDefineNativeCocoaObject(NANativeTextBox, nativetextbox, textbox);
  [nativetextbox setTextAlignment:alignment];
}



NA_DEF void naSetTextBoxFontKind(NATextBox* textbox, NAFontKind kind){
  naDefineNativeCocoaObject(NANativeTextBox, nativetextbox, textbox);
  [nativetextbox setFontKind:kind];
}



NA_DEF void naSetTextBoxEditable(NATextBox* textbox, NABool editable){
  naDefineNativeCocoaObject(NANativeTextBox, nativetextbox, textbox);
  [nativetextbox setReadOnly:!editable];
}



NA_HDEF NARect naGetTextBoxAbsoluteInnerRect(NACoreUIElement* textbox){
  NA_UNUSED(textbox);
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
