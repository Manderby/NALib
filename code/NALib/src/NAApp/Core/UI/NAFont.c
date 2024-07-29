
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HAPI void na_DestructFont(NAFont* font);
NA_RUNTIME_TYPE(NAFont, na_DestructFont, NA_TRUE);



NA_DEF NAFont* naGetSystemFont() {
  if(!na_App->systemFont) {
    na_App->systemFont = naCreateFontWithPreset(NA_FONT_KIND_SYSTEM, NA_FONT_SIZE_DEFAULT);
  }
  return na_App->systemFont;
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
