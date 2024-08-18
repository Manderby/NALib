
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitRadio(NARadio* radio, void* nativePtr) {
  na_InitUIElement(&radio->uiElement, NA_UI_RADIO, nativePtr);
}



NA_HDEF void na_ClearRadio(NARadio* radio) {
  na_ClearUIElement(&radio->uiElement);
}



#endif // NA_COMPILE_GUI == 1
