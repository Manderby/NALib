
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAString.h"
#include "NAApp/NAApp.h"



struct ButtonController{
  NASpace* space;
  
  NALabel* textPushButtonLabel;
  NAButton* textPushButton;
  NALabel* reactionTextPushButton;

  NALabel* textStateButtonLabel;
  NAButton* textStateButton;
  NAButton* textStateButtonDisabled1;
  NAButton* textStateButtonDisabled2;
  NALabel* reactionTextStateButton;

  NALabel* iconPushButtonLabel;
  NAButton* iconPushButton;
  NALabel* reactionIconPushButton;

  NALabel* iconStateButtonLabel;
  NAButton* iconStateButton;
  NAButton* iconStateButtonDisabled1;
  NAButton* iconStateButtonDisabled2;
  NALabel* reactionIconStateButton;

  NALabel* imagePushButtonLabel;
  NAButton* imagePushButton;
  NALabel* reactionImagePushButton;

  NALabel* imageStateButtonLabel;
  NAButton* imageStateButton;
  NAButton* imageStateButtonDisabled1;
  NAButton* imageStateButtonDisabled2;
  NALabel* reactionImageStateButton;

  NALabel* submitLabel;
  NAButton* textButtonSubmit;
  NALabel* reactionSubmitButton;

  NALabel* abortLabel;
  NAButton* textButtonAbort;
  NALabel* reactionAbortButton;

  NALabel* disabledLabel;
};



static void buttonPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;

  if(reaction.uiElement == con->textPushButton){
    flashLabel(con->reactionTextPushButton, "Pressed");
  }else if(reaction.uiElement == con->textStateButton){
    NABool state = naGetButtonState(reaction.uiElement);
    flashLabel(con->reactionTextStateButton, naAllocSprintf(NA_TRUE, "State %d", (int)state));
  }else if(reaction.uiElement == con->iconPushButton){
    flashLabel(con->reactionIconPushButton, "Pressed");
  }else if(reaction.uiElement == con->iconStateButton){
    NABool state = naGetButtonState(reaction.uiElement);
    flashLabel(con->reactionIconStateButton, naAllocSprintf(NA_TRUE, "State %d", (int)state));
  }else if(reaction.uiElement == con->imagePushButton){
    flashLabel(con->reactionImagePushButton, "Pressed");
  }else if(reaction.uiElement == con->imageStateButton){
    NABool state = naGetButtonState(reaction.uiElement);
    flashLabel(con->reactionImageStateButton, naAllocSprintf(NA_TRUE, "State %d", (int)state));
  }
}



static void submitPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  flashLabel(con->reactionSubmitButton, "Activated");
}



static void abortPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  flashLabel(con->reactionAbortButton, "Activated");
}



