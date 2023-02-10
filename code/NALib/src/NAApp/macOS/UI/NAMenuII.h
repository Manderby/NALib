
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaNativeMenu

- (id) initWithMenu:(NACocoaMenu*)newCocoaMenu{
  self = [super initWithTitle:@"NALib"];
  cocoaMenu = newCocoaMenu;
  return self;
}

//- (int) getMenuItemIndex:(NSMenuItem*)item{
//  if(!item){return -1;}
//  NSUInteger index = [[self itemArray] indexOfObject:item];
//  return index == NSNotFound ? -1 : (int)index;
//}

- (void) addMenuItem:(NSMenuItem*)item atItem:(const NAMenuItem*)atItem{
  size_t index = naGetMenuItemIndex(&(cocoaMenu->menu), atItem);
  //int index = [self getMenuItemIndex:atItem];
  if(index == -1){
    [self addItem:item];
  }else{
    [self insertItem:item atIndex: (NSInteger)index];
  }
}

- (void) displayAt:(NAPos)pos{
  [self popUpMenuPositioningItem: nil
    atLocation: naMakeNSPointWithPos(pos)
    inView: nil];
}

- (NARect) getInnerRect{
  // todo
  return naMakeRectS(0, 0, 10, 10);
}

@end



NA_DEF NAMenu* naNewMenu(){
  NACocoaMenu* cocoaMenu = naNew(NACocoaMenu);
  
  NACocoaNativeMenu* nativePtr = [[NACocoaNativeMenu alloc]
    initWithMenu:cocoaMenu];
    
  na_InitMenu((NAMenu*)cocoaMenu, NA_COCOA_PTR_OBJC_TO_C(nativePtr), NA_NULL);

  return (NAMenu*)cocoaMenu;
}



NA_DEF void na_DestructCocoaMenu(NACocoaMenu* cocoaMenu){
  na_ClearMenu((NAMenu*)cocoaMenu);
}



NA_DEF void naAddMenuItem(NAMenu* menu, NAMenuItem* item, const NAMenuItem* atItem){
  naDefineCocoaObject(NACocoaNativeMenu, nativeMenuPtr, menu);
  naDefineCocoaObject(NACocoaNativeMenuItem, nativeItemPtr, item);

  if(atItem){
//    naDefineCocoaObjectConst(NACocoaNativeMenuItem, nativeItemAtPtr, atItem);
    [nativeMenuPtr addMenuItem:nativeItemPtr atItem:atItem];
  }else{
    [nativeMenuPtr addMenuItem:nativeItemPtr atItem:nil];
  }
  
  na_AddMenuChild(menu, item, atItem);
}



NA_DEF void naPresentMenu(const NAMenu* menu, NAPos pos, void* parentUIElement){
  NA_UNUSED(parentUIElement);
  naDefineCocoaObjectConst(NACocoaNativeMenu, nativePtr, menu);
  [nativePtr displayAt:pos];
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
