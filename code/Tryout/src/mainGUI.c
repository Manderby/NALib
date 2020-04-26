
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// To be able to include header files from NALib, you need to either add the
// src directory of NALib as an additional project source path, or you can
// help yourself using the following two lines. Just provide the correct
// relative path to the root directory of the NALib source:

#define GUITRYOUT_STRINGIFY(A) #A
#define GUITRYOUT_NALIB_PATH(file) GUITRYOUT_STRINGIFY(../../NALib/src/file)

// This allows you to reference any NALib file using for example this:
// #include GUITRYOUT_NALIB_PATH(NABase.h)


#include GUITRYOUT_NALIB_PATH(NAUI.h)
#include GUITRYOUT_NALIB_PATH(NATranslator.h)
#include "translationIDs.h"

NAInt trFinanceGroup;
NAInt trGeneralGroup;

NABool reshapeWindow(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Reshape" NA_NL);
  return NA_TRUE;
}

NABool cubPressWindowKey(NAReaction reaction){
  NAKeyboardStatus keyStatus;
  NAUIKeyCode key;

  NA_UNUSED(reaction);
  keyStatus = naGetKeyboardStatus();
  key = keyStatus.keyCode;
  if(key == NA_KEYCODE_ESC){naStopApplication();}
  printf("Key Press" NA_NL);
  return NA_TRUE;
}

NABool cubReleaseWindowKey(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Key Release" NA_NL);
  return NA_TRUE;
}

NABool cubMoveWindowMouse(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse Move" NA_NL);
  return NA_TRUE;
}

NABool cubEnterWindowMouse(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse Enter" NA_NL);
  return NA_TRUE;
}

NABool cubExitWindowMouse(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse exit" NA_NL);
  return NA_TRUE;
}

void prestartup(void* arg){
  NA_UNUSED(arg);
  trFinanceGroup = naRegisterTranslatorGroup();
  #include "../res/translationsFinance.txt"
  trGeneralGroup = naRegisterTranslatorGroup();
  #include "../res/translationsGeneral.txt"
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
  printf ("%s" NA_NL, naTranslate(trGeneralGroup, TR_HELLO_WORLD));
  printf ("%s" NA_NL, naTranslate(trGeneralGroup, TR_99_BEER));
  printf ("%s" NA_NL, naTranslate(trGeneralGroup, TR_BREADCRUMBS_BEAVERSPIT));
  printf ("%s" NA_NL, naTranslate(trFinanceGroup, TR_PROFIT));
  printf ("%s" NA_NL, naTranslate(trFinanceGroup, TR_LOSS));

//  printf("%d" NA_NL, (int)naGetLanguageCode("deu"));

  windowrect = naMakeRectS(20, 20, 400, 300);
  window = naNewWindow("Wurst", windowrect, NA_TRUE, 0);
  naAddUIReaction(window, NA_UI_COMMAND_RESHAPE,       reshapeWindow, NA_NULL);
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


#include <stdio.h>

int main(void){
  printf("NALib Version: %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug ");
  #else
    printf("(Release ");
  #endif
  printf("%d Bits Addresses, %d Bits Integers)" NA_NL, NA_SYSTEM_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();
    naStartApplication(prestartup, poststartup, NA_NULL);
  naStopRuntime();

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished." NA_NL);
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

