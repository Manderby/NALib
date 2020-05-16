
#ifndef NA_UI_INCLUDED
#define NA_UI_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include "NABase.h"
#if NA_COMPILE_GUI == 1


// Have a look at NAConfiguration.h to use OpenGL
#if (NA_COMPILE_OPENGL == 1)
  #if NA_OS == NA_OS_WINDOWS
    #include <windows.h>
    #include <GL/GL.h>
  #else
    #include <OpenGL/gl.h>
  #endif
#endif

#include "NACoord.h"
#include "NAString.h"
#include "NABabyImage.h"
#include "NAEnvironment/NAUIImage.h"



// NALib provides a simple implementation for Graphical User Interfaces (GUI).
//
// GUIs are implemented in various ways across all systems and there is no
// general solution to how one should design a GUI. Therefore, NALib combines
// several ideas into a new implementation but allowing the programmer to use
// that implementation consistently on all systems. Currently Mac and Windows.
//
// In NALib, you create a GUI by starting an NAApplication and then adding
// user interface elements to that application one by one. The topmost ui
// element is the application itself. From there, you can access the screens
// and add Windows. Windows may contain spaces and subspaces where you place
// elements like buttons, sliders, OpenGL spaces, etc.
//
// User interactions can be captured by defining so called "reactions".
//
// Note that UI implementation of NALib automatically makes use of
// multi-threading. Therefore, when including this file on Mac OS X,
// grand central dispatch (GCD) will automatically be included.
// On Windows, the default WINAPI threads will be used.


/////////////////////////////////////
// User Interface Elements
//
// In NALib, the whole application structure is understood to consist of UI
// elements. They have a clear hierarchical ordering:
// - Application
// - Screen
// - Window
// - Space
// - Subspaces...
// - Elements like Buttons, Sliders, ...
//
// In NALib, the implementation of the corresponding structs is hidden in
// system dependent files but all structs are typedef'd as void here. You will
// interact with the NALib GUI by providing pointers to these void types. This
// makes it easy to use any ui element with any ui function where a NAUIElement
// is expected.

typedef void  NAUIElement;

typedef void  NAApplication;
typedef void  NAScreen;
typedef void  NAWindow;
typedef void  NASpace;
typedef void  NAImageSpace;
typedef void  NAOpenGLSpace;
typedef void  NAButton;
typedef void  NARadio;
typedef void  NACheckBox;
typedef void  NALabel;
typedef void  NATextField;
typedef void  NATextBox;



// Each of the hidden structs can identify themselves as what they are.
// In order to find out what a specific UI element is, you can use the
// the following enums and function.

typedef enum{
  NA_UI_APPLICATION,
  NA_UI_SCREEN,
  NA_UI_WINDOW,
  NA_UI_SPACE,
  NA_UI_IMAGESPACE,
  NA_UI_OPENGLSPACE,
  NA_UI_BUTTON,
  NA_UI_RADIO,
  NA_UI_CHECKBOX,
  NA_UI_LABEL,
  NA_UI_TEXTFIELD,
  NA_UI_TEXTBOX
} NAUIElementType;

NA_API NAUIElementType naGetUIElementType(NAUIElement* uielement);

NA_API void naReleaseUIElement(NAUIElement* uielement);

// Any ui element has a strict hierarchical ordering: Application - Screen -
// Window - Space - Subspace - Subsubspace ... You can get the parent element
// with this function. The parent of the Application will be NA_NULL.
NA_API NAUIElement* naGetUIElementParent  (NAUIElement* uielement);

// You can get the window of any ui element except for application and screen
// elements. An application or screen element will return NA_NULL. A window
// element will return itself and any other ui element will return the window
// it is contained in.
NA_API NAWindow*  naGetUIElementWindow  (NAUIElement* uielement);

// You can get the parent space of any ui element. Note that when the
// given element is itself a space, the parental space will be returned.
// If there is no parental space, NA_NULL will be returned. Applications,
// screens and windows will always return NA_NULL.
NA_API NASpace* naGetUIElementParentSpace(NAUIElement* uielement);

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
// Note that the includeborder argument only works for windows for now. It
// returns either the content rectangle (client rectangle) or the window
// outer frame.
NA_API NARect naGetUIElementRect(   NAUIElement* uielement,
                                    NAUIElement* relativeuielement,
                                    NABool includeborder);

