
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_UI_INCLUDED
#define NA_UI_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"
#include "NACoord.h"

// NALib provides a simple implementation for UI.
//
// Here, you can define Applications, Windows, Buttons, Sliders, etc. You can
// react to events such as mouse or keyboard events.
//
// Concerning graphical user interfaces (GUIs):
// GUIs are implemented in various ways across all systems and there is no
// general solution to how one should design a GUI. Therefore, NALib combines
// several ideas into a new implementation but allowing the programmer to use
// that implementation on all systems. Currently Mac and Windows.
//
// In NALib, you create a GUI by creating user interface elements. The topmost
// element is the application, having multiple windows. A window can be just a
// window or made fullscreen.
//
// Inside a window, you place other elements like buttons, sliders, ...


/////////////////////////////////////
// User Interface Elements
//

typedef enum{
  NA_UI_APPLICATION,
  NA_UI_WINDOW,
  NA_UI_OPENGLVIEW,
} NAUIElementType;

typedef struct NAApplication  NAApplication;
typedef struct NAWindow       NAWindow;
typedef struct NAOpenGLView   NAOpenGLView;

// NALib always acts as a layer on top of the native GUI implementation of a
// system. No matter if it is Macintosh or Windows, you can get the native
// structures used in the corresponding UI systems with naGetNativeID. This
// allows you to do with the user interface elements whatever you need to do.
//
// Use the following function to retrieve the native ID for any of the above
// element structs.

typedef void* NANativeUIID;
NANativeUIID naGetNativeID(void* uielement);

// Windows: The native framework used is WINAPI and the nativeID you get is a
// HWND handle. If the user interface element is the application itself, you
// get a HINSTANCE handle.
//
// Macintosh: NALib is using the Cocoa framework as the native UI. This means
// that in the background, NALib implements certain Objective-C methods to
// provide a UI most closely possible to a native experience. The nativeID
// corresponds to NSResponder*. Note that also NSApplication inherits from
// NSResponder.

// Working with the UI in NALib works by defining reaction handlers. Any user
// interface element can react to several commands by calling naAddUIReaction.
// When a specific command occurs, a function handler will be called with
// the function prototype NAReactionHandler.
//
// You can have as many reactions as you like for any number of elements. The
// argument sent to the reaction handler is dependend on the command sent:
//
// command      arg type      arg
// ----------------------------------------------------------------------
// INIT         -             NA_NULL
// REDRAW       NARect*       Rect of the uielement needed to be redrawn.
// RESHAPE      NARect*       The new position and size relative to the parent.
// KEYDOWN      NAUIKeyCode*  The keycode of the key pressed.
// KEYUP        NAUIKeyCode*  The keycode of the key pressed.
// MOUSEMOVED   NASize*       The difference (positive or negative) in x and y.
//
// The INIT method will be called once per element.

typedef enum{
  NA_UI_COMMAND_REDRAW = 0,
  NA_UI_COMMAND_RESHAPE,
  NA_UI_COMMAND_KEYDOWN,
  NA_UI_COMMAND_KEYUP,
  NA_UI_COMMAND_MOUSEMOVED,
} NAUICommand;

typedef void* NAUIArgument;

// The function prototype NAReactionHandler and the function naAddUIReaction
// work in pairs. You can define a controller if you like or set it to NA_NULL.
// Either way, the controller given to naAddUIReaction will be set as the first
// parameter of the reaction handler. The uielement is the element where
// the command occurs.
//
// When the handler returns NA_TRUE, the command will not be processed any
// further. If it is NA_FALSE however, the command will be furher processed. If
// there are other handlers handling this command, they will then be executed.

typedef NABool (*NAReactionHandler)(  void* controller,
                                      void* uielement,
                                NAUICommand command,
                               NAUIArgument arg);

NA_API void naAddUIReaction(          void* controller,
                                      void* uielement,
                                NAUICommand command,
                          NAReactionHandler handler);


// ////////////////////////////////
// Initializing the application.
//
// On some systems, an application is represented by a handle or another
// kind of pointer. In case you need it, you can access it with the following
// API.
//
// Additionally, on Mac OS X, before you can start the UI with, you need
// to have an NSApplication running. You have two options:
// - You call naStartDefaultApplication which does the same as calling
//   [NSApplication sharedApplication]. Note that you have no possibility to
//   use a custom class when choosing this option.
// - You can call [NSApplication sharedApplication] yourself. You can also use
//   any other Objective-C class inheriting from NSApplication. But this means
//   that you have to have a .m file somewhere, importing the <Cocoa/Cocoa.h>
//   framework and handle that yourself.
//
// Note that NALib does not uses a custom NSApplication class but rather more
// attaches its functionality to whatever class you are using.
//
// On Windows, the naStartDefaultApplication function does nothing.

