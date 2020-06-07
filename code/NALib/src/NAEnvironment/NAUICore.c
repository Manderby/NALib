
#include "NAUICore.h"

#if (NA_COMPILE_GUI == 1)

#include "../NAMemory.h"
#include "../NACoord.h"
#include "../NAThreading.h"
#include "../NAPreferences.h"


// The pointer storing the app if any.
NAApplication* na_App = NA_NULL;



NA_HDEF void na_RegisterUIElement(NA_UIElement* uiElement, NAUIElementType elementType, NANativeID nativeID){
  naInitRefCount(&(uiElement->refCount));
  uiElement->parent = NA_NULL;
  uiElement->elementType = elementType;
  uiElement->nativeID = nativeID;
  naInitList(&(uiElement->reactions));
  naInitList(&(uiElement->shortcuts));
  uiElement->mouseInside = NA_FALSE;
  uiElement->allowNotifications = NA_TRUE;

  naAddListLastMutable(&(na_App->uiElements), uiElement);
}



NA_HDEF void na_UnregisterUIElement(NA_UIElement* uiElement){
  naRemoveListData(&(na_App->uiElements), uiElement);
  na_ClearUINativeId(uiElement->nativeID);
}



NA_HDEF void na_InitApplication(NAApplication* application, NANativeID nativeId){
  na_App = application;

  naInitList(&(application->uiElements));

  application->translator = NA_NULL;
  naStartTranslator();
  
  application->mouseStatus.pos = naMakePos(0, 0);
  application->mouseStatus.prevPos = naMakePos(0, 0);
  
  application->keyboardStatus.keyCode = NA_KEYCODE_ESC;
  application->keyboardStatus.modifiers = 0;
  
  application->flags = 0;
  application->flags |= NA_APPLICATION_FLAG_RUNNING;
  application->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;

  application->name = NA_NULL;
  application->companyName = NA_NULL;
  application->versionString = NA_NULL;
  application->buildString = NA_NULL;
  application->iconPath = NA_NULL;

  na_RegisterUIElement(&(application->uiElement), NA_UI_APPLICATION, nativeId);
}



NA_HDEF void na_ClearApplication(NAApplication* application){
  #ifndef NDEBUG
    if(!na_App)
      naCrash("No Application running");
  #endif

  while(naGetListCount(&(na_App->uiElements)) > 1){
    void* uiElement;
    NAListIterator iter = naMakeListModifier(&(na_App->uiElements));
    naLocateListFirst(&iter);
    naIterateList(&iter);
    uiElement = naGetListCurMutable(&iter);
    naClearListIterator(&iter);
    naReleaseUIElement(uiElement);
  }
  naStopTranslator();
  na_UnregisterUIElement(&(application->uiElement));
  naClearList(&(na_App->uiElements));
}



NA_HDEF void na_InitScreen(NAScreen* screen, void* nativeId){
  na_RegisterUIElement(&(screen->uiElement), NA_UI_SCREEN, nativeId);
}
NA_HDEF void na_ClearScreen(NAScreen* screen){
  na_UnregisterUIElement(&(screen->uiElement));
}



NA_HDEF void na_InitWindow(NAWindow* window, void* nativeId, NASpace* contentSpace, NABool fullScreen, NABool resizeable, NARect windowedFrame){
  na_RegisterUIElement(&(window->uiElement), NA_UI_WINDOW, nativeId);
  window->contentSpace = contentSpace;
  window->flags = 0;
  if(fullScreen){window->flags |= NA_CORE_WINDOW_FLAG_FULLSCREEN;}
  if(resizeable){window->flags |= NA_CORE_WINDOW_FLAG_RESIZEABLE;}
  window->windowedFrame = windowedFrame;
}

NA_HDEF void na_ClearWindow(NAWindow* window){
  na_UnregisterUIElement(&(window->uiElement));
}

NA_DEF void naPreventWindowFromClosing(NAWindow* window, NABool prevent){
  #ifndef NDEBUG
    if(!naGetFlagi(window->flags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE))
      naError("This function is only allowed during a \"CLOSES\" event");
  #endif
  naSetFlagi(&(window->flags), NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, prevent);
}

NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  return naGetFlagi(window->flags, NA_CORE_WINDOW_FLAG_FULLSCREEN);
}

NA_DEF NABool naIsWindowResizeable(NAWindow* window){
  return naGetFlagi(window->flags, NA_CORE_WINDOW_FLAG_RESIZEABLE);
}

NA_DEF NASpace* naGetWindowContentSpace(NAWindow* window){
  return window->contentSpace;
}



NA_HDEF void na_InitSpace(NASpace* space, void* nativeId){
  na_RegisterUIElement(&(space->uiElement), NA_UI_SPACE, nativeId);
}
NA_HDEF void na_ClearSpace(NASpace* space){
  na_UnregisterUIElement(&(space->uiElement));
}
NA_DEF NABool naGetSpaceAlternateBackground(NASpace* space){
  return space->alternatebackground;
}



