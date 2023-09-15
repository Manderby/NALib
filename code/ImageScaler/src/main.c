#include "../../NALib/src/NALib.h"

// This is a test application showing the image scaling functionality
// of NALib.

typedef struct ImageTesterApplication ImageTesterApplication;
typedef struct ImageTesterController ImageTesterController;

struct ImageTesterApplication{
  NABabyImage* transparencyGridImage;
  NABabyImage* featherImage;
  NABabyImage* motorImage;

  ImageTesterController* imageTestController;
};

struct ImageTesterController{
  NAWindow* window;
  NAImageSpace* imageSpace;

  NALabel* topLabel;
  NAPopupButton* topPopupButton;
  NAMenuItem* featherItem;
  NAMenuItem* motorItem;
  NAMenuItem* redColorItem;
  
  NALabel* scaleLabel;
  NASlider* scaleSlider;

  NALabel* blendLabel;
  NASlider* blendSlider;

  const NABabyImage* transparencyGridImage;
  const NABabyImage* topImage;
  NAPosi center;
};

ImageTesterApplication* imageTestApplication = NA_NULL;



ImageTesterController* naAllocImageTestController(void);
void naDeallocImageTestController(ImageTesterController* con);
NABool topSelected(NAReaction reaction);
NABool sliderEdited(NAReaction reaction);
NABool mouseMoved(NAReaction reaction);
void updateImage(ImageTesterController* con);



ImageTesterApplication* naAllocImageTestApplication(){
  ImageTesterApplication* app = naAlloc(ImageTesterApplication);

  NAPNG* gridPNG = naNewPNGWithPath("res/transparencyGrid.png");
  NABabyImage* gridImage = naCreateBabyImageFromPNG(gridPNG);
  NASizei gridSize = naGetBabyImageSize(gridImage);
  app->transparencyGridImage = naCreateBabyImageWithResize(gridImage, naMakeSizei(gridSize.width * 2, gridSize.height * 2));
  naReleaseBabyImage(gridImage);

  NAPNG* featherPNG = naNewPNGWithPath("res/feather.png");
  app->featherImage = naCreateBabyImageFromPNG(featherPNG);
  NAPNG* motorPNG = naNewPNGWithPath("res/motor.png");
  app->motorImage = naCreateBabyImageFromPNG(motorPNG);

  app->imageTestController = naAllocImageTestController();

  app->imageTestController->topImage = app->featherImage;
  app->imageTestController->transparencyGridImage = app->transparencyGridImage;
  updateImage(app->imageTestController);

  return app;
}



void naDeallocImageTestApplication(ImageTesterApplication* app){
  naDeallocImageTestController(app->imageTestController);
  naReleaseBabyImage(app->transparencyGridImage);
  naReleaseBabyImage(app->featherImage);
  naReleaseBabyImage(app->motorImage);
  naFree(app);
}



ImageTesterController* naAllocImageTestController(){
  ImageTesterController* con = naAlloc(ImageTesterController);
  con->center.x = 0;
  con->center.y = 0;

  con->window = naNewWindow(
    "ImageScaler",
    naMakeRectS(100, 100, 830, 420),
    NA_WINDOW_FIXED_SIZE | NA_WINDOW_DEFAULT | NA_WINDOW_TITLED | NA_WINDOW_CLOSEABLE | NA_WINDOW_MINIATURIZEABLE,
    0);

  NASpace* contentSpace = naGetWindowContentSpace(con->window);

  con->transparencyGridImage = NA_NULL;
  con->topImage = NA_NULL;

  con->imageSpace = naNewImageSpace(NA_NULL, naMakeSize(600, 400));

  con->topLabel = naNewLabel("Top:", 200);
  con->topPopupButton = naNewPopupButton(200);

  con->blendLabel = naNewLabel("Blend:", 200);
  con->blendSlider = naNewSlider(200);
  naSetSliderRange(con->blendSlider, 0., 1., 0);
  naSetSliderValue(con->blendSlider, 1.);

  con->scaleLabel = naNewLabel("Scale:", 200);
  con->scaleSlider = naNewSlider(200);
  naSetSliderRange(con->scaleSlider, 0., 2., 0);
  naSetSliderValue(con->scaleSlider, 1.);



  naAddSpaceChild(contentSpace, con->imageSpace, naMakePos(10, 10));

  naAddSpaceChild(contentSpace, con->topLabel, naMakePos(620, 325));
  naAddSpaceChild(contentSpace, con->topPopupButton, naMakePos(620, 300));
  con->featherItem = naNewMenuItem("Feather");
  con->motorItem = naNewMenuItem("Motor");
  con->redColorItem = naNewMenuItem("Red");
  naAddPopupButtonMenuItem(con->topPopupButton, con->featherItem, NA_NULL);
  naAddPopupButtonMenuItem(con->topPopupButton, con->motorItem, NA_NULL);
  naAddPopupButtonMenuItem(con->topPopupButton, con->redColorItem, NA_NULL);

  naAddSpaceChild(contentSpace, con->blendLabel, naMakePos(620, 275));
  naAddSpaceChild(contentSpace, con->blendSlider, naMakePos(620, 250));

  naAddSpaceChild(contentSpace, con->scaleLabel, naMakePos(620, 225));
  naAddSpaceChild(contentSpace, con->scaleSlider, naMakePos(620, 200));

  naAddUIReaction(con->featherItem, NA_UI_COMMAND_PRESSED, topSelected, con);
  naAddUIReaction(con->motorItem, NA_UI_COMMAND_PRESSED, topSelected, con);
  naAddUIReaction(con->redColorItem, NA_UI_COMMAND_PRESSED, topSelected, con);
  naAddUIReaction(con->blendSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);
  naAddUIReaction(con->scaleSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);
  naAddUIReaction(con->imageSpace, NA_UI_COMMAND_MOUSE_MOVED, mouseMoved, con);

  naShowWindow(con->window);

  return con;
}



