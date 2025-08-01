
#ifndef NA_UI_WINDOW_INCLUDED
#define NA_UI_WINDOW_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// An NAWindow can react to the following commands:
//
// NA_UI_COMMAND_CLOSES        When the window tries to close.
// NA_UI_COMMAND_KEY_DOWN      When a key has been pressed
// NA_UI_COMMAND_KEY_UP        When a key has been released
// NA_UI_COMMAND_MOUSE_ENTERED When the mouse entered the content space
// NA_UI_COMMAND_MOUSE_EXITED  When the mouse exited the content space
// NA_UI_COMMAND_MOUSE_PRESSED When the window has been activated
// NA_UI_COMMAND_RESHAPE       Resizing, repositioning, UI scale changes

// The CLOSES command will be called when a window tries to close. You can
// prevent it from closing by calling naPreventWindowClosing. That
// function only works in conjunction with such a command and will emit an
// error otherwise.



// Window flags:
#define NA_WINDOW_FIXED_SIZE             0x00
#define NA_WINDOW_RESIZEABLE             0x01

// Auxiliary windows are helper windows. The implementation is os-specific.
#define NA_WINDOW_DEFAULT                0x00
#define NA_WINDOW_AUXILIARY              0x02

// Titleless windows can automatically be dragged by their content space.
#define NA_WINDOW_TITLED                 0x00
#define NA_WINDOW_TITLELESS              0x04

// Hide or show the close button
#define NA_WINDOW_CLOSEABLE              0x00
#define NA_WINDOW_NON_CLOSEABLE          0x08

// Hide or show the miniaturization button
#define NA_WINDOW_MINIATURIZEABLE        0x00
#define NA_WINDOW_NON_MINIATURIZEABLE    0x10

// If a window is not resizeable and does not show the mini nor the close
// button, it will automatically hide all buttons, leaving a pure title bar.



// Creates a new window. Use Null for screen to use the main screen.
NA_API NAWindow* naNewWindow(
  const NAUTF8Char* title,
  NARect rect,
  uint32 flags);

// Get and set the content space.
NA_API NASpace* naGetWindowContentSpace(NAWindow* window);
NA_API void naSetWindowContentSpace(NAWindow* window, void* space);

// Show or close the window.
NA_API void naShowWindow(const NAWindow* window);
NA_API void naCloseWindow(const NAWindow* window);

// Handling modal windows. Opening such will block the parent window. (not fully functional yet)
// Close must be called explicitely.
NA_API void naShowWindowModal(NAWindow* window, NAWindow* parentWindow);
NA_API void naCloseWindowModal(NAWindow* window);

// Returns the screen the window is attached to.
NA_API const NAScreen* naGetWindowScreen(const NAWindow* window);

// During a NA_UI_COMMAND_CLOSES command, call this function to prevent the
// window from closing. If not called, the window will close.
NA_API void naPreventWindowClosing(NAWindow* window, NABool prevent);

// Use this method to mark the window as changed.
// On macOS, this adds a small bubble in the red closing icon.
NA_API void naMarkWindowChanged(NAWindow* window, NABool changed);

// Get and set the window fullscreen state.
NA_API NABool naIsWindowFullscreen(NAWindow* window);
NA_API void naSetWindowFullscreen(NAWindow* window, NABool fullScreen);

// Get various properties of the window
NA_API NABool naIsWindowResizeable(const NAWindow* window);

// Set a value different from zero to automatically store position and size
// of the window. If the same storageTag is used in a later run of an app,
// this call will automatically restore the stored position and size.
// This provides an easy way for small applicaitons to remember window
// arrangements.
NA_API void naSetWindowStorageTag(NAWindow* window, size_t storageTag);

// Get and set the outer (non-client) rect of the window. Note that the
// function naGetUIElementRect will always return the inner (client) rect of
// the window. The returned rect is in absolute coordinates.
NA_API NARect naGetWindowOuterRect(const NAWindow * window);
NA_API void naSetWindowOuterRect(NAWindow * window, NARect rect);

// Returns the area in absolute coordinates which moves the window when dragged.
// For titeled windows, that is the area of the titlebar.
// For titleless windows, it is the content area.
NA_API NARect naGetWindowDraggableRect(const NAWindow * window);

// Set various properties of the window
NA_API void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title);
NA_API void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop);
NA_API void naSetWindowAcceptsKeyboardReactions(NAWindow* window, NABool accepts);

// Get and set the first tab element of the window.
NA_API void* naGetWindowFirstTabElement(NAWindow* window);
NA_API void naSetWindowFirstTabElement(NAWindow* window, const void* firstTabElem);



// ////////////////////////////////
// Alert box

typedef enum{
  NA_ALERT_BOX_INFO,
  NA_ALERT_BOX_WARNING,
  NA_ALERT_BOX_ERROR
} NAAlertBoxType;

NA_API void naPresentAlertBox(
  NAAlertBoxType alertBoxType,
  const NAUTF8Char* titleText,
  const NAUTF8Char* infoText);

// Third button is optional and can be NA_NULL.
// Returns the button index which was pressed.
// 0 = Primary
// 1 = Secondary
// 2 = Ternary
// Note taht the primary option is the one used for pressing enter.
NA_API size_t naPresentOptionBox(
  const NAUTF8Char* titleText,
  const NAUTF8Char* infoText,
  const NAUTF8Char* buttonTextPrimary,
  const NAUTF8Char* buttonTextSecondary,
  const NAUTF8Char* buttonTextTernary);



// ////////////////////////////////
// File panel

// The callback can return false, if the operation did not succeed, which in
// turn will result in naPresentFilePanel returning false, which is equivalent
// to a cancel operation.
typedef NABool (*NAFilePanelCallback)(
  NABool doPerform,
  const NAUTF8Char* path,
  const void* data);

// Returns true, if the callback returns true.
// Returns false if cancel was pressed.
NA_API NABool naPresentFilePanel(
  NABool load,
  const NAUTF8Char* fileName,
  const NAUTF8Char* allowedFileSuffix,
  NAFilePanelCallback callback,
  const void* data);



#ifdef __cplusplus
  } // extern "C"
#endif
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
