
#include "NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitImageSpace(NAImageSpace* imageSpace, void* nativePtr){
  na_InitUIElement(&(imageSpace->uiElement), NA_UI_IMAGE_SPACE, nativePtr);
}



NA_HDEF void na_ClearImageSpace(NAImageSpace* imageSpace){
  na_ClearUIElement(&(imageSpace->uiElement));
}



#endif // NA_COMPILE_GUI == 1
