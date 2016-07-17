
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_BINARY_DATA_INCLUDED
#define NA_BINARY_DATA_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This file contains multiple functions dealing with binary data.
//
// Aside from copying, swapping or comparing bytes, there are also some
// encoding and decoding functions as well as Checksums.


#include "NASystem.h"


// Copies the array s to array d byte-by-byte
NA_IAPI void naCopy8    (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy16   (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy32   (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy64   (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy128  (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopyn    (void* NA_RESTRICT d,
                  const void* NA_RESTRICT s,
                                   NAUInt count);


// Swaps the content of the two pointers with the given number of bits.
NA_IAPI void naSwap8   (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap16  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap32  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap64  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap128 (void* NA_RESTRICT a, void* NA_RESTRICT b);
// Note: These functions are implemented with a well known swapping trick which
// only works if the two pointers do not point to the same location. Therefore
// restrict pointers make prefectly sense here.

// Swaps two Variables
NA_IAPI void naSwap    (double* NA_RESTRICT a, double* NA_RESTRICT b);
NA_IAPI void naSwapf   (float*  NA_RESTRICT a, float*  NA_RESTRICT b);
NA_IAPI void naSwapi   (NAInt*  NA_RESTRICT a, NAInt*  NA_RESTRICT b);
NA_IAPI void naSwapu   (NAUInt* NA_RESTRICT a, NAUInt* NA_RESTRICT b);


// Compares the array of the first argument with the array of the second
// argument and returns NA_TRUE only if they are all equal.
NA_IAPI NABool naEqual8(  void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual16( void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual32( void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual64( void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual128(void* NA_RESTRICT a, void* NA_RESTRICT b);


// Fills the given buffer with the value 0 for count bytes
NA_IAPI void naNulln   (void* d, NAUInt count);



// Fills the array of the d argument with the given bytes in increasing
// address positions.
NA_IAPI void naFill8WithBytes(   void* d,
                      NAByte b0);
NA_IAPI void naFill16WithBytes(  void* d,
                      NAByte b0,  NAByte b1);
NA_IAPI void naFill32WithBytes(  void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3);
NA_IAPI void naFill64WithBytes(  void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                      NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7);
NA_IAPI void naFill128WithBytes( void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                      NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                      NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                      NAByte b12, NAByte b13, NAByte b14, NAByte b15);


// Compares the array of the first argument with the given bytes in increasing
// address positions and returns NA_TRUE only if they are all equal.
NA_IAPI NABool naEqual8WithBytes(   const void* s,
                                NAByte b0);
NA_IAPI NABool naEqual16WithBytes(  const void* s,
                                NAByte b0,  NAByte b1);
NA_IAPI NABool naEqual32WithBytes(  const void* s,
                                NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3);
NA_IAPI NABool naEqual64WithBytes(  const void* s,
                                NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                                NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7);
NA_IAPI NABool naEqual128WithBytes( const void* s,
                                NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                                NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                                NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                                NAByte b12, NAByte b13, NAByte b14, NAByte b15);



// Checksums

typedef struct NAChecksum NAChecksum;

typedef enum{
  NA_CHECKSUM_TYPE_CRC_PNG,
  NA_CHECKSUM_TYPE_ADLER_32, // Used in ZLIB
} NAChecksumType;


NA_API NAChecksum* naInitChecksum(NAChecksum* checksum, NAChecksumType type);
NA_API void naClearChecksum(NAChecksum* checksum);
NA_API void naResetChecksum(NAChecksum* checksum);
NA_API void naAccumulateChecksum(NAChecksum* checksum, const NAByte* buf, NAInt size);
NA_API uint32 naGetChecksumResult(NAChecksum* checksum);

























// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include <string.h>


NA_IDEF void naCopy8  (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist = (NAByte*)d - (NAByte*)s;
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
    NAInt dist = (NAByte*)d - (NAByte*)s;
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
    NAInt dist = (NAByte*)d - (NAByte*)s;
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
    NAInt dist = (NAByte*)d - (NAByte*)s;
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
  *(uint64*)d = *(uint64*)s;
}
NA_IDEF void naCopy128(void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist = (NAByte*)d - (NAByte*)s;
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
  *(uint64*)d = *(uint64*)s;
  d = ((NAByte*)d) + 8;
  s = ((NAByte*)s) + 8;
  *(uint64*)d = *(uint64*)s;
}
NA_IDEF void naCopyn(void* NA_RESTRICT d,
              const void* NA_RESTRICT s,
                               NAUInt count){
  #ifndef NDEBUG
    if(!d){
      naCrash("naCopy8", "Pointer d is Null-Pointer.");
      return;
    }
    if(!s){
      naCrash("naCopy8", "Pointer s is Null-Pointer.");
      return;
    }
    if(count < 1){
      naCrash("naCopyn", "count is %" NA_PRIu " which is smaller than 1.", count);
      return;
    }
  #endif
  memcpy(d, s, count);
}




// /////////////////////////////
// Swap multibyte-values
// /////////////////////////////

NA_IDEF void naSwap8(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist = (NAByte*)a - (NAByte*)b;
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
    NAInt dist = (NAByte*)a - (NAByte*)b;
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
    NAInt dist = (NAByte*)a - (NAByte*)b;
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
    NAInt dist = (NAByte*)a - (NAByte*)b;
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
  *(uint64*)a^=*(uint64*)b;
  *(uint64*)b^=*(uint64*)a;
  *(uint64*)a^=*(uint64*)b;
}

NA_IDEF void naSwap128(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist = (NAByte*)a - (NAByte*)b;
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
  #if NA_SYSTEM_ADDRESS_BITS == 32
    naSwap32(a, b);
  #elif NA_SYSTEM_ADDRESS_BITS == 64
    naSwap64(a, b);
  #endif
}

NA_IDEF void naSwapu(NAUInt* NA_RESTRICT a, NAUInt* NA_RESTRICT b){
  #if NA_SYSTEM_ADDRESS_BITS == 32
    naSwap32(a, b);
  #elif NA_SYSTEM_ADDRESS_BITS == 64
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
  return (*((uint64*)a) == *((uint64*)b));
}
NA_IDEF NABool naEqual128(void* NA_RESTRICT a, void* NA_RESTRICT b){
  if(*((uint64*)&((((NAByte*)a)[0]))) != *((uint64*)&((((NAByte*)b)[0])))){return NA_FALSE;}
  if(*((uint64*)&((((NAByte*)a)[8]))) != *((uint64*)&((((NAByte*)b)[8])))){return NA_FALSE;}
  return NA_TRUE;
}




// ///////////////////////////////////////////////////////////
// Fills all bytes with null values
// ///////////////////////////////////////////////////////////

NA_IDEF void naNulln(void* d, NAUInt count){
  #ifndef NDEBUG
    if(count < 1)
      naError("naNulln", "count is < 1.");
  #endif
  // Note that the bzero function does the same but is deprecated.
  memset(d, 0, count);
}






// ///////////////////////////////////////////////////////////
// Set multiple bytes to the contents of a given pointer
// ///////////////////////////////////////////////////////////

NA_IDEF void naFill8WithBytes( void* d,
                    NAByte b0){
  NAByte* p; // Declaration before implementation. Needed for C90.
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
  NAByte* p; // Declaration before implementation. Needed for C90.
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
  NAByte* p; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!d){
      naCrash("naFill32WithBytes", "Pointer is Null-Pointer.");
      return;
    }
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p++ = b1; *p++ = b2; *p = b3;
}

NA_IDEF void naFill64WithBytesBytes( void* d,
                     NAByte b0, NAByte b1, NAByte b2, NAByte b3,
                     NAByte b4, NAByte b5, NAByte b6, NAByte b7){
  NAByte* p; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!d){
      naCrash("naFill64WithBytesBytes", "Pointer is Null-Pointer.");
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
  NAByte* p; // Declaration before implementation. Needed for C90.
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
  NAByte* p; // Declaration before implementation. Needed for C90
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
  NAByte* p; // Declaration before implementation. Needed for C90
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
  NAByte* p; // Declaration before implementation. Needed for C90
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
  NAByte* p; // Declaration before implementation. Needed for C90
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
  NAByte* p; // Declaration before implementation. Needed for C90
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



struct NAChecksum{
  NAChecksumType type;
  void* data;
};










#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_BINARY_DATA_INCLUDED


// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
