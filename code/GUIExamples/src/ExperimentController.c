
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "GUIExamples.h"
#include "Layout.h"
#include "NAVisual/NA3DHelper.h"
#include "NAVisual/NAPNG.h"


struct ExperimentController{
  NAWindow* window;

  NASpace* buttonSpace;

  NAButton* applicationButton;
  NAButton* screenButton;
  NAButton* windowButton;
  NAButton* spaceButton;

  NAButton* buttonButton;
  NAButton* fontButton;

  NASpace* experimentSpace;

  ScreenController* screenController;
  WindowController* windowController;
  ButtonController* buttonController;
  FontController* fontController;
  
  
  

  NALabel* enabledLabel;
  NALabel* disabledLabel;

  NALabel* labelLabel;
  NALabel* label;
  NALabel* labelDisabled;
  NALabel* labelColor;
  NALabel* labelColorDisabled;

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

  NALabel* textFieldLabel;
  NATextField* textField;
  NATextField* textFieldDisabled;

  NALabel* textBoxLabel;
  NATextBox* textBox;

  NALabel* menuLabel;
  NAButton* menuButton;
  NAMenu* menu;

  NALabel* imageSpaceLabel;
  NAImageSpace* imageSpace;

  size_t fontId;
  NALabel* openGLSpaceLabel;
  NAOpenGLSpace* openGLSpace;
  int openGLSpaceRefreshCount;

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

void updateExperimentController(ExperimentController* con);



//void windowReshaped(NAReaction reaction){
//  ExperimentController* con = reaction.controller;
//  NARect rect = naGetUIElementRect(con->window);
//  NARect borderRect = naGetUIElementRect(con->window);
//  const NAUTF8Char* labelString = naAllocSprintf(NA_TRUE, "Window reshaped.\nRect with border:    %.01f, %.01f, %.01f, %.01f\nRect without border: %.01f, %.01f, %.01f, %.01f", rect.pos.x, rect.pos.y, rect.size.width, rect.size.height, borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
//  naSetLabelText(con->outputLabel, labelString);
//}

void checkBoxPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "CheckBox Pressed");
}

static void initOpenGL(void* initData) {
  ExperimentController* con = (ExperimentController*)initData;
  con->fontId = naStartupPixelFont();
}

void redrawOpenGLSpace(NAReaction reaction){
  // OpenGL is declared deprecated on macOS 10.14. These pragma directives
  // omit the nasty warnings. Do not forget the pragma pop at the end of this
  // function!
  #if NA_OS == NA_OS_MAC_OS_X
    #pragma GCC diagnostic push 
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  #endif

  ExperimentController* con = reaction.controller;
  con->openGLSpaceRefreshCount--;

  // Prepare the next refresh.
  if(con->openGLSpaceRefreshCount == 0){
    naRefreshUIElement(con->openGLSpace, 1./ 60);
    con->openGLSpaceRefreshCount++;
  }

  static float ang = 0.f;

  ang += .01f;
  if(ang > NA_PI2f){ang = 0.f;}


  double uiScale = naGetUIElementResolutionScale(con->openGLSpace);
  NASize viewSize = naGetUIElementRect(reaction.uiElement).size;
  glViewport(
    0,
    0,
    (GLsizei)(viewSize.width * uiScale),
    (GLsizei)(viewSize.height * uiScale));

  glClearColor(0.f, 0.f, .4f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_TEXTURE_2D);
  // Enable blending to render pixel font properly
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(1.f, .5f, 1.f, 1.f);
  glPointSize(5);
  glBegin(GL_POINTS);
    glVertex3f(naSinf(ang) * .9f, 0.f, 0.f);
  glEnd();

  naDrawASCIICharacters(con->fontId, "Hello World", (double)naSinf(ang) * .9, 0, 0, uiScale);
   
  naSwapOpenGLSpaceBuffer(con->openGLSpace);
  
  #if NA_OS == NA_OS_MAC_OS_X
    #pragma GCC diagnostic pop
  #endif
}

void radioPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Radio Pressed");
}

void sliderEdited(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NASlider* slider = reaction.uiElement;
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "Slider Value Edited to %f", naGetSliderValue(slider));
  naSetLabelText(con->outputLabel, outputText);
}

