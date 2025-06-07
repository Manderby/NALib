
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct CheckBoxController{
  NASpace* space;

  NALabel* normalLabel;
  NACheckBox* checkBox;

  NALabel* disabledLabel;
  NACheckBox* checkBoxDisabled1;
  NACheckBox* checkBoxDisabled2;

  NALabel* outputLabel;
};



static void checkBoxPressed(NAReaction reaction){
  CheckBoxController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "CheckBox Pressed");
}



CheckBoxController* createCheckBoxController(){
  CheckBoxController* con = naAlloc(CheckBoxController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalLabel = naNewLabel("Normal", COLUMN0_WIDTH);
  naSetLabelFont(con->normalLabel, getTitleFont());
  naAddSpaceChild(con->space, con->normalLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->checkBox = naNewCheckBox("I am a CheckBox", COLUMN1_WIDTH);
  naAddSpaceChild(con->space, con->checkBox, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->checkBox, NA_UI_COMMAND_PRESSED, checkBoxPressed, con);
  
  curPosY = curPosY - (2. * UI_ELEMENT_HEIGTH);

  con->disabledLabel = naNewLabel("Disabled", COLUMN0_WIDTH);
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(con->space, con->disabledLabel, naMakePos(WINDOW_MARGIN, curPosY + .5 * UI_ELEMENT_HEIGTH));

  con->checkBoxDisabled1 = naNewCheckBox("Disabled CheckBox On", COLUMN1_WIDTH);
  naSetCheckBoxState(con->checkBoxDisabled1, NA_TRUE);
  naSetCheckBoxEnabled(con->checkBoxDisabled1, NA_FALSE);
  naAddSpaceChild(con->space, con->checkBoxDisabled1, naMakePos(TAB1, curPosY + UI_ELEMENT_HEIGTH));
  
  con->checkBoxDisabled1 = naNewCheckBox("Disabled CheckBox Off", COLUMN1_WIDTH);
  naSetCheckBoxEnabled(con->checkBoxDisabled1, NA_FALSE);
  naAddSpaceChild(con->space, con->checkBoxDisabled1, naMakePos(TAB1, curPosY));

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naSetLabelFont(con->outputLabel, getMonoFont());
  naAddSpaceChild(con->space, con->outputLabel, naMakePos(WINDOW_MARGIN, WINDOW_MARGIN));

  return con;
}



void clearCheckBoxController(CheckBoxController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getCheckBoxControllerSpace(CheckBoxController* con){
  return con->space;
}



void updateCheckBoxController(CheckBoxController* con) {
  NA_UNUSED(con);
  // nothing to do.
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
