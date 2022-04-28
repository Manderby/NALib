
#include "NAAppCore.h"

#if (NA_COMPILE_GUI == 1)

#include "../../NAMemory.h"
#include "../../NACoord.h"
#include "../../NAThreading.h"
#include "../../NAPreferences.h"

// The pointer storing the app if any.
NAApplication* na_App = NA_NULL;



NA_HDEF void na_InitUIElement(NA_UIElement* uiElement, NAUIElementType elementType, NANativePtr nativePtr){
  uiElement->parent = NA_NULL;
  uiElement->elementType = elementType;
  uiElement->nativePtr = nativePtr;
  naInitList(&(uiElement->reactions));
  naInitList(&(uiElement->shortcuts));
  uiElement->mouseInside = NA_FALSE;
  uiElement->allowNotifications = NA_TRUE;
  
  naAddListLastMutable(&(na_App->uiElements), uiElement);
}



NA_HDEF void na_ClearUIElement(NA_UIElement* uiElement){
  naForeachListMutable(&(uiElement->reactions), naFree);
  naForeachListMutable(&(uiElement->shortcuts), naFree);
  naClearList(&(uiElement->reactions));
  naClearList(&(uiElement->shortcuts));
  
  na_ClearUINativePtr(uiElement->nativePtr);

  naRemoveListData(&(na_App->uiElements), uiElement);
}



NA_HDEF void na_AddApplicationWindow(NAWindow* window){
  naAddListLastMutable(&(na_App->windows), window);
}



NA_HDEF void na_RemoveApplicationWindow(NAWindow* window){
  naRemoveListData(&(na_App->windows), window);
}



NA_HDEF void na_InitApplication(NAApplication* application, NANativePtr nativePtr){
  na_App = application;

  naInitList(&(application->windows));
  naInitList(&(application->uiElements));

  application->translator = NA_NULL;
  naStartTranslator();
  
  application->systemFont = naNewFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_DEFAULT);

  application->mouseStatus.pos = naMakePos(0, 0);
  application->mouseStatus.prevPos = naMakePos(0, 0);
  
  application->curKeyStroke.keyCode = NA_KEYCODE_ESC;
  application->curKeyStroke.modifiers = 0;
  
  application->flags = 0;
  application->flags |= NA_APPLICATION_FLAG_RUNNING;
  application->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;

  application->name = NA_NULL;
  application->companyName = NA_NULL;
  application->versionString = NA_NULL;
  application->buildString = NA_NULL;
  application->iconPath = NA_NULL;

  // This is done at the very end of the InitApplication function as the
  // application must be fully functional before it can init any UIElements.
  na_InitUIElement(&(application->uiElement), NA_UI_APPLICATION, nativePtr);
}

NA_HDEF void na_ClearApplication(NAApplication* application){
  #if NA_DEBUG
    if(!na_App)
      naCrash("No Application running");
  #endif

  naForeachListMutable(&(na_App->windows), (NAMutator)naDelete);
  naClearList(&(na_App->windows));

  naStopTranslator();
  na_ClearUIElement(&(application->uiElement));

  naRelease(application->systemFont);

  // This must be at the very end as the uiElements are used up until the last
  // ClearUIElement operation.
  // todo test if all uiElements are gone.
  naClearList(&(na_App->uiElements));
}



NA_HDEF void na_InitButton(NAButton* button, void* nativePtr, const NAUIImage* uiImage, uint32 flags){
  na_InitUIElement(&(button->uiElement), NA_UI_BUTTON, nativePtr);
  if(uiImage){
    button->uiImage = naRetainConst(uiImage);
  }else{
    button->uiImage = NA_NULL;
  }
  button->flags = flags;
}
NA_HDEF void na_ClearButton(NAButton* button){
  if(button->uiImage){
    naReleaseConst(button->uiImage);
  }
  na_ClearUIElement(&(button->uiElement));
}
NA_HDEF void na_setButtonImage(NAButton* button, const NAUIImage* uiImage){
//  #if NA_DEBUG
//    if(!uiImage)
//      naError("uiImage is null");
//  #endif
  if(button->uiImage){
    naReleaseConst(button->uiImage);
  }
  if(uiImage)
  {
    button->uiImage = naRetainConst(uiImage);
  }else{
    button->uiImage = NA_NULL;
  }
}



