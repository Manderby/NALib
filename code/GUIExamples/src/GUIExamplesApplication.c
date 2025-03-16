
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.

#include "GUIExamples.h"
#include "../../NALib/src/NAStruct/NAStack.h"
#include "../../NALib/src/NAVisual/NAPNG.h"
#include "NAUtility/NAString.h"

#define RESOURCE_PATH ""
//#define RESOURCE_PATH "res/"


// Put GUI elements belonging together into a controller struct.
struct HelloWorldGUIApplication{
  NAStack temperatureControllers;
  int nextWindowX;
  int nextWindowY;
  
  NAImageSet* iconImageSet;
  NAImageSet* state1ImageSet;
  NAImageSet* state2ImageSet;
  
  ExperimentController* experimentController;
  FontController* fontController;
  ButtonController* buttonController;
};

// The central variable storing the app. Defined as global.
HelloWorldGUIApplication* app;



// preStartup is called before most system specific initialization takes place.
// Initialize all global and general values here.
// See naStartApplication for a detailed explanation.
void preStartup(void* arg){
  NA_UNUSED(arg);
  app = naAlloc(HelloWorldGUIApplication);
  naInitStack(&(app->temperatureControllers), sizeof(TemperatureController*), 0, 0);
  app->nextWindowX = 700;
  app->nextWindowY = 400;

}



// postStartup is called after base application stuff has been initialized.
// Build-up your GUI here.
// See naStartApplication for a detailed explanation.
void postStartup(void* arg){
  NA_UNUSED(arg);
  
  // Load the image files
  NAPNG* pngIcon = naNewPNGWithPath(RESOURCE_PATH "icon.png");
  if(!naIsSizesUseful(naGetPNGSize(pngIcon))){
    printf("\nCould not open the image file. Check that the working directory is correct.\n");
    exit(1);
  }
  NAImage* originalIconImage = naCreateImageWithPNG(pngIcon);
  app->iconImageSet = naCreateImageSet(
    originalIconImage,
    NA_UI_RESOLUTION_2x,
    NA_BLEND_ERODE_LIGHT);
  naDelete(pngIcon);
  naRelease(originalIconImage);

  NAPNG* png1 = naNewPNGWithPath(RESOURCE_PATH "man.png");
  if(!naIsSizesUseful(naGetPNGSize(png1))){
    printf("\nCould not open the image file. Check that the working directory is correct.\n");
    exit(1);
  }
  NAImage* originalState1Image = naCreateImageWithPNG(png1);
  app->state1ImageSet = naCreateImageSet(
    originalState1Image,
    NA_UI_RESOLUTION_2x,
    NA_BLEND_ZERO);
  naDelete(png1);
  naRelease(originalState1Image);

  NAPNG* png2 = naNewPNGWithPath(RESOURCE_PATH "man2.png");
  if(!naIsSizesUseful(naGetPNGSize(png2))){
    printf("\nCould not open the image file. Check that the working directory is correct.\n");
    exit(1);
  }
  NAImage* originalState2Image = naCreateImageWithPNG(png2);
  app->state2ImageSet = naCreateImageSet(
    originalState2Image,
    NA_UI_RESOLUTION_2x,
    NA_BLEND_ZERO);
  naDelete(png2);
  naRelease(originalState2Image);

  // Create the controllers
  app->experimentController = createExperimentController();
  app->fontController = createFontController();
  app->buttonController = createButtonController();
}



// Delete all controllers and finally, delete this application
void clearApplication(void* arg){
  NA_UNUSED(arg);

  naForeachStackpMutable(&(app->temperatureControllers), (NAMutator)despawnTemperatureController);
  naClearStack(&(app->temperatureControllers));
  clearExperimentController(app->experimentController);
  clearFontController(app->fontController);
  clearButtonController(app->buttonController);
  naRelease(app->iconImageSet);
  naRelease(app->state1ImageSet);
  naRelease(app->state2ImageSet);
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

NAImageSet* getIconImageSet(void){
  return app->iconImageSet;
}
NAImageSet* getState1ImageSet(void){
  return app->state1ImageSet;
}
NAImageSet* getState2ImageSet(void){
  return app->state2ImageSet;
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
