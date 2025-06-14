
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

  NASpace* space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalLabel = naNewLabel("Normal Label", COLUMN0_WIDTH);
  naSetLabelFont(con->normalLabel, getTitleFont());
  naAddSpaceChild(space, con->normalLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->label = naNewLabel("I am a label", COLUMN1_WIDTH);
  naAddSpaceChild(space, con->label, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->coloredLabel = naNewLabel("Colored Label", COLUMN0_WIDTH);
  naSetLabelFont(con->coloredLabel, getTitleFont());
  naAddSpaceChild(space, con->coloredLabel, naMakePos(WINDOW_MARGIN, curPosY));

  NAColor textColor;
  naFillColorWithSRGB(&textColor, .8f, .5f, .1f, 1.f);

  con->labelColor = naNewLabel("I am colorful", COLUMN1_WIDTH);
  naSetLabelTextColor(con->labelColor, &textColor);
  naAddSpaceChild(space, con->labelColor, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nonSelectableLabel = naNewLabel("Non-Selectable Label", COLUMN0_WIDTH);
  naSetLabelFont(con->nonSelectableLabel, getTitleFont());
  naAddSpaceChild(space, con->nonSelectableLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->labelNonSelectable = naNewLabel("I am non-selectable", COLUMN1_WIDTH);
  naSetLabelSelectable(con->labelNonSelectable, NA_FALSE);
  naAddSpaceChild(space, con->labelNonSelectable, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->linkLabel = naNewLabel("Label with Link", COLUMN0_WIDTH);
  naSetLabelFont(con->linkLabel, getTitleFont());
  naAddSpaceChild(space, con->linkLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->labelLink = naNewLabel("I am a link", COLUMN1_WIDTH);
  naSetLabelLink(con->labelLink, "https://manderc.com");
  naAddSpaceChild(space, con->labelLink, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  NAFont* paragraphFont = naCreateFontWithPreset(NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_DEFAULT);

  con->fontLabel = naNewLabel("Label with Font", COLUMN0_WIDTH);
  naSetLabelFont(con->fontLabel, getTitleFont());
  naAddSpaceChild(space, con->fontLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->labelFont = naNewLabel("I am pretty", COLUMN1_WIDTH);
  naSetLabelFont(con->labelFont, paragraphFont);
  naAddSpaceChild(space, con->labelFont, naMakePos(TAB1, curPosY));

  naRelease(paragraphFont);

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH - SPACE_MARGIN_V;

  con->multiLineLabel = naNewLabel("Multi-Line Label", COLUMN0_WIDTH);
  naSetLabelFont(con->multiLineLabel, getTitleFont());
  naAddSpaceChild(space, con->multiLineLabel, naMakePos(WINDOW_MARGIN, curPosY + UI_ELEMENT_HEIGTH + SPACE_MARGIN_V));

  con->labelMultiLine = naNewLabel("The meerkat (Suricata suricatta) or suricate is a small mongoose found in southern Africa.", COLUMN1_WIDTH);
  naSetLabelHeight(con->labelMultiLine, 2 * UI_ELEMENT_HEIGTH);
  naAddSpaceChild(space, con->labelMultiLine, naMakePos(TAB1, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->disabledLabel = naNewLabel("Disabled", COLUMN0_WIDTH);
  naSetLabelFont(con->disabledLabel, getTitleFont());
  naAddSpaceChild(space, con->disabledLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->labelDisabled = naNewLabel("I am disabled", COLUMN1_WIDTH);
  naSetLabelEnabled(con->labelDisabled, NA_FALSE);
  naAddSpaceChild(space, con->labelDisabled, naMakePos(TAB1, curPosY));

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
