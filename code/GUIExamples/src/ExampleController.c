
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "GUIExamples.h"
#include "CommonController.h"
#include "Layout.h"
#include "NAVisual/NA3DHelper.h"
#include "NAVisual/NAPNG.h"


struct ExampleController{
  NAWindow* window;

  NASpace* buttonSpace;
  NASpace* exampleSpace;

  NAButton* applicationButton;
  NAButton* screenButton;
  NAButton* windowButton;
  NAButton* spaceButton;
  NAButton* openGLSpaceButton;
  NAButton* metalSpaceButton;
  NAButton* imageSpaceButton;

  NAButton* buttonButton;
  NAButton* checkBoxButton;
  NAButton* labelButton;
  NAButton* menuButton;
  NAButton* radioButton;
  NAButton* selectButton;
  NAButton* sliderButton;
  NAButton* textBoxButton;
  NAButton* textFieldButton;
  
  NAButton* fontButton;

  NAButton* converterButton;
  NAButton* quitButton;

  CommonController* applicationController;
  CommonController* screenController;
  CommonController* windowController;
  CommonController* spaceController;
  CommonController* openGLSpaceController;
  CommonController* metalSpaceController;
  CommonController* imageSpaceController;

  CommonController* buttonController;
  CommonController* checkBoxController;
  CommonController* labelController;
  CommonController* menuController;
  CommonController* radioController;
  CommonController* selectController;
  CommonController* sliderController;
  CommonController* textBoxController;
  CommonController* textFieldController;
  
  CommonController* fontController;
};

void updateExampleController(ExampleController* con);



static void pressButton(NAReaction reaction){
  ExampleController* con = (ExampleController*)reaction.controller;

  CommonController* newController = NA_NULL;

  if(reaction.uiElement == con->applicationButton){
    if(!con->applicationController)
      con->applicationController = createApplicationController();
    newController = con->applicationController;
  }else if(reaction.uiElement == con->screenButton){
    if(!con->screenController)
      con->screenController = createScreenController();
    newController = con->screenController;
  }else if(reaction.uiElement == con->windowButton){
    if(!con->windowController)
      con->windowController = createWindowController();
    newController = con->windowController;
  }else if(reaction.uiElement == con->spaceButton){
    if(!con->spaceController)
      con->spaceController = createSpaceController();
    newController = con->spaceController;
  }else if(reaction.uiElement == con->openGLSpaceButton){
    if(!con->openGLSpaceController)
      con->openGLSpaceController = createOpenGLSpaceController();
    newController = con->openGLSpaceController;
  }else if(reaction.uiElement == con->metalSpaceButton){
    if(!con->metalSpaceController)
      con->metalSpaceController = createMetalSpaceController();
    newController = con->metalSpaceController;
  }else if(reaction.uiElement == con->imageSpaceButton){
    if(!con->imageSpaceController)
      con->imageSpaceController = createImageSpaceController();
    newController = con->imageSpaceController;

  }else if(reaction.uiElement == con->buttonButton){
    if(!con->buttonController)
      con->buttonController = createButtonController();
    newController = con->buttonController;
  }else if(reaction.uiElement == con->checkBoxButton){
    if(!con->checkBoxController)
      con->checkBoxController = createCheckBoxController();
    newController = con->checkBoxController;
  }else if(reaction.uiElement == con->labelButton){
    if(!con->labelController)
      con->labelController = createLabelController();
    newController = con->labelController;
  }else if(reaction.uiElement == con->menuButton){
    if(!con->menuController)
      con->menuController = createMenuController();
    newController = con->menuController;
  }else if(reaction.uiElement == con->radioButton){
    if(!con->radioController)
      con->radioController = createRadioController();
    newController = con->radioController;
  }else if(reaction.uiElement == con->selectButton){
    if(!con->selectController)
      con->selectController = createSelectController();
    newController = con->selectController;
  }else if(reaction.uiElement == con->sliderButton){
    if(!con->sliderController)
      con->sliderController = createSliderController();
    newController = con->sliderController;
  }else if(reaction.uiElement == con->textBoxButton){
    if(!con->textBoxController)
      con->textBoxController = createTextBoxController();
    newController = con->textBoxController;
  }else if(reaction.uiElement == con->textFieldButton){
    if(!con->textFieldController)
      con->textFieldController = createTextFieldController();
    newController = con->textFieldController;

  }else if(reaction.uiElement == con->fontButton){
    if(!con->fontController)
      con->fontController = createFontController();
    newController = con->fontController;

  }else if(reaction.uiElement == con->converterButton){
    spawnTemperatureController();
  }else if(reaction.uiElement == con->quitButton){
    naStopApplication();
  }

  if(newController) {
    NASpace* contentSpace = naGetWindowContentSpace(con->window);

    if(con->exampleSpace) {
      naRemoveSpaceChild(contentSpace, con->exampleSpace);
    }

    NASpace* newSpace = getCommonControllerSpace(newController);
    naAddSpaceChild(
      contentSpace,
      newSpace,
      naMakePos(0, 0));

    updateCommonController(newController);

    con->exampleSpace = newSpace;
  }

  updateExampleController(con);
}



