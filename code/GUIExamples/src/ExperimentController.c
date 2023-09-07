
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "GUIExamples.h"
#include "../../NALib/src/NAVisual/NA3DHelper.h"
#include "../../NALib/src/NAVisual/NAPNG.h"


struct ExperimentController{
  NAWindow* experimentWindow;
  NASpace* contentSpace;

  NALabel* buttonLabel;
  NAButton* buttonButton;

  NALabel* fontLabel;
  NAButton* fontButton;

  NALabel* enabledLabel;
  NALabel* disabledLabel;

  NALabel* labelLabel;
  NALabel* label;
  NALabel* labelDisabled;

  NALabel* popupButtonLabel;
  NAPopupButton* popupButton;
  NAPopupButton* popupButtonDisabled;

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
  NAMenuItem* menuItem0;
  NAMenuItem* menuItem1;
  NAMenuItem* menuItem2;
  NAMenuItem* menuItem3;
  NAMenuItem* menuItem4;
  NAMenuItem* menuSeparator;

  NALabel* imageSpaceLabel;
  NAImageSpace* imageSpace;

  NAInt fontId;
  NALabel* openGLSpaceLabel;
  NAOpenGLSpace* openGLSpace;
  int openGLSpaceRefreshCount;

  NASpace* subSpace;
  NALabel* subSpaceLabel;

  NAButton* converterButton;
  NAButton* quitButton;

  int textOption;
  int imageOption;
  NALabel* outputLabel;
};

void updateExperimentController(ExperimentController* con);



NABool windowReshaped(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  NARect rect = naGetUIElementRect(con->experimentWindow);
  NARect borderRect = naGetUIElementRect(con->experimentWindow);
  const NAUTF8Char* labelString = naAllocSprintf(NA_TRUE, "Window reshaped.\nRect with border:    %.01f, %.01f, %.01f, %.01f\nRect without border: %.01f, %.01f, %.01f, %.01f", rect.pos.x, rect.pos.y, rect.size.width, rect.size.height, borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->outputLabel, labelString);

  return NA_TRUE;
}

NABool checkBoxPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "CheckBox Pressed");

  return NA_TRUE;
}

void initOpenGL(void* initData)
{
  ExperimentController* con = (ExperimentController*)initData;
  con->fontId = naStartupPixelFont();
}

NABool redrawOpenGLSpace(NAReaction reaction){
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

  ang += .05f;
  if(ang > NA_PI2f){ang = 0.f;}


  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  NASize viewSize = naGetUIElementRect(reaction.uiElement).size;
  glViewport(
    0,
    0,
    (GLsizei)(viewSize.width * uiScale),
    (GLsizei)(viewSize.height * uiScale));

  glClearColor(0.f, 0.f, .4f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_TEXTURE_2D);

  glColor4f(1.f, .5f, 1.f, 1.f);
  glPointSize(5);
  glBegin(GL_POINTS);
    glVertex3f(naSinf(ang) * .9f, 0.f, 0.f);
  glEnd();

  naDrawASCIICharacters(con->fontId, "Hello World", (double)naSinf(ang) * .9, 0, 0);
   
  naSwapOpenGLSpaceBuffer(con->openGLSpace);
  

  return NA_TRUE;

  #if NA_OS == NA_OS_MAC_OS_X
    #pragma GCC diagnostic pop
  #endif
}

NABool radioPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Radio Pressed");

  return NA_TRUE;
}

NABool sliderEdited(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NASlider* slider = reaction.uiElement;
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "Slider Value Edited to %f", naGetSliderValue(slider));
  naSetLabelText(con->outputLabel, outputText);

  return NA_TRUE;
}

NABool textFieldEdited(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "TextField Value Edited to %s", naGetStringUTF8Pointer(textFieldString));
  naSetLabelText(con->outputLabel, outputText);
  naDelete(textFieldString);

  return NA_TRUE;
}

NABool textFieldEditFinished(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "TextField Value Finished Editing to %s", naGetStringUTF8Pointer(textFieldString));
  naSetLabelText(con->outputLabel, outputText);
  naDelete(textFieldString);

  return NA_TRUE;
}

