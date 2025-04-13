
#ifndef NA_MOUSE_INCLUDED
#define NA_MOUSE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



#include "../NAMath/NACoord.h"
NA_PROTOTYPE(NAImageSet);



typedef enum NAMouseButton {
  NA_MOUSE_BUTTON_LEFT = 0,
  NA_MOUSE_BUTTON_RIGHT = 1,
  NA_MOUSE_BUTTON_MIDDLE = 2,
  // additional mouse buttons have higher integer numbers are not implemented
  // explicitely.
} NAMouseButton;



// ////////////////////////////////////
// Mouse status
// ////////////////////////////////////

// A mouse status stores various position data of the mouse.
NA_PROTOTYPE(NAMouseStatus);

// Returns the current mouse status.
NA_API const NAMouseStatus* naGetCurrentMouseStatus(void);

// Returns whether the mouse button is pressed.
NA_API NABool naGetMouseButtonPressed(
  const NAMouseStatus* mouseStatus,
  NAMouseButton button);

// Returns the position of the mouse status.
NA_API NAPos naGetMousePos(const NAMouseStatus* mouseStatus);

// Returns the difference between the current and last recorded position.
NA_API NAPos naGetMouseDelta(const NAMouseStatus* mouseStatus);

// Centers the mouse in the given uiElement.
NA_API void naCenterMouse(void* uiElement);

// Shows or hides the mouse
NA_API void naShowMouse(void);
NA_API void naHideMouse(void);
NA_API void naHideMouseUntilMovement(NABool hide);



// ////////////////////////////////////
// Mouse cursor
// ////////////////////////////////////

// The actual type is system dependent.
// macOS: NSCursor
typedef void NACursorImage;

// Creates a resolution independent image for a cursor with a hotspot.
NA_API NACursorImage* naAllocCursorImage(const NAImageSet* imageSet, NAPos hotspot);
NA_API void naDeallocCursorImage(NACursorImage* image);

// Sets the current cursor image to the image provided. If null is sent, the
// default cursor is set.
NA_API void naActivateCursorImage(const NACursorImage* image);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_MOUSE_INCLUDED



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