NA_HDEF void na_InitCheckBox(NACheckBox* checkBox, void* nativePtr){
  na_InitUIElement(&(checkBox->uiElement), NA_UI_CHECKBOX, nativePtr);
}
NA_HDEF void na_ClearCheckBox(NACheckBox* checkBox){
  na_ClearUIElement(&(checkBox->uiElement));
}



NA_HDEF void na_InitImageSpace(NAImageSpace* imageSpace, void* nativePtr){
  na_InitUIElement(&(imageSpace->uiElement), NA_UI_IMAGE_SPACE, nativePtr);
}
NA_HDEF void na_ClearImageSpace(NAImageSpace* imageSpace){
  na_ClearUIElement(&(imageSpace->uiElement));
}



NA_HDEF void na_InitLabel(NALabel* label, void* nativePtr){
  na_InitUIElement(&(label->uiElement), NA_UI_LABEL, nativePtr);
}
NA_HDEF void na_ClearLabel(NALabel* label){
  na_ClearUIElement(&(label->uiElement));
}



NA_HDEF void na_InitMenu(NAMenu* menu, void* nativePtr, NA_UIElement* parent){
  na_InitUIElement(&(menu->uiElement), NA_UI_MENU, nativePtr);
  naInitList(&(menu->childs));
  na_SetUIElementParent(&(menu->uiElement), parent, NA_FALSE);
}
NA_HDEF void na_ClearMenu(NAMenu* menu){
  naForeachListMutable(&(menu->childs), (NAMutator)naDelete);
  naClearList(&(menu->childs));
//  na_ClearUIElement(&(menu->uiElement));
}
NA_HDEF void na_AddMenuChild(NAMenu* menu, NAMenuItem* child, const NAMenuItem* itemAt){
  NAListIterator iter = naMakeListModifier(&(menu->childs));
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  na_SetUIElementParent(&(child->uiElement), menu, NA_FALSE);
}



NA_HDEF void na_InitMenuItem(NAMenuItem* menuItem, void* nativePtr, NA_UIElement* parent){
  // Note that the nativePtr in this case is a pointer to the menuItem itself,
  // as there does not exist a native pointer for a MenuItem in WINAPI at all.
  na_InitUIElement(&(menuItem->uiElement), NA_UI_MENUITEM, nativePtr);
  na_SetUIElementParent(&(menuItem->uiElement), parent, NA_FALSE);
}
NA_HDEF void na_ClearMenuItem(NAMenuItem* menuItem){
  na_ClearUIElement(&(menuItem->uiElement));
}
NA_HDEF void na_SetMenuItemId(NAMenuItem* menuItem, uint32 id){
  menuItem->id = id;
}
NA_HDEF uint32 na_GetMenuItemId(const NAMenuItem* menuItem){
  return menuItem->id;
}



NA_HDEF void na_InitMetalSpace(NAMetalSpace* metalSpace, void* nativePtr){
  na_InitUIElement(&(metalSpace->uiElement), NA_UI_METAL_SPACE, nativePtr);
}
NA_HDEF void na_ClearMetalSpace(NAMetalSpace* metalSpace){
  na_ClearUIElement(&(metalSpace->uiElement));
}



NA_HDEF void na_InitOpenGLSpace(NAOpenGLSpace* openGLSpace, void* nativePtr){
  na_InitUIElement(&(openGLSpace->uiElement), NA_UI_OPENGL_SPACE, nativePtr);
}
NA_HDEF void na_ClearOpenGLSpace(NAOpenGLSpace* openGLSpace){
  na_ClearUIElement(&(openGLSpace->uiElement));
}



NA_HDEF void na_InitPopupButton(NAPopupButton* popupButton, void* nativePtr){
  na_InitUIElement(&(popupButton->uiElement), NA_UI_POPUP_BUTTON, nativePtr);
  naInitList(&(popupButton->childs));
}
NA_HDEF void na_ClearPopupButton(NAPopupButton* popupButton){
  naForeachListMutable(&(popupButton->childs), (NAMutator)naDelete);
  naClearList(&(popupButton->childs));
  na_ClearUIElement(&(popupButton->uiElement));
}
NA_HDEF void na_AddPopupButtonChild(NAPopupButton* popupButton, NAMenuItem* child, const NAMenuItem* itemAt){
  NAListIterator iter = naMakeListModifier(&(popupButton->childs));
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  //naAddListLastMutable(&(menu->childs), child);
  na_SetUIElementParent(&(child->uiElement), popupButton, NA_FALSE);
}



