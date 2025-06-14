
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct WindowController{
  CommonController comCon;

  NALabel* normalWindowLabel;
  NAButton* openNormalWindow;
  NALabel* normalReaction;
  NAWindow* normalWindow;
  NALabel* normalRectReaction;

  NALabel* resizeWindowLabel;
  NAButton* openResizeWindow;
  NAWindow* resizeWindow;
  NALabel* resizeRectReaction;

  NALabel* nothingWindowLabel;
  NAButton* openNothingWindow;
  NAButton* closeNothingWindow;
  NAWindow* nothingWindow;
  NALabel* nothingRectReaction;

  NALabel* titlelessWindowLabel;
  NAButton* openTitlelessWindow;
  NAButton* closeTitlelessWindow;
  NAWindow* titlelessWindow;
  NALabel* titlelessRectReaction;
  
  NALabel* auxiliaryWindowLabel;
  NAButton* openAuxiliaryWindow;
  NAWindow* auxiliaryWindow;
  NALabel* auxiliaryRectReaction;
};

// Prototype:
void updateWindowController(WindowController* con);



static void buttonPressed(NAReaction reaction) {
  WindowController* con = (WindowController*)reaction.controller;
  
  if(reaction.uiElement == con->openNormalWindow) {
    naShowWindow(con->normalWindow);
  }else if(reaction.uiElement == con->openResizeWindow) {
    naShowWindow(con->resizeWindow);
  }else if(reaction.uiElement == con->openNothingWindow) {
    naShowWindow(con->nothingWindow);
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
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalWindowLabel = naNewLabel("Normal", COLUMN0_WIDTH);
  naSetLabelFont(con->normalWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->normalWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  
  con->normalWindow = naNewWindow("Normal Window", naMakeRectS(200 + WINDOW_WIDTH, 700, 300, 100), 0);
  naAddUIReaction(con->normalWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  con->openNormalWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openNormalWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openNormalWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->normalReaction = naNewLabel("", COLUMN2_WIDTH);
  naSetLabelFont(con->normalReaction, getMonoFont());
  naAddSpaceChild(space, con->normalReaction, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalRectReaction = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naSetLabelFont(con->normalRectReaction, getMonoFont());
  naAddSpaceChild(space, con->normalRectReaction, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->resizeWindowLabel = naNewLabel("Resizeable", COLUMN0_WIDTH);
  naSetLabelFont(con->resizeWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->resizeWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  
  con->resizeWindow = naNewWindow("Resizeable Window", naMakeRectS(200 + WINDOW_WIDTH, 550, 300, 100), NA_WINDOW_RESIZEABLE);
  naAddUIReaction(con->resizeWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  con->openResizeWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openResizeWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openResizeWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->resizeRectReaction = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naSetLabelFont(con->resizeRectReaction, getMonoFont());
  naAddSpaceChild(space, con->resizeRectReaction, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nothingWindowLabel = naNewLabel("No title buttons", COLUMN0_WIDTH);
  naSetLabelFont(con->nothingWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->nothingWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  
  con->nothingWindow = naNewWindow("Window with no title buttons", naMakeRectS(200 + WINDOW_WIDTH, 400, 300, 100), NA_WINDOW_NON_MINIATURIZEABLE | NA_WINDOW_NON_CLOSEABLE | NA_WINDOW_FIXED_SIZE);
  naAddUIReaction(con->nothingWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  con->openNothingWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openNothingWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openNothingWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->closeNothingWindow = naNewTextPushButton("close", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->closeNothingWindow, naMakePos(TAB1 + MEDIUM_BUTTON_WIDTH + UI_ELEMENT_MARGIN, curPosY));
  naAddUIReaction(con->closeNothingWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nothingRectReaction = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naSetLabelFont(con->nothingRectReaction, getMonoFont());
  naAddSpaceChild(space, con->nothingRectReaction, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->titlelessWindowLabel = naNewLabel("Titleless Window", COLUMN0_WIDTH);
  naSetLabelFont(con->titlelessWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->titlelessWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  
  con->titlelessWindow = naNewWindow("This title should never be displayed", naMakeRectS(200 + WINDOW_WIDTH, 250, 300, 100), NA_WINDOW_TITLELESS | NA_WINDOW_RESIZEABLE);
  naAddUIReaction(con->titlelessWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  con->openTitlelessWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openTitlelessWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openTitlelessWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  con->closeTitlelessWindow = naNewTextPushButton("close", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->closeTitlelessWindow, naMakePos(TAB1 + MEDIUM_BUTTON_WIDTH + UI_ELEMENT_MARGIN, curPosY));
  naAddUIReaction(con->closeTitlelessWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->titlelessRectReaction = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naSetLabelFont(con->titlelessRectReaction, getMonoFont());
  naAddSpaceChild(space, con->titlelessRectReaction, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->auxiliaryWindowLabel = naNewLabel("Auxiliary", COLUMN0_WIDTH);
  naSetLabelFont(con->auxiliaryWindowLabel, getTitleFont());
  naAddSpaceChild(space, con->auxiliaryWindowLabel, naMakePos(WINDOW_MARGIN, curPosY));
  
  con->auxiliaryWindow = naNewWindow("Auxiliary Window", naMakeRectS(200 + WINDOW_WIDTH, 100, 300, 100), NA_WINDOW_RESIZEABLE | NA_WINDOW_AUXILIARY);
  naAddUIReaction(con->auxiliaryWindow, NA_UI_COMMAND_RESHAPE, reshape, con);

  con->openAuxiliaryWindow = naNewTextPushButton("open", MEDIUM_BUTTON_WIDTH);
  naAddSpaceChild(space, con->openAuxiliaryWindow, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->openAuxiliaryWindow, NA_UI_COMMAND_PRESSED, buttonPressed, con);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->auxiliaryRectReaction = naNewLabel("", COLUMN1_WIDTH + COLUMN2_WIDTH + UI_ELEMENT_MARGIN);
  naSetLabelFont(con->auxiliaryRectReaction, getMonoFont());
  naAddSpaceChild(space, con->auxiliaryRectReaction, naMakePos(TAB1, curPosY));

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
  
  borderRect = naGetWindowOuterRect(con->normalWindow);
  labelString = naAllocSprintf(NA_TRUE, "%.2fx", naGetUIElementResolutionScale(con->normalWindow));
  naSetLabelText(con->normalReaction, labelString);

  borderRect = naGetWindowOuterRect(con->normalWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->normalRectReaction, labelString);

  borderRect = naGetWindowOuterRect(con->resizeWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->resizeRectReaction, labelString);

  borderRect = naGetWindowOuterRect(con->nothingWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->nothingRectReaction, labelString);

  borderRect = naGetWindowOuterRect(con->titlelessWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->titlelessRectReaction, labelString);

  borderRect = naGetWindowOuterRect(con->auxiliaryWindow);
  labelString = naAllocSprintf(NA_TRUE, "Pos: (%.0f, %.0f) Size: (%.0f, %.0f)", borderRect.pos.x, borderRect.pos.y, borderRect.size.width, borderRect.size.height);
  naSetLabelText(con->auxiliaryRectReaction, labelString);
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
