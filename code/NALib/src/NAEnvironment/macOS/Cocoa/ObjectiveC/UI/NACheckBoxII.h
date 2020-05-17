
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


@implementation NACocoaCheckBox
- (id) initWithCoreCheckBox:(NACoreCheckBox*)newcoreCheckBox frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  
  [self setButtonType:NAButtonTypeSwitch];
  coreCheckBox = newcoreCheckBox;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];

  return self;
}
- (void) setText:(const NAUTF8Char*)text{
  [self setTitle:[NSString stringWithUTF8String:text]];
}
- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  naDispatchUIElementCommand((NACoreUIElement*)coreCheckBox, NA_UI_COMMAND_PRESSED);
}
- (void) setCheckBoxState:(NABool)state{
  [self setState:state ? NAStateOn : NAStateOff];
}
- (NABool) checkBoxState{
  return ([self state] == NAStateOn) ? NA_TRUE : NA_FALSE;
}
@end



NA_DEF NACheckBox* naNewCheckBox(const NAUTF8Char* text, NASize size){
  NACocoaCheckBox* cocoaCheckBox;
  NACoreCheckBox* coreCheckBox = naAlloc(NACoreCheckBox);
  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NSRect boundrect = frameRect;
  boundrect.origin.x = 0;
  boundrect.origin.y = 0;

  cocoaCheckBox = [[NACocoaCheckBox alloc] initWithCoreCheckBox:coreCheckBox frame:frameRect];
  naInitCoreCheckBox(coreCheckBox, NA_COCOA_PTR_OBJC_TO_C(cocoaCheckBox));
  [cocoaCheckBox setText:text];
  
  return (NACheckBox*)coreCheckBox;
}



NA_DEF void naDestructCheckBox(NACheckBox* checkBox){
  NACoreCheckBox* coreCheckBox = (NACoreCheckBox*)checkBox;
  naClearCoreCheckBox(coreCheckBox);
}



NA_HDEF NARect naGetCheckBoxAbsoluteInnerRect(NACoreUIElement* checkBox){
  NA_UNUSED(checkBox);
  return naMakeRectS(20, 40, 100, 50);
}



NA_HDEF void naSetCheckBoxState(NACheckBox* checkBox, NABool state){
  naDefineCocoaObject(NACocoaCheckBox, cocoaCheckBox, checkBox);
  [cocoaCheckBox setCheckBoxState:state];
}



NA_HDEF NABool naGetCheckBoxState(NACheckBox* checkBox){
  naDefineCocoaObject(NACocoaCheckBox, cocoaCheckBox, checkBox);
  return [cocoaCheckBox checkBoxState];
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
