
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitUIElement(NA_UIElement* uiElement, NAUIElementType elementType, NANativePtr nativePtr){
  uiElement->parent = NA_NULL;
  uiElement->elementType = elementType;
  uiElement->nativePtr = nativePtr;
  naInitList(&(uiElement->reactions));
  naInitList(&(uiElement->shortcuts));
  uiElement->mouseInside = NA_FALSE;
  uiElement->allowNotifications = NA_TRUE;
  
  naAddListLastMutable(&(naGetApplication()->uiElements), uiElement);
}



NA_HDEF void na_ClearUIElement(NA_UIElement* uiElement){
  naForeachListMutable(&(uiElement->reactions), naFree);
  naForeachListMutable(&(uiElement->shortcuts), naFree);
  naClearList(&(uiElement->reactions));
  naClearList(&(uiElement->shortcuts));
  
  na_ClearUINativePtr(uiElement->nativePtr);

  naRemoveListData(&(naGetApplication()->uiElements), uiElement);
}



// todo: find a faster way. Hash perhaps or something else.
NA_HDEF void* na_GetUINALibEquivalent(NANativePtr nativePtr){
  NAListIterator iter;
  NA_UIElement* retelem = NA_NULL;
  naBeginListMutatorIteration(NA_UIElement* elem, &(na_App->uiElements), iter);
    if(elem->nativePtr == nativePtr){retelem = elem; break;}
  naEndListIteration(iter);
  return retelem;
}



NA_HDEF NABool na_DispatchUIElementCommand(const NA_UIElement* element, NAUICommand command){
  NABool finished = NA_FALSE;
  NAListIterator iter;

  NAReaction reaction;
  reaction.uiElement = element;
  reaction.command = command;
  naBeginListMutatorIteration(NAEventReaction* eventReaction, &(element->reactions), iter);
    if(eventReaction->command == command){
      reaction.controller = eventReaction->controller;
      finished = eventReaction->handler(reaction);
      // If the handler tells us to stop handling the command, we do so.
      if(finished){break;}
    }
  naEndListIteration(iter);

  // If the command has not been finished, search for other reactions in the parent elements.
  if(!finished && command != NA_UI_COMMAND_MOUSE_ENTERED && command != NA_UI_COMMAND_MOUSE_EXITED){
    const NA_UIElement* parentelement = (const NA_UIElement*)naGetUIElementParentConst(element);
    if(parentelement){finished = na_DispatchUIElementCommand(parentelement, command);}
  }

  return finished;
}



NA_DEF void naRefreshUIElement(void* uiElement, double timediff){
  //if(timediff == 0.){
  //  na_RefreshUIElementNow(uiElement);
  //}else{
    naCallApplicationFunctionInSeconds(na_RefreshUIElementNow, (NA_UIElement*)uiElement, timediff);
  //}
}



NA_DEF NAUIElementType naGetUIElementType(const void* uiElement){
  return ((NA_UIElement*)uiElement)->elementType;
}



NA_DEF NANativePtr naGetUIElementNativePtr(void* uiElement){
  return ((NA_UIElement*)uiElement)->nativePtr;
}



NA_DEF const NANativePtr naGetUIElementNativePtrConst(const void* uiElement){
  return ((const NA_UIElement*)uiElement)->nativePtr;
}



NA_DEF void naAddUIReaction(void* uiElement, NAUICommand command, NAReactionHandler handler, void* controller){
  NAEventReaction* eventReaction;
  NA_UIElement* element = (NA_UIElement*)uiElement;
  #if NA_DEBUG
    if((command == NA_UI_COMMAND_RESHAPE) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
      naError("Only windows can receyve RESHAPE commands.");
//    if((command == NA_UI_COMMAND_KEYDOWN) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
//      naError("Only windows can receyve KEYDOWN commands.");
//    if((command == NA_UI_COMMAND_KEYUP) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
//      naError("Only windows can receyve KEYUP commands.");
    if((command == NA_UI_COMMAND_MOUSE_MOVED) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
      naError("Only windows can receyve MOUSE_MOVED commands.");
//    if((command == NA_UI_COMMAND_MOUSE_ENTERED) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
//      naError("Only windows can receyve MOUSE_ENTERED commands.");
//    if((command == NA_UI_COMMAND_MOUSE_EXITED) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
//      naError("Only windows can receyve MOUSE_EXITED commands.");
    if((command == NA_UI_COMMAND_CLOSES) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
      naError("Only windows can receyve CLOSES commands.");
    if((command == NA_UI_COMMAND_PRESSED)
      && (naGetUIElementType(uiElement) != NA_UI_BUTTON)
      && (naGetUIElementType(uiElement) != NA_UI_CHECKBOX)
      && (naGetUIElementType(uiElement) != NA_UI_RADIO)
      && (naGetUIElementType(uiElement) != NA_UI_MENU)
      && (naGetUIElementType(uiElement) != NA_UI_MENUITEM)
      && (naGetUIElementType(uiElement) != NA_UI_SLIDER))
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
  eventReaction = naAlloc(NAEventReaction);
  eventReaction->controller = controller;
  eventReaction->command = command;
  eventReaction->handler = handler;
  // todo: this needs some attention on macOS
  //if(command == NA_UI_COMMAND_MOUSE_MOVED || command == NA_UI_COMMAND_MOUSE_ENTERED || command == NA_UI_COMMAND_MOUSE_EXITED){
  //  element->mouseTrackingCount++;
  //  if(element->mouseTrackingCount == 1){
  //    element->mouseTracking = na_AllocMouseTracking(naGetUIElementNativePtr(element));
  //  }
  //}
  naAddListLastMutable(&((element)->reactions), eventReaction);
}



NA_DEF void* naGetUIElementParent(void* uiElement){
  return ((NA_UIElement*)uiElement)->parent;
}



NA_DEF const void* naGetUIElementParentConst(const void* uiElement){
  return ((const NA_UIElement*)uiElement)->parent;
}



NA_DEF NAWindow* naGetUIElementWindow(void* uiElement){
  void* curElement = uiElement;
  while(curElement && naGetUIElementType(curElement) != NA_UI_WINDOW){
    curElement = naGetUIElementParent(curElement);
  }
  return curElement;
}



NA_DEF const NAWindow* naGetUIElementWindowConst(const void* uiElement){
  const void* curElement = uiElement;
  while(curElement && naGetUIElementType(curElement) != NA_UI_WINDOW){
    curElement = naGetUIElementParentConst(curElement);
  }
  return curElement;
}



NA_DEF NASpace* naGetUIElementParentSpace(void* uiElement){
  NASpace* parent = naGetUIElementParent(uiElement);
  while(parent && naGetUIElementType(parent) != NA_UI_SPACE){
    parent = naGetUIElementParent(parent);
  }
  return parent;
}



#endif // NA_COMPILE_GUI == 1
