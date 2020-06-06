
#include "NAUICore.h"

#if (NA_COMPILE_GUI == 1)

#include "../NAMemory.h"
#include "../NACoord.h"
#include "../NAThreading.h"
#include "../NAPreferences.h"


// The core element storing the app if any.
NA_Application* na_App = NA_NULL;



NA_HDEF void naRegisterCoreUIElement(NA_UIElement* coreUiElement, NAUIElementType elementType, NANativeID nativeID){
  naInitRefCount(&(coreUiElement->refCount));
  coreUiElement->parent = NA_NULL;
  coreUiElement->elementType = elementType;
  coreUiElement->nativeID = nativeID;
  naInitList(&(coreUiElement->reactions));
  naInitList(&(coreUiElement->shortcuts));
  coreUiElement->mouseInside = NA_FALSE;
  coreUiElement->allowNotifications = NA_TRUE;

  naAddListLastMutable(&(na_App->uiElements), coreUiElement);
}



NA_HDEF void na_UnregisterCoreUIElement(NA_UIElement* coreUiElement){
  naRemoveListData(&(na_App->uiElements), coreUiElement);
  na_ClearUINativeId(coreUiElement->nativeID);
}



NA_HDEF void na_InitCoreApplication(NA_Application* coreApplication, NANativeID nativeId){
  na_App = coreApplication;

  naInitList(&(coreApplication->uiElements));

  coreApplication->translator = NA_NULL;
  naStartTranslator();
  
  coreApplication->mouseStatus.pos = naMakePos(0, 0);
  coreApplication->mouseStatus.prevPos = naMakePos(0, 0);
  
  coreApplication->keyboardStatus.keyCode = NA_KEYCODE_ESC;
  coreApplication->keyboardStatus.modifiers = 0;
  
  coreApplication->flags = 0;
  coreApplication->flags |= NA_APPLICATION_FLAG_RUNNING;
  coreApplication->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;

  coreApplication->name = NA_NULL;
  coreApplication->companyName = NA_NULL;
  coreApplication->versionString = NA_NULL;
  coreApplication->buildString = NA_NULL;
  coreApplication->iconPath = NA_NULL;

  naRegisterCoreUIElement(&(coreApplication->uiElement), NA_UI_APPLICATION, nativeId);
}



NA_HDEF void na_ClearCoreApplication(NA_Application* coreApplication){
  NA_UNUSED(coreApplication);
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
  na_UnregisterCoreUIElement(&(coreApplication->uiElement));
  naClearList(&(na_App->uiElements));
}



NA_HDEF void na_InitCoreScreen(NA_Screen* corescreen, void* nativeId){
  naRegisterCoreUIElement(&(corescreen->uiElement), NA_UI_SCREEN, nativeId);
}
NA_HDEF void na_ClearCoreScreen(NA_Screen* corescreen){
  na_UnregisterCoreUIElement(&(corescreen->uiElement));
}



NA_HDEF void na_InitCoreWindow(NA_Window* coreWindow, void* nativeId, NA_Space* contentspace, NABool fullscreen, NABool resizeable, NARect windowedframe){
  naRegisterCoreUIElement(&(coreWindow->uiElement), NA_UI_WINDOW, nativeId);
  coreWindow->contentspace = contentspace;
  coreWindow->flags = 0;
  if(fullscreen){coreWindow->flags |= NA_CORE_WINDOW_FLAG_FULLSCREEN;}
  if(resizeable){coreWindow->flags |= NA_CORE_WINDOW_FLAG_RESIZEABLE;}
  coreWindow->windowedframe = windowedframe;
}

