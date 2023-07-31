
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1


// These macros must be different than the public macros used for
// naCreateXXXButton
#define NA_BUTTON_SUBMIT 0x10
#define NA_BUTTON_ABORT  0x20


NA_HDEF void na_InitButton(NAButton* button, void* nativePtr, const NAUIImage* uiImage, uint32 flags){
  na_InitUIElement(&(button->uiElement), NA_UI_BUTTON, nativePtr);
  if(uiImage){
    button->uiImage = naRetainConst(uiImage);
  }else{
    button->uiImage = NA_NULL;
  }
  button->flags = flags;
}



NA_HDEF void na_ClearButton(NAButton* button){
  if(button->uiImage){
    naReleaseConst(button->uiImage);
  }
  na_ClearUIElement(&(button->uiElement));
}



NA_HDEF void na_setButtonImage(NAButton* button, const NAUIImage* uiImage){
  if(button->uiImage){
    naReleaseConst(button->uiImage);
  }
  if(uiImage)
  {
    button->uiImage = naRetainConst(uiImage);
  }else{
    button->uiImage = NA_NULL;
  }
}



NA_HDEF NABool na_isButtonSubmit(const NAButton* button){
  return naGetFlagu32(button->flags, NA_BUTTON_SUBMIT);
}
NA_HDEF void na_setButtonSubmit(NAButton* button){
  #if NA_DEBUG
    if(na_isButtonAbort(button))
      naError("Button is already an abort button. Undefined behaviour.");
    if(na_isButtonSubmit(button))
      naError("Button is already a submit button. Undefined behaviour.");
  #endif
  naSetFlagu32(&button->flags, NA_BUTTON_SUBMIT, NA_TRUE);
}
NA_HDEF NABool na_isButtonAbort(const NAButton* button){
  return naGetFlagu32(button->flags, NA_BUTTON_ABORT);
}
NA_HDEF void na_setButtonAbort(NAButton* button){
  #if NA_DEBUG
    if(na_isButtonAbort(button))
      naError("Button is already an abort button. Undefined behaviour.");
    if(na_isButtonSubmit(button))
      naError("Button is already a submit button. Undefined behaviour.");
  #endif
  naSetFlagu32(&button->flags, NA_BUTTON_ABORT, NA_TRUE);
}


#endif // NA_COMPILE_GUI == 1
