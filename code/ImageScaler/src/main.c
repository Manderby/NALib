#include "../../NALib/src/NALib.h"

// This is a test application showing the image scaling functionality
// of NALib.


NAWindow* window;
NAImageSpace* imageSpace;
NASlider* scaleSlider;
NABabyImage* transparencyGridImage;
NABabyImage* originalImage;
NAPosi center = {0, 0};

void updateImage(){
  double value = naGetSliderValue(scaleSlider);
  NASizei originalSize = naGetBabyImageSize(originalImage);
  NASizei newSize = naMakeSizeiE((NAInt)(value * originalSize.width), (NAInt)(value * originalSize.height));
  if(naIsSizeiUseful(newSize)){
    NABabyImage* scaledImage = naCreateBabyImageWithResize(originalImage, newSize);
    NASizei baseSize = naGetBabyImageSize(transparencyGridImage);
    NASize spaceSize = naGetUIElementRect(imageSpace).size;

    NAPosi origin = naMakePosi(
      center.x - (NAInt)((newSize.width / 2.) + ((spaceSize.width - baseSize.width) / 2.)),
      center.y - (NAInt)((newSize.height / 2.) + ((spaceSize.height - baseSize.height) / 2.)));

    NABabyImage* fullImage = naCreateBabyImageWithBlend(
      transparencyGridImage,
      scaledImage,
      NA_BLEND_OVERLAY,
      1.,
      origin);
    NAUIImage* uiImage = naCreateUIImage(
      fullImage,
      NA_UIIMAGE_RESOLUTION_SCREEN_1x,
      NA_BLEND_ZERO);
    naSetImageSpaceImage(imageSpace, uiImage);
    naRelease(uiImage);
    naReleaseBabyImage(fullImage);
    naReleaseBabyImage(scaledImage);
  }
}

NABool sliderEdited(NAReaction reaction){
  updateImage();
  return NA_TRUE;
}

NABool mouseMoved(NAReaction reaction){
  const NAMouseStatus* mouse = naGetMouseStatus();
  NARect spaceRect = naGetUIElementRectAbsolute(reaction.uiElement);
  center.x = (NAInt)(mouse->pos.x - spaceRect.pos.x);
  center.y = (NAInt)(mouse->pos.y - spaceRect.pos.y);
  updateImage();
  return NA_TRUE;
}

NABool mouseDown(NAReaction reaction){
  printf("down\n");
  return NA_TRUE;
}

NABool mouseUp(NAReaction reaction){
  printf("up\n");
  return NA_TRUE;
}

void postStartup(void* arg){
  NA_UNUSED(arg);

  window = naNewWindow(
    "ImageScaler",
    naMakeRectS(100, 100, 830, 420),
    NA_WINDOW_FIXED_SIZE | NA_WINDOW_DEFAULT | NA_WINDOW_TITLED | NA_WINDOW_CLOSEABLE | NA_WINDOW_MINIATURIZEABLE,
    0);
  
  NASpace* contentSpace = naGetWindowContentSpace(window);
  
  NAPNG* gridPNG = naNewPNGWithPath("res/transparencyGrid.png");
  NABabyImage* gridImage = naCreateBabyImageFromPNG(gridPNG);
  NASizei gridSize = naGetBabyImageSize(gridImage);
  transparencyGridImage = naCreateBabyImageWithResize(gridImage, naMakeSizei(gridSize.width * 2, gridSize.height * 2));

  //NAPNG* featherPNG = naNewPNGWithPath("res/transparencyGrid.png");
  NAPNG* featherPNG = naNewPNGWithPath("res/feather.png");
  //NAPNG* png = naNewPNGWithPath("res/motor.png");
  originalImage = naCreateBabyImageFromPNG(featherPNG);
  NAUIImage* uiImage = naCreateUIImage(
    transparencyGridImage,
    NA_UIIMAGE_RESOLUTION_SCREEN_1x,
    NA_BLEND_ZERO);
  
  imageSpace = naNewImageSpace(uiImage, naMakeSize(600, 400));
  scaleSlider = naNewSlider(200);
  naSetSliderRange(scaleSlider, 0., 2., 0);
  naSetSliderValue(scaleSlider, 1.);
  
  naAddSpaceChild(contentSpace, imageSpace, naMakePos(10, 10));
  naAddSpaceChild(contentSpace, scaleSlider, naMakePos(620, 200));
  naAddUIReaction(scaleSlider, NA_UI_COMMAND_EDITED, sliderEdited, NA_NULL);
  naAddUIReaction(imageSpace, NA_UI_COMMAND_MOUSE_MOVED, mouseMoved, NA_NULL);
  naAddUIReaction(imageSpace, NA_UI_COMMAND_MOUSE_DOWN, mouseDown, NA_NULL);
  naAddUIReaction(imageSpace, NA_UI_COMMAND_MOUSE_UP, mouseUp, NA_NULL);

  
  naShowWindow(window);
}

int main(void){
 
  naStartRuntime();
  naStartApplication(NA_NULL, postStartup, NA_NULL);
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
