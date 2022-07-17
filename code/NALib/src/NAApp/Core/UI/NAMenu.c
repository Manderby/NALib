
#include "NAAppCore.h"



NA_HDEF void na_InitMenu(NAMenu* menu, void* nativePtr, NA_UIElement* parent){
  na_InitUIElement(&(menu->uiElement), NA_UI_MENU, nativePtr);
  naInitList(&(menu->childs));
  na_SetUIElementParent(&(menu->uiElement), parent, NA_FALSE);
}



NA_HDEF void na_ClearMenu(NAMenu* menu){
  naForeachListMutable(&(menu->childs), (NAMutator)naDelete);
  naClearList(&(menu->childs));
  // todo
  // na_ClearUIElement(&(menu->uiElement));
}



NA_HDEF void na_AddMenuChild(NAMenu* menu, NAMenuItem* child, const NAMenuItem* itemAt){
  NAListIterator iter = naMakeListModifier(&(menu->childs));
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  na_SetUIElementParent(&(child->uiElement), menu, NA_FALSE);
}
