
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitTextField(NATextField* textField, void* nativePtr){
  na_InitUIElement(&(textField->uiElement), NA_UI_TEXTFIELD, nativePtr);
}



NA_HDEF void na_ClearTextField(NATextField* textField){
  na_ClearUIElement(&(textField->uiElement));
  naRelease(textField->font);
}



#endif // NA_COMPILE_GUI == 1
