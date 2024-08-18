
#ifndef NA_UI_CORE_API_INCLUDED
#define NA_UI_CORE_API_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// ///////////////////////////
// This file contains API which should be hidden to the programmer but which
// is shared among the different implementations of the UI in Mac and Win.
// ///////////////////////////



#if NA_COMPILE_GUI == 1

#include "../NAAppDefinitions.h"
    
#include "../../NAStruct/NAList.h"
#include "../../NAUtility/NAThreading.h"
#include "../../NAUtility/NATranslator.h"
#include "../../NAMath/NAVectorAlgebra.h"
#include "../../NAVisual/NAColor.h"

NA_PROTOTYPE(NAFont);
NA_PROTOTYPE(NAMouseStatus);
NA_PROTOTYPE(NAKeyStroke);

NA_PROTOTYPE(NAUIImage);
NA_PROTOTYPE(NAApplication);
NA_PROTOTYPE(NAButton);
NA_PROTOTYPE(NACheckBox);
NA_PROTOTYPE(NAImageSpace);
NA_PROTOTYPE(NALabel);
NA_PROTOTYPE(NAMenu);
NA_PROTOTYPE(NAMenuItem);
NA_PROTOTYPE(NAMetalSpace);
NA_PROTOTYPE(NAOpenGLSpace);
NA_PROTOTYPE(NARadio);
NA_PROTOTYPE(NAScreen);
NA_PROTOTYPE(NASelect);
NA_PROTOTYPE(NASlider);
NA_PROTOTYPE(NASpace);
NA_PROTOTYPE(NATextBox);
NA_PROTOTYPE(NATextField);
NA_PROTOTYPE(NAWindow);

// Very much the same as the nativePtr, there are certain types which are
// casted differently on the different systems and therefore they are
// declared with a global void* or integer big enough to encapsulate all
// possible casts on all systems. Internally, they are caseted to these
// types:

typedef struct NA_UIElement NA_UIElement;

typedef struct NAEventReaction NAEventReaction;
typedef struct NAKeyboardShortcutReaction NAKeyboardShortcutReaction;

// //////////////////////////////
// NA_UIElement is the base type of any ui element. All ui element struct
// definitions have an NA_UIElement as the first entry:

struct NA_UIElement{
  NAUIElementType elementType;
  void*           parent;
  NAList          reactions;
  NAList          shortcuts;
  size_t          hoverReactionCount;
  uint32          flags;             // Currently only used in winAPI
  void*           nativePtr;         // The native pointer
  size_t          mouseTrackingCount;
  void*           mouseTracking;
};

struct NAApplication{
  NA_UIElement      uiElement;
  
  NAList            windows;         // A list of all windows
  NAList            uiElements;      // A list of all ui elements in use.
  
  NATranslator*     translator;
  NAFont*           systemFont;
  NAMouseStatus*    mouseStatus;     // The mouse cursor status
  NAKeyStroke*      keyStroke;       // The currently pressed key combination
  uint32            flags;

  const NAUTF8Char*       name;
  const NAUTF8Char*       companyName;
  const NAUTF8Char*       versionString;
  const NAUTF8Char*       buildString;
  const NAUTF8Char*       resourcePath;
  const NAUTF8Char*       iconPath;
};

struct NAButton{
  NA_UIElement uiElement;
  NAUTF8Char* text;
  NAUTF8Char* text2;
  const NAUIImage* uiImage;
  const NAUIImage* uiImage2;
  uint32 flags;
};

struct NACheckBox{
  NA_UIElement uiElement;
};

struct NAImageSpace{
  NA_UIElement uiElement;
  NAUIImage* uiImage;
};

struct NALabel{
  NA_UIElement uiElement;
  NAFont* font;
};

struct NAMenu{
  NA_UIElement uiElement;
  NAList       childs;
};

struct NAMenuItem{
  NA_UIElement uiElement;
};


struct NAMetalSpace{
  NA_UIElement uiElement;
  NAMat33d transformation;
};

