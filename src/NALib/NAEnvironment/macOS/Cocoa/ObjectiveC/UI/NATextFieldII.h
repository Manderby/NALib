
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NANativeTextField
- (id) initWithCoreTextField:(NACoreTextField*)newcoretextfield frame:(NSRect)frame{
  self = [super initWithFrame:frame];
//  [self setCell:[[MDVerticallyCenteredTextFieldCell alloc] initTextCell:@"Wurst"]];
  [self setSelectable:YES];
  [self setEditable:YES];
  [self setBordered:YES];
  [self setLineBreakMode:NSLineBreakByTruncatingHead];
  [self setTarget:self];
  [self setAction:@selector(onEdited:)];
  [self setFont:[NSFont labelFontOfSize:[NSFont systemFontSize]]];
  [self setDelegate:self];
  coretextfield = newcoretextfield;
  return self;
}
- (void) onEdited:(id)sender{
  NA_UNUSED(sender);
  naDispatchUIElementCommand((NACoreUIElement*)coretextfield, NA_UI_COMMAND_EDITED);
}
- (void)controlTextDidChange:(NSNotification *)notification{
  NA_UNUSED(notification);
  naDispatchUIElementCommand((NACoreUIElement*)coretextfield, NA_UI_COMMAND_EDITED);
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
   [self setFont:(NSFont*)getFontWithKind(kind)];
}
@end



NA_DEF NATextField* naNewTextField(NARect rect){
  NACoreTextField* coretextfield = naAlloc(NACoreTextField);
  
  NANativeTextField* nativeTextField = [[NANativeTextField alloc] initWithCoreTextField:coretextfield frame:naMakeNSRectWithRect(rect)];
  naInitCoreTextField(coretextfield, NA_COCOA_TAKE_OWNERSHIP(nativeTextField));
  
  return (NATextField*)coretextfield;
}



NA_DEF void naDestructTextField(NATextField* textfield){
  NACoreTextField* coretextfield = (NACoreTextField*)textfield;
  naClearCoreTextField(coretextfield);
}



NA_DEF void naSetTextFieldText(NATextField* textfield, const NAUTF8Char* text){
  naDefineNativeCocoaObject(NANativeTextField, nativetextfield, textfield);
  [nativetextfield setText:text];
}



NA_DEF NAString* naNewStringWithTextFieldText(NATextField* textfield){
  naDefineNativeCocoaObject(NANativeTextField, nativetextfield, textfield);
  return [nativetextfield newStringWithText];
}



//NA_DEF void naSetTextFieldEnabled(NATextField* textfield, NABool enabled){
//  [((NANativeTextField*)naGetUIElementNativeID(textfield)) setTextFieldEnabled:enabled];
//}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textfield, NATextAlignment alignment){
  naDefineNativeCocoaObject(NANativeTextField, nativetextfield, textfield);
  [nativetextfield setTextAlignment:alignment];
}



NA_DEF void naSetTextFieldFontKind(NATextField* textfield, NAFontKind kind){
  naDefineNativeCocoaObject(NANativeTextField, nativetextfield, textfield);
  [nativetextfield setFontKind:kind];
}



NA_HDEF NARect naGetTextFieldAbsoluteInnerRect(NACoreUIElement* textfield){
  NA_UNUSED(textfield);
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
