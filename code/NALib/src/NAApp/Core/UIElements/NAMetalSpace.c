
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1

#include "../../UIElements/NAMetalSpace.h"


NA_HDEF void na_InitMetalSpace(NAMetalSpace* metalSpace, void* nativePtr) {
  na_InitUIElement(&metalSpace->uiElement, NA_UI_METAL_SPACE, nativePtr);
  naResetMetalSpaceTransformation(metalSpace);
}



NA_HDEF void na_ClearMetalSpace(NAMetalSpace* metalSpace) {
  na_ClearUIElement(&metalSpace->uiElement);
}



NA_DEF void naSetMetalSpaceTranslation(
  NAMetalSpace* metalSpace,
  double dx,
  double dy)
{
  naTranslateM33dS(metalSpace->transformation, dx, dy);
}



NA_DEF void naSetMetalSpaceMagnification(
  NAMetalSpace* metalSpace,
  double magnification)
{
  naScaleM33dS(metalSpace->transformation, magnification, magnification);
}



NA_DEF const double* naGetMetalSpaceTransformation(const NAMetalSpace* metalSpace) {
  return metalSpace->transformation;
}



NA_DEF void naResetMetalSpaceTransformation(NAMetalSpace* metalSpace) {
  naFillM33dWithDiag(metalSpace->transformation, 1.);
}



#endif // NA_COMPILE_GUI == 1
