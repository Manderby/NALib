
#ifndef NA_WINAPI_INTEFACES_DEFINED
#define NA_WINAPI_INTEFACES_DEFINED

// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



// The following struct stores all relevant data which will then be stored in
// a list of the running NAWINAPIApplication.
NA_PROTOTYPE(NAWINAPITimerStruct);
struct NAWINAPITimerStruct {
  UINT      key;
  NAMutator func;
  void*     arg;
};



// Typedefs of all ui elements
NA_PROTOTYPE(NAWINAPIApplication);
NA_PROTOTYPE(NAWINAPIButton);
NA_PROTOTYPE(NAWINAPICheckBox);
NA_PROTOTYPE(NAWINAPIImageSpace);
NA_PROTOTYPE(NAWINAPILabel);
NA_PROTOTYPE(NAWINAPIMenu);
NA_PROTOTYPE(NA_WINAPIMenuItem);
#if NA_COMPILE_OPENGL == 1
  NA_PROTOTYPE(NAWINAPIOpenGLSpace);
#endif
NA_PROTOTYPE(NAWINAPIRadio);
NA_PROTOTYPE(NAWINAPIScreen);
NA_PROTOTYPE(NAWINAPISelect);
NA_PROTOTYPE(NAWINAPISlider);
NA_PROTOTYPE(NAWINAPISpace);
NA_PROTOTYPE(NAWINAPITextBox);
NA_PROTOTYPE(NAWINAPITextField);
NA_PROTOTYPE(NAWINAPIWindow);



// Prototypes of all ui element desctructors
NA_HAPI void na_DestructWINAPIApplication(NAWINAPIApplication* winapiApplication);
NA_HAPI void na_DestructWINAPIButton(NAWINAPIButton* winapiButton);
NA_HAPI void na_DestructWINAPICheckBox(NAWINAPICheckBox* winapiCheckBox);
NA_HAPI void na_DestructWINAPIImageSpace(NAWINAPIImageSpace* winapiImageSpace);
NA_HAPI void na_DestructWINAPILabel(NAWINAPILabel* winapiLabel);
NA_HAPI void na_DestructWINAPIMenu(NAWINAPIMenu* winapiMenu);
NA_HAPI void na_DestructWINAPIMenuItem(NA_WINAPIMenuItem* winapiMenuItem);
#if NA_COMPILE_OPENGL == 1
  NA_HAPI void na_DestructWINAPIOpenGLSpace(NAWINAPIOpenGLSpace* winapiOpenGLSpace);
#endif
NA_HAPI void na_DestructWINAPIRadio(NAWINAPIRadio* winapiRadio);
NA_HAPI void na_DestructWINAPIScreen(NAWINAPIScreen* winapiScreen);
NA_HAPI void na_DestructWINAPISelect(NAWINAPISelect* winapiSelect);
NA_HAPI void na_DestructWINAPISlider(NAWINAPISlider* winapiSlider);
NA_HAPI void na_DestructWINAPISpace(NAWINAPISpace* winapiSpace);
NA_HAPI void na_DestructWINAPITextBox(NAWINAPITextBox* winapiTextBox);
NA_HAPI void na_DestructWINAPITextField(NAWINAPITextField* winapiTextField);
NA_HAPI void na_DestructWINAPIWindow(NAWINAPIWindow* winapiWindow);



struct NAWINAPIApplication {
  NAApplication    application;
  NAList           timers;
  NAList           openGLRedrawList;
  HWND             offscreenWindow;
  NONCLIENTMETRICS nonClientMetrics;
  HICON            appIcon;

  NA_UIElement*    mouseHoverElement;
  const NAMenu*    lastOpenedMenu;
  UINT             nextMenuItemId;

  WNDPROC          oldButtonWindowProc;
  WNDPROC          oldCheckBoxWindowProc;
  WNDPROC          oldLabelWindowProc;
  WNDPROC          oldRadioWindowProc;
  WNDPROC          oldSelectWindowProc;
  WNDPROC          oldSliderWindowProc;
  WNDPROC          oldTextFieldWindowProc;

  #if NA_DEBUG
    NAList         debugElements;
  #endif
};

struct NAWINAPIButton{
  NAButton button;
  NARect   rect;
  uint32   state;
};

struct NAWINAPICheckBox {
  NACheckBox checkBox;
  NARect     rect;
};

struct NAWINAPIImageSpace {
  NAImageSpace imageSpace;
  NARect       rect;
};

struct NAWINAPILabel {
  NALabel   label;
  NARect    rect;
  NABool    enabled;
  NAString* href;
};

struct NAWINAPIMenu {
  NAMenu   menu;
  HMENU    hMenu;
};

struct NA_WINAPIMenuItem {
  NAMenuItem   menuItem;
  uint32       id;
  NAMenu*      menu;
  NABool       isSeparator;
  NAString*    text;
  NABool       state;
};

#if NA_COMPILE_OPENGL == 1
  struct NAWINAPIOpenGLSpace {
    NAOpenGLSpace openGLSpace;
    NARect        rect;
    HGLRC         hRC;    // The rendering context for OpenGL
  };
#endif

struct NAWINAPIRadio {
  NARadio radio;
  NARect  rect;
};

struct NAWINAPIScreen {
  NAScreen screen;
};

struct NAWINAPISelect {
  NASelect select;
  NARect   rect;
};

struct NAWINAPISlider {
  NASlider slider;
  NARect   rect;
};

struct NAWINAPISpace {
  NASpace        space;
  NARect         rect;
  NAWINAPIColor* curBgColor;
  NABool         forceEraseBackground;
};

struct NAWINAPITextBox {
  NATextBox textBox;
  NARect    rect;
  void*     nextTabStop;
  void*     prevTabStop;
};

struct NAWINAPITextField {
  NATextField textField;
  NARect      rect;
  void*       nextTabStop;
  void*       prevTabStop;
};

struct NAWINAPIWindow {
  NAWindow      window;
  NARect        rect;
  NA_UIElement* firstResponder;
};



NA_RUNTIME_TYPE(NAWINAPIApplication, na_DestructWINAPIApplication, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPIButton, na_DestructWINAPIButton, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPICheckBox, na_DestructWINAPICheckBox, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPIImageSpace, na_DestructWINAPIImageSpace, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPILabel, na_DestructWINAPILabel, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPIMenu, na_DestructWINAPIMenu, NA_FALSE);
NA_RUNTIME_TYPE(NA_WINAPIMenuItem, na_DestructWINAPIMenuItem, NA_FALSE);
#if NA_COMPILE_OPENGL == 1
  NA_RUNTIME_TYPE(NAWINAPIOpenGLSpace, na_DestructWINAPIOpenGLSpace, NA_FALSE);
#endif
NA_RUNTIME_TYPE(NAWINAPIRadio, na_DestructWINAPIRadio, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPIScreen, na_DestructWINAPIScreen, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPISelect, na_DestructWINAPISelect, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPISlider, na_DestructWINAPISlider, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPISpace, na_DestructWINAPISpace, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPITextBox, na_DestructWINAPITextBox, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPITextField, na_DestructWINAPITextField, NA_FALSE);
NA_RUNTIME_TYPE(NAWINAPIWindow, na_DestructWINAPIWindow, NA_FALSE);



#if NA_COMPILE_OPENGL == 1
  NA_HAPI void naAddOpenGLSpaceToRedrawList(NAWINAPIOpenGLSpace* openGLSpace);
#endif


#endif // NA_WINAPI_INTEFACES_DEFINED

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
