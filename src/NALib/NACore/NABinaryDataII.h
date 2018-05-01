
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NABinaryData.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryData.h"



#include <string.h>


NA_IDEF void naCopy8  (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 1)
      naError("naCopy8", "Restrict pointers overlap.");
    if(!d){
      naCrash("naCopy8", "Pointer d is Null-Pointer.");
      return;
    }
    if(!s){
      naCrash("naCopy8", "Pointer s is Null-Pointer.");
      return;
    }
  #endif
  *(uint8*)d = *(uint8*)s;
}
NA_IDEF void naCopy16 (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 2)
      naError("naCopy16", "Restrict pointers overlap.");
    if(!d){
      naCrash("naCopy16", "Pointer d is Null-Pointer.");
      return;
    }
    if(!s){
      naCrash("naCopy16", "Pointer s is Null-Pointer.");
      return;
    }
  #endif
  *(uint16*)d = *(uint16*)s;
}
NA_IDEF void naCopy32 (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 4)
      naError("naCopy32", "Restrict pointers overlap.");
    if(!d){
      naCrash("naCopy32", "Pointer d is Null-Pointer.");
      return;
    }
    if(!s){
      naCrash("naCopy32", "Pointer s is Null-Pointer.");
      return;
    }
  #endif
  *(uint32*)d = *(uint32*)s;
}
NA_IDEF void naCopy64 (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 8)
      naError("naCopy64", "Restrict pointers overlap.");
    if(!d){
      naCrash("naCopy64", "Pointer d is Null-Pointer.");
      return;
    }
    if(!s){
      naCrash("naCopy64", "Pointer s is Null-Pointer.");
      return;
    }
  #endif
  #if defined NA_TYPE_INT64
    *(uint64*)d = *(uint64*)s;
  #else
    *(uint32*)d = *(uint32*)s;
    d = ((NAByte*)d) + 4;
    s = ((NAByte*)s) + 4;
    *(uint32*)d = *(uint32*)s;
  #endif
}
NA_IDEF void naCopy128(void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 16)
      naError("naCopy128", "Restrict pointers overlap.");
    if(!d){
      naCrash("naCopy128", "Pointer d is Null-Pointer.");
      return;
    }
    if(!s){
      naCrash("naCopy128", "Pointer s is Null-Pointer.");
      return;
    }
  #endif
  #if defined NA_TYPE_INT64
    *(uint64*)d = *(uint64*)s;
    d = ((NAByte*)d) + 8;
    s = ((NAByte*)s) + 8;
    *(uint64*)d = *(uint64*)s;
  #else
    *(uint32*)d = *(uint32*)s;
    d = ((NAByte*)d) + 4;
    s = ((NAByte*)s) + 4;
    *(uint32*)d = *(uint32*)s;
    d = ((NAByte*)d) + 4;
    s = ((NAByte*)s) + 4;
    *(uint32*)d = *(uint32*)s;
    d = ((NAByte*)d) + 4;
    s = ((NAByte*)s) + 4;
    *(uint32*)d = *(uint32*)s;
  #endif
}

NA_IDEF void naCopyn(void* NA_RESTRICT d, const void* NA_RESTRICT s, NAInt bytesize){
  #ifndef NDEBUG
    if(!d){
      naCrash("naCopy8", "Pointer d is Null-Pointer.");
      return;
    }
    if(!s){
      naCrash("naCopy8", "Pointer s is Null-Pointer.");
      return;
    }
    if(bytesize < 1){
      naCrash("naCopyn", "count is smaller than 1.");
      return;
    }
  #endif
  memcpy(d, s, (size_t)bytesize);
}




// /////////////////////////////
// Swap multibyte-values
// /////////////////////////////

