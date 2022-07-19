
#if defined NA_UI_OPENGL_SPACE_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_OPENGL_SPACE_INCLUDED
#define NA_UI_OPENGL_SPACE_INCLUDED



// Note that you must have NA_COMPILE_OPENGL configured in NAConfiguration.h
// to use these functions.
#if (NA_COMPILE_OPENGL == 1)
  #if NA_OS == NA_OS_WINDOWS
    #include <windows.h>
    #include <GL/GL.h>
  #else
    #include <OpenGL/gl.h>
  #endif
#endif



// Creates a new openGLSpace
// Use initFunc to perform any initialization necessary like for example
// uploading of textures to the GPU. The initFunc will be called with
// initData as the input parameter. The initFunc can be Null.
// Note that the initFunc will be called...
// Win: Right within the naNewOpenGLSpace
// Mac: when prepareOpenGL is called (which may be as late as when the
//      space comes onsceen)
NA_API NAOpenGLSpace* naNewOpenGLSpace(
  NASize size,
  NAMutator initFunc,
  void* initData);

// Changes the visibility of the openGL space.
NA_API void naSetOpenGLSpaceVisible(
  NAOpenGLSpace* openGLSpace,
  NABool visible);

// Returns the system specific context (macOS: NSOpenGLContext*)
NA_API void* naGetOpenGLSpaceSystemContext(
  const NAOpenGLSpace* openGLSpace);

// Swaps the buffers (double buffer is turned on by default)
NA_API void naSwapOpenGLSpaceBuffer(
  NAOpenGLSpace* openGLSpace);

// Sets the inner bounds of the space.
NA_API void naSetOpenGLSpaceInnerRect(
  NAOpenGLSpace* openGLSpace,
  NARect bounds);



#endif // NA_UI_OPENGL_SPACE_INCLUDED



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