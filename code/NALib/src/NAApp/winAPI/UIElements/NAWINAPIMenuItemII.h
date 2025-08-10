
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NA_DEF NAMenuItem* naNewMenuItem(const NAUTF8Char* text, NAKeyStroke* keyStroke) {
  NA_WINAPIMenuItem* winapiMenuItem = naNew(NA_WINAPIMenuItem);

  winapiMenuItem->isSeparator = NA_FALSE;
  winapiMenuItem->text = naNewStringWithFormat("%s", text);
  winapiMenuItem->state = NA_FALSE;

  na_InitMenuItem(&winapiMenuItem->menuItem, winapiMenuItem, NA_NULL);

  return (NAMenuItem*)winapiMenuItem;
}



NA_DEF NAMenuItem* naNewMenuSeparator() {
  NA_WINAPIMenuItem* winapiMenuItem = naNew(NA_WINAPIMenuItem);

  winapiMenuItem->isSeparator = NA_TRUE;
  winapiMenuItem->text = NA_NULL;

  na_InitMenuItem(&winapiMenuItem->menuItem, winapiMenuItem, NA_NULL);

  return (NAMenuItem*)winapiMenuItem;
}

NA_DEF NAMenuItem* naNewSubmenuItem(const NAUTF8Char* text, NAMenu* subMenu) {
  return NA_NULL;
}


NA_DEF void na_DestructWINAPIMenuItem(NA_WINAPIMenuItem* winapiMenuItem) {
  if(winapiMenuItem->text) {
    naDelete(winapiMenuItem->text);
  }
  na_ClearMenuItem((NAMenuItem*)winapiMenuItem);
}



NA_DEF const NAUTF8Char* naGetMenuItemText(NAMenuItem* menuItem) {
  NA_WINAPIMenuItem* winapiMenuItem = (NA_WINAPIMenuItem*)menuItem;
  return naGetStringUTF8Pointer(winapiMenuItem->text);
}



NA_DEF void naSetMenuItemText(NAMenuItem* menuItem, const NAUTF8Char* text) {
  NA_UNUSED(text);
  NA_UNUSED(menuItem);
  // todo
  //NA_WINAPIMenuItem* winapiMenuItem = (NA_WINAPIMenuItem*)menuItem;
  //naDelete(winapiMenuItem->text);
  //winapiMenuItem->text = naNewStringWithFormat("%s", text);
  //na_updateMenuItem(winapiMenuItem->menu, &winapiMenuItem->menuItem);
}



NA_DEF void naSetMenuItemState(NAMenuItem* menuItem, NABool state) {
  NA_WINAPIMenuItem* winapiMenuItem = (NA_WINAPIMenuItem*)menuItem;
  winapiMenuItem->state = state;
  na_updateMenuItem(winapiMenuItem->menu, &winapiMenuItem->menuItem);
}



NA_HDEF void na_SetMenuItemId(NAMenuItem* menuItem, uint32 id, NAMenu* menu) {
  NA_WINAPIMenuItem* winapiMenuItem = (NA_WINAPIMenuItem*)menuItem;
  winapiMenuItem->id = id;
  winapiMenuItem->menu = menu;
}



NA_HDEF uint32 na_GetMenuItemId(const NAMenuItem* menuItem) {
  NA_WINAPIMenuItem* winapiMenuItem = (NA_WINAPIMenuItem*)menuItem;
  return winapiMenuItem->id;
}



NA_DEF NABool naGetMenuItemIsSeparator(const NAMenuItem* menuItem) {
  NA_WINAPIMenuItem* winapiMenuItem = (NA_WINAPIMenuItem*)menuItem;
  return winapiMenuItem->isSeparator;
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
