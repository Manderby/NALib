
#include "NAAppCore.h"



NA_HDEF void na_InitSlider(NASlider* slider, void* nativePtr){
  na_InitUIElement(&(slider->uiElement), NA_UI_SLIDER, nativePtr);
  slider->staticValue = 0.;
  slider->sliderInMovement = NA_FALSE;
}



NA_HDEF void na_ClearSlider(NASlider* slider){
  na_ClearUIElement(&(slider->uiElement));
}

