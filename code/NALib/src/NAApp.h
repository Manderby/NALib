
#ifndef NA_APP_INCLUDED
#define NA_APP_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include "NABase.h"
#if NA_COMPILE_GUI == 1

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



// Have a look at NAConfiguration.h to use OpenGL
#if (NA_COMPILE_OPENGL == 1)
  #if NA_OS == NA_OS_WINDOWS
    #include <windows.h>
    #include <GL/GL.h>
  #else
    #include <OpenGL/gl.h>
  #endif
#endif

// Have a look at NAConfiguration.h to use Metal
#if (NA_COMPILE_METAL == 1)
  #if NA_OS == NA_OS_WINDOWS
    #error "Metal is a macOS only framework. Does not work on windows."
  #endif
#endif

#include "NACoord.h"
#include "NAString.h"
#include "NABabyImage.h"

#include "NAApp/Core/NAFont.h"
#include "NAApp/Core/NAKeyboard.h"
#include "NAApp/Core/NAMouse.h"
#include "NAApp/Core/NAReaction.h"
#include "NAApp/Core/NAUIImage.h"

#include "NAApp/Core/UIElement/NAUIElement.h"
#include "NAApp/Core/UIElement/NAApplication.h"


#if NA_OS == NA_OS_MAC_OS_X
  NABool naLoadNib(const NAUTF8Char* nibName);
#endif



// In the following, the specific UI elements and their API are presented.
//
// Usually, UI-elementes are placed in a line-by-line fashion, usually asking
// the user to read from top to bottom. Therefore, it is good to know the
// vertical alignment of all UI-elements.
//
// The tallest items are 24 points in height. But the visual representation is
// often times shifted in vertical position such that some elements are even
// overlapping when displayed line by line. Therefore, it is proposed to always
// use at least a difference of 25 points between lines. The author strongly
// recommends that number.

#define NA_BUTTON_BORDERED   0x00
#define NA_BUTTON_BORDERLESS 0x01
#define NA_BUTTON_PUSH       0x00
#define NA_BUTTON_STATEFUL   0x10

// Button. Default height for TextButton: 24.
NA_API NAButton* naNewTextButton(const NAUTF8Char* text, double width, uint32 flags);
NA_API NAButton* naNewImageButton(const NAUIImage* uiImage, NASize size, uint32 flags);
NA_API void naSetButtonEnabled(NAButton* button, NABool enabled);
NA_API void naSetButtonText(NAButton* button, const NAUTF8Char* text);
NA_API void naSetButtonImage(NAButton* button, const NAUIImage* uiImage);
NA_API void naSetButtonState(NAButton* button, NABool state);
NA_API NABool naGetButtonState(NAButton* button);
NA_API void naSetButtonSubmit(NAButton* button, NAReactionHandler handler, void* controller);
NA_API void naSetButtonAbort(NAButton* button, NAReactionHandler handler, void* controller);
NA_API void naSetButtonVisible(NAButton* button, NABool visible);

// CheckBox. Default height: 18
NA_API NACheckBox* naNewCheckBox(const NAUTF8Char* text, double width);
NA_API void naSetCheckBoxTextColor(NACheckBox* checkBox, const NABabyColor* color);
NA_API void naSetCheckBoxState(NACheckBox* checkBox, NABool state);
NA_API void naSetCheckBoxEnabled(NACheckBox* checkBox, NABool enabled);
NA_API NABool naGetCheckBoxState(NACheckBox* checkBox);

// ImageSpace. Will retain the uiImage.
NA_API NAImageSpace* naNewImageSpace(NAUIImage* uiImage, NASize size);

// Label. Default Height 16. Y-Offset: +4
// A labe is by default: Eanbled, visible, selectable
NA_API NALabel* naNewLabel(const NAUTF8Char* text, double width);
NA_API void naSetLabelHeight(NALabel* label, double height);
NA_API void naSetLabelText(NALabel* label, const NAUTF8Char* text);
NA_API void naSetLabelTextColor(NALabel* label, const NABabyColor* color);
// Note that text alignment must be set before calling this method.
NA_API void naSetLabelLink(NALabel* label, const NAUTF8Char* url);
NA_API NABool naIsLabelEnabled(NALabel* label);
NA_API void naSetLabelEnabled(NALabel* label, NABool enabled);
NA_API void naSetLabelSelectable(NALabel* label, NABool selectable);
NA_API void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment);
NA_API void naSetLabelFontKind(NALabel* label, NAFontKind kind, NAFontSize size);
NA_API void naSetLabelVisible(NALabel* label, NABool visible);

