
#if defined NA_UI_TEXTFIELD_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_TEXTFIELD_INCLUDED
#define NA_UI_TEXTFIELD_INCLUDED



// Creates a new TextField. Default height is 21.
NA_API NATextField* naNewTextField(double width);

// Get or set the text of the textField.
NA_API NAString* naNewStringWithTextFieldText(const NATextField* textField);
NA_API void naSetTextFieldText(NATextField* textField, const NAUTF8Char* text);

// Set the color of the textfield. Expects an NAABYColor. If color is NULL,
// the default control color is set.
NA_API void naSetTextFieldColor(NATextField* textField, const NAABYColor* color);

// Get the desired value directly.
NA_API double naGetTextFieldDouble(NATextField* textField);

// Enable or disable the textfield.
NA_API void naSetTextFieldEnabled(NATextField* textField, NABool enabled);

// Sets different kind of font properties of the textBox.
NA_API void naSetTextFieldFont(
  NATextField* textField,
  NAFont* font);
NA_API void naSetTextFieldTextAlignment(
  NATextField* textField,
  NATextAlignment alignment);



#endif // NA_UI_TEXTFIELD_INCLUDED



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