ButtonController* createButtonController(){
  ButtonController* con = naAlloc(ButtonController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->textPushButtonLabel = naNewLabel("Text Push", COLUMN0_WIDTH);
  naSetLabelFont(con->textPushButtonLabel, getTitleFont());
  naAddSpaceChild(con->space, con->textPushButtonLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->textPushButton = naNewTextPushButton("Push", COLUMN1_WIDTH);
  naAddUIReaction(con->textPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->textPushButton, naMakePos(TAB1, curPosY));

  con->reactionTextPushButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionTextPushButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionTextPushButton, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->textStateButtonLabel = naNewLabel("Text Two States", COLUMN0_WIDTH);
  naSetLabelFont(con->textStateButtonLabel, getTitleFont());
  naAddSpaceChild(con->space, con->textStateButtonLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->textStateButton = naNewTextStateButton("Off", "On", COLUMN1_WIDTH);
  naAddUIReaction(con->textStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->textStateButton, naMakePos(TAB1, curPosY));

  con->reactionTextStateButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionTextStateButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionTextStateButton, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->iconPushButtonLabel = naNewLabel("Icon Push", COLUMN0_WIDTH);
  naSetLabelFont(con->iconPushButtonLabel, getTitleFont());
  naAddSpaceChild(con->space, con->iconPushButtonLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->iconPushButton = naNewIconPushButton(getIconImageSet(), COLUMN1_WIDTH);
  naAddUIReaction(con->iconPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconPushButton, naMakePos(TAB1, curPosY));

  con->reactionIconPushButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionIconPushButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionIconPushButton, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->iconStateButtonLabel = naNewLabel("Icon Two States", COLUMN0_WIDTH);
  naSetLabelFont(con->iconStateButtonLabel, getTitleFont());
  naAddSpaceChild(con->space, con->iconStateButtonLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->iconStateButton = naNewIconStateButton(getIconImageSet(), NA_NULL, COLUMN1_WIDTH);
  naAddUIReaction(con->iconStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconStateButton, naMakePos(TAB1, curPosY));

  con->reactionIconStateButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionIconStateButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionIconStateButton, naMakePos(TAB2, curPosY));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->imagePushButtonLabel = naNewLabel("Image Push", COLUMN0_WIDTH);
  naSetLabelFont(con->imagePushButtonLabel, getTitleFont());
  naAddSpaceChild(con->space, con->imagePushButtonLabel, naMakePos(WINDOW_MARGIN, curPosY + .5 * UI_ELEMENT_HEIGTH));

  con->imagePushButton = naNewImagePushButton(getState1ImageSet(), naMakeSize(COLUMN1_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(con->imagePushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imagePushButton, naMakePos(TAB1, curPosY));

  con->reactionImagePushButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionImagePushButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionImagePushButton, naMakePos(TAB2, curPosY + .5 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->imageStateButtonLabel = naNewLabel("Image Two states", COLUMN0_WIDTH);
  naSetLabelFont(con->imageStateButtonLabel, getTitleFont());
  naAddSpaceChild(con->space, con->imageStateButtonLabel, naMakePos(WINDOW_MARGIN, curPosY + .5 * UI_ELEMENT_HEIGTH));

  con->imageStateButton = naNewImageStateButton(getState1ImageSet(), getState2ImageSet(), naMakeSize(COLUMN1_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(con->imageStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imageStateButton, naMakePos(TAB1, curPosY));

  con->reactionImageStateButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionImageStateButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionImageStateButton, naMakePos(TAB2, curPosY + .5 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->disabledLabel = naNewLabel("Disabled", COLUMN0_WIDTH);
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(con->space, con->disabledLabel, naMakePos(WINDOW_MARGIN, curPosY + UI_ELEMENT_HEIGTH * .5));

  con->textStateButtonDisabled1 = naNewTextStateButton("Off", NA_NULL, SMALL_BUTTON_WIDTH);
  naAddUIReaction(con->textStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonEnabled(con->textStateButtonDisabled1, NA_FALSE);
  naAddSpaceChild(con->space, con->textStateButtonDisabled1, naMakePos(TAB1 + 0 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5));

  con->textStateButtonDisabled2 = naNewTextStateButton(NA_NULL, "On", SMALL_BUTTON_WIDTH);
  naAddUIReaction(con->textStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonState(con->textStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->textStateButtonDisabled2, NA_FALSE);
  naAddSpaceChild(con->space, con->textStateButtonDisabled2, naMakePos(TAB1 + 1 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5));

  con->iconStateButtonDisabled1 = naNewIconStateButton(getIconImageSet(), NA_NULL, SMALL_BUTTON_WIDTH);
  naSetButtonEnabled(con->iconStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconStateButtonDisabled1, naMakePos(TAB1 + 2 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5));

  con->iconStateButtonDisabled2 = naNewIconStateButton(getIconImageSet(), NA_NULL, SMALL_BUTTON_WIDTH);
  naSetButtonState(con->iconStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->iconStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconStateButtonDisabled2, naMakePos(TAB1 + 3 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY + UI_ELEMENT_HEIGTH * .5));

  con->imageStateButtonDisabled1 = naNewImageStateButton(getState1ImageSet(), NA_NULL, naMakeSize(SMALL_BUTTON_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naSetButtonEnabled(con->imageStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imageStateButtonDisabled1, naMakePos(TAB1 + 4 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY));

  con->imageStateButtonDisabled2 = naNewImageStateButton(NA_NULL, getState2ImageSet(), naMakeSize(SMALL_BUTTON_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naSetButtonState(con->imageStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->imageStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imageStateButtonDisabled2, naMakePos(TAB1 + 5 * (SMALL_BUTTON_WIDTH + UI_ELEMENT_MARGIN), curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;
  
  con->submitLabel = naNewLabel("Submit (Enter)", COLUMN0_WIDTH);
  naSetLabelFont(con->submitLabel, getTitleFont());
  naAddSpaceChild(con->space, con->submitLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->textButtonSubmit = naNewTextPushButton("Ok Button", COLUMN1_WIDTH);
  naAddUIReaction(con->textButtonSubmit, NA_UI_COMMAND_PRESSED, submitPressed, con);
  naAddSpaceChild(con->space, con->textButtonSubmit, naMakePos(TAB1, curPosY));
//  naSetButtonSubmit(con->textButtonSubmit, submitPressed, con);

  con->reactionSubmitButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionSubmitButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionSubmitButton, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->abortLabel = naNewLabel("Abort (Esc)", COLUMN0_WIDTH);
  naSetLabelFont(con->abortLabel, getTitleFont());
  naAddSpaceChild(con->space, con->abortLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->textButtonAbort = naNewTextPushButton("Cancel Button", COLUMN1_WIDTH);
  naAddUIReaction(con->textButtonAbort, NA_UI_COMMAND_PRESSED, abortPressed, con);
  naAddSpaceChild(con->space, con->textButtonAbort, naMakePos(TAB1, curPosY));
//  naSetButtonAbort(con->textButtonAbort, abortPressed, con);

  con->reactionAbortButton = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->reactionAbortButton, getMonoFont());
  naAddSpaceChild(con->space, con->reactionAbortButton, naMakePos(TAB2, curPosY));

  return con;
}



void clearButtonController(ButtonController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getButtonControllerSpace(ButtonController* con){
  return con->space;
}



void updateButtonController(ButtonController* con){
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
