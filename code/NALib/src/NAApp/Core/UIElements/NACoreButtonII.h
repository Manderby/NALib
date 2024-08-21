
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



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



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
