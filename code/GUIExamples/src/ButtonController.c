
#include "GUIExamples.h"
#include "NAUtility/NAString.h"



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



void buttonPressed(NAReaction reaction){
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



void submitPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Submit pressed");
}



void abortPressed(NAReaction reaction){
  ButtonController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Abort pressed");
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
  con->window = naNewWindow("Buttons", windowRect, 0, 0);
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

  con->iconPushButtonLabel = naNewLabel("Icon Push", labelWidth);
  naAddSpaceChild(windowSpace, con->iconPushButtonLabel, naMakePos(20, curPosY));

  con->iconPushButton = naNewIconPushButton(getIconImageSet(), buttonWidth);
  naAddUIReaction(con->iconPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->iconPushButton, naMakePos(left1, curPosY));

  con->iconPushButtonDisabled = naNewIconPushButton(getIconImageSet(), buttonWidth);
  naSetButtonEnabled(con->iconPushButtonDisabled, NA_FALSE);
  naAddUIReaction(con->iconPushButtonDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->iconPushButtonDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;

  con->iconStateButtonLabel = naNewLabel("Icon Two States", labelWidth);
  naAddSpaceChild(windowSpace, con->iconStateButtonLabel, naMakePos(20, curPosY));

  con->iconStateButton = naNewIconStateButton(getIconImageSet(), NA_NULL, buttonWidth);
  naAddUIReaction(con->iconStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->iconStateButton, naMakePos(left1, curPosY));

  con->iconStateButtonDisabled1 = naNewIconStateButton(getIconImageSet(), NA_NULL, buttonWidth / 2);
  naSetButtonEnabled(con->iconStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->iconStateButtonDisabled1, naMakePos(left2, curPosY));

  con->iconStateButtonDisabled2 = naNewIconStateButton(getIconImageSet(), NA_NULL, buttonWidth / 2);
  naSetButtonState(con->iconStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->iconStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->iconStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->iconStateButtonDisabled2, naMakePos(left2 + buttonWidth / 2, curPosY));

  curPosY -= buttonHeight + 5;

  con->imagePushButtonLabel = naNewLabel("Image Push", labelWidth);
  naAddSpaceChild(windowSpace, con->imagePushButtonLabel, naMakePos(20, curPosY + 15));

  con->imagePushButton = naNewImagePushButton(getState1ImageSet(), naMakeSize(buttonWidth, buttonHeight));
  naAddUIReaction(con->imagePushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imagePushButton, naMakePos(left1, curPosY));

  con->imagePushButtonDisabled = naNewImagePushButton(getState1ImageSet(), naMakeSize(buttonWidth, buttonHeight));
  naSetButtonEnabled(con->imagePushButtonDisabled, NA_FALSE);
  naAddUIReaction(con->imagePushButtonDisabled, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imagePushButtonDisabled, naMakePos(left2, curPosY));

  curPosY -= buttonHeight + 5;

  con->imageStateButtonLabel = naNewLabel("Image Two states", labelWidth);
  naAddSpaceChild(windowSpace, con->imageStateButtonLabel, naMakePos(20, curPosY + 15));

  con->imageStateButton = naNewImageStateButton(getState1ImageSet(), getState2ImageSet(), naMakeSize(buttonWidth, buttonHeight));
  naAddUIReaction(con->imageStateButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButton, naMakePos(left1, curPosY));

  con->imageStateButtonDisabled1 = naNewImageStateButton(getState1ImageSet(), NA_NULL, naMakeSize(buttonWidth / 2, buttonHeight));
  naSetButtonEnabled(con->imageStateButtonDisabled1, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled1, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonDisabled1, naMakePos(left2, curPosY));

  con->imageStateButtonDisabled2 = naNewImageStateButton(NA_NULL, getState2ImageSet(), naMakeSize(buttonWidth / 2, buttonHeight));
  naSetButtonState(con->imageStateButtonDisabled2, NA_TRUE);
  naSetButtonEnabled(con->imageStateButtonDisabled2, NA_FALSE);
  naAddUIReaction(con->imageStateButtonDisabled2, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(windowSpace, con->imageStateButtonDisabled2, naMakePos(left2 + buttonWidth / 2, curPosY));

  curPosY -= 30;
  
  con->submitLabel = naNewLabel("Submit (Enter)", labelWidth);
  naAddSpaceChild(windowSpace, con->submitLabel, naMakePos(20, curPosY));

  con->textButtonSubmit = naNewTextPushButton("Ok Button", buttonWidth);
  naAddUIReaction(con->textButtonSubmit, NA_UI_COMMAND_PRESSED, submitPressed, con);
  naAddSpaceChild(windowSpace, con->textButtonSubmit, naMakePos(left1, curPosY));
  naSetButtonSubmit(con->textButtonSubmit, submitPressed, con);

  curPosY -= 30;

  con->abortLabel = naNewLabel("Abort (Esc)", labelWidth);
  naAddSpaceChild(windowSpace, con->abortLabel, naMakePos(20, curPosY));

  con->textButtonAbort = naNewTextPushButton("Cancel Button", buttonWidth);
  naAddUIReaction(con->textButtonAbort, NA_UI_COMMAND_PRESSED, abortPressed, con);
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
