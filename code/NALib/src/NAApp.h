
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



#include "NAString.h"

#include "NAApp/Core/NAFont.h"
#include "NAApp/Core/NAKeyboard.h"
#include "NAApp/Core/NAMouse.h"
#include "NAApp/Core/NAReaction.h"
#include "NAApp/Core/NAUIImage.h"

// The base of every ui element.
#include "NAApp/Core/UIHeaders/NAUIElement.h"

// The acutal ui elements
#include "NAApp/Core/UIHeaders/NAApplication.h"
#include "NAApp/Core/UIHeaders/NAButton.h"
#include "NAApp/Core/UIHeaders/NACheckBox.h"
#include "NAApp/Core/UIHeaders/NAImageSpace.h"
#include "NAApp/Core/UIHeaders/NALabel.h"
#include "NAApp/Core/UIHeaders/NAMenu.h"
#include "NAApp/Core/UIHeaders/NAMenuItem.h"
#include "NAApp/Core/UIHeaders/NAMetalSpace.h"
#include "NAApp/Core/UIHeaders/NAOpenGLSpace.h"
#include "NAApp/Core/UIHeaders/NAPopupButton.h"
#include "NAApp/Core/UIHeaders/NARadio.h"
#include "NAApp/Core/UIHeaders/NAScreen.h"
#include "NAApp/Core/UIHeaders/NASlider.h"
#include "NAApp/Core/UIHeaders/NASpace.h"
#include "NAApp/Core/UIHeaders/NATextBox.h"
#include "NAApp/Core/UIHeaders/NATextField.h"
#include "NAApp/Core/UIHeaders/NAWindow.h"



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
