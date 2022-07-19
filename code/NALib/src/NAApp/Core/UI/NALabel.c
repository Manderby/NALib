
#include "NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitLabel(NALabel* label, void* nativePtr){
  na_InitUIElement(&(label->uiElement), NA_UI_LABEL, nativePtr);
}



NA_HDEF void na_ClearLabel(NALabel* label){
  na_ClearUIElement(&(label->uiElement));
  naRelease(label->font);
}



#endif // NA_COMPILE_GUI == 1
