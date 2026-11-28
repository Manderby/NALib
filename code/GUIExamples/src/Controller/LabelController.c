
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"
#include "NAVisual/NAColor.h"



struct LabelController{
  CommonController comCon;

  NALabel* normalLabel;
  NALabel* label;
  
  NALabel* coloredLabel;
  NALabel* labelColor;

  NALabel* nonSelectableLabel;
  NALabel* labelNonSelectable;

  NALabel* linkLabel;
  NALabel* labelLink;

  NALabel* fontLabel;
  NALabel* labelFont;

  NALabel* multiLineLabel;
  NALabel* labelMultiLine;

  NALabel* disabledLabel;
  NALabel* labelDisabled;
};



CommonController* createLabelController() {
  LabelController* con = naAlloc(LabelController);

  NASpace* space = naNewSpace();
  naSetUIElementRect(space, naMakeRectS(0., 0., WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalLabel = naNewLabel("Normal Label");
  naSetLabelFont(con->normalLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->normalLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->label = naNewLabel("I am a label");
  naAddSpaceChild(
    space,
    con->label,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->coloredLabel = naNewLabel("Colored Label");
  naSetLabelFont(con->coloredLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->coloredLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  NAColor textColor;
  naFillColorWithSRGB(&textColor, .8f, .5f, .1f, 1.f);

  con->labelColor = naNewLabel("I am colorful");
  naSetLabelTextColor(con->labelColor, &textColor);
  naAddSpaceChild(
    space,
    con->labelColor,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nonSelectableLabel = naNewLabel("Non-Selectable Label");
  naSetLabelFont(con->nonSelectableLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->nonSelectableLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->labelNonSelectable = naNewLabel("I am non-selectable");
  naSetLabelSelectable(con->labelNonSelectable, NA_FALSE);
  naAddSpaceChild(
    space,
    con->labelNonSelectable,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->linkLabel = naNewLabel("Label with Link");
  naSetLabelFont(con->linkLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->linkLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->labelLink = naNewLabel("I am a link");
  naSetLabelLink(con->labelLink, "https://manderc.com");
  naAddSpaceChild(
    space,
    con->labelLink,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  NAFont* paragraphFont = naCreateFontWithPreset(NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_DEFAULT);

  con->fontLabel = naNewLabel("Label with Font");
  naSetLabelFont(con->fontLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->fontLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->labelFont = naNewLabel("I am pretty");
  naSetLabelFont(con->labelFont, paragraphFont);
  naAddSpaceChild(
    space,
    con->labelFont,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  naRelease(paragraphFont);

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH - SPACE_MARGIN_V;

  con->multiLineLabel = naNewLabel("Multi-Line Label");
  naSetLabelFont(con->multiLineLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->multiLineLabel,
    naMakePos(WINDOW_MARGIN, curPosY + UI_ELEMENT_HEIGTH + SPACE_MARGIN_V),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->labelMultiLine = naNewLabel("The meerkat (Suricata suricatta) or suricate is a small mongoose found in southern Africa.");
  naSetLabelHeight(con->labelMultiLine, 2 * UI_ELEMENT_HEIGTH);
  naAddSpaceChild(
    space,
    con->labelMultiLine,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->disabledLabel = naNewLabel("Disabled");
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(
    space,
    con->disabledLabel,
    naMakePos(WINDOW_MARGIN, curPosY),
    naMakeInlineSize(COLUMN0_WIDTH));

  con->labelDisabled = naNewLabel("I am disabled");
  naSetLabelEnabled(con->labelDisabled, NA_FALSE);
  naAddSpaceChild(
    space,
    con->labelDisabled,
    naMakePos(TAB1, curPosY),
    naMakeInlineSize(COLUMN1_WIDTH));

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
