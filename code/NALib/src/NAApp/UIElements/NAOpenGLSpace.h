
#if defined NA_UI_OPENGL_SPACE_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_OPENGL_SPACE_INCLUDED
#define NA_UI_OPENGL_SPACE_INCLUDED



// An NAOpenGLSpace can react to the following commands:
//
// NA_UI_COMMAND_REDRAW        When the content shall be redrawn
// NA_UI_COMMAND_MOUSE_DOWN    When a mouse button has been pressed
// NA_UI_COMMAND_MOUSE_UP      When a mouse button has been released
// NA_UI_COMMAND_MOUSE_MOVED   When the mouse moved within the space
// NA_UI_COMMAND_TRANSFORMED   When scrolled, zoomed, rotated
// NA_UI_COMMAND_KEY_DOWN      When a key has been pressed
// NA_UI_COMMAND_KEY_UP        When a key has been released



// Note that you must have NA_COMPILE_OPENGL configured in NAConfiguration.h
// to use these functions.
#if(NA_COMPILE_OPENGL == 1)
  #if NA_OS == NA_OS_WINDOWS
    #include <windows.h>
    #include <GL/GL.h>
  #else
    #undef GL_SILENCE_DEPRECATION
    #define GL_SILENCE_DEPRECATION
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
//NA_API void naSetOpenGLSpaceInnerRect(
//  NAOpenGLSpace* openGLSpace,
//  NARect bounds);

// In order to draw characters with naDrawASCIICharacters, you first need to
// startup a pixelfont. The startup function must be called after the openGL
// context is set. The best place to call it is in the callback of the
// naNewOpenGLSpace function or, if naNewOpenGLSpace is not used, the
// prepareOpenGL method of Cocoa.
// Upon startup, you get an id which you need to store for later. When the
// pixel font is no longer needed, you can shut it down again with the id.
NA_API size_t naStartupPixelFont(void);
NA_API void naShutdownPixelFont(size_t fontId);

// Draws a series of characters at the specified world coordinates. Requires
// naStartupPixelFont to be called somewhen in advance. The font used is a
// monospaced font, 5x9 pixels in size. An additional 1 pixel will be used when
// multiple characters are placed, both horizontally as well as vertically.
// The character '\n' is treated as a newline, all other non-printable ASCII
// codes are overjumped. The given coordinates are the bottom left corner of
// the first drawn character.
NA_API void naDrawASCIICharacters(
  size_t fontId,
  const char* str,
  double x,
  double y,
  double z);


// Sets the transformation of the space to the depicted delta values.
NA_API void naSetOpenGLSpaceTranslation(
  NAOpenGLSpace* openGLSpace,
  double dx,
  double dy);
  
// Sets the magnification of the space.
NA_API void naSetOpenGLSpaceMagnification(
  NAOpenGLSpace* openGLSpace,
  double magnification);

// Returns the desired transformation of the space. The returned matrix is a
// homogeneous 3x3 matrix which denotes the transformations since the last
// reset.
NA_API const double* naGetOpenGLSpaceTransformation(const NAOpenGLSpace* openGLSpace);

// Resets the transformation to a unity matrix. Will be called automatically
// after reacting to NA_UI_COMMAND_TRANSFORMED.
NA_API void naResetOpenGLSpaceTransformation(NAOpenGLSpace* openGLSpace);



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
