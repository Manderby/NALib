
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct TextFieldController{
  CommonController comCon;

  NALabel* enabledLabel;
  NATextField* textField;
  NALabel* normalReaction;

  NALabel* disabledLabel;
  NATextField* textFieldDisabled;
};



static void textFieldEdited(NAReaction reaction) {
  TextFieldController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "%s", naGetStringUTF8Pointer(textFieldString));
  flashLabel(con->normalReaction, outputText);
  naDelete(textFieldString);
}

static void textFieldEditFinished(NAReaction reaction) {
  TextFieldController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "End %s", naGetStringUTF8Pointer(textFieldString));
  flashLabel(con->normalReaction, outputText);
  naDelete(textFieldString);
}



CommonController* createTextFieldController() {
  TextFieldController* con = naAlloc(TextFieldController);

  NASpace* space = naNewSpace();
  naSetUIElementRect(space, naMakeRectS(0., 0., WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->enabledLabel = naNewLabel("Normal");
  naSetLabelFont(con->enabledLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->enabledLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->textField = naNewTextField();
  naAddSpaceChild(
    space,
    con->textField,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDITED, textFieldEdited, con);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, textFieldEditFinished, con);
  
  con->normalReaction = naNewLabel("");
  naSetLabelFont(con->normalReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->normalReaction,
    naMakePos(TAB2, curPosY),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->disabledLabel = naNewLabel("Disabled");
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->disabledLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->textFieldDisabled = naNewTextField();
  naSetTextFieldEnabled(con->textFieldDisabled, NA_FALSE);
  naSetTextFieldText(con->textFieldDisabled, "Disabled Textfield");
  naAddSpaceChild(
    space,
    con->textFieldDisabled,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

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
