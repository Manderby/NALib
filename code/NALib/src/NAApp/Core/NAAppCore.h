
#ifndef NA_UI_CORE_API_INCLUDED
#define NA_UI_CORE_API_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// ///////////////////////////
// This file contains API which should be hidden to the programmer but which
// is shared among the different implementations of the UI in Mac and Win.
// ///////////////////////////


#include "../../NABase/NAConfiguration.h"
#if NA_COMPILE_GUI == 1



#include "../../NAStruct/NAList.h"
#include "../../NAMath/NAVectorAlgebra.h"

NA_PROTOTYPE(NANotifier);
NA_PROTOTYPE(NATranslator);

#include "../NAApp.h"



#if NA_DEBUG
typedef struct NA_LayoutRects NA_LayoutRects;
struct NA_LayoutRects {
  NARect marginRect;
  NARect paddingRect;
  NARect contentRect;
};
#endif // NA_DEBUG



// //////////////////////////////
// NA_UIElement is the base type of any ui element. All ui element struct
// definitions have an NA_UIElement as the first entry:

NA_PROTOTYPE(NA_UIElement);
struct NA_UIElement{
  NAUIElementType elementType;
  void*           parent;
  NAList          reactions;
  NAList          shortcuts;
  uint32          flags;
  void*           nativePtr;         // The native pointer
  size_t          hoverReactionCount;
  size_t          mouseTrackingCount;
  void*           mouseTracking;
  #if NA_DEBUG
    NA_LayoutRects* layoutRects;
  #endif
};

struct NAApplication{
  NA_UIElement      uiElement;
  
  NAList            screens;        // A list of all screens
  NAList            windows;        // A list of all windows
  NAList            uiElements;     // A list of all ui elements in use.
  
  NARect            totalRect;      // The absolute rect of all screens.
  
  NATranslator*     translator;
  NANotifier*       notifier;
  NAFont*           systemFont;
  NAMouseStatus*    mouseStatus;    // The mouse cursor status
  NAKeyStroke*      keyStroke;      // The currently pressed key combination
  uint32            flags;

  NAString*       appName;
  NAString*       companyName;
  NAString*       versionString;
  NAString*       buildString;
  NAString*       resourceBaseUrl;
  NAString*       iconUrl;
};

struct NAButton{
  NA_UIElement uiElement;
  NAUTF8Char* text;
  NAUTF8Char* text2;
  NAUTF8Char* hint;
  NAFont* font;
  const NAImageSet* imageSet;
  const NAImageSet* imageSet2;
  uint32 flags;
};

struct NACheckBox{
  NA_UIElement uiElement;
  NAFont* font;
};

struct NAImageSpace{
  NA_UIElement uiElement;
  NAImageSet* imageSet;
};

struct NALabel{
  NA_UIElement uiElement;
  NAAlignment alignment;
  NAFont* font;
  NAColor* textColor;
  NABool enabled;
};

struct NAMenu{
  NA_UIElement uiElement;
  NAList childs;
};

struct NAMenuItem{
  NA_UIElement uiElement;
  NAMenu* subMenu;
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
  NAList childs;
  NAFont* font;
};

struct NARadio{
  NA_UIElement uiElement;
  NAFont* font;
};

struct NAScreen{
  NA_UIElement uiElement;
  NABool isMain;
  NAString* name;
  NARect rect;
  NAPos relativeCenter; // relative pos of screen center in whole screen setup.
  double uiScale;
  NAList windows; // References. Does NOT own the windows. The application does.
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
  NAList childs;
  NALayoutDirections layoutDirections;
  NAColor* backgroundColor;
  NABool alternateBackground;
  NABool dragsWindow;
};

struct NATextField{
  NA_UIElement uiElement;
  NAFont* font;
  NAAlignment alignment;
};

struct NATextBox{
  NA_UIElement uiElement;
  NAFont* font;
  NAAlignment alignment;
};

struct NAWindow{
  NA_UIElement uiElement;
  size_t       storageTag;
  void*        contentSpace;
  uint32       coreFlags;
  uint32       flags;
  NARect       windowedFrame;
};



NA_PROTOTYPE(NA_EventReaction);
struct NA_EventReaction{
  void*              controller;
  NAReactionCallback callback;
  NAUICommand        command;
};

NA_PROTOTYPE(NA_KeyboardShortcutReaction);
struct NA_KeyboardShortcutReaction{
  void*              controller;
  NAReactionCallback callback;
  NAKeyStroke*       shortcut;
};



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
NA_HAPI void na_InitCoreUIElement(NA_UIElement* uiElement, NAUIElementType elementType, void* nativePtr);
NA_HAPI void na_ClearCoreUIElement(NA_UIElement* uiElement);

