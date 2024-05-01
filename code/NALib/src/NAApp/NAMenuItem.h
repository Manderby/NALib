
#if defined NA_MENUITEM_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_MENUITEM_INCLUDED
#define NA_MENUITEM_INCLUDED



// Creates a new menuItem with a text entry
NA_API NAMenuItem* naNewMenuItem(const NAUTF8Char* text);

// Creates a new menuItem which is a separator.
NA_API NAMenuItem* naNewMenuSeparator(void);

// Returns the text stored in the menu item. Return Null for a separator.
NA_API const NAUTF8Char* naGetMenuItemText(NAMenuItem* menuItem);

// Sets the text of a given menu item. Also works for submenus.
// The nativePtr must be of type NSMenuItem.
NA_API void naSetMenuItemText(NAMenuItem* menuItem, const NAUTF8Char* text);

// Adds a checkbox in front of the entry if state is NA_TRUE.
NA_API void naSetMenuItemState(NAMenuItem* menuItem, NABool state);


#endif // NA_MENUITEM_INCLUDED



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
