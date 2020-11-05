
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



#include "../../NALib/src/NAUI.h"
#include "HelloWorldGUI.h"
#include "../../NALib/src/NAEnvironment/NAUIImage.h"



struct ExperimentController{
  NAWindow* experimentWindow;
  NASpace* contentSpace;

  NAUIImage* testImage;

  NALabel* pushButtonLabel;
  NAButton* pushButton;

  NALabel* textOptionButtonLabel;
  NAButton* textOptionButton1;
  NAButton* textOptionButton2;
  NAButton* textOptionButton3;

  NALabel* imageOptionButtonLabel;
  NAButton* imageOptionButton1;
  NAButton* imageOptionButton2;
  NAButton* imageOptionButton3;

  NALabel* imageButtonLabel;
  NAButton* imageButton;

  NALabel* checkBoxLabel;
  NACheckBox* checkBox;

  NALabel* imageSpaceLabel;
  NAImageSpace* imageSpace;

  NALabel* labelLabel;
  NALabel* label;

  NALabel* openGLSpaceLabel;
  NAOpenGLSpace* openGLSpace;

  NALabel* radioLabel;
  NARadio* radio;

  NALabel* sliderLabel;
  NASlider* slider;

  NALabel* textBoxLabel;
  NATextBox* textBox;

  NALabel* textFieldLabel;
  NATextField* textField;

  int textOption;
  int imageOption;
  NALabel* outputLabel;
};

NABool pushButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Push Button Pressed");

  return NA_TRUE;
}

NABool textOptionButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  if     (reaction.uiElement == con->textOptionButton1){con->textOption = 1;}
  else if(reaction.uiElement == con->textOptionButton2){con->textOption = 2;}
  else if(reaction.uiElement == con->textOptionButton3){con->textOption = 3;}

  naSetButtonState(con->textOptionButton1, con->textOption == 1);
  naSetButtonState(con->textOptionButton2, con->textOption == 2);
  naSetButtonState(con->textOptionButton3, con->textOption == 3);

  naSetLabelText(con->outputLabel, "Text Option Button Pressed");

  return NA_TRUE;
}

NABool imageOptionButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  if     (reaction.uiElement == con->imageOptionButton1){con->imageOption = 1;}
  else if(reaction.uiElement == con->imageOptionButton2){con->imageOption = 2;}
  else if(reaction.uiElement == con->imageOptionButton3){con->imageOption = 3;}

  naSetButtonState(con->imageOptionButton1, con->imageOption == 1);
  naSetButtonState(con->imageOptionButton2, con->imageOption == 2);
  naSetButtonState(con->imageOptionButton3, con->imageOption == 3);

  naSetLabelText(con->outputLabel, "Image Option Button Pressed");

  return NA_TRUE;
}

NABool imageButtonPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "Image Button Pressed");

  return NA_TRUE;
}

NABool checkBoxPressed(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  naSetLabelText(con->outputLabel, "CheckBox Pressed");

  return NA_TRUE;
}

