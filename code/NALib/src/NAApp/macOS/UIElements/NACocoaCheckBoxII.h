
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



struct NACocoaCheckBox{
  NACheckBox checkBox;
};
NA_HAPI void na_DestructCocoaCheckBox(NACocoaCheckBox* cocoaCheckBox);
NA_RUNTIME_TYPE(NACocoaCheckBox, na_DestructCocoaCheckBox, NA_FALSE);



@implementation NACocoaNativeCheckBox

- (id) initWithCheckBox:(NACocoaCheckBox*)newCocoaCheckBox frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  
  [self setButtonType:NAButtonTypeSwitch];
  cocoaCheckBox = newCocoaCheckBox;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];

  return self;
}

- (void) setText:(const NAUTF8Char*)text{
  [self setTitle:[NSString stringWithUTF8String:text]];
}

- (void) setColor:(const NAColor*)color{
  NSColor* nsColor;
  if(color) {
    uint8 buf[4];
    naFillSRGBu8WithColor(buf, color, NA_COLOR_BUFFER_RGBA);
    nsColor = [NSColor colorWithCalibratedRed:buf[0] / 255. green:buf[1] / 255. blue:buf[2] / 255. alpha:buf[3] / 255.];
  }else{
    nsColor = naGetLabelColor();
  }
  NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithAttributedString:[self attributedTitle]];
  NSRange range = NSMakeRange(0, [attrString length]);

  [attrString beginEditing];
  NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
  [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
  paragraphStyle.alignment = [self alignment];
  [attrString addAttribute:NSForegroundColorAttributeName value:nsColor range:range];
  NA_COCOA_RELEASE(paragraphStyle);
  [attrString endEditing];
  
  [self setAttributedTitle: attrString];
  NA_COCOA_RELEASE(attrString);
}

- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaCheckBox, NA_UI_COMMAND_PRESSED)) {
    // don't know what to do.
  }
}

- (void) setVisible:(NABool)visible{
  [self setHidden:visible ? NO : YES];
}

- (void) setNAFont:(NAFont*)font{
  [self setFont:NA_COCOA_PTR_C_TO_OBJC(naGetFontNativePointer(font))];
}

- (void) setCheckBoxState:(NABool)state{
  [self setState:state ? NAStateOn : NAStateOff];
}

- (NABool) checkBoxState{
  return ([self state] == NAStateOn) ? NA_TRUE : NA_FALSE;
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}
@end



NA_DEF NACheckBox* naNewCheckBox(const NAUTF8Char* text, double width) {
  NACocoaCheckBox* cocoaCheckBox = naNew(NACocoaCheckBox);

  NACocoaNativeCheckBox* nativePtr = [[NACocoaNativeCheckBox alloc]
    initWithCheckBox:cocoaCheckBox
    frame:naMakeNSRectWithSize(naMakeSize(width, 18))];    

  na_InitCheckBox((NACheckBox*)cocoaCheckBox, NA_COCOA_PTR_OBJC_TO_C(nativePtr));
  
  [nativePtr setNAFont:cocoaCheckBox->checkBox.font];

  [nativePtr setText:text];
  
  return (NACheckBox*)cocoaCheckBox;
}



NA_HAPI void na_DestructCocoaCheckBox(NACocoaCheckBox* cocoaCheckBox) {
  na_ClearCheckBox((NACheckBox*)cocoaCheckBox);
}



NA_DEF void naSetCheckBoxTextColor(NACheckBox* checkBox, const NAColor* color) {
  naDefineCocoaObject(NACocoaNativeCheckBox, nativePtr, checkBox);
  [nativePtr setColor:color];
}



NA_DEF void naSetCheckBoxState(NACheckBox* checkBox, NABool state) {
  naDefineCocoaObject(NACocoaNativeCheckBox, nativePtr, checkBox);
  [nativePtr setCheckBoxState:state];
}



NA_DEF void naSetCheckBoxVisible(NACheckBox* checkBox, NABool visible) {
  naDefineCocoaObject(NACocoaNativeCheckBox, nativePtr, checkBox);
  [nativePtr setVisible:visible];
}



NA_DEF void naSetCheckBoxEnabled(NACheckBox* checkBox, NABool enabled) {
  naDefineCocoaObject(NACocoaNativeCheckBox, nativePtr, checkBox);
  [nativePtr setEnabled:(BOOL)enabled];
}



NA_DEF NABool naGetCheckBoxState(const NACheckBox* checkBox) {
  naDefineCocoaObjectConst(NACocoaNativeCheckBox, nativePtr, checkBox);
  return [nativePtr checkBoxState];
}



NA_HDEF NARect na_GetCheckBoxRect(const NA_UIElement* checkBox) {
  naDefineCocoaObjectConst(NACocoaNativeCheckBox, nativePtr, checkBox);
  return naMakeRectWithNSRect([nativePtr frame]);
}

NA_HDEF void na_SetCheckBoxRect(NA_UIElement* checkBox, NARect rect) {
  naDefineCocoaObjectConst(NACocoaNativeCheckBox, nativePtr, checkBox);
  [nativePtr setFrame:naMakeNSRectWithRect(rect)];
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
