
#include "NAAppCore.h"



NA_HDEF void na_InitTextField(NATextField* textField, void* nativePtr){
  na_InitUIElement(&(textField->uiElement), NA_UI_TEXTFIELD, nativePtr);
}



NA_HDEF void na_ClearTextField(NATextField* textField){
  na_ClearUIElement(&(textField->uiElement));
  naRelease(textField->font);
}
