
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "../../NALib/src/NAUI.h"
#include "HelloWorldGUI.h"



struct ExperimentController{
  NAWindow* experimentWindow;
  NASpace* contentSpace;
  NAButton* pushButton;
  NAButton* textOptionButton1;
  NAButton* textOptionButton2;
  NAButton* textOptionButton3;
  int textOption;
  NALabel* outputLabel;
};

NABool buttonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Push Button Pressed");

  return NA_TRUE;
}

NABool optionButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  if     (reaction.uiElement == con->textOptionButton1){con->textOption = 1;}
  else if(reaction.uiElement == con->textOptionButton2){con->textOption = 2;}
  else if(reaction.uiElement == con->textOptionButton3){con->textOption = 3;}

  naSetButtonState(con->textOptionButton1, con->textOption == 1);
  naSetButtonState(con->textOptionButton2, con->textOption == 2);
  naSetButtonState(con->textOptionButton3, con->textOption == 3);

  naSetLabelText(con->outputLabel, "Option Button Pressed");

  return NA_TRUE;
}

ExperimentController* createExperimentController(){
  double windowWidth = 700;
  double windowHeight = 1000;

  ExperimentController* con = naAlloc(ExperimentController);

  con->experimentWindow = naNewWindow(
    "Experiment",
    naMakeRectS(250, 350, windowWidth, windowHeight),
    NA_TRUE,
    0);

  con->contentSpace = naNewSpace(naMakeSize(windowWidth, windowHeight));
  naSetWindowContentSpace(con->experimentWindow, con->contentSpace);

  con->pushButton = naNewPushButton("Push Button", naMakeSize(150, 40));
  naAddSpaceChild(con->contentSpace, con->pushButton, naMakePos(0, windowHeight - 40));
  naAddUIReaction(con->pushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->textOptionButton1 = naNewTextOptionButton("Text 1", naMakeSize(50, 40));
  naAddSpaceChild(con->contentSpace, con->textOptionButton1, naMakePos(200, windowHeight - 40));
  naAddUIReaction(con->textOptionButton1, NA_UI_COMMAND_PRESSED, optionButtonPressed, con);
  con->textOptionButton2 = naNewTextOptionButton("Text 2", naMakeSize(50, 40));
  naAddSpaceChild(con->contentSpace, con->textOptionButton2, naMakePos(250, windowHeight - 40));
  naAddUIReaction(con->textOptionButton2, NA_UI_COMMAND_PRESSED, optionButtonPressed, con);
  con->textOptionButton3 = naNewTextOptionButton("Text 3", naMakeSize(50, 40));
  naAddSpaceChild(con->contentSpace, con->textOptionButton3, naMakePos(300, windowHeight - 40));
  naAddUIReaction(con->textOptionButton3, NA_UI_COMMAND_PRESSED, optionButtonPressed, con);

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    naMakeSize(windowWidth - 20, 44));
  naAddSpaceChild(con->contentSpace, con->outputLabel, naMakePos(10, 10));

  con->textOption = 0;

  naShowWindow(con->experimentWindow);

  return con;
}


void clearExperimentController(ExperimentController* con){
  naReleaseUIElement(con->experimentWindow);
  naFree(con);
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
