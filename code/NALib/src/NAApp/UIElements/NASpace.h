
#ifndef NA_UI_SPACE_INCLUDED
#define NA_UI_SPACE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



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
// The default function will automatically offset the uiElements such that
// they are aligned to a common text baseline. If you want full control over
// positioning, use the Raw variant.
NA_API void naAddSpaceChild(NASpace* space, void* child, NAPos pos);
NA_API void naAddSpaceChildRaw(NASpace* space, void* child, NAPos pos);
// If you want to change the size at the same time, use the following two
// functions:
NA_API void naAddSpaceChildWithSize(NASpace* space, void* child, NAPos pos, NASize size);
NA_API void naAddSpaceChildRawWithSize(NASpace* space, void* child, NAPos pos, NASize size);

// Removes the given child from the space. If the child is not a child of the
// space, nothing happens. NALib will output a warning though.
NA_API void naRemoveSpaceChild(NASpace* space, void* child);

// Removes all children of a space
NA_API void naRemoveAllSpaceChilds(NASpace* space);

// Switch between clear or an alternate color which adds a slight gray to the
// background.
NA_API NABool naGetSpaceAlternateBackground(const NASpace* space);
NA_API void naSetSpaceAlternateBackground(NASpace* space, NABool alternate);

// Set a specific background color. (copies the contents of color)
NA_API void naSetSpaceBackgroundColor(
  NASpace* space,
  const NAColor* color);
// Fills the given color with the background acutally used for displaying.
NA_API void naFillSpaceBackgroundColor(NAColor* color, const NASpace* space);

// Moves all childs with the given x and y coordinates in direction
// to the right and top.
NA_API void naShiftSpaceChilds(NASpace* space, NAPos shift);

// Changes the visibility of the space.
NA_API void naSetSpaceVisible(
  NASpace* space,
  NABool visible);

// Makes the space react to mouse drags in order to move the window.
NA_API NABool naGetSpaceDragsWindow(const NASpace* space);
NA_API void naSetSpaceDragsWindow(NASpace* space, NABool dragsWindow);



// Layouting

typedef enum {
  NA_LAYOUT_DIRECTIONS_LRTB = 0x00,  // reading direction: western
  NA_LAYOUT_DIRECTIONS_RLTB = 0x01,  // reading direction: middle eastern
  NA_LAYOUT_DIRECTIONS_LRBT = 0x02,
  NA_LAYOUT_DIRECTIONS_RLBT = 0x03,

  NA_LAYOUT_DIRECTIONS_TBLR = 0x04, // reading direction: mongolian
  NA_LAYOUT_DIRECTIONS_TBRL = 0x05, // reading direction: traditional eastern
  NA_LAYOUT_DIRECTIONS_BTLR = 0x06,
  NA_LAYOUT_DIRECTIONS_BTRL = 0x07,

  NA_LAYOUT_DIRECTIONS_INHERIT = 0x80, // same as the parent space.
} NALayoutDirections;

// Gets and sets the layout directions for this space.
// Note that the default getter automatically returns an explicit direction and
// hence resolves any inherit direction. If there is no explicit direction or
// space is a nullptr, NA_LAYOUT_DIRECTIONS_RLTB is returned.
// If you need to know whether a space uses inherit, use the raw function
NA_API NALayoutDirections naGetSpaceLayoutDirections(const NASpace* space);
NA_API NALayoutDirections naGetSpaceLayoutDirectionsRaw(const NASpace* space);
NA_API void naSetSpaceLayoutDirections(NASpace* space, NALayoutDirections directions);

// The following getters are convencience functions to determine characteristic
// properties of the directions defined by the space. Usually, this is only
// used internally in NALib but it might be of use for somebody.
NABool naGetSpaceLayoutDirectionsHorizontalIsRightToLeft(const NASpace* space);
NABool naGetSpaceLayoutDirectionsVerticalIsBottomToTop(const NASpace* space);
NABool naGetSpaceLayoutDirectionsPrimaryIsVertical(const NASpace* space);



#ifdef __cplusplus
  } // extern "C"
#endif
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
