
#ifndef NA_TESTING_II_INCLUDED
#define NA_TESTING_II_INCLUDED



// This file contains inline implementations of testing methods.

// First, undefine all macros defined in the .h file
#undef naTest
#undef naTestError
#undef naTestGroup
#undef naTestGroupFunction
#undef naUntested



#if NA_TESTING_ENABLED == 1

NA_HAPI void   naAddTest(const char* expr, int success, int lineNum);
NA_HAPI void   naAddTestError(const char* expr, int lineNum);
NA_HAPI void   naStartTestGroup(const char* name, int lineNum);
NA_HAPI void   naStopTestGroup(void);
NA_HAPI void   naRegisterUntested(const char* text);
NA_HAPI NABool naGetTestCaseRunning(void);
NA_HAPI void   naSetTestCaseRunning(NABool running);
NA_HAPI void   naIncErrorCount(void);
NA_HAPI void   naResetErrorCount(void);
NA_HAPI int    naGetErrorCount(void);



#ifndef NDEBUG
  #define NA_START_TEST_CASE\
    if(naGetTestCaseRunning())\
      naError("A test case is already running. This might lead to bad test results.");\
    naSetTestCaseRunning(NA_TRUE);\
    naResetErrorCount();
#else
  #define NA_START_TEST_CASE\
    naSetTestCaseRunning(NA_TRUE);\
    naResetErrorCount();
#endif


#define NA_STOP_TEST_CASE\
  naSetTestCaseRunning(NA_FALSE);



#define naTest(expr)\
  {\
    NA_START_TEST_CASE\
    NABool success = expr;\
    NA_STOP_TEST_CASE\
    naAddTest(#expr, success, __LINE__);\
  }

// Testing for errors is only useful when NDEBUG is undefined.
#ifndef NDEBUG
  #define naTestError(expr)\
    {\
      NA_START_TEST_CASE\
      { expr; }\
      NA_STOP_TEST_CASE\
      naAddTestError(#expr, __LINE__);\
    }
#else
  #define naTestError(expr)
#endif

#define naTestGroup(string)\
  naStartTestGroup(string, __LINE__);\
  for(int g = 0; g < 1 ; g++, naStopTestGroup())

#define naTestGroupFunction(identifier)\
  {\
  naStartTestGroup(#identifier, __LINE__);\
  test ## identifier();\
  naStopTestGroup();\
  }

#define naUntested(text)\
  naRegisterUntested(#text);


#else

#define naTest(expr)\
  NA_UNUSED(expr)
#define naTestError(expr)\
  NA_UNUSED(expr)
#define naTestGroup(string)\
  NA_UNUSED(string);\
  for(int g = 0; g < 1 ; g++)
#define naTestGroupFunction(identifier)
#define naUntested(text)

#endif // NA_TESTING_ENABLED == 1



#endif // NA_TESTING_II_INCLUDED



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
