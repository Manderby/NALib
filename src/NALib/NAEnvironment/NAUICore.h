
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

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

#if (NA_CONFIG_COMPILE_GUI == 1)

#include "../NAList.h"
#include "../NATranslator.h"

// Very much the same as the native ID, there are certain types which are
// casted differently on the different systems and therefore they are
// declared with a global void* or integer big enough to encapsulate all
// possible casts on all systems. Internally, they are caseted to these
// types:

typedef struct NACoreUIElement      NACoreUIElement;

typedef struct NACoreApplication    NACoreApplication;
typedef struct NACoreScreen         NACoreScreen;
typedef struct NACoreWindow         NACoreWindow;
typedef struct NACoreSpace          NACoreSpace;
typedef struct NACoreImageSpace     NACoreImageSpace;
typedef struct NACoreOpenGLSpace    NACoreOpenGLSpace;
typedef struct NACoreButton         NACoreButton;
typedef struct NACoreRadio          NACoreRadio;
typedef struct NACoreCheckBox       NACoreCheckBox;
typedef struct NACoreLabel          NACoreLabel;
typedef struct NACoreTextField      NACoreTextField;
typedef struct NACoreTextBox        NACoreTextBox;

// NACoreUIElement is the base type of any ui element. All ui element struct
// definitions have an NACoreUIElement as the first entry:

struct NACoreUIElement{
  NARefCount refcount;
  NAUIElementType elementtype;
  NACoreUIElement* parent;
  NAList reactions;
  NAList shortcuts;
  void* nativeID;               // The native object
};

struct NACoreApplication{
  NACoreUIElement uielement;
  NAList          uielements;   // A list of all ui elements.
  NATranslator*   translator;
  NACursorInfo    mouse;        // The mouse cursor info
  NAInt           flags;
};

struct NACoreScreen{
  NACoreUIElement uielement;
};

struct NACoreWindow{
  NACoreUIElement uielement;
  NACoreSpace* contentspace;
  NABool fullscreen;
  NARect windowedframe;
};

struct NACoreSpace{
  NACoreUIElement uielement;
  NABool alternatebackground;
};

struct NACoreImageSpace{
  NACoreUIElement uielement;
};

struct NACoreOpenGLSpace{
  NACoreUIElement uielement;
};

struct NACoreButton{
  NACoreUIElement uielement;
};

struct NACoreRadio{
  NACoreUIElement uielement;
};

struct NACoreCheckBox{
  NACoreUIElement uielement;
};

struct NACoreLabel{
  NACoreUIElement uielement;
};

struct NACoreTextField{
  NACoreUIElement uielement;
};

struct NACoreTextBox{
  NACoreUIElement uielement;
};



NA_HAPI void naRegisterCoreUIElement( NACoreUIElement* coreuielement, NAUIElementType elementtype, void* nativeID);
NA_HAPI void naUnregisterCoreUIElement(NACoreUIElement* coreuielement);

NA_HAPI void naInitCoreApplication(NACoreApplication* coreapplication, void* nativeId);
NA_HAPI void naClearCoreApplication(NACoreApplication* coreapplication);

NA_HAPI void naInitCoreScreen(NACoreScreen* corescreen, void* nativeId);
NA_HAPI void naClearCoreScreen(NACoreScreen* corescreen);

NA_HAPI void naInitCoreWindow(NACoreWindow* corewindow, void* nativeId, NACoreSpace* contentspace, NABool fullscreen, NARect windowedframe);
NA_HAPI void naClearCoreWindow(NACoreWindow* corewindow);

NA_HAPI void naInitCoreSpace(NACoreSpace* corespace, void* nativeId);
NA_HAPI void naClearCoreSpace(NACoreSpace* corecorespace);

NA_HAPI void naInitCoreImageSpace(NACoreImageSpace* coreimagespace, void* nativeId);
NA_HAPI void naClearCoreImageSpace(NACoreImageSpace* corecoreimagespace);

NA_HAPI void naInitCoreOpenGLSpace(NACoreOpenGLSpace* coreopenglspace, void* nativeId);
NA_HAPI void naClearCoreOpenGLSpace(NACoreOpenGLSpace* corecoreopenglspace);

NA_HAPI void naInitCoreButton(NACoreButton* corebutton, void* nativeId);
NA_HAPI void naClearCoreButton(NACoreButton* corebutton);

NA_HAPI void naInitCoreRadio(NACoreRadio* coreradio, void* nativeId);
NA_HAPI void naClearCoreRadio(NACoreRadio* coreradio);

NA_HAPI void naInitCoreCheckBox(NACoreCheckBox* corecheckbox, void* nativeId);
NA_HAPI void naClearCoreCheckBox(NACoreCheckBox* corecheckbox);

NA_HAPI void naInitCoreLabel(NACoreLabel* corelabel, void* nativeId);
NA_HAPI void naClearCoreLabel(NACoreLabel* corelabel);

NA_HAPI void naInitCoreTextField(NACoreTextField* coretextfield, void* nativeId);
NA_HAPI void naClearCoreTextField(NACoreTextField* coretextfield);

NA_HAPI void naInitCoreTextBox(NACoreTextBox* coretextbox, void* nativeId);
NA_HAPI void naClearCoreTextBox(NACoreTextBox* coretextbox);







extern NACoreApplication* na_app;



typedef struct NAReaction NAReaction;
struct NAReaction{
  void* controller;
  NAUICommand command;
  NAReactionHandler handler;
};

typedef struct NAKeyboardShortcutReaction NAKeyboardShortcutReaction;
struct NAKeyboardShortcutReaction{
  void* controller;
  NAKeyboardShortcut shortcut;
  NAReactionHandler handler;
};

#define NA_APPLICATION_FLAG_RUNNING         0x01
#define NA_APPLICATION_FLAG_MOUSE_VISIBLE   0x02












// //////////////////////////////////////////
// Hidden API

// Additional function prototypes and types used internally in NALib.
// Normally, these would be hidden in a separate .h file but in NALib, they are
// gathered here. You are free to use them but note that these are supposed to
// be helper functions.

#include "../NAList.h"



NA_HAPI void naRefreshUIElementNow(NAUIElement* uielement);

NA_API void naSetUIElementParent(NAUIElement* uielement, NAUIElement* parent);


NA_HAPI void naRetainWindowMouseTracking(NAWindow* window);




NA_HAPI NABool naIsCoreApplicationRunning(void);

// Returns a pointer to the ui element which uses the given native ID.
// Every gui element which is handeled by NALib uses a native struct which is
// dependent on the system running. When handling events, a native ID is sent
// but this native ID can in general not be mapped directly to a corresponding
// NALib struct. This function solves that. Slow, but does the job.
NA_HAPI void* naGetUINALibEquivalent(void* nativeID);

NA_HAPI void naClearUINativeId(NANativeID nativeId);

// Dispatches a command with the given uielement and arg.
// As long as the command has not been finished using NA_TRUE as a return value
// in the NAReactionHandler function handler, it will be bubbling upwards in
// the following order:
// - First responder
// - containing space
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



#endif // (NA_CONFIG_COMPILE_GUI == 1)


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_CORE_API_INCLUDED



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


