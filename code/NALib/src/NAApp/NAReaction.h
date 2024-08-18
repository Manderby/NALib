
#if defined NA_REACTION_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_REACTION_INCLUDED
#define NA_REACTION_INCLUDED



// //////////////////////////////
// Reacting to commands
//
// Working with the UI in NALib works by defining reaction callbacks. Any user
// interface element can react to several commands.

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
  void*              uiElement,
  NAUICommand        command,
  NAReactionCallback callback,
  void*              controller);

NA_API void naAddUIKeyboardShortcut(
  void*              uiElement,
  NAKeyStroke        keyStroke,
  NAReactionCallback callback,
  void*              controller);

// Commands are somewhat specific. Not all commands work with all uiElement
// types.
//
// The KEYDOWN and KEYUP commands will only be called to a window but may
// propagate to parent elements.
//
// The CLOSES command will be called when a window tries to close. You can
// prevent it from closing by calling naPreventWindowClosing. That
// function only works in conjunction with such a command and will emit an
// error otherwise.

// Parameter and return type of a reaction callback:
//
// When a reaction callback returns NA_TRUE, the command is considered handeled
// and will not be processed any further. This means, if there are any other
// reaction callbacks registered which handle the same uiElement and command,
// they will not be called. If NA_FALSE is returned however, the command will
// be further processed. This means, if there are other callbacks in the list
// of the same uiElement or in any other element which is a parent of the one
// the command occured, they will then be executed the same way until one of
// the callbacks returns NA_TRUE.
//
// This allows you to propagate reactions outwards until they are handeled by
// an appropriate callback. If none of the registered callbacks returns NA_TRUE,
// the default message handling of the current operation system takes over.
//
// The parameter of a reaction callback is a NAReaction package, provided as
// a value copy. This means that reaction callbacks can not alter the package
// and provide it to an outer callback by returning NA_FALSE. This is by design.


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
