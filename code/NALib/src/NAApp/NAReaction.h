
#ifndef NA_REACTION_INCLUDED
#define NA_REACTION_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// //////////////////////////////
// Reacting to commands
//
// Working with the UI in NALib works by defining reaction callbacks. Any user
// interface element can react to several commands.

typedef enum{
  NA_UI_COMMAND_REDRAW = 0,         // redraw
  NA_UI_COMMAND_RESHAPE,            // reshape
  NA_UI_COMMAND_KEY_DOWN,           // key
  NA_UI_COMMAND_KEY_UP,             // key
  NA_UI_COMMAND_MOUSE_DOWN,         // mouse
  NA_UI_COMMAND_MOUSE_UP,           // mouse
  NA_UI_COMMAND_MOUSE_MOVED,        // mouse
  NA_UI_COMMAND_MOUSE_ENTERED,      // mouse
  NA_UI_COMMAND_MOUSE_EXITED,       // mouse
  NA_UI_COMMAND_TRANSFORMED,        // reshape
  NA_UI_COMMAND_CLOSES,             // reshape
  NA_UI_COMMAND_PRESSED,            // interaction
  NA_UI_COMMAND_EDITED,             // interaction
  NA_UI_COMMAND_EDIT_FINISHED,      // interaction
  NA_UI_COMMAND_KEYBOARD_SHORTCUT   // key
} NAUICommand;



// When a command occurs in a certain uiElement, an information package with
// the NAReaction type will be created:
NA_PROTOTYPE(NAReaction);
struct NAReaction{
  const void* uiElement;
  NAUICommand command;
  void* controller;
};



// This NAReaction package is then provided to reaction callbacks with the
// following prototype. The parameter of a reaction callback is an NAReaction
// package, provided as a value copy.

typedef void(*NAReactionCallback)(NAReaction reaction);



// By calling naAddUIReaction or naAddUIKeyboardShortcut, the programmer can
// register, which reaction callback should be called when a certain uiElement
// reacts to a certain command or when a certain keyboard shortcut is pressed.
//
// The controller given to these functions is an arbitrary void pointer which
// simply will be available in the NAReaction package. You probably will use
// either NA_NULL or some kind of controller pointer for that, hence the name.
//
// You can have as many reactions as you like for any number of elements. They
// will be stored separately for every ui element in a list which will always
// be traversed sequentially forward from start to end.

NA_API void naAddUIReaction(
  void* uiElement,
  NAUICommand command,
  NAReactionCallback callback,
  void* controller);

// Takes the ownership of the keyStroke. Do not call naDelete on it!
NA_API void naAddUIKeyboardShortcut(
  void* uiElement,
  NAKeyStroke* keyStroke,
  NAReactionCallback callback,
  void* controller);

// Commands are somewhat specific. Not all commands work with all uiElement
// types. See header files of the elements for more information.



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_REACTION_INCLUDED



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
