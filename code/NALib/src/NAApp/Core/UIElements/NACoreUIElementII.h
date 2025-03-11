
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitCoreUIElement(NA_UIElement* uiElement, NAUIElementType elementType, void* nativePtr) {
  uiElement->parent = NA_NULL;
  uiElement->elementType = elementType;
  uiElement->nativePtr = nativePtr;
  naInitList(&uiElement->reactions);
  naInitList(&uiElement->shortcuts);
  
  na_InitSystemUIElement(uiElement, nativePtr);
  
  uiElement->flags = 0;

  uiElement->hoverReactionCount = 0;
  if(elementType == NA_UI_BUTTON) {
    uiElement->hoverReactionCount++;
  }

  uiElement->mouseTrackingCount = 0;
  uiElement->mouseTracking = NA_NULL;
  
  naAddListLastMutable(&naGetApplication()->uiElements, uiElement);
}



// Takes ownership of shortcut
NA_KeyboardShortcutReaction* na_AllocKeyboardShortcutReaction(
  void* controller,
  NAKeyStroke* shortcut,
  NAReactionCallback callback)
{
  NA_KeyboardShortcutReaction* keyReaction;
  keyReaction = naAlloc(NA_KeyboardShortcutReaction);
  keyReaction->controller = controller;
  keyReaction->shortcut = shortcut; // takes ownership
  keyReaction->callback = callback;
  return keyReaction;
}



void na_DeallocKeyboardShortcutReaction(NA_KeyboardShortcutReaction* keyReaction) {
  naDelete(keyReaction->shortcut);
  naFree(keyReaction);
}



NA_HDEF void na_ClearCoreUIElement(NA_UIElement* uiElement) {
  if(uiElement->mouseTracking) {
    na_ClearMouseTracking(uiElement, uiElement->mouseTracking);
  }
  
  naClearList(&uiElement->reactions, (NAMutator)naFree);
  naClearList(&uiElement->shortcuts, (NAMutator)na_DeallocKeyboardShortcutReaction);
  
  na_ClearSystemUIElement(uiElement->nativePtr);
  na_ClearUINativePtr(uiElement->nativePtr);

  naRemoveListData(&naGetApplication()->uiElements, uiElement);
}



NA_HDEF NABool na_UIHasElementCommandDispatches(const NA_UIElement* element, NAUICommand command) {
  NAListIterator iter = naMakeListAccessor(&element->reactions);
  while(naIterateList(&iter)) {
    const NA_EventReaction* eventReaction = naGetListCurConst(&iter);
    if(eventReaction->command == command) {
      return NA_TRUE;
    }
  }
  naClearListIterator(&iter);

  return NA_FALSE;
}



NA_HDEF NABool na_DispatchUIElementCommand(const NA_UIElement* element, NAUICommand command) {
  NABool hasReaction = NA_FALSE;

  NAReaction reaction = {
    element,
    command,
    NA_NULL};

  NAListIterator iter = naMakeListMutator(&element->reactions);
  while(naIterateList(&iter)) {
    NA_EventReaction* eventReaction = naGetListCurMutable(&iter);
    if(eventReaction->command == command) {
      reaction.controller = eventReaction->controller;
      eventReaction->callback(reaction);
      hasReaction = NA_TRUE;
    }
  }
  naClearListIterator(&iter);

  if(!hasReaction) {
    // If the command has no reaction, search for other reactions in the
    // parent elements.  
    if(command != NA_UI_COMMAND_MOUSE_ENTERED && command != NA_UI_COMMAND_MOUSE_EXITED) {
      const NA_UIElement* parentElement = (const NA_UIElement*)naGetUIElementParentConst(element);
      return parentElement
        ? na_DispatchUIElementCommand(parentElement, command)
        : NA_FALSE;
    }
  }
  return NA_TRUE;
}



NA_DEF void naRefreshUIElement(void* uiElement, double timediff) {
  if(timediff == 0.) {
    na_RefreshUIElementNow(uiElement);
  }else{
    naCallApplicationFunctionInSeconds(na_RefreshUIElementNow, (NA_UIElement*)uiElement, timediff);
  }
}



