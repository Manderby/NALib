
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "GUIExamples.h"
#include "Layout.h"
#include "NAVisual/NA3DHelper.h"
#include "NAVisual/NAPNG.h"


struct ExampleController{
  NAWindow* window;

  NASpace* buttonSpace;

  NAButton* applicationButton;
  NAButton* screenButton;
  NAButton* windowButton;
  NAButton* spaceButton;
  NAButton* openGLSpaceButton;
  NAButton* metalSpaceButton;
  NAButton* imageSpaceButton;

  NAButton* buttonButton;
  NAButton* labelButton;
  NAButton* menuButton;
  NAButton* textBoxButton;
  NAButton* textFieldButton;
  
  NAButton* fontButton;

  NASpace* exampleSpace;

  ApplicationController* applicationController;
  ScreenController* screenController;
  WindowController* windowController;
  SpaceController* spaceController;
  OpenGLSpaceController* openGLSpaceController;
  MetalSpaceController* metalSpaceController;
  ImageSpaceController* imageSpaceController;

  ButtonController* buttonController;
  LabelController* labelController;
  MenuController* menuController;
  TextBoxController* textBoxController;
  TextFieldController* textFieldController;
  
  FontController* fontController;
  
  
  

  NALabel* enabledLabel;
  NALabel* disabledLabel;

  NALabel* selectLabel;
  NASelect* select;
  NASelect* selectDisabled;

  NALabel* checkBoxLabel;
  NACheckBox* checkBox;
  NACheckBox* checkBoxDisabled;

  NALabel* radioLabel;
  NARadio* radio;
  NARadio* radioDisabled;

  NALabel* sliderLabel;
  NASlider* slider;
  NASlider* sliderDisabled;

  NASpace* subSpace1;
  NASpace* subSpace2;
  NASpace* subSpace3;
  NASpace* subSpace4;
  NALabel* subSpace1Label;
  NALabel* subSpace2Label;
  NALabel* subSpace3Label;
  NALabel* subSpace4Label;

  NAButton* converterButton;
  NAButton* quitButton;

  int textOption;
  int imageOption;
  NALabel* outputLabel;
};

void updateExampleController(ExampleController* con);



//void windowReshaped(NAReaction reaction){
//  ExampleController* con = reaction.controller;
//  NARect rect = naGetUIElementRect(con->window);
//  NARect borderRect = naGetUIElementRect(con->window);
//  const NAUTF8Char* labelString = naAllocSprintf(NA_TRUE, "Window reshaped.\nRect with border:    %.01f, %.01f, %.01f, %.01f\nRect without border: %.01f, %.01f, %.01f, %.01f", rect.pos.x, rect.pos.y, rect.size.width, rect.size.height, borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
//  naSetLabelText(con->outputLabel, labelString);
//}

void checkBoxPressed(NAReaction reaction){
  ExampleController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "CheckBox Pressed");
}

void radioPressed(NAReaction reaction){
  ExampleController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Radio Pressed");
}

void sliderEdited(NAReaction reaction){
  ExampleController* con = reaction.controller;
  const NASlider* slider = reaction.uiElement;
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "Slider Value Edited to %f", naGetSliderValue(slider));
  naSetLabelText(con->outputLabel, outputText);
}

void selectItemSelected(NAReaction reaction){
  ExampleController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "Select item with index %d selected",
    (int)naGetSelectItemIndex(con->select, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);
}




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
  double curButtonPosY = BUTTON_SPACE_HEIGHT - SPACE_MARGIN - UI_ELEMENT_HEIGTH;

  NAButton* button = naNewTextStateButton(text, NA_NULL, SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    button,
    naMakePos(WINDOW_MARGIN + x * (SMALL_BUTTON_WIDTH + BUTTON_MARGIN), curButtonPosY - y * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    button,
    NA_UI_COMMAND_PRESSED, 
    pressButton,
    con);
    
  return button;
}



