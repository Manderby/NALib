
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


//@implementation NANativeCheckBox
//- (id) initWithCoreCheckBox:(NACoreCheckBox*)newcorecheckbox frame:(NSRect)frame{
//  self = [super initWithFrame:frame];
//  
//  [self setButtonType:NSButtonTypeSwitch];
//  corecheckbox = newcorecheckbox;
//  [self setTarget:self];
//  [self setAction:@selector(onPressed:)];
//
//  return self;
//}
//- (void) setText:(const NAUTF8Char*)text{
//  [self setTitle:[NSString stringWithUTF8String:text]];
//}
//- (void) onPressed:(id)sender{
//  NA_UNUSED(sender);
//  naDispatchUIElementCommand((NACoreUIElement*)corecheckbox, NA_UI_COMMAND_PRESSED);
//}
//- (void) setCheckBoxState:(NABool)state{
//  [self setState:state ? NSOnState : NSOffState];
//}
//- (NABool) checkboxState{
//  return ([self state] == NSOnState) ? NA_TRUE : NA_FALSE;
//}
//@end



NA_DEF NACheckBox* naNewCheckBox(const NAUTF8Char* text, NARect rect){
//  NACoreCheckBox* corecheckbox = naAlloc(NACoreCheckBox);
//  NSRect framerect = naMakeNSRectWithRect(rect);
//  NSRect boundrect = framerect;
//  boundrect.origin.x = 0;
//  boundrect.origin.y = 0;
//
//  NANativeCheckBox* nativeCheckBox = [[NANativeCheckBox alloc] initWithCoreCheckBox:corecheckbox frame:framerect];
//  naInitCoreCheckBox(corecheckbox, NA_COCOA_TAKE_OWNERSHIP(nativeCheckBox));
//  [nativeCheckBox setText:text];
//  
//  return (NACheckBox*)corecheckbox;
  return NA_NULL;
}



NA_DEF void naDestructCheckBox(NACheckBox* checkbox){
//  NACoreCheckBox* corecheckbox = (NACoreCheckBox*)checkbox;
//  naClearCoreCheckBox(corecheckbox);
}



NA_HDEF NARect naGetCheckBoxAbsoluteInnerRect(NACoreUIElement* checkbox){
  NA_UNUSED(checkbox);
  return naMakeRectS(20, 40, 100, 50);
}



NA_HDEF void naSetCheckBoxState(NACheckBox* checkbox, NABool state){
//  naDefineNativeCocoaObject(NANativeCheckBox, nativecheckbox, checkbox);
//  [nativecheckbox setCheckBoxState:state];
}



NA_HDEF NABool naGetCheckBoxState(NACheckBox* checkbox){
//  naDefineNativeCocoaObject(NANativeCheckBox, nativecheckbox, checkbox);
//  return [nativecheckbox checkboxState];
  return NA_FALSE;
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
