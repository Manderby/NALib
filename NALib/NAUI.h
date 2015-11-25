
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_UI_INCLUDED
#define NA_UI_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// //////////////////
//
// Warning: This file is still in development. It works but it is not
// recommended for streamlined use just yet.
//
// //////////////////

#include "NASystem.h"
#include "NACoord.h"

// NALib provides a simple implementation for UI.
//
// Here, you can define Applications, Windows, Buttons, Sliders, etc.

typedef struct NAApplication  NAApplication;
typedef struct NAScreen       NAScreen;
typedef struct NAWindow       NAWindow;
typedef struct NAOpenGLView   NAOpenGLView;


// /////////////////////
// Concerning graphical user interfaces (GUIs):
//
// GUIs are implemented in various ways across all systems and there is no
// general solution to how one should design a GUI. Therefore, NALib combines
// several ideas into a new implementation but allowing the programmer to use
// that implementation consistently on all systems. Currently Mac and Windows.
//
// In NALib, you create a GUI by starting an NAApplication and then adding
// user interface elements to that application one by one. The topmost ui
// element is the application itself. From there, you can access the screens
// and add Windows. Windows may contain views and subviews where you place
// elements like buttons, sliders, OpenGL views, ...
//
// User interactions can be captured by defining sp called "reactions".
//

// Note that UI implementation of NALib automatically makes use of
// multi-threading. Therefore, when including this file on Mac OS X,
// grand central dispatch (GCD) will automatically be included.
// On Windows, the default WINAPI threads will be used.


// ////////////////////////////////
// Running the application.
//
// On each system, there is at some point a so called message loop where the
// system informs NALib that the user performed an action on a ui element which
// needs handling. To get to that message loop, you start the application with
// the following function:

NA_API void naStartApplication(  NAFunc prestartup,
                                 NAFunc poststartup,
                                  void* arg);

// All arguments can be NA_NULL but you can ask NALib to call the given two
// startup functions with the given arg. The precise order of the calls is
// described here:
//
// Mac: - NALib calls [NSApplication sharedApplication]
//      - NALib allocates some structures in the background to run the UI.
//      - NALib creates an NSAutoreleasePool
//        * NALib calls prestartup with arg.
//        * NALib calls [NSApp finishLaunching] which in turn will post an
//          NSApplicationDidFinishLaunchingNotification to whatever application
//          delegate you might have set.
//        * NALib calls poststartup with arg.
//      - NALib drains the autorelease pool.
//      - NALib will start a message loop wich a new NSAutoreleasePools for
//        each and every message.
//
// Win: - NALib allocates some structures in the background to run the UI.
//      - NALib calls prestartup with arg.
//      - NALib registers its window classes
//      - NALib calls poststartup with arg.
//      - NALib will start a message loop.
//
// prestartup:
// The prestartup function is here for initialization of global variables
// which have nothing to do with UI. Especially on a Mac, this function is
// intended to not execute Objective-C code although you of couse may. This
// function is rarely used but it is here for you if you really need it.
//
// poststartup:
// In the poststartup function, you usually start building the UI with the
// ui elements explained below. And you add reactions to those elements which
// you would like to control. You are of course free to do this in the
// didFinishLaunching method of you NSApplication delegate on a Mac, if you
// really want to.
//
// Note that both in the prestartup as well as the poststartup function, the
// global NAApplication struct of NALib is ready to be used. You can get this
// struct using the following call. See naGetUINativeID to get the native app
// pointer HINSTANCE (on Windows) or NSApp (on a Macintosh).

NA_API NAApplication*  naGetApplication(void);

// todo stopping the application

// On Mac systems, you can ask to flush the current NSAutoreleasePool once
// naStartApplication had been called with the following function. This
// function does nothing on Windows.

NA_API void   cubFlushGarbageMemory();

// When using a GUI on windows, you will sooner or later have to set the
// subsystem in the project properties->linker->system to /SUBSYSTEM:WINDOWS.
// This means two things:
// 1. Instead of in main(int, char**), you need
//    int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
// 2. There is no console output using the standard channels 0, 1 and 2 like
//    they are used with printf and such.
//
// In order to have a console output, you can call naOpenConsoleWindow
// which opens up a separate console window when running. Note that NALib
// will NOT automatically hide the console when NDEBUG is defined! This
// function does nothing on a Mac.

NA_API void naOpenConsoleWindow();




