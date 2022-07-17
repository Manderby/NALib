
#include "NAAppCore.h"



NA_HDEF void na_InitTextBox(NATextBox* textBox, void* nativePtr){
  na_InitUIElement(&(textBox->uiElement), NA_UI_TEXTBOX, nativePtr);
}



NA_HDEF void na_ClearTextBox(NATextBox* textBox){
  na_ClearUIElement(&(textBox->uiElement));
  naRelease(textBox->font);
}