// You can ask any ui element to refresh its contents. This will cause the
// element to be displayed anew. The time difference defines when the refresh
// shall occur in seconds. Note that even when using 0 as timediff, the redraw
// method will not execute immediately but put a message to the default message
// queue of the application. Therefore, this function will always immediately
// return.
NA_API void naRefreshUIElement    (NAUIElement* uielement, double timediff);

// When navigating with the tab key, this method defines, which will be the
// next ui element to have the focus. You start the tab order with a call to
// naSetWindowFirstTabElement.
NA_API void naSetUIElementNextTabElement(NAUIElement* elem, NAUIElement* nextTabElem);

// Native IDs
//
// NALib always acts as a layer on top of the native UI implementation of a
// system. No matter if it is Macintosh or Windows, you can get the native
// structures used in the corresponding UI systems with naGetUIElementNativeID.
// This allows you to do with the user interface elements whatever you need to
// do.
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
// Use the following function to retrieve the native ID for any ui element:

typedef void* NANativeID;

NA_API NANativeID naGetUIElementNativeID(NAUIElement* element);




// ////////////////////////////////
// The Application
//
// On each system, there is at some point a so called message loop where the
// system informs NALib that the user performed an action on a ui element which
// needs handling. To get to that message loop, you start the application with
// the following function:

NA_API void naStartApplication(  NAMutator prestartup,
                                 NAMutator poststartup,
                                     void* arg);

// All arguments can be NA_NULL but you can ask NALib to call the given two
// startup functions with the given arg. The precise order of the calls is
// described here:
//
// Mac: - NALib calls [NSApplication sharedApplication]
//      - NALib allocates some structures in the background to run the UI
//        including the application internal translator.
//      - NALib creates an NSAutoreleasePool (only when ARC is turned off)
//        * NALib calls prestartup with arg.
//        * NALib calls naResetApplicationPreferredTranslatorLanguages().
//        * NALib calls [NSApp finishLaunching] which in turn will post an
//          NSApplicationWillFinishLaunchingNotification to whatever
//          application delegate you might have set.
//        * NALib calls poststartup with arg.
//      - NALib drains the autorelease pool. (only when ARC is turned off)
//      - NALib sets its own internal application object as the apps delegate.
//      - NALib will start a message loop. When ARC is turned off, a new
//        NSAutoreleasePools is created for each and every message. At its
//        first run, a message to NSApplicationDidFinishLaunchingNotification
//        will be forwarded to whatever your application delegate was before
//        it was set to the NALib internal application object.
//
// Win: - NALib registers its window classes
//      - NALib allocates some structures in the background to run the UI
//        including the application internal translator.
//      - NALib calls prestartup with arg.
//      - NALib calls naResetApplicationPreferredTranslatorLanguages().
//      - NALib calls poststartup with arg.
//      - NALib will start a message loop.
//
// prestartup:
// The prestartup function is here for initialization of global variables
// and structures before any of the UI specific functions gets called.
// This function is intended to execute mainly C code but you of course are
// free to use other languages like Objective-C alongside with it.
//
// Note that the prestartup function is the perfect place to load your
// localizations (strings which are translated into different human languages).
// After this function, the preferred languages of the user are set in the
// translator.
//
// poststartup:
// In the poststartup function, you usually start building the UI with the
// ui elements explained below. And you add reactions to those elements which
// you would like to control. You are of course free to do this in the
// didFinishLaunching method of your NSApplication delegate on a Mac, if you
// really want to.

// ////////////////////
// Mixing an existing Cocoa application with NAApplication:
//
// If you require a mix of both environments, here is a very simple scheme you
// can use in your main.m file:
// 
// void poststartup(void* arg){
//   [NSBundle loadNibNamed:@"MainMenu" owner:NSApp];
//   // Now, do UI stuff with NALib. 
// }
//
// int main(int argc, char *argv[]){
//   naStartRuntime();
//   [MyExistingApplication sharedApplication];
//   naStartApplication(NA_NULL, poststartup, NA_NULL);
//   return 0;
// }
//
// Notes:
// - Make sure to use the correct type for MyExistingApplication!
// - NAApplication will forward all uncaptured events unaltered.
// - You can of course add a prestartup function with translations for example.
// - Also note that the loading of nib files must sometimes be done in the
//   willFinishLaunching method of the application delegate already.
// - In order to react to willFinishLaunching and didFinishLaunching, your
//   application class must use the <NSApplicationDelegate> protocol.
//
// //////// End intermission

