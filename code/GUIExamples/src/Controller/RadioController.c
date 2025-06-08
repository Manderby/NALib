
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct RadioController{
  CommonController comCon;

  NALabel* normalLabel;
  NARadio* radio;
  NALabel* normalReaction;

  NALabel* disabledLabel;
  NARadio* radioDisabled1;
  NARadio* radioDisabled2;
};



static void radioPressed(NAReaction reaction) {
  RadioController* con = reaction.controller;
  flashLabel(con->normalReaction, "Pressed");
}



CommonController* createRadioController() {
  RadioController* con = naAlloc(RadioController);

  NASpace* space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalLabel = naNewLabel("Normal", COLUMN0_WIDTH);
  naSetLabelFont(con->normalLabel, getTitleFont());
  naAddSpaceChild(space, con->normalLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->radio = naNewRadio("I am a Radio", COLUMN1_WIDTH);
  naAddSpaceChild(space, con->radio, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->radio, NA_UI_COMMAND_PRESSED, radioPressed, con);
  
  con->normalReaction = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->normalReaction, getMonoFont());
  naAddSpaceChild(space, con->normalReaction, naMakePos(TAB2, curPosY));

  curPosY = curPosY - (2. * UI_ELEMENT_HEIGTH);

  con->disabledLabel = naNewLabel("Disabled", COLUMN0_WIDTH);
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(space, con->disabledLabel, naMakePos(WINDOW_MARGIN, curPosY + .5 * UI_ELEMENT_HEIGTH));

  con->radioDisabled1 = naNewRadio("Disabled Radio On", COLUMN1_WIDTH);
  naSetRadioState(con->radioDisabled1, NA_TRUE);
  naSetRadioEnabled(con->radioDisabled1, NA_FALSE);
  naAddSpaceChild(space, con->radioDisabled1, naMakePos(TAB1, curPosY + UI_ELEMENT_HEIGTH));

  con->radioDisabled1 = naNewRadio("Disabled Radio Off", COLUMN1_WIDTH);
  naSetRadioEnabled(con->radioDisabled1, NA_FALSE);
  naAddSpaceChild(space, con->radioDisabled1, naMakePos(TAB1, curPosY));

  initCommonController(
    &con->comCon,
    space,
    NA_NULL,
    NA_NULL);

  return (CommonController*)con;
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