/////////////////////////////////////
// User Interface Elements
//
// In NALib, the whole application structure is understood to consist of UI
// elements. They have a clear hierarchical ordering:
// - Application
// - Screen
// - Window
// - View
// - Subviews...
//
// In NALib, the implementation of the corresponding structs is hidden in the
// NAUIHiddenAPI.h file but all structs are typedef'd on the top of this file.
// See NAApplication or NAWindow for example.
//
// Each of these structs contains a hidden structure identifying the elements
// as what they are. A ui element therefore can be sent to any of the following
// ui functions where a void* is expected.
//
// You as a programmer can identify the type of a void* uielement by quering
// its type:

typedef enum{
  NA_UI_APPLICATION,
  NA_UI_SCREEN,
  NA_UI_WINDOW,
  NA_UI_OPENGLVIEW,
} NAUIElementType;

// Returns the type of the given ui element.
NA_API NAUIElementType naGetUIElementType(void* uielement);

// Native IDs
//
// NALib always acts as a layer on top of the native UI implementation of a
// system. No matter if it is Macintosh or Windows, you can get the native
// structures used in the corresponding UI systems with naGetUINativeID. This
// allows you to do with the user interface elements whatever you need to do.
//
// Windows: The native framework used is WINAPI and the nativeID you get is a
// HWND handle. If the user interface element is the application itself, you
// get a HINSTANCE handle.
//
// Macintosh: NALib is using the Cocoa framework as the native UI. This means
// that in the background, NALib implements certain Objective-C methods to
// provide a UI most closely possible to a native experience. The nativeID
// corresponds to NSResponder*. Note that also NSApplication inherits from
// NSResponder.
//
// Use the following function to retrieve the native ID for any ui element

void* naGetUINativeID(void* uielement);

// Any ui element has a strict hierarchical ordering: Application - Screen -
// Window - View - Subview - Subsubview ... You can get the parent element
// with this function. The parent of the Application will be NA_NULL.
NA_API void*      naGetUIElementParent  (void* uielement);

// You can get the window of any ui element except for application and screen
// elements. An application or screen element will return NA_NULL. A window
// element will return itself and any other ui element will return the window
// it is contained in.
NA_API NAWindow*  naGetUIElementWindow  (void* uielement);

// In NALib, all coordinates of the UI are described in a mathematical, right-
// handed coordinate system. The origin of the global coordinate system is
// at the lower left screen corner of the main screen.
//
// Using naGetUIElementRect, you can get the rect of any ui element relative
// to another element. If you send the NAApplication instance as the relative
// element, (using naGetApplication), you will get global coordinates. If you
// send NA_NULL as the relative element, you will get coordinates relative to
// the parent element.
//
// Note that the includebounds argument only works for windows for now. It
// returns either the content rectangle (client rectangle) or the window
// outer frame.
NA_API NARect naGetUIElementRect(   void* uielement,
                                    void* relativeelement,
                                   NABool includebounds);

// You can ask any ui element to refrehs its contents. This will cause the
// element to be displayed anew. The time difference defines when the refresh
// shall occur in seconds. Note that using a value of 0 as timediff will not
// actually execute the redraw method immediately but put a message to the
// default message queue of the application. Therefore, this function will
// always immediately return.
NA_API void       naRefreshUIElement    (void* uielement, double timediff);





// //////////////////////////////
// Reacting to events
//
// Working with the UI in NALib works by defining reaction handlers. Any user
// interface element can react to several commands:

typedef enum{
  NA_UI_COMMAND_REDRAW = 0,
  NA_UI_COMMAND_RESHAPE,
  NA_UI_COMMAND_KEYDOWN,
  NA_UI_COMMAND_KEYUP,
  NA_UI_COMMAND_MOUSE_MOVED,
  NA_UI_COMMAND_MOUSE_ENTERED,
  NA_UI_COMMAND_MOUSE_EXITED,
} NAUICommand;

// A programmer reacts to commands by calling naAddUIReaction. When a specific
// command occurs, a function handler will be called with the function
// prototype NAReactionHandler.

typedef NABool (*NAReactionHandler)(  void* controller,
                                      void* uielement,
                                NAUICommand command,
                                      void* arg);

NA_API void naAddUIReaction(          void* controller,
                                      void* uielement,
                                NAUICommand command,
                          NAReactionHandler handler);