// Note that both in the prestartup as well as the poststartup function, the
// global NAApplication struct of NALib is ready to be used. You can get this
// struct using the following call:

NA_API NAApplication* naGetApplication(void);

// If you need to get the native app pointer HINSTANCE (on Windows) or NSApp
// (on a Macintosh), use naGetUIElementNativeID.

// The message loop will run indefinitely until the application is terminated
// by a signal or it recieves a stop message using the following function: This
// Will send a stop message to the application which then in the next schedule
// will stop the run message loop and eventually return from the call
// to naStartApplication. All attached memory of the application will be freed
// and the application will not be able to run again!
NA_API void naStopApplication(void);

// Executes the given function in the given number of seconds with the given
// arg by posting a message on the main execute loop. This function returns
// immediately. Note that the timediff is given as a double.
//
// Note that this function is not defined in the NAThreading.h file because
// it only makes sense if there is a message loop which only exists when
// running a UI.
NA_API void naCallApplicationFunctionInSeconds(  NAMutator function,
                                                     void* arg,
                                                    double timediff);

// Causes the translator to reset its preferred language list to the preferred
// settings of the current user. This method is called automatically during
// naStartApplication but there might be times where you want to call it
// manually.
NA_API void naResetApplicationPreferredTranslatorLanguages(void);

// Define basic information about the application. On a mac, these informations
// can be extracted automatically from a plist file if not defined. On windows
// though, one has to provide it using the following functions. Note that also
// on Mac, the settings provided here overwrite anything else.
NA_API void naSetApplicationName(NAUTF8Char* name);
NA_API void naSetApplicationCompanyName(NAUTF8Char* name);
NA_API void naSetApplicationVersionString(NAUTF8Char* string);
NA_API void naSetApplicationBuildString(NAUTF8Char* string);
NA_API void naSetApplicationIconPath(NAUTF8Char* path);

// Retrieve the informations. All functions might return NA_NULL.
NA_API NAString* naNewApplicationName(void);
NA_API NAString* naNewApplicationCompanyName(void);
NA_API NAString* naNewApplicationVersionString(void);
NA_API NAString* naNewApplicationBuildString(void);
NA_API NAString* naNewApplicationIconPath(void);

// The application binary usually resides in some kind of base package folder
// and resources are located relative to that location. Using the following
// function, you can retrieve various informations. dir can be Null to search
// in the base package folder.
NA_API NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix);



// //////////////////////////////////////
// Additional macro definitions used for the UI

// When handling keyboard input, NALib captures the keys pressed and provides
// you commands for KeyDown and KeyUp. The key pressed is stored in the
// following type:
typedef NAUInt NAUIKeyCode;

// A NAUIKeyCode can have the following values. These values correspond to the
// "US extended" keyboard layout. They represent the physical keys and not the
// characters they might result in.
#if NA_OS == NA_OS_WINDOWS
  #define NA_KEYCODE_0              0x0b
  #define NA_KEYCODE_1              0x02
  #define NA_KEYCODE_2              0x03
  #define NA_KEYCODE_3              0x04
  #define NA_KEYCODE_4              0x05
  #define NA_KEYCODE_5              0x06
  #define NA_KEYCODE_6              0x07
  #define NA_KEYCODE_7              0x08
  #define NA_KEYCODE_8              0x09
  #define NA_KEYCODE_9              0x0a
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
  #define NA_KEYCODE_TAB            0x0f
  #define NA_KEYCODE_ENTER          0x1c
  #define NA_KEYCODE_LEFT_SHIFT     0x2a
  #define NA_KEYCODE_RIGHT_SHIFT    0x2a // right shift is not captured like this
  #define NA_KEYCODE_LEFT_CONTROL   0x1d
  #define NA_KEYCODE_RIGHT_CONTROL  0x1d
  #define NA_KEYCODE_LEFT_OPTION    0x38 // Handeled differently on windows
  #define NA_KEYCODE_RIGHT_OPTION   0x38 // Handeled differently on windows
  #define NA_KEYCODE_LEFT_COMMAND   0x5b // The windows key
  #define NA_KEYCODE_RIGHT_COMMAND  0x5c
  #define NA_KEYCODE_FN             0xff // Can not catch.
  #define NA_KEYCODE_ESC            0x01
  #define NA_KEYCODE_F11            0x57
  #define NA_KEYCODE_MINUS          0x0c
  #define NA_KEYCODE_EQUAL          0x0d
  #define NA_KEYCODE_PERIOD         0x34
  #define NA_KEYCODE_NUMPAD_MINUS   0x4a
  #define NA_KEYCODE_NUMPAD_PLUS    0x4e