NABool redrawOpenGLSpace(NAReaction reaction){
  // OpenGL is declared deprecated on macOS 10.14. These pragma directives
  // omit the nasty warnings. Do not forget the pragma pop at the end of this
  // function!
  #if NA_OS == NA_OS_MAC_OS_X
    #pragma GCC diagnostic push 
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  #endif

  static float ang = 0.f;

  ang += .05f;
  if(ang > NA_PI2f){ang = 0.f;}

  glClearColor(0.f, 0.f, .4f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor4f(1.f, 1.f, 1.f, 1.f);
  glPointSize(5);
  glBegin(GL_POINTS);
    glVertex3f(naSinf(ang) * .9f, 0.f, 0.f);
  glEnd();

  naSwapOpenGLBuffer(reaction.uiElement);
  
  naRefreshUIElement(reaction.uiElement, 1./ 60);

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
  NASlider* slider = reaction.uiElement;
  const char* outputText = naAllocSprintf(NA_TRUE, "Slider Value Edited to %f", naGetSliderValue(slider));
  naSetLabelText(con->outputLabel, outputText);

  return NA_TRUE;
}

NABool textFieldEdited(NAReaction reaction){
  ExperimentController* con = reaction.controller;
  NATextField* textField = reaction.uiElement;
  NAString* textFieldString = naNewStringWithTextFieldText(textField);
  const char* outputText = naAllocSprintf(NA_TRUE, "TextField Value Edited to %s", naGetStringUTF8Pointer(textFieldString));
  naSetLabelText(con->outputLabel, outputText);
  naDelete(textFieldString);

  return NA_TRUE;
}



ExperimentController* createExperimentController(){
  double windowWidth = 420;
  double windowHeight = 600;

  ExperimentController* con = naAlloc(ExperimentController);

  NABabyColor mainColor = {1., .25, 0., 1.};
  NABabyColor altColor = {.25, 0., 1., 1.};
  NABabyImage* mainImage = naCreateBabyImage(naMakeSizei(20, 10), mainColor);
  NABabyImage* altImage = naCreateBabyImage(naMakeSizei(20, 10), altColor);
  con->testImage = naNewUIImage(mainImage, altImage, NA_UIIMAGE_RESOLUTION_1x, NA_BLEND_ZERO);

  con->experimentWindow = naNewWindow(
    "Experiment",
    naMakeRectS(150, 150, windowWidth, windowHeight),
    NA_TRUE,
    0);

  con->contentSpace = naNewSpace(naMakeSize(windowWidth, windowHeight));
  naSetWindowContentSpace(con->experimentWindow, con->contentSpace);

  double curPosY = windowHeight - 42;
  con->pushButtonLabel = naNewLabel("NAButton: PushButton", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->pushButtonLabel, naMakePos(20, curPosY));
  con->pushButton = naNewPushButton("Push Button", naMakeSize(150, 24));
  naAddSpaceChild(con->contentSpace, con->pushButton, naMakePos(250, curPosY));
  naAddUIReaction(con->pushButton, NA_UI_COMMAND_PRESSED, pushButtonPressed, con);

  curPosY -= 30;
  con->textOptionButtonLabel = naNewLabel("NAButton: TextOptionButton", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->textOptionButtonLabel, naMakePos(20, curPosY));
  con->textOptionButton1 = naNewTextOptionButton("Text 1", naMakeSize(50, 22));
  naAddSpaceChild(con->contentSpace, con->textOptionButton1, naMakePos(250, curPosY));
  naAddUIReaction(con->textOptionButton1, NA_UI_COMMAND_PRESSED, textOptionButtonPressed, con);
  con->textOptionButton2 = naNewTextOptionButton("Text 2", naMakeSize(50, 22));
  naAddSpaceChild(con->contentSpace, con->textOptionButton2, naMakePos(300, curPosY));
  naAddUIReaction(con->textOptionButton2, NA_UI_COMMAND_PRESSED, textOptionButtonPressed, con);
  con->textOptionButton3 = naNewTextOptionButton("Text 3", naMakeSize(50, 22));
  naAddSpaceChild(con->contentSpace, con->textOptionButton3, naMakePos(350, curPosY));
  naAddUIReaction(con->textOptionButton3, NA_UI_COMMAND_PRESSED, textOptionButtonPressed, con);

  curPosY -= 30;  
  con->imageOptionButtonLabel = naNewLabel("NAButton: ImageOptionButton", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->imageOptionButtonLabel, naMakePos(20, curPosY));
  con->imageOptionButton1 = naNewImageOptionButton(con->testImage, naMakeSize(50, 22));
  naAddSpaceChild(con->contentSpace, con->imageOptionButton1, naMakePos(250, curPosY));
  naAddUIReaction(con->imageOptionButton1, NA_UI_COMMAND_PRESSED, imageOptionButtonPressed, con);
  con->imageOptionButton2 = naNewImageOptionButton(con->testImage, naMakeSize(50, 22));
  naAddSpaceChild(con->contentSpace, con->imageOptionButton2, naMakePos(300, curPosY));
  naAddUIReaction(con->imageOptionButton2, NA_UI_COMMAND_PRESSED, imageOptionButtonPressed, con);
  con->imageOptionButton3 = naNewImageOptionButton(con->testImage, naMakeSize(50, 22));
  naAddSpaceChild(con->contentSpace, con->imageOptionButton3, naMakePos(350, curPosY));
  naAddUIReaction(con->imageOptionButton3, NA_UI_COMMAND_PRESSED, imageOptionButtonPressed, con);

  curPosY -= 30;
  con->imageButtonLabel = naNewLabel("NAButton: ImageButton", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->imageButtonLabel, naMakePos(20, curPosY));
  con->imageButton = naNewImageButton(con->testImage, naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->imageButton, naMakePos(250, curPosY));
  naAddUIReaction(con->imageButton, NA_UI_COMMAND_PRESSED, imageButtonPressed, con);

  curPosY -= 30;
  con->checkBoxLabel = naNewLabel("NACheckBox", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->checkBoxLabel, naMakePos(20, curPosY));
  con->checkBox = naNewCheckBox("I am a CheckBox", naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->checkBox, naMakePos(250, curPosY));
  naAddUIReaction(con->checkBox, NA_UI_COMMAND_PRESSED, checkBoxPressed, con);

  curPosY -= 30;
  con->imageSpaceLabel = naNewLabel("NAImageSpace", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->imageSpaceLabel, naMakePos(20, curPosY));
  con->imageSpace = naNewImageSpace(con->testImage, naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->imageSpace, naMakePos(250, curPosY));

  curPosY -= 30;
  con->labelLabel = naNewLabel("NALabel", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->labelLabel, naMakePos(20, curPosY));
  con->label = naNewLabel("I am a Label", naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->label, naMakePos(250, curPosY));

  curPosY -= 30;
  con->openGLSpaceLabel = naNewLabel("NAOpenGLSpace", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->openGLSpaceLabel, naMakePos(20, curPosY));
  con->openGLSpace = naNewOpenGLSpace(naMakeSize(150, 22), NA_NULL, NA_NULL);
  naAddSpaceChild(con->contentSpace, con->openGLSpace, naMakePos(250, curPosY));
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, redrawOpenGLSpace, NA_NULL);

  curPosY -= 30;
  con->radioLabel = naNewLabel("NARadio", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->radioLabel, naMakePos(20, curPosY));
  con->radio = naNewRadio("I am a Radio", naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->radio, naMakePos(250, curPosY));
  naAddUIReaction(con->radio, NA_UI_COMMAND_PRESSED, radioPressed, con);

  curPosY -= 30;
  con->sliderLabel = naNewLabel("NASlider", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->sliderLabel, naMakePos(20, curPosY));
  con->slider = naNewSlider(naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->slider, naMakePos(250, curPosY));
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  curPosY -= 30;
  con->textBoxLabel = naNewLabel("NATextBox", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->textBoxLabel, naMakePos(20, curPosY));
  con->textBox = naNewTextBox(naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->textBox, naMakePos(250, curPosY));

  curPosY -= 30;
  con->textFieldLabel = naNewLabel("NATextField", naMakeSize(200, 22));
  naAddSpaceChild(con->contentSpace, con->textFieldLabel, naMakePos(20, curPosY));
  con->textField = naNewTextField(naMakeSize(150, 22));
  naAddSpaceChild(con->contentSpace, con->textField, naMakePos(250, curPosY));
  naAddUIReaction(con->textField, NA_UI_COMMAND_EDITED, textFieldEdited, con);

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    naMakeSize(windowWidth - 20, 44));
  naAddSpaceChild(con->contentSpace, con->outputLabel, naMakePos(10, 10));

  con->textOption = 0;

  naShowWindow(con->experimentWindow);

  return con;
}


void clearExperimentController(ExperimentController* con){
  naReleaseUIElement(con->experimentWindow);
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
