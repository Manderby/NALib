
#include "NAAppCore.h"



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
