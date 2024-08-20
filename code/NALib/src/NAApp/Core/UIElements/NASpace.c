
#include "../../NAApp.h"

#if NA_COMPILE_GUI == 1

#include "../NAAppCore.h"
#include "../../../NAVisual/NAColor.h"



NA_HDEF void na_InitSpace(NASpace* space, void* nativePtr) {
  na_InitUIElement(&space->uiElement, NA_UI_SPACE, nativePtr);
  naInitList(&space->childs);
  space->backgroundColor = naAlloc(NAColor);
  naFillColorWithTransparent(space->backgroundColor);
}



NA_HDEF void na_ClearSpace(NASpace* space) {
  naFree(space->backgroundColor);
  naForeachListMutable(&space->childs, (NAMutator)naDelete);
  naClearList(&space->childs);
  na_ClearUIElement(&space->uiElement);
}



NA_HDEF void na_AddSpaceChild(NASpace* space, NA_UIElement* child) {
  naAddListLastMutable(&space->childs, child);
  na_SetUIElementParent(child, space, NA_TRUE);
}



NA_HDEF void na_RemoveSpaceChild(NASpace* space, NA_UIElement* child) {
  naRemoveListData(&space->childs, child);
  na_SetUIElementParent(child, NA_NULL, NA_TRUE);
}



NA_DEF NABool naGetSpaceAlternateBackground(const NASpace* space) {
  return space->alternateBackground;
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate) {
  space->alternateBackground = alternate;
  naRefreshUIElement(space, 0.);
}



#endif // NA_COMPILE_GUI == 1