NA_HDEF void na_InitRadio(NARadio* radio, void* nativePtr){
  na_InitUIElement(&(radio->uiElement), NA_UI_RADIO, nativePtr);
}
NA_HDEF void na_ClearRadio(NARadio* radio){
  na_ClearUIElement(&(radio->uiElement));
}



NA_HDEF void na_InitScreen(NAScreen* screen, void* nativePtr){
  na_InitUIElement(&(screen->uiElement), NA_UI_SCREEN, nativePtr);
}
NA_HDEF void na_ClearScreen(NAScreen* screen){
  na_ClearUIElement(&(screen->uiElement));
}



NA_HDEF void na_InitSpace(NASpace* space, void* nativePtr){
  na_InitUIElement(&(space->uiElement), NA_UI_SPACE, nativePtr);
  naInitList(&(space->childs));
}
NA_HDEF void na_ClearSpace(NASpace* space){
  naForeachListMutable(&(space->childs), (NAMutator)naDelete);
  naClearList(&(space->childs));
  na_ClearUIElement(&(space->uiElement));
}
NA_HDEF void na_AddSpaceChild(NASpace* space, NA_UIElement* child){
  naAddListLastMutable(&(space->childs), child);
  na_SetUIElementParent(child, space, NA_TRUE);
}
NA_HDEF void na_RemoveSpaceChild(NASpace* space, NA_UIElement* child){
  naRemoveListData(&(space->childs), child);
  na_SetUIElementParent(child, NA_NULL, NA_TRUE);
}
NA_DEF NABool naGetSpaceAlternateBackground(NASpace* space){
  return space->alternateBackground;
}


NA_HDEF void na_InitSlider(NASlider* slider, void* nativePtr){
  na_InitUIElement(&(slider->uiElement), NA_UI_SLIDER, nativePtr);
  slider->staticValue = 0.;
  slider->sliderInMovement = NA_FALSE;
}
NA_HDEF void na_ClearSlider(NASlider* slider){
  na_ClearUIElement(&(slider->uiElement));
}



NA_HDEF void na_InitTextBox(NATextBox* textBox, void* nativePtr){
  na_InitUIElement(&(textBox->uiElement), NA_UI_TEXTBOX, nativePtr);
}
NA_HDEF void na_ClearTextBox(NATextBox* textBox){
  na_ClearUIElement(&(textBox->uiElement));
}



NA_HDEF void na_InitTextField(NATextField* textField, void* nativePtr){
  na_InitUIElement(&(textField->uiElement), NA_UI_TEXTFIELD, nativePtr);
}
NA_HDEF void na_ClearTextField(NATextField* textField){
  na_ClearUIElement(&(textField->uiElement));
}



NA_HDEF void na_InitWindow(NAWindow* window, void* nativePtr, NASpace* contentSpace, NABool fullScreen, NABool resizeable, NARect windowedFrame){
  na_InitUIElement(&(window->uiElement), NA_UI_WINDOW, nativePtr);
  na_AddApplicationWindow(window);
  window->contentSpace = contentSpace;
  window->coreFlags = NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS;
  if(fullScreen){window->coreFlags |= NA_CORE_WINDOW_FLAG_FULLSCREEN;}
  if(resizeable){window->coreFlags |= NA_CORE_WINDOW_FLAG_RESIZEABLE;}
  window->windowedFrame = windowedFrame;
}

NA_HDEF void na_ClearWindow(NAWindow* window){
  na_RemoveApplicationWindow(window);
  if(window->contentSpace){naDelete(window->contentSpace);}
  na_ClearUIElement(&(window->uiElement));
}

NA_DEF void naPreventWindowFromClosing(NAWindow* window, NABool prevent){
  #if NA_DEBUG
    if(!naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE))
      naError("This function is only allowed during a \"CLOSES\" event");
  #endif
  naSetFlagu32(&(window->coreFlags), NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, prevent);
}

NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  return naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_FULLSCREEN);
}

NA_DEF NABool naIsWindowResizeable(const NAWindow* window){
  return naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_RESIZEABLE);
}

