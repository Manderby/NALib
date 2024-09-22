
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitSelect(NASelect* select, void* nativePtr) {
  na_InitCoreUIElement(&select->uiElement, NA_UI_SELECT, nativePtr);
  naInitList(&select->childs);
}



NA_HDEF void na_ClearSelect(NASelect* select) {
  naForeachListMutable(&select->childs, (NAMutator)naDelete);
  naClearList(&select->childs);
  na_ClearCoreUIElement(&select->uiElement);
}



NA_DEF size_t naGetSelectItemIndex(const NASelect* select, const NAMenuItem* item) {
  return naGetListElemIndex(&select->childs, item);
}



NA_DEF size_t naGetSelectItemCount(const NASelect* select) {
  return naGetListCount(&select->childs);
}



NA_DEF const NAMenuItem* naGetSelectItem(const NASelect* select, size_t index) {
  NAListIterator iter = naMakeListAccessor(&select->childs);
  naLocateListIndex(&iter, index);
  const NAMenuItem* item = naGetListCurConst(&iter);
  naClearListIterator(&iter);
  return item;
}



NA_HDEF void na_AddSelectChild(NASelect* select, NAMenuItem* child, const NAMenuItem* itemAt) {
  NAListIterator iter = naMakeListModifier(&select->childs);
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  // todo
  //naAddListLastMutable(&menu->childs, child);
  na_SetUIElementParent(&child->uiElement, select, NA_FALSE);
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
