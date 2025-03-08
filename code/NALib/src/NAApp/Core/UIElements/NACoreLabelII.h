
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitLabel(NALabel* label, void* nativePtr) {
  na_InitCoreUIElement(&label->uiElement, NA_UI_LABEL, nativePtr);
  label->font = naCreateSystemFont();
  label->textColor = NA_NULL;
}



NA_HDEF void na_ClearLabel(NALabel* label) {
  na_ClearCoreUIElement(&label->uiElement);
  if(label->textColor) { naFree(label->textColor); }
  naRelease(label->font);
}



NA_HDEF void na_SetLabelTextColor(NALabel* label, const NAColor* color) {
  if(label->textColor) { naFree(label->textColor); }
  if(color) {
    label->textColor = naAlloc(NAColor);
    naFillColorWithCopy(label->textColor, color);
  }else{
    label->textColor = NA_NULL;
  }
}



void naFillLabelTextColor(NAColor* color, const NALabel* label) {
  #if NA_DEBUG
  if(!label)
    naError("label is nullptr");
  #endif

  if(label->textColor) {
    naFillColorWithCopy(color, label->textColor);
  }else{
    naFillColorWithSystemSkinDefaultTextColor(color);
  }

  color->alpha *= naIsLabelEnabled(label) ? 1.f : .49f;
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