// The function naAddUIReaction and the function prototype NAReactionHandler
// work in pairs. The controller given to naAddUIReaction will be set as the
// first parameter of the reaction handler. The uielement is the element where
// the command occurs and the command sent is the command observed.
//
// The argument sent to the reaction handler is dependent on the command:
//
// command       arg type     arg
// ----------------------------------------------------------------------
// INIT          -            NA_NULL
// REDRAW        NARect*      Rect of the uielement needed to be redrawn.
// RESHAPE       NARect*      The new position and size relative to the parent.
// KEYDOWN       NAUIKeyCode* The keycode of the key pressed.
// KEYUP         NAUIKeyCode* The keycode of the key pressed.
// MOUSE_MOVED   -            NA_NULL
// MOUSE_ENTERED -            NA_NULL
// MOUSE_EXITED  -            NA_NULL
//
// The INIT method will be called once per element.
//
// You can have as many reactions as you like for any number of elements. They
// will be stored separately for every ui element in a list which will always
// be traversed sequentially from head to tail.
//
// When a reaction handler returns NA_TRUE, the command is considered handeled
// and will not be processed any further. If it is NA_FALSE however, the
// command will be further processed. If there are other handlers in the
// reaction list, they will then be executed.







// From here on, the documentation might be garbage.






// Also part of the UI is the cursor of the mouse.
typedef struct NACursorInfo   NACursorInfo;



NA_API NARect naGetMainScreenRect();

NA_API NAWindow* naNewWindow(const char* title, double posx, double posy, double width, double height, NABool resizeable);
NA_API void naShowWindow(NAWindow* window);
NA_API void naSetWindowContentView(NAWindow* window, void* uielement);
NA_API void naSetWindowFullscreen(NAWindow* window, NABool fullscreen);
NA_API NABool naIsWindowFullscreen(NAWindow* window);
//NA_API NARect naGetWindowRect(NAWindow* window);

NA_API NAOpenGLView* naNewOpenGLView(NAWindow* window, double width, double height);
// Swaps the OpenGL buffer.
NA_API void naSwapOpenGLBuffer(NAOpenGLView* openglview);
NA_API void naSetOpenGLInnerRect(NAOpenGLView* openglview, NARect bounds);


NA_API void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage);
NA_API void naShowMouse();
NA_API void naHideMouse();

NA_API const NACursorInfo* naGetMouseInfo();
NA_API NAPos naGetCursorPos(const NACursorInfo* cursorinfo);
NA_API NASize naGetCursorDelta(const NACursorInfo* cursorinfo);


// Executes the given function in the given number of seconds with the given
// arg by posting a message on the main execute loop. This function returns
// immediately. Note that the timediff is given as a double.
NA_API void naCallFunctionInSeconds(     NAFunc function,
                                           void* arg,
                                         double timediff);
// Note that although this API would perfectly fit into the NAThreading.h file,
// it is located here as it only makes sense to use when an application with
// a message loop is running.


// //////////////////////////////////////
// Additional macro definitions used for the UI

// When handling keyboard input, NALib captures the keys pressed and provides
// you commands for KeyDown and KeyUp. The key pressed is stored in the
// following type:

typedef NAUInt NAUIKeyCode;

