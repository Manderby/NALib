
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NAUICore.h"

#if (NA_CONFIG_COMPILE_GUI == 1)

#include "../NAMemory.h"
#include "../NACoord.h"
#include "../NAThreading.h"


// The core element storing the app if any.
NACoreApplication* na_app = NA_NULL;









// ///////////////////////////////////
// APPLICATION
// ///////////////////////////////////

NA_HDEF void naStartCoreApplication(NAInt bytesize, NANativeID nativeID){
  // This function is called as the first thing in the naStartApplication
  // functions implemented dependent on the system compiled for.
  #ifndef NDEBUG
    if(na_app)
      naError("Application already running");
  #endif

  na_app = (NACoreApplication*)naMalloc(bytesize);

  na_app->translator = NA_NULL;
  naStartTranslator();
  
  naInitList(&(na_app->uielements));
  na_app->flags = 0;
  na_app->flags |= NA_APPLICATION_FLAG_RUNNING;
  na_app->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;

  // After this function, the calling function shall add the NAApplication as the
  // first entry in the list of ui elements.
  naRegisterCoreUIElement(&(na_app->uielement), NA_UI_APPLICATION, nativeID);
}



NA_HDEF void naStopCoreApplication(void){
  na_app->flags &= ~NA_APPLICATION_FLAG_RUNNING;
  while(naGetListCount(&(na_app->uielements))){
    naReleaseUIElement(naGetListFirstMutable(&(na_app->uielements)));
  }
//  naForeachListMutable(&(na_app->uielements), naReleaseUIElement);
  naClearList(&(na_app->uielements));
  naStopTranslator();
}



NA_HDEF void naClearCoreApplication(){
//  NAListIterator iter;
  #ifndef NDEBUG
    if(!na_app)
      naCrash("No Application running");
  #endif

//  naBeginListMutatorIteration(NAUIElement* elem, &(na_app->uielements), iter);
//    naCloseWindow(na_ui->windows[i]);
//    naClearWindow(na_ui->windows[i]);
//  naEndListIteration(iter);

  naClearList(&(na_app->uielements));
  naFree(na_app);
}



NA_HDEF NACoreApplication* naGetCoreApplication(void){
  #ifndef NDEBUG
    if(naGetListFirstMutable(&(na_app->uielements)) != na_app)
      naError("Internal error: application is not in ui elements list");
  #endif
  return na_app;
}



NA_HDEF NABool naIsCoreApplicationRunning(void){
  return (na_app->flags & NA_APPLICATION_FLAG_RUNNING);
}





// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////

NA_HDEF void naRegisterCoreUIElement(NACoreUIElement* coreuielement, NAUIElementType elementtype, void* nativeID){
  naInitRefCount(&(coreuielement->refcount));
  coreuielement->parent = NA_NULL;
  coreuielement->elementtype = elementtype;
  coreuielement->nativeID = nativeID;
  naInitList(&(coreuielement->reactions));
  naAddListLastMutable(&(na_app->uielements), coreuielement);
}



NA_HDEF void* naUnregisterCoreUIElement(NACoreUIElement* coreuielement){
  naRemoveListData(&(na_app->uielements), coreuielement);
  return coreuielement->nativeID;
}



NA_HDEF NAUIElementType naGetCoreUIElementType(NACoreUIElement* coreuielement){
  return coreuielement->elementtype;
}



NA_HDEF NANativeID naGetCoreUIElementNativeID(NACoreUIElement* coreuielement){
  return coreuielement->nativeID;
}



NA_HDEF void naRefreshCoreUIElement(NACoreUIElement* coreuielement, double timediff){
  //if(timediff == 0.){
  //  naRefreshUIElementNow(coreuielement);
  //}else{
    naCallApplicationFunctionInSeconds(naRefreshUIElementNow, coreuielement, timediff);
  //}
}



NA_HDEF NACoreUIElement* naGetCoreUIElementParent(NACoreUIElement* coreuielement){
  return coreuielement->parent;
}



