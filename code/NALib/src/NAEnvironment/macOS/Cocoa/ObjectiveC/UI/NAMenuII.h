
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NACocoaMenu NACocoaMenu;
struct NACocoaMenu{
  NAMenu menu;
};

NA_HAPI void na_DestructCocoaMenu(NACocoaMenu* cocoaMenu);
NA_RUNTIME_TYPE(NACocoaMenu, na_DestructCocoaMenu, NA_FALSE);

@interface NACocoaNativeMenu : NSMenu{
  NACocoaMenu* cocoaMenu;
}
@end

typedef struct NACocoaMenuItem NACocoaMenuItem;
struct NACocoaMenuItem{
  NAMenuItem menuItem;
};

NA_HAPI void na_DestructCocoaMenuItem(NACocoaMenuItem* cocoaMenuItem);
NA_RUNTIME_TYPE(NACocoaMenuItem, na_DestructCocoaMenuItem, NA_FALSE);

@interface NACocoaNativeMenuItem : NSMenuItem{
  NACocoaMenuItem* cocoaMenuItem;
}
@end



@implementation NACocoaNativeMenu

- (id) initWithMenu:(NACocoaMenu*)newCocoaMenu{
  self = [super initWithTitle:@"NALib"];
  cocoaMenu = newCocoaMenu;
  return self;
}

- (int) getMenuItemIndex:(NSMenuItem*)item{
  NSUInteger index = [[self itemArray] indexOfObject:item];
  return index == NSNotFound ? -1 : (int)index;
}

- (void) addMenuItem:(NSMenuItem*)item atItem:(NSMenuItem*)atItem{  
  int index = [self getMenuItemIndex:atItem];
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

@implementation NACocoaNativeMenuItem

- (id) initWithMenuItem:(NACocoaMenuItem*)newCocoaMenuItem text:(const NAUTF8Char*) text{
  self = [super
    initWithTitle:[NSString stringWithUTF8String:text]
    action:@selector(itemSelected:)
    keyEquivalent:@""];
  [self setTarget:self];
  cocoaMenuItem = newCocoaMenuItem;
  return self;
}

- (void)itemSelected:(id)sender{
  na_DispatchUIElementCommand((NA_UIElement*)cocoaMenuItem, NA_UI_COMMAND_PRESSED);
}

- (NARect) getInnerRect{
  // todo
  return naMakeRectS(0, 0, 10, 10);
}

@end



NA_DEF NAMenu* naNewMenu(void* parent){
  NACocoaMenu* cocoaMenu = naNew(NACocoaMenu);
  
  NACocoaNativeMenu* nativePtr = [[NACocoaNativeMenu alloc]
    initWithMenu:cocoaMenu];
  na_InitMenu((NAMenu*)cocoaMenu, NA_COCOA_PTR_OBJC_TO_C(nativePtr), (NA_UIElement*)parent);

  return (NAMenu*)cocoaMenu;
}



NA_DEF void na_DestructCocoaMenu(NACocoaMenu* cocoaMenu){
  na_ClearMenu((NAMenu*)cocoaMenu);
}



NA_DEF NAMenuItem* naNewMenuItem(NAMenu* menu, const NAUTF8Char* text, NAMenuItem* atItem){
  NACocoaMenuItem* cocoaMenuItem = naNew(NACocoaMenuItem);
  
  NACocoaNativeMenuItem* nativeItemPtr = [[NACocoaNativeMenuItem alloc]
    initWithMenuItem:cocoaMenuItem
    text: text];
  na_InitMenuItem((NAMenuItem*)cocoaMenuItem, NA_COCOA_PTR_OBJC_TO_C(nativeItemPtr));

  naDefineCocoaObject(NACocoaNativeMenu, nativeMenuPtr, menu);
  
  if(atItem){
    naDefineCocoaObject(NACocoaNativeMenuItem, nativeItemAtPtr, atItem);
    [nativeMenuPtr addMenuItem:nativeItemPtr atItem:nativeItemAtPtr];
  }else{
    [nativeMenuPtr addMenuItem:nativeItemPtr atItem:nil];
  }
  
  na_AddMenuChild(menu, (NAMenuItem*)cocoaMenuItem);
  
  return (NAMenuItem*)cocoaMenuItem;
}


NA_DEF NAMenuItem* naNewMenuSeparator(NAMenu* menu, NAMenuItem* atItem){
  NACocoaMenuItem* cocoaMenuItem = naNew(NACocoaMenuItem);
  
  NSMenuItem* nativeItemPtr = [NSMenuItem separatorItem];
  na_InitMenuItem((NAMenuItem*)cocoaMenuItem, NA_COCOA_PTR_OBJC_TO_C(nativeItemPtr));

  naDefineCocoaObject(NACocoaNativeMenu, nativeMenuPtr, menu);
  
  if(atItem){
    naDefineCocoaObject(NACocoaNativeMenuItem, nativeItemAtPtr, atItem);
    [nativeMenuPtr addMenuItem:nativeItemPtr atItem:nativeItemAtPtr];
  }else{
    [nativeMenuPtr addMenuItem:nativeItemPtr atItem:nil];
  }
  
  na_AddMenuChild(menu, (NAMenuItem*)cocoaMenuItem);
  
  return (NAMenuItem*)cocoaMenuItem;
}

NA_DEF void na_DestructCocoaMenuItem(NACocoaMenuItem* cocoaMenuItem){
  na_ClearMenuItem((NAMenuItem*)cocoaMenuItem);
}



NA_DEF int32 naGetMenuItemIndex(NAMenu* menu, NAMenuItem* item){
  naDefineCocoaObject(NACocoaNativeMenu, nativeMenuPtr, menu);
  naDefineCocoaObject(NACocoaNativeMenuItem, nativeMenuItemPtr, item);
  return [nativeMenuPtr getMenuItemIndex:nativeMenuItemPtr];
}



NA_DEF void naPresentMenu(NAMenu* menu, NAPos pos){
  naDefineCocoaObject(NACocoaNativeMenu, nativePtr, menu);
  [nativePtr displayAt:pos];
}



NA_HDEF NARect na_GetMenuAbsoluteInnerRect(NA_UIElement* menu){
  naDefineCocoaObject(NACocoaNativeMenu, nativePtr, menu);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(menu), naGetApplication(), NA_FALSE);
  NARect relRect = [nativePtr getInnerRect];
  return naMakeRect(
    naMakePos(parentRect.pos.x + relRect.pos.x, parentRect.pos.y + relRect.pos.y),
    relRect.size);
}



NA_HDEF NARect na_GetMenuItemAbsoluteInnerRect(NA_UIElement* menuItem){
  naDefineCocoaObject(NACocoaNativeMenuItem, nativePtr, menuItem);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(menuItem), naGetApplication(), NA_FALSE);
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