struct NAOpenGLSpace{
  NA_UIElement uiElement;
  NAMat33d transformation;
};

struct NASelect{
  NA_UIElement uiElement;
  NAList       childs;
};

struct NARadio{
  NA_UIElement uiElement;
};

struct NAScreen{
  NA_UIElement uiElement;
};

struct NASlider{
  NA_UIElement uiElement;
  double staticValue; // range in [min, max]
  NABool sliderInMovement;
  double min;
  double max;
};

struct NASpace{
  NA_UIElement uiElement;
  NAList       childs;
  NAColor      backgroundColor;
  NABool       alternateBackground;
  NABool       dragsWindow;
};

struct NATextField{
  NA_UIElement uiElement;
  NAFont* font;
};

struct NATextBox{
  NA_UIElement uiElement;
  NAFont* font;
};

struct NAWindow{
  NA_UIElement uiElement;
  size_t       storageTag;
  void*        contentSpace;
  uint32       coreFlags;
  uint32       flags;
  NARect       windowedFrame;
};



struct NAEventReaction{
  void*              controller;
  NAReactionCallback callback;
  NAUICommand        command;
};

struct NAKeyboardShortcutReaction{
  void*              controller;
  NAReactionCallback callback;
  NAKeyStroke*       shortcut;
};



extern NAApplication* na_App;

#define NA_APPLICATION_FLAG_RUNNING                 0x01
#define NA_APPLICATION_FLAG_MOUSE_VISIBLE           0x02
#define NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING 0x04 // only for winAPI


#define NA_CORE_WINDOW_FLAG_FULLSCREEN             0x01
#define NA_CORE_WINDOW_FLAG_RESIZEABLE             0x02
#define NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE         0x04
#define NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING   0x08
#define NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS  0x10
#define NA_WINDOW_FLAG_FIRST_SYSTEM_DEPENDENT_FLAG 0x100

#define NA_WINDOW_PREF_STRING_POS_X "NAWindow_" NA_SIZE_T_PRI "_Pos_x"
#define NA_WINDOW_PREF_STRING_POS_Y "NAWindow_" NA_SIZE_T_PRI "_Pos_y"
#define NA_WINDOW_PREF_STRING_SIZE_WIDTH "NAWindow_" NA_SIZE_T_PRI "_Size_Width"
#define NA_WINDOW_PREF_STRING_SIZE_HEIGHT "NAWindow_" NA_SIZE_T_PRI "_Size_Height"

// //////////////////////////////////////////
// Hidden API

// Additional function prototypes and types used internally in NALib.
// Normally, these would be hidden in a separate .h file but in NALib, they are
// gathered here. You are free to use them but note that these are supposed to
// be helper functions.



// Native UI Pointer
// Calls the system specific method to clear/deallocate the given nativePtr.
NA_HAPI void na_ClearUINativePtr(void* nativePtr);



// NAUIElement
NA_HAPI void na_InitUIElement(NA_UIElement* uiElement, NAUIElementType elementType, void* nativePtr);
NA_HAPI void na_ClearUIElement(NA_UIElement* uiElement);

NA_HAPI void na_SetUIElementParent(NA_UIElement* uiElement, void* parent, NABool isElementAttachable);
NA_HAPI double na_GetUIElementXOffset(const NA_UIElement* elem);
NA_HAPI double na_GetUIElementYOffset(const NA_UIElement* elem);

NA_HAPI NA_UIElement* na_GetUIElementCommonParent(NA_UIElement* elem1, NA_UIElement* elem2);

NA_HDEF NABool na_GetApplicationMouseVisible();
NA_HDEF void na_SetApplicationMouseVisible(NABool visible);

NA_HAPI void na_SetUIElementMouseInside(NA_UIElement* elem, NABool inside);
NA_HAPI NABool na_GetUIElementMouseInside(const NA_UIElement* elem);
NA_HAPI void na_SetUIElementWINAPINotificationsBlocked(NA_UIElement* elem, NABool block);
NA_HAPI NABool na_GetUIElementWINAPINotificationsBlocked(const NA_UIElement* elem);