// A NAUIKeyCode can have the following values. These values correspond to the
// "US extended" keyboard layout. They represent the physical keys and not the
// characters they might result in.
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #define NA_KEYCODE_A              0x1e
  #define NA_KEYCODE_B              0x30
  #define NA_KEYCODE_C              0x2e
  #define NA_KEYCODE_D              0x20
  #define NA_KEYCODE_E              0x12
  #define NA_KEYCODE_F              0x21
  #define NA_KEYCODE_G              0x22
  #define NA_KEYCODE_H              0x23
  #define NA_KEYCODE_I              0x17
  #define NA_KEYCODE_J              0x24
  #define NA_KEYCODE_K              0x25
  #define NA_KEYCODE_L              0x26
  #define NA_KEYCODE_M              0x32
  #define NA_KEYCODE_N              0x31
  #define NA_KEYCODE_O              0x18
  #define NA_KEYCODE_P              0x19
  #define NA_KEYCODE_Q              0x10
  #define NA_KEYCODE_R              0x13
  #define NA_KEYCODE_S              0x1f
  #define NA_KEYCODE_T              0x14
  #define NA_KEYCODE_U              0x16
  #define NA_KEYCODE_V              0x2f
  #define NA_KEYCODE_W              0x11
  #define NA_KEYCODE_X              0x2d
  #define NA_KEYCODE_Y              0x15
  #define NA_KEYCODE_Z              0x2c
  #define NA_KEYCODE_SPACE          0x39
  #define NA_KEYCODE_LEFT_COMMAND   0x5b
  #define NA_KEYCODE_SHIFT          0x2a
  #define NA_KEYCODE_OPTION         0xff // Handeled differently on windows
  #define NA_KEYCODE_CONTROL        0x1d
  #define NA_KEYCODE_ESC            0x01
  #define NA_KEYCODE_F11            0x57
  #define NA_KEYCODE_MINUS          0x0c
  #define NA_KEYCODE_EQUAL          0x0d
  #define NA_KEYCODE_NUMPAD_MINUS   0x4a
  #define NA_KEYCODE_NUMPAD_PLUS    0x4e
  //#define NA_KEYCODE_A              0x41
  //#define NA_KEYCODE_B              0x42
  //#define NA_KEYCODE_C              0x43
  //#define NA_KEYCODE_D              0x44
  //#define NA_KEYCODE_E              0x45
  //#define NA_KEYCODE_F              0x46
  //#define NA_KEYCODE_G              0x47
  //#define NA_KEYCODE_H              0x48
  //#define NA_KEYCODE_I              0x49
  //#define NA_KEYCODE_J              0x4a
  //#define NA_KEYCODE_K              0x4b
  //#define NA_KEYCODE_L              0x4c
  //#define NA_KEYCODE_M              0x4d
  //#define NA_KEYCODE_N              0x4e
  //#define NA_KEYCODE_O              0x4f
  //#define NA_KEYCODE_P              0x50
  //#define NA_KEYCODE_Q              0x51
  //#define NA_KEYCODE_R              0x52
  //#define NA_KEYCODE_S              0x53
  //#define NA_KEYCODE_T              0x54
  //#define NA_KEYCODE_U              0x55
  //#define NA_KEYCODE_V              0x56
  //#define NA_KEYCODE_W              0x57
  //#define NA_KEYCODE_X              0x58
  //#define NA_KEYCODE_Y              0x59
  //#define NA_KEYCODE_Z              0x5a
  //#define NA_KEYCODE_SPACE          VK_SPACE
  //#define NA_KEYCODE_LEFT_COMMAND   VK_LWIN
  //#define NA_KEYCODE_SHIFT          VK_SHIFT
  //#define NA_KEYCODE_OPTION         VK_MENU
  //#define NA_KEYCODE_CONTROL        VK_CONTROL
  //#define NA_KEYCODE_ESC            VK_ESCAPE
  //#define NA_KEYCODE_F11            VK_F11
  //#define NA_KEYCODE_MINUS          VK_OEM_MINUS
  //#define NA_KEYCODE_EQUAL          VK_OEM_PLUS
  //#define NA_KEYCODE_NUMPAD_MINUS   VK_SUBTRACT
  //#define NA_KEYCODE_NUMPAD_PLUS    VK_ADD
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #define NA_KEYCODE_A              0x00
  #define NA_KEYCODE_B              0x0b
  #define NA_KEYCODE_C              0x08
  #define NA_KEYCODE_D              0x02
  #define NA_KEYCODE_E              0x0e
  #define NA_KEYCODE_F              0x03
  #define NA_KEYCODE_G              0x05
  #define NA_KEYCODE_H              0x04
  #define NA_KEYCODE_I              0x22
  #define NA_KEYCODE_J              0x26
  #define NA_KEYCODE_K              0x28
  #define NA_KEYCODE_L              0x25
  #define NA_KEYCODE_M              0x2e
  #define NA_KEYCODE_N              0x2d
  #define NA_KEYCODE_O              0x1f
  #define NA_KEYCODE_P              0x23
  #define NA_KEYCODE_Q              0x0c
  #define NA_KEYCODE_R              0x0f
  #define NA_KEYCODE_S              0x01
  #define NA_KEYCODE_T              0x11
  #define NA_KEYCODE_U              0x20
  #define NA_KEYCODE_V              0x09
  #define NA_KEYCODE_W              0x0d
  #define NA_KEYCODE_X              0x07
  #define NA_KEYCODE_Y              0x10
  #define NA_KEYCODE_Z              0x06
  #define NA_KEYCODE_SPACE          0x31
  #define NA_KEYCODE_LEFT_COMMAND   0x37
  #define NA_KEYCODE_SHIFT          0x38
  #define NA_KEYCODE_OPTION         0x3a
  #define NA_KEYCODE_CONTROL        0x3b
  #define NA_KEYCODE_ESC            0x35
  #define NA_KEYCODE_F11            0x67
  #define NA_KEYCODE_MINUS          0x1b
  #define NA_KEYCODE_EQUAL          0x18
  #define NA_KEYCODE_NUMPAD_MINUS   0x4e
  #define NA_KEYCODE_NUMPAD_PLUS    0x45
#endif






#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_UI_INCLUDED



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
