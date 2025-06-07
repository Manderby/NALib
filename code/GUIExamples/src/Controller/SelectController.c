
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct SelectController{
  NASpace* space;

  NASelect* select;
  NASelect* selectDisabled;

  NALabel* outputLabel;
};



static void selectItemSelected(NAReaction reaction){
  SelectController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "Select item with index %d selected",
    (int)naGetSelectItemIndex(con->select, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);
}



SelectController* createSelectController(){
  SelectController* con = naAlloc(SelectController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->select = naNewSelect(200);
  naAddSpaceChild(con->space, con->select, naMakePos(TAB1, curPosY));
  for(size_t i = 0; i < 5; ++i){
    NAMenuItem* item = naNewMenuItem(naAllocSprintf(NA_TRUE, "Select menu item %d", i), NA_NULL);
    naAddSelectMenuItem(con->select, item, NA_NULL);
    naAddUIReaction(item, NA_UI_COMMAND_PRESSED, selectItemSelected, con);
  }
  naSetSelectIndexSelected(con->select, 3);

  con->selectDisabled = naNewSelect(200);
  naSetSelectEnabled(con->selectDisabled, NA_FALSE);
  NAMenuItem* disabledItem = naNewMenuItem(naAllocSprintf(NA_TRUE, "Disabled Select Item", 0), NA_NULL);
  naAddSelectMenuItem(con->selectDisabled, disabledItem, NA_NULL);
  naAddSpaceChild(con->space, con->selectDisabled, naMakePos(TAB2, curPosY));

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  NAFont* monospaceFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(con->outputLabel, monospaceFont);
  naRelease(monospaceFont);
  naAddSpaceChild(con->space, con->outputLabel, naMakePos(WINDOW_MARGIN, WINDOW_MARGIN));

  return con;
}



void clearSelectController(SelectController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getSelectControllerSpace(SelectController* con){
  return con->space;
}



void updateSelectController(SelectController* con) {
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
