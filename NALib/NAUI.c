
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAUIHiddenAPI.h"

#include "NAMemory.h"



#include "NACoord.h"
#include "NAThreading.h"




typedef struct NAReaction NAReaction;
struct NAReaction{
  void* controller;
  NAReactionHandler handler;
  NAUICommand command;
};


struct NACursorInfo{
  NAPos pos;
  NAPos prevpos;
};



typedef struct NAUI NAUI;
struct NAUI{
  NAList uielements;   // A list of all elements.
  NACursorInfo    mouse; // The mouse cursor info
};






struct NAWindow{
  NAUIElement uielement;
  NABool fullscreen;
  NARect windowedframe;
  NASize size;
  NABounds4 bounds;
};


NAUI* na_ui = NA_NULL;



void* naGetUINativeID(void* uielement){
  NAUIElement* element = (NAUIElement*)uielement;
  return element->nativeID;
}



NA_HDEF void* naGetUINALibEquivalent(void* nativeID){
  NAUIElement* curelement;
  naFirstList(&(na_ui->uielements));
  while((curelement = naIterateListMutable(&(na_ui->uielements), 1))){
    if(curelement->nativeID == nativeID){return curelement;}
  }
  return NA_NULL;
}




NA_DEF NAApplication* naGetApplication(void){
  return naGetListFirstMutable(&(na_ui->uielements));
}




NA_DEF void naInitBareUI(void){
  // This function is called as the first thing in one of the naInitUI functions
  // dependent on the system compiled for.
  #ifndef NDEBUG
    if(na_ui)
      naError("naInitBareUI", "UI already running");
  #endif
  na_ui = naAlloc(NAUI);
  naInitList(&(na_ui->uielements));
  // After this function, the calling function shall add the NAApplication as the
  // first entry in the list of ui elements.
}




NA_DEF void naClearUI(void){
  NAUIElement* curelement;
  #ifndef NDEBUG
    if(!na_ui)
      naError("naClearUI", "No UI running");
  #endif
  naFirstList(&(na_ui->uielements));
  while((curelement = naIterateListMutable(&(na_ui->uielements), 1))){
//    naCloseWindow(na_ui->windows[i]);
//    naClearWindow(na_ui->windows[i]);
  }
  naClearList(&(na_ui->uielements));
  naFree(na_ui);
}







//@interface NANativeWindow : NSWindow <NSWindowDelegate>{
//  NAWindow* nalibwindow;
//}
//@end
//
//@implementation NANativeWindow
//- (id) initWithNALibWindow:(NAWindow*)nawindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
//  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
//  nalibwindow = nawindow;
//  return self;
//}
//- (NAWindow*) nalibwindow{return nalibwindow;}
//@end






NA_DEF void naRefreshUIElement(void* uielement, double timediff){
//  NAUIElement* element = (NAUIElement*)uielement;
  //if(!element->refreshrequested){
  //  element->refreshrequested = NA_TRUE;
    naCallFunctionInSeconds(naRefreshUIElementNow, uielement, timediff);
  //}
}



NA_DEF void naRemoveUIElement(NAUIElement* element){
  NABool found = naLocateListPointer(&(na_ui->uielements), element);
  if(found){
    naRemoveListCurrentMutable(&(na_ui->uielements), NA_FALSE);
  }else{
    #ifndef NDEBUG
      naError("naRemoveUIElement", "Element not found in UI");
    #endif
  }
}



NA_HDEF void naAddUIElement(void* element){
  naAddListLastMutable(&(na_ui->uielements), element);
}



NA_HDEF NAUIElement* naInitUIElement(NAUIElement* uielement, NAUIElement* parent, NAUIElementType elementtype, void* nativeID){
  NAUIElement* element = (NAUIElement*)uielement;
  element->parent = parent;
  element->elementtype = elementtype;
  element->nativeID = nativeID;
  //element->refreshrequested = NA_FALSE;
  naInitList(&(element->childs));
  naInitList(&(element->reactions));
  naAddUIElement(element);
  return element;
}



NA_DEF void naClearUIElement(NAUIElement* element){
  NAReaction* curreaction;
  naFirstList(&(element->reactions));
  while((curreaction = naIterateListMutable(&(element->reactions), 1))){
    naFree(curreaction);
  }
  naClearList(&(element->reactions));
}