// Menu
// naAddMenuItem adds a menu item before atItem. If atItem is Null, it is
// added to the end of the menu.
// naGetMenuItemIndex returns the index including all separators.
// To present the menu onscreen, you need to provide a parent UIElement.
NA_API NAMenu* naNewMenu(void);
NA_API void naAddMenuItem(NAMenu* menu, NAMenuItem* item, NAMenuItem* atItem);
NA_API size_t naGetMenuItemIndex(NAMenu* menu, NAMenuItem* item);
NA_API void naPresentMenu(NAMenu* menu, NAPos pos, void* parentUIElement);

// MenuItem
NA_API NAMenuItem* naNewMenuItem(const NAUTF8Char* text);
NA_API NAMenuItem* naNewMenuSeparator(void);

// MetalSpace
// Note that you must have NA_COMPILE_METAL configured in NAConfiguration.h
// to use these functions. Also note that metal is macOS only.
// Use initFunc to perform any initialization necessary like for example
// uploading of textures to the GPU. The initFunc will be called with
// initData as the input parameter. The initFunc can be Null.
// Note that the initFunc will be called when prepareMetal is called (which
// may be as late as when the space comes onsceen)
NA_API NAMetalSpace* naNewMetalSpace(NASize size);
NA_API void* naGetMetalSpaceSystemContext(NAMetalSpace* metalSpace);
NA_API void naSetMetalSpaceVisible(NAMetalSpace* metalSpace, NABool visible);
NA_API void naSetMetalSpaceInnerRect(NAMetalSpace* metalSpace, NARect bounds);

// OpenGLSpace
// Note that you must have NA_COMPILE_OPENGL configured in NAConfiguration.h
// to use these functions.
// Use initFunc to perform any initialization necessary like for example
// uploading of textures to the GPU. The initFunc will be called with
// initData as the input parameter. The initFunc can be Null.
// Note that the initFunc will be called...
// Win: Right within the naNewOpenGLSpace
// Mac: when prepareOpenGL is called (which may be as late as when the
//      space comes onsceen)
NA_API NAOpenGLSpace* naNewOpenGLSpace(NASize size, NAMutator initFunc, void* initData);
NA_API void* naGetOpenGLSpaceSystemContext(NAOpenGLSpace* openGLSpace);
NA_API void naSwapOpenGLSpaceBuffer(NAOpenGLSpace* openGLSpace);
NA_API void naSetOpenGLSpaceVisible(NAOpenGLSpace* openGLSpace, NABool visible);
NA_API void naSetOpenGLSpaceInnerRect(NAOpenGLSpace* openGLSpace, NARect bounds);

// PopupButton. Default height is 23
// naAddPopupButtonMenuItem adds a menu item before atItem. If atItem is Null,
// it is added to the end of the menu.
// naGetPopupButtonItemIndex returns the index including all separators.
NA_API NAPopupButton* naNewPopupButton(double width);
NA_API void naSetPopupButtonVisible(NAPopupButton* popupButton, NABool visible);
NA_API void naAddPopupButtonMenuItem(NAPopupButton* popupButton, NAMenuItem* item, NAMenuItem* atItem);
NA_API size_t naGetPopupButtonItemIndex(NAPopupButton* popupButton, NAMenuItem* item);
NA_API void naSetPopupButtonItemSelected(NAPopupButton* popupButton, NAMenuItem* item);
NA_API void naSetPopupButtonEnabled(NAPopupButton* popupButton, NABool enabled);

// Radio. Default height is 18
NA_API NARadio* naNewRadio(const NAUTF8Char* text, double width);
NA_API void naSetRadioTextColor(NARadio* radio, const NABabyColor* color);
NA_API NABool naGetRadioState(NARadio* radio);
NA_API void naSetRadioState(NARadio* radio, NABool state);
NA_API void naSetRadioEnabled(NARadio* radio, NABool enabled);

// Screen
// Screens are not implemented yet, but you can get the main screen rect:
NA_API NARect naGetMainScreenRect(void);

// Space (In other frameworks called View, Frame, Area, Widget...)
// Beware that adding a child to a space which formerly was the first responder
// of a window, that windows first responder will be reset to Null.
NA_API NASpace* naNewSpace(NASize size);
NA_API void naAddSpaceChild(NASpace* space, void* child, NAPos pos);
NA_API void naRemoveSpaceChilds(NASpace* space);
NA_API void naSetSpaceBackgroundColor(NASpace* space, const NABabyColor* color);
NA_API void naSetSpaceAlternateBackground(NASpace* space, NABool alternate);
NA_API NABool naGetSpaceAlternateBackground(NASpace* space);
NA_API void naSetSpaceRect(NASpace* space, NARect rect);
NA_API void naSetSpaceDragsWindow(NASpace* space, NABool isDraggable);