NA_IDEF void naSwap8(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 1)
      naError("naSwap8", "Restrict pointers overlap.");
    if(!a){
      naCrash("naSwap8", "Pointer a is Null-Pointer");
      return;
    }
    if(!b){
      naCrash("naSwap8", "Pointer b is Null-Pointer");
      return;
    }
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint8*)a^=*(uint8*)b;
  *(uint8*)b^=*(uint8*)a;
  *(uint8*)a^=*(uint8*)b;
}

NA_IDEF void naSwap16(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 2)
      naError("naSwap16", "Restrict pointers overlap.");
    if(!a){
      naCrash("naSwap16", "Pointer a is Null-Pointer");
      return;
    }
    if(!b){
      naCrash("naSwap16", "Pointer b is Null-Pointer");
      return;
    }
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint16*)a^=*(uint16*)b;
  *(uint16*)b^=*(uint16*)a;
  *(uint16*)a^=*(uint16*)b;
}

NA_IDEF void naSwap32(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 4)
      naError("naSwap32", "Restrict pointers overlap.");
    if(!a){
      naCrash("naSwap32", "Pointer a is Null-Pointer");
      return;
    }
    if(!b){
      naCrash("naSwap32", "Pointer b is Null-Pointer");
      return;
    }
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint32*)a^=*(uint32*)b;
  *(uint32*)b^=*(uint32*)a;
  *(uint32*)a^=*(uint32*)b;
}

NA_IDEF void naSwap64(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 8)
      naError("naSwap64", "Restrict pointers overlap.");
    if(!a){
      naCrash("naSwap64", "Pointer a is Null-Pointer");
      return;
    }
    if(!b){
      naCrash("naSwap64", "Pointer b is Null-Pointer");
      return;
    }
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  #if defined NA_TYPE_INT64
    *(uint64*)a^=*(uint64*)b;
    *(uint64*)b^=*(uint64*)a;
    *(uint64*)a^=*(uint64*)b;
  #else
    naSwap32(a, b);
    a = ((NAByte*)a) + 4;
    b = ((NAByte*)b) + 4;
    naSwap32(a, b);
  #endif
}

NA_IDEF void naSwap128(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 16)
      naError("naSwap128", "Restrict pointers overlap.");
    if(!a){
      naCrash("naSwap128", "Pointer a is Null-Pointer");
      return;
    }
    if(!b){
      naCrash("naSwap128", "Pointer b is Null-Pointer");
      return;
    }
  #endif
  naSwap64(a, b);
  a = ((NAByte*)a) + 8;
  b = ((NAByte*)b) + 8;
  naSwap64(a, b);
}



NA_IDEF void naSwap(double* NA_RESTRICT a, double* NA_RESTRICT b){
  naSwap64(a, b);
}

NA_IDEF void naSwapf(float* NA_RESTRICT a, float* NA_RESTRICT b){
  naSwap32(a, b);
}

NA_IDEF void naSwapi(NAInt* NA_RESTRICT a, NAInt* NA_RESTRICT b){
  #if NA_TYPE_NAINT_BITS == 32
    naSwap32(a, b);
  #elif NA_TYPE_NAINT_BITS == 64
    naSwap64(a, b);
  #endif
}

NA_IDEF void naSwapu(NAUInt* NA_RESTRICT a, NAUInt* NA_RESTRICT b){
  #if NA_TYPE_NAINT_BITS == 32
    naSwap32(a, b);
  #elif NA_TYPE_NAINT_BITS == 64
    naSwap64(a, b);
  #endif
}





