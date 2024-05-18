
#if defined NA_UI_RADIO_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_RADIO_INCLUDED
#define NA_UI_RADIO_INCLUDED



// Creates a new Radio. Default height is 18.
NA_API NARadio* naNewRadio(const NAUTF8Char* text, double width);

// Changes the visibility or enabled state of the radio.
NA_API void naSetRadioVisible(NARadio* radio, NABool visible);
NA_API void naSetRadioEnabled(NARadio* radio, NABool enabled);

// Gets or sets the state of the radio.
NA_API NABool naGetRadioState(const NARadio* radio);
NA_API void naSetRadioState(NARadio* radio, NABool state);

// Sets the color of the text of the radio.
NA_API void naSetRadioTextColor(NARadio* radio, const NAABYColor* color);



#endif // NA_UI_RADIO_INCLUDED



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
