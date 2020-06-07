
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaTextField
- (id) initWithCoreTextField:(NA_TextField*)newcoreTextField frame:(NSRect)frame{
  self = [super initWithFrame:frame];
//  [self setCell:[[MDVerticallyCenteredTextFieldCell alloc] initTextCell:@"Wurst"]];
  [self setSelectable:YES];
  [self setEditable:YES];
  [self setBordered:YES];
  if([self respondsToSelector:@selector(setLineBreakMode:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_10(
      [self setLineBreakMode:NSLineBreakByTruncatingHead];
    )
  }else{
    [[self cell] setLineBreakMode:NSLineBreakByTruncatingHead];
  }
  [self setTarget:self];
  [self setAction:@selector(onEdited:)];
  [self setFont:[NSFont labelFontOfSize:[NSFont systemFontSize]]];
  [self setDelegate:self];
  coreTextField = newcoreTextField;
  return self;
}
- (void) onEdited:(id)sender{
  NA_UNUSED(sender);
  na_DispatchUIElementCommand((NA_UIElement*)coreTextField, NA_UI_COMMAND_EDITED);
}
- (void)controlTextDidChange:(NSNotification *)notification{
  NA_UNUSED(notification);
  na_DispatchUIElementCommand((NA_UIElement*)coreTextField, NA_UI_COMMAND_EDITED);
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
- (void) setFontKind:(NAFontKind)kind{
   [self setFont:NA_COCOA_PTR_C_TO_OBJC(na_GetFontWithKind(kind))];
}
@end



NA_DEF NATextField* naNewTextField(NASize size){
  NA_TextField* coreTextField = naAlloc(NA_TextField);
  
  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaTextField* cocoaTextField = [[NACocoaTextField alloc] initWithCoreTextField:coreTextField frame:frameRect];
  na_InitTextField(coreTextField, NA_COCOA_PTR_OBJC_TO_C(cocoaTextField));
  
  return (NATextField*)coreTextField;
}



NA_DEF void na_DestructTextField(NATextField* textField){
  NA_TextField* coreTextField = (NA_TextField*)textField;
  na_ClearTextField(coreTextField);
}



NA_DEF void naSetTextFieldText(NATextField* textField, const NAUTF8Char* text){
  naDefineCocoaObject(NACocoaTextField, cocoaTextField, textField);
  [cocoaTextField setText:text];
}



NA_DEF NAString* naNewStringWithTextFieldText(NATextField* textField){
  naDefineCocoaObject(NACocoaTextField, cocoaTextField, textField);
  return [cocoaTextField newStringWithText];
}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textField, NATextAlignment alignment){
  naDefineCocoaObject(NACocoaTextField, cocoaTextField, textField);
  [cocoaTextField setTextAlignment:alignment];
}



NA_DEF void naSetTextFieldFontKind(NATextField* textField, NAFontKind kind){
  naDefineCocoaObject(NACocoaTextField, cocoaTextField, textField);
  [cocoaTextField setFontKind:kind];
}



NA_HHDEF NARect na_GetTextFieldAbsoluteInnerRect(NA_UIElement* textField){
  NA_UNUSED(textField);
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
