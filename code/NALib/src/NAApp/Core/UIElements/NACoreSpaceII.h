
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitSpace(NASpace* space, void* nativePtr) {
  na_InitCoreUIElement(&space->uiElement, NA_UI_SPACE, nativePtr);
  naInitList(&space->childs);
  space->backgroundColor = naAlloc(NAColor);
  naFillColorWithTransparent(space->backgroundColor);
}



NA_HDEF void na_ClearSpace(NASpace* space) {
  naFree(space->backgroundColor);
  naClearList(&space->childs, (NAMutator)naDelete);
  na_ClearCoreUIElement(&space->uiElement);
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
