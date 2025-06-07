
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "GUIExamples.h"
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

  ApplicationController* applicationController;
  ScreenController* screenController;
  WindowController* windowController;
  SpaceController* spaceController;
  OpenGLSpaceController* openGLSpaceController;
  MetalSpaceController* metalSpaceController;
  ImageSpaceController* imageSpaceController;

  ButtonController* buttonController;
  CheckBoxController* checkBoxController;
  LabelController* labelController;
  MenuController* menuController;
  RadioController* radioController;
  SelectController* selectController;
  SliderController* sliderController;
  TextBoxController* textBoxController;
  TextFieldController* textFieldController;
  
  FontController* fontController;
};

void updateExampleController(ExampleController* con);



//void windowReshaped(NAReaction reaction){
//  ExampleController* con = reaction.controller;
//  NARect rect = naGetUIElementRect(con->window);
//  NARect borderRect = naGetUIElementRect(con->window);
//  const NAUTF8Char* labelString = naAllocSprintf(NA_TRUE, "Window reshaped.\nRect with border:    %.01f, %.01f, %.01f, %.01f\nRect without border: %.01f, %.01f, %.01f, %.01f", rect.pos.x, rect.pos.y, rect.size.width, rect.size.height, borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
//  naSetLabelText(con->outputLabel, labelString);
//}



static void pressButton(NAReaction reaction){
  ExampleController* con = (ExampleController*)reaction.controller;
  NASpace* newSpace = NA_NULL;

  if(reaction.uiElement == con->applicationButton){
    if(!con->applicationController)
      con->applicationController = createApplicationController();
    updateApplicationController(con->applicationController);
    newSpace = getApplicationControllerSpace(con->applicationController);
  }else if(reaction.uiElement == con->screenButton){
    if(!con->screenController)
      con->screenController = createScreenController();
    updateScreenController(con->screenController);
    newSpace = getScreenControllerSpace(con->screenController);
  }else if(reaction.uiElement == con->windowButton){
    if(!con->windowController)
      con->windowController = createWindowController();
    updateWindowController(con->windowController);
    newSpace = getWindowControllerSpace(con->windowController);
  }else if(reaction.uiElement == con->spaceButton){
    if(!con->spaceController)
      con->spaceController = createSpaceController();
    updateSpaceController(con->spaceController);
    newSpace = getSpaceControllerSpace(con->spaceController);
  }else if(reaction.uiElement == con->openGLSpaceButton){
    if(!con->openGLSpaceController)
      con->openGLSpaceController = createOpenGLSpaceController();
    updateOpenGLSpaceController(con->openGLSpaceController);
    newSpace = getOpenGLSpaceControllerSpace(con->openGLSpaceController);
  }else if(reaction.uiElement == con->metalSpaceButton){
    if(!con->metalSpaceController)
      con->metalSpaceController = createMetalSpaceController();
    updateMetalSpaceController(con->metalSpaceController);
    newSpace = getMetalSpaceControllerSpace(con->metalSpaceController);
  }else if(reaction.uiElement == con->imageSpaceButton){
    if(!con->imageSpaceController)
      con->imageSpaceController = createImageSpaceController();
    updateImageSpaceController(con->imageSpaceController);
    newSpace = getImageSpaceControllerSpace(con->imageSpaceController);

  }else if(reaction.uiElement == con->buttonButton){
    if(!con->buttonController)
      con->buttonController = createButtonController();
    updateButtonController(con->buttonController);
    newSpace = getButtonControllerSpace(con->buttonController);
  }else if(reaction.uiElement == con->checkBoxButton){
    if(!con->checkBoxController)
      con->checkBoxController = createCheckBoxController();
    updateCheckBoxController(con->checkBoxController);
    newSpace = getCheckBoxControllerSpace(con->checkBoxController);
  }else if(reaction.uiElement == con->labelButton){
    if(!con->labelController)
      con->labelController = createLabelController();
    updateLabelController(con->labelController);
    newSpace = getLabelControllerSpace(con->labelController);
  }else if(reaction.uiElement == con->menuButton){
    if(!con->menuController)
      con->menuController = createMenuController();
    updateMenuController(con->menuController);
    newSpace = getMenuControllerSpace(con->menuController);
  }else if(reaction.uiElement == con->radioButton){
    if(!con->radioController)
      con->radioController = createRadioController();
    updateRadioController(con->radioController);
    newSpace = getRadioControllerSpace(con->radioController);
  }else if(reaction.uiElement == con->selectButton){
    if(!con->selectController)
      con->selectController = createSelectController();
    updateSelectController(con->selectController);
    newSpace = getSelectControllerSpace(con->selectController);
  }else if(reaction.uiElement == con->sliderButton){
    if(!con->sliderController)
      con->sliderController = createSliderController();
    updateSliderController(con->sliderController);
    newSpace = getSliderControllerSpace(con->sliderController);
  }else if(reaction.uiElement == con->textBoxButton){
    if(!con->textBoxController)
      con->textBoxController = createTextBoxController();
    updateTextBoxController(con->textBoxController);
    newSpace = getTextBoxControllerSpace(con->textBoxController);
  }else if(reaction.uiElement == con->textFieldButton){
    if(!con->textFieldController)
      con->textFieldController = createTextFieldController();
    updateTextFieldController(con->textFieldController);
    newSpace = getTextFieldControllerSpace(con->textFieldController);

  }else if(reaction.uiElement == con->fontButton){
    if(!con->fontController)
      con->fontController = createFontController();
    updateFontController(con->fontController);
    newSpace = getFontControllerSpace(con->fontController);

  }else if(reaction.uiElement == con->converterButton){
    spawnTemperatureController();
  }else if(reaction.uiElement == con->quitButton){
    naStopApplication();
  }
  
  NASpace* contentSpace = naGetWindowContentSpace(con->window);
  if(con->exampleSpace) {
    naRemoveSpaceChild(contentSpace, con->exampleSpace);
  }
  if(newSpace) {
    naAddSpaceChild(contentSpace, newSpace, naMakePos(0, 0));
  }
  con->exampleSpace = newSpace;

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
    naMakeRectS(150, 150, WINDOW_WIDTH, WINDOW_HEIGTH),
    0);
