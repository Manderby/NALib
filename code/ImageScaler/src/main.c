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
  IMAGE_CAT_SILHOUETTE,
  IMAGE_FEATHER,
  IMAGE_MOTOR,
  IMAGE_COUNT
} Images;

const NAUTF8Char* imageNames[IMAGE_COUNT] = {
  "Rainbow",
  "Cat Pink",
  "Cat Blue",
  "Cat Green",
  "Cat Silhouette",
  "Feather",
  "Motor",
};

const NAUTF8Char* imagePaths[IMAGE_COUNT] = {
  "res/rainbow.png",
  "res/catPink.png",
  "res/catBlue.png",
  "res/catGreen.png",
  "res/catSilhouette.png",
  "res/feather.png",
  "res/motor.png",
};

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

const NAUTF8Char* colorNames[COLOR_COUNT] = {
  "Transparent",
  "Red",
  "Yellow",
  "Green",
  "Cyan",
  "Blue",
  "Magenta",
};

const NABabyColor colors[COLOR_COUNT] = {
  {0., 0., 0., 0.},
  {1., 0., 0., 1.},
  {1., 1., 0., 1.},
  {0., 1., 0., 1.},
  {0., 1., 1., 1.},
  {0., 0., 1., 1.},
  {1., 0., 1., 1.},
};

struct ImageTesterApplication{
  NABabyImage* transparencyGridImage;
  NABabyImage* images[IMAGE_COUNT];

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

  NALabel* alphaLabel;
  NASlider* alphaSlider;

  NALabel* bottomLabel;
  NAPopupButton* bottomPopupButton;

  const NABabyImage* topImage;
  NABabyColor topColor;
  const NABabyImage* bottomImage;
  NABabyColor bottomColor;
  
  NAPosi center;
  size_t selectedTop;
  float scale;
  float alpha;
  NABlendMode blendMode;
  size_t selectedBottom;
};

ImageTesterApplication* app = NA_NULL;



ImageTesterController* naAllocImageTestController(void);
void naDeallocImageTestController(ImageTesterController* con);
NABool topSelected(NAReaction reaction);
NABool bottomSelected(NAReaction reaction);
NABool blendModeSelected(NAReaction reaction);
NABool sliderEdited(NAReaction reaction);
NABool mouseMoved(NAReaction reaction);
void updateImageTestController(ImageTesterController* con);



void loadImage(size_t index, const NAUTF8Char* path){
  NAPNG* png = naNewPNGWithPath(path);
  app->images[index] = naCreateBabyImageFromPNG(png);
  naDelete(png);
}

void naStartImageTestApplication(){
  app = naAlloc(ImageTesterApplication);

  NAPNG* gridPNG = naNewPNGWithPath("res/transparencyGrid.png");
  NABabyImage* gridImage = naCreateBabyImageFromPNG(gridPNG);
  NASizei gridSize = naGetBabyImageSize(gridImage);
  app->transparencyGridImage = naCreateBabyImageWithResize(gridImage, naMakeSizei(gridSize.width * 2, gridSize.height * 2));
  naReleaseBabyImage(gridImage);

  for(size_t i = 0; i < IMAGE_COUNT; ++i){
    app->images[i] = NA_NULL;
  }

  app->imageTestController = naAllocImageTestController();
  updateImageTestController(app->imageTestController);
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
    addSinglePopupItem(colorNames[i], popup, handler, con);
  }
  for(size_t i = 0; i < IMAGE_COUNT; ++i){
    addSinglePopupItem(imageNames[i], popup, handler, con);
  }
}



