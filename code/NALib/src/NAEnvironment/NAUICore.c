
#include "NAUICore.h"

#if (NA_COMPILE_GUI == 1)

#include "../NAMemory.h"
#include "../NACoord.h"
#include "../NAThreading.h"
#include "../NAPreferences.h"


// The core element storing the app if any.
NACoreApplication* na_app = NA_NULL;



NA_HDEF void naRegisterCoreUIElement(NACoreUIElement* coreuielement, NAUIElementType elementtype, NANativeID nativeID){
  naInitRefCount(&(coreuielement->refcount));
  coreuielement->parent = NA_NULL;
  coreuielement->elementtype = elementtype;
  coreuielement->nativeID = nativeID;
  naInitList(&(coreuielement->reactions));
  naInitList(&(coreuielement->shortcuts));
  coreuielement->mouseinside = NA_FALSE;
  coreuielement->allownotifications = NA_TRUE;

  naAddListLastMutable(&(na_app->uielements), coreuielement);
}



NA_HDEF void naUnregisterCoreUIElement(NACoreUIElement* coreuielement){
  naRemoveListData(&(na_app->uielements), coreuielement);
  naClearUINativeId(coreuielement->nativeID);
}



NA_HDEF void naInitCoreApplication(NACoreApplication* coreapplication, NANativeID nativeId){
  na_app = coreapplication;

  naInitList(&(coreapplication->uielements));

  coreapplication->translator = NA_NULL;
  naStartTranslator();
  
  coreapplication->mouseStatus.pos = naMakePos(0, 0);
  coreapplication->mouseStatus.prevpos = naMakePos(0, 0);
  
  coreapplication->keyboardStatus.keyCode = NA_KEYCODE_ESC;
  coreapplication->keyboardStatus.modifiers = 0;
  
  coreapplication->flags = 0;
  coreapplication->flags |= NA_APPLICATION_FLAG_RUNNING;
  coreapplication->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;

  coreapplication->name = NA_NULL;
  coreapplication->companyName = NA_NULL;
  coreapplication->versionString = NA_NULL;
  coreapplication->buildString = NA_NULL;
  coreapplication->iconPath = NA_NULL;

  naRegisterCoreUIElement(&(coreapplication->uielement), NA_UI_APPLICATION, nativeId);
}



NA_HDEF void naClearCoreApplication(NACoreApplication* coreapplication){
  NA_UNUSED(coreapplication);
//  NAListIterator iter;
  #ifndef NDEBUG
    if(!na_app)
      naCrash("No Application running");
  #endif

  while(naGetListCount(&(na_app->uielements)) > 1){
    NAUIElement* uielement;
    NAListIterator iter = naMakeListModifier(&(na_app->uielements));
    naLocateListFirst(&iter);
    naIterateList(&iter);
    uielement = naGetListCurMutable(&iter);
    naClearListIterator(&iter);
    naReleaseUIElement(uielement);
  }
  naStopTranslator();
  naUnregisterCoreUIElement(&(coreapplication->uielement));
  naClearList(&(na_app->uielements));
}



NA_HDEF void naInitCoreScreen(NACoreScreen* corescreen, void* nativeId){
  naRegisterCoreUIElement(&(corescreen->uielement), NA_UI_SCREEN, nativeId);
}
NA_HDEF void naClearCoreScreen(NACoreScreen* corescreen){
  naUnregisterCoreUIElement(&(corescreen->uielement));
}



NA_HDEF void naInitCoreWindow(NACoreWindow* coreWindow, void* nativeId, NACoreSpace* contentspace, NABool fullscreen, NABool resizeable, NARect windowedframe){
  naRegisterCoreUIElement(&(coreWindow->uielement), NA_UI_WINDOW, nativeId);
  coreWindow->contentspace = contentspace;
  coreWindow->flags = 0;
  if(fullscreen){coreWindow->flags |= NA_CORE_WINDOW_FLAG_FULLSCREEN;}
  if(resizeable){coreWindow->flags |= NA_CORE_WINDOW_FLAG_RESIZEABLE;}
  coreWindow->windowedframe = windowedframe;
}

