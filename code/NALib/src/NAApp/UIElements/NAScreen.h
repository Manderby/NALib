
#ifndef NA_UI_SCREEN_INCLUDED
#define NA_UI_SCREEN_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// These are the default resolutions given in rasterpositions/meter.
#define NA_UI_RESOLUTION_1x 3779.52755905511811   // 96 ppi
#define NA_UI_RESOLUTION_2x 7559.05511811023622   // 192 ppi



// Screens are not properly implemented yet.



// Returns the main screen rect.
NA_API NARect naGetMainScreenRect(void);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_SCREEN_INCLUDED



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
