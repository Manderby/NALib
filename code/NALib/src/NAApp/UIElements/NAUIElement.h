
#ifndef NA_UI_ELEMENT_INCLUDED
#define NA_UI_ELEMENT_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// ///////////////////////////////////
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
// As GUIs are handeled differently amongst different systems, in NALib, the
// implementation of the corresponding structs is split into two parts: A core
// which stores any basic attributes and a system dependent implementation
// containing the core struct as its first entry and having additional fields
// necessary for the system dependent implementation. To the programmer, only
// the core structs are visible and the system dependent implementation is
// hidden.
//
// These are the core structs:

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

// As the C programming language has no notion of inheritance, the functions
// listed further below which expect an arbitrary uiElement will expect it as
// a void pointer.



// Each of the UI elements can identify themselves as what they are.
// In order to find out what a specific UI element is, you can use the
// the following enum and function.

typedef enum{
  NA_UI_APPLICATION,
  NA_UI_BUTTON,
  NA_UI_CHECKBOX,
  NA_UI_IMAGE_SPACE,
  NA_UI_LABEL,
  NA_UI_MENU,
  NA_UI_MENUITEM,
  NA_UI_METAL_SPACE,
  NA_UI_OPENGL_SPACE,
  NA_UI_RADIO,
  NA_UI_SCREEN,
  NA_UI_SELECT,
  NA_UI_SLIDER,
  NA_UI_SPACE,
  NA_UI_TEXTBOX,
  NA_UI_TEXTFIELD,
  NA_UI_WINDOW
} NAUIElementType;

NA_API NAUIElementType naGetUIElementType(const void* uiElement);

// Any ui element has a strict hierarchical ordering: Application - Screen -
// Window - Space - Subspace - Subsubspace ... You can get the parent element
// with this function. The parent of the Application will be NA_NULL.
NA_API const void* naGetUIElementParent(const void* uiElement);
NA_API void* naGetUIElementParentMutable(void* uiElement);

// You can get the window of any ui element except for application and screen
// elements. An application or screen element will return NA_NULL. A window
// element will return itself and any other ui element will return the window
// it is contained in.
NA_API const NAWindow* naGetUIElementWindow(const void* uiElement);
NA_API NAWindow* naGetUIElementWindowMutable(void* uiElement);

// You can get the parent space of any ui element. Note that when the
// given element is itself a space, the parental space will be returned.
// If there is no parental space, NA_NULL will be returned. Applications,
// screens and windows will always return NA_NULL.
NA_API const NASpace* naGetUIElementParentSpace(const void* uiElement);
NA_API NASpace* naGetUIElementParentSpaceMutable(void* uiElement);

// In NALib, all coordinates of the UI are described in a mathematical, right-
// handed coordinate system. The origin of the absolute coordinate system is
// at the lower left screen corner of the main screen.
//
// Using naGetUIElementRect, you can get the rect of any ui element either
// in absolute coordinates or in relative coordinates to its parent element.
//
// Note that windows and screens always return absolute coordinates.
// Additionally, windows only return the inner (client) rect. If you need the
// outer rect (including the titlebar), use naGetWindowOuterRect.
//
// Quering the NSApplication object returns the total rect of all screens.
NA_API NARect naGetUIElementRect(const void* uiElement);
NA_API NARect naGetUIElementRectAbsolute(const void* uiElement);
NA_API void   naSetUIElementRect(void* uiElement, NARect rect);

// You can ask any ui element to refresh its contents. This will cause the
// element to be displayed anew. The time difference defines when the refresh
// shall occur in seconds. Note that when using 0 as timediff, the redraw
// method will execute immediately!
NA_API void naRefreshUIElement(void* uiElement, double timediff);

// When navigating with the tab key, this method defines, which will be the
// next ui element to have the focus. You start the tab order with a call to
// naSetWindowFirstTabElement.
NA_API void naSetUIElementNextTabElement(void* uiElement, const void* nextTabElem);

// Returns the resolution scale for the given element. The scale is a multiple
// of 1 whereas 1x corresponds to pixel per pixel accurate reproduction with
// a "measurable" resolution of 96 ppi.
// The scale 2x for example is returned on macOS high resolution display
// settings. On windows, this returns the UI scale factor defined in display
// settings. Returns 1x if no parent window or screen can be found, as well
// as for NAApplication.
NA_API double naGetUIElementUIScale(const void* uiElement);

// Native IDs
//
// NALib always acts as a layer on top of the native UI implementation of a
// system. No matter if it is macOS or Windows, you can get the native
// structures used in corresponding UI systems with naGetUIElementNativePtr.
// This allows you to do with the user interface elements whatever you need to
// do.
//
// Windows: The native framework used is WINAPI and the nativePtr you get
// usually is a HWND handle. There are some special cases though where you
// get a different pointer:
// Application: HINSTANCE handle
// Screen:      HMONITOR handle
// Menu:        HMENU handle
// MenuItem:    NA_WINAPIMenuItem* (opaque, internal structure)
//
// Macintosh: NALib is using the Cocoa framework as the native UI. This means
// that in the background, NALib implements certain Objective-C methods to
// provide a UI most closely resembling a native experience. The nativePtr
// corresponds to NSResponder*. Note that also NSApplication inherits from
// NSResponder.
//
// Use the following function to retrieve the nativePtr for any ui element:

NA_API void* naGetUIElementNativePtr(void* uiElement);
NA_API void* naGetUIElementNativePtrConst(const void* uiElement);


NA_API void naDebugUIElement(const void* elem);


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_ELEMENT_INCLUDED



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
