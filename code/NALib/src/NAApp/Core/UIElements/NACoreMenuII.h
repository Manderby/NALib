
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitMenu(NAMenu* menu, void* nativePtr, NA_UIElement* parent) {
  na_InitCoreUIElement(&menu->uiElement, NA_UI_MENU, nativePtr);
  naInitList(&menu->childs);
  na_SetUIElementParent(&menu->uiElement, parent, NA_FALSE);
}



NA_HDEF void na_ClearMenu(NAMenu* menu) {
  naForeachListMutable(&menu->childs, (NAMutator)naDelete);
  naClearList(&menu->childs);
  // todo
  // na_ClearCoreUIElement(&menu->uiElement);
}



NA_HDEF void na_AddMenuChild(NAMenu* menu, NAMenuItem* child, const NAMenuItem* itemAt) {
  NAListIterator iter = naMakeListModifier(&menu->childs);
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  na_SetUIElementParent(&child->uiElement, menu, NA_FALSE);
}


NA_DEF size_t naGetMenuItemIndex(const NAMenu* menu, const NAMenuItem* item) {
  return naGetListElemIndex(&menu->childs, item);
}



NA_DEF size_t naGetMenuItemCount(const NAMenu* menu) {
  return naGetListCount(&menu->childs);
}



NA_HDEF NARect na_GetMenuRect(const NA_UIElement* menu)
{
  NA_UNUSED(menu);
  return naMakeRectS(0, 0, 1, 1);
}



NA_HDEF void na_SetMenuRect(NA_UIElement* menu, NARect rect) {
  NA_UNUSED(menu);
  NA_UNUSED(rect);
  #if NA_DEBUG
  naError("Menu rects can not be set.");
  #endif
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
