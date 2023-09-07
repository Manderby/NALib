
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.

#include "GUIExamples.h"
#include "../../NALib/src/NAStruct/NAStack.h"
#include "../../NALib/src/NAVisual/NAPNG.h"


// Put GUI elements belonging together into a controller struct.
struct HelloWorldGUIApplication{
  NAStack temperatureControllers;
  int nextWindowX;
  int nextWindowY;
  
  NAUIImage* iconImage;
  NAUIImage* state1Image;
  NAUIImage* state2Image;
  
  ExperimentController* experimentController;
  FontController* fontController;
  ButtonController* buttonController;
};

// The central variable storing the app. Defined as global.
HelloWorldGUIApplication* app;



// prestartup is called before most system specific initialization takes place.
// Initialize all global and general values here.
// See naStartApplication for a detailed explanation.
void prestartup(void* arg){
  NA_UNUSED(arg);
  app = naAlloc(HelloWorldGUIApplication);
  naInitStack(&(app->temperatureControllers), sizeof(TemperatureController*), 0, 0);
  app->nextWindowX = 700;
  app->nextWindowY = 400;

}



// poststartup is called after base application stuff has been initialized.
// Build-up your GUI here.
// See naStartApplication for a detailed explanation.
void poststartup(void* arg){
  NA_UNUSED(arg);
  
  // Load the image files
  NAPNG* pngIcon = naNewPNGWithPath("res/catIcon.png");
  if(!naIsSizeiUseful(naGetPNGSize(pngIcon))){
    printf("\nCould not open the image file. Check that the working directory is correct.\n");
    exit(1);
  }
  NABabyImage* originalImageIcon = naCreateBabyImageFromPNG(pngIcon);
  app->iconImage = naCreateUIImage(
    originalImageIcon,
    NA_UIIMAGE_RESOLUTION_SCREEN_2x,
    NA_BLEND_BLACK_GREEN);
  naReleaseBabyImage(originalImageIcon);
  naDelete(pngIcon);

  NAPNG* png1 = naNewPNGWithPath("res/cat.png");
  if(!naIsSizeiUseful(naGetPNGSize(png1))){
    printf("\nCould not open the image file. Check that the working directory is correct.\n");
    exit(1);
  }
  NABabyImage* originalImage1 = naCreateBabyImageFromPNG(png1);
  app->state1Image = naCreateUIImage(
    originalImage1,
    NA_UIIMAGE_RESOLUTION_SCREEN_2x,
    NA_BLEND_ZERO);
  naReleaseBabyImage(originalImage1);
  naDelete(png1);

  NAPNG* png2 = naNewPNGWithPath("res/cat2.png");
  if(!naIsSizeiUseful(naGetPNGSize(png2))){
    printf("\nCould not open the image file. Check that the working directory is correct.\n");
    exit(1);
  }
  NABabyImage* originalImage2 = naCreateBabyImageFromPNG(png2);
  app->state2Image = naCreateUIImage(
    originalImage2,
    NA_UIIMAGE_RESOLUTION_SCREEN_2x,
    NA_BLEND_ZERO);
  naReleaseBabyImage(originalImage2);
  naDelete(png2);

  // Create the controllers
  app->experimentController = createExperimentController();
  app->fontController = createFontController();
  app->buttonController = createButtonController();
}


// Delete all controllers and finally, delete this application
void clearApplication(void){
  naForeachStackMutable(&(app->temperatureControllers), (NAMutator)clearTemperatureController);
  naClearStack(&(app->temperatureControllers));
  clearExperimentController(app->experimentController);
  clearFontController(app->fontController);
  clearButtonController(app->buttonController);
  naFree(app);
}



double getAndAdvanceNextWindowX(void){
  double curWindowX = app->nextWindowX;
  app->nextWindowX += 20;
  return curWindowX;
}

double getAndAdvanceNextWindowY(void){
  double curWindowY = app->nextWindowY;
  app->nextWindowY -= 20;
  return curWindowY;
}

NAUIImage* getIconImage(void){
  return app->iconImage;
}
NAUIImage* getState1Image(void){
  return app->state1Image;
}
NAUIImage* getState2Image(void){
  return app->state2Image;
}

void addTemperatureControllerToApplication(TemperatureController* con){
  *(TemperatureController**)naPushStack(&(app->temperatureControllers)) = con;
}

void showFonts(){
  showFontController(app->fontController);
}

void showButtons(){
  showButtonController(app->buttonController);
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
