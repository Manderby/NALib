
#include "NAAppCore.h"



NA_HDEF void na_InitPopupButton(NAPopupButton* popupButton, void* nativePtr){
  na_InitUIElement(&(popupButton->uiElement), NA_UI_POPUP_BUTTON, nativePtr);
  naInitList(&(popupButton->childs));
}



NA_HDEF void na_ClearPopupButton(NAPopupButton* popupButton){
  naForeachListMutable(&(popupButton->childs), (NAMutator)naDelete);
  naClearList(&(popupButton->childs));
  na_ClearUIElement(&(popupButton->uiElement));
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
