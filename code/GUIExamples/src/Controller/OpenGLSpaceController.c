
#include "../GUIExamples.h"
#include "../CommonController.h"
#include "../Layout.h"
#include "NAUtility/NAMemory.h"



struct OpenGLSpaceController{
  CommonController comCon;

  size_t fontId;
  NAOpenGLSpace* openGLSpace;
  int openGLSpaceRefreshCount;
};

// Prototype:
void clearOpenGLSpaceController(OpenGLSpaceController* con);



static void initOpenGL(void* initData) {
  OpenGLSpaceController* con = (OpenGLSpaceController*)initData;
  con->fontId = naStartupPixelFont();
}



void redrawOpenGLSpace(NAReaction reaction) {
  // OpenGL is declared deprecated on macOS 10.14. These pragma directives
  // omit the nasty warnings. Do not forget the pragma pop at the end of this
  // function!
  #if NA_OS == NA_OS_MAC_OS_X
    #pragma GCC diagnostic push 
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  #endif

  OpenGLSpaceController* con = reaction.controller;
  con->openGLSpaceRefreshCount--;

  // Prepare the next refresh.
  if(con->openGLSpaceRefreshCount == 0) {
    naRefreshUIElement(con->openGLSpace, 1./ 60);
    con->openGLSpaceRefreshCount++;
  }

  static float ang = 0.f;

  ang += .01f;
  if(ang > NA_PI2f) {ang = 0.f;}


  double uiScale = naGetUIElementUIScale(con->openGLSpace);
  NASize viewSize = naGetUIElementRect(reaction.uiElement).size;
  glViewport(
    0,
    0,
    (GLsizei)(viewSize.width * uiScale),
    (GLsizei)(viewSize.height * uiScale));

  glClearColor(0.f, 0.f, .4f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_TEXTURE_2D);
  // Enable blending to render pixel font properly
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(1.f, .5f, 1.f, 1.f);
  glPointSize(5);
  glBegin(GL_POINTS);
    glVertex3f(naSinf(ang) * .9f, 0.f, 0.f);
  glEnd();

  naDrawASCIICharacters(con->fontId, "Hello World", (double)naSinf(ang) * .9, 0, 0, uiScale);
   
  naSwapOpenGLSpaceBuffer(con->openGLSpace);
  
  #if NA_OS == NA_OS_MAC_OS_X
    #pragma GCC diagnostic pop
  #endif
}


CommonController* createOpenGLSpaceController() {
  OpenGLSpaceController* con = naAlloc(OpenGLSpaceController);

  con->openGLSpaceRefreshCount = 1;

  NASpace* space = naNewSpace(naMakeSize(WINDOW_WIDTH, EXPERIMENT_HEIGHT));
  double curPosY = EXPERIMENT_HEIGHT - SPACE_MARGIN_V;

  #define OPENGL_DISPLAY_HEIGHT 100.

  curPosY = curPosY - SPACE_MARGIN_V - OPENGL_DISPLAY_HEIGHT;
  
  con->openGLSpace = naNewOpenGLSpace(naMakeSize(WINDOW_WIDTH - 2 * WINDOW_MARGIN, OPENGL_DISPLAY_HEIGHT), initOpenGL, con);
  naAddSpaceChild(space, con->openGLSpace, naMakePos(WINDOW_MARGIN, curPosY + SPACE_MARGIN_V));
  naAddUIReaction(con->openGLSpace, NA_UI_COMMAND_REDRAW, redrawOpenGLSpace, con);

  initCommonController(
    &con->comCon,
    space,
    (NAMutator)clearOpenGLSpaceController,
    NA_NULL);

  return (CommonController*)con;
}



void clearOpenGLSpaceController(OpenGLSpaceController* con) {
  naShutdownPixelFont(con->fontId);
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
