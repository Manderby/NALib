
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct MenuController{
  CommonController comCon;

  NAButton* button;
  NAMenu* menu;

  NALabel* outputLabel;
};

// Prototype:
void clearMenuController(MenuController* con);


static void menuButtonPressed(NAReaction reaction) {
  MenuController* con = reaction.controller;
  
  NARect rect = naGetUIElementRectAbsolute(con->button);
  NAPos menuPos = rect.pos;
  menuPos.x += rect.size.width;
  menuPos.y += rect.size.height;

  naSetLabelText(con->outputLabel, "Menu button pressed");
  naPresentMenu(con->menu, menuPos, con->button);
}



static void menuItemSelected(NAReaction reaction) {
  MenuController* con = reaction.controller;
  const NAUTF8Char* outputText = naAllocSprintf(
    NA_TRUE,
    "MenuItem with index %d selected",
    (int)naGetMenuItemIndex(con->menu, reaction.uiElement));
  naSetLabelText(con->outputLabel, outputText);
}



CommonController* createMenuController() {
  MenuController* con = naAlloc(MenuController);

  NASpace* space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  curPosY = curPosY - UI_ELEMENT_HEIGTH;

  con->button = naNewTextPushButton("Push for Menu", COLUMN1_WIDTH);
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, menuButtonPressed, con);
  naAddSpaceChild(space, con->button, naMakePos(TAB1, curPosY));
  con->menu = naNewMenu();  
  NAMenuItem* menuItem0 = naNewMenuItem("You are Winner", NA_NULL);
  NAMenuItem* menuItem1 = naNewMenuItem("Kohle, Kohle, Kohle", NA_NULL);
  NAMenuItem* menuItem2 = naNewMenuItem("I am Groot", NA_NULL);
  NAMenuItem* menuItem3 = naNewMenuItem("None of that Objective-C rubbish", NA_NULL);
  NAMenuItem* menuItem4 = naNewMenuItem("Bread crumbs and beaver spit", NA_NULL);
  NAMenuItem* menuSeparator = naNewMenuSeparator();
  naAddMenuItem(con->menu, menuItem0, NA_NULL);
  naAddMenuItem(con->menu, menuItem1, NA_NULL);
  naAddMenuItem(con->menu, menuItem2, menuItem0);
  naAddMenuItem(con->menu, menuItem3, NA_NULL);
  naAddMenuItem(con->menu, menuItem4, NA_NULL);
  naAddMenuItem(con->menu, menuSeparator, menuItem3);
  naAddUIReaction(menuItem0, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem1, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem2, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem3, NA_UI_COMMAND_PRESSED, menuItemSelected, con);
  naAddUIReaction(menuItem4, NA_UI_COMMAND_PRESSED, menuItemSelected, con);

  con->outputLabel = naNewLabel(
    "Here will be the output of any operation.",
    WINDOW_WIDTH - 2 * WINDOW_MARGIN);
  naSetLabelFont(con->outputLabel, getMonoFont());
  naAddSpaceChild(space, con->outputLabel, naMakePos(WINDOW_MARGIN, WINDOW_MARGIN));

  initCommonController(
    &con->comCon,
    space,
    clearMenuController,
    NA_NULL);

  return (CommonController*)con;
}



void clearMenuController(MenuController* con) {
  naDelete(con->menu);
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
