
#include "NAAppCore.h"

#if NA_COMPILE_GUI == 1

#include "../NAFont.h"



struct NAFont{
  void* nativePtr;  // HFONT on Windows, NSFont* on Mac
  NAString* name;
  uint32 flags;
  double size;
};


NA_HAPI void na_DestructFont(NAFont* font);
NA_RUNTIME_TYPE(NAFont, na_DestructFont, NA_TRUE);



NA_HDEF NAFont* na_CreateFont(void* nativePtr, const NAString* name, uint32 flags, double size) {
  NAFont* font = naCreate(NAFont);
  font->nativePtr = nativePtr;
  font->name = naNewStringExtraction(name, 0, -1);
  font->flags = flags,
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


#endif // NA_COMPILE_GUI == 1