NA_DEF NAUIElementType naGetUIElementType(const void* uiElement) {
  return uiElement ? ((NA_UIElement*)uiElement)->elementType : NA_UI_ELEMENT_UNDEFINED;
}



NA_DEF void* naGetUIElementNativePtr(void* uiElement) {
  return uiElement ? ((NA_UIElement*)uiElement)->nativePtr : NA_NULL;
}



NA_DEF void* naGetUIElementNativePtrConst(const void* uiElement) {
  return uiElement ? ((NA_UIElement*)uiElement)->nativePtr : NA_NULL;
}



NA_DEF void naAddUIReaction(void* uiElement, NAUICommand command, NAReactionCallback callback, void* controller) {
  NA_EventReaction* eventReaction;
  NA_UIElement* element = (NA_UIElement*)uiElement;
  
  #if NA_DEBUG
    if((command == NA_UI_COMMAND_RESHAPE)
      && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
      naError("Only windows can receyve RESHAPE commands.");
    if((command == NA_UI_COMMAND_MOUSE_MOVED)
      && ((naGetUIElementType(uiElement) == NA_UI_APPLICATION)
       || (naGetUIElementType(uiElement) == NA_UI_SCREEN)))
      naError("Application and screen can NOT receyve MOUSE_MOVED commands.");
    if((command == NA_UI_COMMAND_MOUSE_MOVED)
      && ((naGetUIElementType(uiElement) == NA_UI_BUTTON)
       || (naGetUIElementType(uiElement) == NA_UI_CHECKBOX)
       || (naGetUIElementType(uiElement) == NA_UI_LABEL)
       || (naGetUIElementType(uiElement) == NA_UI_MENU)
       || (naGetUIElementType(uiElement) == NA_UI_MENUITEM)
       || (naGetUIElementType(uiElement) == NA_UI_RADIO)
       || (naGetUIElementType(uiElement) == NA_UI_SELECT)
       || (naGetUIElementType(uiElement) == NA_UI_SLIDER)
       || (naGetUIElementType(uiElement) == NA_UI_TEXTBOX)
       || (naGetUIElementType(uiElement) == NA_UI_TEXTFIELD)))
      naError("MOUSE_MOVED command not implemented yet for this element type.");
    if((command == NA_UI_COMMAND_CLOSES)
      && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
      naError("Only windows can receyve CLOSES commands.");
    if((command == NA_UI_COMMAND_PRESSED)
      && (naGetUIElementType(uiElement) != NA_UI_BUTTON)
      && (naGetUIElementType(uiElement) != NA_UI_CHECKBOX)
      && (naGetUIElementType(uiElement) != NA_UI_RADIO)
      && (naGetUIElementType(uiElement) != NA_UI_MENU)
      && (naGetUIElementType(uiElement) != NA_UI_MENUITEM)
      && (naGetUIElementType(uiElement) != NA_UI_SLIDER)
      && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
      naError("Only buttons, checkBoxes, radios, menus, menuItems and sliders can receyve PRESSED commands.");
    if((command == NA_UI_COMMAND_EDITED)
      && (naGetUIElementType(uiElement) != NA_UI_TEXTBOX)
      && (naGetUIElementType(uiElement) != NA_UI_TEXTFIELD)
      && (naGetUIElementType(uiElement) != NA_UI_SLIDER))
      naError("Only textFields or Sliders can receyve EDITED commands.");
    if((command == NA_UI_COMMAND_EDIT_FINISHED)
      && (naGetUIElementType(uiElement) != NA_UI_SLIDER)
      && (naGetUIElementType(uiElement) != NA_UI_TEXTFIELD))
      naError("Only textFields and sliders can receyve EDIT_FINISHED commands.");
  #endif
  
  eventReaction = naAlloc(NA_EventReaction);
  eventReaction->controller = controller;
  eventReaction->command = command;
  eventReaction->callback = callback;

  naAddListLastMutable(&element->reactions, eventReaction);
  if(command == NA_UI_COMMAND_MOUSE_MOVED
  || command == NA_UI_COMMAND_MOUSE_ENTERED
  || command == NA_UI_COMMAND_MOUSE_EXITED) {
    element->hoverReactionCount++;
  
    NA_UIElement* trackedElement = element;
    if(naGetUIElementType(uiElement) == NA_UI_WINDOW) {
      trackedElement = &naGetWindowContentSpace((NAWindow*)uiElement)->uiElement;
    }
    na_RetainMouseTracking(trackedElement);
  }
  
//  if(command == NA_UI_COMMAND_MOUSE_MOVED) {
//    NA_UIElement* trackedElement = element;
//    if(naGetUIElementType(uiElement) == NA_UI_WINDOW) {
//      trackedElement = &naGetWindowContentSpace((NAWindow*)uiElement)->uiElement;
//    }
//    na_RetainMouseTracking(trackedElement);
//  }
}



NA_DEF void* naGetUIElementParent(void* uiElement) {
  return uiElement ? ((NA_UIElement*)uiElement)->parent : NA_NULL;
}



NA_DEF const void* naGetUIElementParentConst(const void* uiElement) {
  return uiElement ? ((NA_UIElement*)uiElement)->parent : NA_NULL;
}



NA_DEF NAWindow* naGetUIElementWindow(void* uiElement) {
  void* curElement = uiElement;
  while(curElement && naGetUIElementType(curElement) != NA_UI_WINDOW) {
    curElement = naGetUIElementParent(curElement);
  }
  return curElement;
}



NA_DEF const NAWindow* naGetUIElementWindowConst(const void* uiElement) {
  const void* curElement = uiElement;
  while(curElement && naGetUIElementType(curElement) != NA_UI_WINDOW) {
    curElement = naGetUIElementParentConst(curElement);
  }
  return curElement;
}



NA_DEF NASpace* naGetUIElementParentSpace(void* uiElement) {
  NASpace* parent = naGetUIElementParent(uiElement);
  while(parent && naGetUIElementType(parent) != NA_UI_SPACE) {
    parent = naGetUIElementParent(parent);
  }
  return parent;
}



NA_DEF const NASpace* naGetUIElementParentSpaceConst(const void* uiElement) {
  const NASpace* parent = naGetUIElementParentConst(uiElement);
  while(parent && naGetUIElementType(parent) != NA_UI_SPACE) {
    parent = naGetUIElementParentConst(parent);
  }
  return parent;
}



NA_DEF NARect naGetUIElementRectAbsolute(const void* uiElement) {
  NARect rect;
  const NA_UIElement* elem = (const NA_UIElement*)uiElement;

  if(!elem)
    return naMakeRectZero();

  rect = naGetUIElementRect(elem);
  elem = naGetUIElementParentConst(elem);
  while(elem) {
    NARect curRect = naGetUIElementRect(elem);
    rect.pos.x += curRect.pos.x;
    rect.pos.y += curRect.pos.y;
    elem = naGetUIElementParentConst(elem);
  }

  return rect;
}



NA_DEF NARect naGetUIElementRect(const void* uiElement) {

  NARect elemRect;
  switch(naGetUIElementType(uiElement)) {
  case NA_UI_APPLICATION:  elemRect = na_GetApplicationRect(uiElement); break;
  case NA_UI_BUTTON:       elemRect = na_GetButtonRect(uiElement); break;
  case NA_UI_CHECKBOX:     elemRect = na_GetCheckBoxRect(uiElement); break;
  case NA_UI_IMAGE_SPACE:  elemRect = na_GetImageSpaceRect(uiElement); break;
  case NA_UI_LABEL:        elemRect = na_GetLabelRect(uiElement); break;
  case NA_UI_MENU:         elemRect = na_GetMenuRect(uiElement); break;
  case NA_UI_MENUITEM:     elemRect = na_GetMenuItemRect(uiElement); break;
  case NA_UI_METAL_SPACE:  elemRect = na_GetMetalSpaceRect(uiElement); break;
  case NA_UI_OPENGL_SPACE: elemRect = na_GetOpenGLSpaceRect(uiElement); break;
  case NA_UI_RADIO:        elemRect = na_GetRadioRect(uiElement); break;
  case NA_UI_SCREEN:       elemRect = na_GetScreenRect(uiElement); break;
  case NA_UI_SELECT:       elemRect = na_GetSelectRect(uiElement); break;
  case NA_UI_SLIDER:       elemRect = na_GetSliderRect(uiElement); break;
  case NA_UI_SPACE:        elemRect = na_GetSpaceRect(uiElement); break;
  case NA_UI_TEXTBOX:      elemRect = na_GetTextBoxRect(uiElement); break;
  case NA_UI_TEXTFIELD:    elemRect = na_GetTextFieldRect(uiElement); break;
  case NA_UI_WINDOW:       elemRect = na_GetWindowRect(uiElement); break;
  default:                 elemRect = naMakeRectZero(); break;
  }

  return elemRect;
}



NA_DEF void naSetUIElementRect(void* uiElement, NARect rect) {
  switch(naGetUIElementType(uiElement)) {
  case NA_UI_APPLICATION:  na_SetApplicationRect(uiElement, rect); break;
  case NA_UI_BUTTON:       na_SetButtonRect(uiElement, rect); break;
  case NA_UI_CHECKBOX:     na_SetCheckBoxRect(uiElement, rect); break;
  case NA_UI_IMAGE_SPACE:  na_SetImageSpaceRect(uiElement, rect); break;
  case NA_UI_LABEL:        na_SetLabelRect(uiElement, rect); break;
  case NA_UI_MENU:         na_SetMenuRect(uiElement, rect); break;
  case NA_UI_MENUITEM:     na_SetMenuItemRect(uiElement, rect); break;
  case NA_UI_METAL_SPACE:  na_SetMetalSpaceRect(uiElement, rect); break;
  case NA_UI_OPENGL_SPACE: na_SetOpenGLSpaceRect(uiElement, rect); break;
  case NA_UI_RADIO:        na_SetRadioRect(uiElement, rect); break;
  case NA_UI_SCREEN:       na_SetScreenRect(uiElement, rect); break;
  case NA_UI_SELECT:       na_SetSelectRect(uiElement, rect); break;
  case NA_UI_SLIDER:       na_SetSliderRect(uiElement, rect); break;
  case NA_UI_SPACE:        na_SetSpaceRect(uiElement, rect); break;
  case NA_UI_TEXTBOX:      na_SetTextBoxRect(uiElement, rect); break;
  case NA_UI_TEXTFIELD:    na_SetTextFieldRect(uiElement, rect); break;
  case NA_UI_WINDOW:       na_SetWindowRect(uiElement, rect); break;
  default:                 break;
  }
}



NA_HDEF void na_RetainMouseTracking(NA_UIElement* uiElement) {
  uiElement->mouseTrackingCount++;
  if(uiElement->mouseTrackingCount == 1) {
    uiElement->mouseTracking = na_AddMouseTracking(uiElement);
  }
}

NA_HDEF void na_ReleaseMouseTracking(NA_UIElement* uiElement) {
  uiElement->mouseTrackingCount--;
  if(uiElement->mouseTrackingCount == 0) {
    na_ClearMouseTracking(uiElement, uiElement->mouseTracking);
    uiElement->mouseTracking = NA_NULL;
  }
}

NA_HDEF void na_UpdateMouseTracking(NA_UIElement* uiElement) {
  if(uiElement->mouseTracking) {
    na_ClearMouseTracking(uiElement, uiElement->mouseTracking);
    uiElement->mouseTracking = na_AddMouseTracking(uiElement);
  }
}



NA_DEF void naAddUIKeyboardShortcut(
  void* uiElement,
  NAKeyStroke* shortcut,
  NAReactionCallback callback,
  void* controller)
{
  NA_UIElement* element = (NA_UIElement*)uiElement;
  //#if NA_DEBUG
  //  if((naGetUIElementType(uiElement) != NA_UI_APPLICATION) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
  //    naError("Currently, only applications and windows are allowed as uiElement. Use naGetApplication() for the app.");
  //#endif
  naAddListLastMutable(&element->shortcuts, na_AllocKeyboardShortcutReaction(
    controller,
    shortcut, // takes ownership
    callback));
}




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