ExampleController* createExampleController(){

  double descSize = 120;
  double left = 160;
  double left2 = 380;

  ExampleController* con = naAlloc(ExampleController);

  con->applicationController = NA_NULL;
  con->screenController = NA_NULL;
  con->windowController = NA_NULL;
  con->spaceController = NA_NULL;
  con->openGLSpaceController = NA_NULL;
  con->metalSpaceController = NA_NULL;
  con->imageSpaceController = NA_NULL;

  con->buttonController = NA_NULL;
  con->labelController = NA_NULL;

  con->fontController = NA_NULL;

  con->window = naNewWindow(
    "Examples of the NALib UI elements",
    naMakeRectS(150, 150, WINDOW_WIDTH, WINDOW_HEIGTH),
    0);
//  naAddUIReaction(con->window, NA_UI_COMMAND_RESHAPE, windowReshaped, con);

  con->exampleSpace = NA_NULL;

  double curPosY = WINDOW_HEIGTH;

  con->buttonSpace = naNewSpace(naMakeSize(WINDOW_WIDTH, BUTTON_SPACE_HEIGHT));
  naSetSpaceAlternateBackground(con->buttonSpace, NA_TRUE);

  con->applicationButton = addButton(con, "NAApplication", 0, 0);
  con->screenButton = addButton(con, "NAScreen", 0, 1);
  con->windowButton = addButton(con, "NAWindow", 0, 2);
  con->spaceButton = addButton(con, "NASpace", 0, 3);
  con->openGLSpaceButton = addButton(con, "NAOpenGLSpace", 0, 4);
  con->metalSpaceButton = addButton(con, "NAMetalSpace", 0, 5);
  con->imageSpaceButton = addButton(con, "NAImageSpace", 0, 6);
  
  con->buttonButton = addButton(con, "NAButton", 1, 0);
  con->labelButton = addButton(con, "NALabel", 1, 1);
  con->menuButton = addButton(con, "NAMenu", 1, 2);
  con->textBoxButton = addButton(con, "NATextBox", 1, 3);
  con->textFieldButton = addButton(con, "NATextField", 1, 4);

  con->fontButton = addButton(con, "NAFont", 2, 0);

  con->converterButton = addButton(con, "Temp. Converter", 3, 0);
  con->quitButton = addButton(con, "Quit", 3, 1);

  NASpace* contentSpace = naGetWindowContentSpace(con->window);
  naAddSpaceChild(
    contentSpace,
    con->buttonSpace,
    naMakePos(0, curPosY - BUTTON_SPACE_HEIGHT));



  curPosY = curPosY - BUTTON_SPACE_HEIGHT - WINDOW_MARGIN - UI_ELEMENT_HEIGTH;

  con->enabledLabel = naNewLabel("Enabled", 200);
  naSetLabelTextAlignment(con->enabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(contentSpace, con->enabledLabel, naMakePos(left, curPosY));
  con->disabledLabel = naNewLabel("Disabled", 200);
  naSetLabelTextAlignment(con->disabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(contentSpace, con->disabledLabel, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->selectLabel = naNewLabel("NASelect", descSize);
  naAddSpaceChild(contentSpace, con->selectLabel, naMakePos(20, curPosY));
  con->select = naNewSelect(200);
  naAddSpaceChild(contentSpace, con->select, naMakePos(left, curPosY));
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
  naAddSpaceChild(contentSpace, con->selectDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->checkBoxLabel = naNewLabel("NACheckBox", descSize);
  naAddSpaceChild(contentSpace, con->checkBoxLabel, naMakePos(20, curPosY));
  con->checkBox = naNewCheckBox("I am a CheckBox", 200);
  naAddSpaceChild(contentSpace, con->checkBox, naMakePos(left, curPosY));
  naAddUIReaction(con->checkBox, NA_UI_COMMAND_PRESSED, checkBoxPressed, con);
  con->checkBoxDisabled = naNewCheckBox("I am a disabled CheckBox", 200);
  naSetCheckBoxEnabled(con->checkBoxDisabled, NA_FALSE);
  naAddSpaceChild(contentSpace, con->checkBoxDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->radioLabel = naNewLabel("NARadio", descSize);
  naAddSpaceChild(contentSpace, con->radioLabel, naMakePos(20, curPosY));
  con->radio = naNewRadio("I am a Radio", 200);
  naAddSpaceChild(contentSpace, con->radio, naMakePos(left, curPosY));
  naAddUIReaction(con->radio, NA_UI_COMMAND_PRESSED, radioPressed, con);
  con->radioDisabled = naNewRadio("I am a disabled Radio", 200);
  naSetRadioEnabled(con->radioDisabled, NA_FALSE);
  naAddSpaceChild(contentSpace, con->radioDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->sliderLabel = naNewLabel("NASlider", descSize);
  naAddSpaceChild(contentSpace, con->sliderLabel, naMakePos(20, curPosY));
  con->slider = naNewSlider(200);
  naAddSpaceChild(contentSpace, con->slider, naMakePos(left, curPosY));
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, sliderEdited, con);
  con->sliderDisabled = naNewSlider(200);
  naSetSliderEnabled(con->sliderDisabled, NA_FALSE);
  naAddSpaceChild(contentSpace, con->sliderDisabled, naMakePos(left2, curPosY));

  curPosY -= 60;
  con->subSpace1 = naNewSpace(naMakeSize(300, 30));
  con->subSpace2 = naNewSpace(naMakeSize(300, 30));
  con->subSpace3 = naNewSpace(naMakeSize(300, 30));
  con->subSpace4 = naNewSpace(naMakeSize(300, 30));

  NAColor backColor;
  naFillColorWithSRGB(&backColor, .8f, .5f, .2f, .25f);
  naSetSpaceBackgroundColor(con->subSpace3, &backColor);
  naSetSpaceBackgroundColor(con->subSpace4, &backColor);
  
  naSetSpaceAlternateBackground(con->subSpace2, NA_TRUE);
  naSetSpaceAlternateBackground(con->subSpace4, NA_TRUE);
  con->subSpace1Label = naNewLabel("Subspace with normal background", 520);
  con->subSpace2Label = naNewLabel("Subspace with alternate background", 520);
  con->subSpace3Label = naNewLabel("Subspace with colored background", 520);
  con->subSpace4Label = naNewLabel("Subspace with colored and alternate background", 520);
  naAddSpaceChild(con->subSpace1, con->subSpace1Label, naMakePos(20, 0));
  naAddSpaceChild(con->subSpace2, con->subSpace2Label, naMakePos(20, 0));
  naAddSpaceChild(con->subSpace3, con->subSpace3Label, naMakePos(20, 0));
  naAddSpaceChild(con->subSpace4, con->subSpace4Label, naMakePos(20, 0));
  naAddSpaceChild(contentSpace, con->subSpace1, naMakePos(0, curPosY));
  naAddSpaceChild(contentSpace, con->subSpace2, naMakePos(300, curPosY));
  curPosY -= 30;
  naAddSpaceChild(contentSpace, con->subSpace3, naMakePos(0, curPosY));
  naAddSpaceChild(contentSpace, con->subSpace4, naMakePos(300, curPosY));

  curPosY -= 225;


  curPosY -= 25;


  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  NAFont* monospaceFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(con->outputLabel, monospaceFont);
  naRelease(monospaceFont);
  naAddSpaceChild(contentSpace, con->outputLabel, naMakePos(WINDOW_MARGIN, WINDOW_MARGIN));

  con->textOption = 0;

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
//  naSetButtonState(
//    con->spaceButton,
//    con->spaceController && con->exampleSpace == getSpaceControllerSpace(con->spaceController));
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
    con->labelButton,
    con->labelController && con->exampleSpace == getLabelControllerSpace(con->labelController));
  naSetButtonState(
    con->menuButton,
    con->menuController && con->exampleSpace == getMenuControllerSpace(con->menuController));
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
//  if(con->spaceController) { clearSpaceController(con->spaceController); }
  if(con->openGLSpaceController) { clearOpenGLSpaceController(con->openGLSpaceController); }
  if(con->metalSpaceController) { clearMetalSpaceController(con->metalSpaceController); }
  if(con->imageSpaceController) { clearImageSpaceController(con->imageSpaceController); }

  if(con->buttonController) { clearButtonController(con->buttonController); }
  if(con->labelController) { clearLabelController(con->labelController); }
  if(con->menuController) { clearMenuController(con->menuController); }
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
