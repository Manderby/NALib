
#if defined NA_UI_POPUP_BUTTON_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_POPUP_BUTTON_INCLUDED
#define NA_UI_POPUP_BUTTON_INCLUDED



// Creates a new PopupButton. Default height is 23.
NA_API NAPopupButton* naNewPopupButton(double width);

// Changes the visibility or enabled state of the button.
NA_API void naSetPopupButtonVisible(
  NAPopupButton* popupButton,
  NABool visible);
NA_API void naSetPopupButtonEnabled(
  NAPopupButton* popupButton,
  NABool enabled);

// naAddPopupButtonMenuItem adds a menu item before atItem. If atItem is Null,
// it is added to the end of the menu.
NA_API void naAddPopupButtonMenuItem(
  NAPopupButton* popupButton,
  NAMenuItem* item,
  const NAMenuItem* atItem);

// naGetPopupButtonItemIndex returns the index including all separators.
NA_API size_t naGetPopupButtonItemIndex(
  NAPopupButton* popupButton,
  const NAMenuItem* item);

// Sets which item is presented as the selected item.
NA_API void naSetPopupButtonIndexSelected(
  NAPopupButton* popupButton,
  size_t index);



#endif // NA_UI_POPUP_BUTTON_INCLUDED



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
