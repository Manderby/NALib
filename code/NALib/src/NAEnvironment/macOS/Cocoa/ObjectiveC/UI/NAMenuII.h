
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



@implementation NACocoaNativeMenu

- (id) initWithMenu:(NACocoaMenu*)newCocoaMenu{
  self = [super initWithTitle:@"NALib"];

  NSMenuItem* item0 = [[NSMenuItem alloc] initWithTitle:@"I am Groot" action:nil keyEquivalent:@""];
  [self insertItem:item0 atIndex: 0];
  NSMenuItem* item1 = [[NSMenuItem alloc] initWithTitle:@"You are Winner" action:nil keyEquivalent:@""];
  [self insertItem:item1 atIndex: 1];
  NSMenuItem* item2 = [[NSMenuItem alloc] initWithTitle:@"Kohle, Kohle, Kohle" action:nil keyEquivalent:@""];
  [self insertItem:item2 atIndex: 2];
  NSMenuItem* item3 = [[NSMenuItem alloc] initWithTitle:@"None of that Objective-C rubbish" action:nil keyEquivalent:@""];
  [self insertItem:item3 atIndex: 3];

  cocoaMenu = newCocoaMenu;
  return self;
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
  na_InitMenu((NAMenu*)cocoaMenu, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  return (NAMenu*)cocoaMenu;
}



NA_DEF void na_DestructCocoaMenu(NACocoaMenu* cocoaMenu){
  na_ClearMenu((NAMenu*)cocoaMenu);
}



NA_DEF void naDisplayMenu(NAMenu* menu, NAPos pos){
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
