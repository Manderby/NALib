
#ifndef NA_TESTING_II_INCLUDED
#define NA_TESTING_II_INCLUDED



// This file contains inline implementations of testing methods.

// First, undefine all macros defined in the .h file
#undef naTest
#undef naTestVoid
#undef naTestError
#undef naTestCrash
#undef naTestGroup
#undef naTestGroupFunction
#undef naUntested
#undef naBenchmark
#undef naTestIn



#if NA_TESTING_ENABLED == 1

NA_HAPI void   na_AddTest(const char* expr, int success, int lineNum);
NA_HAPI void   na_AddTestError(const char* expr, int lineNum);
NA_HAPI void   na_AddTestCrash(const char* expr, int lineNum);
NA_HAPI void   na_ExecuteCrashProcess(const char* expr, int lineNum);
NA_HAPI NABool na_StartTestGroup(const char* name, int lineNum);
NA_HAPI void   na_StopTestGroup(void);
NA_HAPI void   na_RegisterUntested(const char* text);
NA_HAPI NABool na_GetTestCaseRunning(void);
NA_HAPI void   na_SetTestCaseRunning(NABool running);
NA_HAPI void   na_IncErrorCount(void);
NA_HAPI void   na_ResetErrorCount(void);
NA_HAPI int    na_GetErrorCount(void);
NA_HDEF NABool na_LetCrashTestCrash(void);
NA_HAPI NABool na_ShallExecuteGroup(const char* name);

NA_HAPI uint32 na_GetBenchmarkIn(void);
NA_HAPI double na_BenchmarkTime(void);
NA_HAPI double na_GetBenchmarkLimit(void);
NA_HAPI size_t na_GetBenchmarkTestSizeLimit(void);
NA_HAPI void   na_PrintBenchmark(double timeDiff, size_t testSize, const char* exprString, int lineNum);
NA_HAPI void   na_StoreBenchmarkResult(char);



#if NA_DEBUG
  #define NA_START_TEST_CASE\
    if(na_GetTestCaseRunning())\
      naError("A test case is already running. This might lead to bad test results.");\
    na_SetTestCaseRunning(NA_TRUE);\
    na_ResetErrorCount();
#else
  #define NA_START_TEST_CASE\
    na_SetTestCaseRunning(NA_TRUE);\
    na_ResetErrorCount();
#endif


#define NA_STOP_TEST_CASE\
  na_SetTestCaseRunning(NA_FALSE);



#define naTest(expr)\
  if(na_ShallExecuteGroup(#expr)){\
    NA_START_TEST_CASE\
    NABool success = expr;\
    NA_STOP_TEST_CASE\
    na_AddTest(#expr, success, __LINE__);\
  }

#define naTestVoid(expr)\
  if(na_ShallExecuteGroup(#expr)){\
    NA_START_TEST_CASE\
    expr;\
    NA_STOP_TEST_CASE\
    na_AddTest(#expr, NA_TRUE, __LINE__);\
  }

// Testing for errors and crashes is only useful when NA_DEBUG is 1.
#if NA_DEBUG
  #define naTestError(expr)\
    if(na_ShallExecuteGroup(#expr)){\
      NA_START_TEST_CASE\
      { expr; }\
      NA_STOP_TEST_CASE\
      na_AddTestError(#expr, __LINE__);\
    }

  #define naTestCrash(expr)\
    if(na_ShallExecuteGroup(#expr)){\
      if(na_LetCrashTestCrash()){\
        NA_START_TEST_CASE\
        { expr; }\
        NA_STOP_TEST_CASE\
        na_AddTestCrash(#expr, __LINE__);\
      }else{\
        na_ExecuteCrashProcess(#expr, __LINE__);\
      }\
    }
#else
  #define naTestError(expr)
  #define naTestVoidError(expr)
  #define naTestCrash(expr)
#endif

#define naTestGroup(string)\
  for(int g = 1 - na_StartTestGroup(string, __LINE__); g < 1 ; g++, na_StopTestGroup())

#define naTestGroupFunction(identifier)\
  {\
  if(na_StartTestGroup(#identifier, __LINE__)){\
    test ## identifier();\
    na_StopTestGroup();\
  }\
  }

#define naUntested(text)\
  na_RegisterUntested(#text);

#define naBenchmark(expr)\
{\
  int testSize = 1;\
  double timeDiff = 0;\
  size_t pow;\
  double startT = na_BenchmarkTime();\
  double endT;\
  for(pow = 0; pow < na_GetBenchmarkTestSizeLimit(); pow++){\
    for(int testRun = 0; testRun < testSize; testRun++){\
      /*na_StoreBenchmarkResult((char)(expr));*/\
      {\
        (void)expr; (void)0;\
      }\
    }\
    endT = na_BenchmarkTime();\
    timeDiff = endT - startT;\
    if(timeDiff < 0.){timeDiff = 0; break;}\
    if(timeDiff > na_GetBenchmarkLimit()){break;}\
    testSize <<= 1;\
  }\
  na_PrintBenchmark(timeDiff, testSize * 2, #expr, __LINE__);\
}

#define naTestIn\
  na_GetBenchmarkIn()



#else

#define naTest(expr)\
  NA_UNUSED(expr)
#define naTestVoid(expr)\
  NA_UNUSED(expr)
#define naTestError(expr)\
  NA_UNUSED(expr)
#define naTestCrash(expr)\
  NA_UNUSED(expr)
#define naTestGroup(string)\
  NA_UNUSED(string);\
  for(int g = 0; g < 1 ; g++)
#define naTestGroupFunction(identifier)
#define naUntested(text)
#define naBenchmark(expr)
#define naTestIn 0

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
