
#ifndef NA_UI_TEXTBOX_INCLUDED
#define NA_UI_TEXTBOX_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// An NATextBox can react to the following commands:
//
// NA_UI_COMMAND_EDITED        When the content changes



// Creates a new TextBox.
NA_API NATextBox* naNewTextBox(NASize size);

// Changes the visibility or editable state of the textBox.
NA_API void naSetTextBoxVisible(NATextBox* textBox, NABool visible);
NA_API void naSetTextBoxEditable(NATextBox* textBox, NABool editable);

// Get or set the text of the textBox.
NA_API NAString* naNewStringWithTextBoxText(const NATextBox* textBox);
NA_API void naSetTextBoxText(NATextBox* textBox, const NAUTF8Char* text);

// Sets different kind of font properties of the textBox.
NA_API void naSetTextBoxTextAlignment(
  NATextBox* textBox,
  NATextAlignment alignment);
NA_API void naSetTextBoxFont(
  NATextBox* textBox,
  NAFont* font);

// Sets if the textBox allows horizontal or vertical scrolling
NA_API void naSetTextBoxUseHorizontalScrolling(NATextBox* textBox);
NA_API void naSetTextBoxUseVerticalScrolling(NATextBox* textBox, NABool use);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_TEXTBOX_INCLUDED



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
