
#if defined NA_MENU_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_MENU_INCLUDED
#define NA_MENU_INCLUDED



// Creates a new menu
NA_API NAMenu* naNewMenu(void);

// Sdds a menu item before atItem. If atItem is Null, it is added at the end.
// Note that item must be mutable.
NA_API void naAddMenuItem(NAMenu* menu, NAMenuItem* item, const NAMenuItem* atItem);

// Returns the index of the given item including all separators.
NA_API size_t naGetMenuItemIndex(const NAMenu* menu, const NAMenuItem* item);

// Presents the menu onscreen at the dedicated absolute position.
// You need to provide a parent UIElement. Currently must be a button.
NA_API void naPresentMenu(NAMenu* menu, NAPos pos, void* parentUIElement);



#endif // NA_MENU_INCLUDED



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
