
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



// These macros must be different than the public macros used for
// naCreateXXXButton
#define NA_BUTTON_SUBMIT 0x10
#define NA_BUTTON_ABORT  0x20


NA_HDEF void na_InitButton(NAButton* button, void* nativePtr, const NAUTF8Char* textOff, const NAUTF8Char* textOn, const NAUIImage* uiImageOff, const NAUIImage* uiImageOn, uint32 flags){
  na_InitUIElement(&(button->uiElement), NA_UI_BUTTON, nativePtr);
  button->textOff = NA_NULL;
  button->textOn = NA_NULL;
  button->uiImageOff = NA_NULL;
  button->uiImageOn = NA_NULL;
  na_setButtonTextOff(button, textOff);
  na_setButtonTextOn(button, textOn);
  na_setButtonImageOff(button, uiImageOff);
  na_setButtonImageOn(button, uiImageOn);
  button->flags = flags;
}



NA_HDEF void na_ClearButton(NAButton* button){
  if(button->uiImageOff){
    naReleaseConst(button->uiImageOff);
  }
  if(button->uiImageOn){
    naReleaseConst(button->uiImageOn);
  }
  na_ClearUIElement(&(button->uiElement));
}



NA_HDEF void na_setButtonTextOff(NAButton* button, const NAUTF8Char* text){
  if(button->textOff){
    naFree(button->textOff);
  }
  if(text){
    size_t len = strlen(text);
    button->textOff = naMalloc(len + 1);
    memcpy(button->textOff, text, len);
    button->textOff[len] = '\0';
  }else{
    button->textOff = NA_NULL;
  }
}



NA_HDEF void na_setButtonTextOn(NAButton* button, const NAUTF8Char* text){
  if(button->textOn){
    naFree(button->textOn);
  }
  if(text){
    size_t len = strlen(text);
    button->textOn = naMalloc(len + 1);
    memcpy(button->textOn, text, len);
    button->textOn[len] = '\0';
  }else{
    button->textOn = NA_NULL;
  }
}



NA_HDEF void na_setButtonImageOff(NAButton* button, const NAUIImage* uiImage){
  if(button->uiImageOff){
    naReleaseConst(button->uiImageOff);
  }
  if(uiImage){
    button->uiImageOff = naRetainConst(uiImage);
  }else{
    button->uiImageOff = NA_NULL;
  }
}



NA_HDEF void na_setButtonImageOn(NAButton* button, const NAUIImage* uiImage){
  if(button->uiImageOn){
    naReleaseConst(button->uiImageOn);
  }
  if(uiImage){
    button->uiImageOn = naRetainConst(uiImage);
  }else{
    button->uiImageOn = NA_NULL;
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
