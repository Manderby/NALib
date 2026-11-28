
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAString.h"



struct ButtonController{
  CommonController comCon;
  
  NALabel* textPushButtonLabel;
  NAButton* textPushButton;
  NALabel* textPushButtonReaction;

  NALabel* textStateButtonLabel;
  NAButton* textStateButton;
  NAButton* textStateButtonDisabled1;
  NAButton* textStateButtonDisabled2;
  NALabel* textStateButtonReaction;

  NALabel* iconPushButtonLabel;
  NAButton* iconPushButton;
  NALabel* iconPushButtonReaction;

  NALabel* iconStateButtonLabel;
  NAButton* iconStateButton;
  NAButton* iconStateButtonDisabled1;
  NAButton* iconStateButtonDisabled2;
  NALabel* iconStateButtonReaction;

  NALabel* imagePushButtonLabel;
  NAButton* imagePushButton;
  NALabel* imagePushButtonReaction;

  NALabel* imageStateButtonLabel;
  NAButton* imageStateButton;
  NAButton* imageStateButtonDisabled1;
  NAButton* imageStateButtonDisabled2;
  NALabel* imageStateButtonReaction;

  NALabel* submitLabel;
  NAButton* textButtonSubmit;
  NALabel* submitButtonReaction;

  NALabel* abortLabel;
  NAButton* textButtonAbort;
  NALabel* abortButtonReaction;

  NALabel* disabledLabel;

  NABool firstTime;
};

// Prototype:
void updateButtonController(ButtonController* con);



static void buttonPressed(NAReaction reaction) {
  ButtonController* con = reaction.controller;

  if(reaction.uiElement == con->textPushButton) {
    flashLabel(con->textPushButtonReaction, "Pressed");
  }else if(reaction.uiElement == con->textStateButton) {
    NABool state = naGetButtonState(reaction.uiElement);
    flashLabel(con->textStateButtonReaction, naAllocSprintf(NA_TRUE, "State %d", (int)state));
  }else if(reaction.uiElement == con->iconPushButton) {
    flashLabel(con->iconPushButtonReaction, "Pressed");
  }else if(reaction.uiElement == con->iconStateButton) {
    NABool state = naGetButtonState(reaction.uiElement);
    flashLabel(con->iconStateButtonReaction, naAllocSprintf(NA_TRUE, "State %d", (int)state));
  }else if(reaction.uiElement == con->imagePushButton) {
    flashLabel(con->imagePushButtonReaction, "Pressed");
  }else if(reaction.uiElement == con->imageStateButton) {
    NABool state = naGetButtonState(reaction.uiElement);
    flashLabel(con->imageStateButtonReaction, naAllocSprintf(NA_TRUE, "State %d", (int)state));
  }
}



static void submitPressed(NAReaction reaction) {
  ButtonController* con = reaction.controller;
  flashLabel(con->submitButtonReaction, "Activated");
}



static void abortPressed(NAReaction reaction) {
  ButtonController* con = reaction.controller;
  flashLabel(con->abortButtonReaction, "Activated");
}



