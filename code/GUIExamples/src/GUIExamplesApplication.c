
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.

#include "GUIExamples.h"
#include "NAStruct/NAStack.h"
#include "NAVisual/NAPNG.h"
#include "NAUtility/NADateTime.h"
#include "NAUtility/NAString.h"
#include "NAUtility/NAFile.h"

#define RESOURCE_PATH ""
//#define RESOURCE_PATH "res/"


// Put GUI elements belonging together into a controller struct.
struct GUIExamplesApplication{
  NAStack temperatureControllers;
  int nextWindowX;
  int nextWindowY;
  
  NAImageSet* iconImageSet;
  NAImageSet* state1ImageSet;
  NAImageSet* state2ImageSet;
  
  NAFont* titleFont;
  NAFont* monoFont;
  
  NAList flashList;

  ExampleController* exampleController;
};

// The central variable storing the app.
GUIExamplesApplication* app;



// preStartup is called before most system specific initialization takes place.
// Initialize all general values here.
// See naStartApplication for a detailed explanation.
void preStartup(void* arg){
  NADateTime now = naMakeDateTimeNow();
  NAString* dateTimeStr = naNewStringWithDateTime(&now, NA_DATETIME_FORMAT_NATURAL);
  printf("DateTime: %s\n", naGetStringUTF8Pointer(dateTimeStr));
  naDelete(dateTimeStr);

  NAString* pwd = naNewStringWithCurWorkingDirectory();
  printf("Working Directory: %s\n", naGetStringUTF8Pointer(pwd));
  naDelete(pwd);

  NA_UNUSED(arg);
  app = naAlloc(GUIExamplesApplication);
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

  app->titleFont = naCreateFontWithPreset(NA_FONT_KIND_TITLE, NA_FONT_SIZE_DEFAULT);
  app->monoFont = naCreateFontWithPreset(NA_FONT_KIND_MONOSPACE, NA_FONT_SIZE_DEFAULT);

  naInitList(&app->flashList);

  // Create the controllers
  app->exampleController = createExampleController();
}



// Delete all controllers and finally, delete this application
void clearApplication(void* arg){
  NA_UNUSED(arg);

  naForeachStackpMutable(&(app->temperatureControllers), (NAMutator)despawnTemperatureController);
  naClearStack(&(app->temperatureControllers));
  clearExampleController(app->exampleController);
  naRelease(app->iconImageSet);
  naRelease(app->state1ImageSet);
  naRelease(app->state2ImageSet);
  naRelease(app->titleFont);
  naRelease(app->monoFont);
  naClearList(&app->flashList, naFree);
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

NAFont* getTitleFont(void) {
  return app->titleFont;
}
NAFont* getMonoFont(void) {
  return app->monoFont;
}


typedef struct FlashEntry FlashEntry;
struct FlashEntry {
  NADateTime lastChange;
  NALabel* label;
};

static void naUnflash(void* data) {
  NADateTime now = naMakeDateTimeNow();
  NAListIterator it = naMakeListModifier(&app->flashList);
  while(naIterateList(&it)) {
    FlashEntry* cur = naGetListCurMutable(&it);
    if(naGetDateTimeDifference(&now, &cur->lastChange) >= 1.) {
      naSetLabelText(cur->label, "");
      naRemoveListCurMutable(&it, NA_FALSE);
      naFree(cur);
    }
  }
  naClearListIterator(&it);
}

void flashLabel(NALabel* label, const NAUTF8Char* text) {
  naSetLabelText(label, text);
  // Add the label to the unflash-list
  NADateTime now = naMakeDateTimeNow();
  NAListIterator it = naMakeListModifier(&app->flashList);
  NABool found = NA_FALSE;
  while(naIterateList(&it)) {
    FlashEntry* cur = naGetListCurMutable(&it);
    if(cur->label == label) {
      cur->lastChange = now;
      found = NA_TRUE;
    }
  }
  naClearListIterator(&it);
  if(!found) {
    FlashEntry* newEntry = naAlloc(FlashEntry);
    newEntry->label = label;
    newEntry->lastChange = now;
    naAddListLastMutable(&app->flashList, newEntry);
  }
  // Erase the flash in 1 second:
  naCallApplicationFunctionInSeconds(naUnflash, NA_NULL, 1.);
}

void addTemperatureControllerToApplication(TemperatureController* con){
  *(TemperatureController**)naPushStack(&(app->temperatureControllers)) = con;
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