#elif NA_OS == NA_OS_MAC_OS_X
  #define NA_KEYCODE_0              0x1d
  #define NA_KEYCODE_1              0x12
  #define NA_KEYCODE_2              0x13
  #define NA_KEYCODE_3              0x14
  #define NA_KEYCODE_4              0x15
  #define NA_KEYCODE_5              0x17
  #define NA_KEYCODE_6              0x16
  #define NA_KEYCODE_7              0x1a
  #define NA_KEYCODE_8              0x1c
  #define NA_KEYCODE_9              0x19
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
  #define NA_KEYCODE_TAB            0x30
  #define NA_KEYCODE_ENTER          0x24
  #define NA_KEYCODE_LEFT_SHIFT     0x38
  #define NA_KEYCODE_RIGHT_SHIFT    0x3c
  #define NA_KEYCODE_CONTROL        0x3b
  #define NA_KEYCODE_LEFT_OPTION    0x3a
  #define NA_KEYCODE_RIGHT_OPTION   0x3d
  #define NA_KEYCODE_LEFT_COMMAND   0x37
  #define NA_KEYCODE_RIGHT_COMMAND  0x36
  #define NA_KEYCODE_FN             0x3f
  #define NA_KEYCODE_ESC            0x35
  #define NA_KEYCODE_F11            0x67
  #define NA_KEYCODE_MINUS          0x1b
  #define NA_KEYCODE_EQUAL          0x18
  #define NA_KEYCODE_PERIOD         0x2f
  #define NA_KEYCODE_NUMPAD_MINUS   0x4e
  #define NA_KEYCODE_NUMPAD_PLUS    0x45
#endif

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
  NA_UI_COMMAND_CLOSES,
  NA_UI_COMMAND_PRESSED,
  NA_UI_COMMAND_EDITED,
  NA_UI_COMMAND_KEYBOARD_SHORTCUT
} NAUICommand;

typedef enum{
  NA_MODIFIER_FLAG_NONE          = 0x0000,
  NA_MODIFIER_FLAG_SHIFT         = 0x0003,
  NA_MODIFIER_FLAG_CONTROL       = 0x000c,
  NA_MODIFIER_FLAG_OPTION        = 0x0030,
  NA_MODIFIER_FLAG_COMMAND       = 0x00c0

  // The following flags will be available in the future. Please be patient.
//  NA_MODIFIER_FLAG_LEFT_SHIFT    = 0x0001,
//  NA_MODIFIER_FLAG_RIGHT_SHIFT   = 0x0002,
//  NA_MODIFIER_FLAG_LEFT_CONTROL  = 0x0004,
//  NA_MODIFIER_FLAG_RIGHT_CONTROL = 0x0008,
//  NA_MODIFIER_FLAG_LEFT_OPTION   = 0x0010,
//  NA_MODIFIER_FLAG_RIGHT_OPTION  = 0x0020,
//  NA_MODIFIER_FLAG_LEFT_COMMAND  = 0x0040,
//  NA_MODIFIER_FLAG_RIGHT_COMMAND = 0x0080,
} NAModifierFlag;

typedef struct NAKeyboardStatus NAKeyboardStatus;
struct NAKeyboardStatus{
  NAInt modifiers;
  NAUIKeyCode keyCode;
};

NAKeyboardStatus naMakeKeybardStatus(NAInt modifiers, NAUIKeyCode keyCode);

typedef struct NAMouseStatus NAMouseStatus;
struct NAMouseStatus{
  NAPos pos;
  NAPos prevpos;
};

// A programmer reacts to commands by calling naAddUIReaction. When a specific
// command occurs, a function handler will be called with the function
// prototype NAReactionHandler.

typedef struct NAReaction NAReaction;
struct NAReaction{
  NAUIElement* uielement;
  NAUICommand command;
  void* controller;
};

typedef NABool (*NAReactionHandler)(NAReaction reaction);

NA_API void naAddUIReaction(        NAUIElement* uielement,
                                     NAUICommand command,
                               NAReactionHandler handler,
                                           void* controller);

