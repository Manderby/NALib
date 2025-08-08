
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitSpace(NASpace* space, void* nativePtr) {
  #if NA_DEBUG
    if(!space)
      naCrash("space is nullptr");
  #endif

  na_InitCoreUIElement(&space->uiElement, NA_UI_SPACE, nativePtr);
  naInitList(&space->childs);
  space->backgroundColor = NA_NULL;
}



NA_HDEF void na_ClearSpace(NASpace* space) {
  #if NA_DEBUG
    if(!space)
      naCrash("space is nullptr");
  #endif

  if(space->backgroundColor) { naFree(space->backgroundColor); }
  naClearList(&space->childs, (NAMutator)naDelete);
  na_ClearCoreUIElement(&space->uiElement);
}



NA_HDEF void na_AddSpaceChild(NASpace* space, NA_UIElement* child) {
  #if NA_DEBUG
    if(!space)
      naCrash("space is nullptr");
  #endif

  naAddListLastMutable(&space->childs, child);
  na_SetUIElementParent(child, space, NA_TRUE);
}



NA_HDEF void na_RemoveSpaceChild(NASpace* space, NA_UIElement* child) {
  #if NA_DEBUG
    if(!space)
      naCrash("space is nullptr");
  #endif

  naRemoveListData(&space->childs, child);
  na_SetUIElementParent(child, NA_NULL, NA_TRUE);
}



NA_DEF NABool naGetSpaceAlternateBackground(const NASpace* space) {
  #if NA_DEBUG
    if(!space)
      naCrash("space is nullptr");
  #endif

  return space->alternateBackground;
}



NA_HDEF void na_SetSpaceBackgroundColor(NASpace* space, const NAColor* color) {
  #if NA_DEBUG
    if(!space)
      naCrash("space is nullptr");
  #endif

  if(space->backgroundColor) { naFree(space->backgroundColor); }
  if(color) {
    space->backgroundColor = naAlloc(NAColor);
    naFillColorWithCopy(space->backgroundColor, color);
  }else{
    space->backgroundColor = NA_NULL;
  }
}



NA_DEF void naFillSpaceBackgroundColor(NAColor* color, const NASpace* space) {
  #if NA_DEBUG
    if(!space)
      naCrash("space is nullptr");
  #endif

  NAColor parentBgColor;
  const NASpace* parentSpace = naGetUIElementParentSpace(space);
  if(parentSpace) {
    naFillSpaceBackgroundColor(&parentBgColor, parentSpace);
  }else{
    naFillColorWithDefaultWindowBackgroundColor(&parentBgColor);
  }

  NAColor thisBgColor;
  if(space->backgroundColor) {
    naFillColorWithCopy(&thisBgColor, space->backgroundColor);
  }else{
    naFillColorWithTransparent(&thisBgColor);
  }

  NAColor fgColor;
  if(naGetSpaceAlternateBackground(space)) {
    naFillColorWithDefaultTextColor(&fgColor);
  }else{
    naFillColorWithTransparent(&fgColor);
  }

  NAColor alternatedColor;
  naBlendColors(&alternatedColor, &thisBgColor, &fgColor, 0.075f, NA_BLEND_OVERLAY, 1, NA_FALSE, NA_FALSE);
  naBlendColors(color, &parentBgColor, &alternatedColor, 1.f, NA_BLEND_OVERLAY, 1, NA_FALSE, NA_FALSE);
}



NA_DEF NABool naGetSpaceDragsWindow(const NASpace* space) {
  return space->dragsWindow;
}

NA_DEF void naSetSpaceDragsWindow(NASpace* space, NABool dragsWindow) {
  space->dragsWindow = dragsWindow;
}



NA_HDEF void na_UpdateSpaceUIScale(NA_UIElement* space) {
  NASpace* naSpace = (NASpace*)space;
  
  NAListIterator it = naMakeListMutator(&naSpace->childs);
  while(naIterateList(&it)) {
    NA_UIElement* uiElement = naGetListCurMutable(&it);
    na_UpdateUIElementUIScale(uiElement);
  }
  naClearListIterator(&it);
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