NA_HDEF void na_ClearCoreWindow(NA_Window* coreWindow){
  na_UnregisterCoreUIElement(&(coreWindow->uiElement));
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



NA_HDEF void na_InitCoreSpace(NA_Space* coreespace, void* nativeId){
  naRegisterCoreUIElement(&(coreespace->uiElement), NA_UI_SPACE, nativeId);
}
NA_HDEF void na_ClearCoreSpace(NA_Space* coreespace){
  na_UnregisterCoreUIElement(&(coreespace->uiElement));
}
NA_DEF NABool naGetSpaceAlternateBackground(NASpace* space){
  NA_Space* corespace = (NA_Space*)space;
  return corespace->alternatebackground;
}



NA_HDEF void na_InitCoreImageSpace(NA_ImageSpace* coreImageSpace, void* nativeId){
  naRegisterCoreUIElement(&(coreImageSpace->uiElement), NA_UI_IMAGESPACE, nativeId);
}
NA_HDEF void na_ClearCoreImageSpace(NA_ImageSpace* coreImageSpace){
  na_UnregisterCoreUIElement(&(coreImageSpace->uiElement));
}



NA_HDEF void na_InitCoreOpenGLSpace(NA_OpenGLSpace* coreOpenGLspace, void* nativeId){
  naRegisterCoreUIElement(&(coreOpenGLspace->uiElement), NA_UI_OPENGLSPACE, nativeId);
}
NA_HDEF void na_ClearCoreOpenGLSpace(NA_OpenGLSpace* coreOpenGLspace){
  na_UnregisterCoreUIElement(&(coreOpenGLspace->uiElement));
}



NA_HDEF void na_InitCoreButton(NA_Button* coreButton, void* nativeId){
  naRegisterCoreUIElement(&(coreButton->uiElement), NA_UI_BUTTON, nativeId);
}
NA_HDEF void na_ClearCoreButton(NA_Button* coreButton){
  na_UnregisterCoreUIElement(&(coreButton->uiElement));
}



NA_HDEF void na_InitCoreRadio(NA_Radio* coreRadio, void* nativeId){
  naRegisterCoreUIElement(&(coreRadio->uiElement), NA_UI_RADIO, nativeId);
}
NA_HDEF void na_ClearCoreRadio(NA_Radio* coreRadio){
  na_UnregisterCoreUIElement(&(coreRadio->uiElement));
}



NA_HDEF void na_InitCoreCheckBox(NA_CheckBox* coreCheckBox, void* nativeId){
  naRegisterCoreUIElement(&(coreCheckBox->uiElement), NA_UI_CHECKBOX, nativeId);
}
NA_HDEF void na_ClearCoreCheckBox(NA_CheckBox* coreCheckBox){
  na_UnregisterCoreUIElement(&(coreCheckBox->uiElement));
}



NA_HDEF void na_InitCoreLabel(NA_Label* coreLabel, void* nativeId){
  naRegisterCoreUIElement(&(coreLabel->uiElement), NA_UI_LABEL, nativeId);
}
NA_HDEF void na_ClearCoreLabel(NA_Label* coreLabel){
  na_UnregisterCoreUIElement(&(coreLabel->uiElement));
}



NA_HDEF void na_InitCoreTextField(NA_TextField* coreTextField, void* nativeId){
  naRegisterCoreUIElement(&(coreTextField->uiElement), NA_UI_TEXTFIELD, nativeId);
}
NA_HDEF void na_ClearCoreTextField(NA_TextField* coreTextField){
  na_UnregisterCoreUIElement(&(coreTextField->uiElement));
}



NA_HDEF void na_InitCoreTextBox(NA_TextBox* coreTextBox, void* nativeId){
  naRegisterCoreUIElement(&(coreTextBox->uiElement), NA_UI_TEXTBOX, nativeId);
}
NA_HDEF void na_ClearCoreTextBox(NA_TextBox* coreTextBox){
  na_UnregisterCoreUIElement(&(coreTextBox->uiElement));
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



NA_HDEF NAUIElementType naGetUIElementType(NAUIElement* coreUiElement){
  return ((NA_UIElement*)coreUiElement)->elementType;
}



NA_HDEF NANativeID naGetUIElementNativeID(NAUIElement* coreUiElement){
  return ((NA_UIElement*)coreUiElement)->nativeID;
}



NA_HDEF void naRefreshUIElement(NAUIElement* coreUiElement, double timediff){
  //if(timediff == 0.){
  //  na_RefreshUIElementNow(coreUiElement);
  //}else{
    naCallApplicationFunctionInSeconds(na_RefreshUIElementNow, (NA_UIElement*)coreUiElement, timediff);
  //}
}



NA_HDEF NABool na_IsCoreApplicationRunning(void){
  return (NABool)(na_App->flags & NA_APPLICATION_FLAG_RUNNING);
}




// //////////////////////////////////////
// Public functions



// Prototypes for the naReleaseUIElement frunction.
NA_API void naDestructApplication(NAApplication* application);
NA_API void naDestructWindow(NAWindow* window);
NA_API void naDestructSpace(NASpace* space);
NA_API void naDestructImageSpace(NAImageSpace* imageSpace);
NA_API void naDestructOpenGLSpace(NAOpenGLSpace* space);
NA_API void naDestructButton(NAButton* button);
NA_API void naDestructRadio(NARadio* radio);
NA_API void naDestructCheckBox(NACheckBox* checkBox);
NA_API void naDestructLabel(NALabel* label);
NA_API void naDestructTextField(NATextField* textField);
NA_API void naDestructTextBox(NATextBox* textBox);



NA_DEF void naReleaseUIElement(NAUIElement* uiElement){
  NA_UIElement* element = (NA_UIElement*)uiElement;

  naForeachListMutable(&(element->reactions), naFree);
  naForeachListMutable(&(element->shortcuts), naFree);
  naClearList(&(element->reactions));
  naClearList(&(element->shortcuts));
  element->mouseInside = NA_FALSE;

  switch(naGetUIElementType(element))
  {
  case NA_UI_APPLICATION: naReleaseRefCount(&element->refCount, uiElement, naDestructApplication); break;
//  case NA_UI_SCREEN:      naDeleteScreen(uiElement);
  case NA_UI_WINDOW:      naReleaseRefCount(&element->refCount, uiElement, naDestructWindow); break;
  case NA_UI_SPACE:       naReleaseRefCount(&element->refCount, uiElement, naDestructSpace); break;
  case NA_UI_IMAGESPACE:  naReleaseRefCount(&element->refCount, uiElement, naDestructImageSpace); break;
  #if NA_COMPILE_OPENGL == 1
    case NA_UI_OPENGLSPACE: naReleaseRefCount(&(element->refCount), uiElement, naDestructOpenGLSpace); break;
  #endif
  case NA_UI_BUTTON:      naReleaseRefCount(&element->refCount, uiElement, naDestructButton); break;
  case NA_UI_RADIO:       naReleaseRefCount(&element->refCount, uiElement, naDestructRadio); break;
  case NA_UI_CHECKBOX:    naReleaseRefCount(&element->refCount, uiElement, naDestructCheckBox); break;
  case NA_UI_LABEL:       naReleaseRefCount(&element->refCount, uiElement, naDestructLabel); break;
  case NA_UI_TEXTFIELD:   naReleaseRefCount(&element->refCount, uiElement, naDestructTextField); break;
  case NA_UI_TEXTBOX:     naReleaseRefCount(&element->refCount, uiElement, naDestructTextBox); break;
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



NA_HDEF void naStopApplication(void){
  na_App->flags &= ~NA_APPLICATION_FLAG_RUNNING;
}



NA_HDEF NAApplication* naGetApplication(void){
  #ifndef NDEBUG
    if(naGetListFirstMutable(&(na_App->uiElements)) != na_App)
      naError("Internal error: application is not in ui elements list");
  #endif
  return na_App;
}



NA_HDEF NAUIElement* naGetUIElementParent(NAUIElement* uiElement){
  return ((NA_UIElement*)uiElement)->parent;
}



NA_HDEF NAWindow* naGetUIElementWindow(NAUIElement* uiElement){
  NAUIElement* curelement = uiElement;
  while(curelement && naGetUIElementType(curelement) != NA_UI_WINDOW){
    curelement = naGetUIElementParent(curelement);
  }
  return curelement;
}



NA_HDEF NASpace* naGetUIElementParentSpace(NAUIElement* uiElement){
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



NA_HDEF NARect naSetWindowStorageTag(NAWindow* window, NAInt storageTag, NARect rect, NABool resizeable){
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



NA_HDEF void na_RememberWindowPosition(NA_Window* coreWindow){
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
