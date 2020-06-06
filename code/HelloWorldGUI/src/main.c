
// To use the GUI implementation of NALib, you need to configure it. You can
// do this by defining the preprocessor macro NA_COMPILE_GUI to be 1.
//
// You can define NA_COMPILE_GUI either directly in NAConfiguration.h or add
// a preprocessor macro in your project settings. But the preferred ways are:
//
// XCode: Add a preprocessor macro to your solution settings. Add NALib as 
// a target. Add the Cocoa framework (and in this example the OpenGL framework)
// to your linked libraries.
//
// VisualStudio: Provide a property sheet named NAConfiguration.props right
// next to the solution (go see the example of this solution). Add the
// following line using a text editor to your project (go see this project):
// <Import Project="$(SolutionDir)\NAConfiguration.props" Condition="Exists('$(SolutionDir)\NAConfiguration.props')" />
// It is proposed to add this line right after the line which imports the
// property sheet "Microsoft.Cpp.Default.props". Add the NALib project.



// Note that you could add an include path in your project to access the
// NAXXX.h files. But for the sake of simplicity, we use relative paths here.
#include "../../NALib/src/NABase.h"
#include "../../NALib/src/NAUI.h"
#include "../../NALib/src/NATranslator.h"
#include "translationIDs.h"

NAInt trFinanceGroup;
NAInt trGeneralGroup;

NABool reshapeWindow(NAReaction reaction){
  NA_UNUSED(reaction);
  // commented out as this just fills the console with the same line.
  //printf("Reshape" NA_NL);
  return NA_TRUE;
}

NABool keyPressed(NAReaction reaction){
  NAKeyboardStatus keyStatus;
  NAUIKeyCode key;

  NA_UNUSED(reaction);
  keyStatus = naGetKeyboardStatus();
  key = keyStatus.keyCode;
  if(key == NA_KEYCODE_ESC){naStopApplication();}
  printf("Key Press" NA_NL);
  return NA_TRUE;
}

NABool keyReleased(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Key Release" NA_NL);
  return NA_TRUE;
}

NABool mouseMoved(NAReaction reaction){
  NA_UNUSED(reaction);
  // commented out as this just fills the console with the same line.
  //printf("Mouse Move" NA_NL);
  return NA_TRUE;
}

NABool mouseEntered(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse Enter" NA_NL);
  return NA_TRUE;
}

NABool mouseExited(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Mouse exit" NA_NL);
  return NA_TRUE;
}

NABool buttonPressed(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Oh, do it again." NA_NL);
  return NA_TRUE;
}

NABool checkBoxPressed(NAReaction reaction){
  NA_UNUSED(reaction);
  NABool state = naGetCheckBoxState(reaction.uiElement);
  printf("Checked. New status is %d." NA_NL, (int)state);
  return NA_TRUE;
}

NABool radioPressed(NAReaction reaction){
  NA_UNUSED(reaction);
  printf("Checked radio %p." NA_NL, reaction.uiElement);
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
  NACheckBox* checkBox;
  NALabel* label;
  NARadio* radio1;
  NARadio* radio2;
  NATextBox* textBox;
  NATextField* textField;
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

  printf(NA_NL "===========" NA_NL);

  windowrect = naMakeRectS(120, 20, 400, 300);
  window = naNewWindow("Wurst", windowrect, NA_TRUE, 0);
  naAddUIReaction(window, NA_UI_COMMAND_RESHAPE,       reshapeWindow, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_KEYDOWN,       keyPressed, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_KEYUP,         keyReleased, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_MOUSE_MOVED,   mouseMoved, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_MOUSE_ENTERED, mouseEntered, NA_NULL);
  naAddUIReaction(window, NA_UI_COMMAND_MOUSE_EXITED,  mouseExited, NA_NULL);

//  NAOpenGLSpace* openglspace = naNewOpenGLSpace(window, windowrect.size, initOpenGLSpace, window);
//  naSetWindowContentSpace(window, openglspace);

  contentSpace = naGetWindowContentSpace(window);

  button = naNewPushButton("PushButton asdf", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, button, naMakePos(20., 20.));
  naAddUIReaction(button, NA_UI_COMMAND_PRESSED, buttonPressed, NA_NULL);

  checkBox = naNewCheckBox("CheckBox asdf", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, checkBox, naMakePos(20., 40.));
  naAddUIReaction(checkBox, NA_UI_COMMAND_PRESSED, checkBoxPressed, NA_NULL);

  label = naNewLabel("Label asdf", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, label, naMakePos(20., 60.));
  
  radio1 = naNewRadio("Radio 1", naMakeSize(200., 20.));
  radio2 = naNewRadio("Radio 2", naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, radio1, naMakePos(20., 80.));
  naAddSpaceChild(contentSpace, radio2, naMakePos(220., 80.));
  naAddUIReaction(radio1, NA_UI_COMMAND_PRESSED, radioPressed, NA_NULL);
  naAddUIReaction(radio2, NA_UI_COMMAND_PRESSED, radioPressed, NA_NULL);

  textBox = naNewTextBox(naMakeSize(200., 40.));
  naAddSpaceChild(contentSpace, textBox, naMakePos(20., 100.));
  
  textField = naNewTextField(naMakeSize(200., 20.));
  naAddSpaceChild(contentSpace, textField, naMakePos(20., 140.));

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
  printf("%d Bits Addresses, %d Bits Integers)" NA_NL, NA_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();
    naStartApplication(prestartup, poststartup, NA_NULL);
  naStopRuntime();

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished." NA_NL);
    NA_UNUSED(getchar());
  #endif

  return 0;
}



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
