//
//  NAStringII.h
//  NALib
//
//  Created by Tobias Stamm on 22.01.17.
//
//

#ifndef NAStringII_h
#define NAStringII_h


#include <stdio.h>



NA_IDEF NAUInt naStrlen(const NAUTF8Char* str){
  return (NAUInt)strlen((const char*)str);
}



NA_IDEF NAUInt naVsnprintf(NAUTF8Char* buffer, NAUInt length, const NAUTF8Char* newstr, va_list argumentlist){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAInt)_vsnprintf_s(buffer, (size_t)length, (size_t)length, newstr, argumentlist);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAInt)vsnprintf((char*)buffer, (size_t)length, (const char*)newstr, argumentlist);
  #endif
}



// Returns the number of characters needed to transform the given string and
// arguments using sprintf.
NA_IDEF NAUInt naVarargStringLength(const NAUTF8Char* string, va_list args){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAInt)_vscprintf(string, args);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAInt)naVsnprintf(NA_NULL, 0, string, args);
  #endif
}






#endif /* NAStringII_h */
