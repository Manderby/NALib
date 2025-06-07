
// This is an example showing how to create a GUI application with NALib which
// converts temperatures from Degree Celsius to Degree Fahrenheit.

#include "NAApp/NAApp.h"


typedef struct TemperatureController TemperatureController;
TemperatureController* spawnTemperatureController(void);
void despawnTemperatureController(TemperatureController* con);

typedef struct ExampleController ExampleController;
ExampleController* createExampleController(void);
void clearExampleController(ExampleController* con);



typedef struct ApplicationController ApplicationController;
ApplicationController* createApplicationController(void);
void clearApplicationController(ApplicationController* con);
NASpace* getApplicationControllerSpace(ApplicationController* con);
void updateApplicationController(ApplicationController* con);

typedef struct ScreenController ScreenController;
ScreenController* createScreenController(void);
void clearScreenController(ScreenController* con);
NASpace* getScreenControllerSpace(ScreenController* con);
void updateScreenController(ScreenController* con);

typedef struct WindowController WindowController;
WindowController* createWindowController(void);
void clearWindowController(WindowController* con);
NASpace* getWindowControllerSpace(WindowController* con);
void updateWindowController(WindowController* con);

typedef struct SpaceController SpaceController;
SpaceController* createSpaceController(void);
void clearSpaceController(SpaceController* con);
NASpace* getSpaceControllerSpace(SpaceController* con);
void updateSpaceController(SpaceController* con);

typedef struct OpenGLSpaceController OpenGLSpaceController;
OpenGLSpaceController* createOpenGLSpaceController(void);
void clearOpenGLSpaceController(OpenGLSpaceController* con);
NASpace* getOpenGLSpaceControllerSpace(OpenGLSpaceController* con);
void updateOpenGLSpaceController(OpenGLSpaceController* con);

typedef struct MetalSpaceController MetalSpaceController;
MetalSpaceController* createMetalSpaceController(void);
void clearMetalSpaceController(MetalSpaceController* con);
NASpace* getMetalSpaceControllerSpace(MetalSpaceController* con);
void updateMetalSpaceController(MetalSpaceController* con);

typedef struct ImageSpaceController ImageSpaceController;
ImageSpaceController* createImageSpaceController(void);
void clearImageSpaceController(ImageSpaceController* con);
NASpace* getImageSpaceControllerSpace(ImageSpaceController* con);
void updateImageSpaceController(ImageSpaceController* con);



typedef struct ButtonController ButtonController;
ButtonController* createButtonController(void);
void clearButtonController(ButtonController* con);
NASpace* getButtonControllerSpace(ButtonController* con);
void updateButtonController(ButtonController* con);

typedef struct CheckBoxController CheckBoxController;
CheckBoxController* createCheckBoxController(void);
void clearCheckBoxController(CheckBoxController* con);
NASpace* getCheckBoxControllerSpace(CheckBoxController* con);
void updateCheckBoxController(CheckBoxController* con);

typedef struct LabelController LabelController;
LabelController* createLabelController(void);
void clearLabelController(LabelController* con);
NASpace* getLabelControllerSpace(LabelController* con);
void updateLabelController(LabelController* con);

typedef struct MenuController MenuController;
MenuController* createMenuController(void);
void clearMenuController(MenuController* con);
NASpace* getMenuControllerSpace(MenuController* con);
void updateMenuController(MenuController* con);

typedef struct RadioController RadioController;
RadioController* createRadioController(void);
void clearRadioController(RadioController* con);
NASpace* getRadioControllerSpace(RadioController* con);
void updateRadioController(RadioController* con);

typedef struct SelectController SelectController;
SelectController* createSelectController(void);
void clearSelectController(SelectController* con);
NASpace* getSelectControllerSpace(SelectController* con);
void updateSelectController(SelectController* con);

typedef struct SliderController SliderController;
SliderController* createSliderController(void);
void clearSliderController(SliderController* con);
NASpace* getSliderControllerSpace(SliderController* con);
void updateSliderController(SliderController* con);

typedef struct TextBoxController TextBoxController;
TextBoxController* createTextBoxController(void);
void clearTextBoxController(TextBoxController* con);
NASpace* getTextBoxControllerSpace(TextBoxController* con);
void updateTextBoxController(TextBoxController* con);

typedef struct TextFieldController TextFieldController;
TextFieldController* createTextFieldController(void);
void clearTextFieldController(TextFieldController* con);
NASpace* getTextFieldControllerSpace(TextFieldController* con);
void updateTextFieldController(TextFieldController* con);



typedef struct FontController FontController;
FontController* createFontController(void);
void clearFontController(FontController* con);
NASpace* getFontControllerSpace(FontController* con);
void updateFontController(FontController* con);



typedef struct GUIExamplesApplication GUIExamplesApplication;
void preStartup(void* arg);
void postStartup(void* arg);
void clearApplication(void* arg);
double getAndAdvanceNextWindowX(void);
double getAndAdvanceNextWindowY(void);
NAImageSet* getIconImageSet(void);
NAImageSet* getState1ImageSet(void);
NAImageSet* getState2ImageSet(void);
NAFont* getTitleFont(void);
NAFont* getMonoFont(void);
void flashLabel(NALabel* label, const NAUTF8Char* text);


void addTemperatureControllerToApplication(TemperatureController* con);



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
