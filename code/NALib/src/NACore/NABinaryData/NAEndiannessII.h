
// This file contains inline implementations of the file NABinaryData.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryDataII.h"


#include <stddef.h>


NA_IDEF  void naConvertEndianness8(NAInt endianness, void* buffer){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig8(buffer);}
}
NA_IDEF  void naConvertEndianness16(NAInt endianness, void* buffer){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig16(buffer);}
}
NA_IDEF  void naConvertEndianness32(NAInt endianness, void* buffer){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig32(buffer);}
}
NA_IDEF  void naConvertEndianness64(NAInt endianness, void* buffer){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig64(buffer);}
}
NA_IDEF  void naConvertEndianness128(NAInt endianness, void* buffer){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig128(buffer);}
}
NA_IDEF  void naConvertEndianness8v(NAInt endianness, void* buffer, size_t count){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig8v(buffer, count);}
}
NA_IDEF  void naConvertEndianness16v(NAInt endianness, void* buffer, size_t count){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig16v(buffer, count);}
}
NA_IDEF  void naConvertEndianness32v(NAInt endianness, void* buffer, size_t count){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig32v(buffer, count);}
}
NA_IDEF  void naConvertEndianness64v(NAInt endianness, void* buffer, size_t count){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig64v(buffer, count);}
}
NA_IDEF  void naConvertEndianness128v(NAInt endianness, void* buffer, size_t count){
  if(endianness != NA_ENDIANNESS_HOST){naConvertLittleBig128v(buffer, count);}
}


// //////////////////////////////////////////////////////////////////////
// Conversions Little to Big and vice versa. Single converters
// //////////////////////////////////////////////////////////////////////