void textFieldEdited(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "TextField Value Edited to %s", naGetStringUTF8Pointer(textFieldString));
  naSetLabelText(con->outputLabel, outputText);
  naDelete(textFieldString);
}

void textFieldEditFinished(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "TextField Value Finished Editing to %s", naGetStringUTF8Pointer(textFieldString));
  naSetLabelText(con->outputLabel, outputText);
  naDelete(textFieldString);
}

void menuButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  
  NARect rect = naGetUIElementRectAbsolute(con->menuButton);
  NAPos menuPos = rect.pos;
  menuPos.x += rect.size.width;
  menuPos.y += rect.size.height;

  naSetLabelText(con->outputLabel, "Menu button pressed");
  naPresentMenu(con->menu, menuPos, con->menuButton);
}

void selectItemSelected(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "Select item with index %d selected",
    (int)naGetSelectItemIndex(con->select, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);
}

void menuItemSelected(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "MenuItem with index %d selected",
    (int)naGetMenuItemIndex(con->menu, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);
}

void menuItemKeyboardSelected(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "MenuItem with index %d selected by keyboard shortcut",
    (int)naGetMenuItemIndex(con->menu, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);
}



static void pressButton(NAReaction reaction){
  ExperimentController* con = (ExperimentController*)reaction.controller;
  NASpace* newSpace = NA_NULL;

  if(reaction.uiElement == con->screenButton){
    if(!con->screenController)
      con->screenController = createScreenController();
    updateScreenController(con->screenController);
    newSpace = getScreenControllerSpace(con->screenController);
  }else if(reaction.uiElement == con->windowButton){
    if(!con->windowController)
      con->windowController = createWindowController();
    updateWindowController(con->windowController);
    newSpace = getWindowControllerSpace(con->windowController);
  }else if(reaction.uiElement == con->buttonButton){
    if(!con->buttonController)
      con->buttonController = createButtonController();
    updateButtonController(con->buttonController);
    newSpace = getButtonControllerSpace(con->buttonController);
  }else if(reaction.uiElement == con->fontButton){
    if(!con->fontController)
      con->fontController = createFontController();
    updateFontController(con->fontController);
    newSpace = getFontControllerSpace(con->fontController);
  }else if(reaction.uiElement == con->applicationButton){
    showNAApplication();
  }else if(reaction.uiElement == con->converterButton){
    spawnTemperatureController();
  }else if(reaction.uiElement == con->quitButton){
    naStopApplication();
  }
  
  NASpace* contentSpace = naGetWindowContentSpace(con->window);
  if(con->experimentSpace) {
    naRemoveSpaceChild(contentSpace, con->experimentSpace);
  }
  if(newSpace) {
    naAddSpaceChild(contentSpace, newSpace, naMakePos(0, 0));
  }
  con->experimentSpace = newSpace;

  updateExperimentController(con);
}

