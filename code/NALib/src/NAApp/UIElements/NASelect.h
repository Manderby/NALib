
#ifndef NA_UI_SELECT_INCLUDED
#define NA_UI_SELECT_INCLUDED



// An NASelect can does not react directly. Instead, the NAMenuItems attached
// to the select will perform actions.



// Creates a new select (also called popup button, dropdown menu etc.).
// Default height is 23.
NA_API NASelect* naNewSelect(double width);

// Changes the visibility or enabled state of the select.
NA_API void naSetSelectVisible(
  NASelect* select,
  NABool visible);
NA_API void naSetSelectEnabled(
  NASelect* select,
  NABool enabled);

// Adds a menu item before atItem. If atItem is Null,
// it is added to the end of the menu.
NA_API void naAddSelectMenuItem(
  NASelect* select,
  NAMenuItem* item,
  const NAMenuItem* atItem);

// Returns the index of the given item. If not found, equals the item count of
// the menu. 
NA_API size_t naGetSelectItemIndex(
  const NASelect* select,
  const NAMenuItem* item);

// returns the number of items in the select menu.
NA_API size_t naGetSelectItemCount(const NASelect* select);

NA_API const NAMenuItem* naGetSelectItem(
  const NASelect* select,
  size_t index);

// Sets which item is presented as the selected item.
NA_API void naSetSelectIndexSelected(
  NASelect* select,
  size_t index);
NA_API void naSetSelectItemSelected(
  NASelect* select,
  const NAMenuItem* item);



#endif // NA_UI_SELECT_INCLUDED



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
