
#ifndef NA_UI_COLOR_INCLUDED
#define NA_UI_COLOR_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// Windows and macOS have different internal structures to represent color.
// Windows: NAWINAPIColor struct consisting of a COLORREF and HBRUSH.
// macOS:   NSColor class.
typedef void NAUIColor;

// Allocate and deallocate colors used for system dependent visualization.
// Some systems have no notion of transparency, so converting an NAColor
// properly requires the background color. Can be nullptr to make it opaque.
NA_API NAUIColor* naAllocUIColor(const NAColor* color, const NAColor* bgColor);
NA_API void naDeallocUIColor(NAUIColor* uiColor);

// Other application and system dependent color functions:

#if NA_OS == NA_OS_WINDOWS
  // Conversion functions for COLORREF value used in WinAPI.
  NA_API void naFillColorWithColorRef(NAColor* color, const void* colorRef);
  NA_API void naFillColorRefWithColor(void* colorRef, const NAColor* color);

  NA_API NAUIColor* naAllocUIColorWithColorRef(const void* colorRef);
#endif

NA_API void naFillColorWithDefaultWindowBackgroundColor(NAColor* color);
NA_API void naFillColorWithDefaultTextColor(NAColor* color);
NA_API void naFillColorWithDefaultLinkColor(NAColor* color);
NA_API void naFillColorWithDefaultAccentColor(NAColor* color);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_COLOR_INCLUDED



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
