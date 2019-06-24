
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//

#include "../NALib/NAUI.h"

NABool resizeWindow(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  printf("Resize\n");
  return NA_TRUE;
}

NABool drawWindow(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  printf("Draw\n");
  return NA_TRUE;
}

NABool cubPressWindowKey(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  printf("Key Press\n");
  return NA_TRUE;
}

NABool cubReleaseWindowKey(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  printf("Key Release\n");
  return NA_TRUE;
}

NABool cubMoveWindowMouse(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  printf("Mouse Move\n");
  return NA_TRUE;
}

NABool cubEnterWindowMouse(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  printf("Mouse Enter\n");
  return NA_TRUE;
}

NABool cubExitWindowMouse(void* controllerdata, NAUIElement* uielement, NAUICommand command, void* arg){
  NA_UNUSED(controllerdata);
  NA_UNUSED(uielement);
  NA_UNUSED(command);
  NA_UNUSED(arg);
//  printf("Mouse exit\n");
  return NA_TRUE;
}

void prestartup(void* arg){
  NA_UNUSED(arg);
}

void initOpenGLSpace(void* initData){
  NA_UNUSED(initData);
}

void poststartup(void* arg){
  NA_UNUSED(arg);
  NARect windowrect = naMakeRectS(20, 20, 400, 300);
  NAWindow* window = naNewWindow("Wurst", windowrect, NA_TRUE);
  naAddUIReaction(NA_NULL, window, NA_UI_COMMAND_RESHAPE,       resizeWindow);
  naAddUIReaction(NA_NULL, window, NA_UI_COMMAND_REDRAW,        drawWindow);
  naAddUIReaction(NA_NULL, window, NA_UI_COMMAND_KEYDOWN,       cubPressWindowKey);
  naAddUIReaction(NA_NULL, window, NA_UI_COMMAND_KEYUP,         cubReleaseWindowKey);
  naAddUIReaction(NA_NULL, window, NA_UI_COMMAND_MOUSE_MOVED,   cubMoveWindowMouse);
  naAddUIReaction(NA_NULL, window, NA_UI_COMMAND_MOUSE_ENTERED, cubEnterWindowMouse);
  naAddUIReaction(NA_NULL, window, NA_UI_COMMAND_MOUSE_EXITED,  cubExitWindowMouse);

//  NAOpenGLSpace* openglspace = naNewOpenGLSpace(window, windowrect.size, initOpenGLSpace, window);
//  naSetWindowContentSpace(window, openglspace);

  NAButton* button = naNewButton();
  NASpace* contentSpace = naGetWindowContentSpace(window);
  naAddSpaceChild(contentSpace, button);

  naShowWindow(window);
}


#include "../NALib/NABase.h"
#include "../NALib/NAMemory.h"
#include <stdio.h>

int main(void){
  printf("NALib Version: %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug ");
  #else
    printf("(Release ");
  #endif
  printf("%d Bits Addresses, %d Bits Integers)\n", NA_SYSTEM_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();
    naStartApplication(prestartup, poststartup, NA_NULL);
  naStopRuntime();

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished.\n");
    NA_UNUSED(getchar());
  #endif

  return 0;
}


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