NA_HDEF void na_InitImageSpace(NAImageSpace* imageSpace, void* nativeId){
  na_RegisterUIElement(&(imageSpace->uiElement), NA_UI_IMAGESPACE, nativeId);
}
NA_HDEF void na_ClearImageSpace(NAImageSpace* imageSpace){
  na_UnregisterUIElement(&(imageSpace->uiElement));
}



NA_HDEF void na_InitOpenGLSpace(NAOpenGLSpace* openGLSpace, void* nativeId){
  na_RegisterUIElement(&(openGLSpace->uiElement), NA_UI_OPENGLSPACE, nativeId);
}
NA_HDEF void na_ClearOpenGLSpace(NAOpenGLSpace* openGLSpace){
  na_UnregisterUIElement(&(openGLSpace->uiElement));
}



NA_HDEF void na_InitButton(NAButton* button, void* nativeId){
  na_RegisterUIElement(&(button->uiElement), NA_UI_BUTTON, nativeId);
}
NA_HDEF void na_ClearButton(NAButton* button){
  na_UnregisterUIElement(&(button->uiElement));
}



NA_HDEF void na_InitRadio(NARadio* radio, void* nativeId){
  na_RegisterUIElement(&(radio->uiElement), NA_UI_RADIO, nativeId);
}
NA_HDEF void na_ClearRadio(NARadio* radio){
  na_UnregisterUIElement(&(radio->uiElement));
}



NA_HDEF void na_InitCheckBox(NACheckBox* checkBox, void* nativeId){
  na_RegisterUIElement(&(checkBox->uiElement), NA_UI_CHECKBOX, nativeId);
}
NA_HDEF void na_ClearCheckBox(NACheckBox* checkBox){
  na_UnregisterUIElement(&(checkBox->uiElement));
}



NA_HDEF void na_InitLabel(NALabel* label, void* nativeId){
  na_RegisterUIElement(&(label->uiElement), NA_UI_LABEL, nativeId);
}
NA_HDEF void na_ClearLabel(NALabel* label){
  na_UnregisterUIElement(&(label->uiElement));
}



NA_HDEF void na_InitTextField(NATextField* textField, void* nativeId){
  na_RegisterUIElement(&(textField->uiElement), NA_UI_TEXTFIELD, nativeId);
}
NA_HDEF void na_ClearTextField(NATextField* textField){
  na_UnregisterUIElement(&(textField->uiElement));
}



NA_HDEF void na_InitTextBox(NATextBox* textBox, void* nativeId){
  na_RegisterUIElement(&(textBox->uiElement), NA_UI_TEXTBOX, nativeId);
}
NA_HDEF void na_ClearTextBox(NATextBox* textBox){
  na_UnregisterUIElement(&(textBox->uiElement));
}



// todo: find a faster way. Hash perhaps or something else.
NA_HDEF void* na_GetUINALibEquivalent(NANativeID nativeID){
  NAListIterator iter;
  NA_UIElement* retelem = NA_NULL;
  naBeginListMutatorIteration(NA_UIElement* elem, &(na_App->uiElements), iter);
    if(elem->nativeID == nativeID){retelem = elem; break;}
  naEndListIteration(iter);
  return retelem;
}



