
#if defined NA_DEBUGGING_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_DEBUGGING_II_INCLUDED
#define NA_DEBUGGING_II_INCLUDED



// This file contains inline implementations of debugging methods.

#ifndef NDEBUG

  #include <stdlib.h>   // for the exit function

  // First, undefine the macros from the .h file.
  #undef NA_DEBUG_FUNCTIONSYMBOL
  #undef naError
  #undef naCrash

  #if NA_OS == NA_OS_WINDOWS
    #define NA_DEBUG_FUNCTIONSYMBOL __FUNCTION__
  #else
    #define NA_DEBUG_FUNCTIONSYMBOL __func__
  #endif

  NA_HHAPI void na_Error(const char* functionSymbol, const char* text);
  NA_HHAPI void na_Crash(const char* functionSymbol, const char* text);

  #define naError(text)\
    na_Error(NA_DEBUG_FUNCTIONSYMBOL, text)

  // Note for the future: The exit call must be in the macro and must not be
  // moved to the capture function!
  #define naCrash(text)\
    {\
      na_Crash(NA_DEBUG_FUNCTIONSYMBOL, text);\
      exit(EXIT_FAILURE);\
    }

#endif // NDEBUG



#endif // NA_DEBUGGING_II_INCLUDED



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
