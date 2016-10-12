
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_UI_CORE_API_INCLUDED
#define NA_UI_CORE_API_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// ///////////////////////////
// This file contains API which should be hidden to the programmer but which
// is shared among the different implementations of the UI in Mac and Win.
// ///////////////////////////


#include "../NAUI.h"
#include "../NAList.h"


// Very much the same as the native ID, there are certain types which are
// casted differently on the different systems and therefore they are
// declared with a global void* or integer big enough to encapsulate all
// possible casts on all systems.


typedef struct NAReaction           NAReaction;
typedef struct NACoreUIElement      NACoreUIElement;
typedef struct NACoreApplication    NACoreApplication;
typedef struct NACoreWindow         NACoreWindow;
typedef struct NACoreOpenGLView     NACoreOpenGLView;


// Performs all necessary initialization of the UI independent of the system.
// The naInitUI functions are system dependent and will call this function
// before doing anything else.
NA_HAPI void naStartCoreApplication(NAInt bytesize, NANativeID nativeID);
NA_HAPI void naStopCoreApplication();
NA_HAPI void naClearCoreApplication(void);
NA_HAPI NACoreApplication* naGetCoreApplication(void);
NA_HAPI NABool naIsCoreApplicationRunning(void);


// UIElements
NA_HAPI void naRegisterCoreUIElement( NACoreUIElement* coreuielement,
                                      NACoreUIElement* parent,
                                       NAUIElementType elementtype,
                                                 void* nativeID);
NA_HAPI void naUnregisterCoreUIElement(NACoreUIElement* coreuielement);
NA_HAPI NAUIElementType naGetCoreUIElementType(NACoreUIElement* coreuielement);
NA_HAPI NANativeID naGetCoreUIElementNativeID(NACoreUIElement* coreuielement);
NA_HAPI void naRefreshCoreUIElement(  NACoreUIElement* coreuielement,
                                                double timediff);
NA_HAPI NACoreUIElement* naGetCoreUIElementParent(NACoreUIElement* coreuielement);
NA_HAPI NACoreWindow* naGetCoreUIElementWindow(NACoreUIElement* coreuielement);

NA_HAPI void naRefreshUIElementNow(NAUIElement* uielement);









struct NAReaction{
  void* controller;
  NAReactionHandler handler;
  NAUICommand command;
};


// The base type of any ui element. All ui element struct definitions have
// an NACoreUIElement as the first entry.
// The definition of NACoreUIElement should normally be hidden to the user. So,
// there exists a public NAUIElement type which
// is just a typedef of a void*. But internally, every UI element has the
// following struct as its base:
struct NACoreUIElement{
  NAUIElementType elementtype;
  NACoreUIElement* parent;
  NAList childs;
  NAList reactions;
  void* nativeID;  // The native object
};


struct NACoreApplication{
  NACoreUIElement uielement;
  NAList uielements;   // A list of all ui elements.
  NACursorInfo    mouse; // The mouse cursor info
  NAInt           flags;
};


#define NA_APPLICATION_FLAG_RUNNING         0x01
#define NA_APPLICATION_FLAG_MOUSE_VISIBLE   0x02


//struct NAWindow{
//  NACoreUIElement uielement;
//  NABool fullscreen;
//  NARect windowedframe;
//  NASize size;
//  NABounds4 bounds;
//};


struct NAScreen{
  NACoreUIElement uielement;
};



struct NACoreOpenGLView{
  NACoreUIElement uielement;
};



struct NACoreWindow{
  NACoreUIElement uielement;
  NABool fullscreen;
  NARect windowedframe;
};









// //////////////////////////////////////////
// Hidden API

// Additional function prototypes and types used internally in NALib.
// Normally, these would be hidden in a separate .h file but in NALib, they are
// gathered here. You are free to use them but note that these are supposed to
// be helper functions.

#include "../NAList.h"






NA_HAPI void naRetainWindowMouseTracking(NAWindow* window);





// Returns a pointer to the gui element which uses the given native ID.
// Every gui element which is handeled by NALib uses a native struct which is
// dependent on the system running. When handling events, a native ID is sent
// but this native ID can in general not be mapped directly to a corresponding
// NALib struct. This function solves that. Slow, but does the job.
NA_HAPI void* naGetUINALibEquivalent(void* nativeID);

// Dispatches a command with the given uielement and arg.
// As long as the command has not been finished using NA_TRUE as a return value
// in the NAReactionHandler function handler, it will be bubbling upwards in
// the following order:
// - First responder
// - containing view
// - window
// - application
// - discard command as unhandled.
// The function will return NA_TRUE if the event shall not be processed any
// further. If this function returns NA_FALSE, the event shall still be
// processed by the calling function. This is especially important on Windows
// where non-handling of certain events might interrupt the whole messaging
// chain.
NA_HAPI NABool naDispatchUIElementCommand(  NACoreUIElement* element,
                                    NAUICommand command,
                                   void* arg);




NA_HAPI void naSetMouseWarpedTo(NAPos newpos);
NA_HAPI void naSetMouseMovedByDiff(double deltaX, double deltaY);
NA_HAPI void naSetMouseEnteredAtPos(NAPos newpos);
NA_HAPI void naSetMouseExitedAtPos(NAPos newpos);




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_UI_CORE_API_INCLUDED



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


