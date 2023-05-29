
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitPopupButton(NAPopupButton* popupButton, void* nativePtr){
  na_InitUIElement(&(popupButton->uiElement), NA_UI_POPUP_BUTTON, nativePtr);
  naInitList(&(popupButton->childs));
}



NA_HDEF void na_ClearPopupButton(NAPopupButton* popupButton){
  naForeachListMutable(&(popupButton->childs), (NAMutator)naDelete);
  naClearList(&(popupButton->childs));
  na_ClearUIElement(&(popupButton->uiElement));
}



NA_DEF size_t naGetPopupButtonItemIndex(NAPopupButton* popupButton, const NAMenuItem* item){
  return naGetListElemIndex(&(popupButton->childs), item);
}



NA_DEF size_t naGetPopupButtonItemCount(NAPopupButton* popupButton){
  return naGetListCount(&(popupButton->childs));
}



NA_DEF const NAMenuItem* naGetPopupButtonItem(const NAPopupButton* popupButton, size_t index){
  NAListIterator iter = naMakeListAccessor(&(popupButton->childs));
  naLocateListIndex(&iter, index);
  const NAMenuItem* item = naGetListCurConst(&iter);
  naClearListIterator(&iter);
  return item;
}



NA_HDEF void na_AddPopupButtonChild(NAPopupButton* popupButton, NAMenuItem* child, const NAMenuItem* itemAt){
  NAListIterator iter = naMakeListModifier(&(popupButton->childs));
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  // todo
  //naAddListLastMutable(&(menu->childs), child);
  na_SetUIElementParent(&(child->uiElement), popupButton, NA_FALSE);
}



#endif // NA_COMPILE_GUI == 1