NA_HDEF void naClearCoreWindow(NACoreWindow* coreWindow){
  naUnregisterCoreUIElement(&(coreWindow->uielement));
}

NA_DEF void naPreventWindowFromClosing(NAWindow* window, NABool prevent){
  NACoreWindow* coreWindow = (NACoreWindow*)window;
  #ifndef NDEBUG
    if(!naGetFlagi(coreWindow->flags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE))
      naError("This function is only allowed during a \"CLOSES\" event");
  #endif
  naSetFlagi(&(coreWindow->flags), NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, prevent);
}

NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  NACoreWindow* coreWindow = (NACoreWindow*)window;
  return naGetFlagi(coreWindow->flags, NA_CORE_WINDOW_FLAG_FULLSCREEN);
}

NA_DEF NABool naIsWindowResizeable(NAWindow* window){
  NACoreWindow* coreWindow = (NACoreWindow*)window;
  return naGetFlagi(coreWindow->flags, NA_CORE_WINDOW_FLAG_RESIZEABLE);
}

NA_DEF NASpace* naGetWindowContentSpace(NAWindow* window){
  NACoreWindow* coreWindow = (NACoreWindow*)window;
  return coreWindow->contentspace;
}



NA_HDEF void naInitCoreSpace(NACoreSpace* coreespace, void* nativeId){
  naRegisterCoreUIElement(&(coreespace->uielement), NA_UI_SPACE, nativeId);
}
NA_HDEF void naClearCoreSpace(NACoreSpace* coreespace){
  naUnregisterCoreUIElement(&(coreespace->uielement));
}
NA_DEF NABool naGetSpaceAlternateBackground(NASpace* space){
  NACoreSpace* corespace = (NACoreSpace*)space;
  return corespace->alternatebackground;
}



NA_HDEF void naInitCoreImageSpace(NACoreImageSpace* coreImageSpace, void* nativeId){
  naRegisterCoreUIElement(&(coreImageSpace->uielement), NA_UI_IMAGESPACE, nativeId);
}
NA_HDEF void naClearCoreImageSpace(NACoreImageSpace* coreImageSpace){
  naUnregisterCoreUIElement(&(coreImageSpace->uielement));
}



NA_HDEF void naInitCoreOpenGLSpace(NACoreOpenGLSpace* coreOpenGLspace, void* nativeId){
  naRegisterCoreUIElement(&(coreOpenGLspace->uielement), NA_UI_OPENGLSPACE, nativeId);
}
NA_HDEF void naClearCoreOpenGLSpace(NACoreOpenGLSpace* coreOpenGLspace){
  naUnregisterCoreUIElement(&(coreOpenGLspace->uielement));
}



NA_HDEF void naInitCoreButton(NACoreButton* coreButton, void* nativeId){
  naRegisterCoreUIElement(&(coreButton->uielement), NA_UI_BUTTON, nativeId);
}
NA_HDEF void naClearCoreButton(NACoreButton* coreButton){
  naUnregisterCoreUIElement(&(coreButton->uielement));
}



NA_HDEF void naInitCoreRadio(NACoreRadio* coreRadio, void* nativeId){
  naRegisterCoreUIElement(&(coreRadio->uielement), NA_UI_RADIO, nativeId);
}
NA_HDEF void naClearCoreRadio(NACoreRadio* coreRadio){
  naUnregisterCoreUIElement(&(coreRadio->uielement));
}



NA_HDEF void naInitCoreCheckBox(NACoreCheckBox* coreCheckBox, void* nativeId){
  naRegisterCoreUIElement(&(coreCheckBox->uielement), NA_UI_CHECKBOX, nativeId);
}
NA_HDEF void naClearCoreCheckBox(NACoreCheckBox* coreCheckBox){
  naUnregisterCoreUIElement(&(coreCheckBox->uielement));
}



NA_HDEF void naInitCoreLabel(NACoreLabel* coreLabel, void* nativeId){
  naRegisterCoreUIElement(&(coreLabel->uielement), NA_UI_LABEL, nativeId);
}
NA_HDEF void naClearCoreLabel(NACoreLabel* coreLabel){
  naUnregisterCoreUIElement(&(coreLabel->uielement));
}