NA_DEF NASpace* naGetWindowContentSpace(NAWindow* window){
  return window->contentSpace;
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



NA_HDEF void na_SetMouseWarpedTo(NAPos newpos){
  na_App->mouseStatus.prevPos = newpos;
  na_App->mouseStatus.pos = newpos;
}


NA_HDEF void na_SetMouseMovedByDiff(double deltaX, double deltaY){
  na_App->mouseStatus.prevPos = na_App->mouseStatus.pos;
  na_App->mouseStatus.pos.x += deltaX;
  na_App->mouseStatus.pos.y += deltaY;
}


NA_HDEF void na_SetMouseEnteredAtPos(NAPos newpos){
  na_App->mouseStatus.prevPos = newpos;
  na_App->mouseStatus.pos = newpos;
}


NA_HDEF void na_SetMouseExitedAtPos(NAPos newpos){
  na_App->mouseStatus.prevPos = na_App->mouseStatus.pos;
  na_App->mouseStatus.pos = newpos;
}



NA_DEF NAFont* naGetSystemFont(){
  return na_App->systemFont;
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



NA_DEF void naRefreshUIElement(void* uiElement, double timediff){
  //if(timediff == 0.){
  //  na_RefreshUIElementNow(uiElement);
  //}else{
    naCallApplicationFunctionInSeconds(na_RefreshUIElementNow, (NA_UIElement*)uiElement, timediff);
  //}
}



NA_HDEF NABool na_IsApplicationRunning(void){
  return (NABool)(na_App->flags & NA_APPLICATION_FLAG_RUNNING);
}



NA_HDEF NARect na_GetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



// //////////////////////////////////////
// Public functions

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



NA_DEF void naStopApplication(void){
  na_App->flags &= ~NA_APPLICATION_FLAG_RUNNING;
}



NA_DEF NAApplication* naGetApplication(void){
  #if NA_DEBUG
    if(!na_App)
      naError("Internal error: application is not in ui elements list");
  #endif
  return na_App;
}



NA_DEF void* naGetUIElementParent(void* uiElement){
  return ((NA_UIElement*)uiElement)->parent;
}



NA_DEF const void* naGetUIElementParentConst(const void* uiElement){
  return ((const NA_UIElement*)uiElement)->parent;
}



NA_DEF NAWindow* naGetUIElementWindow(void* uiElement){
  void* curelement = uiElement;
  while(curelement && naGetUIElementType(curelement) != NA_UI_WINDOW){
    curelement = naGetUIElementParent(curelement);
  }
  return curelement;
}



NA_DEF const NAWindow* naGetUIElementWindowConst(const void* uiElement){
  const void* curelement = uiElement;
  while(curelement && naGetUIElementType(curelement) != NA_UI_WINDOW){
    curelement = naGetUIElementParentConst(curelement);
  }
  return curelement;
}



NA_DEF NASpace* naGetUIElementParentSpace(void* uiElement){
  NASpace* parent = naGetUIElementParent(uiElement);
  while(parent && naGetUIElementType(parent) != NA_UI_SPACE){
    parent = naGetUIElementParent(parent);
  }
  return parent;
}



NA_DEF const NAMouseStatus* naGetMouseStatus(){
  return &(na_App->mouseStatus);
}



NA_DEF NAPos naGetMousePos(const NAMouseStatus* mousestatus){
  return mousestatus->pos;
}



NA_DEF NASize naGetMouseDelta(const NAMouseStatus* mousestatus){
  return naMakeSizeE(mousestatus->pos.x - mousestatus->prevPos.x, mousestatus->pos.y - mousestatus->prevPos.y);
}



NA_DEF NAKeyStroke naGetCurrentKeyStroke(){
  return na_App->curKeyStroke;
}



NA_DEF NARect naSetWindowStorageTag(NAWindow* window, NAInt storageTag, NARect rect, NABool resizeable){
  window->storageTag = storageTag;
  if(window->storageTag){
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, (int)window->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, (int)window->storageTag);
    rect.pos.x = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    rect.pos.y = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(resizeable){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, (int)window->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, (int)window->storageTag);
      rect.size.width = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
      rect.size.height = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
      naDelete(prefSizeWidthString);
      naDelete(prefSizeHeightString);
    }
  }
  return rect;
}



NA_HDEF void na_RememberWindowPosition(const NAWindow* window){
  if(window->storageTag){
    NARect rect = na_GetWindowAbsoluteInnerRect(&(window->uiElement));
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, (int)window->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, (int)window->storageTag);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(naIsWindowResizeable(window)){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, (int)window->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, (int)window->storageTag);
      naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
      naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
      naDelete(prefSizeWidthString);
      naDelete(prefSizeHeightString);
    }
  }
}

#endif // (NA_COMPILE_GUI == 1)



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
