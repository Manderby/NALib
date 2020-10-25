
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaSlider
- (id) initWithSlider:(NASlider*)newSlider frame:(NSRect)frame{
  NSRect documentrect = NSMakeRect(0, 0, frame.size.width, frame.size.height);
  self = [super initWithFrame:documentrect];
  [self setTarget:self];
  [self setAction:@selector(onValueChanged:)];
  slider = newSlider;
  return self;
}
- (void) setTickCount:(NAInt)tickCount{
  self.numberOfTickMarks = tickCount;
  self.allowsTickMarkValuesOnly = tickCount > 0;
}
- (double) getSliderValue{
  return [self doubleValue];
}
- (void) setSliderValue:(double) value{
  [self setDoubleValue:value];
}
- (void) onValueChanged:(id)sender{
  NA_UNUSED(sender);
  na_DispatchUIElementCommand((NA_UIElement*)slider, NA_UI_COMMAND_EDITED);
}
@end



NA_DEF NASlider* naNewSlider(NASize size){
  NASlider* slider = naAlloc(NASlider);
  
  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaSlider* cocoaSlider = [[NACocoaSlider alloc] initWithSlider:slider frame:frameRect];
  na_InitSlider(slider, NA_COCOA_PTR_OBJC_TO_C(cocoaSlider));
  [cocoaSlider setTag: (NSInteger)slider];

  return (NASlider*)slider;
}



NA_DEF void na_DestructSlider(NASlider* slider){
  na_ClearSlider(slider);
}



NA_DEF void naSetSliderTickCount(NASlider* slider, NAInt tickCount){
  naDefineCocoaObject(NACocoaSlider, cocoaSlider, slider);
  [cocoaSlider setTickCount:tickCount];
}



NA_DEF double naGetSliderValue(NASlider* slider){
  naDefineCocoaObject(NACocoaSlider, cocoaSlider, slider);
  return [cocoaSlider getSliderValue];
}



NA_DEF void naSetSliderValue(NASlider* slider, double value){
  naDefineCocoaObject(NACocoaSlider, cocoaSlider, slider);
  [cocoaSlider setSliderValue:value];
}



NA_HDEF NARect na_GetSliderAbsoluteInnerRect(NA_UIElement* slider){
  NA_UNUSED(slider);
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
