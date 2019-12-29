
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//

#include "../NALib/NAUI.h"
#include "../NALib/NATranslator.h"
#include "translationIDs.h"

NAInt trFinanceGroup;
NAInt trGeneralGroup;

NABool resizeWindow(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Resize\n");
  return NA_TRUE;
}

NABool cubPressWindowKey(NAReaction reaction){
  NA_UNUSED(reaction);
  NAKeyboardStatus keyStatus = naGetKeyboardStatus();
  NAUIKeyCode key = keyStatus.keyCode;
  if(key == NA_KEYCODE_ESC){naStopApplication();}
  printf("Key Press\n");
  return NA_TRUE;
}

NABool cubReleaseWindowKey(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Key Release\n");
  return NA_TRUE;
}

NABool cubMoveWindowMouse(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse Move\n");
  return NA_TRUE;
}

NABool cubEnterWindowMouse(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse Enter\n");
  return NA_TRUE;
}

NABool cubExitWindowMouse(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse exit\n");
  return NA_TRUE;
}

void prestartup(void* arg){
  NA_UNUSED(arg);
  trFinanceGroup = naRegisterTranslatorGroup();
  #include "res/translationsFinance.txt"
  trGeneralGroup = naRegisterTranslatorGroup();
  #include "res/translationsGeneral.txt"
}

void initOpenGLSpace(void* initData){
  NA_UNUSED(initData);
}

void poststartup(void* arg){
  NARect windowrect;
  NAWindow* window;
  NASpace* contentSpace;
  
  NAButton* button;
  NACheckBox* checkbox;
  NALabel* label;
  NARadio* radio;
  NATextBox* textbox;
  NATextField* textfield;
  NA_UNUSED(arg);

  naSetTranslatorLanguagePreference(naGetLanguageCode("deu"));
  //naSetTranslatorLanguagePreference(naGetLanguageCode("eng"));
  //naSetTranslatorLanguagePreference(naGetLanguageCode("gsw"));
  printf ("%s\n", naTranslate(trGeneralGroup, TR_HELLO_WORLD));
  printf ("%s\n", naTranslate(trGeneralGroup, TR_99_BEER));
  printf ("%s\n", naTranslate(trGeneralGroup, TR_BREADCRUMBS_BEAVERSPIT));
  printf ("%s\n", naTranslate(trFinanceGroup, TR_PROFIT));
  printf ("%s\n", naTranslate(trFinanceGroup, TR_LOSS));

//  printf("%d\n", (int)naGetLanguageCode("deu"));

  windowrect = naMakeRectS(20, 20, 400, 300);
  window = naNewWindow("Wurst", windowrect, NA_TRUE, 0);
  naAddUIReaction(window, NA_UI_COMMAND_RESHAPE,       resizeWindow, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_KEYDOWN,       cubPressWindowKey, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_KEYUP,         cubReleaseWindowKey, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_MOUSE_MOVED,   cubMoveWindowMouse, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_MOUSE_ENTERED, cubEnterWindowMouse, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_MOUSE_EXITED,  cubExitWindowMouse, NA_NULL);

//  NAOpenGLSpace* openglspace = naNewOpenGLSpace(window, windowrect.size, initOpenGLSpace, window);
//  naSetWindowContentSpace(window, openglspace);

  contentSpace = naGetWindowContentSpace(window);

  button = naNewPushButton("PushButton asdf", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, button, naMakePos(20., 20.));
  checkbox = naNewCheckBox("Checkbox asdf", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, checkbox, naMakePos(20., 40.));
  label = naNewLabel("Label asdf", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, label, naMakePos(20., 60.));
  radio = naNewRadio("Radio asdf", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, radio, naMakePos(20., 80.));
  textbox = naNewTextBox(naMakeSize(200., 40.));
  naAddSpaceChild(contentSpace, textbox, naMakePos(20., 100.));
  textfield = naNewTextField(naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, textfield, naMakePos(20., 140.));

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

