
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1

#include "../../UIElements/NATextField.h"


NA_HDEF void na_InitTextField(NATextField* textField, void* nativePtr) {
  na_InitUIElement(&textField->uiElement, NA_UI_TEXTFIELD, nativePtr);
}



NA_HDEF void na_ClearTextField(NATextField* textField) {
  na_ClearUIElement(&textField->uiElement);
  naRelease(textField->font);
}



NA_DEF double naGetTextFieldDouble(NATextField* textField) {
  NAString* string = naNewStringWithTextFieldText(textField);
  double value = atof(naGetStringUTF8Pointer(string));
  naDelete(string);
  return value;
}



#endif // NA_COMPILE_GUI == 1