void naDeallocImageTestController(ImageTesterController* con){
  naReleaseBabyImage(con->transparencyGridImage);
  naReleaseBabyImage(con->transparencyGridImage);
  naFree(con);
}



void updateImage(ImageTesterController* con){
  if(!con->transparencyGridImage || !con->topImage)
    return;

  float alpha = (float)naGetSliderValue(con->blendSlider);
  double scale = naGetSliderValue(con->scaleSlider);

  NASizei originalSize = naGetBabyImageSize(con->topImage);
  NASizei newSize = naMakeSizeiE((NAInt)(scale * originalSize.width), (NAInt)(scale * originalSize.height));
  if(naIsSizeiUseful(newSize)){
    NABabyImage* scaledImage = naCreateBabyImageWithResize(con->topImage, newSize);
    NASizei baseSize = naGetBabyImageSize(con->transparencyGridImage);
    NASize spaceSize = naGetUIElementRect(con->imageSpace).size;

    NAPosi origin = naMakePosi(
      con->center.x - (NAInt)((newSize.width / 2.) + ((spaceSize.width - baseSize.width) / 2.)),
      con->center.y - (NAInt)((newSize.height / 2.) + ((spaceSize.height - baseSize.height) / 2.)));

    NABabyImage* fullImage = naCreateBabyImageWithBlend(
      con->transparencyGridImage,
      scaledImage,
      NA_BLEND_OVERLAY,
      alpha,
      origin);
    NAUIImage* uiImage = naCreateUIImage(
      fullImage,
      NA_UIIMAGE_RESOLUTION_SCREEN_1x,
      NA_BLEND_ZERO);
    naSetImageSpaceImage(con->imageSpace, uiImage);
    naRelease(uiImage);
    naReleaseBabyImage(fullImage);
    naReleaseBabyImage(scaledImage);
  }
}



NABool topSelected(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  if(reaction.uiElement == con->featherItem){
    con->topImage = imageTestApplication->featherImage;
  }else if(reaction.uiElement == con->motorItem){
    con->topImage = imageTestApplication->motorImage;
  }
  con->transparencyGridImage = imageTestApplication->transparencyGridImage;
  updateImage(con);

  return NA_TRUE;
}



NABool sliderEdited(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  updateImage(con);
  return NA_TRUE;
}



NABool mouseMoved(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  const NAMouseStatus* mouse = naGetMouseStatus();
  NARect spaceRect = naGetUIElementRectAbsolute(reaction.uiElement);
  con->center.x = (NAInt)(mouse->pos.x - spaceRect.pos.x);
  con->center.y = (NAInt)(mouse->pos.y - spaceRect.pos.y);
  updateImage(con);
  return NA_TRUE;
}



void postStartup(void* arg){
  NA_UNUSED(arg);
  imageTestApplication = naAllocImageTestApplication();
}



int main(void){
 
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, NA_NULL);
  naDeallocImageTestApplication(imageTestApplication);
  naStopRuntime();
    
  return 0;
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
