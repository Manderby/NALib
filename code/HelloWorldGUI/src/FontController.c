
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.

#include "HelloWorldGUI.h"



struct FontController{
  NAWindow* window;
};



FontController* createFontController(){
  FontController* con = naAlloc(FontController);

  NARect windowRect = naMakeRectS(100, 800, 400, 300);
  con->window = naNewWindow("Fonts", windowRect, NA_FALSE, 0);
  NASpace* windowSpace = naGetWindowContentSpace(con->window);

  NALabel* label;

  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_SMALL);
  naAddSpaceChild(windowSpace, label, naMakePos(10, 260));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_DEFAULT);
  naAddSpaceChild(windowSpace, label, naMakePos(110, 260));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_BIG);
  naAddSpaceChild(windowSpace, label, naMakePos(210, 260));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_HUGE);
  naAddSpaceChild(windowSpace, label, naMakePos(310, 260));

  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_TITLE, NA_FONT_SIZE_SMALL);
  naAddSpaceChild(windowSpace, label, naMakePos(10, 210));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_TITLE, NA_FONT_SIZE_DEFAULT);
  naAddSpaceChild(windowSpace, label, naMakePos(110, 210));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_TITLE, NA_FONT_SIZE_BIG);
  naAddSpaceChild(windowSpace, label, naMakePos(210, 210));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_TITLE, NA_FONT_SIZE_HUGE);
  naAddSpaceChild(windowSpace, label, naMakePos(310, 210));

  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_SMALL);
  naAddSpaceChild(windowSpace, label, naMakePos(10, 160));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naAddSpaceChild(windowSpace, label, naMakePos(110, 160));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_BIG);
  naAddSpaceChild(windowSpace, label, naMakePos(210, 160));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_HUGE);
  naAddSpaceChild(windowSpace, label, naMakePos(310, 160));

  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_SMALL);
  naAddSpaceChild(windowSpace, label, naMakePos(10, 110));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_DEFAULT);
  naAddSpaceChild(windowSpace, label, naMakePos(110, 110));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_BIG);
  naAddSpaceChild(windowSpace, label, naMakePos(210, 110));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_HUGE);
  naAddSpaceChild(windowSpace, label, naMakePos(310, 110));

  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MATH, NA_FONT_SIZE_SMALL);
  naAddSpaceChild(windowSpace, label, naMakePos(10, 60));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MATH, NA_FONT_SIZE_DEFAULT);
  naAddSpaceChild(windowSpace, label, naMakePos(110, 60));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MATH, NA_FONT_SIZE_BIG);
  naAddSpaceChild(windowSpace, label, naMakePos(210, 60));
  label = naNewLabel(u8"Hello 你好", 100);
  naSetLabelFontKind(label, NA_FONT_KIND_MATH, NA_FONT_SIZE_HUGE);
  naAddSpaceChild(windowSpace, label, naMakePos(310, 60));

  naShowWindow(con->window);

  return con;
}



void clearFontController(FontController* con){
  NA_UNUSED(con);
}



void showFontController(FontController* con){
  naShowWindow(con->window);
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
