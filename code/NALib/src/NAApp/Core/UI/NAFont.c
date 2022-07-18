
#include "NAAppCore.h"



NA_HAPI void na_DestructFont(NAFont* font);
NA_RUNTIME_TYPE(NAFont, na_DestructFont, NA_TRUE);

NA_DEF NAFont* naGetSystemFont(){
  return na_App->systemFont;
}

NA_DEF void* naGetFontNativePointer(const NAFont* font){
  return font->nativePtr;
}

NA_DEF const NAString* naGetFontName(const NAFont* font){
  return font->name;
}
NA_DEF uint32 naGetFontFlags(const NAFont* font){
  return font->flags;
}
NA_DEF double naGetFontSize(const NAFont* font){
  return font->size;
}