NA_IDEF NABool naEqual8(  void* NA_RESTRICT a, void* NA_RESTRICT b){
  return (*((uint8*)a) == *((uint8*)b));
}
NA_IDEF NABool naEqual16( void* NA_RESTRICT a, void* NA_RESTRICT b){
  return (*((uint16*)a) == *((uint16*)b));
}
NA_IDEF NABool naEqual32( void* NA_RESTRICT a, void* NA_RESTRICT b){
  return (*((uint32*)a) == *((uint32*)b));
}
NA_IDEF NABool naEqual64( void* NA_RESTRICT a, void* NA_RESTRICT b){
  #if defined NA_TYPE_INT64
    return (*((uint64*)a) == *((uint64*)b));
  #else
    if(*((uint32*)&((((NAByte*)a)[0]))) != *((uint32*)&((((NAByte*)b)[0])))){return NA_FALSE;}
    if(*((uint32*)&((((NAByte*)a)[4]))) != *((uint32*)&((((NAByte*)b)[4])))){return NA_FALSE;}
    return NA_TRUE;
  #endif
}
NA_IDEF NABool naEqual128(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #if defined NA_TYPE_INT64
    if(*((uint64*)&((((NAByte*)a)[0]))) != *((uint64*)&((((NAByte*)b)[0])))){return NA_FALSE;}
    if(*((uint64*)&((((NAByte*)a)[8]))) != *((uint64*)&((((NAByte*)b)[8])))){return NA_FALSE;}
    return NA_TRUE;
  #else
    if(*((uint32*)&((((NAByte*)a)[ 0]))) != *((uint32*)&((((NAByte*)b)[ 0])))){return NA_FALSE;}
    if(*((uint32*)&((((NAByte*)a)[ 4]))) != *((uint32*)&((((NAByte*)b)[ 4])))){return NA_FALSE;}
    if(*((uint32*)&((((NAByte*)a)[ 8]))) != *((uint32*)&((((NAByte*)b)[ 8])))){return NA_FALSE;}
    if(*((uint32*)&((((NAByte*)a)[12]))) != *((uint32*)&((((NAByte*)b)[12])))){return NA_FALSE;}
    return NA_TRUE;
  #endif
}




// ///////////////////////////////////////////////////////////
// Fills all bytes with null values
// ///////////////////////////////////////////////////////////

NA_IDEF void naNulln32(void* d, int32 bytesize){
  #ifndef NDEBUG
    if(bytesize < NA_ONE_32)
      naError("naNulln32", "count should not be < 1");
  #endif
  // Note that the bzero function does the same but is deprecated.
  memset(d, 0, (size_t)bytesize);
}

NA_IDEF void naNulln64(void* d, int64 bytesize){
  #ifndef NDEBUG
    if(naSmallerInt64(bytesize, NA_ONE_64))
      naError("naNulln64", "count should not be < 1");
  #endif
  // Note that the bzero function does the same but is deprecated.
  #if defined NA_TYPE_INT64
    memset(d, 0, (size_t)bytesize);
  #else
    #ifndef NDEBUG
      naError("naNulln64", "Impossible to convert to a 64 bit size_t type, as no accessible 64 bit integer type exists in this configuration. Falling back to 32 bits.");
    #endif
    memset(d, 0, (size_t)naCastInt64ToInt32(bytesize));
  #endif
}

NA_IDEF void naNulln(void* d, NAInt bytesize){
  #if NA_TYPE_NAINT_BITS == 32
    naNulln32(d, bytesize);
  #elif NA_TYPE_NAINT_BITS == 64
    naNulln64(d, bytesize);
  #else
    #error "Integer bytesize unknown"
  #endif
}



NA_IDEF void naSetn32(void* d, int32 bytesize, NAByte value){
  #ifndef NDEBUG
    if(bytesize < NA_ONE_32)
      naError("naSetn32", "count should not be < 1");
  #endif
  // Note that the bzero function does the same but is deprecated.
  memset(d, value, (size_t)bytesize);
}

NA_IDEF void naSetn64(void* d, int64 bytesize, NAByte value){
  #ifndef NDEBUG
    if(naSmallerInt64(bytesize, NA_ONE_64))
      naError("naNulln64", "count should not be < 1");
  #endif
  // Note that the bzero function does the same but is deprecated.
  #if defined NA_TYPE_INT64
    memset(d, value, (size_t)bytesize);
  #else
    #ifndef NDEBUG
      naError("naSetn64", "Impossible to convert to a 64 bit size_t type, as no accessible 64 bit integer type exists in this configuration. Falling back to 32 bits.");
    #endif
    memset(d, value, (size_t)naCastInt64ToInt32(bytesize));
  #endif
}

