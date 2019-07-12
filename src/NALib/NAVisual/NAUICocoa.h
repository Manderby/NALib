
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file gets only included by the NAUICocoa.m file. We need to include
// NABase.h first to check if NA_OS ist MAC_OS_X.

#include "NABase.h"


#if (NA_CONFIG_COMPILE_GUI == 1)
#ifdef __OBJC__
#if NA_OS == NA_OS_MAC_OS_X


// Reaching here, the first thing we must do is include Cocoa. If this is
// done anywhere later, the compiler might crash.
#import <Cocoa/Cocoa.h>
// After that, we are free to include whatever we want.



//#include <objc/message.h>

#include "NAUICore.h"


typedef struct NACocoaApplication NACocoaApplication;
typedef struct NACocoaWindow      NACocoaWindow;
typedef struct NACocoaSpace       NACocoaSpace;
typedef struct NACocoaOpenGLSpace NACocoaOpenGLSpace;
typedef struct NACocoaButton      NACocoaButton;
typedef struct NACocoaRadioButton NACocoaRadioButton;
typedef struct NACocoaLabel       NACocoaLabel;


// ////////////////
// The bridging structs
//
// Native:       Adapter for the native implementation (in a specific language)
// Cocoa/WINAPI: Adapter for the technology (for any language)
// Core:         Adapter for NALib
// UIElement:    Direct pointer back to the native struct.
//
// Native:       Compiled with Objective-C - native class inheriting from Cocoa
// Cocoa/WINAPI: Compiled with Objective-C - C struct inheriting Core
// Core:         Compiled with C           - C struct inheriting UIElement
// UIElement:    Compiled with C           - C struct pointer to native.
//
// Native:       Compiled with Swift       - native class inheriting from Cocoa
// Cocoa/WINAPI: Compiled with Swift       - C struct inheriting Core
// Core:         Compiled with C           - C struct inheriting UIElement
// UIElement:    Compiled with C           - C struct pointer to native.

struct NACocoaApplication{
  NACoreApplication coreapp;
};

struct NACocoaWindow{
  NACoreWindow corewindow;
};

struct NACocoaSpace{
  NACoreSpace corespace;
};

struct NACocoaOpenGLSpace{
  NACoreOpenGLSpace coreopenglspace;
};

struct NACocoaButton{
  NACoreButton corebutton;
};

struct NACocoaRadioButton{
  NACoreRadioButton coreradiobutton;
  // Cocoa thinks it's smart by doing things automatically. Unfortunately, we
  // have to encapsulate the radiobutton into its own view to get the behaviour
  // we need.
  NSView* containingnsview;
};

struct NACocoaLabel{
  NACoreLabel corelabel;
};



// ///////////////
// The native classes

@interface NANativeWindow : NSWindow <NSWindowDelegate>{
  NACocoaWindow* cocoawindow;
  NAUInt trackingcount;
  NSTrackingArea* trackingarea;
}
@end

@interface NANativeSpace : NSView{
  NACocoaSpace* cocoaspace;
}
@end

#if (NA_CONFIG_COMPILE_OPENGL == 1)
  @interface NANativeOpenGLSpace : NSOpenGLView{
    NACocoaOpenGLSpace* cocoaopenglspace;
    NAMutator initFunc;
    void* initData;
  }
  @end
#endif

@interface NANativeButton : NSButton{
  NACocoaButton* cocoabutton;
}
@end

@interface NANativeRadioButton : NSButton{
  NACocoaRadioButton* cocoaradiobutton;
}
@end

@interface MDVerticallyCenteredTextFieldCell : NSTextFieldCell{
}
@end

@interface NANativeLabel : NSTextField{
  NACocoaLabel* cocoalabel;
}
@end




#endif // NA_OS == NA_OS_MAC_OS_X
#endif // __OBJC__
#endif //(NA_CONFIG_COMPILE_GUI == 1)

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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