NA_DEF void naAddUIReaction(void* controller, void* uielement, NAUICommand command, NAReactionHandler handler){
  NAReaction* newreaction;
  #ifndef NDEBUG
    if((command == NA_UI_COMMAND_RESHAPE) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("naAddUIReaction", "Only windows can receyve RESHAPE commands.");
    if((command == NA_UI_COMMAND_KEYDOWN) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("naAddUIReaction", "Only windows can receyve KEYDOWN commands.");
    if((command == NA_UI_COMMAND_KEYUP) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("naAddUIReaction", "Only windows can receyve KEYUP commands.");
    if((command == NA_UI_COMMAND_MOUSE_MOVED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("naAddUIReaction", "Only windows can receyve MOUSE_MOVED commands.");
    if((command == NA_UI_COMMAND_MOUSE_ENTERED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("naAddUIReaction", "Only windows can receyve MOUSE_ENTERED commands.");
    if((command == NA_UI_COMMAND_MOUSE_EXITED) && (naGetUIElementType(uielement) != NA_UI_WINDOW))
      naError("naAddUIReaction", "Only windows can receyve MOUSE_EXITED commands.");
  #endif
  newreaction = naAlloc(NAReaction);
  newreaction->controller = controller;
  newreaction->handler = handler;
  newreaction->command = command;
  if(command ==  NA_UI_COMMAND_MOUSE_MOVED){naRetainWindowMouseTracking(naGetUIElementWindow(uielement));}
  naAddListLastMutable(&(((NAUIElement*)uielement)->reactions), newreaction);
}






NA_DEF NABool naDispatchUIElementCommand(void* uielement, NAUICommand command, void* arg){
  NAUIElement* element = (NAUIElement*)uielement;
  NAReaction* curreaction;
  NABool finished = NA_FALSE;

  naFirstList(&(element->reactions));
  while((curreaction = naIterateListMutable(&(element->reactions), 1))){
    if(curreaction->command == command){
      finished = curreaction->handler(curreaction->controller, element, command, arg);
      // If the handler tells us to stop handling the command, we do so.
      if(finished){break;}
    }
  }

  // If the command has not been finished, search for other reactions in the parent elements.
  if(!finished){
    NAUIElement* parentelement = naGetUIElementParent(element);
    if(parentelement){finished = naDispatchUIElementCommand(parentelement, command, arg);}
  }

  return finished;
}



//NA_DEF NAScreen* naGetUIElementScreen(void* uielement){
//  NAUIElement* element = (NAUIElement*)uielement;
//  if(element->elementtype == NA_UI_APPLICATION){return NA_NULL;}
//  if(element->elementtype == NA_UI_SCREEN){return (NAScreen*)uielement;}
//  return naGetUIElementScreen(naGetUIElementScreen(uielement));
//}


NA_DEF NAWindow* naGetUIElementWindow(void* uielement){
  NAUIElement* element = (NAUIElement*)uielement;
  if(element->elementtype == NA_UI_APPLICATION){return NA_NULL;}
  if(element->elementtype == NA_UI_SCREEN){return NA_NULL;}
  if(element->elementtype == NA_UI_WINDOW){return (NAWindow*)uielement;}
  return naGetUIElementWindow(naGetUIElementParent(uielement));
}



NA_DEF void* naGetUIElementParent(void* uielement){
  NAUIElement* element = (NAUIElement*)uielement;
  return element->parent;
}



NA_DEF NAUIElementType naGetUIElementType(void* uielement){
  NAUIElement* element = (NAUIElement*)uielement;
  return element->elementtype;
}



NA_DEF const NACursorInfo* naGetMouseInfo(){
  return &(na_ui->mouse);
}



NA_DEF NAPos naGetCursorPos(const NACursorInfo* cursorinfo){
  return cursorinfo->pos;
}


NA_DEF NASize naGetCursorDelta(const NACursorInfo* cursorinfo){
  return naMakeSizeE(cursorinfo->pos.x - cursorinfo->prevpos.x, cursorinfo->pos.y - cursorinfo->prevpos.y);
}




NA_DEF void naSetMouseWarpedTo(NAPos newpos){
  na_ui->mouse.prevpos = newpos;
  na_ui->mouse.pos = newpos;
}


NA_DEF void naSetMouseMovedByDiff(double deltaX, double deltaY){
  na_ui->mouse.prevpos = na_ui->mouse.pos;
  na_ui->mouse.pos.x += deltaX;
  na_ui->mouse.pos.y += deltaY;
}


NA_DEF void naSetMouseEnteredAtPos(NAPos newpos){
  na_ui->mouse.prevpos = newpos;
  na_ui->mouse.pos = newpos;
}


NA_DEF void naSetMouseExitedAtPos(NAPos newpos){
  na_ui->mouse.prevpos = na_ui->mouse.pos;
  na_ui->mouse.pos = newpos;
}











// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