NA_API void naAddUIKeyboardShortcut(NAUIElement* uielement,
                                NAKeyboardStatus shortcut,
                               NAReactionHandler handler,
                                           void* controller);

// The function naAddUIReaction and the function prototype NAReactionHandler
// work in pairs. The controller given to naAddUIReaction is an arbitrary
// void pointer which simply will be set as the first parameter of the
// reaction handler. You probably will use either NA_NULL or some kind of
// controller pointer for that, hence the name controller. The uielement
// is the NAUIElement where the command occurs and the command sent is the
// command observed or the shortcut is the shortcut pressed respectively.
//
// The INIT method will be called once per element.
//
// The KEYDOWN and KEYUP method will only be called to a window but may
// propagate to parent elements.
//
// The CLOSES command will be called when a window tries to close. You can
// prevent it from closing by calling naPreventWindowFromClosing. That
// function only works in conjunction with such a command and will emit an
// error otherwise.
//
// You can have as many reactions as you like for any number of elements. They
// will be stored separately for every ui element in a list which will always
// be traversed sequentially forward from start to end.
//
// When a reaction handler returns NA_TRUE, the command is considered handeled
// and will not be processed any further. If it is NA_FALSE however, the
// command will be further processed. If there are other handlers in the
// reaction list, they will then be executed.



// /////////////////////////////////
// The actual UI elements
// 
// Following is a collection of UI elements like Buttons, Windows, Checkboxes
// you can use in NALib. They are mostly undocumented but should be easy to
// understand.

typedef enum{
  NA_FONT_KIND_SYSTEM,      // The default system font
  NA_FONT_KIND_TITLE,       // A bolder kind of the default system font
  NA_FONT_KIND_MONOSPACE,   // A monospace font.
  NA_FONT_KIND_PARAGRAPH,   // A nice to read font for displaying longer texts.
  NA_FONT_KIND_MATH,        // A font close to mathematical representation.
  NA_FONT_KIND_COUNT
} NAFontKind;

typedef enum{
  NA_TEXT_ALIGNMENT_LEFT,
  NA_TEXT_ALIGNMENT_RIGHT,
  NA_TEXT_ALIGNMENT_CENTER
} NATextAlignment;

#if NA_OS == NA_OS_MAC_OS_X
  NABool naLoadNib(NAUTF8Char* nibName);
#endif

// Screen
// Screens are not implemented yet, but you can get the main screen rect:
NA_API NARect naGetMainScreenRect(void);

// Window
NA_API NAWindow* naNewWindow(const NAUTF8Char* title, NARect rect, NABool resizeable, NAInt storageTag);
NA_API void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title);
NA_API void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop);
NA_API void naSetWindowRect(NAWindow* window, NARect rect);
NA_API NAUIImageResolution naGetWindowUIResolution(NAWindow* window);
NA_API void naSetWindowFirstTabElement(NAWindow* window, NAUIElement* firstTabElem);
NA_API NAUIElement* naGetWindowFirstTabElement(NAWindow* window);
NA_API void naShowWindow(NAWindow* window);
NA_API void naCloseWindow(NAWindow* window);
NA_API NASpace* naGetWindowContentSpace(NAWindow* window);
NA_API void naSetWindowContentSpace(NAWindow* window, NAUIElement* uielement);
NA_API void naSetWindowFullscreen(NAWindow* window, NABool fullscreen);
NA_API NABool naIsWindowFullscreen(NAWindow* window);
NA_API NABool naIsWindowResizeable(NAWindow* window);
NA_API void naPreventWindowFromClosing(NAWindow* window, NABool prevent);
NA_API NARect naSetWindowStorageTag(NAWindow* window, NAInt storageTag, NARect rect, NABool resizeable);

// Space (In other frameworks called View, Frame, Area, Widget...)
// Beware that adding a child to a space which formerly was the first responder
// of a window, that windows first responder will be reset to NULL.
NA_API NASpace* naNewSpace(NASize size);
NA_API void naAddSpaceChild(NASpace* space, NAUIElement* child, NAPos pos);
NA_API void naSetSpaceAlternateBackground(NASpace* space, NABool alternate);
NA_API NABool naGetSpaceAlternateBackground(NASpace* space);
NA_API void naSetSpaceRect(NASpace* space, NARect rect);

// ImageSpace
NA_API NAImageSpace* naNewImageSpace(NAUIImage* uiimage, NASize size);

