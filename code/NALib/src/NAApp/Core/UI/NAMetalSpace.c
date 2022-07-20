
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitMetalSpace(NAMetalSpace* metalSpace, void* nativePtr){
  na_InitUIElement(&(metalSpace->uiElement), NA_UI_METAL_SPACE, nativePtr);
}



NA_HDEF void na_ClearMetalSpace(NAMetalSpace* metalSpace){
  na_ClearUIElement(&(metalSpace->uiElement));
}



#endif // NA_COMPILE_GUI == 1
