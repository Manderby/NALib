
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

typedef struct NA_UIElement      NA_UIElement;

typedef struct NAEventReaction       NAEventReaction;
typedef struct NAKeyboardShortcutReaction NAKeyboardShortcutReaction;

typedef void*  NAFont;

// //////////////////////////////
//
// NA_UIElement is the base type of any ui element. All ui element struct
// definitions have an NA_UIElement as the first entry:

struct NA_UIElement{
  NARefCount refCount;
  NAUIElementType elementType;
  void* parent;
  NAList reactions;
  NAList shortcuts;
  NABool mouseInside;
  NABool allowNotifications;
  void* nativeID;               // The native object
};

struct NAApplication{
  NA_UIElement      uiElement;
  NAList            uiElements;      // A list of all ui elements.
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

struct NAScreen{
  NA_UIElement uiElement;
};

struct NAWindow{
  NA_UIElement uiElement;
  NAInt storageTag;
  NASpace* contentSpace;
  NAInt flags;
  NARect windowedFrame;
};

struct NASpace{
  NA_UIElement uiElement;
  NABool alternatebackground;
};

struct NAImageSpace{
  NA_UIElement uiElement;
};

struct NAOpenGLSpace{
  NA_UIElement uiElement;
};

struct NAButton{
  NA_UIElement uiElement;
};

struct NARadio{
  NA_UIElement uiElement;
};

struct NACheckBox{
  NA_UIElement uiElement;
};

struct NALabel{
  NA_UIElement uiElement;
};

struct NATextField{
  NA_UIElement uiElement;
};

struct NATextBox{
  NA_UIElement uiElement;
};

struct NAEventReaction{
  void* controller;
  NAUICommand command;
  NAReactionHandler handler;
};

struct NAKeyboardShortcutReaction{
  void* controller;
  NAKeyboardStatus shortcut;
  NAReactionHandler handler;
};



extern NAApplication* na_App;

#define NA_APPLICATION_FLAG_RUNNING               0x01
#define NA_APPLICATION_FLAG_MOUSE_VISIBLE         0x02

#define NA_CORE_WINDOW_FLAG_FULLSCREEN            0x01
#define NA_CORE_WINDOW_FLAG_RESIZEABLE            0x02
#define NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE        0x04
#define NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING  0x08

#define NA_WINDOW_PREF_STRING_POS_X "NAWindow_%d_Pos_x"
#define NA_WINDOW_PREF_STRING_POS_Y "NAWindow_%d_Pos_y"
#define NA_WINDOW_PREF_STRING_SIZE_WIDTH "NAWindow_%d_Size_Width"
#define NA_WINDOW_PREF_STRING_SIZE_HEIGHT "NAWindow_%d_Size_Height"

// //////////////////////////////////////////
// Hidden API

// Additional function prototypes and types used internally in NALib.
// Normally, these would be hidden in a separate .h file but in NALib, they are
// gathered here. You are free to use them but note that these are supposed to
// be helper functions.

NA_HAPI void na_UnregisterUIElement(NA_UIElement* uiElement);
NA_HAPI void na_SetUIElementParent(void* uiElement, void* parent);
NA_HAPI NA_UIElement* na_GetUIElementCommonParent(NA_UIElement* elem1, NA_UIElement* elem2);
NA_HAPI void na_BlockUIElementNotifications(NA_UIElement* elem);
NA_HAPI void na_AllowUIElementNotifications(NA_UIElement* elem);
NA_HAPI NABool na_AreUIElementNotificationsAllowed(NA_UIElement* elem);

NA_HAPI NAApplication* na_NewApplication(void);
NA_HAPI void na_InitApplication(NAApplication* application, NANativeID nativeId);
NA_HAPI void na_ClearApplication(NAApplication* application);

NA_HAPI void na_InitScreen(NAScreen* screen, void* nativeId);
NA_HAPI void na_ClearScreen(NAScreen* screen);

NA_HAPI void na_InitWindow(NAWindow* window, void* nativeId, NASpace* contentSpace, NABool fullScreen, NABool resizeable, NARect windowedFrame);
NA_HAPI void na_ClearWindow(NAWindow* window);
NA_HAPI void na_RememberWindowPosition(NAWindow* window);
NA_HAPI NARect na_GetWindowAbsoluteInnerRect(NA_UIElement* window);

NA_HAPI void na_InitSpace(NASpace* space, void* nativeId);
NA_HAPI void na_ClearSpace(NASpace* space);

NA_HAPI void na_InitImageSpace(NAImageSpace* imageSpace, void* nativeId);
NA_HAPI void na_ClearImageSpace(NAImageSpace* imageSpace);

NA_HAPI void na_InitOpenGLSpace(NAOpenGLSpace* openGLSpace, void* nativeId);
NA_HAPI void na_ClearOpenGLSpace(NAOpenGLSpace* openGLSpace);

NA_HAPI void na_InitButton(NAButton* button, void* nativeId);
NA_HAPI void na_ClearButton(NAButton* button);

NA_HAPI void na_InitRadio(NARadio* radio, void* nativeId);
NA_HAPI void na_ClearRadio(NARadio* radio);

NA_HAPI void na_InitCheckBox(NACheckBox* checkBox, void* nativeId);
NA_HAPI void na_ClearCheckBox(NACheckBox* checkBox);

NA_HAPI void na_InitLabel(NALabel* label, void* nativeId);
NA_HAPI void na_ClearLabel(NALabel* label);

NA_HAPI void na_InitTextField(NATextField* textField, void* nativeId);
NA_HAPI void na_ClearTextField(NATextField* textField);

NA_HAPI void na_InitTextBox(NATextBox* textBox, void* nativeId);
NA_HAPI void na_ClearTextBox(NATextBox* textBox);



// Returns a pointer to the ui element which uses the given native ID.
// Every gui element which is handeled by NALib uses a native struct which is
// dependent on the system running. When handling events, a native ID is sent
// but this native ID can in general not be mapped directly to a corresponding
// NALib struct. This function solves that. Slow, but does the job.
NA_HAPI void* na_GetUINALibEquivalent(void* nativeID);


NA_HAPI NABool na_IsApplicationRunning(void);

NA_HAPI NAFont na_GetFontWithKind(NAFontKind kind);

// Dispatches a command with the given uiElement.
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
NA_HAPI NABool na_DispatchUIElementCommand(  NA_UIElement* element, NAUICommand command);

// Mouse related functions
NA_HAPI void na_SetMouseWarpedTo(NAPos newpos);
NA_HAPI void na_SetMouseMovedByDiff(double deltaX, double deltaY);
NA_HAPI void na_SetMouseEnteredAtPos(NAPos newpos);
NA_HAPI void na_SetMouseExitedAtPos(NAPos newpos);



// To be implemented in the system dependent files:

NA_HAPI void na_RefreshUIElementNow(void* uiElement);

// Calls the system specific method to clear/deallocate the given native id.
NA_HAPI void na_ClearUINativeId(NANativeID nativeId);

NA_HAPI void* na_AllocMouseTracking(NANativeID nativeId);
NA_HAPI void na_DeallocMouseTracking(void* tracking);





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
