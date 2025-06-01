
#include "GUIExamples.h"
#include "Layout.h"
#include "NAUtility/NAMemory.h"



struct FontController{
  NASpace* space;
};



#define HAMBURGEFONTSIV u8"H\u00e4xig \u4f60\u597d"



FontController* createFontController(){
  FontController* con = naAlloc(FontController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  NALabel* label;
  NAFont* font;

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_SMALL);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 12.);
  naAddSpaceChild(con->space, label, naMakePos(10, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(label, font);
  naRelease(font);
  naAddSpaceChild(con->space, label, naMakePos(110, curPosY));
   
  label = naNewLabel(HAMBURGEFONTSIV, 120);
  font = naCreateFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_BIG);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 20.);
  naAddSpaceChild(con->space, label, naMakePos(210, curPosY));
   
  label = naNewLabel(HAMBURGEFONTSIV, 150);
  font = naCreateFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_HUGE);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 24.);
  naAddSpaceChild(con->space, label, naMakePos(350, curPosY));



  curPosY = curPosY - 50;

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_SMALL);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 12.);
  naAddSpaceChild(con->space, label, naMakePos(10, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(label, font);
  naRelease(font);
  naAddSpaceChild(con->space, label, naMakePos(110, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 120);
  font = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_BIG);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 20.);
  naAddSpaceChild(con->space, label, naMakePos(210, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 150);
  font = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_HUGE);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 24.);
  naAddSpaceChild(con->space, label, naMakePos(350, curPosY));



  curPosY = curPosY - 50;

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_SMALL);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 12.);
  naAddSpaceChild(con->space, label, naMakePos(10, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(label, font);
  naRelease(font);
  naAddSpaceChild(con->space, label, naMakePos(110, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 120);
  font = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_BIG);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 20.);
  naAddSpaceChild(con->space, label, naMakePos(210, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 150);
  font = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_HUGE);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 24.);
  naAddSpaceChild(con->space, label, naMakePos(350, curPosY));



  curPosY = curPosY - 50;

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_SMALL);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 12.);
  naAddSpaceChild(con->space, label, naMakePos(10, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(label, font);
  naRelease(font);
  naAddSpaceChild(con->space, label, naMakePos(110, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 120);
  font = naCreateFontWithPreset(NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_BIG);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 20.);
  naAddSpaceChild(con->space, label, naMakePos(210, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 150);
  font = naCreateFontWithPreset(NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_HUGE);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 24.);
  naAddSpaceChild(con->space, label, naMakePos(350, curPosY));



  curPosY = curPosY - 50;

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_SMALL);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 12.);
  naAddSpaceChild(con->space, label, naMakePos(10, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 100);
  font = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(label, font);
  naRelease(font);
  naAddSpaceChild(con->space, label, naMakePos(110, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 120);
  font = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_BIG);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 20.);
  naAddSpaceChild(con->space, label, naMakePos(210, curPosY));

  label = naNewLabel(HAMBURGEFONTSIV, 150);
  font = naCreateFontWithPreset(NA_FONT_KIND_MATH, NA_FONT_SIZE_HUGE);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 24.);
  naAddSpaceChild(con->space, label, naMakePos(350, curPosY));



  curPosY = curPosY - 60;

  label = naNewLabel(u8"Comic Sans", 200);
  font = naCreateFont("Comic Sans MS", NA_FONT_FLAG_REGULAR, 32);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 50);
  naAddSpaceChild(con->space, label, naMakePos(10, curPosY));

  label = naNewLabel(u8"Comic Sans Link", 250);
  font = naCreateFont("Comic Sans MS", NA_FONT_FLAG_REGULAR, 32);
  naSetLabelFont(label, font);
  naRelease(font);
  naSetLabelHeight(label, 50);
  naSetLabelLink(label, "https://manderc.com");
  naAddSpaceChild(con->space, label, naMakePos(210, curPosY));

  return con;
}



void clearFontController(FontController* con){
  NA_UNUSED(con);
}



NASpace* getFontControllerSpace(FontController* con){
  return con->space;
}



void updateFontController(FontController* con) {
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