// Returns a pointer to the ui element which uses the given native pointer.
// Every gui element which is handeled by NALib uses a native struct which is
// dependent on the system running. When handling events, a nativePtr is sent
// but this nativePtr can in general not be mapped directly to a corresponding
// NALib struct. This function solves that. Slow, but does the job.
NA_HAPI void* na_GetUINALibEquivalent(void* nativePtr);

// Returns true if the given element has any reaction attached which responds
// to the given command.
NA_HAPI NABool na_UIHasElementCommandDispatches(const NA_UIElement* element, NAUICommand command);

// Dispatches a command with the given uiElement.
// 
// Dispatching happends according to the following rules:
// - If the given element has at least 1 reaction fitting to the given command,
//   these reaction will be called in the same order they were added. Afterwards
//   the function returns with NA_TRUE. No bubbling towards parent elements
//   happens automatically. If bubbling is desired, one has to do it manually by
//   calling the dispatch method with the parent element in the reaction methd.
// - If the given element has NO reactions fitting the given command, the
//   dispatch method will be called for the parent element of the given element.
//   The parent element is the parent given by the NALib structure, not the
//   native parent! The return value of this function is determined by the
//   return value of the recursive call.
// - If there is no parent available in the NALib structure, the function
//   returns NA_FALSE, meaning, the event has not been handeled at all.
//   This in turn will usually cause the NALib GUI elements to call the event
//   handling method given by the system or base class of the native element.
NA_HAPI NABool na_DispatchUIElementCommand(const NA_UIElement* element, NAUICommand command);

// To be implemented in the system dependent files:
NA_HAPI void na_RefreshUIElementNow(void* uiElement);

// Ensures the mouse being tracked as soon as there is one reason to do so.
NA_HAPI void na_RetainMouseTracking(NA_UIElement* uiElement);
NA_HAPI void na_ReleaseMouseTracking(NA_UIElement* uiElement);
NA_HAPI void na_UpdateMouseTracking(NA_UIElement* uiElement);
// System dependent implementation for adding a new mouse tracking object:
NA_HAPI void* na_AddMouseTracking(NA_UIElement* uiElement);
NA_HAPI void na_ClearMouseTracking(NA_UIElement* uiElement, void* mouseTracking);


// NAApplication
NA_HAPI NAApplication* na_NewApplication(void);
NA_HAPI NABool na_IsApplicationRunning(void);
NA_HAPI void na_InitApplication(NAApplication* application, void* nativePtr);
NA_HAPI void na_ClearApplication(NAApplication* application);
NA_HAPI NARect na_GetApplicationRect(const NAApplication* application);
NA_HAPI void na_SetApplicationRect(const NAApplication* application, NARect rect);
NA_HAPI const NAFont* na_GetApplicationSystemFont(const NAApplication* app);
NA_HAPI NAMouseStatus* na_GetApplicationMouseStatus(NAApplication* application);
// Takes the ownership of the keyStroke. Do not call naDelete on it!
NA_HAPI void na_SetApplicationKeyStroke(NAApplication* app, NAKeyStroke* keyStroke);

// NAButton
#define NA_BUTTON_BORDERED   0x01
#define NA_BUTTON_STATEFUL   0x02

NA_HAPI void na_InitButton(NAButton* button, void* nativePtr, const NAUTF8Char* text, const NAUTF8Char* text2, const NAUIImage* uiImage, const NAUIImage* uiImage2, uint32 flags);
NA_HAPI void na_ClearButton(NAButton* button);
NA_HAPI void na_setButtonText(NAButton* button, const NAUTF8Char* text);
NA_HAPI void na_setButtonText2(NAButton* button, const NAUTF8Char* text);
NA_HAPI void na_setButtonImage(NAButton* button, const NAUIImage* uiImage);
NA_HAPI void na_setButtonImage2(NAButton* button, const NAUIImage* uiImage);
NA_HAPI NARect na_GetButtonRect(const NA_UIElement* button);
NA_HAPI void na_SetButtonRect(NA_UIElement* button, NARect rect);