NA_HDEF void naInitCoreTextField(NACoreTextField* coreTextField, void* nativeId){
  naRegisterCoreUIElement(&(coreTextField->uielement), NA_UI_TEXTFIELD, nativeId);
}
NA_HDEF void naClearCoreTextField(NACoreTextField* coreTextField){
  naUnregisterCoreUIElement(&(coreTextField->uielement));
}



NA_HDEF void naInitCoreTextBox(NACoreTextBox* coreTextBox, void* nativeId){
  naRegisterCoreUIElement(&(coreTextBox->uielement), NA_UI_TEXTBOX, nativeId);
}
NA_HDEF void naClearCoreTextBox(NACoreTextBox* coreTextBox){
  naUnregisterCoreUIElement(&(coreTextBox->uielement));
}



// todo: find a faster way. Hash perhaps or something else.
NA_HDEF void* naGetUINALibEquivalent(NANativeID nativeID){
  NAListIterator iter;
  NACoreUIElement* retelem = NA_NULL;
  naBeginListMutatorIteration(NACoreUIElement* elem, &(na_app->uielements), iter);
    if(elem->nativeID == nativeID){retelem = elem; break;}
  naEndListIteration(iter);
  return retelem;
}



NA_HDEF NABool naDispatchUIElementCommand(NACoreUIElement* element, NAUICommand command){
  NABool finished = NA_FALSE;
  NAListIterator iter;

  NAReaction reaction;
  reaction.uielement = (NAUIElement*)element;
  reaction.command = command;
  naBeginListMutatorIteration(NACoreReaction* corereaction, &(element->reactions), iter);
    if(corereaction->command == command){
      reaction.controller = corereaction->controller;
      finished = corereaction->handler(reaction);
      // If the handler tells us to stop handling the command, we do so.
      if(finished){break;}
    }
  naEndListIteration(iter);

  // If the command has not been finished, search for other reactions in the parent elements.
  if(!finished && command != NA_UI_COMMAND_MOUSE_ENTERED && command != NA_UI_COMMAND_MOUSE_EXITED){
    NACoreUIElement* parentelement = (NACoreUIElement*)naGetUIElementParent((NAUIElement*)element);
    if(parentelement){finished = naDispatchUIElementCommand(parentelement, command);}
  }

  return finished;
}



NA_HDEF void naSetMouseWarpedTo(NAPos newpos){
  na_app->mouseStatus.prevpos = newpos;
  na_app->mouseStatus.pos = newpos;
}


NA_HDEF void naSetMouseMovedByDiff(double deltaX, double deltaY){
  na_app->mouseStatus.prevpos = na_app->mouseStatus.pos;
  na_app->mouseStatus.pos.x += deltaX;
  na_app->mouseStatus.pos.y += deltaY;
}


NA_HDEF void naSetMouseEnteredAtPos(NAPos newpos){
  na_app->mouseStatus.prevpos = newpos;
  na_app->mouseStatus.pos = newpos;
}


NA_HDEF void naSetMouseExitedAtPos(NAPos newpos){
  na_app->mouseStatus.prevpos = na_app->mouseStatus.pos;
  na_app->mouseStatus.pos = newpos;
}



NA_HDEF NAUIElementType naGetUIElementType(NAUIElement* coreuielement){
  return ((NACoreUIElement*)coreuielement)->elementtype;
}



NA_HDEF NANativeID naGetUIElementNativeID(NAUIElement* coreuielement){
  return ((NACoreUIElement*)coreuielement)->nativeID;
}



NA_HDEF void naRefreshUIElement(NAUIElement* coreuielement, double timediff){
  //if(timediff == 0.){
  //  naRefreshUIElementNow(coreuielement);
  //}else{
    naCallApplicationFunctionInSeconds(naRefreshUIElementNow, (NACoreUIElement*)coreuielement, timediff);
  //}
}



