
#if defined NA_MOUSE_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_MOUSE_INCLUDED
#define NA_MOUSE_INCLUDED



// A mouse status currently consists of the current position and the last one
// recorded.
typedef struct NAMouseStatus NAMouseStatus;
struct NAMouseStatus{
  NAPos pos;
  NAPos prevPos;
  NABool leftPressed;
};

typedef void* NACursorImage;



// Returns the current mouse status.
NA_API const NAMouseStatus* naGetMouseStatus(void);

// Returns the position of the mouse status.
NA_API NAPos naGetMousePos(const NAMouseStatus* mousestatus);

// Returns the difference between the current and last recorded position.
NA_API NASize naGetMouseDelta(const NAMouseStatus* mousestatus);

// Centers the mouse in the given uiElement.
NA_API void naCenterMouse(void* uiElement);

// Shows or hides the mouse
NA_API void naShowMouse(void);
NA_API void naHideMouse(void);
NA_API void naHideMouseUntilMovement(NABool hide);

// Creates a resolution independent image for a cursor with a hotspot.
// On macOS, NACursorImage is of type NSCursor*
NA_API NACursorImage naAllocCursorImage(const NAUIImage* uiImage, NAPos hotspot);

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
