
#ifndef NA_LABEL_INCLUDED
#define NA_LABEL_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// An NALabel does not reacts to any command.



// Creates a new label. Default Height is 16.
// A label is by default: Enabled, visible, selectable
NA_API NALabel* naNewLabel(const NAUTF8Char* text, double width);

// Gets or sets the visibility or enabled state of the button.
NA_API void naSetLabelVisible(NALabel* label, NABool visible);
NA_API NABool naIsLabelEnabled(const NALabel* label);
NA_API void naSetLabelEnabled(NALabel* label, NABool enabled);

// Sets the contents of a label. Setting a link makes the whole label
// clickable and opens the default browser with the given url.
// Note that text alignment will be overwritten when setting a link.
NA_API void naSetLabelText(NALabel* label, const NAUTF8Char* text);
NA_API void naSetLabelLink(NALabel* label, const NAUTF8Char* url);

// Sets general properties of a label.
NA_API void naSetLabelSelectable(NALabel* label, NABool selectable);
NA_API void naSetLabelHeight(NALabel* label, double height);

// Sets font properties of a label.
NA_API void naSetLabelTextColor(NALabel* label, const NAColor* color);
NA_API void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment);
NA_API void naSetLabelFont(NALabel* label, NAFont* font);

void naFillLabelTextColor(NAColor* color, const NALabel* label);


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_LABEL_INCLUDED



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
