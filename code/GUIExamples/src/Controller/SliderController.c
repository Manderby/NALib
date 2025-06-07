
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct SliderController{
  NASpace* space;

  NALabel* normalLabel;
  NASlider* slider;

  NALabel* ticksLabel;
  NASlider* ticksSlider;
  
  NALabel* disabledLabel;
  NASlider* sliderDisabled1;
  NASlider* sliderDisabled2;

  NALabel* outputLabel;
};



static void sliderEdited(NAReaction reaction){
  SliderController* con = reaction.controller;
  const NASlider* slider = reaction.uiElement;
  const NAUTF8Char* outputText = naAllocSprintf(NA_TRUE, "Slider Value Edited to %f", naGetSliderValue(slider));
  naSetLabelText(con->outputLabel, outputText);
}



SliderController* createSliderController(){
  SliderController* con = naAlloc(SliderController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalLabel = naNewLabel("Normal", COLUMN0_WIDTH);
  naSetLabelFont(con->normalLabel, getTitleFont());
  naAddSpaceChild(con->space, con->normalLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->slider = naNewSlider(COLUMN1_WIDTH);
  naAddSpaceChild(con->space, con->slider, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->slider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->ticksLabel = naNewLabel("Ticks", COLUMN0_WIDTH);
  naSetLabelFont(con->ticksLabel, getTitleFont());
  naAddSpaceChild(con->space, con->ticksLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->ticksSlider = naNewSlider(COLUMN1_WIDTH);
  naSetSliderRange(con->ticksSlider, 0., 1., 11);
  naAddSpaceChild(con->space, con->ticksSlider, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->ticksSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  curPosY = curPosY - (2. * UI_ELEMENT_HEIGTH);

  con->disabledLabel = naNewLabel("Disabled", COLUMN0_WIDTH);
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(con->space, con->disabledLabel, naMakePos(WINDOW_MARGIN, curPosY + .5 * UI_ELEMENT_HEIGTH));
  
  con->sliderDisabled1 = naNewSlider(COLUMN1_WIDTH);
  naSetSliderEnabled(con->sliderDisabled1, NA_FALSE);
  naSetSliderValue(con->sliderDisabled1, .5);
  naAddSpaceChild(con->space, con->sliderDisabled1, naMakePos(TAB1, curPosY + UI_ELEMENT_HEIGTH));

  con->sliderDisabled2 = naNewSlider(COLUMN1_WIDTH);
  naSetSliderEnabled(con->sliderDisabled2, NA_FALSE);
  naSetSliderRange(con->sliderDisabled2, 0., 1., 11);
  naSetSliderValue(con->sliderDisabled2, .5);
  naAddSpaceChild(con->space, con->sliderDisabled2, naMakePos(TAB1, curPosY));
  naAddUIReaction(con->sliderDisabled2, NA_UI_COMMAND_EDITED, sliderEdited, con);

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naSetLabelFont(con->outputLabel, getMonoFont());
  naAddSpaceChild(con->space, con->outputLabel, naMakePos(WINDOW_MARGIN, WINDOW_MARGIN));

  return con;
}



void clearSliderController(SliderController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getSliderControllerSpace(SliderController* con){
  return con->space;
}



void updateSliderController(SliderController* con) {
  NA_UNUSED(con);
  // nothing to do.
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