NA_HAPI void na_IncCoreUIElementHoverTrackingCount(NA_UIElement* uiElement);

NA_HAPI void na_UndebugUIElement(const NA_UIElement* elem);
NA_HDEF NABool na_IsUIElementBeingDebugged(const NA_UIElement* elem);

// System dependent implementations:
NA_HAPI void na_InitSystemUIElement(NA_UIElement* uiElement, void* nativePtr);
NA_HAPI void na_ClearSystemUIElement(void* nativePtr);

NA_HAPI void na_SetUIElementParent(NA_UIElement* uiElement, void* parent);
NA_HAPI NARect na_GetUIElementOffsetRect(const NA_UIElement* uiElement);

NA_HAPI const NA_UIElement* na_GetUIElementCommonParent(const NA_UIElement* elem1, const NA_UIElement* elem2);

NA_HAPI NABool na_GetApplicationMouseVisible();
NA_HAPI void na_SetApplicationMouseVisible(NABool visible);
NA_HAPI void na_UpdateApplicationIconUrl(void);

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

// Returns true if the given element has a reaction attached which responds
// to the given command.
NA_HAPI NABool na_UIHasElementCommandDispatches(const NA_UIElement* uiElement, NAUICommand command);

// Dispatches a command with the given uiElement.
// 
// Dispatching happends according to the following rules:
// - If the given element has a reaction fitting to the given command, that
//   reaction will be called. Afterwards the function returns with NA_TRUE.
//   No bubbling towards parent elements happens automatically. If bubbling is
//   desired, one has to do it manually by calling the dispatch method with
//   the parent element in the reaction methd.
// - If the given element has NO reaction fitting the given command, the
//   dispatch method will be called for the parent element of the given element.
//   The parent element is the parent given by the NALib structure, not the
//   native parent! The return value of this function is determined by the
//   return value of the recursive call.
// - If there is no parent available in the NALib structure, the function
//   returns NA_FALSE, meaning, the event has not been handeled by NALib.
//   This in turn will usually cause the NALib GUI elements to call the event
//   handling method given by the system or base class of the native element.
NA_HAPI NABool na_DispatchUIElementCommand(const NA_UIElement* uiElement, NAUICommand command);

// To be implemented in the system dependent files:
NA_HAPI void na_RefreshUIElementNow(void* uiElement);

// Updates the given element and all its children (if any) for a new uiScale.
NA_HAPI void na_UpdateUIElementUIScale(void* uiElement);

// Ensures the mouse being tracked as soon as there is one reason to do so.
NA_HAPI void na_RetainMouseTracking(NA_UIElement* uiElement);
NA_HAPI void na_ReleaseMouseTracking(NA_UIElement* uiElement);
NA_HAPI void na_UpdateMouseTracking(NA_UIElement* uiElement);
// System dependent implementation for adding a new mouse tracking object:
NA_HAPI void* na_AddMouseTracking(NA_UIElement* uiElement);
NA_HAPI void na_ClearMouseTracking(NA_UIElement* uiElement, void* mouseTracking);

NA_HAPI NABool na_GetUIElementDebugLayout(const NA_UIElement* elem);
NA_HAPI void na_SetUIElementDebugLayout(NA_UIElement* elem, NABool debug);



// //////////////////////////////////
// UI Elements
// //////////////////////////////////



// Initializing the core ui Elements
NA_HAPI void na_InitCoreApplication(NAApplication* application, void* nativeApplicationPtr);
NA_HAPI void na_InitCoreButton(NAButton* button, void* nativeButtonPtr, const NAUTF8Char* text, const NAUTF8Char* text2, const NAImageSet* imageSet, const NAImageSet* imageSet2, uint32 flags);
NA_HAPI void na_InitCoreCheckBox(NACheckBox* checkBox, void* nativeCheckBoxPtr);
NA_HAPI void na_InitCoreImageSpace(NAImageSpace* imageSpace, void* nativeImageSpacePtr);
NA_HAPI void na_InitCoreLabel(NALabel* label, void* nativeLabelPtr);
NA_HAPI void na_InitCoreMenu(NAMenu* menu, void* nativeMenuPtr, NA_UIElement* parent);
NA_HAPI void na_InitCoreMenuItem(NAMenuItem* menuItem, void* nativeMenuItemPtr, NA_UIElement* parent);
NA_HAPI void na_InitCoreMetalSpace(NAMetalSpace* metalSpace, void* nativeMetalSpacePtr);
NA_HAPI void na_InitCoreOpenGLSpace(NAOpenGLSpace* openGLSpace, void* nativeOpenGLSpacePtr);
NA_HAPI void na_InitCoreRadio(NARadio* radio, void* nativeRadioPtr);
NA_HAPI void na_InitCoreScreen(NAScreen* screen, void* nativeScreenPtr, NABool isMain, const NAUTF8Char* name, NARect rect, double uiScale);
NA_HAPI void na_InitCoreSelect(NASelect* select, void* nativeSelectPtr);
NA_HAPI void na_InitCoreSlider(NASlider* slider, void* nativeSliderPtr);
NA_HAPI void na_InitCoreSpace(NASpace* space, void* nativeSpacePtr);
NA_HAPI void na_InitCoreTextBox(NATextBox* textBox, void* nativeTextBoxPtr);
NA_HAPI void na_InitCoreTextField(NATextField* textField, void* nativeTextFieldPtr);
NA_HAPI void na_InitCoreWindow(NAWindow* window, void* nativeWindowPtr, NAScreen* screen, NASpace* contentSpace, NABool fullScreen, NABool resizeable, NARect windowedFrame);



