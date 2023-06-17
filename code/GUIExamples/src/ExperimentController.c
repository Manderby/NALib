
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "GUIExamples.h"
#include "../../NALib/src/NAVisual/NA3DHelper.h"


struct ExperimentController{
  NAWindow* experimentWindow;
  NASpace* contentSpace;

  NAUIImage* testImage;

  NALabel* textButtonLabel;
  NAButton* textPushButton;
  NAButton* textPushButtonState;

  NALabel* imageButtonLabel;
  NAButton* imagePushButton;
  NAButton* imagePushButtonBorderless;
  NAButton* imagePushButtonState;
  NAButton* imagePushButtonStateBorderless;

  NALabel* popupButtonLabel;
  NAPopupButton* popupButton;

  NALabel* checkBoxLabel;
  NACheckBox* checkBox;

  NALabel* imageSpaceLabel;
  NAImageSpace* imageSpace;

  NALabel* labelLabel;
  NALabel* label;

  NAInt fontId;
  NALabel* openGLSpaceLabel;
  NAOpenGLSpace* openGLSpace;
  int openGLSpaceRefreshCount;

  NALabel* radioLabel;
  NARadio* radio;

  NALabel* sliderLabel;
  NASlider* slider;

  NALabel* textBoxLabel;
  NATextBox* textBox;

  NALabel* textFieldLabel;
  NATextField* textField;

  NALabel* menuLabel;
  NAButton* menuButton;
  NAMenu* menu;
  NAMenuItem* menuItem0;
  NAMenuItem* menuItem1;
  NAMenuItem* menuItem2;
  NAMenuItem* menuItem3;
  NAMenuItem* menuItem4;
  NAMenuItem* menuSeparator;

  NASpace* subSpace;

  NAButton* fontButton;
  NAButton* quitButton;

  int textOption;
  int imageOption;
  NALabel* outputLabel;
};

NABool windowReshaped(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  NARect rect = naGetUIElementRect(con->experimentWindow);
  NARect borderRect = naGetUIElementRect(con->experimentWindow);
  const NAUTF8Char* labelString = naAllocSprintf(NA_TRUE, "Window reshaped.\nRect with border:    %.01f, %.01f, %.01f, %.01f\nRect without border: %.01f, %.01f, %.01f, %.01f", rect.pos.x, rect.pos.y, rect.size.width, rect.size.height, borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->outputLabel, labelString);

  return NA_TRUE;
}

NABool buttonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  NAString* labelString;
  
  if(reaction.uiElement == con->textPushButton){
    labelString = naNewStringWithFormat("Text Push Button Pressed");
  //}else if(reaction.uiElement == con->textPushButtonBorderless){
  //  labelString = naNewStringWithFormat("Borderless Text Push Button Pressed");
  }else if(reaction.uiElement == con->textPushButtonState){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Text Stateful Button Switched to %d", (int)state);
  //}else if(reaction.uiElement == con->textPushButtonStateBorderless){
  //  labelString = naNewStringWithFormat("Borderless Text State Button Switched to %d", (int)state);
  }else if(reaction.uiElement == con->imagePushButton){
    labelString = naNewStringWithFormat("Image Push Button Pressed");
  }else if(reaction.uiElement == con->imagePushButtonBorderless){
    labelString = naNewStringWithFormat("Borderless Image Push Button Pressed");
  }else if(reaction.uiElement == con->imagePushButtonState){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Image Stateful Button Switched to %d", (int)state);
  }else if(reaction.uiElement == con->imagePushButtonStateBorderless){
    NABool state = naGetButtonState(reaction.uiElement);
    labelString = naNewStringWithFormat("Borderless Image Stateful Button Switched to %d", (int)state);
  }else{
    labelString = naNewString();
  }
  
  naSetLabelText(con->outputLabel, naGetStringUTF8Pointer(labelString));
  naDelete(labelString);

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

  naDrawASCIICharacters(con->fontId, "Hello World", naSinf(ang) * .9f, 0, 0);
   
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

NABool menuButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  
  NARect rect = naGetUIElementRect(con->menuButton);
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
  }else if(reaction.uiElement == con->quitButton){
    naStopApplication();
  }

  return NA_TRUE;
}

