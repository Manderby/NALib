
#include "NAUICore.h"

#if (NA_COMPILE_GUI == 1)

#include "../NAMemory.h"
#include "../NACoord.h"
#include "../NAThreading.h"
#include "../NAPreferences.h"


// The core element storing the app if any.
NA_Application* na_App = NA_NULL;



NA_HHDEF void na_RegisterUIElement(NA_UIElement* uiElement, NAUIElementType elementType, NANativeID nativeID){
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



NA_HHDEF void na_UnregisterUIElement(NA_UIElement* uiElement){
  naRemoveListData(&(na_App->uiElements), uiElement);
  na_ClearUINativeId(uiElement->nativeID);
}



NA_HHDEF void na_InitApplication(NA_Application* application, NANativeID nativeId){
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



NA_HHDEF void na_ClearApplication(NA_Application* application){
  NA_UNUSED(application);
//  NAListIterator iter;
  #ifndef NDEBUG
    if(!na_App)
      naCrash("No Application running");
  #endif

  while(naGetListCount(&(na_App->uiElements)) > 1){
    NAUIElement* uiElement;
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



NA_HHDEF void na_InitScreen(NA_Screen* screen, void* nativeId){
  na_RegisterUIElement(&(screen->uiElement), NA_UI_SCREEN, nativeId);
}
NA_HHDEF void na_ClearScreen(NA_Screen* screen){
  na_UnregisterUIElement(&(screen->uiElement));
}



NA_HHDEF void na_InitWindow(NA_Window* window, void* nativeId, NA_Space* contentspace, NABool fullscreen, NABool resizeable, NARect windowedFrame){
  na_RegisterUIElement(&(window->uiElement), NA_UI_WINDOW, nativeId);
  window->contentspace = contentspace;
  window->flags = 0;
  if(fullscreen){window->flags |= NA_CORE_WINDOW_FLAG_FULLSCREEN;}
  if(resizeable){window->flags |= NA_CORE_WINDOW_FLAG_RESIZEABLE;}
  window->windowedFrame = windowedFrame;
}

NA_HHDEF void na_ClearWindow(NA_Window* window){
  na_UnregisterUIElement(&(window->uiElement));
}

NA_DEF void naPreventWindowFromClosing(NAWindow* window, NABool prevent){
  NA_Window* coreWindow = (NA_Window*)window;
  #ifndef NDEBUG
    if(!naGetFlagi(coreWindow->flags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE))
      naError("This function is only allowed during a \"CLOSES\" event");
  #endif
  naSetFlagi(&(coreWindow->flags), NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, prevent);
}

NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  NA_Window* coreWindow = (NA_Window*)window;
  return naGetFlagi(coreWindow->flags, NA_CORE_WINDOW_FLAG_FULLSCREEN);
}

NA_DEF NABool naIsWindowResizeable(NAWindow* window){
  NA_Window* coreWindow = (NA_Window*)window;
  return naGetFlagi(coreWindow->flags, NA_CORE_WINDOW_FLAG_RESIZEABLE);
}

NA_DEF NASpace* naGetWindowContentSpace(NAWindow* window){
  NA_Window* coreWindow = (NA_Window*)window;
  return coreWindow->contentspace;
}



NA_HHDEF void na_InitSpace(NA_Space* space, void* nativeId){
  na_RegisterUIElement(&(space->uiElement), NA_UI_SPACE, nativeId);
}
NA_HHDEF void na_ClearSpace(NA_Space* space){
  na_UnregisterUIElement(&(space->uiElement));
}
NA_DEF NABool naGetSpaceAlternateBackground(NASpace* space){
  NA_Space* corespace = (NA_Space*)space;
  return corespace->alternatebackground;
}



NA_HHDEF void na_InitImageSpace(NA_ImageSpace* imageSpace, void* nativeId){
  na_RegisterUIElement(&(imageSpace->uiElement), NA_UI_IMAGESPACE, nativeId);
}
NA_HHDEF void na_ClearImageSpace(NA_ImageSpace* imageSpace){
  na_UnregisterUIElement(&(imageSpace->uiElement));
}



NA_HHDEF void na_InitOpenGLSpace(NA_OpenGLSpace* openGLSpace, void* nativeId){
  na_RegisterUIElement(&(openGLSpace->uiElement), NA_UI_OPENGLSPACE, nativeId);
}
NA_HHDEF void na_ClearOpenGLSpace(NA_OpenGLSpace* openGLSpace){
  na_UnregisterUIElement(&(openGLSpace->uiElement));
}



NA_HHDEF void na_InitButton(NA_Button* button, void* nativeId){
  na_RegisterUIElement(&(button->uiElement), NA_UI_BUTTON, nativeId);
}
NA_HHDEF void na_ClearButton(NA_Button* button){
  na_UnregisterUIElement(&(button->uiElement));
}



NA_HHDEF void na_InitRadio(NA_Radio* radio, void* nativeId){
  na_RegisterUIElement(&(radio->uiElement), NA_UI_RADIO, nativeId);
}
NA_HHDEF void na_ClearRadio(NA_Radio* radio){
  na_UnregisterUIElement(&(radio->uiElement));
}



NA_HHDEF void na_InitCheckBox(NA_CheckBox* checkBox, void* nativeId){
  na_RegisterUIElement(&(checkBox->uiElement), NA_UI_CHECKBOX, nativeId);
}
NA_HHDEF void na_ClearCheckBox(NA_CheckBox* checkBox){
  na_UnregisterUIElement(&(checkBox->uiElement));
}



NA_HHDEF void na_InitLabel(NA_Label* label, void* nativeId){
  na_RegisterUIElement(&(label->uiElement), NA_UI_LABEL, nativeId);
}
NA_HHDEF void na_ClearLabel(NA_Label* label){
  na_UnregisterUIElement(&(label->uiElement));
}



NA_HHDEF void na_InitTextField(NA_TextField* textField, void* nativeId){
  na_RegisterUIElement(&(textField->uiElement), NA_UI_TEXTFIELD, nativeId);
}
NA_HHDEF void na_ClearTextField(NA_TextField* textField){
  na_UnregisterUIElement(&(textField->uiElement));
}



NA_HHDEF void na_InitTextBox(NA_TextBox* textBox, void* nativeId){
  na_RegisterUIElement(&(textBox->uiElement), NA_UI_TEXTBOX, nativeId);
}
NA_HHDEF void na_ClearTextBox(NA_TextBox* textBox){
  na_UnregisterUIElement(&(textBox->uiElement));
}



// todo: find a faster way. Hash perhaps or something else.
NA_HHDEF void* na_GetUINALibEquivalent(NANativeID nativeID){
  NAListIterator iter;
  NA_UIElement* retelem = NA_NULL;
  naBeginListMutatorIteration(NA_UIElement* elem, &(na_App->uiElements), iter);
    if(elem->nativeID == nativeID){retelem = elem; break;}
  naEndListIteration(iter);
  return retelem;
}



NA_HHDEF NABool na_DispatchUIElementCommand(NA_UIElement* element, NAUICommand command){
  NABool finished = NA_FALSE;
  NAListIterator iter;

  NAReaction reaction;
  reaction.uiElement = (NAUIElement*)element;
  reaction.command = command;
  naBeginListMutatorIteration(NA_Reaction* coreReaction, &(element->reactions), iter);
    if(coreReaction->command == command){
      reaction.controller = coreReaction->controller;
      finished = coreReaction->handler(reaction);
      // If the handler tells us to stop handling the command, we do so.
      if(finished){break;}
    }
  naEndListIteration(iter);

  // If the command has not been finished, search for other reactions in the parent elements.
  if(!finished && command != NA_UI_COMMAND_MOUSE_ENTERED && command != NA_UI_COMMAND_MOUSE_EXITED){
    NA_UIElement* parentelement = (NA_UIElement*)naGetUIElementParent((NAUIElement*)element);
    if(parentelement){finished = na_DispatchUIElementCommand(parentelement, command);}
  }

  return finished;
}



NA_HHDEF void na_SetMouseWarpedTo(NAPos newpos){
  na_App->mouseStatus.prevPos = newpos;
  na_App->mouseStatus.pos = newpos;
}


NA_HHDEF void na_SetMouseMovedByDiff(double deltaX, double deltaY){
  na_App->mouseStatus.prevPos = na_App->mouseStatus.pos;
  na_App->mouseStatus.pos.x += deltaX;
  na_App->mouseStatus.pos.y += deltaY;
}


NA_HHDEF void na_SetMouseEnteredAtPos(NAPos newpos){
  na_App->mouseStatus.prevPos = newpos;
  na_App->mouseStatus.pos = newpos;
}


NA_HHDEF void na_SetMouseExitedAtPos(NAPos newpos){
  na_App->mouseStatus.prevPos = na_App->mouseStatus.pos;
  na_App->mouseStatus.pos = newpos;
}



NA_DEF NAUIElementType naGetUIElementType(NAUIElement* uiElement){
  return ((NA_UIElement*)uiElement)->elementType;
}



NA_DEF NANativeID naGetUIElementNativeID(NAUIElement* uiElement){
  return ((NA_UIElement*)uiElement)->nativeID;
}



NA_DEF void naRefreshUIElement(NAUIElement* uiElement, double timediff){
  //if(timediff == 0.){
  //  na_RefreshUIElementNow(uiElement);
  //}else{
    naCallApplicationFunctionInSeconds(na_RefreshUIElementNow, (NA_UIElement*)uiElement, timediff);
  //}
}



NA_HHDEF NABool na_IsCoreApplicationRunning(void){
  return (NABool)(na_App->flags & NA_APPLICATION_FLAG_RUNNING);
}




// //////////////////////////////////////
// Public functions



// Prototypes for the naReleaseUIElement frunction.
NA_HHAPI void na_DestructApplication(NAApplication* application);
NA_HHAPI void na_DestructWindow(NAWindow* window);
NA_HHAPI void na_DestructSpace(NASpace* space);
NA_HHAPI void na_DestructImageSpace(NAImageSpace* imageSpace);
NA_HHAPI void na_DestructOpenGLSpace(NAOpenGLSpace* space);
NA_HHAPI void na_DestructButton(NAButton* button);
NA_HHAPI void na_DestructRadio(NARadio* radio);
NA_HHAPI void na_DestructCheckBox(NACheckBox* checkBox);
NA_HHAPI void na_DestructLabel(NALabel* label);
NA_HHAPI void na_DestructTextField(NATextField* textField);
NA_HHAPI void na_DestructTextBox(NATextBox* textBox);



NA_DEF void naReleaseUIElement(NAUIElement* uiElement){
  NA_UIElement* element = (NA_UIElement*)uiElement;

  naForeachListMutable(&(element->reactions), naFree);
  naForeachListMutable(&(element->shortcuts), naFree);
  naClearList(&(element->reactions));
  naClearList(&(element->shortcuts));
  element->mouseInside = NA_FALSE;

  switch(naGetUIElementType(element))
  {
  case NA_UI_APPLICATION: naReleaseRefCount(&element->refCount, uiElement, na_DestructApplication); break;
//  case NA_UI_SCREEN:      naDeleteScreen(uiElement);
  case NA_UI_WINDOW:      naReleaseRefCount(&element->refCount, uiElement, na_DestructWindow); break;
  case NA_UI_SPACE:       naReleaseRefCount(&element->refCount, uiElement, na_DestructSpace); break;
  case NA_UI_IMAGESPACE:  naReleaseRefCount(&element->refCount, uiElement, na_DestructImageSpace); break;
  #if NA_COMPILE_OPENGL == 1
    case NA_UI_OPENGLSPACE: naReleaseRefCount(&(element->refCount), uiElement, na_DestructOpenGLSpace); break;
  #endif
  case NA_UI_BUTTON:      naReleaseRefCount(&element->refCount, uiElement, na_DestructButton); break;
  case NA_UI_RADIO:       naReleaseRefCount(&element->refCount, uiElement, na_DestructRadio); break;
  case NA_UI_CHECKBOX:    naReleaseRefCount(&element->refCount, uiElement, na_DestructCheckBox); break;
  case NA_UI_LABEL:       naReleaseRefCount(&element->refCount, uiElement, na_DestructLabel); break;
  case NA_UI_TEXTFIELD:   naReleaseRefCount(&element->refCount, uiElement, na_DestructTextField); break;
  case NA_UI_TEXTBOX:     naReleaseRefCount(&element->refCount, uiElement, na_DestructTextBox); break;
  default:
    #ifndef NDEBUG
      naError("Invalid element type");
    #endif
    break;
  }
}



NA_DEF void naAddUIReaction(NAUIElement* uiElement, NAUICommand command, NAReactionHandler handler, void* controller){
  NA_Reaction* coreReaction;
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
  coreReaction = naAlloc(NA_Reaction);
  coreReaction->controller = controller;
  coreReaction->command = command;
  coreReaction->handler = handler;
  // todo: this needs some attention on macOS
  //if(command == NA_UI_COMMAND_MOUSE_MOVED || command == NA_UI_COMMAND_MOUSE_ENTERED || command == NA_UI_COMMAND_MOUSE_EXITED){
  //  element->moustrackingcount++;
  //  if(element->moustrackingcount == 1){
  //    element->mousetracking = na_AllocMouseTracking(naGetUIElementNativeID(element));
  //  }
  //}
  naAddListLastMutable(&((element)->reactions), coreReaction);
}



NA_DEF NAKeyboardStatus naMakeKeybardStatus(NAInt modifiers, NAUIKeyCode keyCode){
  NAKeyboardStatus newshtatus;
  newshtatus.modifiers = modifiers;
  newshtatus.keyCode = keyCode;
  return newshtatus;
}



NA_DEF void naAddUIKeyboardShortcut(NAUIElement* uiElement, NAKeyboardStatus shortcut, NAReactionHandler handler, void* controller){
  NA_KeyboardShortcutReaction* corekeyreaction;
  NA_UIElement* element = (NA_UIElement*)uiElement;
  //#ifndef NDEBUG
  //  if((naGetUIElementType(uiElement) != NA_UI_APPLICATION) && (naGetUIElementType(uiElement) != NA_UI_WINDOW))
  //    naError("Currently, only applications and windows are allowed as uiElement. Use naGetApplication() for the app.");
  //#endif
  corekeyreaction = naAlloc(NA_KeyboardShortcutReaction);
  corekeyreaction->controller = controller;
  corekeyreaction->shortcut = shortcut;
  corekeyreaction->handler = handler;
  naAddListLastMutable(&((element)->shortcuts), corekeyreaction);
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



NA_DEF NAUIElement* naGetUIElementParent(NAUIElement* uiElement){
  return ((NA_UIElement*)uiElement)->parent;
}



NA_DEF NAWindow* naGetUIElementWindow(NAUIElement* uiElement){
  NAUIElement* curelement = uiElement;
  while(curelement && naGetUIElementType(curelement) != NA_UI_WINDOW){
    curelement = naGetUIElementParent(curelement);
  }
  return curelement;
}



NA_DEF NASpace* naGetUIElementParentSpace(NAUIElement* uiElement){
  NA_Space* parent = naGetUIElementParent(uiElement);
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
  NA_Window* coreWindow = (NA_Window*)window;
  coreWindow->storageTag = storageTag;
  if(coreWindow->storageTag){
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, (int)coreWindow->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, (int)coreWindow->storageTag);
    rect.pos.x = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    rect.pos.y = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(resizeable){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, (int)coreWindow->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, (int)coreWindow->storageTag);
      rect.size.width = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
      rect.size.height = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
      naDelete(prefSizeWidthString);
      naDelete(prefSizeHeightString);
    }
  }
  return rect;
}



NA_HHDEF void na_RememberWindowPosition(NA_Window* coreWindow){
  if(coreWindow->storageTag){
    NARect rect = na_GetWindowAbsoluteInnerRect(&(coreWindow->uiElement));
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, (int)coreWindow->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, (int)coreWindow->storageTag);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(naIsWindowResizeable(coreWindow)){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, (int)coreWindow->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, (int)coreWindow->storageTag);
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