// Clearing the core ui Elements
NA_HAPI void na_ClearCoreApplication(NAApplication* application);
NA_HAPI void na_ClearCoreButton(NAButton* button);
NA_HAPI void na_ClearCoreCheckBox(NACheckBox* checkBox);
NA_HAPI void na_ClearCoreImageSpace(NAImageSpace* imageSpace);
NA_HAPI void na_ClearCoreLabel(NALabel* label);
NA_HAPI void na_ClearCoreMenu(NAMenu* menu);
NA_HAPI void na_ClearCoreMenuItem(NAMenuItem* menuItem);
NA_HAPI void na_ClearCoreMetalSpace(NAMetalSpace* metalSpace);
NA_HAPI void na_ClearCoreOpenGLSpace(NAOpenGLSpace* openGLSpace);
NA_HAPI void na_ClearCoreRadio(NARadio* radio);
NA_HAPI void na_ClearCoreScreen(NAScreen* screen);
NA_HAPI void na_ClearCoreSelect(NASelect* select);
NA_HAPI void na_ClearCoreSlider(NASlider* slider);
NA_HAPI void na_ClearCoreSpace(NASpace* space);
NA_HAPI void na_ClearCoreTextBox(NATextBox* textBox);
NA_HAPI void na_ClearCoreTextField(NATextField* textField);
NA_HAPI void na_ClearCoreWindow(NAWindow* window);



// NAApplication
NA_HAPI NAApplication* na_NewApplication(void);
NA_HAPI NABool na_IsApplicationRunning(void);
NA_HAPI void na_TerminateApplication(NAMutator cleanup, void* arg);
NA_HAPI void na_RenewApplicationScreens();
NA_HAPI NARect na_GetApplicationRect(const NAApplication* application);
NA_HAPI void na_SetApplicationRect(const NAApplication* application, NARect rect);
NA_HAPI const NAFont* na_GetCoreApplicationSystemFont(const NAApplication* app);
NA_HAPI NAMouseStatus* na_GetApplicationMouseStatus(NAApplication* application);
// Takes the ownership of the keyStroke. Do not call naDelete on it!
NA_HAPI void na_SetApplicationKeyStroke(NAApplication* app, NAKeyStroke* keyStroke);
#if NA_DEBUG
  NA_HAPI void na_RegisterUIElement(NA_UIElement* elem);
  NA_HAPI void na_UnregisterUIElement(NA_UIElement* elem);
#endif

// NAButton
#define NA_BUTTON_BORDERED   0x01
#define NA_BUTTON_STATEFUL   0x02

NA_HAPI void na_setButtonText(NAButton* button, const NAUTF8Char* text);
NA_HAPI void na_setButtonText2(NAButton* button, const NAUTF8Char* text);
NA_HAPI void na_setButtonHint(NAButton* button, const NAUTF8Char* text);
NA_HAPI void na_setButtonImage(NAButton* button, const NAImageSet* imageSet);
NA_HAPI void na_setButtonImage2(NAButton* button, const NAImageSet* imageSet);
NA_HAPI NARect na_GetButtonRect(const NA_UIElement* button);
NA_HAPI void na_SetButtonRect(NA_UIElement* button, NARect rect);
NA_HAPI void na_UpdateButtonUIScale(NA_UIElement* button);

// NACheckBox
NA_HAPI NARect na_GetCheckBoxRect(const NA_UIElement* checkBox);
NA_HAPI void na_SetCheckBoxRect(NA_UIElement* checkBox, NARect rect);
NA_HAPI void na_UpdateCheckBoxUIScale(NA_UIElement* button);