NA_HDEF NABool na_DispatchUIElementCommand(NA_UIElement* element, NAUICommand command){
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
    NA_UIElement* parentelement = (NA_UIElement*)naGetUIElementParent(element);
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



NA_DEF NAUIElementType naGetUIElementType(void* uiElement){
  return ((NA_UIElement*)uiElement)->elementType;
}



NA_DEF NANativeID naGetUIElementNativeID(void* uiElement){
  return ((NA_UIElement*)uiElement)->nativeID;
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




// //////////////////////////////////////
// Public functions



// Prototypes for the naReleaseUIElement frunction.
NA_HAPI void na_DestructApplication(NAApplication* application);
NA_HAPI void na_DestructWindow(NAWindow* window);
NA_HAPI void na_DestructSpace(NASpace* space);
NA_HAPI void na_DestructImageSpace(NAImageSpace* imageSpace);
NA_HAPI void na_DestructOpenGLSpace(NAOpenGLSpace* space);
NA_HAPI void na_DestructButton(NAButton* button);
NA_HAPI void na_DestructRadio(NARadio* radio);
NA_HAPI void na_DestructCheckBox(NACheckBox* checkBox);
NA_HAPI void na_DestructLabel(NALabel* label);
NA_HAPI void na_DestructTextField(NATextField* textField);
NA_HAPI void na_DestructTextBox(NATextBox* textBox);



NA_DEF void naReleaseUIElement(void* uiElement){
  NA_UIElement* element = (NA_UIElement*)uiElement;

  naForeachListMutable(&(element->reactions), naFree);
  naForeachListMutable(&(element->shortcuts), naFree);
  naClearList(&(element->reactions));
  naClearList(&(element->shortcuts));
  element->mouseInside = NA_FALSE;

  switch(naGetUIElementType(element))
  {
  case NA_UI_APPLICATION: naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructApplication); break;
//  case NA_UI_SCREEN:      naDeleteScreen(uiElement);
  case NA_UI_WINDOW:      naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructWindow); break;
  case NA_UI_SPACE:       naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructSpace); break;
  case NA_UI_IMAGESPACE:  naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructImageSpace); break;
  #if NA_COMPILE_OPENGL == 1
    case NA_UI_OPENGLSPACE: naReleaseRefCount(&(element->refCount), uiElement, (NAMutator)na_DestructOpenGLSpace); break;
  #endif
  case NA_UI_BUTTON:      naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructButton); break;
  case NA_UI_RADIO:       naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructRadio); break;
  case NA_UI_CHECKBOX:    naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructCheckBox); break;
  case NA_UI_LABEL:       naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructLabel); break;
  case NA_UI_TEXTFIELD:   naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructTextField); break;
  case NA_UI_TEXTBOX:     naReleaseRefCount(&element->refCount, uiElement, (NAMutator)na_DestructTextBox); break;
  default:
    #ifndef NDEBUG
      naError("Invalid element type");
    #endif
    break;
  }
}



NA_DEF void naAddUIReaction(void* uiElement, NAUICommand command, NAReactionHandler handler, void* controller){
  NAEventReaction* eventReaction;
  NA_UIElement* element = (NA_UIElement*)uiElement;
  #ifndef NDEBUG
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
      && (naGetUIElementType(uiElement) != NA_UI_RADIO)
      && (naGetUIElementType(uiElement) != NA_UI_CHECKBOX))
      naError("Only buttons, radios and checkBoxes can receyve PRESSED commands.");
    if((command == NA_UI_COMMAND_EDITED) && (naGetUIElementType(uiElement) != NA_UI_TEXTFIELD))
      naError("Only textFields can receyve EDITED commands.");
  #endif
  eventReaction = naAlloc(NAEventReaction);
  eventReaction->controller = controller;
  eventReaction->command = command;
  eventReaction->handler = handler;
  // todo: this needs some attention on macOS
  //if(command == NA_UI_COMMAND_MOUSE_MOVED || command == NA_UI_COMMAND_MOUSE_ENTERED || command == NA_UI_COMMAND_MOUSE_EXITED){
  //  element->moustrackingcount++;
  //  if(element->moustrackingcount == 1){
  //    element->mousetracking = na_AllocMouseTracking(naGetUIElementNativeID(element));
  //  }
  //}
  naAddListLastMutable(&((element)->reactions), eventReaction);
}



NA_DEF NAKeyboardStatus naMakeKeybardStatus(NAInt modifiers, NAUIKeyCode keyCode){
  NAKeyboardStatus newshtatus;
  newshtatus.modifiers = modifiers;
  newshtatus.keyCode = keyCode;
  return newshtatus;
}



NA_DEF void naAddUIKeyboardShortcut(void* uiElement, NAKeyboardStatus shortcut, NAReactionHandler handler, void* controller){
  NAKeyboardShortcutReaction* keyReaction;
  NA_UIElement* element = (NA_UIElement*)uiElement;
  //#ifndef NDEBUG
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
  #ifndef NDEBUG
    if(naGetListFirstMutable(&(na_App->uiElements)) != na_App)
      naError("Internal error: application is not in ui elements list");
  #endif
  return na_App;
}



NA_DEF void* naGetUIElementParent(void* uiElement){
  return ((NA_UIElement*)uiElement)->parent;
}



NA_DEF NAWindow* naGetUIElementWindow(void* uiElement){
  void* curelement = uiElement;
  while(curelement && naGetUIElementType(curelement) != NA_UI_WINDOW){
    curelement = naGetUIElementParent(curelement);
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
  return &(na_App->mouseStatus
  );
}



NA_DEF NAPos naGetMousePos(const NAMouseStatus* mousestatus){
  return mousestatus->pos;
}



NA_DEF NASize naGetMouseDelta(const NAMouseStatus* mousestatus){
  return naMakeSizeE(mousestatus->pos.x - mousestatus->prevPos.x, mousestatus->pos.y - mousestatus->prevPos.y);
}



NA_DEF NAKeyboardStatus naGetKeyboardStatus(){
  return na_App->keyboardStatus;
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



NA_HDEF void na_RememberWindowPosition(NAWindow* window){
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
