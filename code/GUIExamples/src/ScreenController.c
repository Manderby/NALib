
#include "GUIExamples.h"
#include "NAUtility/NAString.h"
#include "NAApp/NAApp.h"
#include "NAVisual/NA3DHelper.h"


struct ScreenController{
  NAWindow* window;

  NAOpenGLSpace* display;

  NALabel* totalRectTitle;
  NALabel* totalRectLabel;

  NALabel* outputLabel;
};



void udpateScreenController(ScreenController* con);



void applicationReshaped(NAReaction reaction) {
  ScreenController* con = reaction.controller;
  
  udpateScreenController(con);
}



void redrawDisplayReaction(NAReaction reaction) {
  ScreenController* con = reaction.controller;

  double uiScale = naGetUIElementResolutionScale(con->display);
  NASize viewSize = naGetUIElementRect(reaction.uiElement).size;
  glViewport(
    0,
    0,
    (GLsizei)(viewSize.width * uiScale),
    (GLsizei)(viewSize.height * uiScale));

  glClearColor(0.f, 0.f, .4f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_TEXTURE_2D);

  glMatrixMode(GL_PROJECTION);
  NAMat44d ortho;
  NARect totalRect = naGetUIElementRect(naGetApplication());
  NAPos totalCenter = naGetRectCenter(totalRect);
  if(totalRect.size.width / totalRect.size.height > viewSize.width / viewSize.height) {
    double margin = totalRect.size.width * .05;
    naFillMatrixOrtho(
      ortho,
      totalCenter.x - 0.5 * totalRect.size.width - margin,
      totalCenter.x + 0.5 * totalRect.size.width + margin,
      totalCenter.y - 0.5 * totalRect.size.width * (viewSize.height / viewSize.width) - margin,
      totalCenter.y + 0.5 * totalRect.size.width * (viewSize.height / viewSize.width) + margin,
      -1., 1.);
  }else{
    double margin = totalRect.size.height * .05;
    naFillMatrixOrtho(
      ortho,
      totalCenter.x - 0.5 * totalRect.size.height * (viewSize.width / viewSize.height) - margin,
      totalCenter.x + 0.5 * totalRect.size.height * (viewSize.width / viewSize.height) + margin,
      totalCenter.y - 0.5 * totalRect.size.height - margin,
      totalCenter.y + 0.5 * totalRect.size.height + margin,
      -1., 1.);
  }
  glLoadIdentity();
  glMultMatrixd(ortho);

  glLineWidth(1);
  glColor4f(.25f, .25f, .5f, 1.f);
    glBegin(GL_TRIANGLE_STRIP);
      glVertex2d(totalRect.pos.x, totalRect.pos.y);
      glVertex2d(naGetRectEndX(totalRect), totalRect.pos.y);
      glVertex2d(totalRect.pos.x, naGetRectEndY(totalRect));
      glVertex2d(naGetRectEndX(totalRect), naGetRectEndY(totalRect));
    glEnd();
  glColor4f(.75f, .75f, .75f, 1.f);
  glBegin(GL_LINES);
    glVertex2d(-100000., 0.);
    glVertex2d(+100000., 0.);
    glVertex2d(0., -100000.);
    glVertex2d(0., +100000.);
  glEnd();
  glPointSize(10.);
  glBegin(GL_POINTS);
    glVertex2d(0., 0.);
  glEnd();

  glLineWidth(5);
  size_t screenCount = naGetApplicationScreenCount();
  for(size_t i = 0; i < screenCount; ++i) {
    const NAScreen* screen = naGetApplicationScreenWithIndex(i);

    NARect usableRect = naGetScreenUsableRect(screen);
    glColor4f(.25f, .5f, .25f, 1.f);
    glBegin(GL_TRIANGLE_STRIP);
      glVertex2d(usableRect.pos.x, usableRect.pos.y);
      glVertex2d(naGetRectEndX(usableRect), usableRect.pos.y);
      glVertex2d(usableRect.pos.x, naGetRectEndY(usableRect));
      glVertex2d(naGetRectEndX(usableRect), naGetRectEndY(usableRect));
    glEnd();

    NARect rect = naGetUIElementRect(screen);
    glColor4f(.5f, .5f, 1.f, 1.f);
    glBegin(GL_LINE_LOOP);
      glVertex2d(rect.pos.x, rect.pos.y);
      glVertex2d(naGetRectEndX(rect), rect.pos.y);
      glVertex2d(naGetRectEndX(rect), naGetRectEndY(rect));
      glVertex2d(rect.pos.x, naGetRectEndY(rect));
    glEnd();
  }

  naSwapOpenGLSpaceBuffer(con->display);
}



ScreenController* createScreenController(){
  ScreenController* con = naAlloc(ScreenController);

  double windowWidth = 550;
  double windowHeight = 500;

  double labelWidth = 200;
  double left1 = 240;
  double left2 = 400;

  double buttonWidth = 120;
  double buttonHeight = 50;

  NARect windowRect = naMakeRectS(500, 400, windowWidth, windowHeight);
  con->window = naNewWindow("NAScreen", windowRect, 0);
  NASpace* contentSpace = naGetWindowContentSpace(con->window);

  double curPosY = windowHeight - 210;
  
  con->display = naNewOpenGLSpace(naMakeSize(windowWidth - 40, 200), NA_NULL, con);
  naAddUIReaction(con->display, NA_UI_COMMAND_REDRAW, redrawDisplayReaction, con);
  naAddSpaceChild(contentSpace, con->display, naMakePos(20, curPosY));
  
  curPosY -= 25;

  con->totalRectTitle = naNewLabel("Absolute application space:", labelWidth);
  con->totalRectLabel = naNewLabel("", labelWidth);
  naAddSpaceChild(contentSpace, con->totalRectTitle, naMakePos(20, curPosY));
  naAddSpaceChild(contentSpace, con->totalRectLabel, naMakePos(left1, curPosY));

  curPosY -= 25;
   
  con->outputLabel = naNewLabel( "Here will be the output of any operation.", windowWidth - 40);
  NAFont* monospaceFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);
  naSetLabelFont(con->outputLabel, monospaceFont);
  naRelease(monospaceFont);
  naAddSpaceChild(contentSpace, con->outputLabel, naMakePos(20, curPosY));

  // We capture the event when the screen setup is changing.
  naAddUIReaction(naGetApplication(), NA_UI_COMMAND_RESHAPE, applicationReshaped, con);

  return con;
}



void clearScreenController(ScreenController* con){
  NA_UNUSED(con);
}



void showScreenController(ScreenController* con){
  udpateScreenController(con);
  naShowWindow(con->window);
}



void udpateScreenController(ScreenController* con){
  naRefreshUIElement(con->display, 0.);

  NARect totalRect = naGetUIElementRect(naGetApplication());

  naSetLabelText(con->totalRectLabel,
    naAllocSprintf(NA_TRUE, "(%.0f, %.0f) - (%.0f, %.0f)",
      totalRect.pos.x,
      totalRect.pos.y,
      naGetRectEndX(totalRect),
      naGetRectEndY(totalRect)));
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
