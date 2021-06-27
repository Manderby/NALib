
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.



// To use the GUI implementation of NALib, you need to configure it. You can
// do this by defining the preprocessor macro NA_COMPILE_GUI to be 1.
//
// You can define NA_COMPILE_GUI either directly in NAConfiguration.h or add
// a preprocessor macro in your project settings. But the preferred ways are:
//
// XCode: Add a preprocessor macro to your solution settings. Add NALib as 
// a target. Add the Cocoa framework (and in this example the OpenGL framework)
// to your linked libraries.
//
// VisualStudio: Provide a property sheet named NAConfiguration.props right
// next to the solution (go see the example of this solution). Add the
// following line using a text editor to your project (go see this project):
// <Import Project="$(SolutionDir)\NAConfiguration.props" Condition="Exists('$(SolutionDir)\NAConfiguration.props')" />
// It is proposed to add this line right after the line which imports the
// property sheet "Microsoft.Cpp.Default.props". Add the NALib project.



// Note that you could add an include path in your project to access the
// NAXXX.h files. But for the sake of simplicity, we use relative paths here.
#include "../../NALib/src/NAUI.h"
#include "../../NALib/src/NAStack.h"

#include "HelloWorldGUI.h"


// Put GUI elements belonging together into a controller struct.
typedef struct ConverterApplication ConverterApplication;
struct ConverterApplication{
  NAStack controllers;
  int nextWindowX;
  int nextWindowY;
  ExperimentController* experimentController;
};

// The variable storing the app. For simplicity, here defined as global.
ConverterApplication* app;



// Put GUI elements belonging together into a controller struct.
typedef struct ConverterController ConverterController;
struct ConverterController{
  NAWindow* window;
  NAButton* button;
  NATextField* textField;
  NALabel* label;
  NAButton* newButton;
  NAButton* quitButton;
};



// Will be called when the calculate Button or Enter is pressed.
NABool pressButton(NAReaction reaction){
  // The reaction parameter contains - amongst other things - the controller
  // provided with naAddUIReaction.
  ConverterController* con = reaction.controller;

  // Read out the textField
  NAString* celsiusString = naNewStringWithTextFieldText(con->textField);
  double celsius = atof(naGetStringUTF8Pointer(celsiusString));
  naDelete(celsiusString);

  // Compute the model
  double fahrenheit = celsius * 1.8 + 32.;

  // Write the output to the label
  NAString* fahrenheitString = naNewStringWithFormat("%f", fahrenheit);
  naSetLabelText(con->label, naGetStringUTF8Pointer(fahrenheitString));
  naDelete(fahrenheitString);

  // By returning true, NALib knows that this event has been completely
  // handeled and it should stop propagating to other potential handlers.
  return NA_TRUE;
}



// Prototype
void createController(void);

// Will be called when the + Button is pressed.
NABool newController(NAReaction reaction){
  createController();
  return NA_TRUE;
}



// Will be called when the Quit Button is pressed.
NABool quitApplication(NAReaction reaction){
  naStopApplication();
  return NA_TRUE;
}



void createController(){
  // Note for the sake of simplicity, the memory allocated here will leak.
  // You should store this con pointer somewhere in the application and
  // clean up properly upon application stop.

  ConverterController* con = naPushStack(&(app->controllers));

  // Create a new window
  NARect windowrect = naMakeRectS(app->nextWindowX, app->nextWindowY, 400, 200);
  con->window = naNewWindow("Temperature Converter", windowrect, NA_FALSE, 0);

  // Update the window coordinates for the next window.
  app->nextWindowX += 20;
  app->nextWindowY -= 20;

  // Every window has a space which defines its contents.
  NASpace* windowSpace = naGetWindowContentSpace(con->window);

  // Create the main button, assign a press command and make it the default
  // button.
  con->button = naNewTextButton("Compute", naMakeSize(100, 30), 0);
  naAddSpaceChild(windowSpace, con->button, naMakePos(150, 20));
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, pressButton, con);
  naSetButtonSubmit(con->button, pressButton, con);

  // Create the textField.
  con->textField = naNewTextField(naMakeSize(80, 20));
  naSetTextFieldTextAlignment(con->textField, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(windowSpace, con->textField, naMakePos(160, 100));

  // Create the label containing the results.
  con->label = naNewLabel("Result", naMakeSize(80, 20));
  naSetLabelTextAlignment(con->label, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(windowSpace, con->label, naMakePos(160, 70));

  // Create a + button for opening a new window.
  con->newButton = naNewTextButton("+", naMakeSize(60, 30), 0);
  naAddSpaceChild(windowSpace, con->newButton, naMakePos(20, 20));
  naAddUIReaction(con->newButton, NA_UI_COMMAND_PRESSED, newController, con);

  // Create a Quit button for terminating the whole application.
  con->quitButton = naNewTextButton("Quit", naMakeSize(60, 30), 0);
  naAddSpaceChild(windowSpace, con->quitButton, naMakePos(320, 20));
  naAddUIReaction(con->quitButton, NA_UI_COMMAND_PRESSED, quitApplication, con);

  // Show the current window.
  naShowWindow(con->window);
}



// Clear all allocated elements.
void clearController(ConverterController* con){
  // Note that all UI elements which are attached in some way to the root
  // application UIElement will be cleared automatically.
}



// prestartup is called before most system specific initialization takes place.
// Initialize all global and general values here.
// See naStartApplication for a detailed explanation.
void prestartup(void* arg){
  NA_UNUSED(arg);
  app = naAlloc(ConverterApplication);
  naInitStack(&(app->controllers), sizeof(ConverterController), 0, 0);
  app->nextWindowX = 700;
  app->nextWindowY = 400;
}



// Delete all controllers and finally, delete this application
void clearApplication(void){
  naForeachStackMutable(&(app->controllers), (NAMutator)clearController);
  naClearStack(&(app->controllers));
  clearExperimentController(app->experimentController);
  naFree(app);
}



// poststartup is called after base application stuff has been initialized.
// Build-up your GUI here.
// See naStartApplication for a detailed explanation.
void poststartup(void* arg){
  NA_UNUSED(arg);
  createController();
  app->experimentController = createExperimentController();
}




int main(){

  naStartRuntime();
    naStartApplication(prestartup, poststartup, NA_NULL);

    // When returning from naStartApplication, the GUI has already been
    // released. But we need to free the memory allocated in the app and
    // the controllers.
    clearApplication();
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
