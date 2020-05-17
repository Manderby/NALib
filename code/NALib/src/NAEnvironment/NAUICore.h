
#ifndef NA_UI_CORE_API_INCLUDED
#define NA_UI_CORE_API_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// ///////////////////////////
// This file contains API which should be hidden to the programmer but which
// is shared among the different implementations of the UI in Mac and Win.
// ///////////////////////////

#include "../NAUI.h"

#if (NA_COMPILE_GUI == 1)

#include "../NAList.h"
#include "../NATranslator.h"

// Very much the same as the native ID, there are certain types which are
// casted differently on the different systems and therefore they are
// declared with a global void* or integer big enough to encapsulate all
// possible casts on all systems. Internally, they are caseted to these
// types:

typedef struct NACoreUIElement      NACoreUIElement;

typedef struct NACoreApplication    NACoreApplication;
typedef struct NACoreScreen         NACoreScreen;
typedef struct NACoreWindow         NACoreWindow;
typedef struct NACoreSpace          NACoreSpace;
typedef struct NACoreImageSpace     NACoreImageSpace;
typedef struct NACoreOpenGLSpace    NACoreOpenGLSpace;
typedef struct NACoreButton         NACoreButton;
typedef struct NACoreRadio          NACoreRadio;
typedef struct NACoreCheckBox       NACoreCheckBox;
typedef struct NACoreLabel          NACoreLabel;
typedef struct NACoreTextField      NACoreTextField;
typedef struct NACoreTextBox        NACoreTextBox;

typedef struct NACoreReaction       NACoreReaction;
typedef struct NACoreKeyboardShortcutReaction NACoreKeyboardShortcutReaction;

typedef void*  NAFont;

// //////////////////////////////
//
// NACoreUIElement is the base type of any ui element. All ui element struct
// definitions have an NACoreUIElement as the first entry:

struct NACoreUIElement{
  NARefCount refcount;
  NAUIElementType elementtype;
  NACoreUIElement* parent;
  NAList reactions;
  NAList shortcuts;
  NABool mouseinside;
  NABool allownotifications;
  void* nativeID;               // The native object
};

struct NACoreApplication{
  NACoreUIElement   uielement;
  NAList            uielements;      // A list of all ui elements.
  NATranslator*     translator;
  NAMouseStatus     mouseStatus;     // The mouse cursor status
  NAKeyboardStatus  keyboardStatus;  // The keyboard status
  NAInt             flags;

  NAUTF8Char* name;
  NAUTF8Char* companyName;
  NAUTF8Char* versionString;
  NAUTF8Char* buildString;
  NAUTF8Char* iconPath;
};

struct NACoreScreen{
  NACoreUIElement uielement;
};

struct NACoreWindow{
  NACoreUIElement uielement;
  NAInt storageTag;
  NACoreSpace* contentspace;
  NAInt flags;
  NARect windowedframe;
};

struct NACoreSpace{
  NACoreUIElement uielement;
  NABool alternatebackground;
};

struct NACoreImageSpace{
  NACoreUIElement uielement;
};

struct NACoreOpenGLSpace{
  NACoreUIElement uielement;
};

struct NACoreButton{
  NACoreUIElement uielement;
};

struct NACoreRadio{
  NACoreUIElement uielement;
};

struct NACoreCheckBox{
  NACoreUIElement uielement;
};

struct NACoreLabel{
  NACoreUIElement uielement;
};

struct NACoreTextField{
  NACoreUIElement uielement;
};

struct NACoreTextBox{
  NACoreUIElement uielement;
};

struct NACoreReaction{
  void* controller;
  NAUICommand command;
  NAReactionHandler handler;
};

struct NACoreKeyboardShortcutReaction{
  void* controller;
  NAKeyboardStatus shortcut;
  NAReactionHandler handler;
};



extern NACoreApplication* na_app;

#define NA_APPLICATION_FLAG_RUNNING               0x01
#define NA_APPLICATION_FLAG_MOUSE_VISIBLE         0x02

#define NA_CORE_WINDOW_FLAG_FULLSCREEN            0x01
#define NA_CORE_WINDOW_FLAG_RESIZEABLE            0x02
#define NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE        0x04
#define NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING  0x08

#define NA_WINDOW_PREF_STRING_POS_X "NAWindow_%" NA_PRIi "_Pos_x"
#define NA_WINDOW_PREF_STRING_POS_Y "NAWindow_%" NA_PRIi "_Pos_y"
#define NA_WINDOW_PREF_STRING_SIZE_WIDTH "NAWindow_%" NA_PRIi "_Size_Width"
#define NA_WINDOW_PREF_STRING_SIZE_HEIGHT "NAWindow_%" NA_PRIi "_Size_Height"

// //////////////////////////////////////////
// Hidden API

// Additional function prototypes and types used internally in NALib.
// Normally, these would be hidden in a separate .h file but in NALib, they are
// gathered here. You are free to use them but note that these are supposed to
// be helper functions.

