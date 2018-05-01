
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "NAUICoreAPI.h"

#if (NA_CONFIG_COMPILE_GUI == 1)
#if NA_OS == NA_OS_WINDOWS


//#include <windows.h>
//#ifdef __gl_h_
//  #include <GL/GL.h>
//#endif

#define CUB_WINDOW_IGNORE_MOUSE_WARP  0x01



// The struct NAWINAPIApplication stores a list of timers which could otherwise
// not be done.
typedef struct NAWINAPIApplication NAWINAPIApplication;
struct NAWINAPIApplication {
  NACoreApplication coreapp;
  NAList timers;
};



// The following struct stores all relevant data which will then be stored in
// a list of the running NAWINAPIApplication.
typedef struct NATimerStruct NATimerStruct;
struct NATimerStruct {
  UINT key;
  NAMutator func;
  void* arg;
};


typedef struct NAWINAPIWindow NAWINAPIWindow;
struct NAWINAPIWindow {
  NACoreWindow corewindow;
  uint32 flags;
  NAUInt trackingcount;
  NABool fullscreen;
  NARect windowedframe;
  NASize size;
  NABounds4 bounds;
};


typedef struct NAWINAPIOpenGLView NAWINAPIOpenGLView;
struct NAWINAPIOpenGLView {
  NACoreOpenGLView coreopenglview;
  HGLRC hRC;    // The rendering context for OpenGL
};





#endif // NA_OS == NA_OS_WINDOWS
#endif // (NA_CONFIG_COMPILE_GUI == 1)


// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