// Slider. Default height: 24
NA_API NASlider* naNewSlider(double width);
NA_API void naSetSliderEnabled(NASlider* slider, NABool enabled);
NA_API void naSetSliderTickCount(NASlider* slider, NAInt tickCount);
NA_API double naGetSliderValue(NASlider* slider);
NA_API void naSetSliderValue(NASlider* slider, double value);

// TextBox
NA_API NATextBox* naNewTextBox(NASize size);
NA_API void naSetTextBoxText(NATextBox* textBox, const NAUTF8Char* text);
NA_API NAString* naNewStringWithTextBoxText(NATextBox* textBox);
NA_API void naSetTextBoxTextAlignment(NATextBox* textBox, NATextAlignment alignment);
NA_API void naSetTextBoxFontKind(NATextBox* textBox, NAFontKind kind, NAFontSize size);
NA_API void naSetTextBoxCustomFont(NATextBox* textBox, const NAUTF8Char* fontName, uint32 flags, double size);
NA_API void naSetTextBoxUseHorizontalScrolling(NATextBox* textBox);
NA_API void naSetTextBoxEditable(NATextBox* textBox, NABool editable);
NA_API void naSetTextBoxVisible(NATextBox* textBox, NABool visible);

// TextField. Default height is 21
NA_API NATextField* naNewTextField(double width);
NA_API void naSetTextFieldText(NATextField* textField, const NAUTF8Char* text);
NA_API NAString* naNewStringWithTextFieldText(NATextField* textField);
NA_API void naSetTextFieldFontKind(NATextField* textField, NAFontKind kind, NAFontSize size);
NA_API void naSetTextFieldTextAlignment(NATextField* textField, NATextAlignment alignment);
NA_API void naSetTextFieldFontKind(NATextField* textField, NAFontKind kind, NAFontSize size);

// A window is by bu default:
#define NA_WINDOW_FIXED_SIZE             0x00
#define NA_WINDOW_RESIZEABLE             0x01
#define NA_WINDOW_DEFAULT                0x00
#define NA_WINDOW_AUXILIARY              0x02
#define NA_WINDOW_TITLED                 0x00
#define NA_WINDOW_TITLELESS              0x04
#define NA_WINDOW_CLOSEABLE              0x00
#define NA_WINDOW_NON_CLOSEABLE          0x08
#define NA_WINDOW_MINIATURIZEABLE        0x00
#define NA_WINDOW_NON_MINIATURIZEABLE    0x10

// Window
NA_API NAWindow* naNewWindow(
  const NAUTF8Char* title,
  NARect rect,
  uint32 flags,
  NAInt storageTag);
NA_API void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title);
NA_API void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop);
NA_API void naSetWindowRect(NAWindow* window, NARect rect);
NA_API NAUIImageResolution naGetWindowUIResolution(NAWindow* window);
NA_API void naSetWindowFirstTabElement(NAWindow* window, void* firstTabElem);
NA_API void* naGetWindowFirstTabElement(NAWindow* window);
NA_API void naShowWindow(NAWindow* window);
NA_API void naCloseWindow(NAWindow* window);
NA_API NASpace* naGetWindowContentSpace(NAWindow* window);
NA_API void naSetWindowContentSpace(NAWindow* window, void* space);
NA_API void naSetWindowFullscreen(NAWindow* window, NABool fullScreen);
NA_API void naSetWindowAcceptsKeyReactions(NAWindow* window, NABool accepts);
NA_API NABool naIsWindowFullscreen(NAWindow* window);
NA_API NABool naIsWindowResizeable(NAWindow* window);
NA_API void naPreventWindowFromClosing(NAWindow* window, NABool prevent);
NA_API NARect naSetWindowStorageTag(
  NAWindow* window,
  NAInt storageTag,
  NARect rect,
  NABool resizeable);





// ////////////////////////////////
// Other UI functions.

typedef enum{
  NA_ALERT_BOX_INFO,
  NA_ALERT_BOX_WARNING,
  NA_ALERT_BOX_ERROR
} NAAlertBoxType;

typedef NABool (*FilePanelCallback)(NABool doPerform, const NAUTF8Char* path);

NA_API void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText);
NA_API void naPresentFilePanel(void* window, NABool load, const NAUTF8Char* fileName, const NAUTF8Char* allowedFileSuffix, FilePanelCallback callback);
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
// will NOT automatically hide the console when NA_DEBUG is 0.
// 
// This function does nothing on a Mac.

NA_API void naOpenConsoleWindow(void);

#endif // NA_COMPILE_GUI == 1

#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_APP_INCLUDED



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
