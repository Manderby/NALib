
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaNativePopupButton

- (id) initWithPopupButton:(NACocoaPopupButton*)newCocoaButton frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  
  cocoaPopupButton = newCocoaButton;

  [self setTarget:self];
  [self setAction:@selector(onPressed:)];
  
  return self;
}

- (void)dealloc{
  NA_COCOA_SUPER_DEALLOC();
}

- (int) getMenuItemIndex:(NSMenuItem*)item{
  NSUInteger index = [[self itemArray] indexOfObject:item];
  return index == NSNotFound ? -1 : (int)index;
}

- (void) addMenuItem:(NSMenuItem*)item atItem:(NSMenuItem*)atItem{  
  int index = [self getMenuItemIndex:atItem];
  if(index == -1){
    [[self menu] addItem:item];
  }else{
    [[self menu] insertItem:item atIndex: (NSInteger)index];
  }
}

//- (void) onPressed:(id)sender{
//  NA_UNUSED(sender);
//  na_DispatchUIElementCommand((NA_UIElement*)cocoaPopupButton, NA_UI_COMMAND_PRESSED);
//}

- (void) setVisible:(NABool)visible{
  [self setHidden:visible ? NO : YES];
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}
@end



NA_DEF NAPopupButton* naNewPopupButton(double width){
  NACocoaPopupButton* cocoaPopupButton = naNew(NACocoaPopupButton);

  NACocoaNativePopupButton* nativePtr = [[NACocoaNativePopupButton alloc]
    initWithPopupButton:cocoaPopupButton
    frame:naMakeNSRectWithSize(naMakeSize(width, 23))];
  na_InitPopupButton((NAPopupButton*)cocoaPopupButton, NA_COCOA_PTR_OBJC_TO_C(nativePtr));
  
  return (NAPopupButton*)cocoaPopupButton;
}



NA_DEF void na_DestructCocoaPopupButton(NACocoaPopupButton* cocoaPopupButton){
  na_ClearPopupButton((NAPopupButton*)cocoaPopupButton);
}



NA_DEF void naSetPopupButtonVisible(NAPopupButton* popupButton, NABool visible){
  naDefineCocoaObject(NACocoaNativePopupButton, nativePtr, popupButton);
  [nativePtr setVisible:visible];
}



NA_DEF void naAddPopupButtonMenuItem(NAPopupButton* popupButton, NAMenuItem* item, NAMenuItem* atItem){
  naDefineCocoaObject(NACocoaNativePopupButton, nativePopupPtr, popupButton);
  naDefineCocoaObject(NACocoaNativeMenuItem, nativeItemPtr, item);

  if(atItem){
    naDefineCocoaObject(NACocoaNativeMenuItem, nativeItemAtPtr, atItem);
    [nativePopupPtr addMenuItem:nativeItemPtr atItem:nativeItemAtPtr];
  }else{
    [nativePopupPtr addMenuItem:nativeItemPtr atItem:nil];
  }
  
  na_AddPopupButtonChild(popupButton, item, atItem);
}



NA_DEF size_t naGetPopupButtonItemIndex(NAPopupButton* popupButton, NAMenuItem* item){
  return naGetListElemIndex(&(popupButton->childs), item);
}



NA_DEF void naSetPopupButtonItemSelected(NAPopupButton* popupButton, NAMenuItem* item){
  naDefineCocoaObject(NACocoaNativePopupButton, nativePopupPtr, popupButton);
  size_t index = naGetPopupButtonItemIndex(popupButton, item);
  [nativePopupPtr selectItemAtIndex:(NSInteger)index];
}



NA_DEF void naSetPopupButtonEnabled(NAPopupButton* popupButton, NABool enabled){
  naDefineCocoaObject(NACocoaNativePopupButton, nativePopupPtr, popupButton);
  [nativePopupPtr setEnabled:(BOOL)enabled];
}



NA_HDEF NARect na_GetPopupButtonAbsoluteInnerRect(NA_UIElement* popupButton){
  naDefineCocoaObject(NACocoaNativePopupButton, nativePtr, popupButton);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(popupButton), naGetApplication(), NA_FALSE);
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
