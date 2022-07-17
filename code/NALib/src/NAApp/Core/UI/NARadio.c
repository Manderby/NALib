
#include "NAAppCore.h"



NA_HDEF void na_InitRadio(NARadio* radio, void* nativePtr){
  na_InitUIElement(&(radio->uiElement), NA_UI_RADIO, nativePtr);
}



NA_HDEF void na_ClearRadio(NARadio* radio){
  na_ClearUIElement(&(radio->uiElement));
}

