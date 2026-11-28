
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"
#include "NAVisual/NAColor.h"



struct SpaceController{
  CommonController comCon;

  NASpace* subSpace1;
  NASpace* subSpace2;
  NASpace* subSpace3;
  NASpace* subSpace4;
  NALabel* subSpace1Label;
  NALabel* subSpace2Label;
  NALabel* subSpace3Label;
  NALabel* subSpace4Label;
};



CommonController* createSpaceController() {
  SpaceController* con = naAlloc(SpaceController);

  NASpace* space = naNewSpace();
  naSetUIElementRect(space, naMakeRectS(0., 0., WINDOW_WIDTH, EXPERIMENT_HEIGHT));

  double spaceHeigth = UI_ELEMENT_HEIGTH + 2 * SPACE_MARGIN_V;
  double curPosY = EXPERIMENT_HEIGHT - spaceHeigth;

  con->subSpace1 = naNewSpace();
  con->subSpace2 = naNewSpace();
  con->subSpace3 = naNewSpace();
  con->subSpace4 = naNewSpace();

  NAColor backColor;
  naFillColorWithSRGB(&backColor, .8f, .5f, .2f, .25f);
  naSetSpaceBackgroundColor(con->subSpace3, &backColor);
  naSetSpaceBackgroundColor(con->subSpace4, &backColor);
  
  naSetSpaceAlternateBackground(con->subSpace2, NA_TRUE);
  naSetSpaceAlternateBackground(con->subSpace4, NA_TRUE);
  con->subSpace1Label = naNewLabel("Space with normal background");
  con->subSpace2Label = naNewLabel("Space with alternate background");
  con->subSpace3Label = naNewLabel("Space with colored background");
  con->subSpace4Label = naNewLabel("Space with colored and alternate background");
  naSetLabelTextAlignment(con->subSpace1Label, NA_ALIGNMENT_CENTER);
  naSetLabelTextAlignment(con->subSpace2Label, NA_ALIGNMENT_CENTER);
  naSetLabelTextAlignment(con->subSpace3Label, NA_ALIGNMENT_CENTER);
  naSetLabelTextAlignment(con->subSpace4Label, NA_ALIGNMENT_CENTER);
  naAddSpaceChild(
    con->subSpace1,
    con->subSpace1Label,
    naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V),
    naMakeInlineSize(WINDOW_WIDTH - 2 * WINDOW_MARGIN));
  naAddSpaceChild(
    con->subSpace2,
    con->subSpace2Label,
    naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V),
    naMakeInlineSize(WINDOW_WIDTH - 2 * WINDOW_MARGIN));
  naAddSpaceChild(
    con->subSpace3,
    con->subSpace3Label,
    naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V),
    naMakeInlineSize(WINDOW_WIDTH - 2 * WINDOW_MARGIN));
  naAddSpaceChild(
    con->subSpace4,
    con->subSpace4Label,
    naMakePos(WINDOW_MARGIN, SPACE_MARGIN_V),
    naMakeInlineSize(WINDOW_WIDTH - 2 * WINDOW_MARGIN));
  
  naAddSpaceChild(
    space,
    con->subSpace1,
    naMakePos(0, curPosY - 0 * spaceHeigth),
    naMakeSize(WINDOW_WIDTH, spaceHeigth));
  naAddSpaceChild(
    space,
    con->subSpace2,
    naMakePos(0, curPosY - 1 * spaceHeigth),
    naMakeSize(WINDOW_WIDTH, spaceHeigth));
  naAddSpaceChild(
    space,
    con->subSpace3,
    naMakePos(0, curPosY - 2 * spaceHeigth),
    naMakeSize(WINDOW_WIDTH, spaceHeigth));
  naAddSpaceChild(
    space,
    con->subSpace4,
    naMakePos(0, curPosY - 3 * spaceHeigth),
    naMakeSize(WINDOW_WIDTH, spaceHeigth));

  initCommonController(
    &con->comCon,
    space,
    NA_NULL,
    NA_NULL);

  return (CommonController*)con;
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