ExperimentController* createExperimentController(){

  double descSize = 120;
  double left = 160;
  double left2 = 380;

  ExperimentController* con = naAlloc(ExperimentController);

  con->screenController = NA_NULL;
  con->buttonController = NA_NULL;
  con->fontController = NA_NULL;

  con->window = naNewWindow(
    "Experiment with the NALib UI elements",
    naMakeRectS(150, 150, WINDOW_WIDTH, WINDOW_HEIGTH),
    0);
//  naAddUIReaction(con->window, NA_UI_COMMAND_RESHAPE, windowReshaped, con);

  con->experimentSpace = NA_NULL;

  double curPosY = WINDOW_HEIGTH;

  con->buttonSpace = naNewSpace(naMakeSize(WINDOW_WIDTH, BUTTON_SPACE_HEIGHT));
  naSetSpaceAlternateBackground(con->buttonSpace, NA_TRUE);

  double curButtonPosY = BUTTON_SPACE_HEIGHT - WINDOW_MARGIN - UI_ELEMENT_HEIGTH;
  
  con->applicationButton = naNewTextPushButton("NAApplication", SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->applicationButton,
    naMakePos(WINDOW_MARGIN, curButtonPosY - 0 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->applicationButton,
    NA_UI_COMMAND_PRESSED,
    pressButton,
    con);
  
  con->screenButton = naNewTextStateButton("NAScreen", NA_NULL, SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->screenButton,
    naMakePos(WINDOW_MARGIN, curButtonPosY - 1 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->screenButton,
    NA_UI_COMMAND_PRESSED,
    pressButton,
    con);
  
  con->windowButton = naNewTextStateButton("NAWindow", NA_NULL, SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->windowButton,
    naMakePos(WINDOW_MARGIN, curButtonPosY - 2 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->windowButton,
    NA_UI_COMMAND_PRESSED,
    pressButton,
    con);
  
  con->spaceButton = naNewTextPushButton("NASpace", SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->spaceButton,
    naMakePos(WINDOW_MARGIN, curButtonPosY - 3 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->spaceButton,
    NA_UI_COMMAND_PRESSED,
    pressButton,
    con);



  con->buttonButton = naNewTextStateButton("NAButton", NA_NULL, SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->buttonButton,
    naMakePos(WINDOW_MARGIN + SMALL_BUTTON_WIDTH + BUTTON_MARGIN, curButtonPosY - 0 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->buttonButton,
    NA_UI_COMMAND_PRESSED, 
    pressButton,
    con);



  con->fontButton = naNewTextStateButton("NAFont", NA_NULL, SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->fontButton,
    naMakePos(WINDOW_MARGIN + 2 * SMALL_BUTTON_WIDTH + 2 * BUTTON_MARGIN, curButtonPosY - 0 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->fontButton,
    NA_UI_COMMAND_PRESSED,
    pressButton,
    con);



  con->converterButton = naNewTextPushButton("Temp. Converter", SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->converterButton,
    naMakePos(WINDOW_MARGIN + 3 * SMALL_BUTTON_WIDTH + 3 * BUTTON_MARGIN, curButtonPosY - 0 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->converterButton,
    NA_UI_COMMAND_PRESSED,
    pressButton,
    con);

  con->quitButton = naNewTextPushButton("Quit", SMALL_BUTTON_WIDTH);
  naAddSpaceChild(
    con->buttonSpace,
    con->quitButton,
    naMakePos(WINDOW_MARGIN + 3 * SMALL_BUTTON_WIDTH + 3 * BUTTON_MARGIN, curButtonPosY - 1 * UI_ELEMENT_HEIGTH));
  naAddUIReaction(
    con->quitButton,
    NA_UI_COMMAND_PRESSED,
    pressButton,
    con);



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
  con->labelLabel = naNewLabel("NALabel", descSize);
  naAddSpaceChild(contentSpace, con->labelLabel, naMakePos(20, curPosY));
  con->label = naNewLabel("Normal Label", 100);
  naAddSpaceChild(contentSpace, con->label, naMakePos(left, curPosY));
  con->labelColor = naNewLabel("Colored Label", 100);
  naAddSpaceChild(contentSpace, con->labelColor, naMakePos(left + 100, curPosY));
  
  con->labelDisabled = naNewLabel("Disabled Label", 100);
  naSetLabelEnabled(con->labelDisabled, NA_FALSE);
  con->labelColorDisabled = naNewLabel("Disabled Color Label", 100);
  naSetLabelEnabled(con->labelColorDisabled, NA_FALSE);
  naAddSpaceChild(contentSpace, con->labelDisabled, naMakePos(left2, curPosY));
  naAddSpaceChild(contentSpace, con->labelColorDisabled, naMakePos(left2 + 100, curPosY));

  NAColor textColor;
  naFillColorWithSRGB(&textColor, .1f, .2f, 1.f, 1.f);
  naSetLabelTextColor(con->labelColor, &textColor);
  naSetLabelTextColor(con->labelColorDisabled, &textColor);

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

  curPosY -= 30;
  con->textFieldLabel = naNewLabel("NATextField", descSize);
  naAddSpaceChild(contentSpace, con->textFieldLabel, naMakePos(20, curPosY));
  con->textField = naNewTextField(200);
  naAddSpaceChild(contentSpace, con->textField, naMakePos(left, curPosY));
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDITED, textFieldEdited, con);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, textFieldEditFinished, con);
  con->textFieldDisabled = naNewTextField(200);
  naSetTextFieldEnabled(con->textFieldDisabled, NA_FALSE);
  naSetTextFieldText(con->textFieldDisabled, "Disabled Textfield");
  naAddSpaceChild(contentSpace, con->textFieldDisabled, naMakePos(left2, curPosY));

  curPosY -= 50;
  con->textBoxLabel = naNewLabel("NATextBox", descSize);
  naAddSpaceChild(contentSpace, con->textBoxLabel, naMakePos(20, curPosY));
  con->textBox = naNewTextBox(naMakeSize(200, 50));
  naAddSpaceChild(contentSpace, con->textBox, naMakePos(left, curPosY - 20));

  curPosY -= 50;
  con->menuLabel = naNewLabel("NAMenu", descSize);
  naAddSpaceChild(contentSpace, con->menuLabel, naMakePos(20, curPosY));
  con->menuButton = naNewTextPushButton("Push for Menu", 200);
  naAddUIReaction(con->menuButton, NA_UI_COMMAND_PRESSED, menuButtonPressed, con);
  naAddSpaceChild(contentSpace, con->menuButton, naMakePos(left, curPosY));
  con->menu = naNewMenu();  
  NAMenuItem* menuItem0 = naNewMenuItem("You are Winner", NA_NULL);
  NAMenuItem* menuItem1 = naNewMenuItem("Kohle, Kohle, Kohle", NA_NULL);
  NAMenuItem* menuItem2 = naNewMenuItem("I am Groot", NA_NULL);
  NAMenuItem* menuItem3 = naNewMenuItem("None of that Objective-C rubbish", NA_NULL);
  NAMenuItem* menuItem4 = naNewMenuItem("Bread crumbs and beaver spit", NA_NULL);
  NAMenuItem* menuSeparator = naNewMenuSeparator();
  naAddMenuItem(con->menu, menuItem0, NA_NULL);
  naAddMenuItem(con->menu, menuItem1, NA_NULL);
  naAddMenuItem(con->menu, menuItem2, menuItem0);
  naAddMenuItem(con->menu, menuItem3, NA_NULL);
  naAddMenuItem(con->menu, menuItem4, NA_NULL);
  naAddMenuItem(con->menu, menuSeparator, menuItem3);
  naAddUIReaction(menuItem0, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem1, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem2, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem3, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem4, NA_UI_COMMAND_PRESSED, menuItemSelected, con);

  curPosY -= 30;
  con->imageSpaceLabel = naNewLabel("NAImageSpace", descSize);
  naAddSpaceChild(contentSpace, con->imageSpaceLabel, naMakePos(20, curPosY));
  con->imageSpace = naNewImageSpace(getIconImageSet(), naMakeSize(200, 22));
  naAddSpaceChild(contentSpace, con->imageSpace, naMakePos(left, curPosY));

  curPosY -= 30;
  con->openGLSpaceLabel = naNewLabel("NAOpenGLSpace", descSize);
  naAddSpaceChild(contentSpace, con->openGLSpaceLabel, naMakePos(20, curPosY));
  con->openGLSpace = naNewOpenGLSpace(naMakeSize(200, 22), initOpenGL, con);
  naAddSpaceChild(contentSpace, con->openGLSpace, naMakePos(left, curPosY));
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, redrawOpenGLSpace, con);
  con->openGLSpaceRefreshCount = 1;

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


void updateExperimentController(ExperimentController* con){
  naSetButtonState(
    con->screenButton,
    con->screenController && con->experimentSpace == getScreenControllerSpace(con->screenController));
  naSetButtonState(
    con->windowButton,
    con->windowController && con->experimentSpace == getWindowControllerSpace(con->windowController));
  naSetButtonState(
    con->buttonButton,
    con->buttonController && con->experimentSpace == getButtonControllerSpace(con->buttonController));
  naSetButtonState(
    con->fontButton,
    con->fontController && con->experimentSpace == getFontControllerSpace(con->fontController));
}


void clearExperimentController(ExperimentController* con){
  // Note that all UI elements which are attached in some way to the root
  // application UIElement will be cleared automatically.
  if(con->screenController) { clearScreenController(con->screenController); }
  if(con->buttonController) { clearButtonController(con->buttonController); }
  if(con->fontController) { clearFontController(con->fontController); }
  naDelete(con->menu);
  naShutdownPixelFont(con->fontId);

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
