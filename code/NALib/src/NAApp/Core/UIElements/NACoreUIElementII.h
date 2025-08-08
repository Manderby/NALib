
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

  uiElement->mouseTrackingCount = 0;
  uiElement->mouseTracking = NA_NULL;
  
  naAddListLastMutable(&naGetApplication()->uiElements, uiElement);
}



NA_HDEF void na_IncCoreUIElementHoverTrackingCount(NA_UIElement* uiElement) {
  uiElement->hoverReactionCount++;
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
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  if(uiElement->mouseTracking) {
    na_ClearMouseTracking(uiElement, uiElement->mouseTracking);
  }
  
  naClearList(&uiElement->reactions, (NAMutator)naFree);
  naClearList(&uiElement->shortcuts, (NAMutator)na_DeallocKeyboardShortcutReaction);
  
  na_ClearSystemUIElement(uiElement->nativePtr);
  na_ClearUINativePtr(uiElement->nativePtr);

  naRemoveListData(&naGetApplication()->uiElements, uiElement);

  na_UndebugUIElement(uiElement);
}



NA_HDEF NABool na_UIHasElementCommandDispatches(const NA_UIElement* uiElement, NAUICommand command) {
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  NAListIterator iter = naMakeListAccessor(&uiElement->reactions);
  while(naIterateList(&iter)) {
    const NA_EventReaction* eventReaction = naGetListCurConst(&iter);
    if(eventReaction->command == command) {
      return NA_TRUE;
    }
  }
  naClearListIterator(&iter);

  return NA_FALSE;
}



NA_HDEF NABool na_DispatchUIElementCommand(const NA_UIElement* uiElement, NAUICommand command) {
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  const NA_EventReaction* eventReaction = NA_NULL;
  NAListIterator iter = naMakeListAccessor(&uiElement->reactions);
  while(naIterateList(&iter)) {
    eventReaction = naGetListCurConst(&iter);
    if(eventReaction->command == command) {
      break;
    }
    eventReaction = NA_NULL;
  }
  naClearListIterator(&iter);

  if(eventReaction) {
    // We found a suitable reaction. Execute it.
    NAReaction reaction = {
      uiElement,
      command,
      NA_NULL};
    reaction.controller = eventReaction->controller;
    eventReaction->callback(reaction);
  }else{
    // If the command has no reaction, search for other reactions in the
    // parent elements.  
    if(command != NA_UI_COMMAND_MOUSE_ENTERED && command != NA_UI_COMMAND_MOUSE_EXITED) {
      const NA_UIElement* parentElement = (const NA_UIElement*)naGetUIElementParent(uiElement);
      return parentElement
        ? na_DispatchUIElementCommand(parentElement, command)
        : NA_FALSE;
    }
  }
  return NA_TRUE;
}



NA_DEF void naRefreshUIElement(void* uiElement, double timediff) {
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  if(timediff == 0.) {
    na_RefreshUIElementNow(uiElement);
  }else{
    naCallApplicationFunctionInSeconds(na_RefreshUIElementNow, (NA_UIElement*)uiElement, timediff);
  }
}



NA_DEF NAUIElementType naGetUIElementType(const void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  return ((NA_UIElement*)uiElement)->elementType;
}



NA_DEF void* naGetUIElementNativePtr(void* uiElement) {
  return uiElement ? ((NA_UIElement*)uiElement)->nativePtr : NA_NULL;
}



NA_DEF void* naGetUIElementNativePtrConst(const void* uiElement) {
  return uiElement ? ((NA_UIElement*)uiElement)->nativePtr : NA_NULL;
}



