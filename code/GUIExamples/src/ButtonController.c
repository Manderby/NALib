
#include "GUIExamples.h"



struct ButtonController{
  NAWindow* window;
  
  NALabel* enabledLabel;
  NALabel* disabledLabel;

  NALabel* textPushButtonLabel;
  NAButton* textPushButton;
  NAButton* textPushButtonDisabled;

  NALabel* textStateButtonLabel;
  NAButton* textStateButton;
  NAButton* textStateButtonDisabled1;
  NAButton* textStateButtonDisabled2;
  
  NALabel* imagePushButtonBorderedLabel;
  NAButton* imagePushButtonBordered;
  NAButton* imagePushButtonBorderedDisabled;
  
  NALabel* imageStateButtonBorderedLabel;
  NAButton* imageStateButtonBordered;
  NAButton* imageStateButtonBorderedDisabled1;
  NAButton* imageStateButtonBorderedDisabled2;
  
  NALabel* imagePushButtonBorderlessLabel;
  NAButton* imagePushButtonBorderless;
  NAButton* imagePushButtonBorderlessDisabled;
  
  NALabel* imageStateButtonBorderlessLabel;
  NAButton* imageStateButtonBorderless;
  NAButton* imageStateButtonBorderlessDisabled1;
  NAButton* imageStateButtonBorderlessDisabled2;

  NALabel* submitLabel;
  NAButton* textButtonSubmit;
  NALabel* abortLabel;
  NAButton* textButtonAbort;


  NALabel* outputLabel;
};



NABool buttonPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;

  NAString* labelString;

  if(reaction.uiElement == con->textPushButton){
    labelString = naNewStringWithFormat("Text Push Button Pressed");
  }else if(reaction.uiElement == con->textStateButton){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Text State Button Switched to %d", (int)state);
  }else if(reaction.uiElement == con->imagePushButtonBordered){
    labelString = naNewStringWithFormat("Bordered Image Push Button Pressed");
  }else if(reaction.uiElement == con->imagePushButtonBorderless){
    labelString = naNewStringWithFormat("Borderless Image Push Button Pressed");
  }else if(reaction.uiElement == con->imageStateButtonBordered){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Bordered Image State Button Switched to %d", (int)state);
  }else if(reaction.uiElement == con->imageStateButtonBorderless){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Borderless Image State Button Switched to %d", (int)state);
  }else{
    labelString = naNewString();
  }

  naSetLabelText(con->outputLabel, naGetStringUTF8Pointer(labelString));
  naDelete(labelString);

  return NA_TRUE;
}



NABool submitPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Submit pressed");
  return NA_TRUE;
}



NABool abortPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Abort pressed");
  return NA_TRUE;
}



