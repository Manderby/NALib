#include "../../NALib/src/NALib.h"

// This is a test application showing the image scaling functionality
// of NALib.

typedef struct ImageTesterApplication ImageTesterApplication;
typedef struct ImageTesterController ImageTesterController;

typedef enum{
  IMAGE_RAINBOW,
  IMAGE_CAT_PINK,
  IMAGE_CAT_BLUE,
  IMAGE_CAT_GREEN,
  IMAGE_FEATHER,
  IMAGE_MOTOR,
  IMAGE_COUNT
} Images;

typedef enum{
  COLOR_TRANSPARENT,
  COLOR_RED,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_CYAN,
  COLOR_BLUE,
  COLOR_MAGENTA,
  COLOR_COUNT
} Colors;

struct ImageTesterApplication{
  NABabyImage* transparencyGridImage;
  NABabyImage* images[IMAGE_COUNT];
  const NAUTF8Char* imageNames[IMAGE_COUNT];
  NABabyColor colors[COLOR_COUNT];
  NAUTF8Char* colorNames[COLOR_COUNT];

  ImageTesterController* imageTestController;
};

struct ImageTesterController{
  NAWindow* window;
  NAImageSpace* imageSpace;

  NALabel* topLabel;
  NAPopupButton* topPopupButton;

  NALabel* scaleLabel;
  NASlider* scaleSlider;

  NALabel* blendModeLabel;
  NAPopupButton* blendModePopupButton;

  NALabel* blendLabel;
  NASlider* blendSlider;

  NALabel* bottomLabel;
  NAPopupButton* bottomPopupButton;

  const NABabyImage* transparencyGridImage;
  const NABabyImage* topImage;
  NABabyColor topColor;
  const NABabyImage* bottomImage;
  NABabyColor bottomColor;
  NAPosi center;
  NABlendMode blendMode;
};

ImageTesterApplication* app = NA_NULL;



ImageTesterController* naAllocImageTestController(void);
void naDeallocImageTestController(ImageTesterController* con);
NABool topSelected(NAReaction reaction);
NABool bottomSelected(NAReaction reaction);
NABool blendModeSelected(NAReaction reaction);
NABool sliderEdited(NAReaction reaction);
NABool mouseMoved(NAReaction reaction);
void updateImage(ImageTesterController* con);



void addImage(size_t index, const NAUTF8Char* text, const NAUTF8Char* path){
  NAPNG* png = naNewPNGWithPath(path);
  app->images[index] = naCreateBabyImageFromPNG(png);
  app->imageNames[index] = text;
  naDelete(png);
}

void naStartImageTestApplication(){
  app = naAlloc(ImageTesterApplication);

  NAPNG* gridPNG = naNewPNGWithPath("res/transparencyGrid.png");
  NABabyImage* gridImage = naCreateBabyImageFromPNG(gridPNG);
  NASizei gridSize = naGetBabyImageSize(gridImage);
  app->transparencyGridImage = naCreateBabyImageWithResize(gridImage, naMakeSizei(gridSize.width * 2, gridSize.height * 2));
  naReleaseBabyImage(gridImage);

  addImage(IMAGE_RAINBOW, "Rainbow", "res/rainbow.png");
  addImage(IMAGE_CAT_PINK, "Cat Pink", "res/catPink.png");
  addImage(IMAGE_CAT_BLUE, "Cat Blue", "res/catBlue.png");
  addImage(IMAGE_CAT_GREEN, "Cat Green", "res/catGreen.png");
  addImage(IMAGE_FEATHER, "Feather", "res/feather.png");
  addImage(IMAGE_MOTOR, "Motor", "res/motor.png");

  naFillBabyColor(app->colors[COLOR_TRANSPARENT], 0., 0., 0., 0.);
  naFillBabyColor(app->colors[COLOR_RED], 1., 0., 0., 1.);
  naFillBabyColor(app->colors[COLOR_YELLOW], 1., 1., 0., 1.);
  naFillBabyColor(app->colors[COLOR_GREEN], 0., 1., 0., 1.);
  naFillBabyColor(app->colors[COLOR_CYAN], 0., 1., 1., 1.);
  naFillBabyColor(app->colors[COLOR_BLUE], 0., 0., 1., 1.);
  naFillBabyColor(app->colors[COLOR_MAGENTA], 1., 0., 1., 1.);
  app->colorNames[COLOR_TRANSPARENT] = "Transparent";
  app->colorNames[COLOR_RED] = "Red";
  app->colorNames[COLOR_YELLOW] = "Yellow";
  app->colorNames[COLOR_GREEN] = "Green";
  app->colorNames[COLOR_CYAN] = "Cyan";
  app->colorNames[COLOR_BLUE] = "Blue";
  app->colorNames[COLOR_MAGENTA] = "Magenta";

  app->imageTestController = naAllocImageTestController();

  app->imageTestController->topImage = app->images[IMAGE_FEATHER];
  naFillBabyColor(app->imageTestController->topColor, 0., 0., 0., 0.);
  app->imageTestController->bottomImage = NA_NULL;
  naFillBabyColor(app->imageTestController->bottomColor, 0., 0., 0., 0.);
  app->imageTestController->transparencyGridImage = app->transparencyGridImage;
  app->imageTestController->blendMode = NA_BLEND_OVERLAY;

  updateImage(app->imageTestController);
}



