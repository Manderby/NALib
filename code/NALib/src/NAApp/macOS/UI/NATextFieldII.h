
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaNativeTextField

- (id) initWithTextField:(NACocoaTextField*)newCocoaTextField frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  [self setSelectable:YES];
  [self setEditable:YES];
//  [self setBordered:YES];
  if([self respondsToSelector:@selector(setLineBreakMode:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_10(
      [self setLineBreakMode:NSLineBreakByTruncatingHead];
    )
  }else{
    [[self cell] setLineBreakMode:NSLineBreakByTruncatingHead];
  }
  [self setFont:[NSFont labelFontOfSize:[NSFont systemFontSize]]];
  [self setDelegate:self];

  cocoaTextField = newCocoaTextField;
  return self;
}

- (void) controlTextDidEndEditing:(NSNotification *)notification{
  NA_UNUSED(notification);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaTextField, NA_UI_COMMAND_EDIT_FINISHED);
}

- (void)controlTextDidChange:(NSNotification *)notification{
  NA_UNUSED(notification);
  na_DispatchUIElementCommand((NA_UIElement*)cocoaTextField, NA_UI_COMMAND_EDITED);
}

- (void) setText:(const NAUTF8Char*)text{
  [self setStringValue:[NSString stringWithUTF8String:text]];
}

- (NAString*) newStringWithText{
  return naNewStringWithFormat([[self stringValue] UTF8String]);
}

//- (void) setTextFieldEnabled:(NABool)enabled{
////  [self setAlphaValue:enabled ? 1. : .35];
//}

- (void) setTextAlignment:(NATextAlignment) alignment{
  [self setAlignment:getNSTextAlignmentWithAlignment(alignment)];
}

- (void) setFontKind:(NAFontKind)kind size:(NAFontSize)size{
   [self setFont:NA_COCOA_PTR_C_TO_OBJC(na_GetFontWithKindAndSize(kind, size))];
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}

@end



NA_DEF NATextField* naNewTextField(double width){
  NACocoaTextField* cocoaTextField = naNew(NACocoaTextField);
  
  NACocoaNativeTextField* nativePtr = [[NACocoaNativeTextField alloc]
    initWithTextField:cocoaTextField
    frame:naMakeNSRectWithSize(naMakeSize(width, 21))];
  na_InitTextField((NATextField*)cocoaTextField, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  return (NATextField*)cocoaTextField;
}



NA_DEF void na_DestructCocoaTextField(NACocoaTextField* cocoaTextField){
  na_ClearTextField((NATextField*)cocoaTextField);
}



NA_DEF void naSetTextFieldEnabled(NATextField* textField, NABool enabled){
  naDefineCocoaObject(NACocoaNativeTextField, nativePtr, textField);
  [nativePtr setEnabled:(BOOL)enabled];
}



NA_DEF void naSetTextFieldText(NATextField* textField, const NAUTF8Char* text){
  naDefineCocoaObject(NACocoaNativeTextField, nativePtr, textField);
  [nativePtr setText:text];
}



NA_DEF NAString* naNewStringWithTextFieldText(const NATextField* textField){
  naDefineCocoaObjectConst(NACocoaNativeTextField, nativePtr, textField);
  return [nativePtr newStringWithText];
}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textField, NATextAlignment alignment){
  naDefineCocoaObject(NACocoaNativeTextField, nativePtr, textField);
  [nativePtr setTextAlignment:alignment];
}



NA_DEF void naSetTextFieldFont(NATextField* textField, NAFont* font){
  naDefineCocoaObject(NACocoaNativeTextField, nativePtr, textField);
  [nativePtr setFontKind:kind size:size];
}



NA_HDEF NARect na_GetTextFieldAbsoluteInnerRect(const NA_UIElement* textField){
  naDefineCocoaObjectConst(NACocoaNativeTextField, nativePtr, textField);
  NARect parentRect = naGetUIElementRect(naGetUIElementParentConst(textField), naGetApplication(), NA_FALSE);
  NARect relRect = [nativePtr getInnerRect];
  return naMakeRect(
    naMakePos(parentRect.pos.x + relRect.pos.x, parentRect.pos.y + relRect.pos.y),
    relRect.size);
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