NA_IDEF void naSetn(void* d, NAInt bytesize, NAByte value){
  #if NA_TYPE_NAINT_BITS == 32
    naSetn32(d, bytesize, value);
  #elif NA_TYPE_NAINT_BITS == 64
    naSetn64(d, bytesize, value);
  #else
    #error "Integer bytesize unknown"
  #endif
}




// ///////////////////////////////////////////////////////////
// Set multiple bytes to the contents of a given pointer
// ///////////////////////////////////////////////////////////

NA_IDEF void naFill8WithBytes( void* d,
                    NAByte b0){
  NAByte* p;
  #ifndef NDEBUG
    if(!d){
      naCrash("naFill8WithBytes", "Pointer is Null-Pointer.");
      return;
    }
  #endif
  p = (NAByte*)d;
  *p = b0;
}

NA_IDEF void naFill16WithBytes( void* d,
                     NAByte b0, NAByte b1){
  NAByte* p;
  #ifndef NDEBUG
    if(!d){
      naCrash("naFill16WithBytes", "Pointer is Null-Pointer.");
      return;
    }
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p = b1;
}

NA_IDEF void naFill32WithBytes( void* d,
                     NAByte b0, NAByte b1, NAByte b2, NAByte b3){
  NAByte* p;
  #ifndef NDEBUG
    if(!d){
      naCrash("naFill32WithBytes", "Pointer is Null-Pointer.");
      return;
    }
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p++ = b1; *p++ = b2; *p = b3;
}

NA_IDEF void naFill64WithBytes( void* d,
                     NAByte b0, NAByte b1, NAByte b2, NAByte b3,
                     NAByte b4, NAByte b5, NAByte b6, NAByte b7){
  NAByte* p;
  #ifndef NDEBUG
    if(!d){
      naCrash("naFill64WithBytes", "Pointer is Null-Pointer.");
      return;
    }
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
  *p++ = b4; *p++ = b5; *p++ = b6; *p   = b7;
}

NA_IDEF void naFill128WithBytes( void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                      NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                      NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                      NAByte b12, NAByte b13, NAByte b14, NAByte b15){
  NAByte* p;
  #ifndef NDEBUG
    if(!d){
      naCrash("naFill128WithBytes", "Pointer is Null-Pointer.");
      return;
    }
  #endif
  p = (NAByte*)d;
  *p++ = b0;  *p++ = b1;  *p++ = b2;  *p++ = b3;
  *p++ = b4;  *p++ = b5;  *p++ = b6;  *p++ = b7;
  *p++ = b8;  *p++ = b9;  *p++ = b10; *p++ = b11;
  *p++ = b12; *p++ = b13; *p++ = b14; *p   = b15;
}



// ///////////////////////////////////////////////////////////
// Compare multiple bytes to the contents of a given pointer
// ///////////////////////////////////////////////////////////

