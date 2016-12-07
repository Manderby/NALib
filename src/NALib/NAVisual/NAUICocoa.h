
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file gets only included by the NAUICocoa.m file. We need to include
// NASystem first to check if NA_SYSTEM ist MAC_OS_X.

#include "NASystem.h"


#ifdef __OBJC__
#if NA_SYSTEM == NA_SYSTEM_MAC_OS_X


// Reaching here, the first thing we must do is include Cocoa. If this is
// done anywhere later, the compiler might crash.
#import <Cocoa/Cocoa.h>
// After that, we are free to include whatever we want.



//#include <objc/message.h>

#include "NAUICoreAPI.h"


typedef struct NACocoaApplication NACocoaApplication;
typedef struct NACocoaWindow NACocoaWindow;


struct NACocoaApplication{
  NACoreApplication coreapp;
};


struct NACocoaWindow{
  NACoreWindow corewindow;
  NAUInt trackingcount;
  NSTrackingArea* trackingarea;
};




@interface NANativeView : NSView
@end


@interface NANativeWindow : NSWindow <NSWindowDelegate>{
  NACocoaWindow* cocoawindow;
}
@end


@interface NANativeOpenGLView : NSOpenGLView{
  NAOpenGLView* nalibopenglview;
  NAFunc initFunc;
  void* initData;
}
@end




#endif // NA_SYSTEM == NA_SYSTEM_MAC_OS_X
#endif // __OBJC__

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
