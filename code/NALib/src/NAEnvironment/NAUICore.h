
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

typedef struct NA_Application    NA_Application;
typedef struct NA_Screen         NA_Screen;
typedef struct NA_Window         NA_Window;
typedef struct NA_Space          NA_Space;
typedef struct NA_ImageSpace     NA_ImageSpace;
typedef struct NA_OpenGLSpace    NA_OpenGLSpace;
typedef struct NA_Button         NA_Button;
typedef struct NA_Radio          NA_Radio;
typedef struct NA_CheckBox       NA_CheckBox;
typedef struct NA_Label          NA_Label;
typedef struct NA_TextField      NA_TextField;
typedef struct NA_TextBox        NA_TextBox;

typedef struct NA_Reaction       NA_Reaction;
typedef struct NA_KeyboardShortcutReaction NA_KeyboardShortcutReaction;

typedef void*  NAFont;

// //////////////////////////////
//
// NA_UIElement is the base type of any ui element. All ui element struct
// definitions have an NA_UIElement as the first entry:

struct NA_UIElement{
  NARefCount refCount;
  NAUIElementType elementType;
  NA_UIElement* parent;
  NAList reactions;
  NAList shortcuts;
  NABool mouseInside;
  NABool allowNotifications;
  void* nativeID;               // The native object
};

struct NA_Application{
  NA_UIElement   uiElement;
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

struct NA_Screen{
  NA_UIElement uiElement;
};

struct NA_Window{
  NA_UIElement uiElement;
  NAInt storageTag;
  NA_Space* contentspace;
  NAInt flags;
  NARect windowedframe;
};

struct NA_Space{
  NA_UIElement uiElement;
  NABool alternatebackground;
};

struct NA_ImageSpace{
  NA_UIElement uiElement;
};

struct NA_OpenGLSpace{
  NA_UIElement uiElement;
};

struct NA_Button{
  NA_UIElement uiElement;
};

struct NA_Radio{
  NA_UIElement uiElement;
};

struct NA_CheckBox{
  NA_UIElement uiElement;
};

struct NA_Label{
  NA_UIElement uiElement;
};

struct NA_TextField{
  NA_UIElement uiElement;
};

struct NA_TextBox{
  NA_UIElement uiElement;
};

struct NA_Reaction{
  void* controller;
  NAUICommand command;
  NAReactionHandler handler;
};

struct NA_KeyboardShortcutReaction{
  void* controller;
  NAKeyboardStatus shortcut;
  NAReactionHandler handler;
};



extern NA_Application* na_App;

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

NA_HHAPI void na_UnregisterCoreUIElement(NA_UIElement* coreUiElement);
NA_HHAPI void na_SetUIElementParent(NAUIElement* uiElement, NAUIElement* parent);
NA_HHAPI NA_UIElement* na_GetUIElementCommonParent(NA_UIElement* elem1, NA_UIElement* elem2);
NA_HHAPI void na_BlockUIElementNotifications(NA_UIElement* elem);
NA_HHAPI void na_AllowUIElementNotifications(NA_UIElement* elem);
NA_HHAPI NABool na_AreUIElementNotificationsAllowed(NA_UIElement* elem);

NA_HHAPI NAApplication* na_NewApplication(void);
NA_HHAPI void na_InitCoreApplication(NA_Application* coreApplication, NANativeID nativeId);
NA_HHAPI void na_ClearCoreApplication(NA_Application* coreApplication);

NA_HHAPI void na_InitCoreScreen(NA_Screen* corescreen, void* nativeId);
NA_HHAPI void na_ClearCoreScreen(NA_Screen* corescreen);

NA_HHAPI void na_InitCoreWindow(NA_Window* coreWindow, void* nativeId, NA_Space* contentspace, NABool fullscreen, NABool resizeable, NARect windowedframe);
NA_HHAPI void na_ClearCoreWindow(NA_Window* coreWindow);
NA_HHAPI void na_RememberWindowPosition(NA_Window* coreWindow);
NA_HHAPI NARect na_GetWindowAbsoluteInnerRect(NA_UIElement* window);

NA_HHAPI void na_InitCoreSpace(NA_Space* corespace, void* nativeId);
NA_HHAPI void na_ClearCoreSpace(NA_Space* corecorespace);

NA_HHAPI void na_InitCoreImageSpace(NA_ImageSpace* coreImageSpace, void* nativeId);
NA_HHAPI void na_ClearCoreImageSpace(NA_ImageSpace* corecoreImageSpace);

NA_HHAPI void na_InitCoreOpenGLSpace(NA_OpenGLSpace* coreOpenGLspace, void* nativeId);
NA_HHAPI void na_ClearCoreOpenGLSpace(NA_OpenGLSpace* corecoreOpenGLspace);

NA_HHAPI void na_InitCoreButton(NA_Button* coreButton, void* nativeId);
NA_HHAPI void na_ClearCoreButton(NA_Button* coreButton);

NA_HHAPI void na_InitCoreRadio(NA_Radio* coreRadio, void* nativeId);
NA_HHAPI void na_ClearCoreRadio(NA_Radio* coreRadio);

NA_HHAPI void na_InitCoreCheckBox(NA_CheckBox* coreCheckBox, void* nativeId);
NA_HHAPI void na_ClearCoreCheckBox(NA_CheckBox* coreCheckBox);

NA_HHAPI void na_InitCoreLabel(NA_Label* coreLabel, void* nativeId);
NA_HHAPI void na_ClearCoreLabel(NA_Label* coreLabel);

NA_HHAPI void na_InitCoreTextField(NA_TextField* coreTextField, void* nativeId);
NA_HHAPI void na_ClearCoreTextField(NA_TextField* coreTextField);

NA_HHAPI void na_InitCoreTextBox(NA_TextBox* coreTextBox, void* nativeId);
NA_HHAPI void na_ClearCoreTextBox(NA_TextBox* coreTextBox);



// Returns a pointer to the ui element which uses the given native ID.
// Every gui element which is handeled by NALib uses a native struct which is
// dependent on the system running. When handling events, a native ID is sent
// but this native ID can in general not be mapped directly to a corresponding
// NALib struct. This function solves that. Slow, but does the job.
NA_HHAPI void* na_GetUINALibEquivalent(void* nativeID);


NA_HHAPI NABool na_IsCoreApplicationRunning(void);

NA_HHAPI NAFont na_GetFontWithKind(NAFontKind kind);

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
NA_HHAPI NABool na_DispatchUIElementCommand(  NA_UIElement* element, NAUICommand command);

// Mouse related functions
NA_HHAPI void na_SetMouseWarpedTo(NAPos newpos);
NA_HHAPI void na_SetMouseMovedByDiff(double deltaX, double deltaY);
NA_HHAPI void na_SetMouseEnteredAtPos(NAPos newpos);
NA_HHAPI void na_SetMouseExitedAtPos(NAPos newpos);



// To be implemented in the system dependent files:

NA_HHAPI void na_RefreshUIElementNow(NAUIElement* uiElement);

// Calls the system specific method to clear/deallocate the given native id.
NA_HHAPI void na_ClearUINativeId(NANativeID nativeId);

NA_HHAPI void* na_AllocMouseTracking(NANativeID nativeId);
NA_HHAPI void na_DeallocMouseTracking(void* tracking);





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
