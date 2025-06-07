
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct TextFieldController{
  NASpace* space;

  NALabel* enabledLabel;
  NALabel* disabledLabel;

  NATextField* textField;
  NATextField* textFieldDisabled;

  NALabel* outputLabel;
};



static void textFieldEdited(NAReaction reaction){
  TextFieldController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "TextField Value Edited to %s", naGetStringUTF8Pointer(textFieldString));
  naSetLabelText(con->outputLabel, outputText);
  naDelete(textFieldString);
}

static void textFieldEditFinished(NAReaction reaction){
  TextFieldController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "TextField Value Finished Editing to %s", naGetStringUTF8Pointer(textFieldString));
  naSetLabelText(con->outputLabel, outputText);
  naDelete(textFieldString);
}



TextFieldController* createTextFieldController(){
  TextFieldController* con = naAlloc(TextFieldController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->enabledLabel = naNewLabel("Normal", COLUMN0_WIDTH);
  naSetLabelFont(con->enabledLabel, getTitleFont());
  naSetLabelTextAlignment(con->enabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(con->space, con->enabledLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->textField = naNewTextField(COLUMN1_WIDTH);
  naAddSpaceChild(con->space, con->textField, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDITED, textFieldEdited, con);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, textFieldEditFinished, con);
  
  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->disabledLabel = naNewLabel("Disabled", COLUMN0_WIDTH);
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naSetLabelTextAlignment(con->disabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(con->space, con->disabledLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->textFieldDisabled = naNewTextField(COLUMN1_WIDTH);
  naSetTextFieldEnabled(con->textFieldDisabled, NA_FALSE);
  naSetTextFieldText(con->textFieldDisabled, "Disabled Textfield");
  naAddSpaceChild(con->space, con->textFieldDisabled, naMakePos(TAB1, curPosY));

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naSetLabelFont(con->outputLabel, getMonoFont());
  naAddSpaceChild(con->space, con->outputLabel, naMakePos(WINDOW_MARGIN, WINDOW_MARGIN));

  return con;
}



void clearTextFieldController(TextFieldController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getTextFieldControllerSpace(TextFieldController* con){
  return con->space;
}



void updateTextFieldController(TextFieldController* con) {
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