NA_IDEF void naConvertLittleBig8(void* buffer){
  #if NA_DEBUG
    if(!buffer)
      naError("Buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
}

NA_IDEF void naConvertLittleBig16(void* buffer){
  #if NA_DEBUG
    if(!buffer)
      naCrash("Buffer is Null-Pointer");
  #endif
  naSwap8(((NAByte*)buffer)+1, ((NAByte*)buffer)+0);
}

NA_IDEF void naConvertLittleBig32(void* buffer){
  #if NA_DEBUG
    if(!buffer)
      naCrash("Buffer is Null-Pointer");
  #endif
  naSwap8(((NAByte*)buffer)+3, ((NAByte*)buffer)+0);
  naSwap8(((NAByte*)buffer)+2, ((NAByte*)buffer)+1);
}

NA_IDEF void naConvertLittleBig64(void* buffer){
//  #if !defined NA_TYPE_INT64
//    uint32* hi;
//    uint32* lo;
//  #endif
  #if NA_DEBUG
    if(!buffer)
      naCrash("Buffer is Null-Pointer");
  #endif
//    naSwap8((&((NAByte*)buffer)[7]), (&((NAByte*)buffer)[0]));
//    naSwap8((&((NAByte*)buffer)[6]), (&((NAByte*)buffer)[1]));
//    naSwap8((&((NAByte*)buffer)[5]), (&((NAByte*)buffer)[2]));
//    naSwap8((&((NAByte*)buffer)[4]), (&((NAByte*)buffer)[3]));
//  #if defined NA_TYPE_INT64
    naSwap8(((NAByte*)buffer)+7, ((NAByte*)buffer)+0);
    naSwap8(((NAByte*)buffer)+6, ((NAByte*)buffer)+1);
    naSwap8(((NAByte*)buffer)+5, ((NAByte*)buffer)+2);
    naSwap8(((NAByte*)buffer)+4, ((NAByte*)buffer)+3);
//  #else
//    hi = &(((NAu64*)buffer)->hi);
//    lo = &(((NAu64*)buffer)->lo);
//    naSwap8(((NAByte*)hi)+3, ((NAByte*)lo)+0);
//    naSwap8(((NAByte*)hi)+2, ((NAByte*)lo)+1);
//    naSwap8(((NAByte*)hi)+1, ((NAByte*)lo)+2);
//    naSwap8(((NAByte*)hi)+0, ((NAByte*)lo)+3);
//  #endif
}

NA_IDEF void naConvertLittleBig128(void* buffer){
  #if NA_DEBUG
    if(!buffer)
      naCrash("Buffer is Null-Pointer");
  #endif
  naSwap8(((NAByte*)buffer)+15, ((NAByte*)buffer)+0);
  naSwap8(((NAByte*)buffer)+14, ((NAByte*)buffer)+1);
  naSwap8(((NAByte*)buffer)+13, ((NAByte*)buffer)+2);
  naSwap8(((NAByte*)buffer)+12, ((NAByte*)buffer)+3);
  naSwap8(((NAByte*)buffer)+11, ((NAByte*)buffer)+4);
  naSwap8(((NAByte*)buffer)+10, ((NAByte*)buffer)+5);
  naSwap8(((NAByte*)buffer)+ 9, ((NAByte*)buffer)+6);
  naSwap8(((NAByte*)buffer)+ 8, ((NAByte*)buffer)+7);
}

// Little sidemark: The arguments of the naSwap8 call are ordered such that
// the second argument denotes a lower memory position. This is because
// usually, compilers evaluate arguments from right to left and hence access
// the lower memory position first which in turn causes the cache to be filled
// properly.



// //////////////////////////////
// Conversions Little to Big and vice versa. Array converters
// //////////////////////////////

NA_IDEF void naConvertLittleBig8v(void* buffer, size_t count){
  #if NA_DEBUG
    if(!buffer)
      naError("buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
  NA_UNUSED(count);
}

NA_IDEF void naConvertLittleBig16v(void* buffer, size_t count){
  #if NA_DEBUG
    if(!buffer)
      naError("buffer is Null-Pointer");
  #endif
  for(size_t i = 0; i < count; i++){
    naConvertLittleBig16(buffer);
    buffer = (NAByte*)buffer + 2;
  }
}

NA_IDEF void naConvertLittleBig32v(void* buffer, size_t count){
  #if NA_DEBUG
    if(!buffer)
      naError("buffer is Null-Pointer");
  #endif
  for(size_t i = 0; i < count; i++){
    naConvertLittleBig32(buffer);
    buffer = (NAByte*)buffer + 4;
  }
}

NA_IDEF void naConvertLittleBig64v(void* buffer, size_t count){
  #if NA_DEBUG
    if(!buffer)
      naError("buffer is Null-Pointer");
  #endif
  for(size_t i = 0; i < count; i++){
    naConvertLittleBig64(buffer);
    buffer = (NAByte*)buffer + 8;
  }
}

NA_IDEF void naConvertLittleBig128v(void* buffer, size_t count){
  #if NA_DEBUG
    if(!buffer)
      naError("buffer is Null-Pointer");
  #endif
  for(size_t i = 0; i < count; i++){
    naConvertLittleBig128(buffer);
    buffer = (NAByte*)buffer + 16;
  }
}



NA_IAPI NABool naIsEndiannessHost(NAInt endianness){
  #if NA_DEBUG
    if((endianness != 1) && (endianness != 2))
      naError("endianness is invalid. Result is undefined.");
  #endif
  return (endianness == NA_ENDIANNESS_HOST);
}



#define NA_NATIVE_ENDIANNESS_MARKER     0x42446462
#if NA_ENDIANNESS_HOST == NA_ENDIANNESS_LITTLE
  #define NA_LITTLE_ENDIANNESS_MARKER   0x42446462
  #define NA_BIG_ENDIANNESS_MARKER      0x62644442
#elif NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
  #define NA_LITTLE_ENDIANNESS_MARKER   0x62644442
  #define NA_BIG_ENDIANNESS_MARKER      0x42446462
#endif


NA_IDEF void naFillEndiannessMarker(NAByte marker[4], NAInt endianness){
  switch(endianness){
  case NA_ENDIANNESS_LITTLE:
    (*((uint32*)marker)) = NA_LITTLE_ENDIANNESS_MARKER;
    break;
  case NA_ENDIANNESS_BIG:
    (*((uint32*)marker)) = NA_BIG_ENDIANNESS_MARKER;
    break;
  default:
    (*((uint32*)marker)) = NA_NATIVE_ENDIANNESS_MARKER;
    break;
  }
}

NA_IDEF NAInt naParseEndiannessMarker(const NAByte marker[4]){
  switch(*((uint32*)marker)){
  case NA_LITTLE_ENDIANNESS_MARKER:
    return NA_ENDIANNESS_LITTLE;
    break;
  case NA_BIG_ENDIANNESS_MARKER:
    return NA_ENDIANNESS_BIG;
    break;
  default:
    #if NA_DEBUG
      naError("Endianness unknown. Returning Little");
    #endif
    return NA_ENDIANNESS_LITTLE;
    break;
  }
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
