
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAUI.h"

#include "NAMemory.h"



#include "NACoord.h"


typedef struct NAReaction NAReaction;
struct NAReaction{
  void* controller;
  NAReactionHandler handler;
  NAUICommand command;
};


typedef struct NAUI NAUI;
struct NAUI{
  NAList uielements;   // A list of all elements.
};






struct NAWindow{
  NAUIElement uielement;
  NABool fullscreen;
  NARect windowedframe;
//  HDC hDC;    // Device context
//  HGLRC hRC;  // Rendering context
//  NAList elements;
//  Rect previouswinrect;
//  TCHAR fullscreendevicename[CCHDEVICENAME];
//  Rect fullscreenrect;
//  MBController* controller;
//  Rect winrect;
//  Rect viewrect;
//  MBBool erasebackground;
//  MBBool quitonclose;
//  void show();
//  void close();
//  void enableContext();
//  virtual void prepareOpenGL(); // loads all needed stuff for best OpenGL experience
//  virtual void startOpenGLDrawing(); // opens the context
//  virtual void endOpenGLDrawing();  // swaps the buffer
//  virtual void draw();  // default implementation does nothing
//  virtual void resize(); // default implementation stores the position and size
//  virtual void keyDown(int32 key);  // default  does nothing.
//  void setPos(int posx, int posy);
//
//  void setFullscreenRect(WCHAR szDevice[CCHDEVICENAME], Rect rect);
//
//  void addElement(MBUIElement* newelement);
//  void removeElement(MBUIElement* oldelement);
//  MBUIElement* getUIElement(HWND handle);
//  
//  // The background of a window is usually filled with the default windows
//  // background color. Set it to false, if you want to prevent that.
//  MBBool eraseBackground();
//  void setEraseBackground(MBBool erase);
//
//  // The background of a window is usually filled with the default windows
//  // background color. Set it to false, if you want to prevent that.
//  MBBool quitOnClose();
//  void setQuitOnClose(MBBool quit);
};
//
//struct NAOpenGLView{
//  NAUIElement uielement;
//};


NAUI* na_gui = NA_NULL;



NANativeUIID naGetNativeID(void* uielement){
  NAUIElement* element = (NAUIElement*)uielement;
  return element->nativeID;
}



NA_HDEF void* naGetUINALibEquivalent(NANativeUIID nativeID){
  NAUIElement* curelement;
  naFirstList(&(na_gui->uielements));
  while((curelement = naIterateListMutable(&(na_gui->uielements), 1))){
    if(curelement->nativeID == nativeID){return curelement;}
  }
  return NA_NULL;
}




NA_DEF NAApplication* naGetApplication(void){
  return naGetListFirstMutable(&(na_gui->uielements));
}




NA_DEF void naInitBareUI(void){
  // This function is called as the first thing in one of the naInitUI functions
  // dependent on the system compiled for.
  #ifndef NDEBUG
    if(na_gui)
      naError("naInitBareUI", "UI already running");
  #endif
  na_gui = naAlloc(NAUI);
  naInitList(&(na_gui->uielements));
  // After this function, the calling function shall add the NAApplication as the
  // first entry in the list of ui elements.
}




NA_DEF void naClearUI(void){
  NAUIElement* curelement;
  #ifndef NDEBUG
    if(!na_gui)
      naError("naClearUI", "No UI running");
  #endif
  naFirstList(&(na_gui->uielements));
  while((curelement = naIterateListMutable(&(na_gui->uielements), 1))){
//    naCloseWindow(na_gui->windows[i]);
//    naClearWindow(na_gui->windows[i]);
  }
  naClearList(&(na_gui->uielements));
  naFree(na_gui);
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









NA_DEF void naRemoveUIElement(NAUIElement* element){
  NABool found = naLocateListPointer(&(na_gui->uielements), element);
  if(found){
    naRemoveListCurrentMutable(&(na_gui->uielements), NA_FALSE);
  }else{
    #ifndef NDEBUG
      naError("naRemoveUIElement", "Element not found in UI");
    #endif
  }
}



NA_HDEF void naAddUIElement(void* element){
  naAddListLastMutable(&(na_gui->uielements), element);
}



NA_HDEF NAUIElement* naInitUIElement(void* uielement, NAUIElement* parent, NAUIElementType elementtype, NANativeUIID nativeID){
  NAUIElement* element = (NAUIElement*)uielement;
  element->parent = parent;
  element->elementtype = elementtype;
  element->nativeID = nativeID;
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
  #endif
  newreaction = naAlloc(NAReaction);
  newreaction->controller = controller;
  newreaction->handler = handler;
  newreaction->command = command;
  naAddListLastMutable(&(((NAUIElement*)uielement)->reactions), newreaction);
}






NA_DEF NABool naDispatchUIElementCommand(void* uielement, NAUICommand command, NAUIArgument arg){
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



NA_DEF NAWindow* naGetUIElementWindow(void* uielement){
  NAUIElement* element = (NAUIElement*)uielement;
  if(element->elementtype == NA_UI_APPLICATION){return NA_NULL;}
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
