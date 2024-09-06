
#ifndef NA_UI_METAL_SPACE_INCLUDED
#define NA_UI_METAL_SPACE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// An NAMetalSpace can react to the following commands:
//
// NA_UI_COMMAND_REDRAW        When the content shall be redrawn
// NA_UI_COMMAND_MOUSE_DOWN    When a mouse button has been pressed
// NA_UI_COMMAND_MOUSE_UP      When a mouse button has been released
// NA_UI_COMMAND_MOUSE_MOVED   When the mouse moved within the space
// NA_UI_COMMAND_TRANSFORMED   When scrolled, zoomed, rotated
// NA_UI_COMMAND_KEY_DOWN      When a key has been pressed
// NA_UI_COMMAND_KEY_UP        When a key has been released



// Note that you must have NA_COMPILE_METAL configured in NAConfiguration.h
// to use these functions. Also note that Metal is macOS only.
#if(NA_COMPILE_METAL == 1)
  #if NA_OS != NA_OS_MAC_OS_X
    #error "Metal is a macOS only framework. Does not work on windows."
  #endif
#endif



// Creates a new metalSpace
NA_API NAMetalSpace* naNewMetalSpace(NASize size);

// Returns true, if the given metalSpace has allocated a valid Metal structure.
NA_API NABool naIsMetalTechnologyAvailable(NAMetalSpace* metalSpace);

// Changes the visibility of the metal space.
NA_API void naSetMetalSpaceVisible(NAMetalSpace* metalSpace, NABool visible);

// Returns the system specific context (macOS: MTLLayer*)
NA_API void* naGetMetalSpaceSystemContext(const NAMetalSpace* metalSpace);

// Sets the inner bounds of the space.
NA_API void naSetMetalSpaceInnerRect(NAMetalSpace* metalSpace, NARect bounds);

// Sets the transformation of the space to the depicted delta values.
NA_API void naSetMetalSpaceTranslation(
  NAMetalSpace* metalSpace,
  double dx,
  double dy);

// Sets the magnification of the space.
NA_API void naSetMetalSpaceMagnification(
  NAMetalSpace* metalSpace,
  double magnification);

// Returns the desired transformation of the space. The returned matrix is a
// homogeneous 3x3 matrix which denotes the transformations since the last
// reset.
NA_API const double* naGetMetalSpaceTransformation(const NAMetalSpace* metalSpace);

// Resets the transformation to a unity matrix. Will be called automatically
// after reacting to NA_UI_COMMAND_TRANSFORMED.
NA_API void naResetMetalSpaceTransformation(NAMetalSpace* metalSpace);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_METAL_SPACE_INCLUDED



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
