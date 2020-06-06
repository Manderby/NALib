
#include "../NABase.h"

#if NA_PRINTOUT_ENABLED == 1

#include "../NAString.h"

#include <stdio.h>  // for the printf function

const char* na_YesNoStrings[] = {
  "No",
  "Yes",
};



#define NA_TEST_MAX_MACRO_LENGTH 40

NA_HDEF void naPrintMacroHead(const NAUTF8Char* macroString){
  printf("%s", macroString);
  printf(" ");  // Write at least one space
  for(int c = 0; c < (int)(NA_TEST_MAX_MACRO_LENGTH - strlen(macroString) - 1); c++){
    printf(" ");
  }
}



NA_HDEF void naPrintMacroEnumBase(int macroValue, const NAUTF8Char** strings, int enumCount){
  if(macroValue >= enumCount){
    printf("%d Invalid value", macroValue);
  }else{
    printf("%d (%s)", macroValue, strings[macroValue]);
  }
}



NA_HDEF void naPrintMacroPlainCore(const NAUTF8Char* macroString, const NAUTF8Char* macroStringified, NABool printContent){
  naPrintMacroHead(macroString);
  if(strncmp(macroString, macroStringified, strlen(macroString)) != 0){
    if(strlen(macroStringified) == 0){
      printf("(Defined)");
    }else{
      if(printContent)
        printf("%s", macroStringified);
      else
        printf("(Defined with content)");
    }
  }else{
    printf("(Undefined)");
  }
  printf(NA_NL);
}



NA_HDEF void naPrintMacroIntCore(const NAUTF8Char* macroString, int value){
  naPrintMacroHead(macroString);
  printf("%d", value);
  printf(NA_NL);
}



NA_HDEF void naPrintMacroIntSpecialCore(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString){
  naPrintMacroHead(macroString);
  if(value == specialValue){
    printf("%d (%s)", value, specialString);
  }else{
    printf("%d", value);
  }
  printf(NA_NL);
}



NA_HDEF void naPrintMacroIntSpecialHexCore(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString){
  naPrintMacroHead(macroString);
  if(value == specialValue){
    printf("0x%x (%s)", value, specialString);
  }else{
    printf("0x%x", value);
  }
  printf(NA_NL);
}



NA_HDEF void naPrintMacroIntYesNoCore(const NAUTF8Char* macroString, int value){
  naPrintMacroHead(macroString);
  naPrintMacroEnumBase(value, na_YesNoStrings, 2);
  printf(NA_NL);
}



NA_HDEF void naPrintMacroEnumCore(const NAUTF8Char* macroString, int value, const NAUTF8Char** strings, int enumCount){
  naPrintMacroHead(macroString);
  naPrintMacroEnumBase(value, strings, enumCount);
  printf(NA_NL);
}



NA_HDEF void naPrintMacroux8Core(const NAUTF8Char* macroString, uint8 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriux8(value));
  printf(NA_NL);
}
NA_HDEF void naPrintMacroix8Core(const NAUTF8Char* macroString, int8 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriix8(value));
  printf(NA_NL);
}

NA_HDEF void naPrintMacroux16Core(const NAUTF8Char* macroString, uint16 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriux16(value));
  printf(NA_NL);
}
NA_HDEF void naPrintMacroix16Core(const NAUTF8Char* macroString, int16 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriix16(value));
  printf(NA_NL);
}

NA_HDEF void naPrintMacroux32Core(const NAUTF8Char* macroString, uint32 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriux32(value));
  printf(NA_NL);
}
NA_HDEF void naPrintMacroix32Core(const NAUTF8Char* macroString, int32 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriix32(value));
  printf(NA_NL);
}

NA_HDEF void naPrintMacroux64Core(const NAUTF8Char* macroString, uint64 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriux64(value));
  printf(NA_NL);
}
NA_HDEF void naPrintMacroix64Core(const NAUTF8Char* macroString, int64 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriix64(value));
  printf(NA_NL);
}

NA_HDEF void naPrintMacroux128Core(const NAUTF8Char* macroString, uint128 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriux128(value));
  printf(NA_NL);
}
NA_HDEF void naPrintMacroix128Core(const NAUTF8Char* macroString, int128 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriix128(value));
  printf(NA_NL);
}

NA_HDEF void naPrintMacroux256Core(const NAUTF8Char* macroString, uint256 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriux256(value));
  printf(NA_NL);
}
NA_HDEF void naPrintMacroix256Core(const NAUTF8Char* macroString, int256 value){
  naPrintMacroHead(macroString);
  printf("0x%s", naPriix256(value));
  printf(NA_NL);
}



#endif // NA_PRINTOUT_ENABLED == 1



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
