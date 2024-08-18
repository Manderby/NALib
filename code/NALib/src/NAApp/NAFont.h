
#ifndef NA_FONT_INCLUDED
#define NA_FONT_INCLUDED



NA_PROTOTYPE(NAString);



// NALib defines a set of well working fonts with different sizes. They have
// been tested on different systems and provide a consistent appearance.
typedef enum{
  NA_FONT_KIND_SYSTEM,      // The default system font
  NA_FONT_KIND_TITLE,       // A bolder kind of the default system font
  NA_FONT_KIND_MONOSPACE,   // A monospace font.
  NA_FONT_KIND_PARAGRAPH,   // A nice to read font for displaying longer texts.
  NA_FONT_KIND_MATH,        // A font close to mathematical representation.
  NA_FONT_KIND_COUNT
} NAFontKind;

typedef enum{
  NA_FONT_SIZE_SMALL,
  NA_FONT_SIZE_DEFAULT,     // The default system font size
  NA_FONT_SIZE_BIG,
  NA_FONT_SIZE_HUGE,
  NA_FONT_SIZE_COUNT
} NAFontSize;

#define NA_FONT_FLAG_REGULAR    0x00
#define NA_FONT_FLAG_BOLD       0x01
#define NA_FONT_FLAG_ITALIC     0x02
#define NA_FONT_FLAG_UNDERLINE  0x04

typedef enum{
  NA_TEXT_ALIGNMENT_LEFT,
  NA_TEXT_ALIGNMENT_RIGHT,
  NA_TEXT_ALIGNMENT_CENTER
} NATextAlignment;




typedef struct NAFont NAFont;
NA_EXTERN_RUNTIME_TYPE(NAFont);



// Creates a custom font. Use a combination of NA_FONT_FLAG_??? for flags.
NA_API NAFont* naCreateFont(
  const NAUTF8Char* fontFamilyName,
  uint32 flags,
  double size);

// Creates a preset font.
NA_API NAFont* naCreateFontWithPreset(
  NAFontKind kind,
  NAFontSize size);

// Creates a new font with the default font of the system.
NA_API NAFont* naCreateSystemFont(void);

// Getter
NA_API const NAString* naGetFontName(const NAFont* font);
NA_API uint32 naGetFontFlags(const NAFont* font);
NA_API double naGetFontSize(const NAFont* font);

// Returns the native representation of the font.
// HFONT on windows
// NSFont* on macOS
NA_API void* naGetFontNativePointer(const NAFont* font);



#endif // NA_FONT_INCLUDED



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