NABool menuButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  
  NARect rect = naGetUIElementRectAbsolute(con->menuButton);
  NAPos menuPos = rect.pos;
  menuPos.x += rect.size.width;
  menuPos.y += rect.size.height;

  naSetLabelText(con->outputLabel, "Menu button pressed");
  naPresentMenu(con->menu, menuPos, con->menuButton);

  return NA_TRUE;
}

NABool popupButtonItemSelected(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "PopupButton item with index %d selected",
    (int)naGetPopupButtonItemIndex(con->popupButton, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);

  return NA_TRUE;
}

NABool menuItemSelected(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "MenuItem with index %d selected",
    (int)naGetMenuItemIndex(con->menu, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);

  return NA_TRUE;
}

NABool menuItemKeyboardSelected(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "MenuItem with index %d selected by keyboard shortcut", (int)naGetMenuItemIndex(con->menu, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);

  return NA_TRUE;
}

NABool pressExperimentButton(NAReaction reaction){
  ExperimentController* con = (ExperimentController*)reaction.controller;

  if(reaction.uiElement == con->fontButton){
    showFonts();
  }else if(reaction.uiElement == con->buttonButton){
    showButtons();
  }else if(reaction.uiElement == con->converterButton){
    createTemperatureController();
  }else if(reaction.uiElement == con->quitButton){
    naStopApplication();
  }

  return NA_TRUE;
}

