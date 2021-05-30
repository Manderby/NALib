
#include "../NABase.h"

#if NA_PRINTOUT_ENABLED == 1

#include "../NAString.h"

#include <stdio.h>  // for the printf function

const char* na_YesNoStrings[] = {
  "No",
  "Yes",
};



#define NA_TEST_MAX_MACRO_LENGTH 40

NA_HDEF void na_PrintMacroHead(const NAUTF8Char* macroString){
  printf("%s", macroString);
  printf(" ");  // Write at least one space
  for(int c = 0; c < (int)(NA_TEST_MAX_MACRO_LENGTH - strlen(macroString) - 1); c++){
    printf(" ");
  }
}



NA_HDEF void na_PrintMacroEnumBase(int macroValue, const NAUTF8Char** strings, int enumCount){
  if(macroValue >= enumCount){
    printf("%d Invalid value", macroValue);
  }else{
    printf("%d (%s)", macroValue, strings[macroValue]);
  }
}



NA_HDEF void na_PrintMacroPlain(const NAUTF8Char* macroString, const NAUTF8Char* macroStringified, NABool printContent){
  na_PrintMacroHead(macroString);
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



NA_HDEF void na_PrintMacroInt(const NAUTF8Char* macroString, int value){
  na_PrintMacroHead(macroString);
  printf("%d", value);
  printf(NA_NL);
}



NA_HDEF void na_PrintMacroIntSpecial(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString){
  na_PrintMacroHead(macroString);
  if(value == specialValue){
    printf("%d (%s)", value, specialString);
  }else{
    printf("%d", value);
  }
  printf(NA_NL);
}



NA_HDEF void na_PrintMacroIntSpecialHex(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString){
  na_PrintMacroHead(macroString);
  if(value == specialValue){
    printf("0x%x (%s)", value, specialString);
  }else{
    printf("0x%x", value);
  }
  printf(NA_NL);
}



NA_HDEF void na_PrintMacroIntYesNo(const NAUTF8Char* macroString, int value){
  na_PrintMacroHead(macroString);
  na_PrintMacroEnumBase(value, na_YesNoStrings, 2);
  printf(NA_NL);
}



NA_HDEF void na_PrintMacroEnum(const NAUTF8Char* macroString, int value, const NAUTF8Char** strings, int enumCount){
  na_PrintMacroHead(macroString);
  na_PrintMacroEnumBase(value, strings, enumCount);
  printf(NA_NL);
}



NA_HDEF void na_PrintMacroux8(const NAUTF8Char* macroString, uint8 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriux8(value));
  printf(NA_NL);
}
NA_HDEF void na_PrintMacroix8(const NAUTF8Char* macroString, int8 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriix8(value));
  printf(NA_NL);
}

NA_HDEF void na_PrintMacroux16(const NAUTF8Char* macroString, uint16 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriux16(value));
  printf(NA_NL);
}
NA_HDEF void na_PrintMacroix16(const NAUTF8Char* macroString, int16 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriix16(value));
  printf(NA_NL);
}

NA_HDEF void na_PrintMacroux32(const NAUTF8Char* macroString, uint32 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriux32(value));
  printf(NA_NL);
}
NA_HDEF void na_PrintMacroix32(const NAUTF8Char* macroString, int32 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriix32(value));
  printf(NA_NL);
}

NA_HDEF void na_PrintMacroux64(const NAUTF8Char* macroString, uint64 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriux64(value));
  printf(NA_NL);
}
NA_HDEF void na_PrintMacroix64(const NAUTF8Char* macroString, int64 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriix64(value));
  printf(NA_NL);
}

NA_HDEF void na_PrintMacroux128(const NAUTF8Char* macroString, uint128 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriux128(value));
  printf(NA_NL);
}
NA_HDEF void na_PrintMacroix128(const NAUTF8Char* macroString, int128 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriix128(value));
  printf(NA_NL);
}

NA_HDEF void na_PrintMacroux256(const NAUTF8Char* macroString, uint256 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naPriux256(value));
  printf(NA_NL);
}
NA_HDEF void na_PrintMacroix256(const NAUTF8Char* macroString, int256 value){
  na_PrintMacroHead(macroString);
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
