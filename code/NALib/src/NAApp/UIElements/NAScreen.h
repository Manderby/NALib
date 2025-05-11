
#ifndef NA_UI_SCREEN_INCLUDED
#define NA_UI_SCREEN_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// These are the default resolutions given in rasterpositions/meter.
#define NA_UI_RESOLUTION_1x 3779.52755905511811   // 96 ppi
#define NA_UI_RESOLUTION_2x 7559.05511811023622   // 192 ppi
// One can dream of having a metric unit in the future. Like for example
// 4000 spm (subdivisions per meter). By chance, this would be almost, but not
// completely be the square root of 2 apart from 72 ppi and would barely be
// noticeable by the naked eye. Wouldn't that be a nice change of unit?
// But for now, we stick to the cumbersome relation to ppi.



// NALib will automatically create all present screen objects based on the
// current system settings. Retrieve the desired screen using the following
// mehtods:
//
// naGetApplicationMainScreen();
// naGetApplicationCenterScreen();
//
// You will only get const access. But then, you can use the following methods
// as well as for example naGetUIElementRect.

// Returns true if the given screen is the main screen
NA_API NABool naIsScreenMain(const NAScreen* screen);

// Returns the center relative to the whole screen setup. Results in values
// in the range of [0, 1], whereas 0.5 is in the center of the whole setup.
NA_API NAPos naGetScreenRelativeCenter(const NAScreen* screen);

// Returns the rectangle in the whole screen setup which is safe for display
// regarding the given screen. This excludes places where a dock, taskbar or
// menu might be located.
NA_API NARect naGetScreenUsableRect(const NAScreen* screen);

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