// NACheckBox
NA_HAPI void na_InitCheckBox(NACheckBox* checkBox, void* nativePtr);
NA_HAPI void na_ClearCheckBox(NACheckBox* checkBox);
NA_HAPI NARect na_GetCheckBoxRect(const NA_UIElement* checkBox);
NA_HAPI void na_SetCheckBoxRect(NA_UIElement* checkBox, NARect rect);

// NAImageSpace
NA_HAPI void na_InitImageSpace(NAImageSpace* imageSpace, void* nativePtr);
NA_HAPI void na_ClearImageSpace(NAImageSpace* imageSpace);
NA_HAPI NARect na_GetImageSpaceRect(const NA_UIElement* imageSpace);
NA_HAPI void na_SetImageSpaceRect(NA_UIElement* imageSpace, NARect rect);

// NALabel
NA_HAPI void na_InitLabel(NALabel* label, void* nativePtr);
NA_HAPI void na_ClearLabel(NALabel* label);
NA_HAPI NARect na_GetLabelRect(const NA_UIElement* space);
NA_HAPI void na_SetLabelRect(NA_UIElement* label, NARect rect);

// NAMenu
NA_HAPI void na_InitMenu(NAMenu* menu, void* nativePtr, NA_UIElement* parent);
NA_HAPI void na_ClearMenu(NAMenu* menu);
NA_HAPI void na_AddMenuChild(NAMenu* menu, NAMenuItem* child, const NAMenuItem* itemAt);
NA_HAPI NARect na_GetMenuRect(const NA_UIElement* menu);
NA_HAPI void na_SetMenuRect(NA_UIElement* menu, NARect rect);

// NAMenuItem
NA_HAPI void na_InitMenuItem(NAMenuItem* menuItem, void* nativePtr, NA_UIElement* parent);
NA_HAPI void na_ClearMenuItem(NAMenuItem* menuItem);
NA_HAPI NARect na_GetMenuItemRect(const NA_UIElement* menuItem);
NA_HAPI void na_SetMenuItemRect(NA_UIElement* menuItem, NARect rect);

// NAMetalSpace
NA_HAPI void na_InitMetalSpace(NAMetalSpace* metalSpace, void* nativePtr);
NA_HAPI void na_ClearMetalSpace(NAMetalSpace* metalSpace);
NA_HAPI NARect na_GetMetalSpaceRect(const NA_UIElement* metalSpace);
NA_HAPI void na_SetMetalSpaceRect(NA_UIElement* metalSpace, NARect rect);

// NAOpenGLSpace
NA_HAPI void na_InitOpenGLSpace(NAOpenGLSpace* openGLSpace, void* nativePtr);
NA_HAPI void na_ClearOpenGLSpace(NAOpenGLSpace* openGLSpace);
NA_HAPI NARect na_GetOpenGLSpaceRect(const NA_UIElement* openGLSpace);
NA_HAPI void na_SetOpenGLSpaceRect(NA_UIElement* openGLSpace, NARect rect);

// NARadio
NA_HAPI void na_InitRadio(NARadio* radio, void* nativePtr);
NA_HAPI void na_ClearRadio(NARadio* radio);
NA_HAPI NARect na_GetRadioRect(const NA_UIElement* radio);
NA_HAPI void na_SetRadioRect(NA_UIElement* radio, NARect rect);

// NAScreen
NA_HAPI void na_InitScreen(NAScreen* screen, void* nativePtr);
NA_HAPI void na_ClearScreen(NAScreen* screen);
NA_HAPI NARect na_GetScreenRect(const NA_UIElement* screen);
NA_HAPI void na_SetScreenRect(NA_UIElement* screen, NARect rect);

