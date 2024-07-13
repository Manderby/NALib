
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitSlider(NASlider* slider, void* nativePtr) {
  na_InitUIElement(&slider->uiElement, NA_UI_SLIDER, nativePtr);
  slider->staticValue = 0.;
  slider->sliderInMovement = NA_FALSE;
  slider->min = 0.;
  slider->max = 1.;
}



NA_HDEF void na_ClearSlider(NASlider* slider) {
  na_ClearUIElement(&slider->uiElement);
}



#endif // NA_COMPILE_GUI == 1
