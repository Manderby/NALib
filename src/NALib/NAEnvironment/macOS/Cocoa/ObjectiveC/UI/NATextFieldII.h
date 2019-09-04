
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NANativeTextField
- (id) initWithCocoaTextField:(NACocoaTextField*)newcocoatextfield frame:(NSRect)frame{
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
  cocoatextfield = newcocoatextfield;
  return self;
}
- (void) onEdited:(id)sender{
  NA_UNUSED(sender);
  naDispatchUIElementCommand((NACoreUIElement*)cocoatextfield, NA_UI_COMMAND_EDITED, NA_NULL);
}
- (void)controlTextDidChange:(NSNotification *)notification{
  NA_UNUSED(notification);
  naDispatchUIElementCommand((NACoreUIElement*)cocoatextfield, NA_UI_COMMAND_EDITED, NA_NULL);
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
   [self setFont:getNSFontWithKind(kind)];
}
@end

NA_DEF NATextField* naNewTextField(NARect rect){
  NACocoaTextField* cocoatextfield = naAlloc(NACocoaTextField);

  NANativeTextField* nativeTextField = [[NANativeTextField alloc] initWithCocoaTextField:cocoatextfield frame:naMakeNSRectWithRect(rect)];
  naRegisterCoreUIElement(&(cocoatextfield->coretextfield.uielement), NA_UI_TEXTFIELD, (void*)NA_COCOA_RETAIN(nativeTextField));
  
  return (NATextField*)cocoatextfield;
}



NA_DEF void naDestructTextField(NATextField* textfield){
  NACocoaTextField* cocoatextfield = (NACocoaTextField*)textfield;
  NA_COCOA_RELEASE(naUnregisterCoreUIElement(&(cocoatextfield->coretextfield.uielement)));
}



NA_DEF void naSetTextFieldText(NATextField* textfield, const NAUTF8Char* text){
  [((NA_COCOA_BRIDGE NANativeTextField*)naGetUIElementNativeID(textfield)) setText:text];
}



NA_DEF NAString* naNewStringWithTextFieldText(NATextField* textfield){
  return [((NA_COCOA_BRIDGE NANativeTextField*)naGetUIElementNativeID(textfield)) newStringWithText];
}



//NA_DEF void naSetTextFieldEnabled(NATextField* textfield, NABool enabled){
//  [((NA_COCOA_BRIDGE NANativeTextField*)naGetUIElementNativeID(textfield)) setTextFieldEnabled:enabled];
//}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textfield, NATextAlignment alignment){
  [((NA_COCOA_BRIDGE NANativeTextField*)naGetUIElementNativeID(textfield)) setTextAlignment:alignment];
}



NA_DEF void naSetTextFieldFontKind(NATextField* textfield, NAFontKind kind){
  [((NA_COCOA_BRIDGE NANativeTextField*)naGetUIElementNativeID(textfield)) setFontKind:kind];
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
