
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



#include "../NAFont.h"
#include "../Core/NAAppCore.h"



NA_DEF NAFont* naCreateFont(const NAUTF8Char* fontFamilyName, uint32 flags, double size) {
  NSString* systemFontName = [NSString stringWithUTF8String: fontFamilyName];

  NSFont* systemFont = [NSFont systemFontOfSize:[NSFont systemFontSize]];
  NSFont* nativeFontObj;

  if(systemFontName == [systemFont familyName]) {
    nativeFontObj = (naGetFlagu32(flags, NA_FONT_FLAG_BOLD)) ?
      [NSFont systemFontOfSize:size] :
      [NSFont boldSystemFontOfSize:size];
  }else{
    NSFontTraitMask traits = 0;
    if(naGetFlagu32(flags, NA_FONT_FLAG_BOLD)) { traits |= NSBoldFontMask; }
    if(naGetFlagu32(flags, NA_FONT_FLAG_ITALIC)) { traits |= NSItalicFontMask; }
    if(naGetFlagu32(flags, NA_FONT_FLAG_UNDERLINE)) { }

    nativeFontObj = [[NSFontManager sharedFontManager]
      fontWithFamily:systemFontName
      traits:traits
      weight:5  // ignored if NSBoldFontMask is set.
      size:size];
  }
  
  NAString* fontName = naNewStringWithFormat("%s", fontFamilyName);
  
  // asdf
  NAFont* retFont = na_CreateFont(
    NA_COCOA_PTR_OBJC_TO_C(NA_COCOA_RETAIN(nativeFontObj)),
    fontName,
    flags,
    size);
    
  naDelete(fontName);
  
  return retFont;
}



NA_HDEF void na_DestructFontNativePtr(void* nativeFontPtr) {
  NA_COCOA_RELEASE(NA_COCOA_PTR_C_TO_OBJC(nativeFontPtr));
}



NAFont* naCreateFontWithPreset(NAFontKind kind, NAFontSize fontSize) {
  CGFloat baseSize;
  switch(fontSize) {
  case NA_FONT_SIZE_SMALL: baseSize = 11; break;
  case NA_FONT_SIZE_DEFAULT: baseSize = [NSFont systemFontSize]; break;
  case NA_FONT_SIZE_BIG: baseSize = 18; break;
  case NA_FONT_SIZE_HUGE: baseSize = 24; break;
  default: baseSize = [NSFont systemFontSize]; break;
  }

  NSFont* systemFont = [NSFont systemFontOfSize:[NSFont systemFontSize]];

  NAFont* retFont;
  switch(kind) {
    case NA_FONT_KIND_SYSTEM:
      retFont = naCreateFont([[systemFont familyName] UTF8String], NA_FONT_FLAG_REGULAR, baseSize);
      break;
    case NA_FONT_KIND_TITLE:
      retFont = naCreateFont([[systemFont familyName] UTF8String], NA_FONT_FLAG_BOLD, baseSize);
      break;
    case NA_FONT_KIND_MONOSPACE:
      retFont = naCreateFont("Courier", NA_FONT_FLAG_REGULAR, baseSize);
      break;
    case NA_FONT_KIND_PARAGRAPH:
      retFont = naCreateFont("Palatino", NA_FONT_FLAG_REGULAR, baseSize);
      break;
    case NA_FONT_KIND_MATH:
      // Note: Times new roman would be more traditional but unicode support is worse.
      retFont = naCreateFont("STIX Two Math", NA_FONT_FLAG_REGULAR, baseSize);
      //retFont = naCreateFont("STIX Two Text", NA_FONT_FLAG_ITALIC, baseSize);
      break;
    default:
      #if NA_DEBUG
        naError("Unknown font kind");
      #endif
      retFont = naCreateFont("San Francisco", NA_FONT_FLAG_REGULAR, baseSize);
      break;
  }
  
  return retFont;
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
