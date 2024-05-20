
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitScreen(NAScreen* screen, void* nativePtr) {
  na_InitUIElement(&(screen->uiElement), NA_UI_SCREEN, nativePtr);
}



NA_HDEF void na_ClearScreen(NAScreen* screen) {
  na_ClearUIElement(&(screen->uiElement));
}



#endif // NA_COMPILE_GUI == 1
