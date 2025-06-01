
#include "../GUIExamples.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"
#include "NAVisual/NAColor.h"



struct LabelController{
  NASpace* space;

  NALabel* enabledLabel;
  NALabel* disabledLabel;

  NALabel* normalLabel;
  NALabel* label;
  NALabel* labelDisabled;
  
  NALabel* coloredLabel;
  NALabel* labelColor;
  NALabel* labelColorDisabled;

  NALabel* nonSelectableLabel;
  NALabel* labelNonSelectable;
  NALabel* labelNonSelectableDisabled;

  NALabel* linkLabel;
  NALabel* labelLink;
  NALabel* labelLinkDisabled;

  NALabel* fontLabel;
  NALabel* labelFont;
  NALabel* labelFontDisabled;

  NALabel* multiLineLabel;
  NALabel* labelMultiLine;
  NALabel* labelMultiLineDisabled;
};



LabelController* createLabelController(){
  LabelController* con = naAlloc(LabelController);

  con->space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN;

  NAFont* titleFont = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_DEFAULT);

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->enabledLabel = naNewLabel("Enabled", COLUMN1_WIDTH);
  naSetLabelTextAlignment(con->enabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelFont(con->enabledLabel, titleFont);
  naAddSpaceChild(con->space, con->enabledLabel, naMakePos(TAB1, curPosY));
  con->disabledLabel = naNewLabel("Disabled", COLUMN2_WIDTH);
  naSetLabelTextAlignment(con->disabledLabel, NA_TEXT_ALIGNMENT_CENTER);
  naSetLabelFont(con->disabledLabel, titleFont);
  naAddSpaceChild(con->space, con->disabledLabel, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->normalLabel = naNewLabel("Normal Label", COLUMN0_WIDTH);
  naSetLabelFont(con->normalLabel, titleFont);
  naAddSpaceChild(con->space, con->normalLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->label = naNewLabel("I am a label", COLUMN1_WIDTH);
  naAddSpaceChild(con->space, con->label, naMakePos(TAB1, curPosY));
  con->labelDisabled = naNewLabel("I am a label", COLUMN2_WIDTH);
  naSetLabelEnabled(con->labelDisabled, NA_FALSE);
  naAddSpaceChild(con->space, con->labelDisabled, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->coloredLabel = naNewLabel("Colored Label", COLUMN0_WIDTH);
  naSetLabelFont(con->coloredLabel, titleFont);
  naAddSpaceChild(con->space, con->coloredLabel, naMakePos(WINDOW_MARGIN, curPosY));

  NAColor textColor;
  naFillColorWithSRGB(&textColor, .8f, .5f, .1f, 1.f);

  con->labelColor = naNewLabel("I am colorful", COLUMN1_WIDTH);
  naSetLabelTextColor(con->labelColor, &textColor);
  naAddSpaceChild(con->space, con->labelColor, naMakePos(TAB1, curPosY));
  con->labelColorDisabled = naNewLabel("I am colorful", COLUMN2_WIDTH);
  naSetLabelTextColor(con->labelColorDisabled, &textColor);
  naSetLabelEnabled(con->labelColorDisabled, NA_FALSE);
  naAddSpaceChild(con->space, con->labelColorDisabled, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->nonSelectableLabel = naNewLabel("Non-Selectable Label", COLUMN0_WIDTH);
  naSetLabelFont(con->nonSelectableLabel, titleFont);
  naAddSpaceChild(con->space, con->nonSelectableLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->labelNonSelectable = naNewLabel("I am non-selectable", COLUMN1_WIDTH);
  naSetLabelSelectable(con->labelNonSelectable, NA_FALSE);
  naAddSpaceChild(con->space, con->labelNonSelectable, naMakePos(TAB1, curPosY));
  con->labelNonSelectableDisabled = naNewLabel("I am non-selectable", COLUMN2_WIDTH);
  naSetLabelSelectable(con->labelNonSelectableDisabled, NA_FALSE);
  naSetLabelEnabled(con->labelNonSelectableDisabled, NA_FALSE);
  naAddSpaceChild(con->space, con->labelNonSelectableDisabled, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->linkLabel = naNewLabel("Label with Link", COLUMN0_WIDTH);
  naSetLabelFont(con->linkLabel, titleFont);
  naAddSpaceChild(con->space, con->linkLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->labelLink = naNewLabel("I am a link", COLUMN1_WIDTH);
  naSetLabelLink(con->labelLink, "https://manderc.com");
  naAddSpaceChild(con->space, con->labelLink, naMakePos(TAB1, curPosY));
  con->labelLinkDisabled = naNewLabel("I am a link", COLUMN2_WIDTH);
  naSetLabelLink(con->labelLinkDisabled, "https://manderc.com");
  naSetLabelEnabled(con->labelLinkDisabled, NA_FALSE);
  naAddSpaceChild(con->space, con->labelLinkDisabled, naMakePos(TAB2, curPosY));

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  NAFont* paragraphFont = naCreateFontWithPreset(NA_FONT_KIND_PARAGRAPH, NA_FONT_SIZE_DEFAULT);

  con->fontLabel = naNewLabel("Label with Font", COLUMN0_WIDTH);
  naSetLabelFont(con->fontLabel, titleFont);
  naAddSpaceChild(con->space, con->fontLabel, naMakePos(WINDOW_MARGIN, curPosY));

  con->labelFont = naNewLabel("I am pretty", COLUMN1_WIDTH);
  naSetLabelFont(con->labelFont, paragraphFont);
  naAddSpaceChild(con->space, con->labelFont, naMakePos(TAB1, curPosY));
  con->labelFontDisabled = naNewLabel("I am pretty", COLUMN2_WIDTH);
  naSetLabelFont(con->labelFontDisabled, paragraphFont);
  naSetLabelEnabled(con->labelFontDisabled, NA_FALSE);
  naAddSpaceChild(con->space, con->labelFontDisabled, naMakePos(TAB2, curPosY));

  naRelease(paragraphFont);

  curPosY = curPosY - 2 * UI_ELEMENT_HEIGTH - SPACE_MARGIN;

  con->multiLineLabel = naNewLabel("Multi-Line Label", COLUMN0_WIDTH);
  naSetLabelFont(con->multiLineLabel, titleFont);
  naAddSpaceChild(con->space, con->multiLineLabel, naMakePos(WINDOW_MARGIN, curPosY + UI_ELEMENT_HEIGTH + SPACE_MARGIN));

  con->labelMultiLine = naNewLabel("The meerkat (Suricata suricatta) or suricate is a small mongoose found in southern Africa.", COLUMN1_WIDTH);
  naSetLabelHeight(con->labelMultiLine, 2 * UI_ELEMENT_HEIGTH);
  naAddSpaceChild(con->space, con->labelMultiLine, naMakePos(TAB1, curPosY));
  con->labelMultiLineDisabled = naNewLabel("The meerkat (Suricata suricatta) or suricate is a small mongoose found in southern Africa.", COLUMN2_WIDTH);
  naSetLabelHeight(con->labelMultiLineDisabled, 2 * UI_ELEMENT_HEIGTH);
  naSetLabelEnabled(con->labelMultiLineDisabled, NA_FALSE);
  naAddSpaceChild(con->space, con->labelMultiLineDisabled, naMakePos(TAB2, curPosY));


  naRelease(titleFont);

  return con;
}



void clearLabelController(LabelController* con){
  naDelete(con->space);
  naFree(con);
}



NASpace* getLabelControllerSpace(LabelController* con){
  return con->space;
}



void updateLabelController(LabelController* con) {
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