// NAImageSpace
NA_HAPI NARect na_GetImageSpaceRect(const NA_UIElement* imageSpace);
NA_HAPI void na_SetImageSpaceRect(NA_UIElement* imageSpace, NARect rect);
NA_HAPI void na_UpdateImageSpaceUIScale(NA_UIElement* button);

// NALabel
NA_HAPI void na_SetLabelEnabled(NALabel* label, NABool enabled);
NA_HAPI void na_SetLabelTextColor(NALabel* label, const NAColor* color);
NA_HAPI NARect na_GetLabelRect(const NA_UIElement* space);
NA_HAPI void na_SetLabelRect(NA_UIElement* label, NARect rect);
NA_HAPI void na_UpdateLabelUIScale(NA_UIElement* button);
NA_HAPI void na_UpdateLabelTextAlignment(NALabel* label);

// NAMenu
NA_HAPI void na_AddMenuChild(NAMenu* menu, NAMenuItem* child, const NAMenuItem* itemAt);
NA_HAPI NARect na_GetMenuRect(const NA_UIElement* menu);
NA_HAPI void na_SetMenuRect(NA_UIElement* menu, NARect rect);
NA_HAPI void na_UpdateMenuUIScale(NA_UIElement* button);

// NAMenuItem
NA_HAPI NARect na_GetMenuItemRect(const NA_UIElement* menuItem);
NA_HAPI void na_SetMenuItemRect(NA_UIElement* menuItem, NARect rect);
NA_HAPI void na_UpdateMenuItemUIScale(NA_UIElement* button);

// NAMetalSpace
NA_HAPI NARect na_GetMetalSpaceRect(const NA_UIElement* metalSpace);
NA_HAPI void na_SetMetalSpaceRect(NA_UIElement* metalSpace, NARect rect);
NA_HAPI void na_UpdateMetalSpaceUIScale(NA_UIElement* button);

// NAOpenGLSpace
NA_HAPI NARect na_GetOpenGLSpaceRect(const NA_UIElement* openGLSpace);
NA_HAPI void na_SetOpenGLSpaceRect(NA_UIElement* openGLSpace, NARect rect);
NA_HAPI void na_UpdateOpenGLSpaceUIScale(NA_UIElement* button);

// NARadio
NA_HAPI NARect na_GetRadioRect(const NA_UIElement* radio);
NA_HAPI void na_SetRadioRect(NA_UIElement* radio, NARect rect);
NA_HAPI void na_UpdateRadioUIScale(NA_UIElement* button);

// NAScreen
NA_HAPI void na_FlipScreenCoordinatesVertically(NAScreen* screen, NARect totalRect);
NA_HAPI void na_UpdateScreenRelativeCenter(NAScreen* screen, NARect totalRect);
NA_HAPI NARect na_FillScreenList(NAList* screenList);
NA_HAPI NAScreen* na_GetApplicationScreenWithNativeScreenPtr(const void* nativeScreenPtr);
NA_HAPI NARect na_GetScreenRect(const NA_UIElement* screen);
NA_HAPI void na_SetScreenRect(NA_UIElement* screen, NARect rect);

// NASelect
NA_HAPI void na_AddSelectChild(NASelect* select, NAMenuItem* child, const NAMenuItem* itemAt);
NA_HAPI NARect na_GetSelectRect(const NA_UIElement* select);
NA_HAPI void na_SetSelectRect(NA_UIElement* select, NARect rect);
NA_HAPI void na_UpdateSelectUIScale(NA_UIElement* button);

// NASlider
NA_HAPI NARect na_GetSliderRect(const NA_UIElement* slider);
NA_HAPI void na_SetSliderRect(NA_UIElement* slider, NARect rect);
NA_HAPI void na_UpdateSliderUIScale(NA_UIElement* button);

// NASpace
NA_HAPI void na_AddSpaceChild(NASpace* space, NA_UIElement* child);
NA_HAPI void na_RemoveSpaceChild(NASpace* space, NA_UIElement* child);
NA_HAPI void na_SetSpaceBackgroundColor(NASpace* space, const NAColor* color);
NA_HAPI NARect na_GetSpaceRect(const NA_UIElement* space);
NA_HAPI void na_SetSpaceRect(NA_UIElement* space, NARect rect);
NA_HAPI void na_UpdateSpaceUIScale(NA_UIElement* button);
// In some rare cases, you just want to add a child to a space and position
// it later. This function does the trick without triggering any repositioning
// or resize. Note that the latter should be done manually afterwards or some
// unexpected drawing artifacts might occur.
NA_HAPI void na_AddSpaceChildUnpositioned(NASpace* space, void* child);

