
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct SelectController{
  CommonController comCon;

  NALabel* normalLabel;
  NASelect* select;
  NALabel* normalReaction;

  NALabel* disabledLabel;
  NASelect* selectDisabled;
};



static void selectItemSelected(NAReaction reaction) {
  SelectController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "Index %d",
    (int)naGetSelectItemIndex(con->select, reaction.uiElement));
  flashLabel(con->normalReaction, outputText);
}



CommonController* createSelectController() {
  SelectController* con = naAlloc(SelectController);

  NASpace* space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalLabel = naNewLabel("Normal", COLUMN0_WIDTH);
  naSetLabelFont(con->normalLabel, getTitleFont());
  naAddSpaceChild(space, con->normalLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->select = naNewSelect(COLUMN1_WIDTH);
  naAddSpaceChild(space, con->select, naMakePos(TAB1, curPosY));
  for(size_t i = 0; i < 5; ++i) {
    NAMenuItem* item = naNewMenuItem(naAllocSprintf(NA_TRUE, "Select menu item %d", i), NA_NULL);
    naAddSelectMenuItem(con->select, item, NA_NULL);
    naAddUIReaction(item, NA_UI_COMMAND_PRESSED, selectItemSelected, con);
  }
  naSetSelectIndexSelected(con->select, 3);

  con->normalReaction = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->normalReaction, getMonoFont());
  naAddSpaceChild(space, con->normalReaction, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->disabledLabel = naNewLabel("Disabled", COLUMN0_WIDTH);
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(space, con->disabledLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->selectDisabled = naNewSelect(COLUMN1_WIDTH);
  naSetSelectEnabled(con->selectDisabled, NA_FALSE);
  NAMenuItem* disabledItem = naNewMenuItem(naAllocSprintf(NA_TRUE, "Disabled Select", 0), NA_NULL);
  naAddSelectMenuItem(con->selectDisabled, disabledItem, NA_NULL);
  naAddSpaceChild(space, con->selectDisabled, naMakePos(TAB1, curPosY));

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