ImageTesterController* naAllocImageTestController(){
  ImageTesterController* con = naAlloc(ImageTesterController);

  con->window = naNewWindow(
    "ImageScaler",
    naMakeRectS(100, 100, 860, 440),
    NA_WINDOW_FIXED_SIZE | NA_WINDOW_DEFAULT | NA_WINDOW_TITLED | NA_WINDOW_CLOSEABLE | NA_WINDOW_MINIATURIZEABLE,
    0);

  NASpace* contentSpace = naGetWindowContentSpace(con->window);

  con->topImage = NA_NULL;
  con->bottomImage = NA_NULL;

  con->center.x = 0;
  con->center.y = 0;
  con->selectedTop = COLOR_COUNT + IMAGE_FEATHER;
  con->scale = 1.;
  con->alpha = 1.;
  con->blendMode = NA_BLEND_OVERLAY;
  con->selectedBottom = COLOR_TRANSPARENT;

  con->imageSpace = naNewImageSpace(NA_NULL, naMakeSize(600, 400));
  naAddUIReaction(con->imageSpace, NA_UI_COMMAND_MOUSE_MOVED, mouseMoved, con);
  naAddSpaceChild(contentSpace, con->imageSpace, naMakePos(20, 20));

  con->topLabel = naNewLabel("Top:", 200);
  con->topPopupButton = naNewPopupButton(200);
  naAddSpaceChild(contentSpace, con->topLabel, naMakePos(640, 325));
  naAddSpaceChild(contentSpace, con->topPopupButton, naMakePos(640, 300));
  fillPopupButton(con->topPopupButton, topSelected, con);

  con->scaleLabel = naNewLabel("Scale:", 200);
  con->scaleSlider = naNewSlider(200);
  naSetSliderRange(con->scaleSlider, 0., 2., 0);
  naSetSliderValue(con->scaleSlider, 1.);
  naAddSpaceChild(contentSpace, con->scaleLabel, naMakePos(640, 275));
  naAddSpaceChild(contentSpace, con->scaleSlider, naMakePos(640, 250));
  naAddUIReaction(con->scaleSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  con->blendModeLabel = naNewLabel("Blend Mode:", 200);
  con->blendModePopupButton = naNewPopupButton(200);
  naAddSpaceChild(contentSpace, con->blendModeLabel, naMakePos(640, 225));
  naAddSpaceChild(contentSpace, con->blendModePopupButton, naMakePos(640, 200));
  addSinglePopupItem("ZERO",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("BLEND",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("OVERLAY",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("OPAQUE",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("BLACK_GREEN",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("WHITE_GREEN",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("MULTIPLY",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("SCREEN",con->blendModePopupButton, blendModeSelected, con);
  addSinglePopupItem("ERASE_HUE",con->blendModePopupButton, blendModeSelected, con);

  con->alphaLabel = naNewLabel("Blend:", 200);
  con->alphaSlider = naNewSlider(200);
  naSetSliderRange(con->alphaSlider, 0., 1., 0);
  naSetSliderValue(con->alphaSlider, 1.);
  naAddSpaceChild(contentSpace, con->alphaLabel, naMakePos(640, 175));
  naAddSpaceChild(contentSpace, con->alphaSlider, naMakePos(640, 150));
  naAddUIReaction(con->alphaSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  con->bottomLabel = naNewLabel("Bottom:", 200);
  con->bottomPopupButton = naNewPopupButton(200);
  naAddSpaceChild(contentSpace, con->bottomLabel, naMakePos(640, 125));
  naAddSpaceChild(contentSpace, con->bottomPopupButton, naMakePos(640, 100));
  fillPopupButton(con->bottomPopupButton, bottomSelected, con);

  naShowWindow(con->window);

  return con;
}



void naDeallocImageTestController(ImageTesterController* con){
  naFree(con);
}



void selectionChanged(const NABabyImage** imagePtr, NABabyColor colorPtr, size_t index){
  if(index < COLOR_COUNT){
    *imagePtr = NA_NULL;
    naCopyV4f(colorPtr, colors[index]);
  }else{
    index -= COLOR_COUNT;
    if(!app->images[index]){
      loadImage(index, imagePaths[index]);
    }
    *imagePtr = app->images[index];
  }
}



void updateImageTestController(ImageTesterController* con){
  if(!app->transparencyGridImage)
    return;

  selectionChanged(&con->topImage, con->topColor, con->selectedTop);
  selectionChanged(&con->bottomImage, con->bottomColor, con->selectedBottom);

  naSetPopupButtonIndexSelected(con->topPopupButton, con->selectedTop);
  naSetSliderValue(con->scaleSlider, con->scale);
  naSetSliderValue(con->alphaSlider, con->alpha);
  naSetPopupButtonIndexSelected(con->blendModePopupButton, con->blendMode);
  naSetPopupButtonIndexSelected(con->bottomPopupButton, con->selectedBottom);

  NASizei gridSize = naGetBabyImageSize(app->transparencyGridImage);

  NABabyImage* backImage;
  if(con->bottomImage){
    NASizei newSize = naGetBabyImageSize(con->bottomImage);
    newSize.width *= 2;
    newSize.height *= 2;
    backImage = naCreateBabyImageWithResize(con->bottomImage, newSize);
  }else{
    backImage = naCreateBabyImage(gridSize, con->bottomColor);
  }
    
  NABabyImage* blendedImage;

  if(con->topImage){
    NASizei originalSize = naGetBabyImageSize(con->topImage);
    NASizei newSize = naMakeSizeiE((NAInt)(con->scale * originalSize.width), (NAInt)(con->scale * originalSize.height));
    if(naIsSizeiUseful(newSize)){
      NABabyImage* scaledImage = naCreateBabyImageWithResize(con->topImage, newSize);
      NASizei baseSize = naGetBabyImageSize(backImage);
      NASize spaceSize = naGetUIElementRect(con->imageSpace).size;

      NAPosi origin = naMakePosi(
        con->center.x - (NAInt)((newSize.width / 2.) + ((spaceSize.width - baseSize.width) / 2.)),
        con->center.y - (NAInt)((newSize.height / 2.) + ((spaceSize.height - baseSize.height) / 2.)));

      blendedImage = naCreateBabyImageWithBlend(
        backImage,
        scaledImage,
        con->blendMode,
        con->alpha,
        origin);
      naReleaseBabyImage(scaledImage);
    }else{
      blendedImage = naRetainBabyImage(backImage);
    }
  }else{
    blendedImage = naCreateBabyImageWithTint(
      backImage,
      con->topColor,
      con->blendMode,
      con->alpha);
  }

  naReleaseBabyImage(backImage);

  NASizei blendedSize = naGetBabyImageSize(blendedImage);
  NAPosi origin = naMakePosi(
    (gridSize.width - blendedSize.width) / 2,
    (gridSize.height - blendedSize.height) / 2);

  NABabyImage* fullImage = naCreateBabyImageWithBlend(
    app->transparencyGridImage,
    blendedImage,
    NA_BLEND_OVERLAY,
    1.,
    origin);

  naReleaseBabyImage(blendedImage);

  NAUIImage* uiImage = naCreateUIImage(
    fullImage,
    NA_UIIMAGE_RESOLUTION_SCREEN_1x,
    NA_BLEND_ZERO);
  naReleaseBabyImage(fullImage);

  naSetImageSpaceImage(con->imageSpace, uiImage);
  naRelease(uiImage);

}



NABool topSelected(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  con->selectedTop = naGetPopupButtonItemIndex(con->topPopupButton, reaction.uiElement);
  updateImageTestController(con);
  return NA_TRUE;
}

NABool bottomSelected(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  con->selectedBottom = naGetPopupButtonItemIndex(con->bottomPopupButton, reaction.uiElement);
  updateImageTestController(con);
  return NA_TRUE;
}

NABool blendModeSelected(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  size_t index = naGetPopupButtonItemIndex(con->blendModePopupButton, reaction.uiElement);
  con->blendMode = (NABlendMode)index;
  updateImageTestController(con);
  return NA_TRUE;
}



NABool sliderEdited(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  if(reaction.uiElement == con->alphaSlider){
    con->alpha = (float)naGetSliderValue(con->alphaSlider);
  }else if(reaction.uiElement == con->scaleSlider){
    con->scale = (float)naGetSliderValue(con->scaleSlider);
  }
  updateImageTestController(con);
  return NA_TRUE;
}



NABool mouseMoved(NAReaction reaction){
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  const NAMouseStatus* mouse = naGetMouseStatus();
  NARect spaceRect = naGetUIElementRectAbsolute(reaction.uiElement);
  con->center.x = (NAInt)(mouse->pos.x - spaceRect.pos.x);
  con->center.y = (NAInt)(mouse->pos.y - spaceRect.pos.y);
  updateImageTestController(con);
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