NA_DEF void naAddUIReaction(
  void* uiElement,
  NAUICommand command,
  NAReactionCallback callback,
  void* controller)
{
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  NA_UIElement* element = (NA_UIElement*)uiElement;

  NABool reactionFound = NA_FALSE;
  NAListIterator iter = naMakeListAccessor(&element->reactions);
  while(naIterateList(&iter)) {
    const NA_EventReaction* existingEventReaction = naGetListCurConst(&iter);
    if(existingEventReaction->command == command) {
      reactionFound = NA_TRUE;
      break;
    }
  }
  naClearListIterator(&iter);
  if(reactionFound) {
    #if NA_DEBUG
      naError("One UI element can only have one reaction per command. Ignoring this new reaction.");
    #endif
    return;
  }

  NAUIElementType elementType = naGetUIElementType(uiElement);

  #if NA_DEBUG
    if((command == NA_UI_COMMAND_RESHAPE)
      && (elementType != NA_UI_APPLICATION)
      && (elementType != NA_UI_WINDOW))
      naError("Only windows and the application can receyve RESHAPE commands.");
    if((command == NA_UI_COMMAND_MOUSE_MOVED)
      && ((elementType == NA_UI_APPLICATION)
       || (elementType == NA_UI_SCREEN)))
      naError("Application and screen can NOT receyve MOUSE_MOVED commands.");
    if((command == NA_UI_COMMAND_MOUSE_MOVED)
      && ((elementType == NA_UI_BUTTON)
       || (elementType == NA_UI_CHECKBOX)
       || (elementType == NA_UI_LABEL)
       || (elementType == NA_UI_MENU)
       || (elementType == NA_UI_MENUITEM)
       || (elementType == NA_UI_RADIO)
       || (elementType == NA_UI_SELECT)
       || (elementType == NA_UI_SLIDER)
       || (elementType == NA_UI_TEXTBOX)
       || (elementType == NA_UI_TEXTFIELD)))
      naError("MOUSE_MOVED command not implemented yet for this element type.");
    if((command == NA_UI_COMMAND_CLOSES)
      && (elementType != NA_UI_WINDOW))
      naError("Only windows can receyve CLOSES commands.");
    if((command == NA_UI_COMMAND_PRESSED)
      && (elementType != NA_UI_BUTTON)
      && (elementType != NA_UI_CHECKBOX)
      && (elementType != NA_UI_RADIO)
      && (elementType != NA_UI_MENU)
      && (elementType != NA_UI_MENUITEM)
      && (elementType != NA_UI_SLIDER)
      && (elementType != NA_UI_WINDOW))
      naError("Only buttons, checkBoxes, radios, menus, menuItems and sliders can receyve PRESSED commands.");
    if((command == NA_UI_COMMAND_EDITED)
      && (elementType != NA_UI_TEXTBOX)
      && (elementType != NA_UI_TEXTFIELD)
      && (elementType != NA_UI_SLIDER))
      naError("Only textFields or Sliders can receyve EDITED commands.");
    if((command == NA_UI_COMMAND_EDIT_FINISHED)
      && (elementType != NA_UI_SLIDER)
      && (elementType != NA_UI_TEXTFIELD))
      naError("Only textFields and sliders can receyve EDIT_FINISHED commands.");
#endif // NA_DEBUG
  
  NA_EventReaction* eventReaction = naAlloc(NA_EventReaction);
  eventReaction->controller = controller;
  eventReaction->command = command;
  eventReaction->callback = callback;

  naAddListLastMutable(&element->reactions, eventReaction);
  if(command == NA_UI_COMMAND_MOUSE_MOVED
  || command == NA_UI_COMMAND_MOUSE_ENTERED
  || command == NA_UI_COMMAND_MOUSE_EXITED) {
    element->hoverReactionCount++;
  
    NA_UIElement* trackedElement = element;
    if(elementType == NA_UI_WINDOW) {
      trackedElement = &naGetWindowContentSpace((NAWindow*)uiElement)->uiElement;
    }
    na_RetainMouseTracking(trackedElement);
  }
}


#if NA_OS == NA_OS_WINDOWS

// todo: This is just so that the windows version compiles.
// but should be done the same way as below.

NA_DEF double naGetUIElementUIScale(const void* uiElement) {
	NA_UNUSED(uiElement);
	int dpi;
	HDC hDC = GetDC(NULL);
	if (hDC) {
		dpi = GetDeviceCaps(hDC, LOGPIXELSX);
		ReleaseDC(NULL, hDC);
	}
	else {
		dpi = USER_DEFAULT_SCREEN_DPI;
	}
	return (double)dpi / (double)USER_DEFAULT_SCREEN_DPI;
}

#else

NA_DEF double naGetUIElementUIScale(const void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  if(naGetUIElementType(uiElement) == NA_UI_SCREEN) {
    return naGetScreenUIScale(uiElement);
  }

  const void* parent = naGetUIElementParent(uiElement);
  if(parent) {
    return naGetUIElementUIScale(parent);
  }else{
    return 1.;
  }
}

#endif

NA_DEF const void* naGetUIElementParent(const void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  return uiElement ? ((NA_UIElement*)uiElement)->parent : NA_NULL;
}



NA_DEF void* naGetUIElementParentMutable(void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  return uiElement ? ((NA_UIElement*)uiElement)->parent : NA_NULL;
}



NA_DEF const NAWindow* naGetUIElementWindow(const void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  const void* curElement = uiElement;
  while(curElement && naGetUIElementType(curElement) != NA_UI_WINDOW) {
    curElement = naGetUIElementParent(curElement);
  }
  return curElement;
}