NA_HAPI void naUnregisterCoreUIElement(NACoreUIElement* coreuielement);
NA_HAPI void naSetUIElementParent(NAUIElement* uielement, NAUIElement* parent);
NA_HAPI NACoreUIElement* naGetUIElementCommonParent(NACoreUIElement* elem1, NACoreUIElement* elem2);
NA_HAPI void naBlockUIElementNotifications(NACoreUIElement* elem);
NA_HAPI void naAllowUIElementNotifications(NACoreUIElement* elem);
NA_HAPI NABool naAreUIElementNotificationsAllowed(NACoreUIElement* elem);

NA_HAPI NAApplication* naNewApplication(void);
NA_HAPI void naInitCoreApplication(NACoreApplication* coreapplication, NANativeID nativeId);
NA_HAPI void naClearCoreApplication(NACoreApplication* coreapplication);

NA_HAPI void naInitCoreScreen(NACoreScreen* corescreen, void* nativeId);
NA_HAPI void naClearCoreScreen(NACoreScreen* corescreen);

NA_HAPI void naInitCoreWindow(NACoreWindow* coreWindow, void* nativeId, NACoreSpace* contentspace, NABool fullscreen, NABool resizeable, NARect windowedframe);
NA_HAPI void naClearCoreWindow(NACoreWindow* coreWindow);
NA_HAPI void naRememberWindowPosition(NACoreWindow* coreWindow);
NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window);

NA_HAPI void naInitCoreSpace(NACoreSpace* corespace, void* nativeId);
NA_HAPI void naClearCoreSpace(NACoreSpace* corecorespace);

NA_HAPI void naInitCoreImageSpace(NACoreImageSpace* coreImageSpace, void* nativeId);
NA_HAPI void naClearCoreImageSpace(NACoreImageSpace* corecoreImageSpace);

NA_HAPI void naInitCoreOpenGLSpace(NACoreOpenGLSpace* coreOpenGLspace, void* nativeId);
NA_HAPI void naClearCoreOpenGLSpace(NACoreOpenGLSpace* corecoreOpenGLspace);

NA_HAPI void naInitCoreButton(NACoreButton* coreButton, void* nativeId);
NA_HAPI void naClearCoreButton(NACoreButton* coreButton);

NA_HAPI void naInitCoreRadio(NACoreRadio* coreRadio, void* nativeId);
NA_HAPI void naClearCoreRadio(NACoreRadio* coreRadio);

NA_HAPI void naInitCoreCheckBox(NACoreCheckBox* coreCheckBox, void* nativeId);
NA_HAPI void naClearCoreCheckBox(NACoreCheckBox* coreCheckBox);

NA_HAPI void naInitCoreLabel(NACoreLabel* coreLabel, void* nativeId);
NA_HAPI void naClearCoreLabel(NACoreLabel* coreLabel);

NA_HAPI void naInitCoreTextField(NACoreTextField* coreTextField, void* nativeId);
NA_HAPI void naClearCoreTextField(NACoreTextField* coreTextField);

NA_HAPI void naInitCoreTextBox(NACoreTextBox* coreTextBox, void* nativeId);
NA_HAPI void naClearCoreTextBox(NACoreTextBox* coreTextBox);



// Returns a pointer to the ui element which uses the given native ID.
// Every gui element which is handeled by NALib uses a native struct which is
// dependent on the system running. When handling events, a native ID is sent
// but this native ID can in general not be mapped directly to a corresponding
// NALib struct. This function solves that. Slow, but does the job.
NA_HAPI void* naGetUINALibEquivalent(void* nativeID);


NA_HAPI NABool naIsCoreApplicationRunning(void);

NAFont getFontWithKind(NAFontKind kind);

// Dispatches a command with the given uielement.
// As long as the command has not been finished using NA_TRUE as a return value
// in the NAReactionHandler function handler, it will be bubbling upwards in
// the following order:
// - First responder
// - containing space
// - window
// - application
// - discard command as unhandled.
// The function will return NA_TRUE if the event shall not be processed any
// further. If this function returns NA_FALSE, the event shall still be
// processed by the calling function. This is especially important on Windows
// where non-handling of certain events might interrupt the whole messaging
// chain.
NA_HAPI NABool naDispatchUIElementCommand(  NACoreUIElement* element, NAUICommand command);

// Mouse related functions
NA_HAPI void naSetMouseWarpedTo(NAPos newpos);
NA_HAPI void naSetMouseMovedByDiff(double deltaX, double deltaY);
NA_HAPI void naSetMouseEnteredAtPos(NAPos newpos);
NA_HAPI void naSetMouseExitedAtPos(NAPos newpos);



// To be implemented in the system dependent files:

NA_HAPI void naRefreshUIElementNow(NAUIElement* uielement);

// Calls the system specific method to clear/deallocate the given native id.
NA_HAPI void naClearUINativeId(NANativeID nativeId);

NA_HAPI void* naAllocMouseTracking(NANativeID nativeId);
NA_HAPI void naDeallocMouseTracking(void* tracking);





#endif // (NA_COMPILE_GUI == 1)

#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_CORE_API_INCLUDED



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