NA_API NAApplication*  naGetApplication(void);
NA_API void   naStartDefaultApplication(void);


// ///////////////////////////////
// Initializing, Running and Stopping the UI:

// In NALib, there exists a global UI struct hidden to you. When you need to
// have an UI, you first have to create the UI using naInitUI. When you are
// done with the UI, you can call naClearUI to free all structures associated
// with it.
//
// Note that when programming on Mac OS X, you need to start an NSApplication
// before using naInitUI. See documentation of naStartDefaultApplication above.
//
// After you initialized the UI with naInitUI, you start building the UI with
// the ui elements. And you add reactions to those elements which you would
// like to control.
//
// Finally, when you set up all user interface elements and reactions, you can
// run the UI with a call to naRunUI. When you do this, an infinite loop is
// entered reacting to any input coming.

NA_API void   naInitUI  (void);
NA_API void   naClearUI (void);
NA_API NAUInt naRunUI   (void);


// When using a GUI on windows, you will sooner or later have to set the
// subsystem in the project properties->linker->system to /SUBSYSTEM:WINDOWS.
// This means two things:
// 1. Instead of in main(int, char**), you need
//    int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
// 2. There is no console output using the standard channels 0, 1 and 2 like
//    they are used with printf and such.
// In order to have a console output, you can call naOpenConsoleOnWindows
// which opens up a separate console window when running. Note that NALib
// will NOT automatically hide the console when NDEBUG is defined!
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  NA_API void naOpenConsoleOnWindows();
#endif


// //////////////////////////
// Functions for all UI elements

// You can get the window of any ui element except for the application element.
// The application element will return NA_NULL. A window element will return
// itself and any other ui element will return the window it is contained in.
NA_API NAWindow*  naGetUIElementWindow  (void* uielement);
// Any ui element has a strict hierarchical ordering: Application - Windows -
// Views - Subviews - Subsubviews ... You can get the parent element with this
// function. The parent of the Application will be NA_NULL.
NA_API void*      naGetUIElementParent  (void* uielement);
// Returns the type of the given ui element.
NA_API NAUIElementType naGetUIElementType(void* uielement);
// You can ask any ui element to refrehs its contents. This will cause the
// element to be displayed anew. The time difference defines when the refresh
// shall occur in seconds. Note that a value of 0 will not actually execute the
// redraw method immediately but put a message to the default message queue of
// the application. Therefore, this function will always immediately return.
NA_API void       naRefreshUIElement    (void* uielement, double timediff);


NA_API NAWindow* naNewWindow(const char* title, double posx, double posy, double width, double height, NABool resizeable);
NA_API void naShowWindow(NAWindow* window);
NA_API void naSetWindowContentView(NAWindow* window, void* uielement);
NA_API void naSetWindowFullscreen(NAWindow* window, NABool fullscreen);
NA_API NABool naIsWindowFullscreen(NAWindow* window);
NA_API NARect naGetWindowRect(NAWindow* window);

NA_API NAOpenGLView* naNewOpenGLView(NAWindow* window, double width, double height);
NA_API void naStartOpenGLDrawing(NAOpenGLView* openglview);
NA_API void naEndOpenGLDrawing(NAOpenGLView* openglview);

NA_API void naCenterMouseInView(NAOpenGLView* openglview);
NA_API void naHideMouse();




// //////////////////////////////////////
// Additional macro definitions used for the UI

// Very much the same as the native ID, there are certain types which are
// casted differently on the different systems and therefore they are
// declared with a global void* or integer big enough to encapsulate all
// possible casts on all systems.
typedef NAUInt NAUIKeyCode;