NA_HDEF NACoreWindow* naGetCoreUIElementWindow(NACoreUIElement* coreuielement){
  NACoreWindow* elementwindow;
  if(coreuielement->elementtype == NA_UI_APPLICATION){
    elementwindow = NA_NULL;
  }else if(coreuielement->elementtype == NA_UI_SCREEN){
    elementwindow = NA_NULL;
  }else if(coreuielement->elementtype == NA_UI_WINDOW){
    elementwindow = (NACoreWindow*)coreuielement;
  }else{
    elementwindow = naGetCoreUIElementWindow(naGetUIElementParent(coreuielement));
  }
  return elementwindow;
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




















NA_DEF void naReleaseUIElement(NAUIElement* uielement){
//  NAListIterator iter;
  NACoreUIElement* element = (NACoreUIElement*)uielement;

  naForeachListMutable(&(element->reactions), naFree);
  naClearList(&(element->reactions));

  switch(naGetUIElementType(element))
  {
  case NA_UI_APPLICATION: naReleaseRefCount(&(element->refcount), uielement, naDestructApplication); break;
//  case NA_UI_SCREEN:      naDeleteScreen(uielement);
  case NA_UI_WINDOW:      naReleaseRefCount(&(element->refcount), uielement, naDestructWindow); break;
  case NA_UI_SPACE:       naReleaseRefCount(&(element->refcount), uielement, naDestructSpace); break;
  #if NA_CONFIG_COMPILE_OPENGL == 1
    case NA_UI_OPENGLSPACE: naReleaseRefCount(&(element->refcount), uielement, naDestructOpenGLSpace); break;
  #endif
  case NA_UI_BUTTON:      naReleaseRefCount(&(element->refcount), uielement, naDestructButton); break;
  case NA_UI_RADIOBUTTON: naReleaseRefCount(&(element->refcount), uielement, naDestructRadioButton); break;
  case NA_UI_LABEL:       naReleaseRefCount(&(element->refcount), uielement, naDestructLabel); break;
  default:
    #ifndef NDEBUG
      naError("Invalid element type");
    #endif
    break;
  }
}





NA_DEF void naAddUIReaction(void* controller, NAUIElement* uielement, NAUICommand command, NAReactionHandler handler){
  NAReaction* newreaction;
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
    if((command == NA_UI_COMMAND_MOUSE_ENTERED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("Only windows can receyve MOUSE_ENTERED commands.");
    if((command == NA_UI_COMMAND_MOUSE_EXITED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("Only windows can receyve MOUSE_EXITED commands.");
    if((command == NA_UI_COMMAND_PRESSED) && (naGetUIElementType(uielement) != NA_UI_BUTTON) && (naGetUIElementType(uielement) != NA_UI_RADIOBUTTON))
      naError("Only buttons and radiobuttons can receyve PRESSED commands.");
  #endif
  newreaction = naAlloc(NAReaction);
  newreaction->controller = controller;
  newreaction->handler = handler;
  newreaction->command = command;
  if(command ==  NA_UI_COMMAND_MOUSE_MOVED){naRetainWindowMouseTracking(naGetUIElementWindow(uielement));}
  naAddListLastMutable(&((element)->reactions), newreaction);
}






NA_DEF NABool naDispatchUIElementCommand(NACoreUIElement* element, NAUICommand command, void* arg){
  NABool finished = NA_FALSE;
  NAListIterator iter;

  naBeginListMutatorIteration(NAReaction* curreaction, &(element->reactions), iter);
    if(curreaction->command == command){
      finished = curreaction->handler(curreaction->controller, (NAUIElement*)element, command, arg);
      // If the handler tells us to stop handling the command, we do so.
      if(finished){break;}
    }
  naEndListIteration(iter);

  // If the command has not been finished, search for other reactions in the parent elements.
  if(!finished){
    NACoreUIElement* parentelement = (NACoreUIElement*)naGetUIElementParent((NAUIElement*)element);
    if(parentelement){finished = naDispatchUIElementCommand(parentelement, command, arg);}
  }

  return finished;
}








NA_DEF const NACursorInfo* naGetMouseInfo(){
  return &(na_app->mouse);
}



NA_DEF NAPos naGetCursorPos(const NACursorInfo* cursorinfo){
  return cursorinfo->pos;
}


NA_DEF NASize naGetCursorDelta(const NACursorInfo* cursorinfo){
  return naMakeSizeE(cursorinfo->pos.x - cursorinfo->prevpos.x, cursorinfo->pos.y - cursorinfo->prevpos.y);
}




NA_DEF void naSetMouseWarpedTo(NAPos newpos){
  na_app->mouse.prevpos = newpos;
  na_app->mouse.pos = newpos;
}


NA_DEF void naSetMouseMovedByDiff(double deltaX, double deltaY){
  na_app->mouse.prevpos = na_app->mouse.pos;
  na_app->mouse.pos.x += deltaX;
  na_app->mouse.pos.y += deltaY;
}


NA_DEF void naSetMouseEnteredAtPos(NAPos newpos){
  na_app->mouse.prevpos = newpos;
  na_app->mouse.pos = newpos;
}


NA_DEF void naSetMouseExitedAtPos(NAPos newpos){
  na_app->mouse.prevpos = na_app->mouse.pos;
  na_app->mouse.pos = newpos;
}







#endif // (NA_CONFIG_COMPILE_GUI == 1)




// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
