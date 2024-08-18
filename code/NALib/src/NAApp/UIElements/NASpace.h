
#if defined NA_UI_SPACE_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_SPACE_INCLUDED
#define NA_UI_SPACE_INCLUDED



// An NASpace can react to the following commands:
//
// NA_UI_COMMAND_KEY_DOWN      When a key has been pressed
// NA_UI_COMMAND_KEY_UP        When a key has been released
// NA_UI_COMMAND_MOUSE_DOWN    When a mouse button has been pressed
// NA_UI_COMMAND_MOUSE_UP      When a mouse button has been released
// NA_UI_COMMAND_MOUSE_MOVED   When the mouse moved within the space
// NA_UI_COMMAND_MOUSE_ENTERED When the mouse entered the space
// NA_UI_COMMAND_MOUSE_EXITED  When the mouse exited the space



// Creates a new Space
// (In other frameworks called View, Frame, Area, Widget...)
NA_API NASpace* naNewSpace(NASize size);

// Adds a child ui element at the given position.
// Beware that adding a child to a space which formerly was the first responder
// of a window, that windows first responder will be reset to Null.
//
// Usually, UI-elementes are placed in a line-by-line fashion, usually asking
// the user to read from top to bottom. Therefore, it is good to know the
// vertical alignment of all UI-elements.
//
// The tallest items are 24 points in height. But the visual representation is
// often times shifted in vertical position such that some elements are even
// overlapping when displayed line by line. The author recommends a line height
// of 24 points.
NA_API void naAddSpaceChild(NASpace* space, void* child, NAPos pos);

// Removes the given child from the space. If the child is not a child of the
// space, nothing happens. NALib will output a warning though.
NA_API void naRemoveSpaceChild(NASpace* space, void* child);

// Removes all children of a space
NA_API void naRemoveAllSpaceChilds(NASpace* space);

// Switch between clear or an alternate color which adds a slight gray to the
// background.
NA_API NABool naGetSpaceAlternateBackground(const NASpace* space);
NA_API void naSetSpaceAlternateBackground(NASpace* space, NABool alternate);

// Set a specific background color.
NA_API void naSetSpaceBackgroundColor(
  NASpace* space,
  const NAColor* color);

// Moves all childs with the given x and y coordinates in direction
// to the right and top.
NA_API void naShiftSpaceChilds(NASpace* space, NAPos shift);

// Changes the visibility of the space.
NA_API void naSetSpaceVisible(
  NASpace* ospace,
  NABool visible);

// Makes the space react to mouse drags in order to move the window.
NA_API void naSetSpaceDragsWindow(NASpace* space, NABool isDraggable);



#endif // NA_UI_SPACE_INCLUDED



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
