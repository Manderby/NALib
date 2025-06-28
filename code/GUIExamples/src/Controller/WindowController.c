
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct WindowController{
  CommonController comCon;

  NALabel* normalWindowLabel;
  NAButton* openNormalWindow;
  NAWindow* normalWindow;
  NALabel* normalScreenLabel;
  NALabel* normalUIScale;
  NALabel* normalRect;

  NALabel* nonresizeWindowLabel;
  NAButton* openNonResizeWindow;
  NAWindow* nonresizeWindow;
  NALabel* nonresizeScreenLabel;
  NALabel* nonresizeUIScale;
  NALabel* nonresizeRect;

  NALabel* noncloseWindowLabel;
  NAButton* openNonCloseWindow;
  NAButton* closeNonCloseWindow;
  NAWindow* noncloseWindow;
  NALabel* noncloseScreenLabel;
  NALabel* noncloseUIScale;
  NALabel* noncloseRect;

  NALabel* nonminiaturizeWindowLabel;
  NAButton* openNonMinimizeWindow;
  NAWindow* nonminiaturizeWindow;
  NALabel* nonminiaturizeScreenLabel;
  NALabel* nonminiaturizeUIScale;
  NALabel* nonminiaturizeRect;

  NALabel* nothingWindowLabel;
  NAButton* openNothingWindow;
  NAButton* closeNothingWindow;
  NAWindow* nothingWindow;
  NALabel* nothingScreenLabel;
  NALabel* nothingUIScale;
  NALabel* nothingRect;

  NALabel* titlelessWindowLabel;
  NAButton* openTitlelessWindow;
  NAButton* closeTitlelessWindow;
  NAWindow* titlelessWindow;
  NALabel* titlelessScreenLabel;
  NALabel* titlelessUIScale;
  NALabel* titlelessRect;
  
  NALabel* auxiliaryWindowLabel;
  NAButton* openAuxiliaryWindow;
  NAWindow* auxiliaryWindow;
  NALabel* auxiliaryScreenLabel;
  NALabel* auxiliaryUIScale;
  NALabel* auxiliaryRect;
};

// Prototype:
void updateWindowController(WindowController* con);



static void buttonPressed(NAReaction reaction) {
  WindowController* con = (WindowController*)reaction.controller;
  
  if(reaction.uiElement == con->openNormalWindow) {
    naShowWindow(con->normalWindow);
  }else if(reaction.uiElement == con->openNonResizeWindow) {
    naShowWindow(con->nonresizeWindow);
  }else if(reaction.uiElement == con->openNonCloseWindow) {
    naShowWindow(con->noncloseWindow);
  }else if(reaction.uiElement == con->openNonMinimizeWindow) {
    naShowWindow(con->nonminiaturizeWindow);
  }else if(reaction.uiElement == con->openNothingWindow) {
    naShowWindow(con->nothingWindow);
  }else if(reaction.uiElement == con->closeNonCloseWindow) {
    naCloseWindow(con->noncloseWindow);
  }else if(reaction.uiElement == con->closeNothingWindow) {
    naCloseWindow(con->nothingWindow);
  }else if(reaction.uiElement == con->openTitlelessWindow) {
    naShowWindow(con->titlelessWindow);
  }else if(reaction.uiElement == con->closeTitlelessWindow) {
    naCloseWindow(con->titlelessWindow);
  }else if(reaction.uiElement == con->openAuxiliaryWindow) {
    naShowWindow(con->auxiliaryWindow);
  }
}

static void reshape(NAReaction reaction) {
  WindowController* con = (WindowController*)reaction.controller;
  
  updateWindowController(con);
}



