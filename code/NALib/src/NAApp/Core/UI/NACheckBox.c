
#include "NAAppCore.h"



NA_HDEF void na_InitCheckBox(NACheckBox* checkBox, void* nativePtr){
  na_InitUIElement(&(checkBox->uiElement), NA_UI_CHECKBOX, nativePtr);
}



NA_HDEF void na_ClearCheckBox(NACheckBox* checkBox){
  na_ClearUIElement(&(checkBox->uiElement));
}
