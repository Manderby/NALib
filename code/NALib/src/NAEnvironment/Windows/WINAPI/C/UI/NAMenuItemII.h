
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NA_DEF NAMenuItem* naNewMenuItem(const NAUTF8Char* text){
  NAWINAPIMenuItem* winapiMenuItem = naNew(NAWINAPIMenuItem);

  winapiMenuItem->isSeparator = NA_FALSE;
  winapiMenuItem->text = naNewStringWithFormat("%s", text);
  //NAWINAPIMenu* winapiMenu = (NAWINAPIMenu*)menu;

  //MENUITEMINFO menuItemInfo;
  //naZeron(&menuItemInfo, sizeof(MENUITEMINFO));
  //menuItemInfo.cbSize = sizeof(MENUITEMINFO);
  //// Note for the future, do not combine MIIM_TYPE with MIIM_FTYPE.
  //menuItemInfo.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
  //menuItemInfo.wID = na_GetApplicationNextMenuItemId(naGetApplication());
  //menuItemInfo.fType = MFT_STRING;
  //menuItemInfo.dwTypeData = naAllocSystemStringWithUTF8String(text);
  //menuItemInfo.cch = (UINT)naStrlen(text);
  //menuItemInfo.fState = /*MFS_CHECKED | */MFS_ENABLED/* | MFS_DEFAULT*/;

  //na_SetMenuItemId(&(winapiMenuItem->menuItem), menuItemInfo.wID);

  //size_t index = naGetMenuItemIndex(menu, atItem);

  //InsertMenuItem(
  //  winapiMenu->hMenu,
  //  (UINT)index,
  //  TRUE,
  //  &menuItemInfo);

  na_InitMenuItem(&(winapiMenuItem->menuItem), winapiMenuItem, NA_NULL);
  //na_InitMenuItem(&(winapiMenuItem->menuItem), winapiMenuItem, (NA_UIElement*)menu);
  //na_AddMenuChild(menu, (NAMenuItem*)winapiMenuItem, atItem);

  return (NAMenuItem*)winapiMenuItem;
}

NA_DEF NAMenuItem* naNewMenuSeparator(){
  NAWINAPIMenuItem* winapiMenuItem = naNew(NAWINAPIMenuItem);
  //NAWINAPIMenu* winapiMenu = (NAWINAPIMenu*)menu;

  //MENUITEMINFO menuItemInfo;
  //naZeron(&menuItemInfo, sizeof(MENUITEMINFO));
  //menuItemInfo.cbSize = sizeof(MENUITEMINFO);
  //menuItemInfo.fMask = MIIM_ID | MIIM_FTYPE;
  //menuItemInfo.wID = na_GetApplicationNextMenuItemId(naGetApplication());
  //menuItemInfo.fType = MFT_SEPARATOR;

  //size_t index = naGetMenuItemIndex(menu, atItem);

  //BOOL test = InsertMenuItem(
  //  winapiMenu->hMenu,
  //  (UINT)index,
  //  TRUE,
  //  &menuItemInfo);

  na_InitMenuItem(&(winapiMenuItem->menuItem), winapiMenuItem, NA_NULL);
  //na_InitMenuItem(&(winapiMenuItem->menuItem), winapiMenuItem, (NA_UIElement*)menu);
  //na_AddMenuChild(menu, (NAMenuItem*)winapiMenuItem, atItem);

  return (NAMenuItem*)winapiMenuItem;
}

NA_DEF void na_DestructWINAPIMenuItem(NAWINAPIMenuItem* winapiMenuItem){
  na_ClearMenuItem((NAMenuItem*)winapiMenuItem);
}

NA_HDEF NARect na_GetMenuItemAbsoluteInnerRect(NA_UIElement* menu){
  NA_UNUSED(menu);
  return naMakeRectS(0, 0, 1, 1);
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