// OpenGLSpace
#if NA_COMPILE_OPENGL == 1
  // the initfunc will be called with initdata as the input parameter as
  // soon as there is an OpenGLContext available. You can put there all
  // initialization necessary like for example uploading of textures to the
  // GPU.
  // Note that the initFunc will be called...
  // Win: Right within the naNewOpenGLSpace
  // Mac: when prepareOpenGL is called (which may be as late as when the
  //      space comes onsceen)
  NA_API NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata);
  NA_API void naSwapOpenGLBuffer(NAOpenGLSpace* openglspace);
  NA_API void naSetOpenGLInnerRect(NAOpenGLSpace* openglspace, NARect bounds);
#endif

// Button
NA_API NAButton* naNewPushButton(const NAUTF8Char* text, NASize size);
NA_API NAButton* naNewTextOptionButton(const NAUTF8Char* text, NASize size);
NA_API NAButton* naNewImageOptionButton(NAUIImage* uiimage, NASize size);
NA_API NAButton* naNewImageButton(NAUIImage* uiimage, NASize size);
NA_API void naSetButtonState(NAButton* button, NABool state);
NA_API void naSetButtonSubmit(NAButton* button, NAReactionHandler handler, NAUIElement* controller);
NA_API void naSetButtonAbort(NAButton* button, NAReactionHandler handler, NAUIElement* controller);

// Radio
NA_API NARadio* naNewRadio(const NAUTF8Char* text, NASize size);
NA_API NABool naGetRadioState(NARadio* radio);
NA_API void naSetRadioState(NARadio* radio, NABool state);

// CheckBox
NA_API NACheckBox* naNewCheckBox(const NAUTF8Char* text, NASize size);
NA_API void naSetCheckBoxState(NACheckBox* checkbox, NABool state);
NA_API NABool naGetCheckBoxState(NACheckBox* checkbox);

// Label
NA_API NALabel* naNewLabel(const NAUTF8Char* text, NASize size);
NA_API void naSetLabelText(NALabel* label, const NAUTF8Char* text);
// Note that text alignment must be set before calling this method.
NA_API void naSetLabelLink(NALabel* label, const NAUTF8Char* url);
NA_API NABool naIsLabelEnabled(NALabel* label);
NA_API void naSetLabelEnabled(NALabel* label, NABool enabled);
NA_API void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment);
NA_API void naSetLabelFontKind(NALabel* label, NAFontKind kind);

// TextField
NA_API NATextField* naNewTextField(NASize size);
NA_API void naSetTextFieldText(NATextField* textfield, const NAUTF8Char* text);
NA_API NAString* naNewStringWithTextFieldText(NATextField* textfield);
NA_API void naSetTextFieldFontKind(NATextField* textfield, NAFontKind kind);
NA_API void naSetTextFieldTextAlignment(NATextField* textfield, NATextAlignment alignment);
NA_API void naSetTextFieldFontKind(NATextField* textfield, NAFontKind kind);

// TextBox
NA_API NATextBox* naNewTextBox(NASize size);
NA_API void naSetTextBoxText(NATextBox* textbox, const NAUTF8Char* text);
NA_API void naSetTextBoxTextAlignment(NATextBox* textbox, NATextAlignment alignment);
NA_API void naSetTextBoxFontKind(NATextBox* textbox, NAFontKind kind);
NA_API void naSetTextBoxEditable(NATextBox* textbox, NABool editable);



// Mouse
NA_API const NAMouseStatus* naGetMouseStatus(void);
NA_API NAPos naGetMousePos(const NAMouseStatus* mousestatus);
NA_API NASize naGetMouseDelta(const NAMouseStatus* mousestatus);
NA_API void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage);
NA_API void naShowMouse(void);
NA_API void naHideMouse(void);

// Keyboard
NA_API NAKeyboardStatus naGetKeyboardStatus(void);



// ////////////////////////////////
// Other UI functions.

typedef enum{
  NA_ALERT_BOX_INFO,
  NA_ALERT_BOX_WARNING,
  NA_ALERT_BOX_ERROR
} NAAlertBoxType;

NA_API void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText);
NA_API void naOpenURLInBrowser(const NAUTF8Char* url);

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

NA_API void naOpenConsoleWindow(void);

#endif // NA_COMPILE_GUI == 1

#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_INCLUDED



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