//  naAddUIReaction(con->window, NA_UI_COMMAND_RESHAPE, windowReshaped, con);

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


void updateExampleController(ExampleController* con){
  naSetButtonState(
    con->screenButton,
    con->screenController && con->exampleSpace == getScreenControllerSpace(con->screenController));
  naSetButtonState(
    con->applicationButton,
    con->applicationController && con->exampleSpace == getApplicationControllerSpace(con->applicationController));
  naSetButtonState(
    con->windowButton,
    con->windowController && con->exampleSpace == getWindowControllerSpace(con->windowController));
  naSetButtonState(
    con->spaceButton,
    con->spaceController && con->exampleSpace == getSpaceControllerSpace(con->spaceController));
  naSetButtonState(
    con->openGLSpaceButton,
    con->openGLSpaceController && con->exampleSpace == getOpenGLSpaceControllerSpace(con->openGLSpaceController));
  naSetButtonState(
    con->metalSpaceButton,
    con->metalSpaceController && con->exampleSpace == getMetalSpaceControllerSpace(con->metalSpaceController));
  naSetButtonState(
    con->imageSpaceButton,
    con->imageSpaceController && con->exampleSpace == getImageSpaceControllerSpace(con->imageSpaceController));

  naSetButtonState(
    con->buttonButton,
    con->buttonController && con->exampleSpace == getButtonControllerSpace(con->buttonController));
  naSetButtonState(
    con->checkBoxButton,
    con->checkBoxController && con->exampleSpace == getCheckBoxControllerSpace(con->checkBoxController));
  naSetButtonState(
    con->labelButton,
    con->labelController && con->exampleSpace == getLabelControllerSpace(con->labelController));
  naSetButtonState(
    con->menuButton,
    con->menuController && con->exampleSpace == getMenuControllerSpace(con->menuController));
  naSetButtonState(
    con->radioButton,
    con->radioController && con->exampleSpace == getRadioControllerSpace(con->radioController));
  naSetButtonState(
    con->selectButton,
    con->selectController && con->exampleSpace == getSelectControllerSpace(con->selectController));
  naSetButtonState(
    con->sliderButton,
    con->sliderController && con->exampleSpace == getSliderControllerSpace(con->sliderController));
  naSetButtonState(
    con->textBoxButton,
    con->textBoxController && con->exampleSpace == getTextBoxControllerSpace(con->textBoxController));
  naSetButtonState(
    con->textFieldButton,
    con->textFieldController && con->exampleSpace == getTextFieldControllerSpace(con->textFieldController));
    
  naSetButtonState(
    con->fontButton,
    con->fontController && con->exampleSpace == getFontControllerSpace(con->fontController));
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

  if(con->applicationController) { clearApplicationController(con->applicationController); }
  if(con->screenController) { clearScreenController(con->screenController); }
  if(con->windowController) { clearWindowController(con->windowController); }
  if(con->spaceController) { clearSpaceController(con->spaceController); }
  if(con->openGLSpaceController) { clearOpenGLSpaceController(con->openGLSpaceController); }
  if(con->metalSpaceController) { clearMetalSpaceController(con->metalSpaceController); }
  if(con->imageSpaceController) { clearImageSpaceController(con->imageSpaceController); }

  if(con->buttonController) { clearButtonController(con->buttonController); }
  if(con->checkBoxController) { clearCheckBoxController(con->checkBoxController); }
  if(con->labelController) { clearLabelController(con->labelController); }
  if(con->menuController) { clearMenuController(con->menuController); }
  if(con->radioController) { clearRadioController(con->radioController); }
  if(con->selectController) { clearSelectController(con->selectController); }
  if(con->sliderController) { clearSliderController(con->sliderController); }
  if(con->textBoxController) { clearTextBoxController(con->textBoxController); }
  if(con->textFieldController) { clearTextFieldController(con->textFieldController); }

  if(con->fontController) { clearFontController(con->fontController); }

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
