#include "../../NALib/src/NALib.h"

// This is a test application showing the image scaling functionality
// of NALib.

typedef struct ImageTesterApplication ImageTesterApplication;
typedef struct ImageTesterController ImageTesterController;

typedef enum{
  IMAGE_RAINBOW,
  IMAGE_LENZBURG,
  IMAGE_CAT_SILHOUETTE,
  IMAGE_TEXT,
  IMAGE_RUBIK,
  IMAGE_FEATHER,
  IMAGE_MOTOR,
  IMAGE_COUNT
} Images;

const NAUTF8Char* imageNames[IMAGE_COUNT] = {
  "Rainbow",
  "Lenzburg",
  "Cat Silhouette",
  "Text",
  "Rubik",
  "Feather",
  "Motor",
};

const NAUTF8Char* imagePaths[IMAGE_COUNT] = {
  "rainbow.png",
  "lenzburg.png",
  "catSilhouette.png",
  "text.png",
  "rubik.png",
  "feather.png",
  "motor.png",
};

typedef enum{
  COLOR_TRANSPARENT,
  COLOR_RED,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_CYAN,
  COLOR_BLUE,
  COLOR_MAGENTA,
  COLOR_WHITE,
  COLOR_BLACK,
  COLOR_COUNT
} Colors;

const NAUTF8Char* colorNames[COLOR_COUNT] = {
  [COLOR_TRANSPARENT] = "Transparent",
  [COLOR_RED]         = "Red",
  [COLOR_YELLOW]      = "Yellow",
  [COLOR_GREEN]       = "Green",
  [COLOR_CYAN]        = "Cyan",
  [COLOR_BLUE]        = "Blue",
  [COLOR_MAGENTA]     = "Magenta",
  [COLOR_WHITE]       = "White",
  [COLOR_BLACK]       = "Black",
};

NAColor colors[COLOR_COUNT];

struct ImageTesterApplication{
  NAImage* transparencyGridImage;
  NAImage* images[IMAGE_COUNT];

  ImageTesterController* imageTestController;
};

struct ImageTesterController{
  NAWindow* window;
  NAImageSpace* imageSpace;

  NALabel* topLabel;
  NASelect* topSelect;

  NALabel* scaleLabel;
  NASlider* scaleSlider;

  NALabel* blendModeLabel;
  NASelect* blendModeSelect;

  NALabel* alphaLabel;
  NASlider* alphaSlider;

  NALabel* bottomLabel;
  NASelect* bottomSelect;

  const NAImage* topImage;
  const NAColor* topColor;
  const NAImage* bottomImage;
  const NAColor* bottomColor;
  
  NAPosi32 center;
  size_t selectedTop;
  float scale;
  float alpha;
  NABlendMode blendMode;
  size_t selectedBottom;
};

ImageTesterApplication* app = NA_NULL;



ImageTesterController* naAllocImageTestController(void);
void naDeallocImageTestController(ImageTesterController* con);
void topSelected(NAReaction reaction);
void bottomSelected(NAReaction reaction);
void blendModeSelected(NAReaction reaction);
void sliderEdited(NAReaction reaction);
void mouseMoved(NAReaction reaction);
void updateImageTestController(ImageTesterController* con);



void loadImage(size_t index, const NAUTF8Char* path) {
  NAPNG* png = naNewPNGWithPath(path);
  app->images[index] = naCreateImageWithPNG(png);
  naDelete(png);
}

void naStartImageTestApplication(void) {
  app = naAlloc(ImageTesterApplication);

  NAPNG* gridPNG = naNewPNGWithPath("transparencyGrid.png");
  NAImage* gridImage = naCreateImageWithPNG(gridPNG);
  NASizes gridSize = naGetImageSize(gridImage);
  app->transparencyGridImage = naCreateImageWithResize(gridImage, naMakeSizes(gridSize.width * 2, gridSize.height * 2));
  naRelease(gridImage);

  for(size_t i = 0; i < IMAGE_COUNT; ++i) {
    app->images[i] = NA_NULL;
  }

  app->imageTestController = naAllocImageTestController();
  updateImageTestController(app->imageTestController);
}



void naStopImageTestApplication(void) {
  naDeallocImageTestController(app->imageTestController);
  naRelease(app->transparencyGridImage);
  for(size_t i = 0; i < IMAGE_COUNT; ++i) {
    naRelease(app->images[i]);
  }
  naFree(app);
}


void addSingleSelectItem(const NAUTF8Char* text, NASelect* select, NAReactionCallback callback, ImageTesterController* con) {
  NAMenuItem* item = naNewMenuItem(text);
  naAddSelectMenuItem(select, item, NA_NULL);
  naAddUIReaction(item, NA_UI_COMMAND_PRESSED, callback, con);
}

