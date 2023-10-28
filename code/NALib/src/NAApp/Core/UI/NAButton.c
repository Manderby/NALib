
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitButton(NAButton* button, void* nativePtr, const NAUTF8Char* text, const NAUTF8Char* text2, const NAUIImage* uiImage, const NAUIImage* uiImage2, uint32 flags){
  na_InitUIElement(&(button->uiElement), NA_UI_BUTTON, nativePtr);
  button->text = NA_NULL;
  button->text2 = NA_NULL;
  button->uiImage = NA_NULL;
  button->uiImage2 = NA_NULL;
  na_setButtonText(button, text);
  na_setButtonText2(button, text2);
  na_setButtonImage(button, uiImage);
  na_setButtonImage2(button, uiImage2);
  button->flags = flags;
}



NA_HDEF void na_ClearButton(NAButton* button){
  if(button->text){naFree(button->text);}
  if(button->text2){naFree(button->text2);}
  if(button->uiImage){
    naReleaseConst(button->uiImage);
  }
  if(button->uiImage2){
    naReleaseConst(button->uiImage2);
  }
  na_ClearUIElement(&(button->uiElement));
}



NA_HDEF void na_setButtonText(NAButton* button, const NAUTF8Char* text){
  if(button->text){
    naFree(button->text);
  }
  if(text){
    size_t len = strlen(text);
    button->text = naMalloc(len + 1);
    memcpy(button->text, text, len);
    button->text[len] = '\0';
  }else{
    button->text = NA_NULL;
  }
}



NA_HDEF void na_setButtonText2(NAButton* button, const NAUTF8Char* text){
  if(button->text2){
    naFree(button->text2);
  }
  if(text){
    size_t len = strlen(text);
    button->text2 = naMalloc(len + 1);
    memcpy(button->text2, text, len);
    button->text2[len] = '\0';
  }else{
    button->text2 = NA_NULL;
  }
}



NA_HDEF void na_setButtonImage(NAButton* button, const NAUIImage* uiImage){
  if(button->uiImage){
    naReleaseConst(button->uiImage);
  }
  if(uiImage){
    button->uiImage = naRetainConst(uiImage);
  }else{
    button->uiImage = NA_NULL;
  }
}



NA_HDEF void na_setButtonImage2(NAButton* button, const NAUIImage* uiImage){
  if(button->uiImage2){
    naReleaseConst(button->uiImage2);
  }
  if(uiImage){
    button->uiImage2 = naRetainConst(uiImage);
  }else{
    button->uiImage2 = NA_NULL;
  }
}



#endif // NA_COMPILE_GUI == 1
