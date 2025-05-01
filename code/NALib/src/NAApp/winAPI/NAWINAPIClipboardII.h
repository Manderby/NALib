
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



void naBeginClipboard() {
  // NA_NULL means: Current task becomes owner instead of a hWnd
  if(!OpenClipboard(NA_NULL))
    return;
  EmptyClipboard();
  // After emtying, the clipboard owner is set to NA_NULL and SetClipboardData
  // will fail. Therefore, we have to cloase and reopen the clipboard again.
  CloseClipboard();
  
  if(!OpenClipboard(NA_NULL))
    return;
}

void naEndClipboard() {
  CloseClipboard();
}



void naPutStringToClipboard(const NAString* string) {
  size_t stringLength = naGetStringByteSize(string);
  wchar_t* unicodeString = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(string));

  HGLOBAL clipboardHandle = GlobalAlloc(GMEM_MOVEABLE, (stringLength + 1) * sizeof(wchar_t));
  if(clipboardHandle) {
    LPTSTR globalMemory = GlobalLock(clipboardHandle);
    if(globalMemory) {
      memcpy(globalMemory, unicodeString, stringLength * sizeof(wchar_t)); 
      globalMemory[stringLength] = (wchar_t) 0;    // null character 
      GlobalUnlock(clipboardHandle);
      SetClipboardData(CF_UNICODETEXT, clipboardHandle);
    }
  }

  naFree(unicodeString);
}



NAString* naNewStringFromClipboard() {
  NAString* string = NA_NULL;
  // NA_NULL means: Current task becomes owner instead of a hWnd
  if(!OpenClipboard(NA_NULL))
    return;

  HGLOBAL clipboardHandle = GetClipboardData(CF_UNICODETEXT);
  if(clipboardHandle) {
    LPTSTR globalMemory = GlobalLock(clipboardHandle);
    if(globalMemory) {
      size_t stringLength = wcslen(globalMemory);
      wchar_t* unicodeString = naMalloc(stringLength * sizeof(wchar_t));
      memcpy(unicodeString, globalMemory, stringLength * sizeof(wchar_t)); 
      unicodeString[stringLength] = (wchar_t) 0;    // null character 
      GlobalUnlock(clipboardHandle);
      string = naNewStringWithWideCharString(unicodeString);
    }
  }

  CloseClipboard();
  return string;
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
