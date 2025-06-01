
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAString.h"
#include "NAApp/NAApp.h"



struct ButtonController{
  NASpace* space;
  
  NALabel* enabledLabel;
  NALabel* disabledLabel;

  NALabel* textPushButtonLabel;
  NAButton* textPushButton;
  NAButton* textPushButtonDisabled;

  NALabel* textStateButtonLabel;
  NAButton* textStateButton;
  NAButton* textStateButtonDisabled1;
  NAButton* textStateButtonDisabled2;
  
  NALabel* iconPushButtonLabel;
  NAButton* iconPushButton;
  NAButton* iconPushButtonDisabled;
  
  NALabel* iconStateButtonLabel;
  NAButton* iconStateButton;
  NAButton* iconStateButtonDisabled1;
  NAButton* iconStateButtonDisabled2;
  
  NALabel* imagePushButtonLabel;
  NAButton* imagePushButton;
  NAButton* imagePushButtonDisabled;
  
  NALabel* imageStateButtonLabel;
  NAButton* imageStateButton;
  NAButton* imageStateButtonDisabled1;
  NAButton* imageStateButtonDisabled2;

  NALabel* submitLabel;
  NAButton* textButtonSubmit;
  NALabel* abortLabel;
  NAButton* textButtonAbort;

  NALabel* outputLabel;
};



static void buttonPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;

  NAString* labelString;

  if(reaction.uiElement == con->textPushButton){
    labelString = naNewStringWithFormat("Text Push Button Pressed");
  }else if(reaction.uiElement == con->textStateButton){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Text State Button Switched to %d", (int)state);
  }else if(reaction.uiElement == con->iconPushButton){
    labelString = naNewStringWithFormat("Icon Push Button Pressed");
  }else if(reaction.uiElement == con->iconStateButton){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Icon State Button Switched to %d", (int)state);
  }else if(reaction.uiElement == con->imagePushButton){
    labelString = naNewStringWithFormat("Image Push Button Pressed");
  }else if(reaction.uiElement == con->imageStateButton){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Image State Button Switched to %d", (int)state);
  }else{
    labelString = naNewString();
  }

  naSetLabelText(con->outputLabel, naGetStringUTF8Pointer(labelString));
  naDelete(labelString);
}



static void submitPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Submit pressed");
}



static void abortPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Abort pressed");
}



