
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



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



NA_DEF NAMenuItem* naNewMenuItem(const NAUTF8Char* text){
  NACocoaMenuItem* cocoaMenuItem = naNew(NACocoaMenuItem);
  
  NACocoaNativeMenuItem* nativeItemPtr = [[NACocoaNativeMenuItem alloc]
    initWithMenuItem:cocoaMenuItem
    text: text];
  na_InitMenuItem((NAMenuItem*)cocoaMenuItem, NA_COCOA_PTR_OBJC_TO_C(nativeItemPtr), NA_NULL);

  return (NAMenuItem*)cocoaMenuItem;
}


NA_DEF NAMenuItem* naNewMenuSeparator(){
  NACocoaMenuItem* cocoaMenuItem = naNew(NACocoaMenuItem);
  
  NSMenuItem* nativeItemPtr = [NSMenuItem separatorItem];
  na_InitMenuItem((NAMenuItem*)cocoaMenuItem, NA_COCOA_PTR_OBJC_TO_C(nativeItemPtr), NA_NULL);
  
  return (NAMenuItem*)cocoaMenuItem;
}



NA_DEF void na_DestructCocoaMenuItem(NACocoaMenuItem* cocoaMenuItem){
  na_ClearMenuItem((NAMenuItem*)cocoaMenuItem);
}



NA_DEF void naSetMenuItemText(NAMenuItem* menuItem, const NAUTF8Char* text){
  #if NA_DEBUG
    if(!menuItem)
      naError("Menu item is Null pointer");
  #endif

  naDefineCocoaObject(NACocoaNativeMenuItem, nativePtr, menuItem);
  
  NSString* nsString = [NSString stringWithUTF8String:text];
  [nativePtr setTitle: nsString];
  if([nativePtr hasSubmenu]){
    [[nativePtr submenu] setTitle: nsString];
  }
}



NA_DEF void naSetMenuItemState(NAMenuItem* menuItem, NABool state){
  naDefineCocoaObject(NACocoaNativeMenuItem, nativePtr, menuItem);
  [nativePtr setState:state ? NAValueStateOn : NAValueStateOff];
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