NAButton* addButton(ExampleController* con, const NAUTF8Char* text, int32 x, int32 y) {
  double curButtonPosY = BUTTON_SPACE_HEIGHT - SPACE_MARGIN_V - UI_ELEMENT_HEIGTH;

  NAButton* button = naNewTextStateButton(text, NA_NULL, BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    button,
    naMakePos(WINDOW_MARGIN + x * (BUTTON_WIDTH + UI_ELEMENT_MARGIN), curButtonPosY - y * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    button,
    NA_UI_COMMAND_PRESSED, 
    pressButton,
    con);
    
  return button;
}



ExampleController* createExampleController(){
  ExampleController* con = naAlloc(ExampleController);

  con->applicationController = NA_NULL;
  con->screenController = NA_NULL;
  con->windowController = NA_NULL;
  con->spaceController = NA_NULL;
  con->openGLSpaceController = NA_NULL;
  con->metalSpaceController = NA_NULL;
  con->imageSpaceController = NA_NULL;

  con->buttonController = NA_NULL;
  con->checkBoxController = NA_NULL;
  con->labelController = NA_NULL;
  con->menuController = NA_NULL;
  con->radioController = NA_NULL;
  con->selectController = NA_NULL;
  con->sliderController = NA_NULL;
  con->textBoxController = NA_NULL;
  con->textFieldController = NA_NULL;

  con->fontController = NA_NULL;

  con->window = naNewWindow(
    "Examples of the NALib UI elements",
    naMakeRectS(WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGTH),
    0);

  con->exampleSpace = NA_NULL;

  con->buttonSpace = naNewSpace(naMakeSize(WINDOW_WIDTH, BUTTON_SPACE_HEIGHT));
  naSetSpaceAlternateBackground(con->buttonSpace, NA_TRUE);

  con->applicationButton = addButton(con, "NAApplication", 0, 0);
  con->screenButton = addButton(con, "NAScreen", 0, 1);
  con->windowButton = addButton(con, "NAWindow", 0, 2);
  con->spaceButton = addButton(con, "NASpace", 0, 5);
  con->openGLSpaceButton = addButton(con, "NAOpenGLSpace", 0, 6);
  con->metalSpaceButton = addButton(con, "NAMetalSpace", 0, 7);
  con->imageSpaceButton = addButton(con, "NAImageSpace", 0, 8);
  
  con->buttonButton = addButton(con, "NAButton", 1, 0);
  con->checkBoxButton = addButton(con, "NACheckBox", 1, 1);
  con->labelButton = addButton(con, "NALabel", 1, 2);
  con->menuButton = addButton(con, "NAMenu", 1, 3);
  con->radioButton = addButton(con, "NARadio", 1, 4);
  con->selectButton = addButton(con, "NASelect", 1, 5);
  con->sliderButton = addButton(con, "NASlider", 1, 6);
  con->textBoxButton = addButton(con, "NATextBox", 1, 7);
  con->textFieldButton = addButton(con, "NATextField", 1, 8);

  con->fontButton = addButton(con, "NAFont", 2, 0);

  con->converterButton = addButton(con, "Temp. Converter", 2, 7);
  con->quitButton = addButton(con, "Quit", 2, 8);

  NASpace* contentSpace = naGetWindowContentSpace(con->window);
  naAddSpaceChild(
    contentSpace,
    con->buttonSpace,
    naMakePos(0, WINDOW_HEIGTH - BUTTON_SPACE_HEIGHT));

  naShowWindow(con->window);

  return con;
}


static void setExampleButtonState(ExampleController* con, NAButton* button, CommonController* comCon) {
  naSetButtonState(
    button,
    comCon && con->exampleSpace == getCommonControllerSpace(comCon));
}

void updateExampleController(ExampleController* con){
  setExampleButtonState(con, con->applicationButton, con->applicationController);
  setExampleButtonState(con, con->screenButton,      con->screenController);
  setExampleButtonState(con, con->windowButton,      con->windowController);
  setExampleButtonState(con, con->spaceButton,       con->spaceController);
  setExampleButtonState(con, con->openGLSpaceButton, con->openGLSpaceController);
  setExampleButtonState(con, con->metalSpaceButton,  con->metalSpaceController);
  setExampleButtonState(con, con->imageSpaceButton,  con->imageSpaceController);

  setExampleButtonState(con, con->buttonButton,      con->buttonController);
  setExampleButtonState(con, con->checkBoxButton,    con->checkBoxController);
  setExampleButtonState(con, con->labelButton,       con->labelController);
  setExampleButtonState(con, con->menuButton,        con->menuController);
  setExampleButtonState(con, con->radioButton,       con->radioController);
  setExampleButtonState(con, con->selectButton,      con->selectController);
  setExampleButtonState(con, con->sliderButton,      con->sliderController);
  setExampleButtonState(con, con->textBoxButton,     con->textBoxController);
  setExampleButtonState(con, con->textFieldButton,   con->textFieldController);

  setExampleButtonState(con, con->fontButton,        con->fontController);
}


void clearExampleController(ExampleController* con){
  // Note that usually, all UI elements which are attached in some way to a
  // window will be cleared automatically.
  // But as the spaces in this example application are added and removed
  // manually, we need to clean them up manually as well.
  
  if(con->exampleSpace) {
    naRemoveSpaceChild(naGetWindowContentSpace(con->window), con->exampleSpace);
    con->exampleSpace = NA_NULL;
  }

  clearCommonController(con->applicationController);
  clearCommonController(con->screenController);
  clearCommonController(con->windowController);
  clearCommonController(con->spaceController);
  clearCommonController(con->openGLSpaceController);
  clearCommonController(con->metalSpaceController);
  clearCommonController(con->imageSpaceController);

  clearCommonController(con->buttonController);
  clearCommonController(con->checkBoxController);
  clearCommonController(con->labelController);
  clearCommonController(con->menuController);
  clearCommonController(con->radioController);
  clearCommonController(con->selectController);
  clearCommonController(con->sliderController);
  clearCommonController(con->textBoxController);
  clearCommonController(con->textFieldController);

  clearCommonController(con->fontController);

  naFree(con);
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
