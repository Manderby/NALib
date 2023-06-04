#include "../../NALib/src/NALib.h"

// This is a test application showing the image scaling functionality
// of NALib.


NAWindow* window;
NAImageSpace* imageSpace;
NASlider* slider;
NABabyImage* originalImage;

NABool sliderEdited(NAReaction reaction){
  double value = naGetSliderValue(slider);
  NASizei originalSize = naGetBabyImageSize(originalImage);
  NASizei newSize = naMakeSizeiE((NAInt)(value * originalSize.width), (NAInt)(value * originalSize.height));
  if(naIsSizeiUseful(newSize)){
    NABabyImage* scaledImage = naCreateBabyImageWithResize(originalImage, newSize);
    NAUIImage* uiImage = naCreateUIImage(
      scaledImage,
      NA_NULL,
      NA_UIIMAGE_RESOLUTION_1x,
      NA_BLEND_ZERO);
    naSetImageSpaceImage(imageSpace, uiImage);
    naRelease(uiImage);
    naReleaseBabyImage(scaledImage);
  }
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
  
//  NAPNG* png = naNewPNGWithPath("res/marientaefer.png");
  NAPNG* png = naNewPNGWithPath("res/motor.png");
  originalImage = naCreateBabyImageFromPNG(png);
  NAUIImage* uiImage = naCreateUIImage(
    originalImage,
    NA_NULL,
    NA_UIIMAGE_RESOLUTION_1x,
    NA_BLEND_ZERO);
  
  imageSpace = naNewImageSpace(uiImage, naMakeSize(600, 400));
  slider = naNewSlider(200);
  naSetSliderRange(slider, 0., 2., 0);
  naSetSliderValue(slider, 1.);
  
  naAddSpaceChild(contentSpace, imageSpace, naMakePos(10, 10));
  naAddSpaceChild(contentSpace, slider, naMakePos(620, 200));
  naAddUIReaction(slider, NA_UI_COMMAND_EDITED, sliderEdited, NA_NULL);
  
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
