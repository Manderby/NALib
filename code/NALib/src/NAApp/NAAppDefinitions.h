
#ifndef NA_APP_DEFINITIONS_INCLUDED
#define NA_APP_DEFINITIONS_INCLUDED



typedef enum{
  NA_UI_ELEMENT_UNDEFINED = 0,
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

typedef enum{
  NA_UI_COMMAND_REDRAW = 0,
  NA_UI_COMMAND_RESHAPE,
  NA_UI_COMMAND_KEY_DOWN,
  NA_UI_COMMAND_KEY_UP,
  NA_UI_COMMAND_MOUSE_DOWN,
  NA_UI_COMMAND_MOUSE_UP,
  NA_UI_COMMAND_MOUSE_MOVED,
  NA_UI_COMMAND_MOUSE_ENTERED,
  NA_UI_COMMAND_MOUSE_EXITED,
  NA_UI_COMMAND_TRANSFORMED,
  NA_UI_COMMAND_CLOSES,
  NA_UI_COMMAND_PRESSED,
  NA_UI_COMMAND_EDITED,
  NA_UI_COMMAND_EDIT_FINISHED,
  NA_UI_COMMAND_KEYBOARD_SHORTCUT
} NAUICommand;

typedef enum{
  NA_TEXT_ALIGNMENT_LEFT,
  NA_TEXT_ALIGNMENT_RIGHT,
  NA_TEXT_ALIGNMENT_CENTER
} NATextAlignment;



// When a command occurs in a certain uiElement, an information package with
// the NAReaction type will be created:
typedef struct NAReaction NAReaction;
struct NAReaction{
  const void* uiElement;
  NAUICommand command;
  void* controller;
};

// This NAReaction package is then provided to reaction callbacks with the
// following prototype.
typedef void(*NAReactionCallback)(NAReaction reaction);



typedef enum NAMouseButton {
  NA_MOUSE_BUTTON_LEFT = 0,
  NA_MOUSE_BUTTON_RIGHT = 1,
  NA_MOUSE_BUTTON_MIDDLE = 2,
  // additional mouse buttons have higher integer numbers are not implemented
  // explicitely.
} NAMouseButton;

// The modifier keys are stored as a bitmask with the following predefined
// elements:
typedef enum{
  NA_KEY_MODIFIER_NONE          = 0x0000,
  NA_KEY_MODIFIER_SHIFT         = 0x0003,
  NA_KEY_MODIFIER_CONTROL       = 0x000c,
  NA_KEY_MODIFIER_OPTION        = 0x0030,
  NA_KEY_MODIFIER_COMMAND       = 0x00c0

  // The following flags might be available in the future. Please be patient.
  //  NA_KEY_MODIFIER_LEFT_SHIFT    = 0x0001,
  //  NA_KEY_MODIFIER_RIGHT_SHIFT   = 0x0002,
  //  NA_KEY_MODIFIER_LEFT_CONTROL  = 0x0004,
  //  NA_KEY_MODIFIER_RIGHT_CONTROL = 0x0008,
  //  NA_KEY_MODIFIER_LEFT_OPTION   = 0x0010,
  //  NA_KEY_MODIFIER_RIGHT_OPTION  = 0x0020,
  //  NA_KEY_MODIFIER_LEFT_COMMAND  = 0x0040,
  //  NA_KEY_MODIFIER_RIGHT_COMMAND = 0x0080,
} NAKeyModifier;



#endif // NA_APP_DEFINITIONS_INCLUDED

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
