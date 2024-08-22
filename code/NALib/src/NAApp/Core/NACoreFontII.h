
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



struct NAFont{
  void* nativePtr;  // HFONT on Windows, NSFont* on Mac
  NAString* name;
  uint32 flags;
  double size;
};

NA_HAPI void na_DestructFont(NAFont* font);
NA_RUNTIME_TYPE(NAFont, na_DestructFont, NA_TRUE);



NA_HDEF NAFont* na_CreateFont(
  void* nativePtr,
  const NAString* name,
  uint32 flags,
  double size)
{
  NAFont* font = naCreate(NAFont);
  font->nativePtr = nativePtr;
  font->name = naNewStringExtraction(name, 0, -1);
  font->flags = flags;
  font->size = size;
  return font;
}

NA_HDEF void na_DestructFont(NAFont* font) {
  na_DestructFontNativePtr(font->nativePtr);
  naDelete(font->name);
}



NA_DEF NAFont* naCreateSystemFont(void) {
  return naCreateFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_DEFAULT);
}

NA_DEF void* naGetFontNativePointer(const NAFont* font) {
  return font->nativePtr;
}

NA_DEF const NAString* naGetFontName(const NAFont* font) {
  return font->name;
}

NA_DEF uint32 naGetFontFlags(const NAFont* font) {
  return font->flags;
}

NA_DEF double naGetFontSize(const NAFont* font) {
  return font->size;
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
