
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitSelect(NASelect* select, void* nativePtr) {
  na_InitUIElement(&(select->uiElement), NA_UI_SELECT, nativePtr);
  naInitList(&(select->childs));
}



NA_HDEF void na_ClearSelect(NASelect* select) {
  naForeachListMutable(&(select->childs), (NAMutator)naDelete);
  naClearList(&(select->childs));
  na_ClearUIElement(&(select->uiElement));
}



NA_DEF size_t naGetSelectItemIndex(const NASelect* select, const NAMenuItem* item) {
  return naGetListElemIndex(&(select->childs), item);
}



NA_DEF size_t naGetSelectItemCount(const NASelect* select) {
  return naGetListCount(&(select->childs));
}



NA_DEF const NAMenuItem* naGetSelectItem(const NASelect* select, size_t index) {
  NAListIterator iter = naMakeListAccessor(&(select->childs));
  naLocateListIndex(&iter, index);
  const NAMenuItem* item = naGetListCurConst(&iter);
  naClearListIterator(&iter);
  return item;
}



NA_HDEF void na_AddSelectChild(NASelect* select, NAMenuItem* child, const NAMenuItem* itemAt) {
  NAListIterator iter = naMakeListModifier(&(select->childs));
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  // todo
  //naAddListLastMutable(&(menu->childs), child);
  na_SetUIElementParent(&(child->uiElement), select, NA_FALSE);
}



#endif // NA_COMPILE_GUI == 1