CommonController* createButtonController() {
  ButtonController* con = naAlloc(ButtonController);
  
  NASpace* space = naNewSpace();
  naSetUIElementRect(space, naMakeRectS(0., 0., WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->textPushButtonLabel = naNewLabel("Text Push");
  naSetLabelFont(con->textPushButtonLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->textPushButtonLabel,
    naMakePos(WINDOW_MARGIN,
    curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->textPushButton = naNewTextPushButton("Push");
  naAddUIReaction(con->textPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->textPushButton,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  con->textPushButtonReaction = naNewLabel("");
  naSetLabelFont(con->textPushButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->textPushButtonReaction,
    naMakePos(TAB2, curPosY),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->textStateButtonLabel = naNewLabel("Text Two States");
  naSetLabelFont(con->textStateButtonLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->textStateButtonLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->textStateButton = naNewTextStateButton("Off", "On");
  naAddUIReaction(con->textStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->textStateButton,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  con->textStateButtonReaction = naNewLabel("");
  naSetLabelFont(con->textStateButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->textStateButtonReaction,
    naMakePos(TAB2, curPosY),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->iconPushButtonLabel = naNewLabel("Icon Push");
  naSetLabelFont(con->iconPushButtonLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->iconPushButtonLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->iconPushButton = naNewIconPushButton(getIconImageSet());
  naAddUIReaction(con->iconPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->iconPushButton,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  con->iconPushButtonReaction = naNewLabel("");
  naSetLabelFont(con->iconPushButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->iconPushButtonReaction,
    naMakePos(TAB2, curPosY),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->iconStateButtonLabel = naNewLabel("Icon Two States");
  naSetLabelFont(con->iconStateButtonLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->iconStateButtonLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->iconStateButton = naNewIconStateButton(getIconImageSet(), NA_NULL);
  naAddUIReaction(con->iconStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->iconStateButton,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  con->iconStateButtonReaction = naNewLabel("");
  naSetLabelFont(con->iconStateButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->iconStateButtonReaction,
    naMakePos(TAB2, curPosY),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->imagePushButtonLabel = naNewLabel("Image Push");
  naSetLabelFont(con->imagePushButtonLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->imagePushButtonLabel,
    naMakePos(WINDOW_MARGIN, curPosY + .5 * UI_ELEMENT_HEIGTH),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->imagePushButton = naNewImagePushButton(getState1ImageSet());
  naAddUIReaction(con->imagePushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->imagePushButton,
    naMakePos(TAB1, curPosY),
    naMakeSize(COLUMN1_WIDTH, 2 * UI_ELEMENT_HEIGTH));

  //naDebugUIElement(con->imagePushButton);
  //naDebugUIElement(space);

  con->imagePushButtonReaction = naNewLabel("");
  naSetLabelFont(con->imagePushButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->imagePushButtonReaction,
    naMakePos(TAB2, curPosY + .5 * UI_ELEMENT_HEIGTH),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->imageStateButtonLabel = naNewLabel("Image Two states");
  naSetLabelFont(con->imageStateButtonLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->imageStateButtonLabel,
    naMakePos(WINDOW_MARGIN, curPosY + .5 * UI_ELEMENT_HEIGTH),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->imageStateButton = naNewImageStateButton(getState1ImageSet(), getState2ImageSet());
  naAddUIReaction(con->imageStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->imageStateButton,
    naMakePos(TAB1, curPosY),
    naMakeSize(COLUMN1_WIDTH, 2 * UI_ELEMENT_HEIGTH));

  con->imageStateButtonReaction = naNewLabel("");
  naSetLabelFont(con->imageStateButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->imageStateButtonReaction,
    naMakePos(TAB2, curPosY + .5 * UI_ELEMENT_HEIGTH),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->disabledLabel = naNewLabel("Disabled");
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->disabledLabel,
    naMakePos(WINDOW_MARGIN, curPosY + UI_ELEMENT_HEIGTH * .5),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->textStateButtonDisabled1 = naNewTextStateButton("Off", NA_NULL);
  naAddUIReaction(con->textStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonEnabled(con->textStateButtonDisabled1, NA_FALSE);
  naAddSpaceChild(
    space,
    con->textStateButtonDisabled1,
    naMakePos(TAB1 + 0 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5),
    naMakeInlineSize(SMALL_BUTTON_WIDTH));

  con->textStateButtonDisabled2 = naNewTextStateButton(NA_NULL, "On");
  naAddUIReaction(con->textStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonState(con->textStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->textStateButtonDisabled2, NA_FALSE);
  naAddSpaceChild(
    space,
    con->textStateButtonDisabled2,
    naMakePos(TAB1 + 1 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5),
    naMakeInlineSize(SMALL_BUTTON_WIDTH));

  con->iconStateButtonDisabled1 = naNewIconStateButton(getIconImageSet(), NA_NULL);
  naSetButtonEnabled(con->iconStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->iconStateButtonDisabled1,
    naMakePos(TAB1 + 2 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5),
    naMakeInlineSize(SMALL_BUTTON_WIDTH));

  con->iconStateButtonDisabled2 = naNewIconStateButton(getIconImageSet(), NA_NULL);
  naSetButtonState(con->iconStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->iconStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->iconStateButtonDisabled2,
    naMakePos(TAB1 + 3 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5),
    naMakeInlineSize(SMALL_BUTTON_WIDTH));

  con->imageStateButtonDisabled1 = naNewImageStateButton(getState1ImageSet(), NA_NULL);
  naSetButtonEnabled(con->imageStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->imageStateButtonDisabled1,
    naMakePos(TAB1 + 4 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY),
    naMakeSize(SMALL_BUTTON_WIDTH, 2 * UI_ELEMENT_HEIGTH));

  con->imageStateButtonDisabled2 = naNewImageStateButton(NA_NULL, getState2ImageSet());
  naSetButtonState(con->imageStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->imageStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(
    space,
    con->imageStateButtonDisabled2,
    naMakePos(TAB1 + 5 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY),
    naMakeSize(SMALL_BUTTON_WIDTH, 2 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;
  
  con->submitLabel = naNewLabel("Submit (Enter)");
  naSetLabelFont(con->submitLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->submitLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->textButtonSubmit = naNewTextPushButton("Ok Button");
  naAddUIReaction(con->textButtonSubmit, NA_UI_COMMAND_PRESSED, submitPressed, con);
  naAddSpaceChild(
    space,
    con->textButtonSubmit,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));
  // Usually, one would set the submit button upon creation. But submit and
  // cancel buttons require the space they're added to to belong to a window.
  // As this example application attaches the spaces to the window later, we
  // must set these buttons properly in the update function below.
  // naSetButtonSubmit(con->textButtonSubmit, submitPressed, con);

  con->submitButtonReaction = naNewLabel("");
  naSetLabelFont(con->submitButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->submitButtonReaction,
    naMakePos(TAB2, curPosY),
    naMakeInlineSize(COLUMN2_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->abortLabel = naNewLabel("Abort (Esc)");
  naSetLabelFont(con->abortLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->abortLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->textButtonAbort = naNewTextPushButton("Cancel Button");
  naAddUIReaction(con->textButtonAbort, NA_UI_COMMAND_PRESSED, abortPressed, con);
  naAddSpaceChild(
    space,
    con->textButtonAbort,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));
  // Usually, one would set the submit button upon creation. But submit and
  // cancel buttons require the space they're added to to belong to a window.
  // As this example application attaches the spaces to the window later, we
  // must set these buttons properly in the update function below.
  // naSetButtonAbort(con->textButtonAbort, abortPressed, con);

  con->abortButtonReaction = naNewLabel("");
  naSetLabelFont(con->abortButtonReaction, getMonoFont());
  naAddSpaceChild(
    space,
    con->abortButtonReaction,
    naMakePos(TAB2, curPosY),
    naMakeInlineSize(COLUMN2_WIDTH));

  con->firstTime = NA_TRUE;

  initCommonController(
    &con->comCon,
    space,
    NA_NULL,
    (NAMutator)updateButtonController);

  return (CommonController*)con;
}



void updateButtonController(ButtonController* con) {
  if(con->firstTime) {
    naSetButtonSubmit(con->textButtonSubmit, submitPressed, con);
    naSetButtonAbort(con->textButtonAbort, abortPressed, con);

    con->firstTime = NA_FALSE;
  }
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