ExperimentController* createExperimentController(){
  double windowWidth = 500;
  double windowHeight = 600;

  double descSize = 190;
  double left = 240;
  double buttonSize = 60;

  ExperimentController* con = naAlloc(ExperimentController);

  NABabyColor mainColor = {1., .25, 0., 1.};
//  NABabyColor altColor = {.25, 0., 1., 1.};
  NABabyImage* mainImage = naCreateBabyImage(naMakeSizei(20, 10), mainColor);
//  NABabyImage* altImage = naCreateBabyImage(naMakeSizei(20, 10), altColor);
  con->testImage = naCreateUIImage(mainImage, NA_UIIMAGE_RESOLUTION_SCREEN_1x, NA_BLEND_ZERO);

  con->experimentWindow = naNewWindow(
    "Experiment",
    naMakeRectS(150, 150, windowWidth, windowHeight),
    NA_TRUE,
    0);

  con->contentSpace = naNewSpace(naMakeSize(windowWidth, windowHeight));
  naAddUIReaction(con->experimentWindow, NA_UI_COMMAND_RESHAPE, windowReshaped, con);

  double curPosY = windowHeight - 42;
  con->textButtonLabel = naNewLabel("NAButton: TextButton", 200);
  naAddSpaceChild(con->contentSpace, con->textButtonLabel, naMakePos(20, curPosY));

  con->textPushButton = naNewTextButton("Push", buttonSize, 0);
  naAddUIReaction(con->textPushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->contentSpace, con->textPushButton, naMakePos(left, curPosY));

  //con->textPushButtonBorderless = naNewTextButton("Push", naMakeSize(buttonSize, 24), NA_BUTTON_BORDERLESS);
  //naAddUIReaction(con->textPushButtonBorderless, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  //naAddSpaceChild(con->contentSpace, con->textPushButtonBorderless, naMakePos(left + 1 * buttonSize, curPosY));

  con->textPushButtonState = naNewTextButton("State", buttonSize, NA_BUTTON_STATEFUL);
  naAddUIReaction(con->textPushButtonState, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->contentSpace, con->textPushButtonState, naMakePos(left + 2 * buttonSize, curPosY));

  //con->textPushButtonStateBorderless = naNewTextButton("State", naMakeSize(buttonSize, 24), NA_BUTTON_BORDERLESS | NA_BUTTON_STATEFUL);
  //naAddUIReaction(con->textPushButtonStateBorderless, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  //naAddSpaceChild(con->contentSpace, con->textPushButtonStateBorderless, naMakePos(left + 3 * buttonSize, curPosY));

  curPosY -= 30;
  con->imageButtonLabel = naNewLabel("NAButton: ImageButton", descSize);
  naAddSpaceChild(con->contentSpace, con->imageButtonLabel, naMakePos(20, curPosY));

  con->imagePushButton = naNewImageButton(con->testImage, naMakeSize(buttonSize, 24), 0);
  naAddUIReaction(con->imagePushButton, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->contentSpace, con->imagePushButton, naMakePos(left, curPosY));

  con->imagePushButtonBorderless = naNewImageButton(con->testImage, naMakeSize(buttonSize, 24), NA_BUTTON_BORDERLESS);
  naAddUIReaction(con->imagePushButtonBorderless, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->contentSpace, con->imagePushButtonBorderless, naMakePos(left + 1 * buttonSize, curPosY));

  con->imagePushButtonState = naNewImageButton(con->testImage, naMakeSize(buttonSize, 24), NA_BUTTON_STATEFUL);
  naAddUIReaction(con->imagePushButtonState, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->contentSpace, con->imagePushButtonState, naMakePos(left + 2 * buttonSize, curPosY));

  con->imagePushButtonStateBorderless = naNewImageButton(con->testImage, naMakeSize(buttonSize, 24), NA_BUTTON_BORDERLESS | NA_BUTTON_STATEFUL);
  naAddUIReaction(con->imagePushButtonStateBorderless, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  naAddSpaceChild(con->contentSpace, con->imagePushButtonStateBorderless, naMakePos(left + 3 * buttonSize, curPosY));

  curPosY -= 30;
  con->popupButtonLabel = naNewLabel("NAPopupButton", descSize);
  naAddSpaceChild(con->contentSpace, con->popupButtonLabel, naMakePos(20, curPosY));
  con->popupButton = naNewPopupButton(150);
  naAddSpaceChild(con->contentSpace, con->popupButton, naMakePos(left, curPosY));
  for(size_t i = 0; i < 5; ++i){
    NAMenuItem* item = naNewMenuItem(naAllocSprintf(NA_TRUE, "Popup menu item %d", i));
    naAddPopupButtonMenuItem(con->popupButton, item, NA_NULL);
    naAddUIReaction(item, NA_UI_COMMAND_PRESSED, popupButtonItemSelected, con);
  }
  naSetPopupButtonIndexSelected(con->popupButton, 3);

  curPosY -= 30;
  con->checkBoxLabel = naNewLabel("NACheckBox", descSize);
  naAddSpaceChild(con->contentSpace, con->checkBoxLabel, naMakePos(20, curPosY));
  con->checkBox = naNewCheckBox("I am a CheckBox", 150);
  naAddSpaceChild(con->contentSpace, con->checkBox, naMakePos(left, curPosY));
  naAddUIReaction(con->checkBox, NA_UI_COMMAND_PRESSED, checkBoxPressed, con);

  curPosY -= 30;
  con->imageSpaceLabel = naNewLabel("NAImageSpace", descSize);
  naAddSpaceChild(con->contentSpace, con->imageSpaceLabel, naMakePos(20, curPosY));
  con->imageSpace = naNewImageSpace(con->testImage, naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->imageSpace, naMakePos(left, curPosY));

  curPosY -= 30;
  con->labelLabel = naNewLabel("NALabel", descSize);
  naAddSpaceChild(con->contentSpace, con->labelLabel, naMakePos(20, curPosY));
  con->label = naNewLabel("I am a Label", 150);
  naAddSpaceChild(con->contentSpace, con->label, naMakePos(left, curPosY));

  curPosY -= 30;
  con->openGLSpaceLabel = naNewLabel("NAOpenGLSpace", descSize);
  naAddSpaceChild(con->contentSpace, con->openGLSpaceLabel, naMakePos(20, curPosY));
  con->openGLSpace = naNewOpenGLSpace(naMakeSize(150, 22), initOpenGL, con);
  naAddSpaceChild(con->contentSpace, con->openGLSpace, naMakePos(left, curPosY));
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, redrawOpenGLSpace, con);
  con->openGLSpaceRefreshCount = 1;

  curPosY -= 30;
  con->radioLabel = naNewLabel("NARadio", descSize);
  naAddSpaceChild(con->contentSpace, con->radioLabel, naMakePos(20, curPosY));
  con->radio = naNewRadio("I am a Radio", 150);
  naAddSpaceChild(con->contentSpace, con->radio, naMakePos(left, curPosY));
  naAddUIReaction(con->radio, NA_UI_COMMAND_PRESSED, radioPressed, con);

  curPosY -= 30;
  con->sliderLabel = naNewLabel("NASlider", descSize);
  naAddSpaceChild(con->contentSpace, con->sliderLabel, naMakePos(20, curPosY));
  con->slider = naNewSlider(150);
  naAddSpaceChild(con->contentSpace, con->slider, naMakePos(left, curPosY));
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  curPosY -= 30;
  con->textBoxLabel = naNewLabel("NATextBox", descSize);
  naAddSpaceChild(con->contentSpace, con->textBoxLabel, naMakePos(20, curPosY));
  con->textBox = naNewTextBox(naMakeSize(150, 16));
  naAddSpaceChild(con->contentSpace, con->textBox, naMakePos(left, curPosY));

  curPosY -= 30;
  con->textFieldLabel = naNewLabel("NATextField", descSize);
  naAddSpaceChild(con->contentSpace, con->textFieldLabel, naMakePos(20, curPosY));
  con->textField = naNewTextField(150);
  naAddSpaceChild(con->contentSpace, con->textField, naMakePos(left, curPosY));
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDITED, textFieldEdited, con);

  curPosY -= 30;
  con->menuLabel = naNewLabel("NAMenu", descSize);
  naAddSpaceChild(con->contentSpace, con->menuLabel, naMakePos(20, curPosY));
  con->menuButton = naNewTextButton("Push for Menu", buttonSize * 2, 0);
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
  //naAddUIKeyboardShortcut(
  //  con->menuItem1,
  //  naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ESC),
  //  menuItemKeyboardSelected,
  //  con);

  curPosY -= 50;
  con->subSpace = naNewSpace(naMakeSize(300, 30));
  naSetSpaceAlternateBackground(con->subSpace, NA_TRUE);
  naAddSpaceChild(con->contentSpace, con->subSpace, naMakePos(20, curPosY));

  con->fontButton = naNewTextButton("Fonts", 60, 0);
  con->quitButton = naNewTextButton("Quit", 60, 0);
  naAddSpaceChild(con->contentSpace, con->fontButton, naMakePos(10, 140));
  naAddSpaceChild(con->contentSpace, con->quitButton, naMakePos(10, 110));
  naAddUIReaction(con->fontButton, NA_UI_COMMAND_PRESSED, pressExperimentButton, con);
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


void clearExperimentController(ExperimentController* con){
  // Note that all UI elements which are attached in some way to the root
  // application UIElement will be cleared automatically.
  naRelease(con->testImage);
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
