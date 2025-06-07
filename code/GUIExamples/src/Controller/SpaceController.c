
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"
#include "NAVisual/NAColor.h"



struct SpaceController{
  NASpace* space;

  NASpace* subSpace1;
  NASpace* subSpace2;
  NASpace* subSpace3;
  NASpace* subSpace4;
  NALabel* subSpace1Label;
  NALabel* subSpace2Label;
  NALabel* subSpace3Label;
  NALabel* subSpace4Label;
};



SpaceController* createSpaceController(){
  SpaceController* con = naAlloc(SpaceController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

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
  naAddSpaceChild(con->space, con->subSpace1, naMakePos(0, curPosY));
  naAddSpaceChild(con->space, con->subSpace2, naMakePos(300, curPosY));
  
  curPosY -= 30;
  naAddSpaceChild(con->space, con->subSpace3, naMakePos(0, curPosY));
  naAddSpaceChild(con->space, con->subSpace4, naMakePos(300, curPosY));

  return con;
}



void clearSpaceController(SpaceController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getSpaceControllerSpace(SpaceController* con){
  return con->space;
}



void updateSpaceController(SpaceController* con) {
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
