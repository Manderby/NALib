
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


// The following struct stores all relevant data which will then be stored in
// a list of the running NAWINAPIApplication.
typedef struct NAWINAPITimerStruct NAWINAPITimerStruct;
struct NAWINAPITimerStruct {
  UINT      key;
  NAMutator func;
  void*     arg;
};

typedef struct NAWINAPIColor NAWINAPIColor;
struct NAWINAPIColor {
  COLORREF color;
  HBRUSH   brush;
};

typedef struct NAWINAPIApplication NAWINAPIApplication;
struct NAWINAPIApplication {
  NAApplication    application;
  NAList           timers;
  HWND             offscreenWindow;
  NONCLIENTMETRICS nonClientMetrics;
  HICON            appIcon;

  HFONT            systemFont;
  HFONT            titleFont;
  HFONT            monospaceFont;
  HFONT            paragraphFont;
  HFONT            mathFont;

  NA_UIElement*    mouseHoverElement;
  NAMenu*          lastOpenedMenu;
  UINT             nextMenuItemId;

  WNDPROC          oldButtonWindowProc;
  WNDPROC          oldCheckBoxWindowProc;
  WNDPROC          oldLabelWindowProc;
  WNDPROC          oldRadioWindowProc;
  WNDPROC          oldSliderWindowProc;
  WNDPROC          oldTextFieldWindowProc;

  NAWINAPIColor    fgColor;
  NAWINAPIColor    fgColorDisabled;
  NAWINAPIColor    bgColor;
  NAWINAPIColor    bgColorAlternate;
  NAWINAPIColor    bgColorAlternate2;
};
NA_HAPI void na_DestructWINAPIApplication(NAWINAPIApplication* winapiApplication);
NA_RUNTIME_TYPE(NAWINAPIApplication, na_DestructWINAPIApplication, NA_FALSE);

typedef struct NAWINAPIButton NAWINAPIButton;
struct NAWINAPIButton{
  NAButton   button;
  uint32 state;
};
NA_HAPI void na_DestructWINAPIButton(NAWINAPIButton* winapiButton);
NA_RUNTIME_TYPE(NAWINAPIButton, na_DestructWINAPIButton, NA_FALSE);

typedef struct NAWINAPICheckBox NAWINAPICheckBox;
struct NAWINAPICheckBox {
  NACheckBox checkBox;
};
NA_HAPI void na_DestructWINAPICheckBox(NAWINAPICheckBox* winapiCheckBox);
NA_RUNTIME_TYPE(NAWINAPICheckBox, na_DestructWINAPICheckBox, NA_FALSE);

typedef struct NAWINAPIImageSpace NAWINAPIImageSpace;
struct NAWINAPIImageSpace {
  NAImageSpace imageSpace;
  NAUIImage*   image;
};
NA_HAPI void na_DestructWINAPIImageSpace(NAWINAPIImageSpace* winapiImageSpace);
NA_RUNTIME_TYPE(NAWINAPIImageSpace, na_DestructWINAPIImageSpace, NA_FALSE);

typedef struct NAWINAPILabel NAWINAPILabel;
struct NAWINAPILabel {
  NALabel   label;
  NABool    enabled;
  NAString* href;
};
NA_HAPI void na_DestructWINAPILabel(NAWINAPILabel* winapiLabel);
NA_RUNTIME_TYPE(NAWINAPILabel, na_DestructWINAPILabel, NA_FALSE);

typedef struct NAWINAPIMenu NAWINAPIMenu;
struct NAWINAPIMenu {
  NAMenu   menu;
  HMENU    hMenu;
};
NA_HAPI void na_DestructWINAPIMenu(NAWINAPIMenu* winapiMenu);
NA_RUNTIME_TYPE(NAWINAPIMenu, na_DestructWINAPIMenu, NA_FALSE);

typedef struct NAWINAPIMenuItem NAWINAPIMenuItem;
struct NAWINAPIMenuItem {
  NAMenuItem   menuItem;
  NABool       isSeparator;
  NAString*    text;
};
NA_HAPI void na_DestructWINAPIMenuItem(NAWINAPIMenuItem* winapiMenuItem);
NA_RUNTIME_TYPE(NAWINAPIMenuItem, na_DestructWINAPIMenuItem, NA_FALSE);

#if NA_COMPILE_OPENGL == 1
  //#include "../../../../../NADateTime.h"
  //
  typedef struct NAWINAPIOpenGLSpace NAWINAPIOpenGLSpace;
  struct NAWINAPIOpenGLSpace {
    NAOpenGLSpace openGLSpace;
    HGLRC         hRC;    // The rendering context for OpenGL
  };
  NA_HAPI void na_DestructWINAPIOpenGLSpace(NAWINAPIOpenGLSpace* winapiOpenGLSpace);
  NA_RUNTIME_TYPE(NAWINAPIOpenGLSpace, na_DestructWINAPIOpenGLSpace, NA_FALSE);
#endif

typedef struct NAWINAPIPopupButton NAWINAPIPopupButton;
struct NAWINAPIPopupButton {
  NAPopupButton   popupButton;
};
NA_HAPI void na_DestructWINAPIPopupButton(NAWINAPIPopupButton* winapiPopupButton);
NA_RUNTIME_TYPE(NAWINAPIPopupButton, na_DestructWINAPIPopupButton, NA_FALSE);

typedef struct NAWINAPIRadio NAWINAPIRadio;
struct NAWINAPIRadio {
  NARadio radio;
};
NA_HAPI void na_DestructWINAPIRadio(NAWINAPIRadio* winapiRadio);
NA_RUNTIME_TYPE(NAWINAPIRadio, na_DestructWINAPIRadio, NA_FALSE);

typedef struct NAWINAPISlider NAWINAPISlider;
struct NAWINAPISlider {
  NASlider slider;
};
NA_HAPI void na_DestructWINAPISlider(NAWINAPISlider* winapiSlider);
NA_RUNTIME_TYPE(NAWINAPISlider, na_DestructWINAPISlider, NA_FALSE);

typedef struct NAWINAPISpace NAWINAPISpace;
struct NAWINAPISpace {
  NASpace        space;
  NAWINAPIColor* lastBgColor;
};
NA_HAPI void na_DestructWINAPISpace(NAWINAPISpace* winapiSpace);
NA_RUNTIME_TYPE(NAWINAPISpace, na_DestructWINAPISpace, NA_FALSE);

typedef struct NAWINAPITextBox NAWINAPITextBox;
struct NAWINAPITextBox {
  NATextBox textBox;
  void*     nextTabStop;
  void*     prevTabStop;
};
NA_HAPI void na_DestructWINAPITextBox(NAWINAPITextBox* winapiTextBox);
NA_RUNTIME_TYPE(NAWINAPITextBox, na_DestructWINAPITextBox, NA_FALSE);

typedef struct NAWINAPITextField NAWINAPITextField;
struct NAWINAPITextField {
  NATextField textField;
  void*       nextTabStop;
  void*       prevTabStop;
};
NA_HAPI void na_DestructWINAPITextField(NAWINAPITextField* winapiTextField);
NA_RUNTIME_TYPE(NAWINAPITextField, na_DestructWINAPITextField, NA_FALSE);

typedef struct NAWINAPIWindow NAWINAPIWindow;
struct NAWINAPIWindow {
  NAWindow      window;
  NA_UIElement* firstResponder;
};
NA_HAPI void na_DestructWINAPIWindow(NAWINAPIWindow* winapiWindow);
NA_RUNTIME_TYPE(NAWINAPIWindow, na_DestructWINAPIWindow, NA_FALSE);

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
