
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


typedef struct NAWINAPIMenu NAWINAPIMenu;
struct NAWINAPIMenu {
  NAMenu   menu;
  HMENU    hMenu;
};

NA_HAPI void na_DestructWINAPIMenu(NAWINAPIMenu* winapiMenu);
NA_RUNTIME_TYPE(NAWINAPIMenu, na_DestructWINAPIMenu, NA_FALSE);



typedef struct NAWINAPIMenuItem NAWINAPIMenuItem;
struct NAWINAPIMenuItem {
  NAMenuItem   menuItem;
};

NA_HAPI void na_DestructWINAPIMenuItem(NAWINAPIMenuItem* winapiMenuItem);
NA_RUNTIME_TYPE(NAWINAPIMenuItem, na_DestructWINAPIMenuItem, NA_FALSE);



NA_DEF NAMenu* naNewMenu(void* parent){
  NAWINAPIMenu* winapiMenu = naNew(NAWINAPIMenu);

  winapiMenu->hMenu = CreatePopupMenu();

  //MENUINFO menuInfo;
  //naZeron(&menuInfo, sizeof(MENUINFO));
  //menuInfo.cbSize = sizeof(MENUINFO);
  //menuInfo.fMask = MIM_STYLE;
  //menuInfo.dwStyle = MNS_NOCHECK;
  //SetMenuInfo(winapiMenu->hMenu, &menuInfo);

  na_InitMenu(&(winapiMenu->menu), winapiMenu->hMenu, parent);

  return (NAMenu*)winapiMenu;
}

NA_DEF NAMenuItem* naNewMenuItem(NAMenu* menu, const NAUTF8Char* text, NAMenuItem* atItem){
  NAWINAPIMenuItem* winapiMenuItem = naNew(NAWINAPIMenuItem);
  NAWINAPIMenu* winapiMenu = (NAWINAPIMenu*)menu;

  MENUITEMINFO menuItemInfo;
  naZeron(&menuItemInfo, sizeof(MENUITEMINFO));
  menuItemInfo.cbSize = sizeof(MENUITEMINFO);
  // Note for the future, do not combine MIIM_TYPE with MIIM_FTYPE.
  menuItemInfo.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
  menuItemInfo.wID = na_GetApplicationNextMenuItemId(naGetApplication());
  menuItemInfo.fType = MFT_STRING;
  menuItemInfo.dwTypeData = naAllocSystemStringWithUTF8String(text);
  menuItemInfo.cch = (UINT)naStrlen(text);
  menuItemInfo.fState = /*MFS_CHECKED | */MFS_ENABLED/* | MFS_DEFAULT*/;

  na_SetMenuItemId(&(winapiMenuItem->menuItem), menuItemInfo.wID);

  size_t index = naGetMenuItemIndex(menu, atItem);

  BOOL test = InsertMenuItem(
    winapiMenu->hMenu,
    (UINT)index,
    TRUE,
    &menuItemInfo);

  na_InitMenuItem(&(winapiMenuItem->menuItem), winapiMenuItem, (NA_UIElement*)menu);
  na_AddMenuChild(menu, (NAMenuItem*)winapiMenuItem, atItem);

  return (NAMenuItem*)winapiMenuItem;
}

NA_DEF NAMenuItem* naNewMenuSeparator(NAMenu* menu, NAMenuItem* atItem){
  NAWINAPIMenuItem* winapiMenuItem = naNew(NAWINAPIMenuItem);
  NAWINAPIMenu* winapiMenu = (NAWINAPIMenu*)menu;

  MENUITEMINFO menuItemInfo;
  naZeron(&menuItemInfo, sizeof(MENUITEMINFO));
  menuItemInfo.cbSize = sizeof(MENUITEMINFO);
  menuItemInfo.fMask = MIIM_ID | MIIM_FTYPE;
  menuItemInfo.wID = na_GetApplicationNextMenuItemId(naGetApplication());
  menuItemInfo.fType = MFT_SEPARATOR;

  size_t index = naGetMenuItemIndex(menu, atItem);

  BOOL test = InsertMenuItem(
    winapiMenu->hMenu,
    (UINT)index,
    TRUE,
    &menuItemInfo);

  na_InitMenuItem(&(winapiMenuItem->menuItem), winapiMenuItem, (NA_UIElement*)menu);
  na_AddMenuChild(menu, (NAMenuItem*)winapiMenuItem, atItem);

  return (NAMenuItem*)winapiMenuItem;
}

NA_DEF size_t naGetMenuItemIndex(NAMenu* menu, NAMenuItem* item){
  return naGetListElemIndex(&(menu->childs), item);
}

NA_DEF void naPresentMenu(NAMenu* menu, NAPos pos){
  NAWINAPIMenu* winapiMenu = (NAWINAPIMenu*)menu;

  HMENU hMenu = CreatePopupMenu();

  POINT cursorPos;
  GetCursorPos(&cursorPos);

  na_SetApplicationLastOpenedMenu(naGetApplication(), menu);

  int selection = TrackPopupMenu(
    winapiMenu->hMenu, 
    TPM_LEFTALIGN | TPM_RIGHTBUTTON/* | TPM_RETURNCMD*/, 
    cursorPos.x, cursorPos.y, 0,
    naGetUIElementNativePtr(naGetUIElementParent(menu)), NULL);
}



NA_DEF void na_DestructWINAPIMenu(NAWINAPIMenu* winapiMenu){
  DestroyMenu(winapiMenu->hMenu);
  na_ClearMenu((NAMenu*)winapiMenu);
}

NA_DEF void na_DestructWINAPIMenuItem(NAWINAPIMenuItem* winapiMenuItem){
  na_ClearMenuItem((NAMenuItem*)winapiMenuItem);
}



NA_HDEF NARect na_GetMenuAbsoluteInnerRect(NA_UIElement* menu){
  NA_UNUSED(menu);
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
