
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NA_DEF NAMenu* naNewMenu(){
  NAWINAPIMenu* winapiMenu = naNew(NAWINAPIMenu);

  winapiMenu->hMenu = CreatePopupMenu();

  //MENUINFO menuInfo;
  //naZeron(&menuInfo, sizeof(MENUINFO));
  //menuInfo.cbSize = sizeof(MENUINFO);
  //menuInfo.fMask = MIM_STYLE;
  //menuInfo.dwStyle = MNS_NOCHECK;
  //SetMenuInfo(winapiMenu->hMenu, &menuInfo);

  na_InitMenu(&(winapiMenu->menu), winapiMenu->hMenu, NA_NULL);

  return (NAMenu*)winapiMenu;
}



NA_DEF void na_DestructWINAPIMenu(NAWINAPIMenu* winapiMenu){
  DestroyMenu(winapiMenu->hMenu);
  na_ClearMenu((NAMenu*)winapiMenu);
}



NA_DEF void naAddMenuItem(NAMenu* menu, NAMenuItem* item, const NAMenuItem* atItem){
  NAWINAPIMenu* winapiMenu = (NAWINAPIMenu*)menu;
  NA_WINAPIMenuItem* winapiMenuItem = (NA_WINAPIMenuItem*)item;

  MENUITEMINFO menuItemInfo;
  naZeron(&menuItemInfo, sizeof(MENUITEMINFO));
  menuItemInfo.cbSize = sizeof(MENUITEMINFO);

  if(winapiMenuItem->isSeparator){
    menuItemInfo.fMask = MIIM_ID | MIIM_FTYPE;
    menuItemInfo.wID = na_GetApplicationNextMenuItemId(naGetApplication());
    menuItemInfo.fType = MFT_SEPARATOR;
  }else{
    // Note for the future, do not combine MIIM_TYPE with MIIM_FTYPE.
    menuItemInfo.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
    menuItemInfo.wID = na_GetApplicationNextMenuItemId(naGetApplication());
    menuItemInfo.fType = MFT_STRING;
    menuItemInfo.dwTypeData = naAllocSystemStringWithUTF8String(naGetStringUTF8Pointer(winapiMenuItem->text));
    menuItemInfo.cch = (UINT)naGetStringByteSize(winapiMenuItem->text);
    menuItemInfo.fState = /*MFS_CHECKED | */MFS_ENABLED/* | MFS_DEFAULT*/;
  }

  na_SetMenuItemId(&(winapiMenuItem->menuItem), menuItemInfo.wID);

  size_t index = naGetMenuItemIndex(menu, atItem);

  InsertMenuItem(
    winapiMenu->hMenu,
    (UINT)index,
    TRUE,
    &menuItemInfo);

  na_AddMenuChild(menu, item, atItem);
}



NA_DEF void naPresentMenu(const NAMenu* menu, NAPos pos, void* parentUIElement){
  NAWINAPIMenu* winapiMenu = (NAWINAPIMenu*)menu;

  HMENU hMenu = CreatePopupMenu();

  na_SetApplicationLastOpenedMenu(naGetApplication(), menu);
  NARect screenRect = naGetMainScreenRect();

  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  // The messages are sent to the parent ui element. See WM_ENTERMENULOOP for
  // example. Currently only works with buttons.

  /*int selection = */TrackPopupMenu(
    winapiMenu->hMenu, 
    TPM_LEFTALIGN | TPM_RIGHTBUTTON/* | TPM_RETURNCMD*/, 
    (int)(pos.x * uiScale),
    (int)(screenRect.size.height - pos.y * uiScale),
    0,
    naGetUIElementNativePtr(parentUIElement),
    NULL);
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
