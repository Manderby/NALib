
// This file contains inline implementations of testing methods.

#if NA_TESTING_ENABLED == 1

#include <string.h>



// First, undefine all macros defined in the .h file
#undef naT
#undef naG
#undef naF
#undef naPrintMacroPlain
#undef naPrintMacroInt
#undef naPrintMacroIntSpecial
#undef naPrintMacroIntSpecialHex
#undef naPrintMacroIntYesNo
#undef naPrintMacroEnum

#undef naPrintMacroux8
#undef naPrintMacroix8
#undef naPrintMacroux16
#undef naPrintMacroix16
#undef naPrintMacroux32
#undef naPrintMacroix32
#undef naPrintMacroux64
#undef naPrintMacroix64
#undef naPrintMacroux128
#undef naPrintMacroix128
#undef naPrintMacroux256
#undef naPrintMacroix256



void naAddTest(const char* expr, int success, int lineNum);
void naStartTestGroup(const char* name, int lineNum);
void naStopTestGroup(void);

#define naT(expr)\
  {\
    NABool success = expr;\
    naAddTest(#expr, success, __LINE__);\
  }

#define naG(string)\
  naStartTestGroup(string, __LINE__);\
  for(int g = 0; g < 1 ; g++, naStopTestGroup())

#define naF(identifier)\
  {\
  naStartTestGroup(#identifier, __LINE__);\
  test ## identifier();\
  naStopTestGroup();\
  }



#define NA_TEST_MAX_MACRO_LENGTH 40

#define naPrintMacroHead(macro)\
{\
  const char* __macroStr = macro;\
  printf("%s", __macroStr);\
  for(int c = 0; c < (int)(NA_TEST_MAX_MACRO_LENGTH - strlen(__macroStr)); c++){\
    printf(" ");\
  }\
}

#define naPrintMacroEnumCore(macro, strings, maxValue)\
  if((int32)macro >= (int32)maxValue){\
    printf("%d Invalid value", macro);\
  }else{\
    printf("%d (%s)", macro, strings[macro]);\
  }\



#define naPrintMacroPlain(macro)\
  naPrintMacroHead(#macro);\
  if(strncmp(#macro, NA_STRINGIFY(macro), strlen(#macro)) != 0){\
    if(strlen(NA_STRINGIFY(macro)) == 0){\
      printf("(Defined)");\
    }else{\
      printf(NA_STRINGIFY(macro));\
    }\
  }else{\
    printf("(Undefined)");\
  }\
  printf(NA_NL)



#define naPrintMacroInt(macro)\
  naPrintMacroHead(#macro);\
  printf("%d", (int32)macro);\
  printf(NA_NL)



#define naPrintMacroIntSpecial(macro, specialValue, specialString)\
  naPrintMacroHead(#macro);\
  if((int32)macro == (int32)specialValue){\
    printf("%d (%s)", macro, specialString);\
  }else{\
    printf("%d", macro);\
  }\
  printf(NA_NL)



#define naPrintMacroIntSpecialHex(macro, specialValue, specialString)\
  naPrintMacroHead(#macro);\
  if((int32)macro == (int32)specialValue){\
    printf("0x%x (%s)", macro, specialString);\
  }else{\
    printf("0x%x", macro);\
  }\
  printf(NA_NL)



extern const char* na_yesno_strings[];

#define naPrintMacroIntYesNo(macro)\
  naPrintMacroHead(#macro);\
  naPrintMacroEnumCore(macro, na_yesno_strings, 2)\
  printf(NA_NL)



#define naPrintMacroEnum(macro, strings, maxValue)\
  naPrintMacroHead(#macro);\
  naPrintMacroEnumCore(macro, strings, maxValue)\
  printf(NA_NL)



#define naPrintMacroux8(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriux8(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriux8(macro));\
  printf(NA_NL)
#define naPrintMacroix8(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriix8(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriix8(macro));\
  printf(NA_NL)

#define naPrintMacroux16(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriux16(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriux16(macro));\
  printf(NA_NL)
#define naPrintMacroix16(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriix16(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriix16(macro));\
  printf(NA_NL)

#define naPrintMacroux32(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriux32(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriux32(macro));\
  printf(NA_NL)
#define naPrintMacroix32(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriix32(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriix32(macro));\
  printf(NA_NL)

#define naPrintMacroux64(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriux64(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriux64(macro));\
  printf(NA_NL)
#define naPrintMacroix64(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriix64(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriix64(macro));\
  printf(NA_NL)

#define naPrintMacroux128(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriux128(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriux128(macro));\
  printf(NA_NL);
#define naPrintMacroix128(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriix128(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriix128(macro));\
  printf(NA_NL);

#define naPrintMacroux256(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriux256(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriux256(macro));\
  printf(NA_NL);
#define naPrintMacroix256(macro, showMacro)\
  naPrintMacroHead(#macro);\
  if(showMacro)\
    printf("0x%s | %s", naPriix256(macro), NA_STRINGIFY(macro));\
  else\
    printf("0x%s", naPriix256(macro));\
  printf(NA_NL);


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
