
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naPopupButtonWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){

  default:
    printf("Uncaught Button message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF NAPopupButton* naNewPopupButton(double width){
  NAWINAPIPopupButton* winapiPopupButton = naNew(NAWINAPIPopupButton);

  //winapiPopupButton->hMenu = CreatePopupMenu();

  ////MENUINFO menuInfo;
  ////naZeron(&menuInfo, sizeof(MENUINFO));
  ////menuInfo.cbSize = sizeof(MENUINFO);
  ////menuInfo.fMask = MIM_STYLE;
  ////menuInfo.dwStyle = MNS_NOCHECK;
  ////SetMenuInfo(winapiPopupButton->hMenu, &menuInfo);

  //na_InitMenu(&(winapiPopupButton->menu), winapiPopupButton->hMenu, parent);

  return (NAPopupButton*)winapiPopupButton;
}

NA_DEF void na_DestructWINAPIPopupButton(NAWINAPIPopupButton* winapiPopupButton){
  na_ClearPopupButton((NAPopupButton*)winapiPopupButton);
}

NA_DEF void naSetPopupButtonVisible(NAPopupButton* popupButton, NABool visible){
  // todo
}

NA_DEF void naSetPopupButtonEnabled(NAPopupButton* popupButton, NABool enabled){
  // todo
}

NA_DEF void naAddPopupButtonMenuItem(NAPopupButton* popupButton, NAMenuItem* item, const NAMenuItem* atItem){
  // todo
}

NA_DEF size_t naGetPopupButtonItemIndex(NAPopupButton* popupButton, NAMenuItem* item){
  // todo
  return 0;
}

NA_DEF void naSetPopupButtonItemSelected(NAPopupButton* popupButton, NAMenuItem* item){
  // todo
}

NA_HDEF NARect na_GetPopupButtonAbsoluteInnerRect(const NA_UIElement* menu){
  NA_UNUSED(menu);
  return naMakeRectS(0, 0, 1, 1);
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