ButtonController* createButtonController(){
  ButtonController* con = naAlloc(ButtonController);

  double windowWidth = 550;
  double windowHeight = 500;

  double labelWidth = 200;
  double left1 = 240;
  double left2 = 400;

  double buttonWidth = 120;
  double buttonHeight = 50;

  NARect windowRect = naMakeRectS(500, 400, windowWidth, windowHeight);
  con->window = naNewWindow("Buttons", windowRect, NA_FALSE, 0);
  NASpace* windowSpace = naGetWindowContentSpace(con->window);

  double curPosY = windowHeight - 42;
  
  con->enabledLabel = naNewLabel("Enabled", buttonWidth);
  naSetLabelTextAlignment(con->enabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(windowSpace, con->enabledLabel, naMakePos(left1, curPosY));

  con->disabledLabel = naNewLabel("Disabled", buttonWidth);
  naSetLabelTextAlignment(con->disabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(windowSpace, con->disabledLabel, naMakePos(left2, curPosY));

  curPosY -= 30;

  con->textPushButtonLabel = naNewLabel("Text Push", labelWidth);
  naAddSpaceChild(windowSpace, con->textPushButtonLabel, naMakePos(20, curPosY));

  con->textPushButton = naNewTextPushButton("Push", buttonWidth);
  naAddUIReaction(con->textPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->textPushButton, naMakePos(left1, curPosY));

  con->textPushButtonDisabled = naNewTextPushButton("Push", buttonWidth);
  naAddUIReaction(con->textPushButtonDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonEnabled(con->textPushButtonDisabled, NA_FALSE);
  naAddSpaceChild(windowSpace, con->textPushButtonDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;

  con->textStateButtonLabel = naNewLabel("Text Two States", labelWidth);
  naAddSpaceChild(windowSpace, con->textStateButtonLabel, naMakePos(20, curPosY));

  con->textStateButton = naNewTextStateButton("Off", "On", buttonWidth);
  naAddUIReaction(con->textStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->textStateButton, naMakePos(left1, curPosY));

  con->textStateButtonDisabled1 = naNewTextStateButton("Off", NA_NULL, buttonWidth / 2);
  naAddUIReaction(con->textStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonEnabled(con->textStateButtonDisabled1, NA_FALSE);
  naAddSpaceChild(windowSpace, con->textStateButtonDisabled1, naMakePos(left2, curPosY));

  con->textStateButtonDisabled2 = naNewTextStateButton(NA_NULL, "On", buttonWidth / 2);
  naAddUIReaction(con->textStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naSetButtonState(con->textStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->textStateButtonDisabled2, NA_FALSE);
  naAddSpaceChild(windowSpace, con->textStateButtonDisabled2, naMakePos(left2 + buttonWidth / 2, curPosY));

  curPosY -= 30;

  con->imagePushButtonBorderedLabel = naNewLabel("Image Push", labelWidth);
  naAddSpaceChild(windowSpace, con->imagePushButtonBorderedLabel, naMakePos(20, curPosY));

  con->imagePushButtonBordered = naNewImagePushButton(getIconImage(), naMakeSize(buttonWidth, 25), NA_TRUE);
  naAddUIReaction(con->imagePushButtonBordered, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imagePushButtonBordered, naMakePos(left1, curPosY));

  con->imagePushButtonBorderedDisabled = naNewImagePushButton(getIconImage(), naMakeSize(buttonWidth, 25), NA_TRUE);
  naSetButtonEnabled(con->imagePushButtonBorderedDisabled, NA_FALSE);
  naAddUIReaction(con->imagePushButtonBorderedDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imagePushButtonBorderedDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;

  con->imagePushButtonBorderlessLabel = naNewLabel("Borderless Image Push", labelWidth);
  naAddSpaceChild(windowSpace, con->imagePushButtonBorderlessLabel, naMakePos(20, curPosY));

  con->imagePushButtonBorderless = naNewImagePushButton(getIconImage(), naMakeSize(buttonWidth, 25), NA_FALSE);
  naAddUIReaction(con->imagePushButtonBorderless, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imagePushButtonBorderless, naMakePos(left1, curPosY));

  con->imagePushButtonBorderlessDisabled = naNewImagePushButton(getIconImage(), naMakeSize(buttonWidth, 25), NA_FALSE);
  naSetButtonEnabled(con->imagePushButtonBorderlessDisabled, NA_FALSE);
  naAddUIReaction(con->imagePushButtonBorderlessDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imagePushButtonBorderlessDisabled, naMakePos(left2, curPosY));

  curPosY -= buttonHeight + 5;

  con->imageStateButtonBorderedLabel = naNewLabel("Bordered Image Two States", labelWidth);
  naAddSpaceChild(windowSpace, con->imageStateButtonBorderedLabel, naMakePos(20, curPosY + 15));

  con->imageStateButtonBordered = naNewImageStateButton(getState1Image(), getState2Image(), naMakeSize(buttonWidth, buttonHeight), NA_TRUE);
  naAddUIReaction(con->imageStateButtonBordered, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonBordered, naMakePos(left1, curPosY));

  con->imageStateButtonBorderedDisabled1 = naNewImageStateButton(getState1Image(), NA_NULL, naMakeSize(buttonWidth / 2, buttonHeight), NA_TRUE);
  naSetButtonEnabled(con->imageStateButtonBorderedDisabled1, NA_FALSE);
  naAddUIReaction(con->imageStateButtonBorderedDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonBorderedDisabled1, naMakePos(left2, curPosY));

  con->imageStateButtonBorderedDisabled2 = naNewImageStateButton(NA_NULL, getState2Image(), naMakeSize(buttonWidth / 2, buttonHeight), NA_TRUE);
  naSetButtonState(con->imageStateButtonBorderedDisabled2, NA_TRUE);
  naSetButtonEnabled(con->imageStateButtonBorderedDisabled2, NA_FALSE);
  naAddUIReaction(con->imageStateButtonBorderedDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonBorderedDisabled2, naMakePos(left2 + buttonWidth / 2, curPosY));

  curPosY -= buttonHeight + 5;

  con->imageStateButtonBorderlessLabel = naNewLabel("Borderless Image Two states", labelWidth);
  naAddSpaceChild(windowSpace, con->imageStateButtonBorderlessLabel, naMakePos(20, curPosY + 15));

  con->imageStateButtonBorderless = naNewImageStateButton(getState1Image(), getState2Image(), naMakeSize(buttonWidth, buttonHeight), NA_FALSE);
  naAddUIReaction(con->imageStateButtonBorderless, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonBorderless, naMakePos(left1, curPosY));

  con->imageStateButtonBorderlessDisabled1 = naNewImageStateButton(getState1Image(), NA_NULL, naMakeSize(buttonWidth / 2, buttonHeight), NA_FALSE);
  naSetButtonEnabled(con->imageStateButtonBorderlessDisabled1, NA_FALSE);
  naAddUIReaction(con->imageStateButtonBorderlessDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonBorderlessDisabled1, naMakePos(left2, curPosY));

  con->imageStateButtonBorderlessDisabled2 = naNewImageStateButton(NA_NULL, getState2Image(), naMakeSize(buttonWidth / 2, buttonHeight), NA_FALSE);
  naSetButtonState(con->imageStateButtonBorderlessDisabled2, NA_TRUE);
  naSetButtonEnabled(con->imageStateButtonBorderlessDisabled2, NA_FALSE);
  naAddUIReaction(con->imageStateButtonBorderlessDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonBorderlessDisabled2, naMakePos(left2 + buttonWidth / 2, curPosY));

  curPosY -= 30;
  
  con->submitLabel = naNewLabel("Submit (Enter)", labelWidth);
  naAddSpaceChild(windowSpace, con->submitLabel, naMakePos(20, curPosY));

  con->textButtonSubmit = naNewTextPushButton("Ok", buttonWidth);
  naAddUIReaction(con->textButtonSubmit, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->textButtonSubmit, naMakePos(left1, curPosY));
  naSetButtonSubmit(con->textButtonSubmit, submitPressed, con);

  curPosY -= 30;

  con->abortLabel = naNewLabel("Abort (Esc)", labelWidth);
  naAddSpaceChild(windowSpace, con->abortLabel, naMakePos(20, curPosY));

  con->textButtonAbort = naNewTextPushButton("Cancel", buttonWidth);
  naAddUIReaction(con->textButtonAbort, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->textButtonAbort, naMakePos(left1, curPosY));
  naSetButtonAbort(con->textButtonAbort, abortPressed, con);

  curPosY -= 60;

  con->outputLabel = naNewLabel( "Here will be the output of any operation.", windowWidth - 40);
  NAFont* monospaceFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(con->outputLabel, monospaceFont);
  naRelease(monospaceFont);
  naAddSpaceChild(windowSpace, con->outputLabel, naMakePos(20, curPosY));

  return con;
}



void clearButtonController(ButtonController* con){
  NA_UNUSED(con);
}



void showButtonController(ButtonController* con){
  naShowWindow(con->window);
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