void fillSelect(NASelect* select, NAReactionCallback callback, ImageTesterController* con) {
  for(size_t i = 0; i < COLOR_COUNT; ++i) {
    addSingleSelectItem(colorNames[i], select, callback, con);
  }
  for(size_t i = 0; i < IMAGE_COUNT; ++i) {
    addSingleSelectItem(imageNames[i], select, callback, con);
  }
}



ImageTesterController* naAllocImageTestController() {
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
  con->topSelect = naNewSelect(200);
  naAddSpaceChild(contentSpace, con->topLabel, naMakePos(640, 325));
  naAddSpaceChild(contentSpace, con->topSelect, naMakePos(640, 300));
  fillSelect(con->topSelect, topSelected, con);

  con->scaleLabel = naNewLabel("Scale:", 200);
  con->scaleSlider = naNewSlider(200);
  naSetSliderRange(con->scaleSlider, 0., 2., 0);
  naSetSliderValue(con->scaleSlider, 1.);
  naAddSpaceChild(contentSpace, con->scaleLabel, naMakePos(640, 275));
  naAddSpaceChild(contentSpace, con->scaleSlider, naMakePos(640, 250));
  naAddUIReaction(con->scaleSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  con->blendModeLabel = naNewLabel("Blend Mode:", 200);
  con->blendModeSelect = naNewSelect(200);
  naAddSpaceChild(contentSpace, con->blendModeLabel, naMakePos(640, 225));
  naAddSpaceChild(contentSpace, con->blendModeSelect, naMakePos(640, 200));
  addSingleSelectItem("ZERO",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("LINEAR",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("OVERLAY",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("OPAQUE",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("MULTIPLY",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("SCREEN",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("ERODE_LIGHT",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("ERODE_DARK",con->blendModeSelect, blendModeSelected, con);
  addSingleSelectItem("ERASE_HUE",con->blendModeSelect, blendModeSelected, con);

  con->alphaLabel = naNewLabel("Blend:", 200);
  con->alphaSlider = naNewSlider(200);
  naSetSliderRange(con->alphaSlider, 0., 1., 0);
  naSetSliderValue(con->alphaSlider, 1.);
  naAddSpaceChild(contentSpace, con->alphaLabel, naMakePos(640, 175));
  naAddSpaceChild(contentSpace, con->alphaSlider, naMakePos(640, 150));
  naAddUIReaction(con->alphaSlider, NA_UI_COMMAND_EDITED, sliderEdited, con);

  con->bottomLabel = naNewLabel("Bottom:", 200);
  con->bottomSelect = naNewSelect(200);
  naAddSpaceChild(contentSpace, con->bottomLabel, naMakePos(640, 125));
  naAddSpaceChild(contentSpace, con->bottomSelect, naMakePos(640, 100));
  fillSelect(con->bottomSelect, bottomSelected, con);

  naShowWindow(con->window);

  return con;
}



void naDeallocImageTestController(ImageTesterController* con) {
  naFree(con);
}



void selectionChanged(const NAImage** imagePtr, const NAColor** color, size_t index) {
  if(index < COLOR_COUNT) {
    *imagePtr = NA_NULL;
    *color = &colors[index];
  }else{
    index -= COLOR_COUNT;
    if(!app->images[index]) {
      loadImage(index, imagePaths[index]);
    }
    *imagePtr = app->images[index];
  }
}



void updateImageTestController(ImageTesterController* con) {
  if(!app->transparencyGridImage)
    return;

  selectionChanged(&con->topImage, &con->topColor, con->selectedTop);
  selectionChanged(&con->bottomImage, &con->bottomColor, con->selectedBottom);

  naSetSelectIndexSelected(con->topSelect, con->selectedTop);
  naSetSliderValue(con->scaleSlider, con->scale);
  naSetSliderValue(con->alphaSlider, con->alpha);
  naSetSelectIndexSelected(con->blendModeSelect, con->blendMode);
  naSetSelectIndexSelected(con->bottomSelect, con->selectedBottom);

  NASizes gridSize = naGetImageSize(app->transparencyGridImage);

  NAImage* backImage;
  if(con->bottomImage) {
    NASizes newSize = naGetImageSize(con->bottomImage);
    newSize.width *= 2;
    newSize.height *= 2;
    backImage = naCreateImageWithResize(con->bottomImage, newSize);
  }else{
    backImage = naCreateImage(gridSize, con->bottomColor);
  }
    
  NAImage* blendedImage;

  if(con->topImage) {
    NASizes originalSize = naGetImageSize(con->topImage);
    NASizes newSize = naMakeSizesE(
      (size_t)(con->scale * originalSize.width),
      (size_t)(con->scale * originalSize.height));
    if(naIsSizesUseful(newSize)) {
      NAImage* scaledImage = naCreateImageWithResize(con->topImage, newSize);
      NASizes baseSize = naGetImageSize(backImage);
      NASize spaceSize = naGetUIElementRect(con->imageSpace).size;

      NAPosi32 origin = naMakePosi32(
        con->center.x - (((int32)newSize.width / 2) + (((int32)spaceSize.width - (int32)baseSize.width) / 2)),
        con->center.y - (((int32)newSize.height / 2) + (((int32)spaceSize.height - (int32)baseSize.height) / 2)));

      blendedImage = naCreateImageWithBlend(
        backImage,
        scaledImage,
        con->blendMode,
        con->alpha,
        origin);
      naRelease(scaledImage);
    }else{
      blendedImage = naRetain(backImage);
    }
  }else{
    blendedImage = naCreateImageWithTint(
      backImage,
      con->topColor,
      con->blendMode,
      con->alpha);
  }

  naRelease(backImage);

  NASizes blendedSize = naGetImageSize(blendedImage);
  NAPosi32 origin = naMakePosi32(
    ((int32)gridSize.width - (int32)blendedSize.width) / 2,
    ((int32)gridSize.height - (int32)blendedSize.height) / 2);

  NAImage* fullImage = naCreateImageWithBlend(
    app->transparencyGridImage,
    blendedImage,
    NA_BLEND_OVERLAY,
    1.,
    origin);

  naRelease(blendedImage);

  NAImageSet* imageSet = naCreateImageSet(
    fullImage,
    NA_UI_RESOLUTION_1x,
    NA_BLEND_ZERO);
  naRelease(fullImage);

  naSetImageSpaceImage(con->imageSpace, imageSet);
  naRelease(imageSet);

}



void topSelected(NAReaction reaction) {
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  con->selectedTop = naGetSelectItemIndex(con->topSelect, reaction.uiElement);
  updateImageTestController(con);
}

void bottomSelected(NAReaction reaction) {
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  con->selectedBottom = naGetSelectItemIndex(con->bottomSelect, reaction.uiElement);
  updateImageTestController(con);
}

void blendModeSelected(NAReaction reaction) {
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  size_t index = naGetSelectItemIndex(con->blendModeSelect, reaction.uiElement);
  con->blendMode = (NABlendMode)index;
  updateImageTestController(con);
}



void sliderEdited(NAReaction reaction) {
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  if(reaction.uiElement == con->alphaSlider) {
    con->alpha = (float)naGetSliderValue(con->alphaSlider);
  }else if(reaction.uiElement == con->scaleSlider) {
    con->scale = (float)naGetSliderValue(con->scaleSlider);
  }
  updateImageTestController(con);
}



void mouseMoved(NAReaction reaction) {
  ImageTesterController* con = (ImageTesterController*)reaction.controller;
  const NAMouseStatus* mouseStatus = naGetCurrentMouseStatus();
  NAPos mousePos = naGetMousePos(mouseStatus);
  NARect spaceRect = naGetUIElementRectAbsolute(reaction.uiElement);
  con->center.x = (int32)(mousePos.x - spaceRect.pos.x);
  con->center.y = (int32)(mousePos.y - spaceRect.pos.y);
  updateImageTestController(con);
}



void preStartup(void* arg) {
  NA_UNUSED(arg);

  naFillColorWithSRGB(&colors[0], 0., 0., 0., 0.);
  naFillColorWithSRGB(&colors[1], 1., 0., 0., 1.);
  naFillColorWithSRGB(&colors[2], 1., 1., 0., 1.);
  naFillColorWithSRGB(&colors[3], 0., 1., 0., 1.);
  naFillColorWithSRGB(&colors[4], 0., 1., 1., 1.);
  naFillColorWithSRGB(&colors[5], 0., 0., 1., 1.);
  naFillColorWithSRGB(&colors[6], 1., 0., 1., 1.);
  naFillColorWithSRGB(&colors[7], 1., 1., 1., 1.);
  naFillColorWithSRGB(&colors[8], 0., 0., 0., 1.);
}



void postStartup(void* arg) {
  NA_UNUSED(arg);
  naStartImageTestApplication();
}



int main(void) {
 
  naStartRuntime();
  naStartApplication(preStartup, postStartup, NA_NULL, NA_NULL);
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
