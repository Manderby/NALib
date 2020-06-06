
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaTextBox
- (id) initWithCoreTextBox:(NA_TextBox*)newcoreTextBox frame:(NSRect)frame{
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

  if([scrollview respondsToSelector:@selector(setAutomaticallyAdjustsContentInsets:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_10(
      [scrollview setAutomaticallyAdjustsContentInsets:YES];
    )
  }
  if([[scrollview contentView] respondsToSelector:@selector(setAutomaticallyAdjustsContentInsets:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_10(
      [[scrollview contentView] setAutomaticallyAdjustsContentInsets:YES];
    )
  }

  coreTextBox = newcoreTextBox;
  return self;
}
- (void) setText:(const NAUTF8Char*)text{
  [self setString:[NSString stringWithUTF8String:text]];
}
- (void) setTextAlignment:(NATextAlignment) alignment{
  [self setAlignment:getNSTextAlignmentWithAlignment(alignment)];
}
- (void) setFontKind:(NAFontKind)kind{
  [self setFont:NA_COCOA_PTR_C_TO_OBJC(na_GetFontWithKind(kind))];
}
- (void) setReadOnly:(NABool)readonly{
  [self setEditable:!readonly];
}
- (NSView*) getContainingView{
  return scrollview;
}
@end



NA_DEF NATextBox* naNewTextBox(NASize size){
  NA_TextBox* coreTextBox = naAlloc(NA_TextBox);
  
  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaTextBox* cocoaTextBox = [[NACocoaTextBox alloc] initWithCoreTextBox:coreTextBox frame:frameRect];
  na_InitCoreTextBox(coreTextBox, NA_COCOA_PTR_OBJC_TO_C(cocoaTextBox));
  
  return (NATextBox*)coreTextBox;
}



NA_DEF void naDestructTextBox(NATextBox* textBox){
  NA_TextBox* coreTextBox = (NA_TextBox*)textBox;
  na_ClearCoreTextBox(coreTextBox);
}



NA_DEF void naSetTextBoxText(NATextBox* textBox, const NAUTF8Char* text){
  naDefineCocoaObject(NACocoaTextBox, cocoaTextBox, textBox);
  [cocoaTextBox setText:text];
}



NA_DEF void naSetTextBoxTextAlignment(NATextBox* textBox, NATextAlignment alignment){
  naDefineCocoaObject(NACocoaTextBox, cocoaTextBox, textBox);
  [cocoaTextBox setTextAlignment:alignment];
}



NA_DEF void naSetTextBoxFontKind(NATextBox* textBox, NAFontKind kind){
  naDefineCocoaObject(NACocoaTextBox, cocoaTextBox, textBox);
  [cocoaTextBox setFontKind:kind];
}



NA_DEF void naSetTextBoxEditable(NATextBox* textBox, NABool editable){
  naDefineCocoaObject(NACocoaTextBox, cocoaTextBox, textBox);
  [cocoaTextBox setReadOnly:!editable];
}



NA_HHDEF NARect na_GetTextBoxAbsoluteInnerRect(NA_UIElement* textBox){
  NA_UNUSED(textBox);
  return naMakeRectS(20, 40, 100, 50);
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
