
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitOpenGLSpace(NAOpenGLSpace* openGLSpace, void* nativePtr){
  na_InitUIElement(&(openGLSpace->uiElement), NA_UI_OPENGL_SPACE, nativePtr);
}



NA_HDEF void na_ClearOpenGLSpace(NAOpenGLSpace* openGLSpace){
  na_ClearUIElement(&(openGLSpace->uiElement));
}



#endif // NA_COMPILE_GUI == 1
