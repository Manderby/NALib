
// This file contains inline implementations of testing methods.

// First, undefine all macros defined in the .h file
#undef naT
#undef naE
#undef naG
#undef naF



#if NA_TESTING_ENABLED == 1

#include <string.h>

extern NABool na_test_case_running;
extern int na_error_count;

NA_HAPI void naStartTestCase();
NA_HAPI void naStopTestCase();

NA_HAPI void naAddTest(const char* expr, int success, int lineNum);
NA_HAPI void naAddTestError(const char* expr, int lineNum);
NA_HAPI void naStartTestGroup(const char* name, int lineNum);
NA_HAPI void naStopTestGroup(void);

#define naT(expr)\
  {\
    naStartTestCase();\
    NABool success = expr;\
    naStopTestCase();\
    naAddTest(#expr, success, __LINE__);\
  }

// Testing for errors is only useful when NDEBUG is undefined.
#ifndef NDEBUG
  #define naE(expr)\
    {\
      naStartTestCase();\
      { expr; }\
      naStopTestCase();\
      naAddTestError(#expr, __LINE__);\
    }
#else
  #define naE(expr)
#endif

#define naG(string)\
  naStartTestGroup(string, __LINE__);\
  for(int g = 0; g < 1 ; g++, naStopTestGroup())

#define naF(identifier)\
  {\
  naStartTestGroup(#identifier, __LINE__);\
  test ## identifier();\
  naStopTestGroup();\
  }


#else // NA_TESTING_ENABLED == 1

#define naT(expr)\
  NA_UNUSED(expr)
#define naE(expr)\
  NA_UNUSED(expr)
#define naG(string)\
  NA_UNUSED(string);\
  while(0)
#define naF(identifier)

#endif // NA_TESTING_ENABLED == 1



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
