
#ifndef NA_CLIPBOARD_INCLUDED
#define NA_CLIPBOARD_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



NA_PROTOTYPE(NAString);
NA_PROTOTYPE(NABuffer);



// You can put multiple objects onto the clipboard, denoting different
// representations of the currently copied content.
//
// To put objects onto the clipboard, you need to encapsulate the calls into
// the following begin and end calls. Begin will empty the current clipboard.
// Do not forget the end call!
void naBeginClipboard();
void naEndClipboard();

// Puts the given string on the systems clipboard in a standard string format.
NA_API void naPutStringToClipboard(const NAString* string);
// Puts the given buffer on the systems clipboard as raw data.
NA_API void naPutBufferToClipboard(const NABuffer* buffer);



// Returns a new string containing what has been stored as as standard string
// in the systems clipboard.
NA_API NAString* naNewStringFromClipboard(void);
// Creates a new buffer containing what has been stored as NABuffer in the
// systems clipboard.
NA_API NABuffer* naCreateBufferFromClipboard(void);


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_CLIPBOARD_INCLUDED



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
