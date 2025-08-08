
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



struct NACocoaSelect{
  NASelect select;
};
NA_HAPI void na_DestructCocoaSelect(NACocoaSelect* cocoaSelect);
NA_RUNTIME_TYPE(NACocoaSelect, na_DestructCocoaSelect, NA_FALSE);



@implementation NACocoaNativeSelect

- (id) initWithSelect:(NACocoaSelect*)newCocoaSelect frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  
  cocoaSelect = newCocoaSelect;

  [self setTarget:self];
  [self setAction:@selector(onPressed:)];

  return self;
}

- (void)dealloc{
  NA_COCOA_SUPER_DEALLOC();
}

- (void) addMenuItem:(NSMenuItem*)item atItem:(const NAMenuItem*)atItem{  
  size_t index = naGetSelectItemIndex(&cocoaSelect->select, atItem);
  //int index = [self getMenuItemIndex:atItem];
  if(index == naGetSelectItemCount(&cocoaSelect->select)) {
    [[self menu] addItem:item];
  }else{
    [[self menu] insertItem:item atIndex: (NSInteger)index];
  }
}

- (void) setNAFont:(NAFont*)font{
  [self setFont:NA_COCOA_PTR_C_TO_OBJC(naGetFontNativePointer(font))];
}

- (void) setVisible:(NABool)visible{
  [self setHidden:visible ? NO : YES];
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}
@end



NA_DEF NASelect* naNewSelect(double width) {
  NACocoaSelect* cocoaSelect = naNew(NACocoaSelect);

  NACocoaNativeSelect* nativePtr = [[NACocoaNativeSelect alloc]
    initWithSelect:cocoaSelect
    frame:naMakeNSRectWithSize(naMakeSize(width, 25))];
    
  na_InitSelect((NASelect*)cocoaSelect, NA_COCOA_PTR_OBJC_TO_C(nativePtr));
  
  [nativePtr setNAFont:cocoaSelect->select.font];

  return (NASelect*)cocoaSelect;
}



NA_DEF void na_DestructCocoaSelect(NACocoaSelect* cocoaSelect) {
  na_ClearSelect((NASelect*)cocoaSelect);
}



NA_DEF void naSetSelectVisible(NASelect* select, NABool visible) {
  naDefineCocoaObject(NACocoaNativeSelect, nativePtr, select);
  [nativePtr setVisible:visible];
}



NA_DEF void naAddSelectMenuItem(NASelect* select, NAMenuItem* item, const NAMenuItem* atItem) {
  naDefineCocoaObject(NACocoaNativeSelect, nativePtr, select);
  naDefineCocoaObjectConst(NACocoaNativeMenuItem, nativeItemPtr, item);

  if(atItem) {
//    naDefineCocoaObjectConst(NACocoaNativeMenuItem, nativeItemAtPtr, atItem);
    [nativePtr addMenuItem:nativeItemPtr atItem:atItem];
  }else{
    [nativePtr addMenuItem:nativeItemPtr atItem:nil];
  }
  
  na_AddSelectChild(select, item, atItem);
}



NA_DEF void naSetSelectIndexSelected(NASelect* select, size_t index) {
  naDefineCocoaObject(NACocoaNativeSelect, nativePtr, select);
  [nativePtr selectItemAtIndex:(NSInteger)index];
}



NA_DEF void naSetSelectItemSelected(NASelect* select, const NAMenuItem* item) {
  naDefineCocoaObject(NACocoaNativeSelect, nativePtr, select);
  size_t index = naGetSelectItemIndex(select, item);
  [nativePtr selectItemAtIndex:(NSInteger)index];
}



NA_DEF void naSetSelectEnabled(NASelect* select, NABool enabled) {
  naDefineCocoaObject(NACocoaNativeSelect, nativePtr, select);
  [nativePtr setEnabled:(BOOL)enabled];
}


NA_HDEF NARect na_GetSelectRect(const NA_UIElement* select) {
  naDefineCocoaObjectConst(NACocoaNativeSelect, nativePtr, select);
  return naMakeRectWithNSRect([nativePtr frame]);
}

NA_HDEF void na_SetSelectRect(NA_UIElement* select, NARect rect) {
  naDefineCocoaObject(NACocoaNativeSelect, nativePtr, select);
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
