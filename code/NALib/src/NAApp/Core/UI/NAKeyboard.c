
#include "NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_DEF NAKeyStroke naMakeKeyStroke(uint32 modifiers, NAUIKeyCode keyCode){
  NAKeyStroke newStroke;
  newStroke.modifiers = modifiers;
  newStroke.keyCode = keyCode;
  return newStroke;
}



NA_DEF void naAddUIKeyboardShortcut(
  void* uiElement,
  NAKeyStroke shortcut,
  NAReactionHandler handler,
  void* controller)
{
  NAKeyboardShortcutReaction* keyReaction;
  NA_UIElement* element = (NA_UIElement*)uiElement;
  //#if NA_DEBUG
  //  if((naGetUIElementType(uiElement) != NA_UI_APPLICATION) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
  //    naError("Currently, only applications and windows are allowed as uiElement. Use naGetApplication() for the app.");
  //#endif
  keyReaction = naAlloc(NAKeyboardShortcutReaction);
  keyReaction->controller = controller;
  keyReaction->shortcut = shortcut;
  keyReaction->handler = handler;
  naAddListLastMutable(&((element)->shortcuts), keyReaction);
}



NA_DEF NAKeyStroke naGetCurrentKeyStroke(){
  return na_App->curKeyStroke;
}



#endif // NA_COMPILE_GUI == 1