// NASelect
NA_HAPI void na_InitSelect(NASelect* select, void* nativePtr);
NA_HAPI void na_ClearSelect(NASelect* select);
NA_HAPI void na_AddSelectChild(NASelect* select, NAMenuItem* child, const NAMenuItem* itemAt);
NA_HAPI NARect na_GetSelectRect(const NA_UIElement* select);
NA_HAPI void na_SetSelectRect(NA_UIElement* select, NARect rect);

// NASlider
NA_HAPI void na_InitSlider(NASlider* slider, void* nativePtr);
NA_HAPI void na_ClearSlider(NASlider* slider);
NA_HAPI NARect na_GetSliderRect(const NA_UIElement* slider);
NA_HAPI void na_SetSliderRect(NA_UIElement* slider, NARect rect);

// NASpace
NA_HAPI void na_InitSpace(NASpace* space, void* nativePtr);
NA_HAPI void na_ClearSpace(NASpace* space);
NA_HAPI void na_AddSpaceChild(NASpace*, NA_UIElement* child);
NA_HAPI void na_RemoveSpaceChild(NASpace* space, NA_UIElement* child);
NA_HAPI NARect na_GetSpaceRect(const NA_UIElement* space);
NA_HAPI void na_SetSpaceRect(NA_UIElement* space, NARect rect);

// NATextBox
NA_HAPI void na_InitTextBox(NATextBox* textBox, void* nativePtr);
NA_HAPI void na_ClearTextBox(NATextBox* textBox);
NA_HAPI NARect na_GetTextBoxRect(const NA_UIElement* textBox);
NA_HAPI void na_SetTextBoxRect(NA_UIElement* textBox, NARect rect);

// NATextField
NA_HAPI void na_InitTextField(NATextField* textField, void* nativePtr);
NA_HAPI void na_ClearTextField(NATextField* textField);
NA_HAPI NARect na_GetTextFieldRect(const NA_UIElement* textField);
NA_HAPI void na_SetTextFieldRect(NA_UIElement* textField, NARect rect);

// NAWindow
NA_HAPI void na_InitWindow(NAWindow* window, void* nativePtr, NASpace* contentSpace, NABool fullScreen, NABool resizeable, NARect windowedFrame);
NA_HAPI void na_ClearWindow(NAWindow* window);
NA_HAPI void na_RememberWindowPosition(const NAWindow* window);
NA_HAPI void na_RetainWindowMouseTracking(NAWindow* window);
NA_HAPI void na_ReleaseWindowMouseTracking(NAWindow* window);
NA_HAPI NARect na_GetWindowAbsoluteInnerRect(const NA_UIElement* window);
NA_HAPI NARect na_GetWindowRect(const NA_UIElement* window);
NA_HAPI void na_SetWindowRect(NA_UIElement* window, NARect rect);




// Mouse related functions
NA_HAPI NAMouseStatus* na_AllocMouseStatus();
NA_HAPI void na_DeallocMouseStatus(NAMouseStatus* status);

NA_HAPI void na_SetMouseButtonPressed(NAMouseStatus* status, NAMouseButton button, NABool pressed);

NA_HAPI void na_SetMouseWarpedTo(NAMouseStatus* status, NAPos newpos);
NA_HAPI void na_SetMouseMovedTo(NAMouseStatus* status, NAPos newpos);
NA_HAPI void na_SetMouseMovedByDiff(NAMouseStatus* status, double deltaX, double deltaY);
NA_HAPI void na_SetMouseEnteredAtPos(NAMouseStatus* status, NAPos newpos);
NA_HAPI void na_SetMouseExitedAtPos(NAMouseStatus* status, NAPos newpos);




// Font related functions
NA_HAPI NAFont* na_CreateFont(void* nativePtr, const NAString* name, uint32 flags, double size);
NA_HAPI void na_DestructFontNativePtr(void* nativePtr);



#endif // NA_COMPILE_GUI == 1

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