NA_IDEF NABool naEqual8WithBytes(const void* s, NAByte b0){
  NAByte* p;
  #ifndef NDEBUG
    if(!s){
      naCrash("naEqual8WithBytes", "Pointer is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  p = (NAByte*)s;
  if(*p   != b0){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual16WithBytes(const void* s, NAByte b0, NAByte b1){
  NAByte* p;
  #ifndef NDEBUG
    if(!s){
      naCrash("naEqual16WithBytes", "Pointer is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  p = (NAByte*)s;
  if(*p++ != b0){return NA_FALSE;}
  if(*p   != b1){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual32WithBytes(const void* s,
                              NAByte b0, NAByte b1, NAByte b2, NAByte b3){
  NAByte* p;
  #ifndef NDEBUG
    if(!s){
      naCrash("naEqual32WithBytes", "Pointer is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  p = (NAByte*)s;
  if(*p++ != b0){return NA_FALSE;}
  if(*p++ != b1){return NA_FALSE;}
  if(*p++ != b2){return NA_FALSE;}
  if(*p   != b3){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual64WithBytes(const void* s,
                              NAByte b0, NAByte b1, NAByte b2, NAByte b3,
                              NAByte b4, NAByte b5, NAByte b6, NAByte b7){
  NAByte* p;
  #ifndef NDEBUG
    if(!s){
      naCrash("naEqual64WithBytes", "Pointer is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  p = (NAByte*)s;
  if(*p++ != b0){return NA_FALSE;}
  if(*p++ != b1){return NA_FALSE;}
  if(*p++ != b2){return NA_FALSE;}
  if(*p++ != b3){return NA_FALSE;}
  if(*p++ != b4){return NA_FALSE;}
  if(*p++ != b5){return NA_FALSE;}
  if(*p++ != b6){return NA_FALSE;}
  if(*p   != b7){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual128WithBytes(const void* s,
                               NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                               NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                               NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                               NAByte b12, NAByte b13, NAByte b14, NAByte b15){
  NAByte* p;
  #ifndef NDEBUG
    if(!s){
      naCrash("naEqual128WithBytes", "Pointer is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  p = (NAByte*)s;
  if(*p++ != b0) {return NA_FALSE;}
  if(*p++ != b1) {return NA_FALSE;}
  if(*p++ != b2) {return NA_FALSE;}
  if(*p++ != b3) {return NA_FALSE;}
  if(*p++ != b4) {return NA_FALSE;}
  if(*p++ != b5) {return NA_FALSE;}
  if(*p++ != b6) {return NA_FALSE;}
  if(*p++ != b7) {return NA_FALSE;}
  if(*p++ != b8) {return NA_FALSE;}
  if(*p++ != b9) {return NA_FALSE;}
  if(*p++ != b10){return NA_FALSE;}
  if(*p++ != b11){return NA_FALSE;}
  if(*p++ != b12){return NA_FALSE;}
  if(*p++ != b13){return NA_FALSE;}
  if(*p++ != b14){return NA_FALSE;}
  if(*p   != b15){return NA_FALSE;}
  return NA_TRUE;
}




// //////////////////////////////////////////////////////////////////////
// Endianness conversions
//
// The Endianness-Conversion functions are divided into single-value-converters
// and array-converters. The v-converters simply go through a whole array
// of single values and convert them using the corresponding single-value-
// converter.
//
// Note that the array-converters do not contain a test if count == 0 when
// debugging, as it will never result in an error. Even more so, there are
// situations, where sending a count of 0 is convenient.
// //////////////////////////////////////////////////////////////////////


// //////////////////////////////////////////////////////////////////////
// Convert between two same Endiannesses. Single converters
// //////////////////////////////////////////////////////////////////////

NA_IDEF void naConvertNative8(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN8", "Buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
}

NA_IDEF void naConvertNative16(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN16", "Buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
}

NA_IDEF void naConvertNative32(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN32", "Buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
}

NA_IDEF void naConvertNative64(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN64", "Buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
}

NA_IDEF void naConvertNative128(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN128", "Buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
}


// //////////////////////////////
// Convert between two same Endiannesses. Converters whole arrays
// //////////////////////////////

NA_IDEF void naConvertNative8v(void* buffer, NAUInt count){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertNative8v", "buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
  NA_UNUSED(count);
}

NA_IDEF void naConvertNative16v(void* buffer, NAUInt count){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertNative16v", "buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
  NA_UNUSED(count);
}

NA_IDEF void naConvertNative32v(void* buffer, NAUInt count){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertNative32v", "buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
  NA_UNUSED(count);
}

NA_IDEF void naConvertNative64v(void* buffer, NAUInt count){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertNative64v", "buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
  NA_UNUSED(count);
}

NA_IDEF void naConvertNative128v(void* buffer, NAUInt count){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertNative128v", "buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
  NA_UNUSED(count);
}



// //////////////////////////////////////////////////////////////////////
// Conversions Little to Big and vice versa. Single converters
// //////////////////////////////////////////////////////////////////////

NA_IDEF void naConvertLittleBig8(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertLittleBig8", "Buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
}

NA_IDEF void naConvertLittleBig16(void* buffer){
  #ifndef NDEBUG
    if(!buffer){
      naCrash("naConvertLittleBig16", "Buffer is Null-Pointer");
      return;
    }
  #endif
  naSwap8(((NAByte*)buffer)+1, ((NAByte*)buffer)+0);
}

NA_IDEF void naConvertLittleBig32(void* buffer){
  #ifndef NDEBUG
    if(!buffer){
      naCrash("naConvertLittleBig32", "Buffer is Null-Pointer");
      return;
    }
  #endif
  naSwap8(((NAByte*)buffer)+3, ((NAByte*)buffer)+0);
  naSwap8(((NAByte*)buffer)+2, ((NAByte*)buffer)+1);
}

NA_IDEF void naConvertLittleBig64(void* buffer){
  #ifndef NDEBUG
    if(!buffer){
      naCrash("naConvertLittleBig64", "Buffer is Null-Pointer");
      return;
    }
  #endif
  naSwap8(((NAByte*)buffer)+7, ((NAByte*)buffer)+0);
  naSwap8(((NAByte*)buffer)+6, ((NAByte*)buffer)+1);
  naSwap8(((NAByte*)buffer)+5, ((NAByte*)buffer)+2);
  naSwap8(((NAByte*)buffer)+4, ((NAByte*)buffer)+3);
}

NA_IDEF void naConvertLittleBig128(void* buffer){
  #ifndef NDEBUG
    if(!buffer){
      naCrash("naConvertLittleBig128", "Buffer is Null-Pointer");
      return;
    }
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

NA_IDEF void naConvertLittleBig8v(void* buffer, NAUInt count){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertLittleBig8v", "buffer is Null-Pointer");
  #endif
  NA_UNUSED(buffer);
  NA_UNUSED(count);
}

NA_IDEF void naConvertLittleBig16v(void* buffer, NAUInt count){
  NAUInt i;
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertLittleBig16v", "buffer is Null-Pointer");
  #endif
  for(i=0; i<count; i++){
    naConvertLittleBig16(buffer);
    buffer = (NAByte*)buffer + 2;
  }
}

NA_IDEF void naConvertLittleBig32v(void* buffer, NAUInt count){
  NAUInt i;
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertLittleBig32v", "buffer is Null-Pointer");
  #endif
  for(i=0; i<count; i++){
    naConvertLittleBig32(buffer);
    buffer = (NAByte*)buffer + 4;
  }
}

NA_IDEF void naConvertLittleBig64v(void* buffer, NAUInt count){
  NAUInt i;
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertLittleBig64v", "buffer is Null-Pointer");
  #endif
  for(i=0; i<count; i++){
    naConvertLittleBig64(buffer);
    buffer = (NAByte*)buffer + 8;
  }
}

NA_IDEF void naConvertLittleBig128v(void* buffer, NAUInt count){
  NAUInt i;
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertLittleBig128v", "buffer is Null-Pointer");
  #endif
  for(i=0; i<count; i++){
    naConvertLittleBig128(buffer);
    buffer = (NAByte*)buffer + 16;
  }
}




// //////////////////////////////////////
// All other endianness converters are simply redirects to the already defined
// converters, depending on this systems endianness.
//
// They are not defined as Macros as debugging would be difficult.
// //////////////////////////////////////

#if NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_LITTLE

  // Native <-> Little. Single converters
  NA_IDEF void naConvertNativeLittle8(  void* buffer){
    naConvertNative8(buffer);
  }
  NA_IDEF void naConvertNativeLittle16( void* buffer){
    naConvertNative16(buffer);
  }
  NA_IDEF void naConvertNativeLittle32( void* buffer){
    naConvertNative32(buffer);
  }
  NA_IDEF void naConvertNativeLittle64( void* buffer){
    naConvertNative64(buffer);
  }
  NA_IDEF void naConvertNativeLittle128(void* buffer){
    naConvertNative128(buffer);
  }

  // Native <-> Little. Array converters
  NA_IDEF void naConvertNativeLittle8v(  void* buffer, NAUInt count){
    naConvertNative8v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle16v( void* buffer, NAUInt count){
    naConvertNative16v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle32v( void* buffer, NAUInt count){
    naConvertNative32v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle64v( void* buffer, NAUInt count){
    naConvertNative64v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle128v(void* buffer, NAUInt count){
    naConvertNative128v(buffer, count);
  }

  // Native <-> Big. Single converters
  NA_IDEF void naConvertNativeBig8(  void* buffer){
    naConvertLittleBig8(buffer);
  }
  NA_IDEF void naConvertNativeBig16( void* buffer){
    naConvertLittleBig16(buffer);
  }
  NA_IDEF void naConvertNativeBig32( void* buffer){
    naConvertLittleBig32(buffer);
  }
  NA_IDEF void naConvertNativeBig64( void* buffer){
    naConvertLittleBig64(buffer);
  }
  NA_IDEF void naConvertNativeBig128(void* buffer){
    naConvertLittleBig128(buffer);
  }

  // Native <-> Big. Array converters
  NA_IDEF void naConvertNativeBig8v(  void* buffer, NAUInt count){
    naConvertLittleBig8v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig16v( void* buffer, NAUInt count){
    naConvertLittleBig16v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig32v( void* buffer, NAUInt count){
    naConvertLittleBig32v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig64v( void* buffer, NAUInt count){
    naConvertLittleBig64v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig128v(void* buffer, NAUInt count){
    naConvertLittleBig128v(buffer, count);
  }

#elif NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_BIG

  // Native <-> Little. Single converters
  NA_IDEF void naConvertNativeLittle8(  void* buffer){
    naConvertLittleBig8(buffer);
  }
  NA_IDEF void naConvertNativeLittle16( void* buffer){
    naConvertLittleBig16(buffer);
  }
  NA_IDEF void naConvertNativeLittle32( void* buffer){
    naConvertLittleBig32(buffer);
  }
  NA_IDEF void naConvertNativeLittle64( void* buffer){
    naConvertLittleBig64(buffer);
  }
  NA_IDEF void naConvertNativeLittle128(void* buffer){
    naConvertLittleBig128(buffer);
  }

  // Native <-> Little. Array converters
  NA_IDEF void naConvertNativeLittle8v(  void* buffer, NAUInt count){
    naConvertLittleBig8v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle16v( void* buffer, NAUInt count){
    naConvertLittleBig16v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle32v( void* buffer, NAUInt count){
    naConvertLittleBig32v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle64v( void* buffer, NAUInt count){
    naConvertLittleBig64v(buffer, count);
  }
  NA_IDEF void naConvertNativeLittle128v(void* buffer, NAUInt count){
    naConvertLittleBig128v(buffer, count);
  }

  // Native <-> Big. Single converters
  NA_IDEF void naConvertNativeBig8(  void* buffer){
    naConvertN8(buffer);
  }
  NA_IDEF void naConvertNativeBig16( void* buffer){
    naConvertN16(buffer);
  }
  NA_IDEF void naConvertNativeBig32( void* buffer){
    naConvertN32(buffer);
  }
  NA_IDEF void naConvertNativeBig64( void* buffer){
    naConvertN64(buffer);
  }
  NA_IDEF void naConvertNativeBig128(void* buffer){
    naConvertN128(buffer);
  }

  // Native <-> Big. Array converters
  NA_IDEF void naConvertNativeBig8v(  void* buffer, NAUInt count){
    naConvertNative8v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig16v( void* buffer, NAUInt count){
    naConvertNative16v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig32v( void* buffer, NAUInt count){
    naConvertNative32v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig64v( void* buffer, NAUInt count){
    naConvertNative64v(buffer, count);
  }
  NA_IDEF void naConvertNativeBig128v(void* buffer, NAUInt count){
    naConvertNative128v(buffer, count);
  }

#else
  // Other cases are not implemented yet.
#endif








NA_IDEF NAEndiannessConverter naMakeEndiannessConverter(NAInt end1, NAInt end2){
  NAEndiannessConverter converter;
  #ifndef NDEBUG
    if((end1 < 0) || (end1 >= NA_ENDIANNESS_COUNT))
      naError("createEndiannessConverter", "endianness1 is invalid. Result is undefined.");
    if((end2 < 0) || (end2 >= NA_ENDIANNESS_COUNT))
      naError("createEndiannessConverter", "endianness2 is invalid. Result is undefined.");
  #endif
  if((end1 == NA_ENDIANNESS_NATIVE) || (end1 == NA_ENDIANNESS_UNKNOWN)){
    end1 = NA_SYSTEM_ENDIANNESS;
  }
  if((end2 == NA_ENDIANNESS_NATIVE) || (end2 == NA_ENDIANNESS_UNKNOWN)){
    end2 = NA_SYSTEM_ENDIANNESS;
  }
  // Note that currently, as only Little and Big endianness is implemented,
  // there are only two possibilities: Endiannesses are the same or they are
  // little-big. If there are more endiannesses implemented in the futute,
  // this implementation will likely become a little more complicated.
  if(end1 == end2){
    converter.convert128       = &naConvertNative128;
    converter.convert64        = &naConvertNative64;
    converter.convert32        = &naConvertNative32;
    converter.convert16        = &naConvertNative16;
    converter.convert8         = &naConvertNative8;
    converter.convert128v  = &naConvertNative128v;
    converter.convert64v   = &naConvertNative64v;
    converter.convert32v   = &naConvertNative32v;
    converter.convert16v   = &naConvertNative16v;
    converter.convert8v    = &naConvertNative8v;
  }else{
    converter.convert128       = &naConvertLittleBig128;
    converter.convert64        = &naConvertLittleBig64;
    converter.convert32        = &naConvertLittleBig32;
    converter.convert16        = &naConvertLittleBig16;
    converter.convert8         = &naConvertLittleBig8;
    converter.convert128v  = &naConvertLittleBig128v;
    converter.convert64v   = &naConvertLittleBig64v;
    converter.convert32v   = &naConvertLittleBig32v;
    converter.convert16v   = &naConvertLittleBig16v;
    converter.convert8v    = &naConvertLittleBig8v;
  }
  return converter;
}





NA_IAPI NABool naIsEndiannessNative(NAInt endianness){
  #ifndef NDEBUG
    if((endianness < 0) || (endianness >= NA_ENDIANNESS_COUNT))
      naError("naIsEndiannessNative", "endianness is invalid. Result is undefined.");
  #endif
  return ( (endianness == NA_ENDIANNESS_NATIVE)
        || (endianness == NA_ENDIANNESS_UNKNOWN)
        || (endianness == NA_SYSTEM_ENDIANNESS) );
}





#define NA_NATIVE_ENDIANNESS_MARKER     0x42446462
#if NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_LITTLE
  #define NA_LITTLE_ENDIANNESS_MARKER   0x42446462
  #define NA_BIG_ENDIANNESS_MARKER      0x62644442
#elif NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_BIG
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
    return NA_ENDIANNESS_UNKNOWN;
    break;
  }
}



struct NAChecksum{
  NAChecksumType type;
  void* data;
};











// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
