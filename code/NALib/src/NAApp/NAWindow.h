
#if defined NA_UI_WINDOW_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_WINDOW_INCLUDED
#define NA_UI_WINDOW_INCLUDED



// An NAWindow can react to the following commands:
//
// NA_UI_COMMAND_CLOSES        When the window tries to close
// NA_UI_COMMAND_KEY_DOWN      When a key has been pressed
// NA_UI_COMMAND_KEY_UP        When a key has been released
// NA_UI_COMMAND_MOUSE_ENTERED When the mouse entered the content space
// NA_UI_COMMAND_MOUSE_EXITED  When the mouse exited the content space
// NA_UI_COMMAND_MOUSE_PRESSED When the window has been activated
// NA_UI_COMMAND_RESHAPE       Resizing, repositioning, UI scale changes



// Window flags:
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

// Creates a new window.
NA_API NAWindow* naNewWindow(
  const NAUTF8Char* title,
  NARect rect,
  uint32 flags,
  size_t storageTag);

// Get and set the content space.
NA_API NASpace* naGetWindowContentSpace(NAWindow* window);
NA_API void naSetWindowContentSpace(NAWindow* window, void* space);

// Show or close the window.
NA_API void naShowWindow(const NAWindow* window);
NA_API void naCloseWindow(const NAWindow* window);

// Handling modal windows. Opening such will block the rest of the app.
// Close must be called explicitely.
NA_API void naShowWindowModal(const NAWindow* window);
NA_API void naCloseWindowModal(const NAWindow* window);

// During a NA_UI_COMMAND_CLOSES command, call this function to prevent the
// window from closing. If not called, the window will close.
NA_API void naPreventWindowFromClosing(NAWindow* window, NABool prevent);

// Get and set the window fullscreen state.
NA_API NABool naIsWindowFullscreen(NAWindow* window);
NA_API void naSetWindowFullscreen(NAWindow* window, NABool fullScreen);

// Get various properties of the window
NA_API NABool naIsWindowResizeable(const NAWindow* window);

// Get and set the outer (non-client) rect of the window. Note that the
// function naGetUIElementRect will always return the inner (clinet) rect of
// the window.
NA_API NARect naGetWindowOuterRect(const NAWindow* window);
NA_API void naSetWindowOuterRect(NAWindow* window, NARect rect);

// Set various properties of the window
NA_API void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title);
//NA_API void naSetWindowRect(NAWindow* window, NARect rect);
NA_API void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop);
NA_API void naSetWindowAcceptsKeyReactions(NAWindow* window, NABool accepts);

// Get and set the first tab element of the window.
NA_API void* naGetWindowFirstTabElement(NAWindow* window);
NA_API void naSetWindowFirstTabElement(NAWindow* window, const void* firstTabElem);

// Set the storage tag for automatically remembering the position of the
// window.
NA_API NARect naSetWindowStorageTag(
  NAWindow* window,
  size_t storageTag,
  NARect rect,
  NABool resizeable);



// ////////////////////////////////
// Alert box

typedef enum{
  NA_ALERT_BOX_INFO,
  NA_ALERT_BOX_WARNING,
  NA_ALERT_BOX_ERROR
} NAAlertBoxType;

NA_API void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText);



// ////////////////////////////////
// File panel

typedef NABool (*NAFilePanelCallback)(
  NABool doPerform,
  const NAUTF8Char* path,
  const void* data);

NA_API void naPresentFilePanel(
  void* window,
  NABool load,
  const NAUTF8Char* fileName,
  const NAUTF8Char* allowedFileSuffix,
  NAFilePanelCallback callback,
  const void* data);



#endif // NA_UI_WINDOW_INCLUDED



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
