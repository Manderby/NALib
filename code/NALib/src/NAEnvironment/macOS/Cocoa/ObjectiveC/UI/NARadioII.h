
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaRadio
- (id) initWithRadio:(NARadio*)newRadio frame:(NSRect)frame{
  NSRect newbounds = frame;
  newbounds.origin.x = 0;
  newbounds.origin.y = 0;

  self = [super initWithFrame:newbounds];
  
  [self setButtonType:NAButtonTypeRadio];
//  [self setBezelStyle:NSBezelStyleRounded];
//  [self setBezelStyle:NSBezelStyleShadowlessSquare];
//  [self setBordered:YES];
  radio = newRadio;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];

  containingview = [[NSView alloc] initWithFrame:frame];
  [containingview addSubview:self];

  return self;
}
- (NSView*) getContainingView{
  return containingview;
}
- (void) setText:(const NAUTF8Char*)text{
  [self setTitle:[NSString stringWithUTF8String:text]];
}
- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  na_DispatchUIElementCommand((NA_UIElement*)radio, NA_UI_COMMAND_PRESSED);
}
- (void) setRadioState:(NABool)state{
  [self setState:state ? NAStateOn : NAStateOff];
}
- (NABool) radioState{
  return ([self state] == NAStateOn) ? NA_TRUE : NA_FALSE;
}
@end



NA_DEF NARadio* naNewRadio(const NAUTF8Char* text, NASize size){
  NARadio* radio = naAlloc(NARadio);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaRadio* cocoaRadio = [[NACocoaRadio alloc] initWithRadio:radio frame:frameRect];
  na_InitRadio(radio, NA_COCOA_PTR_OBJC_TO_C(cocoaRadio));
  [cocoaRadio setText:text];
  
  return radio;
}



NA_DEF void na_DestructRadio(NARadio* radio){
  na_ClearRadio(radio);
}



NA_HDEF NARect na_GetRadioAbsoluteInnerRect(NA_UIElement* radio){
  NA_UNUSED(radio);
  return naMakeRectS(20, 40, 100, 50);
}



NA_DEF void naSetRadioState(NARadio* radio, NABool state){
  naDefineCocoaObject(NACocoaRadio, cocoaRadio, radio);
  [cocoaRadio setRadioState:state];
}



NA_DEF NABool naGetRadioState(NARadio* radio){
  naDefineCocoaObject(NACocoaRadio, cocoaRadio, radio);
  return [cocoaRadio radioState];
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
