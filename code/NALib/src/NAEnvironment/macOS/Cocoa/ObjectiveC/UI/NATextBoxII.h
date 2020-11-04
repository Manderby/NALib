
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NACocoaTextBox NACocoaTextBox;
struct NACocoaTextBox{
  NATextBox   textBox;
};

NA_HAPI void na_DestructCocoaTextBox(NACocoaTextBox* cocoaTextBox);
NA_RUNTIME_TYPE(NACocoaTextBox, na_DestructCocoaTextBox, NA_FALSE);

@interface NACocoaNativeTextBox : NSTextView <NACocoaNativeEncapsulatedElement>{
  NACocoaTextBox* cocoaTextBox;
  NSScrollView* scrollView;
}
- (NSView*) getEncapsulatingView;
@end



@implementation NACocoaNativeTextBox

- (id) initWithTextBox:(NACocoaTextBox*)newCocoaTextBox frame:(NSRect)frame{
  NSRect clipRect;
  NSClipView* clipView;
  NSRect documentrect = NSMakeRect(0, 0, frame.size.width, frame.size.height);
  self = [super initWithFrame:documentrect];

  scrollView = [[NSScrollView alloc] initWithFrame:frame];
  [scrollView setHasHorizontalScroller:NO];
  [scrollView setHasVerticalScroller:YES];
  [scrollView setAutohidesScrollers:YES];

  clipRect = NSMakeRect(0, 0, frame.size.width, frame.size.height);
  clipView = [[NSClipView alloc] initWithFrame:clipRect];
  [scrollView setContentView:clipView];
  [scrollView setDocumentView:self];

  if([scrollView respondsToSelector:@selector(setAutomaticallyAdjustsContentInsets:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_10(
      [scrollView setAutomaticallyAdjustsContentInsets:YES];
    )
  }
  if([[scrollView contentView] respondsToSelector:@selector(setAutomaticallyAdjustsContentInsets:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_10(
      [[scrollView contentView] setAutomaticallyAdjustsContentInsets:YES];
    )
  }

  cocoaTextBox = newCocoaTextBox;
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

- (NSView*) getEncapsulatingView{
  return scrollView;
}

@end



NA_DEF NATextBox* naNewTextBox(NASize size){
  NACocoaTextBox* cocoaTextBox = naNew(NACocoaTextBox);
  
  NACocoaNativeTextBox* nativePtr = [[NACocoaNativeTextBox alloc]
    initWithTextBox:cocoaTextBox
    frame:naMakeNSRectWithSize(size)];
  na_InitTextBox((NATextBox*)cocoaTextBox, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  return (NATextBox*)cocoaTextBox;
}



NA_DEF void na_DestructCocoaTextBox(NACocoaTextBox* cocoaTextBox){
  na_ClearTextBox((NATextBox*)cocoaTextBox);
}



NA_DEF void naSetTextBoxText(NATextBox* textBox, const NAUTF8Char* text){
  naDefineCocoaObject(NACocoaNativeTextBox, nativePtr, textBox);
  [nativePtr setText:text];
}



NA_DEF void naSetTextBoxTextAlignment(NATextBox* textBox, NATextAlignment alignment){
  naDefineCocoaObject(NACocoaNativeTextBox, nativePtr, textBox);
  [nativePtr setTextAlignment:alignment];
}



NA_DEF void naSetTextBoxFontKind(NATextBox* textBox, NAFontKind kind){
  naDefineCocoaObject(NACocoaNativeTextBox, nativePtr, textBox);
  [nativePtr setFontKind:kind];
}



NA_DEF void naSetTextBoxEditable(NATextBox* textBox, NABool editable){
  naDefineCocoaObject(NACocoaNativeTextBox, nativePtr, textBox);
  [nativePtr setReadOnly:!editable];
}



NA_HDEF NARect na_GetTextBoxAbsoluteInnerRect(NA_UIElement* textBox){
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
