
#include <NAApp/NAApp.h>



typedef struct CommonController CommonController;
struct CommonController{
  NASpace* space;
  NAMutator clearer;
  NAMutator updater;
};

void initCommonController(CommonController* con, NASpace* space, NAMutator clearer, NAMutator updater);
void clearCommonController(CommonController* con);
NASpace* getCommonControllerSpace(CommonController* con);
void updateCommonController(CommonController* con);

typedef struct ApplicationController ApplicationController;
typedef struct ScreenController ScreenController;
typedef struct WindowController WindowController;
typedef struct SpaceController SpaceController;
typedef struct OpenGLSpaceController OpenGLSpaceController;
typedef struct MetalSpaceController MetalSpaceController;
typedef struct ImageSpaceController ImageSpaceController;

typedef struct ButtonController ButtonController;
typedef struct CheckBoxController CheckBoxController;
typedef struct LabelController LabelController;
typedef struct MenuController MenuController;
typedef struct RadioController RadioController;
typedef struct SelectController SelectController;
typedef struct SliderController SliderController;
typedef struct TextBoxController TextBoxController;
typedef struct TextFieldController TextFieldController;

typedef struct FontController FontController;



CommonController* createApplicationController(void);
CommonController* createScreenController(void);
CommonController* createWindowController(void);
CommonController* createSpaceController(void);
CommonController* createOpenGLSpaceController(void);
CommonController* createMetalSpaceController(void);
CommonController* createImageSpaceController(void);

CommonController* createButtonController(void);
CommonController* createCheckBoxController(void);
CommonController* createLabelController(void);
CommonController* createMenuController(void);
CommonController* createRadioController(void);
CommonController* createSelectController(void);
CommonController* createSliderController(void);
CommonController* createTextBoxController(void);
CommonController* createTextFieldController(void);

CommonController* createFontController(void);



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
