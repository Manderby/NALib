
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.

#include "HelloWorldGUI.h"
#include "../../NALib/src/NAStack.h"


// Put GUI elements belonging together into a controller struct.
struct HelloWorldGUIApplication{
  NAStack temperatureControllers;
  int nextWindowX;
  int nextWindowY;
  ExperimentController* experimentController;
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
  createTemperatureController();
  app->experimentController = createExperimentController();
}



// Delete all controllers and finally, delete this application
void clearApplication(void){
  naForeachStackMutable(&(app->temperatureControllers), (NAMutator)clearTemperatureController);
  naClearStack(&(app->temperatureControllers));
  clearExperimentController(app->experimentController);
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


addTemperatureControllerToApplication(TemperatureController* con){
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