NA_DEF NAWindow* naGetUIElementWindowMutable(void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  void* curElement = uiElement;
  while(curElement && naGetUIElementType(curElement) != NA_UI_WINDOW) {
    curElement = naGetUIElementParentMutable(curElement);
  }
  return curElement;
}



NA_DEF const NASpace* naGetUIElementParentSpace(const void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  const NASpace* parent = naGetUIElementParent(uiElement);
  while(parent && naGetUIElementType(parent) != NA_UI_SPACE) {
    parent = naGetUIElementParent(parent);
  }
  return parent;
}



NA_DEF NASpace* naGetUIElementParentSpaceMutable(void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  NASpace* parent = naGetUIElementParentMutable(uiElement);
  while(parent && naGetUIElementType(parent) != NA_UI_SPACE) {
    parent = naGetUIElementParentMutable(parent);
  }
  return parent;
}



NA_DEF NARect naGetUIElementRectAbsolute(const void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  NARect rect;
  const NA_UIElement* elem = (const NA_UIElement*)uiElement;
  rect = naGetUIElementRect(elem);

  while(1) {
    NAUIElementType type = naGetUIElementType(elem);
    if(type == NA_UI_WINDOW || type == NA_UI_SCREEN || type == NA_UI_APPLICATION)
      break;

    elem = naGetUIElementParent(elem);
    if(!elem) 
      break;
    
    NARect curRect = naGetUIElementRect(elem);
    rect.pos.x += curRect.pos.x;
    rect.pos.y += curRect.pos.y;
  }

  return rect;
}



NA_DEF NARect naGetUIElementRect(const void* uiElement) {
  NARect elemRect = naMakeRectS(0., 0., 1., 1.);

  if(uiElement) {
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
    }
  }

  return elemRect;
}



NA_DEF void naSetUIElementRect(void* uiElement, NARect rect) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

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



NA_DEF void na_UpdateUIElementUIScale(void* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  switch(naGetUIElementType(uiElement)) {
  case NA_UI_APPLICATION:
    #if NA_DEBUG
      naError("The application should never be used as an argument for this function.");
    #endif
    break;
  case NA_UI_BUTTON:       na_UpdateButtonUIScale(uiElement); break;
  case NA_UI_CHECKBOX:     na_UpdateCheckBoxUIScale(uiElement); break;
  case NA_UI_IMAGE_SPACE:  na_UpdateImageSpaceUIScale(uiElement); break;
  case NA_UI_LABEL:        na_UpdateLabelUIScale(uiElement); break;
  case NA_UI_MENU:         na_UpdateMenuUIScale(uiElement); break;
  case NA_UI_MENUITEM:     na_UpdateMenuItemUIScale(uiElement); break;
  case NA_UI_METAL_SPACE:  na_UpdateMetalSpaceUIScale(uiElement); break;
  case NA_UI_OPENGL_SPACE: na_UpdateOpenGLSpaceUIScale(uiElement); break;
  case NA_UI_RADIO:        na_UpdateRadioUIScale(uiElement); break;
  case NA_UI_SCREEN:
    #if NA_DEBUG
      naError("A screen should never be used as an argument for this function.");
    #endif
    break;
  case NA_UI_SELECT:       na_UpdateSelectUIScale(uiElement); break;
  case NA_UI_SLIDER:       na_UpdateSliderUIScale(uiElement); break;
  case NA_UI_SPACE:        na_UpdateSpaceUIScale(uiElement); break;
  case NA_UI_TEXTBOX:      na_UpdateTextBoxUIScale(uiElement); break;
  case NA_UI_TEXTFIELD:    na_UpdateTextFieldUIScale(uiElement); break;
  case NA_UI_WINDOW:       na_UpdateWindowUIScale(uiElement); break;
  default:                 break;
  }
}



NA_HDEF void na_RetainMouseTracking(NA_UIElement* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  uiElement->mouseTrackingCount++;
  if(uiElement->mouseTrackingCount == 1) {
    uiElement->mouseTracking = na_AddMouseTracking(uiElement);
  }
}

NA_HDEF void na_ReleaseMouseTracking(NA_UIElement* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

  uiElement->mouseTrackingCount--;
  if(uiElement->mouseTrackingCount == 0) {
    na_ClearMouseTracking(uiElement, uiElement->mouseTracking);
    uiElement->mouseTracking = NA_NULL;
  }
}

NA_HDEF void na_UpdateMouseTracking(NA_UIElement* uiElement) {
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

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
  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is nullptr");
  #endif

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
