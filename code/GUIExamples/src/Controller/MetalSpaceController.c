
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct MetalSpaceController{
  NASpace* space;

  NALabel* dummyLabel;
};



MetalSpaceController* createMetalSpaceController(){
  MetalSpaceController* con = naAlloc(MetalSpaceController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->dummyLabel = naNewLabel("NAMetalSpace examples not available yet", 400);
  naAddSpaceChild(con->space, con->dummyLabel, naMakePos(WINDOW_MARGIN, curPosY));

  return con;
}



void clearMetalSpaceController(MetalSpaceController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getMetalSpaceControllerSpace(MetalSpaceController* con){
  return con->space;
}



void updateMetalSpaceController(MetalSpaceController* con) {
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