// NATextBox
NA_HAPI NARect na_GetTextBoxRect(const NA_UIElement* textBox);
NA_HAPI void na_SetTextBoxRect(NA_UIElement* textBox, NARect rect);
NA_HAPI void na_UpdateTextBoxUIScale(NA_UIElement* button);
NA_HAPI void na_UpdateTextBoxTextAlignment(NATextBox* textBox);

// NATextField
NA_HAPI NARect na_GetTextFieldRect(const NA_UIElement* textField);
NA_HAPI void na_SetTextFieldRect(NA_UIElement* textField, NARect rect);
NA_HAPI void na_UpdateTextFieldUIScale(NA_UIElement* button);
NA_HAPI void na_UpdateTextFieldTextAlignment(NATextField* textField);

// NAWindow
NA_HAPI void na_RememberWindowPosition(const NAWindow* window);
NA_HAPI void na_UpdateWindowScreen(NAWindow* window, NAScreen* screen);
NA_HAPI void na_RetainWindowMouseTracking(NAWindow* window);
NA_HAPI void na_ReleaseWindowMouseTracking(NAWindow* window);
NA_HAPI NARect na_GetWindowAbsoluteInnerRect(const NA_UIElement* window);
NA_HAPI NARect na_GetWindowRect(const NA_UIElement* window);
NA_HAPI void na_SetWindowRect(NA_UIElement* window, NARect rect);
NA_HAPI void na_UpdateWindowUIScale(NA_UIElement* button);




// Mouse related functions
NA_HAPI NAMouseStatus* na_AllocMouseStatus();
NA_HAPI void na_DeallocMouseStatus(NAMouseStatus* status);

NA_HAPI void na_SetMouseButtonPressed(NAMouseStatus* status, NAMouseButton button, NABool pressed);

NA_HAPI void na_SetMouseWarpedTo(NAMouseStatus* status, NAPos newpos);
NA_HAPI void na_SetMouseMovedTo(NAMouseStatus* status, NAPos newpos);
NA_HAPI void na_SetMouseMovedByDiff(NAMouseStatus* status, double deltaX, double deltaY);
NA_HAPI void na_SetMouseEnteredAtPos(NAMouseStatus* status, NAPos newpos);
NA_HAPI void na_SetMouseExitedAtPos(NAMouseStatus* status, NAPos newpos);


// Font
NA_HAPI NAFont* na_CreateFont(void* nativeFontPtr, const NAString* name, uint32 flags, double size);
NA_HAPI void na_DestructFontNativePtr(void* nativeFontPtr);


// Preferences
NA_HAPI void* na_GetNativePreferences(void);
NA_HAPI void na_ShutdownPreferences(void);

NA_HAPI int64     na_GetRawPreferencesBool  (void* prefs, const char* key);
NA_HAPI int64     na_GetRawPreferencesi64   (void* prefs, const char* key);
NA_HAPI int64     na_GetRawPreferencesEnum  (void* prefs, const char* key);
NA_HAPI double    na_GetRawPreferencesDouble(void* prefs, const char* key);
NA_HAPI NAString* na_GetRawPreferencesString(void* prefs, const char* key);

NA_HAPI void na_SetRawPreferencesBool  (void* prefs, const char* key, int64 valueStorage);
NA_HAPI void na_SetRawPreferencesi64   (void* prefs, const char* key, int64 valueStorage);
NA_HAPI void na_SetRawPreferencesEnum  (void* prefs, const char* key, int64 valueStorage);
NA_HAPI void na_SetRawPreferencesDouble(void* prefs, const char* key, double valueStorage);
NA_HAPI void na_SetRawPreferencesString(void* prefs, const char* key, NAString* valueStorage);

#if NA_OS == NA_OS_WINDOWS

#include <windows.h>

  // Returns a value from the windows registry.
  // 
  // The "Variable" method will allocate sufficient memory with malloc for you
  // and return the size allocated in valueSize if available.
  // 
  // The "Fixed" method expects a pointer to an existing memory location and
  // the size in bytes available at that location. The requested value will
  // be stored at that location if and only if the value size is equal to the
  // one retrieved from the registry.
  // 
  // Both methods will return NA_NULL if something went wrong.
  NA_HAPI void* na_GetWINRegistryVariableEntry(
    HKEY rootKey,
    const NAUTF8Char* path,
    const NAUTF8Char* key,
    size_t* valueSize);

  NA_HAPI void* na_GetWINRegistryFixedEntry(
    HKEY rootKey,
    const NAUTF8Char* path,
    const NAUTF8Char* key,
    void* value,
    size_t valueSize);

#endif



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