ExperimentController* createExperimentController(){
  double windowWidth = 600;
  double windowHeight = 700;

  double descSize = 120;
  double left = 160;
  double left2 = 380;

  ExperimentController* con = naAlloc(ExperimentController);

  con->experimentWindow = naNewWindow(
    "Experiment",
    naMakeRectS(150, 150, windowWidth, windowHeight),
    NA_TRUE,
    0);

  con->contentSpace = naNewSpace(naMakeSize(windowWidth, windowHeight));
  naAddUIReaction(con->experimentWindow, NA_UI_COMMAND_RESHAPE, windowReshaped, con);

  double curPosY = windowHeight - 42;

  con->buttonLabel = naNewLabel("NAButton", descSize);
  naAddSpaceChild(con->contentSpace, con->buttonLabel, naMakePos(20, curPosY));
  con->buttonButton = naNewTextButton("Show Buttons window...", 200, NA_BUTTON_PUSH | NA_BUTTON_BORDERED);
  naAddSpaceChild(con->contentSpace, con->buttonButton, naMakePos(left, curPosY));
  naAddUIReaction(con->buttonButton, NA_UI_COMMAND_PRESSED, pressExperimentButton, con);

  curPosY -= 30;
  con->fontLabel = naNewLabel("NAFont", descSize);
  naAddSpaceChild(con->contentSpace, con->fontLabel, naMakePos(20, curPosY));
  con->fontButton = naNewTextButton("Show Fonts window...", 200, NA_BUTTON_PUSH | NA_BUTTON_BORDERED);
  naAddSpaceChild(con->contentSpace, con->fontButton, naMakePos(left, curPosY));
  naAddUIReaction(con->fontButton, NA_UI_COMMAND_PRESSED, pressExperimentButton, con);

  curPosY -= 50;
  con->enabledLabel = naNewLabel("Enabled", 200);
  naSetLabelTextAlignment(con->enabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(con->contentSpace, con->enabledLabel, naMakePos(left, curPosY));
  con->disabledLabel = naNewLabel("Disabled", 200);
  naSetLabelTextAlignment(con->disabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(con->contentSpace, con->disabledLabel, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->labelLabel = naNewLabel("NALabel", descSize);
  naAddSpaceChild(con->contentSpace, con->labelLabel, naMakePos(20, curPosY));
  con->label = naNewLabel("I am a Label", 200);
  naAddSpaceChild(con->contentSpace, con->label, naMakePos(left, curPosY));
  con->labelDisabled = naNewLabel("I am a disabled Label", 200);
  naSetLabelEnabled(con->labelDisabled, NA_FALSE);
  naAddSpaceChild(con->contentSpace, con->labelDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->popupButtonLabel = naNewLabel("NAPopupButton", descSize);
  naAddSpaceChild(con->contentSpace, con->popupButtonLabel, naMakePos(20, curPosY));
  con->popupButton = naNewPopupButton(200);
  naAddSpaceChild(con->contentSpace, con->popupButton, naMakePos(left, curPosY));
  for(size_t i = 0; i < 5; ++i){
    NAMenuItem* item = naNewMenuItem(naAllocSprintf(NA_TRUE, "Popup menu item %d", i));
    naAddPopupButtonMenuItem(con->popupButton, item, NA_NULL);
    naAddUIReaction(item, NA_UI_COMMAND_PRESSED, popupButtonItemSelected, con);
  }
  naSetPopupButtonIndexSelected(con->popupButton, 3);

  con->popupButtonDisabled = naNewPopupButton(200);
  naSetPopupButtonEnabled(con->popupButtonDisabled, NA_FALSE);
  NAMenuItem* disabledItem = naNewMenuItem(naAllocSprintf(NA_TRUE, "Disabled Popup Item", 0));
  naAddPopupButtonMenuItem(con->popupButtonDisabled, disabledItem, NA_NULL);
  naAddSpaceChild(con->contentSpace, con->popupButtonDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->checkBoxLabel = naNewLabel("NACheckBox", descSize);
  naAddSpaceChild(con->contentSpace, con->checkBoxLabel, naMakePos(20, curPosY));
  con->checkBox = naNewCheckBox("I am a CheckBox", 200);
  naAddSpaceChild(con->contentSpace, con->checkBox, naMakePos(left, curPosY));
  naAddUIReaction(con->checkBox, NA_UI_COMMAND_PRESSED, checkBoxPressed, con);
  con->checkBoxDisabled = naNewCheckBox("I am a disabled CheckBox", 200);
  naSetCheckBoxEnabled(con->checkBoxDisabled, NA_FALSE);
  naAddSpaceChild(con->contentSpace, con->checkBoxDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->radioLabel = naNewLabel("NARadio", descSize);
  naAddSpaceChild(con->contentSpace, con->radioLabel, naMakePos(20, curPosY));
  con->radio = naNewRadio("I am a Radio", 200);
  naAddSpaceChild(con->contentSpace, con->radio, naMakePos(left, curPosY));
  naAddUIReaction(con->radio, NA_UI_COMMAND_PRESSED, radioPressed, con);
  con->radioDisabled = naNewRadio("I am a disabled Radio", 200);
  naSetRadioEnabled(con->radioDisabled, NA_FALSE);
  naAddSpaceChild(con->contentSpace, con->radioDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->sliderLabel = naNewLabel("NASlider", descSize);
  naAddSpaceChild(con->contentSpace, con->sliderLabel, naMakePos(20, curPosY));
  con->slider = naNewSlider(200);
  naAddSpaceChild(con->contentSpace, con->slider, naMakePos(left, curPosY));
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, sliderEdited, con);
  con->sliderDisabled = naNewSlider(200);
  naSetSliderEnabled(con->sliderDisabled, NA_FALSE);
  naAddSpaceChild(con->contentSpace, con->sliderDisabled, naMakePos(left2, curPosY));

  curPosY -= 30;
  con->textFieldLabel = naNewLabel("NATextField", descSize);
  naAddSpaceChild(con->contentSpace, con->textFieldLabel, naMakePos(20, curPosY));
  con->textField = naNewTextField(200);
  naAddSpaceChild(con->contentSpace, con->textField, naMakePos(left, curPosY));
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDITED, textFieldEdited, con);
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDIT_FINISHED, textFieldEditFinished, con);
  con->textFieldDisabled = naNewTextField(200);
  naSetTextFieldEnabled(con->textFieldDisabled, NA_FALSE);
  naSetTextFieldText(con->textFieldDisabled, "Disabled Textfield");
  naAddSpaceChild(con->contentSpace, con->textFieldDisabled, naMakePos(left2, curPosY));

  curPosY -= 50;
  con->textBoxLabel = naNewLabel("NATextBox", descSize);
  naAddSpaceChild(con->contentSpace, con->textBoxLabel, naMakePos(20, curPosY));
  con->textBox = naNewTextBox(naMakeSize(200, 50));
  naAddSpaceChild(con->contentSpace, con->textBox, naMakePos(left, curPosY - 20));

  curPosY -= 50;
  con->menuLabel = naNewLabel("NAMenu", descSize);
  naAddSpaceChild(con->contentSpace, con->menuLabel, naMakePos(20, curPosY));
  con->menuButton = naNewTextButton("Push for Menu", 200, 0);
  naAddUIReaction(con->menuButton, NA_UI_COMMAND_PRESSED, menuButtonPressed, con);
  naAddSpaceChild(con->contentSpace, con->menuButton, naMakePos(left, curPosY));
  con->menu = naNewMenu();  
  con->menuItem0 = naNewMenuItem("You are Winner");
  con->menuItem1 = naNewMenuItem("Kohle, Kohle, Kohle");
  con->menuItem2 = naNewMenuItem("I am Groot");
  con->menuItem3 = naNewMenuItem("None of that Objective-C rubbish");
  con->menuItem4 = naNewMenuItem("Bread crumbs and beaver spit");
  con->menuSeparator = naNewMenuSeparator();
  naAddMenuItem(con->menu, con->menuItem0, NA_NULL);
  naAddMenuItem(con->menu, con->menuItem1, NA_NULL);
  naAddMenuItem(con->menu, con->menuItem2, con->menuItem0);
  naAddMenuItem(con->menu, con->menuItem3, NA_NULL);
  naAddMenuItem(con->menu, con->menuItem4, NA_NULL);
  naAddMenuItem(con->menu, con->menuSeparator, con->menuItem3);
  naAddUIReaction(con->menuItem0, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(con->menuItem1, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(con->menuItem2, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(con->menuItem3, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(con->menuItem4, NA_UI_COMMAND_PRESSED, menuItemSelected, con);

  curPosY -= 30;
  con->imageSpaceLabel = naNewLabel("NAImageSpace", descSize);
  naAddSpaceChild(con->contentSpace, con->imageSpaceLabel, naMakePos(20, curPosY));
  con->imageSpace = naNewImageSpace(getState1Image(), naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->imageSpace, naMakePos(left, curPosY));

  curPosY -= 30;
  con->openGLSpaceLabel = naNewLabel("NAOpenGLSpace", descSize);
  naAddSpaceChild(con->contentSpace, con->openGLSpaceLabel, naMakePos(20, curPosY));
  con->openGLSpace = naNewOpenGLSpace(naMakeSize(200, 22), initOpenGL, con);
  naAddSpaceChild(con->contentSpace, con->openGLSpace, naMakePos(left, curPosY));
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, redrawOpenGLSpace, con);
  con->openGLSpaceRefreshCount = 1;

  curPosY -= 60;
  con->subSpace = naNewSpace(naMakeSize(600, 50));
  naSetSpaceAlternateBackground(con->subSpace, NA_TRUE);
  con->subSpaceLabel = naNewLabel("Subspace with alternate background", 520);
  naAddSpaceChild(con->subSpace, con->subSpaceLabel, naMakePos(20, 10));
  naAddSpaceChild(con->contentSpace, con->subSpace, naMakePos(0, curPosY));

  curPosY -= 50;
  con->converterButton = naNewTextButton("Example: Temp. Converter", 200, 0);
  naAddSpaceChild(con->contentSpace, con->converterButton, naMakePos(20, curPosY));
  naAddUIReaction(con->converterButton, NA_UI_COMMAND_PRESSED, pressExperimentButton, con);

  curPosY -= 30;
  con->quitButton = naNewTextButton("Quit", 200, 0);
  naAddSpaceChild(con->contentSpace, con->quitButton, naMakePos(20, curPosY));
  naAddUIReaction(con->quitButton, NA_UI_COMMAND_PRESSED, pressExperimentButton, con);

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    windowWidth - 20);
  NAFont* monospaceFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(con->outputLabel, monospaceFont);
  naRelease(monospaceFont);
  naAddSpaceChild(con->contentSpace, con->outputLabel, naMakePos(10, 10));

  con->textOption = 0;

  naSetWindowContentSpace(con->experimentWindow, con->contentSpace);
  naShowWindow(con->experimentWindow);

  return con;
}


void updateExperimentController(ExperimentController* con){
}


void clearExperimentController(ExperimentController* con){
  // Note that all UI elements which are attached in some way to the root
  // application UIElement will be cleared automatically.
  naFree(con);

  naShutdownPixelFont(con->fontId);
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