// A NAUIKeyCode can have the following values. These values correspond to the
// "US extended" keyboard layout. They represent the physical keys and not the
// characters they might result in.
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #define NA_KEYCODE_A             0x41
  #define NA_KEYCODE_B             0x42
  #define NA_KEYCODE_C             0x43
  #define NA_KEYCODE_D             0x44
  #define NA_KEYCODE_E             0x45
  #define NA_KEYCODE_F             0x46
  #define NA_KEYCODE_G             0x47
  #define NA_KEYCODE_H             0x48
  #define NA_KEYCODE_I             0x49
  #define NA_KEYCODE_J             0x4a
  #define NA_KEYCODE_K             0x4b
  #define NA_KEYCODE_L             0x4c
  #define NA_KEYCODE_M             0x4d
  #define NA_KEYCODE_N             0x4e
  #define NA_KEYCODE_O             0x4f
  #define NA_KEYCODE_P             0x50
  #define NA_KEYCODE_Q             0x51
  #define NA_KEYCODE_R             0x52
  #define NA_KEYCODE_S             0x53
  #define NA_KEYCODE_T             0x54
  #define NA_KEYCODE_U             0x55
  #define NA_KEYCODE_V             0x56
  #define NA_KEYCODE_W             0x57
  #define NA_KEYCODE_X             0x58
  #define NA_KEYCODE_Y             0x59
  #define NA_KEYCODE_Z             0x5a
  #define NA_KEYCODE_SPACE         VK_SPACE
  #define NA_KEYCODE_LEFT_COMMAND  VK_LWIN
  #define NA_KEYCODE_SHIFT         VK_SHIFT
  #define NA_KEYCODE_OPTION        VK_MENU
  #define NA_KEYCODE_CONTROL       VK_CONTROL
  #define NA_KEYCODE_ESC           VK_ESCAPE
  #define NA_KEYCODE_F11           VK_F11
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #define NA_KEYCODE_A       0x00
  #define NA_KEYCODE_B       0x0b
  #define NA_KEYCODE_C       0x08
  #define NA_KEYCODE_D       0x02
  #define NA_KEYCODE_E       0x0e
  #define NA_KEYCODE_F       0x03
  #define NA_KEYCODE_G       0x05
  #define NA_KEYCODE_H       0x04
  #define NA_KEYCODE_I       0x22
  #define NA_KEYCODE_J       0x26
  #define NA_KEYCODE_K       0x28
  #define NA_KEYCODE_L       0x25
  #define NA_KEYCODE_M       0x2e
  #define NA_KEYCODE_N       0x2d
  #define NA_KEYCODE_O       0x1f
  #define NA_KEYCODE_P       0x23
  #define NA_KEYCODE_Q       0x0c
  #define NA_KEYCODE_R       0x0f
  #define NA_KEYCODE_S       0x01
  #define NA_KEYCODE_T       0x11
  #define NA_KEYCODE_U       0x20
  #define NA_KEYCODE_V       0x09
  #define NA_KEYCODE_W       0x0d
  #define NA_KEYCODE_X       0x07
  #define NA_KEYCODE_Y       0x10
  #define NA_KEYCODE_Z       0x06
  #define NA_KEYCODE_SPACE   0x31
  #define NA_KEYCODE_COMMAND 0x37
  #define NA_KEYCODE_SHIFT   0x38
  #define NA_KEYCODE_OPTION  0x3a
  #define NA_KEYCODE_CONTROL 0x3b
  #define NA_KEYCODE_ESC     0x35
  #define NA_KEYCODE_F11     0x67
#endif




// //////////////////////////////////////////
// Hidden API

// Additional function prototypes and types used internally in NALib.
// Normally, these would be hidden in a separate .h file but in NALib, they are
// gathered here. You are free to use them but note that these are supposed to
// be helper functions.

#include "NAList.h"


// The base type of any ui element. All ui element struct definitions have
// an NAUIElement as the first entry.
// Each UIElement is a struct storing an NAUIElement as its first entry. The
// definition of NAUIElement should normally be hidden to the user. You can
// find it further below in this file.
//
// You can cast all elements as an NAUIElement*. Nontheless, as casting can
typedef struct NAUIElement NAUIElement;
struct NAUIElement{
  NAUIElementType elementtype;
  NAUIElement* parent;
  NAList reactions;
  NANativeUIID nativeID;  // The native object
};

// The application as an NAApplication struct
typedef struct NAApplication NAApplication;
struct NAApplication{
  NAUIElement uielement;
};


// Performs all necessary initialization of the UI independent of the system.
// The naInitUI functions are system dependent and will call this function
// before doing anything else.
NA_HAPI void naInitBareUI(void);

// Adds an NAUIElement to the hidden ui instance.
NA_HAPI void naAddUIElement(void* element);

NA_HAPI NAUIElement* naInitUIElement(  void* uielement,
                                NAUIElement* parent,
                             NAUIElementType elementtype,
                                NANativeUIID nativeID);

// Returns a pointer to the gui element which uses the given native ID.
// Every gui element which is handeled by NALib uses a native struct which is
// dependent on the system running. When handling events, a native ID is sent
// but this native ID can in general not be mapped directly to a corresponding
// NALib struct. This function solves that. Slow, but does the job.
NA_HAPI void* naGetUINALibEquivalent(NANativeUIID nativeID);

// Dispatches a command with the given uielement and arg.
// As long as the command has not been finished using NA_TRUE as a return value
// in the NAReactionHandler function handler, it will be bubbling upwards in
// the following order:
// - First responder
// - containing view
// - window
// - application
// - discard command as unhandled.
// The function will return NA_TRUE if the event shall not be processed any
// further. If this function returns NA_FALSE, the event shall still be
// processed by the calling function. This is especially important on Windows
// where non-handling of certain events might interrupt the whole messaging
// chain.
NA_HAPI NABool naDispatchUIElementCommand(  void* uielement,
                                    NAUICommand command,
                                   NAUIArgument arg);





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