ButtonController* createButtonController(){
  ButtonController* con = naAlloc(ButtonController);

//  double labelWidth = 200;
//  double left1 = 240;
//  double left2 = 400;
//
//  double buttonWidth = 120;
//  double buttonHeight = 50;

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN;

  NAFont* titleFont = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_DEFAULT);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->enabledLabel = naNewLabel("Enabled", COLUMN1_WIDTH);
  naSetLabelFont(con->enabledLabel, titleFont);
  naSetLabelTextAlignment(con->enabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(con->space, con->enabledLabel, naMakePos(TAB1, curPosY));

  con->disabledLabel = naNewLabel("Disabled", COLUMN2_WIDTH);
  naSetLabelFont(con->disabledLabel, titleFont);
  naSetLabelTextAlignment(con->disabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(con->space, con->disabledLabel, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->textPushButtonLabel = naNewLabel("Text Push", COLUMN0_WIDTH);
  naSetLabelFont(con->textPushButtonLabel, titleFont);
  naAddSpaceChild(con->space, con->textPushButtonLabel, naMakePos(20, curPosY));

  con->textPushButton = naNewTextPushButton("Push", COLUMN1_WIDTH);
  naAddUIReaction(con->textPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->textPushButton, naMakePos(TAB1, curPosY));

  con->textPushButtonDisabled = naNewTextPushButton("Push", COLUMN2_WIDTH);
  naAddUIReaction(con->textPushButtonDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonEnabled(con->textPushButtonDisabled, NA_FALSE);
  naAddSpaceChild(con->space, con->textPushButtonDisabled, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->textStateButtonLabel = naNewLabel("Text Two States", COLUMN0_WIDTH);
  naSetLabelFont(con->textStateButtonLabel, titleFont);
  naAddSpaceChild(con->space, con->textStateButtonLabel, naMakePos(20, curPosY));

  con->textStateButton = naNewTextStateButton("Off", "On", COLUMN1_WIDTH);
  naAddUIReaction(con->textStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->textStateButton, naMakePos(TAB1, curPosY));

  con->textStateButtonDisabled1 = naNewTextStateButton("Off", NA_NULL, COLUMN2_HALF_WIDTH);
  naAddUIReaction(con->textStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonEnabled(con->textStateButtonDisabled1, NA_FALSE);
  naAddSpaceChild(con->space, con->textStateButtonDisabled1, naMakePos(TAB2, curPosY));

  con->textStateButtonDisabled2 = naNewTextStateButton(NA_NULL, "On", COLUMN2_HALF_WIDTH);
  naAddUIReaction(con->textStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonState(con->textStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->textStateButtonDisabled2, NA_FALSE);
  naAddSpaceChild(con->space, con->textStateButtonDisabled2, naMakePos(TAB2_1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->iconPushButtonLabel = naNewLabel("Icon Push", COLUMN0_WIDTH);
  naSetLabelFont(con->iconPushButtonLabel, titleFont);
  naAddSpaceChild(con->space, con->iconPushButtonLabel, naMakePos(20, curPosY));

  con->iconPushButton = naNewIconPushButton(getIconImageSet(), COLUMN1_WIDTH);
  naAddUIReaction(con->iconPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconPushButton, naMakePos(TAB1, curPosY));

  con->iconPushButtonDisabled = naNewIconPushButton(getIconImageSet(), COLUMN2_WIDTH);
  naSetButtonEnabled(con->iconPushButtonDisabled, NA_FALSE);
  naAddUIReaction(con->iconPushButtonDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconPushButtonDisabled, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->iconStateButtonLabel = naNewLabel("Icon Two States", COLUMN0_WIDTH);
  naSetLabelFont(con->iconStateButtonLabel, titleFont);
  naAddSpaceChild(con->space, con->iconStateButtonLabel, naMakePos(20, curPosY));

  con->iconStateButton = naNewIconStateButton(getIconImageSet(), NA_NULL, COLUMN1_WIDTH);
  naAddUIReaction(con->iconStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconStateButton, naMakePos(TAB1, curPosY));

  con->iconStateButtonDisabled1 = naNewIconStateButton(getIconImageSet(), NA_NULL, COLUMN2_HALF_WIDTH);
  naSetButtonEnabled(con->iconStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconStateButtonDisabled1, naMakePos(TAB2, curPosY));

  con->iconStateButtonDisabled2 = naNewIconStateButton(getIconImageSet(), NA_NULL, COLUMN2_HALF_WIDTH);
  naSetButtonState(con->iconStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->iconStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->iconStateButtonDisabled2, naMakePos(TAB2_1, curPosY));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->imagePushButtonLabel = naNewLabel("Image Push", COLUMN0_WIDTH);
  naSetLabelFont(con->imagePushButtonLabel, titleFont);
  naAddSpaceChild(con->space, con->imagePushButtonLabel, naMakePos(20, curPosY + UI_ELEMENT_HEIGTH));

  con->imagePushButton = naNewImagePushButton(getState1ImageSet(), naMakeSize(COLUMN1_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(con->imagePushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imagePushButton, naMakePos(TAB1, curPosY));

  con->imagePushButtonDisabled = naNewImagePushButton(getState1ImageSet(), naMakeSize(COLUMN2_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naSetButtonEnabled(con->imagePushButtonDisabled, NA_FALSE);
  naAddUIReaction(con->imagePushButtonDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imagePushButtonDisabled, naMakePos(TAB2, curPosY));

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH;

  con->imageStateButtonLabel = naNewLabel("Image Two states", COLUMN0_WIDTH);
  naSetLabelFont(con->imageStateButtonLabel, titleFont);
  naAddSpaceChild(con->space, con->imageStateButtonLabel, naMakePos(20, curPosY + UI_ELEMENT_HEIGTH));

  con->imageStateButton = naNewImageStateButton(getState1ImageSet(), getState2ImageSet(), naMakeSize(COLUMN1_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(con->imageStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imageStateButton, naMakePos(TAB1, curPosY));

  con->imageStateButtonDisabled1 = naNewImageStateButton(getState1ImageSet(), NA_NULL, naMakeSize(COLUMN2_HALF_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naSetButtonEnabled(con->imageStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imageStateButtonDisabled1, naMakePos(TAB2, curPosY));

  con->imageStateButtonDisabled2 = naNewImageStateButton(NA_NULL, getState2ImageSet(), naMakeSize(COLUMN2_HALF_WIDTH, 2 * UI_ELEMENT_HEIGTH));
  naSetButtonState(con->imageStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->imageStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->space, con->imageStateButtonDisabled2, naMakePos(TAB2_1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;
  
  con->submitLabel = naNewLabel("Submit (Enter)", COLUMN0_WIDTH);
  naSetLabelFont(con->submitLabel, titleFont);
  naAddSpaceChild(con->space, con->submitLabel, naMakePos(20, curPosY));

  con->textButtonSubmit = naNewTextPushButton("Ok Button", COLUMN1_WIDTH);
  naAddUIReaction(con->textButtonSubmit, NA_UI_COMMAND_PRESSED, submitPressed, con);
  naAddSpaceChild(con->space, con->textButtonSubmit, naMakePos(TAB1, curPosY));
//  naSetButtonSubmit(con->textButtonSubmit, submitPressed, con);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->abortLabel = naNewLabel("Abort (Esc)", COLUMN0_WIDTH);
  naSetLabelFont(con->abortLabel, titleFont);
  naAddSpaceChild(con->space, con->abortLabel, naMakePos(20, curPosY));

  con->textButtonAbort = naNewTextPushButton("Cancel Button", COLUMN1_WIDTH);
  naAddUIReaction(con->textButtonAbort, NA_UI_COMMAND_PRESSED, abortPressed, con);
  naAddSpaceChild(con->space, con->textButtonAbort, naMakePos(TAB1, curPosY));
//  naSetButtonAbort(con->textButtonAbort, abortPressed, con);

  curPosY = curPosY - 60;

  con->outputLabel = naNewLabel( "Here will be the output of any operation.", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  NAFont* monospaceFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(con->outputLabel, monospaceFont);
  naRelease(monospaceFont);
  naAddSpaceChild(con->space, con->outputLabel, naMakePos(20, curPosY));



  naRelease(titleFont);

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
