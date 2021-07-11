
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.

#include "../../NALib/src/NAUI.h"
#include "HelloWorldGUI.h"



// Put GUI elements belonging together into a controller struct.
struct TemperatureController{
  NAWindow* window;
  NAButton* button;
  NALabel* infoLabel;
  NATextField* textField;
  NALabel* resutLabel;
  NAButton* newButton;
  NAButton* quitButton;
};



// Will be called when the calculate Button or Enter is pressed.
NABool pressButton(NAReaction reaction){
  // The reaction parameter contains - amongst other things - the controller
  // provided with naAddUIReaction.
  TemperatureController* con = reaction.controller;

  // Read out the textField
  NAString* celsiusString = naNewStringWithTextFieldText(con->textField);
  double celsius = atof(naGetStringUTF8Pointer(celsiusString));
  naDelete(celsiusString);

  // Compute the model
  double fahrenheit = celsius * 1.8 + 32.;

  // Write the output to the resutLabel
  NAString* fahrenheitString = naNewStringWithFormat("%f", fahrenheit);
  naSetLabelText(con->resutLabel, naGetStringUTF8Pointer(fahrenheitString));
  naDelete(fahrenheitString);

  // By returning true, NALib knows that this event has been completely
  // handeled and it should stop propagating to other potential handlers.
  return NA_TRUE;
}



// Will be called when the + Button is pressed.
NABool newTemperatureController(NAReaction reaction){
  createTemperatureController();
  return NA_TRUE;
}



// Will be called when the Quit Button is pressed.
NABool quitApplication(NAReaction reaction){
  naStopApplication();
  return NA_TRUE;
}



TemperatureController* createTemperatureController(){
  // Note for the sake of simplicity, the memory allocated here will leak.
  // You should store this con pointer somewhere in the application and
  // clean up properly upon application stop.

  TemperatureController* con = naAlloc(TemperatureController);

  // Create a new window
  NARect windowrect = naMakeRectS(getAndAdvanceNextWindowX(), getAndAdvanceNextWindowY(), 400, 200);
  con->window = naNewWindow("Temperature Converter", windowrect, NA_FALSE, 0);

  // Every window has a space which defines its contents.
  NASpace* windowSpace = naGetWindowContentSpace(con->window);

  // Create the main button, assign a press command and make it the default
  // button.
  con->button = naNewTextButton("Compute", naMakeSize(100, 30), 0);
  naAddSpaceChild(windowSpace, con->button, naMakePos(150, 20));
  naAddUIReaction(con->button, NA_UI_COMMAND_PRESSED, pressButton, con);
  naSetButtonSubmit(con->button, pressButton, con);

    // Create the infoLabel containing informations.
  con->infoLabel = naNewLabel("Temperature in Celsius", naMakeSize(160, 20));
  naSetLabelTextAlignment(con->infoLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(windowSpace, con->infoLabel, naMakePos(120, 130));

  // Create the textField.
  con->textField = naNewTextField(naMakeSize(80, 20));
  naSetTextFieldTextAlignment(con->textField, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(windowSpace, con->textField, naMakePos(160, 100));

  // Create the resutLabel containing the results.
  con->resutLabel = naNewLabel("Result in Fahrenheit", naMakeSize(160, 20));
  naSetLabelTextAlignment(con->resutLabel, NA_TEXT_ALIGNMENT_CENTER);
  naAddSpaceChild(windowSpace, con->resutLabel, naMakePos(120, 70));

  // Create a + button for opening a new window.
  con->newButton = naNewTextButton("+", naMakeSize(60, 30), 0);
  naAddSpaceChild(windowSpace, con->newButton, naMakePos(20, 20));
  naAddUIReaction(con->newButton, NA_UI_COMMAND_PRESSED, newTemperatureController, con);

  // Create a Quit button for terminating the whole application.
  con->quitButton = naNewTextButton("Quit", naMakeSize(60, 30), 0);
  naAddSpaceChild(windowSpace, con->quitButton, naMakePos(320, 20));
  naAddUIReaction(con->quitButton, NA_UI_COMMAND_PRESSED, quitApplication, con);

  // Show the current window.
  naShowWindow(con->window);

  addTemperatureControllerToApplication(con);

  return con;
}



// Clear all allocated elements.
void clearTemperatureController(TemperatureController* con){
  // Note that all UI elements which are attached in some way to the root
  // application UIElement will be cleared automatically.
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
