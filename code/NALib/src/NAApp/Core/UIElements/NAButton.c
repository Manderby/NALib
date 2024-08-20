
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitButton(NAButton* button, void* nativePtr, const NAUTF8Char* text, const NAUTF8Char* text2, const NAImageSet* imageSet, const NAImageSet* imageSet2, uint32 flags) {
  na_InitUIElement(&button->uiElement, NA_UI_BUTTON, nativePtr);
  button->text = NA_NULL;
  button->text2 = NA_NULL;
  button->imageSet = NA_NULL;
  button->imageSet2 = NA_NULL;
  na_setButtonText(button, text);
  na_setButtonText2(button, text2);
  na_setButtonImage(button, imageSet);
  na_setButtonImage2(button, imageSet2);
  button->flags = flags;
}



NA_HDEF void na_ClearButton(NAButton* button) {
  if(button->text)
    naFree(button->text);
  if(button->text2)
    naFree(button->text2);
  if(button->imageSet)
    naReleaseConst(button->imageSet);
  if(button->imageSet2)
    naReleaseConst(button->imageSet2);
    
  na_ClearUIElement(&button->uiElement);
}



NA_HDEF void na_setButtonText(NAButton* button, const NAUTF8Char* text) {
  if(button->text) {
    naFree(button->text);
  }
  if(text) {
    size_t len = strlen(text);
    button->text = naMalloc(len + 1);
    memcpy(button->text, text, len);
    button->text[len] = '\0';
  }else{
    button->text = NA_NULL;
  }
}



NA_HDEF void na_setButtonText2(NAButton* button, const NAUTF8Char* text) {
  if(button->text2) {
    naFree(button->text2);
  }
  if(text) {
    size_t len = strlen(text);
    button->text2 = naMalloc(len + 1);
    memcpy(button->text2, text, len);
    button->text2[len] = '\0';
  }else{
    button->text2 = NA_NULL;
  }
}



NA_HDEF void na_setButtonImage(NAButton* button, const NAImageSet* imageSet) {
  if(button->imageSet) {
    naReleaseConst(button->imageSet);
  }
  if(imageSet) {
    button->imageSet = naRetainConst(imageSet);
  }else{
    button->imageSet = NA_NULL;
  }
}



NA_HDEF void na_setButtonImage2(NAButton* button, const NAImageSet* imageSet) {
  if(button->imageSet2) {
    naReleaseConst(button->imageSet2);
  }
  if(imageSet) {
    button->imageSet2 = naRetainConst(imageSet);
  }else{
    button->imageSet2 = NA_NULL;
  }
}



#endif // NA_COMPILE_GUI == 1