void naStopImageTestApplication(){
  naDeallocImageTestController(app->imageTestController);
  naReleaseBabyImage(app->transparencyGridImage);
  for(size_t i = 0; i < IMAGE_COUNT; ++i){
    naReleaseBabyImage(app->images[i]);
  }
  naFree(app);
}


void addSinglePopupItem(const NAUTF8Char* text, NAPopupButton* popup, NAReactionHandler handler, ImageTesterController* con){
  NAMenuItem* item = naNewMenuItem(text);
  naAddPopupButtonMenuItem(popup, item, NA_NULL);
  naAddUIReaction(item, NA_UI_COMMAND_PRESSED, handler, con);
}

void fillPopupButton(NAPopupButton* popup, NAReactionHandler handler, ImageTesterController* con){
  for(size_t i = 0; i < COLOR_COUNT; ++i){
    addSinglePopupItem(app->colorNames[i], popup, handler, con);
  }
  for(size_t i = 0; i < IMAGE_COUNT; ++i){
    addSinglePopupItem(app->imageNames[i], popup, handler, con);
  }
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
  con->bottomImage = NA_NULL;

  con->imageSpace = naNewImageSpace(NA_NULL, naMakeSize(600, 400));
  naAddUIReaction(con->imageSpace, NA_UI_COMMAND_MOUSE_MOVED, mouseMoved, con);
  naAddSpaceChild(contentSpace, con->imageSpace, naMakePos(10, 10));

  con->topLabel = naNewLabel("Top:", 200);
  con->topPopupButton = naNewPopupButton(200);
  naAddSpaceChild(contentSpace, con->topLabel, naMakePos(620, 325));
  naAddSpaceChild(contentSpace, con->topPopupButton, naMakePos(620, 300));
  fillPopupButton(con->topPopupButton, topSelected, con);

  con->scaleLabel = naNewLabel("Scale:", 200);
  con->scaleSlider = naNewSlider(200);
  naSetSliderRange(con->scaleSlider, 0., 2., 0);
  naSetSliderValue(con->scaleSlider, 1.);
  naAddSpaceChild(contentSpace, con->scaleLabel, naMakePos(620, 275));
  naAddSpaceChild(contentSpace, con->scaleSlider, naMakePos(620, 250));
  naAddUIReaction(con->scaleSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  con->blendModeLabel = naNewLabel("Blend Mode:", 200);
  con->blendModePopupButton = naNewPopupButton(200);
  naAddSpaceChild(contentSpace, con->blendModeLabel, naMakePos(620, 225));
  naAddSpaceChild(contentSpace, con->blendModePopupButton, naMakePos(620, 200));
  addSinglePopupItem("ZERO",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("BLEND",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("OVERLAY",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("OPAQUE",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("BLACK_GREEN",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("WHITE_GREEN",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("MULTIPLY",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("ERASE_HUE",con->blendModePopupButton, blendModeSelected, con);

  con->blendLabel = naNewLabel("Blend:", 200);
  con->blendSlider = naNewSlider(200);
  naSetSliderRange(con->blendSlider, 0., 1., 0);
  naSetSliderValue(con->blendSlider, 1.);
  naAddSpaceChild(contentSpace, con->blendLabel, naMakePos(620, 175));
  naAddSpaceChild(contentSpace, con->blendSlider, naMakePos(620, 150));
  naAddUIReaction(con->blendSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  con->bottomLabel = naNewLabel("Bottom:", 200);
  con->bottomPopupButton = naNewPopupButton(200);
  naAddSpaceChild(contentSpace, con->bottomLabel, naMakePos(620, 125));
  naAddSpaceChild(contentSpace, con->bottomPopupButton, naMakePos(620, 100));
  fillPopupButton(con->bottomPopupButton, bottomSelected, con);

  naShowWindow(con->window);

  return con;
}



void naDeallocImageTestController(ImageTesterController* con){
  naReleaseBabyImage(con->transparencyGridImage);
  naReleaseBabyImage(con->transparencyGridImage);
  naFree(con);
}



void updateImage(ImageTesterController* con){
  if(!con->transparencyGridImage)
    return;

  float alpha = (float)naGetSliderValue(con->blendSlider);
  double scale = naGetSliderValue(con->scaleSlider);

  NABabyImage* backImage;
  if(con->bottomImage){
    NASizei originalSize = naGetBabyImageSize(con->bottomImage);
    NASizei newSize = naMakeSizeiE((NAInt)(2. * originalSize.width), (NAInt)(2. * originalSize.height));

    NABabyImage* scaledImage = naCreateBabyImageWithResize(con->bottomImage, newSize);
    NASizei baseSize = naGetBabyImageSize(con->transparencyGridImage);

    NAPosi origin = naMakePosi(
      (baseSize.width - newSize.width) / 2,
      (baseSize.height - newSize.height) / 2);

    backImage = naCreateBabyImageWithBlend(
      con->transparencyGridImage,
      scaledImage,
      NA_BLEND_OVERLAY,
      1.,
      origin);
    naReleaseBabyImage(scaledImage);
  }else{
    backImage = naCreateBabyImageWithTint(
      con->transparencyGridImage,
      con->bottomColor,
      NA_BLEND_OVERLAY,
      1.);
  }

  NABabyImage* fullImage;

  if(con->topImage){
    NASizei originalSize = naGetBabyImageSize(con->topImage);
    NASizei newSize = naMakeSizeiE((NAInt)(scale * originalSize.width), (NAInt)(scale * originalSize.height));
    if(naIsSizeiUseful(newSize)){
      NABabyImage* scaledImage = naCreateBabyImageWithResize(con->topImage, newSize);
      NASizei baseSize = naGetBabyImageSize(backImage);
      NASize spaceSize = naGetUIElementRect(con->imageSpace).size;

      NAPosi origin = naMakePosi(
        con->center.x - (NAInt)((newSize.width / 2.) + ((spaceSize.width - baseSize.width) / 2.)),
        con->center.y - (NAInt)((newSize.height / 2.) + ((spaceSize.height - baseSize.height) / 2.)));

      fullImage = naCreateBabyImageWithBlend(
        backImage,
        scaledImage,
        con->blendMode,
        alpha,
        origin);
      naReleaseBabyImage(scaledImage);
    }else{
      fullImage = naRetainBabyImage(backImage);
    }
  }else{
    fullImage = naCreateBabyImageWithTint(
      backImage,
      con->topColor,
      con->blendMode,
      alpha);
  }

  naReleaseBabyImage(backImage);

  NAUIImage* uiImage = naCreateUIImage(
    fullImage,
    NA_UIIMAGE_RESOLUTION_SCREEN_1x,
    NA_BLEND_ZERO);
  naSetImageSpaceImage(con->imageSpace, uiImage);
  naRelease(uiImage);
  naReleaseBabyImage(fullImage);
}



void selectionChanged(const NABabyImage** imagePtr, NABabyColor colorPtr, const NAPopupButton* popup, const void* uiElement){
  size_t index = naGetPopupButtonItemIndex(popup, uiElement);
  if(index < COLOR_COUNT){
    *imagePtr = NA_NULL;
    naCopyV4f(colorPtr, app->colors[index]);
  }else{
    index -= COLOR_COUNT;
    *imagePtr = app->images[index];
  }
}

NABool topSelected(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  selectionChanged(&con->topImage, con->topColor, con->topPopupButton, reaction.uiElement);
  updateImage(con);
  return NA_TRUE;
}

NABool bottomSelected(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  selectionChanged(&con->bottomImage, con->bottomColor, con->bottomPopupButton, reaction.uiElement);
  updateImage(con);
  return NA_TRUE;
}

NABool blendModeSelected(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  size_t index = naGetPopupButtonItemIndex(con->blendModePopupButton, reaction.uiElement);
  con->blendMode = (NABlendMode)index;
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
  naStartImageTestApplication();
}



int main(void){
 
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, NA_NULL);
  naStopImageTestApplication();
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