CommonController* createWindowController() {
  WindowController* con = naAlloc(WindowController);

  NASpace* space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  
  double smallWindowLeft = WINDOW_POS_X + WINDOW_WIDTH + WINDOW_MARGIN;
  double smallWindowHeight = 3 * UI_ELEMENT_HEIGTH + 2 * SPACE_MARGIN_V;
  double windowTop = smallWindowHeight - SPACE_MARGIN_V - UI_ELEMENT_HEIGTH;
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalWindowLabel = naNewLabel("Normal & Resizeable", COLUMN0_WIDTH);
  naSetLabelFont(con->normalWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->normalWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  con->openNormalWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openNormalWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openNormalWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->normalWindow = naNewWindow("Normal Window", naMakeRectS(smallWindowLeft, 550, 300, smallWindowHeight), NA_WINDOW_RESIZEABLE);
  naAddUIReaction(con->normalWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  NASpace* normalSpace = naGetWindowContentSpace(con->normalWindow);

  con->normalScreenLabel = naNewLabel("", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naAddSpaceChild(normalSpace, con->normalScreenLabel, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 2 * UI_ELEMENT_HEIGTH));
  con->normalUIScale = naNewLabel("", COLUMN2_WIDTH);
  naAddSpaceChild(normalSpace, con->normalUIScale, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 1 * UI_ELEMENT_HEIGTH));
  con->normalRect = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naAddSpaceChild(normalSpace, con->normalRect, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 0 * UI_ELEMENT_HEIGTH));
  
  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nonresizeWindowLabel = naNewLabel("Non-Resizeable", COLUMN0_WIDTH);
  naSetLabelFont(con->nonresizeWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->nonresizeWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  con->openNonResizeWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openNonResizeWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openNonResizeWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  
  con->nonresizeWindow = naNewWindow("Non-Resizeable Window", naMakeRectS(smallWindowLeft + 0 * 50, 325 + 3 * 25, 300, smallWindowHeight), 0);
  naAddUIReaction(con->nonresizeWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  NASpace* nonresizeSpace = naGetWindowContentSpace(con->nonresizeWindow);

  con->nonresizeScreenLabel = naNewLabel("", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naAddSpaceChild(nonresizeSpace, con->nonresizeScreenLabel, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 2 * UI_ELEMENT_HEIGTH));
  con->nonresizeUIScale = naNewLabel("", COLUMN2_WIDTH);
  naAddSpaceChild(nonresizeSpace, con->nonresizeUIScale, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 1 * UI_ELEMENT_HEIGTH));
  con->nonresizeRect = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naAddSpaceChild(nonresizeSpace, con->nonresizeRect, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 0 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->noncloseWindowLabel = naNewLabel("Non-Closeable", COLUMN0_WIDTH);
  naSetLabelFont(con->noncloseWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->noncloseWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  con->openNonCloseWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openNonCloseWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openNonCloseWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  con->closeNonCloseWindow = naNewTextPushButton("close", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->closeNonCloseWindow, naMakePos(TAB1 + MEDIUM_BUTTON_WIDTH + UI_ELEMENT_MARGIN, curPosY));
  naAddUIReaction(con->closeNonCloseWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->noncloseWindow = naNewWindow("Non-Closeable Window", naMakeRectS(smallWindowLeft + 1 * 50, 325 + 2 * 25, 300, smallWindowHeight), NA_WINDOW_RESIZEABLE | NA_WINDOW_NON_CLOSEABLE);
  naAddUIReaction(con->noncloseWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  NASpace* noncloseSpace = naGetWindowContentSpace(con->noncloseWindow);

  con->noncloseScreenLabel = naNewLabel("", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naAddSpaceChild(noncloseSpace, con->noncloseScreenLabel, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 2 * UI_ELEMENT_HEIGTH));
  con->noncloseUIScale = naNewLabel("", COLUMN2_WIDTH);
  naAddSpaceChild(noncloseSpace, con->noncloseUIScale, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 1 * UI_ELEMENT_HEIGTH));
  con->noncloseRect = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naAddSpaceChild(noncloseSpace, con->noncloseRect, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 0 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nonminiaturizeWindowLabel = naNewLabel("Non-Miniaturizeable", COLUMN0_WIDTH);
  naSetLabelFont(con->nonminiaturizeWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->nonminiaturizeWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  con->openNonMinimizeWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openNonMinimizeWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openNonMinimizeWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  
  con->nonminiaturizeWindow = naNewWindow("Non-Miniaturizeable Window", naMakeRectS(smallWindowLeft + 2 * 50, 325 + 1 * 25, 300, smallWindowHeight), NA_WINDOW_RESIZEABLE | NA_WINDOW_NON_MINIATURIZEABLE);
  naAddUIReaction(con->nonminiaturizeWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  NASpace* nonminiaturizeSpace = naGetWindowContentSpace(con->nonminiaturizeWindow);

  con->nonminiaturizeScreenLabel = naNewLabel("", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naAddSpaceChild(nonminiaturizeSpace, con->nonminiaturizeScreenLabel, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 2 * UI_ELEMENT_HEIGTH));
  con->nonminiaturizeUIScale = naNewLabel("", COLUMN2_WIDTH);
  naAddSpaceChild(nonminiaturizeSpace, con->nonminiaturizeUIScale, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 1 * UI_ELEMENT_HEIGTH));
  con->nonminiaturizeRect = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naAddSpaceChild(nonminiaturizeSpace, con->nonminiaturizeRect, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 0 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nothingWindowLabel = naNewLabel("No title buttons", COLUMN0_WIDTH);
  naSetLabelFont(con->nothingWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->nothingWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  con->openNothingWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openNothingWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openNothingWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  con->closeNothingWindow = naNewTextPushButton("close", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->closeNothingWindow, naMakePos(TAB1 + MEDIUM_BUTTON_WIDTH + UI_ELEMENT_MARGIN, curPosY));
  naAddUIReaction(con->closeNothingWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->nothingWindow = naNewWindow("Window with no title buttons", naMakeRectS(smallWindowLeft + 3 * 50, 325 + 0 * 25, 300, smallWindowHeight), NA_WINDOW_NON_MINIATURIZEABLE | NA_WINDOW_NON_CLOSEABLE | NA_WINDOW_FIXED_SIZE);
  naAddUIReaction(con->nothingWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  NASpace* nothingSpace = naGetWindowContentSpace(con->nothingWindow);

  con->nothingScreenLabel = naNewLabel("", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naAddSpaceChild(nothingSpace, con->nothingScreenLabel, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 2 * UI_ELEMENT_HEIGTH));
  con->nothingUIScale = naNewLabel("", COLUMN2_WIDTH);
  naAddSpaceChild(nothingSpace, con->nothingUIScale, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 1 * UI_ELEMENT_HEIGTH));
  con->nothingRect = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naAddSpaceChild(nothingSpace, con->nothingRect, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 0 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->titlelessWindowLabel = naNewLabel("Titleless Window", COLUMN0_WIDTH);
  naSetLabelFont(con->titlelessWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->titlelessWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  con->openTitlelessWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openTitlelessWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openTitlelessWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);
  con->closeTitlelessWindow = naNewTextPushButton("close", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->closeTitlelessWindow, naMakePos(TAB1 + MEDIUM_BUTTON_WIDTH + UI_ELEMENT_MARGIN, curPosY));
  naAddUIReaction(con->closeTitlelessWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->titlelessWindow = naNewWindow("This title should never be displayed", naMakeRectS(smallWindowLeft, 200, 300, smallWindowHeight), NA_WINDOW_TITLELESS | NA_WINDOW_RESIZEABLE);
  naAddUIReaction(con->titlelessWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  NASpace* titlelessSpace = naGetWindowContentSpace(con->titlelessWindow);

  con->titlelessScreenLabel = naNewLabel("", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naAddSpaceChild(titlelessSpace, con->titlelessScreenLabel, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 2 * UI_ELEMENT_HEIGTH));
  con->titlelessUIScale = naNewLabel("", COLUMN2_WIDTH);
  naAddSpaceChild(titlelessSpace, con->titlelessUIScale, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 1 * UI_ELEMENT_HEIGTH));
  con->titlelessRect = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naAddSpaceChild(titlelessSpace, con->titlelessRect, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 0 * UI_ELEMENT_HEIGTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->auxiliaryWindowLabel = naNewLabel("Auxiliary", COLUMN0_WIDTH);
  naSetLabelFont(con->auxiliaryWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->auxiliaryWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  con->openAuxiliaryWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openAuxiliaryWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openAuxiliaryWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->auxiliaryWindow = naNewWindow("Auxiliary Window", naMakeRectS(smallWindowLeft, 50, 300, smallWindowHeight), NA_WINDOW_RESIZEABLE | NA_WINDOW_AUXILIARY);
  naAddUIReaction(con->auxiliaryWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  NASpace* auxiliarySpace = naGetWindowContentSpace(con->auxiliaryWindow);

  con->auxiliaryScreenLabel = naNewLabel("", WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naAddSpaceChild(auxiliarySpace, con->auxiliaryScreenLabel, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 2 * UI_ELEMENT_HEIGTH));
  con->auxiliaryUIScale = naNewLabel("", COLUMN2_WIDTH);
  naAddSpaceChild(auxiliarySpace, con->auxiliaryUIScale, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 1 * UI_ELEMENT_HEIGTH));
  con->auxiliaryRect = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naAddSpaceChild(auxiliarySpace, con->auxiliaryRect, naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V + 0 * UI_ELEMENT_HEIGTH));

  initCommonController(
    &con->comCon,
    space,
    NA_NULL,
    (NAMutator)updateWindowController);

  return (CommonController*)con;
}



void updateWindowController(WindowController* con) {
  NARect borderRect;
  const NAUTF8Char* labelString;
  
  // Normal window 
  NAString* normalScreenName = naNewScreenName(naGetWindowScreen(con->normalWindow));
  labelString = naAllocSprintf(NA_TRUE, "Screen: %s", naGetStringUTF8Pointer(normalScreenName));
  naDelete(normalScreenName);
  naSetLabelText(con->normalScreenLabel, labelString);
  labelString = naAllocSprintf(NA_TRUE, "UI-Scale: %.2fx", naGetUIElementUIScale(con->normalWindow));
  naSetLabelText(con->normalUIScale, labelString);
  borderRect = naGetWindowOuterRect(con->normalWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->normalRect, labelString);

  // Non-Resize Window
  NAString* nonresizeScreenName = naNewScreenName(naGetWindowScreen(con->nonresizeWindow));
  labelString = naAllocSprintf(NA_TRUE, "Screen: %s", naGetStringUTF8Pointer(nonresizeScreenName));
  naDelete(nonresizeScreenName);
  naSetLabelText(con->nonresizeScreenLabel, labelString);
  labelString = naAllocSprintf(NA_TRUE, "UI-Scale: %.2fx", naGetUIElementUIScale(con->nonresizeWindow));
  naSetLabelText(con->nonresizeUIScale, labelString);
  borderRect = naGetWindowOuterRect(con->nonresizeWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->nonresizeRect, labelString);

  // Non-Close Window
  NAString* noncloseScreenName = naNewScreenName(naGetWindowScreen(con->noncloseWindow));
  labelString = naAllocSprintf(NA_TRUE, "Screen: %s", naGetStringUTF8Pointer(noncloseScreenName));
  naDelete(noncloseScreenName);
  naSetLabelText(con->noncloseScreenLabel, labelString);
  labelString = naAllocSprintf(NA_TRUE, "UI-Scale: %.2fx", naGetUIElementUIScale(con->noncloseWindow));
  naSetLabelText(con->noncloseUIScale, labelString);
  borderRect = naGetWindowOuterRect(con->noncloseWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->noncloseRect, labelString);

  // Non-Minimize Window
  NAString* nonminiaturizeScreenName = naNewScreenName(naGetWindowScreen(con->nonminiaturizeWindow));
  labelString = naAllocSprintf(NA_TRUE, "Screen: %s", naGetStringUTF8Pointer(nonminiaturizeScreenName));
  naDelete(nonminiaturizeScreenName);
  naSetLabelText(con->nonminiaturizeScreenLabel, labelString);
  labelString = naAllocSprintf(NA_TRUE, "UI-Scale: %.2fx", naGetUIElementUIScale(con->nonminiaturizeWindow));
  naSetLabelText(con->nonminiaturizeUIScale, labelString);
  borderRect = naGetWindowOuterRect(con->nonminiaturizeWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->nonminiaturizeRect, labelString);

  // Window without buttons
  NAString* nothingScreenName = naNewScreenName(naGetWindowScreen(con->nothingWindow));
  labelString = naAllocSprintf(NA_TRUE, "Screen: %s", naGetStringUTF8Pointer(nothingScreenName));
  naDelete(nothingScreenName);
  naSetLabelText(con->nothingScreenLabel, labelString);
  labelString = naAllocSprintf(NA_TRUE, "UI-Scale: %.2fx", naGetUIElementUIScale(con->nothingWindow));
  naSetLabelText(con->nothingUIScale, labelString);
  borderRect = naGetWindowOuterRect(con->nothingWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->nothingRect, labelString);

  // Titleless Window
  NAString* titlelessScreenName = naNewScreenName(naGetWindowScreen(con->titlelessWindow));
  labelString = naAllocSprintf(NA_TRUE, "Screen: %s", naGetStringUTF8Pointer(titlelessScreenName));
  naDelete(titlelessScreenName);
  naSetLabelText(con->titlelessScreenLabel, labelString);
  labelString = naAllocSprintf(NA_TRUE, "UI-Scale: %.2fx", naGetUIElementUIScale(con->titlelessWindow));
  naSetLabelText(con->titlelessUIScale, labelString);
  borderRect = naGetWindowOuterRect(con->titlelessWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->titlelessRect, labelString);

  // Auxiliary Window
  NAString* auxiliaryScreenName = naNewScreenName(naGetWindowScreen(con->auxiliaryWindow));
  labelString = naAllocSprintf(NA_TRUE, "Screen: %s", naGetStringUTF8Pointer(auxiliaryScreenName));
  naDelete(auxiliaryScreenName);
  naSetLabelText(con->auxiliaryScreenLabel, labelString);
  labelString = naAllocSprintf(NA_TRUE, "UI-Scale: %.2fx", naGetUIElementUIScale(con->auxiliaryWindow));
  naSetLabelText(con->auxiliaryUIScale, labelString);
  borderRect = naGetWindowOuterRect(con->auxiliaryWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->auxiliaryRect, labelString);
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