NA_HDEF NABool naIsCoreApplicationRunning(void){
  return (NABool)(na_app->flags & NA_APPLICATION_FLAG_RUNNING);
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



NA_DEF void naReleaseUIElement(NAUIElement* uielement){
  NACoreUIElement* element = (NACoreUIElement*)uielement;

  naForeachListMutable(&(element->reactions), naFree);
  naForeachListMutable(&(element->shortcuts), naFree);
  naClearList(&(element->reactions));
  naClearList(&(element->shortcuts));
  element->mouseinside = NA_FALSE;

  switch(naGetUIElementType(element))
  {
  case NA_UI_APPLICATION: naReleaseRefCount(&element->refcount, uielement, naDestructApplication); break;
//  case NA_UI_SCREEN:      naDeleteScreen(uielement);
  case NA_UI_WINDOW:      naReleaseRefCount(&element->refcount, uielement, naDestructWindow); break;
  case NA_UI_SPACE:       naReleaseRefCount(&element->refcount, uielement, naDestructSpace); break;
  case NA_UI_IMAGESPACE:  naReleaseRefCount(&element->refcount, uielement, naDestructImageSpace); break;
  #if NA_COMPILE_OPENGL == 1
    case NA_UI_OPENGLSPACE: naReleaseRefCount(&(element->refcount), uielement, naDestructOpenGLSpace); break;
  #endif
  case NA_UI_BUTTON:      naReleaseRefCount(&element->refcount, uielement, naDestructButton); break;
  case NA_UI_RADIO:       naReleaseRefCount(&element->refcount, uielement, naDestructRadio); break;
  case NA_UI_CHECKBOX:    naReleaseRefCount(&element->refcount, uielement, naDestructCheckBox); break;
  case NA_UI_LABEL:       naReleaseRefCount(&element->refcount, uielement, naDestructLabel); break;
  case NA_UI_TEXTFIELD:   naReleaseRefCount(&element->refcount, uielement, naDestructTextField); break;
  case NA_UI_TEXTBOX:     naReleaseRefCount(&element->refcount, uielement, naDestructTextBox); break;
  default:
    #ifndef NDEBUG
      naError("Invalid element type");
    #endif
    break;
  }
}



NA_DEF void naAddUIReaction(NAUIElement* uielement, NAUICommand command, NAReactionHandler handler, void* controller){
  NACoreReaction* corereaction;
  NACoreUIElement* element = (NACoreUIElement*)uielement;
  #ifndef NDEBUG
    if((command == NA_UI_COMMAND_RESHAPE) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("Only windows can receyve RESHAPE commands.");
//    if((command == NA_UI_COMMAND_KEYDOWN) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
//      naError("Only windows can receyve KEYDOWN commands.");
//    if((command == NA_UI_COMMAND_KEYUP) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
//      naError("Only windows can receyve KEYUP commands.");
    if((command == NA_UI_COMMAND_MOUSE_MOVED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("Only windows can receyve MOUSE_MOVED commands.");
//    if((command == NA_UI_COMMAND_MOUSE_ENTERED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
//      naError("Only windows can receyve MOUSE_ENTERED commands.");
//    if((command == NA_UI_COMMAND_MOUSE_EXITED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
//      naError("Only windows can receyve MOUSE_EXITED commands.");
    if((command == NA_UI_COMMAND_CLOSES) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("Only windows can receyve CLOSES commands.");
    if((command == NA_UI_COMMAND_PRESSED)
      && (naGetUIElementType(uielement) != NA_UI_BUTTON)
      && (naGetUIElementType(uielement) != NA_UI_RADIO)
      && (naGetUIElementType(uielement) != NA_UI_CHECKBOX))
      naError("Only buttons, radios and checkBoxes can receyve PRESSED commands.");
    if((command == NA_UI_COMMAND_EDITED) && (naGetUIElementType(uielement) != NA_UI_TEXTFIELD))
      naError("Only textFields can receyve EDITED commands.");
  #endif
  corereaction = naAlloc(NACoreReaction);
  corereaction->controller = controller;
  corereaction->command = command;
  corereaction->handler = handler;
  // todo: this needs some attention on macOS
  //if(command == NA_UI_COMMAND_MOUSE_MOVED || command == NA_UI_COMMAND_MOUSE_ENTERED || command == NA_UI_COMMAND_MOUSE_EXITED){
  //  element->moustrackingcount++;
  //  if(element->moustrackingcount == 1){
  //    element->mousetracking = naAllocMouseTracking(naGetUIElementNativeID(element));
  //  }
  //}
  naAddListLastMutable(&((element)->reactions), corereaction);
}



NA_DEF NAKeyboardStatus naMakeKeybardStatus(NAInt modifiers, NAUIKeyCode keyCode){
  NAKeyboardStatus newshtatus;
  newshtatus.modifiers = modifiers;
  newshtatus.keyCode = keyCode;
  return newshtatus;
}



NA_DEF void naAddUIKeyboardShortcut(NAUIElement* uielement, NAKeyboardStatus shortcut, NAReactionHandler handler, void* controller){
  NACoreKeyboardShortcutReaction* corekeyreaction;
  NACoreUIElement* element = (NACoreUIElement*)uielement;
  //#ifndef NDEBUG
  //  if((naGetUIElementType(uielement) != NA_UI_APPLICATION) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
  //    naError("Currently, only applications and windows are allowed as uielement. Use naGetApplication() for the app.");
  //#endif
  corekeyreaction = naAlloc(NACoreKeyboardShortcutReaction);
  corekeyreaction->controller = controller;
  corekeyreaction->shortcut = shortcut;
  corekeyreaction->handler = handler;
  naAddListLastMutable(&((element)->shortcuts), corekeyreaction);
}



NA_HDEF void naStopApplication(void){
  na_app->flags &= ~NA_APPLICATION_FLAG_RUNNING;
}



NA_HDEF NAApplication* naGetApplication(void){
  #ifndef NDEBUG
    if(naGetListFirstMutable(&(na_app->uielements)) != na_app)
      naError("Internal error: application is not in ui elements list");
  #endif
  return na_app;
}



NA_HDEF NAUIElement* naGetUIElementParent(NAUIElement* uielement){
  return ((NACoreUIElement*)uielement)->parent;
}



NA_HDEF NAWindow* naGetUIElementWindow(NAUIElement* uielement){
  NAUIElement* curelement = uielement;
  while(curelement && naGetUIElementType(curelement) != NA_UI_WINDOW){
    curelement = naGetUIElementParent(curelement);
  }
  return curelement;
}



NA_HDEF NASpace* naGetUIElementParentSpace(NAUIElement* uielement){
  NACoreSpace* parent = naGetUIElementParent(uielement);
  while(parent && naGetUIElementType(parent) != NA_UI_SPACE){
    parent = naGetUIElementParent(parent);
  }
  return parent;
}



NA_DEF const NAMouseStatus* naGetMouseStatus(){
  return &(na_app->mouseStatus
  );
}



NA_DEF NAPos naGetMousePos(const NAMouseStatus* mousestatus){
  return mousestatus->pos;
}



NA_DEF NASize naGetMouseDelta(const NAMouseStatus* mousestatus){
  return naMakeSizeE(mousestatus->pos.x - mousestatus->prevpos.x, mousestatus->pos.y - mousestatus->prevpos.y);
}



NA_DEF NAKeyboardStatus naGetKeyboardStatus(){
  return na_app->keyboardStatus;
}



NA_HDEF NARect naSetWindowStorageTag(NAWindow* window, NAInt storageTag, NARect rect, NABool resizeable){
  NACoreWindow* coreWindow = (NACoreWindow*)window;
  coreWindow->storageTag = storageTag;
  if(coreWindow->storageTag){
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, coreWindow->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, coreWindow->storageTag);
    rect.pos.x = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    rect.pos.y = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(resizeable){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, coreWindow->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, coreWindow->storageTag);
      rect.size.width = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
      rect.size.height = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
      naDelete(prefSizeWidthString);
      naDelete(prefSizeHeightString);
    }
  }
  return rect;
}



NA_HDEF void naRememberWindowPosition(NACoreWindow* coreWindow){
  if(coreWindow->storageTag){
    NARect rect = naGetWindowAbsoluteInnerRect(&(coreWindow->uielement));
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, coreWindow->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, coreWindow->storageTag);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(naIsWindowResizeable(coreWindow)){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, coreWindow->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, coreWindow->storageTag);
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
