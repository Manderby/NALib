
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#ifndef NA_ENDIANNESS_INCLUDED
#define NA_ENDIANNESS_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"


// Static Endianness conversions.
// Use those if you know exactly from which source endianness to which
// destination endianness you want to convert. If you don't know it at
// compile-time, use the NAEndiannessConverter below or NAByteArray.
//
// Will convert the data directly within the buffer
// provided. All converters work in both ways, for example Little to Big
// will also convert Big to Little.
//
// The Array-variants convert a whole array with count elements. The
// non-Array-variants convert only one single value.

// Conversion between two same Endiannesses.
static  void naConvertNative8(              void* buffer);
static  void naConvertNative16(             void* buffer);
static  void naConvertNative32(             void* buffer);
static  void naConvertNative64(             void* buffer);
static  void naConvertNative128(            void* buffer);
static  void naConvertArrayNative8(         void* buffer, NAInt count);
static  void naConvertArrayNative16(        void* buffer, NAInt count);
static  void naConvertArrayNative32(        void* buffer, NAInt count);
static  void naConvertArrayNative64(        void* buffer, NAInt count);
static  void naConvertArrayNative128(       void* buffer, NAInt count);

// Conversion between Little and Big Endianness
static  void naConvertLittleBig8(           void* buffer);
static  void naConvertLittleBig16(          void* buffer);
static  void naConvertLittleBig32(          void* buffer);
static  void naConvertLittleBig64(          void* buffer);
static  void naConvertLittleBig128(         void* buffer);
static  void naConvertArrayLittleBig8(      void* buffer, NAInt count);
static  void naConvertArrayLittleBig16(     void* buffer, NAInt count);
static  void naConvertArrayLittleBig32(     void* buffer, NAInt count);
static  void naConvertArrayLittleBig64(     void* buffer, NAInt count);
static  void naConvertArrayLittleBig128(    void* buffer, NAInt count);

// Conversion between Native and Little Endianness
static  void naConvertNativeLittle8(        void* buffer);
static  void naConvertNativeLittle16(       void* buffer);
static  void naConvertNativeLittle32(       void* buffer);
static  void naConvertNativeLittle64(       void* buffer);
static  void naConvertNativeLittle128(      void* buffer);
static  void naConvertArrayNativeLittle8(   void* buffer, NAInt count);
static  void naConvertArrayNativeLittle16(  void* buffer, NAInt count);
static  void naConvertArrayNativeLittle32(  void* buffer, NAInt count);
static  void naConvertArrayNativeLittle64(  void* buffer, NAInt count);
static  void naConvertArrayNativeLittle128( void* buffer, NAInt count);

// Conversion between Native and Big Endianness
static  void naConvertNativeBig8(           void* buffer);
static  void naConvertNativeBig16(          void* buffer);
static  void naConvertNativeBig32(          void* buffer);
static  void naConvertNativeBig64(          void* buffer);
static  void naConvertNativeBig128(         void* buffer);
static  void naConvertArrayNativeBig8(      void* buffer, NAInt count);
static  void naConvertArrayNativeBig16(     void* buffer, NAInt count);
static  void naConvertArrayNativeBig32(     void* buffer, NAInt count);
static  void naConvertArrayNativeBig64(     void* buffer, NAInt count);
static  void naConvertArrayNativeBig128(    void* buffer, NAInt count);





// If you have dynamically defined endiannesses, use an NAEndiannessConverter.
// Use the naMakeEndiannessConverter function to fill the structure.
// After creation, you can use the function-pointers stored in the
// NAEndiannessConverter struct directly.
// Note: This is not an opaque type.
typedef struct NAEndiannessConverter NAEndiannessConverter;
struct NAEndiannessConverter{
  void (*convert8)        (void* buffer);
  void (*convert16)       (void* buffer);
  void (*convert32)       (void* buffer);
  void (*convert64)       (void* buffer);
  void (*convert128)      (void* buffer);
  void (*convertArray8)   (void* buffer, NAInt count);
  void (*convertArray16)  (void* buffer, NAInt count);
  void (*convertArray32)  (void* buffer, NAInt count);
  void (*convertArray64)  (void* buffer, NAInt count);
  void (*convertArray128) (void* buffer, NAInt count);
};


// Returns an NAEndiannessConverter structure with all converters for all
// endiannesses available in NALib. Note that it does not really matter which
// is the endianness1 and which is endianness2 as all converters can be used
// bidirectional.
NAEndiannessConverter naMakeEndiannessConverter( NAInt endianness1,
                                                 NAInt endianness2);


// naIsEndiannessNative returns NA_TRUE if the given endianness is the same as
// the endianness of this system. Also returns NA_TRUE when sending native or
// unknown endianness. This function is useful for dynamically checking for
// endiannesses. If you simply are interested in the (static) endianness of
// this system, check the NA_SYSTEM_ENDIANNESS macro.
NABool naIsEndiannessNative(NAInt endianness);


// Use the following functions to write and read a 4-byte endianness
// marker. This is useful for files which state what endianness they
// are encoded with. Important notice: Read and write the marker as a
// raw 4-byte-array, not as a 4-byte-entity!
//
// The markers are
// - "BDdb" for Big Endian
// - "bdDB" for Little Endian
// Note that the marker will not likely change in the future.
void  naFillEndiannessMarker (NAByte marker[4], NAInt endianness);
NAInt naParseEndiannessMarker(const NAByte marker[4]);
























// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "../NALib/NABinaryData.h"




// //////////////////////////////////////////////////////////////////////
// Endianness conversions
//
// The Endianness-Conversion functions are divided into single-value-converters
// and array-converters. The Array converters simply go through a whole array
// of single values and convert them using the corresponding single-value-
// converter.
//
// Note that the array-converters do not contain a test if count == 0 when
// debugging, as it will never result in an error. Even more so, there are
// situations, where sending a count of 0 is convenient.
// //////////////////////////////////////////////////////////////////////


// //////////////////////////////////////////////////////////////////////
// Convert between two same Endiannesses. Single converters
//
// Nothing needs to be converted. The buffer = buffer forces the compiler
// to not emit warnings.
// //////////////////////////////////////////////////////////////////////

NA_INLINE_API void naConvertNative8(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN8", "Buffer is Null-Pointer");
  #endif
  buffer = buffer;
}

NA_INLINE_API void naConvertNative16(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN16", "Buffer is Null-Pointer");
  #endif
  buffer = buffer;
}

NA_INLINE_API void naConvertNative32(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN32", "Buffer is Null-Pointer");
  #endif
  buffer = buffer;
}

NA_INLINE_API void naConvertNative64(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN64", "Buffer is Null-Pointer");
  #endif
  buffer = buffer;
}

NA_INLINE_API void naConvertNative128(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertN128", "Buffer is Null-Pointer");
  #endif
  buffer = buffer;
}


// //////////////////////////////
// Convert between two same Endiannesses. Array converters
//
// Nothing needs to be converted. The buffer=buffer and count=count forces the
// compiler to not emit warnings.
// //////////////////////////////

NA_INLINE_API void naConvertArrayNative8(void* buffer, NAInt count){
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayN8", "count is negative");
    if(!buffer)
      naError("naConvertArrayN8", "buffer is Null-Pointer");
  #endif
  buffer=buffer; count=count;
}

NA_INLINE_API void naConvertArrayNative16(void* buffer, NAInt count){
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayN16", "count is negative");
    if(!buffer)
      naError("naConvertArrayN16", "buffer is Null-Pointer");
  #endif
  buffer=buffer; count=count;
}

NA_INLINE_API void naConvertArrayNative32(void* buffer, NAInt count){
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayN32", "count is negative");
    if(!buffer)
      naError("naConvertArrayN32", "buffer is Null-Pointer");
  #endif
  buffer=buffer; count=count;
}

NA_INLINE_API void naConvertArrayNative64(void* buffer, NAInt count){
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayN64", "count is negative");
    if(!buffer)
      naError("naConvertArrayN64", "buffer is Null-Pointer");
  #endif
  buffer=buffer; count=count;
}

NA_INLINE_API void naConvertArrayNative128(void* buffer, NAInt count){
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayN128", "count is negative");
    if(!buffer)
      naError("naConvertArrayN128", "buffer is Null-Pointer");
  #endif
  buffer=buffer; count=count;
}



// //////////////////////////////////////////////////////////////////////
// Conversions Little to Big and vice versa. Single converters
// //////////////////////////////////////////////////////////////////////

NA_INLINE_API void naConvertLittleBig8(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naConvertLittleBig8", "Buffer is Null-Pointer");
  #endif
  // Bytes don't need to be converted. The following line forces the compiler
  // to not emit any warning.
  buffer = buffer;
}

NA_INLINE_API void naConvertLittleBig16(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      {naCrash("naConvertLittleBig16", "Buffer is Null-Pointer"); return;}
  #endif
  naSwap8(((NAByte*)buffer)+1, ((NAByte*)buffer)+0);
}

NA_INLINE_API void naConvertLittleBig32(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      {naCrash("naConvertLittleBig32", "Buffer is Null-Pointer"); return;}
  #endif
  naSwap8(((NAByte*)buffer)+3, ((NAByte*)buffer)+0);
  naSwap8(((NAByte*)buffer)+2, ((NAByte*)buffer)+1);
}

NA_INLINE_API void naConvertLittleBig64(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      {naCrash("naConvertLittleBig64", "Buffer is Null-Pointer"); return;}
  #endif
  naSwap8(((NAByte*)buffer)+7, ((NAByte*)buffer)+0);
  naSwap8(((NAByte*)buffer)+6, ((NAByte*)buffer)+1);
  naSwap8(((NAByte*)buffer)+5, ((NAByte*)buffer)+2);
  naSwap8(((NAByte*)buffer)+4, ((NAByte*)buffer)+3);
}

NA_INLINE_API void naConvertLittleBig128(void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      {naCrash("naConvertLittleBig128", "Buffer is Null-Pointer"); return;}
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

NA_INLINE_API void naConvertArrayLittleBig8(void* buffer, NAInt count){
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayLittleBig8", "count is negative");
    if(!buffer)
      naError("naConvertArrayLittleBig8", "buffer is Null-Pointer");
  #endif
  // Bytes don't need to be converted. The following line forces the compiler
  // to not emit any warning.
  buffer=buffer; count=count;
}

NA_INLINE_API void naConvertArrayLittleBig16(void* buffer, NAInt count){
  NAInt i; // Declaration before implementation. Needed for C90
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayLittleBig16", "count is negative");
    if(!buffer)
      naError("naConvertArrayLittleBig16", "buffer is Null-Pointer");
  #endif
  for(i=0; i<count; i++){
    naConvertLittleBig16(buffer);
    buffer = (NAByte*)buffer + 2;
  }
}

NA_INLINE_API void naConvertArrayLittleBig32(void* buffer, NAInt count){
  NAInt i; // Declaration before implementation. Needed for C90
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayLittleBig32", "count is negative");
    if(!buffer)
      naError("naConvertArrayLittleBig32", "buffer is Null-Pointer");
  #endif
  for(i=0; i<count; i++){
    naConvertLittleBig32(buffer);
    buffer = (NAByte*)buffer + 4;
  }
}

NA_INLINE_API void naConvertArrayLittleBig64(void* buffer, NAInt count){
  NAInt i; // Declaration before implementation. Needed for C90
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayLittleBig64", "count is negative");
    if(!buffer)
      naError("naConvertArrayLittleBig64", "buffer is Null-Pointer");
  #endif
  for(i=0; i<count; i++){
    naConvertLittleBig64(buffer);
    buffer = (NAByte*)buffer + 8;
  }
}

NA_INLINE_API void naConvertArrayLittleBig128(void* buffer, NAInt count){
  NAInt i; // Declaration before implementation. Needed for C90
  #ifndef NDEBUG
    if(count < 0)
      naError("naConvertArrayLittleBig128", "count is negative");
    if(!buffer)
      naError("naConvertArrayLittleBig128", "buffer is Null-Pointer");
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
  NA_INLINE_API void naConvertNativeLittle8(  void* buffer){
    naConvertNative8(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle16( void* buffer){
    naConvertNative16(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle32( void* buffer){
    naConvertNative32(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle64( void* buffer){
    naConvertNative64(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle128(void* buffer){
    naConvertNative128(buffer);
  }

  // Native <-> Little. Array converters
  NA_INLINE_API void naConvertArrayNativeLittle8(  void* buffer,
                                                      NAInt count){
    naConvertArrayNative8(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle16( void* buffer,
                                                      NAInt count){
    naConvertArrayNative16(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle32( void* buffer,
                                                      NAInt count){
    naConvertArrayNative32(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle64( void* buffer,
                                                      NAInt count){
    naConvertArrayNative64(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle128(void* buffer,
                                                      NAInt count){
    naConvertArrayNative128(buffer, count);
  }

  // Native <-> Big. Single converters
  NA_INLINE_API void naConvertNativeBig8(  void* buffer){
    naConvertLittleBig8(buffer);
  }
  NA_INLINE_API void naConvertNativeBig16( void* buffer){
    naConvertLittleBig16(buffer);
  }
  NA_INLINE_API void naConvertNativeBig32( void* buffer){
    naConvertLittleBig32(buffer);
  }
  NA_INLINE_API void naConvertNativeBig64( void* buffer){
    naConvertLittleBig64(buffer);
  }
  NA_INLINE_API void naConvertNativeBig128(void* buffer){
    naConvertLittleBig128(buffer);
  }

  // Native <-> Big. Array converters
  NA_INLINE_API void naConvertArrayNativeBig8(  void* buffer,
                                                   NAInt count){
    naConvertArrayLittleBig8(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig16( void* buffer,
                                                   NAInt count){
    naConvertArrayLittleBig16(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig32( void* buffer,
                                                   NAInt count){
    naConvertArrayLittleBig32(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig64( void* buffer,
                                                   NAInt count){
    naConvertArrayLittleBig64(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig128(void* buffer,
                                                   NAInt count){
    naConvertArrayLittleBig128(buffer, count);
  }

#elif NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_BIG

  // Native <-> Little. Single converters
  NA_INLINE_API void naConvertNativeLittle8(  void* buffer){
    naConvertLittleBig8(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle16( void* buffer){
    naConvertLittleBig16(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle32( void* buffer){
    naConvertLittleBig32(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle64( void* buffer){
    naConvertLittleBig64(buffer);
  }
  NA_INLINE_API void naConvertNativeLittle128(void* buffer){
    naConvertLittleBig128(buffer);
  }

  // Native <-> Little. Array converters
  NA_INLINE_API void naConvertArrayNativeLittle8(  void* buffer,
                                                      NAInt count){
    naConvertArrayLittleBig8(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle16( void* buffer,
                                                      NAInt count){
    naConvertArrayLittleBig16(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle32( void* buffer,
                                                      NAInt count){
    naConvertArrayLittleBig32(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle64( void* buffer,
                                                      NAInt count){
    naConvertArrayLittleBig64(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeLittle128(void* buffer,
                                                      NAInt count){
    naConvertArrayLittleBig128(buffer, count);
  }

  // Native <-> Big. Single converters
  NA_INLINE_API void naConvertNativeBig8(  void* buffer){
    naConvertN8(buffer);
  }
  NA_INLINE_API void naConvertNativeBig16( void* buffer){
    naConvertN16(buffer);
  }
  NA_INLINE_API void naConvertNativeBig32( void* buffer){
    naConvertN32(buffer);
  }
  NA_INLINE_API void naConvertNativeBig64( void* buffer){
    naConvertN64(buffer);
  }
  NA_INLINE_API void naConvertNativeBig128(void* buffer){
    naConvertN128(buffer);
  }

  // Native <-> Big. Array converters
  NA_INLINE_API void naConvertArrayNativeBig8(  void* buffer,
                                                   NAInt count){
    naConvertArrayN8(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig16( void* buffer,
                                                   NAInt count){
    naConvertArrayN16(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig32( void* buffer,
                                                   NAInt count){
    naConvertArrayN32(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig64( void* buffer,
                                                   NAInt count){
    naConvertArrayN64(buffer, count);
  }
  NA_INLINE_API void naConvertArrayNativeBig128(void* buffer,
                                                   NAInt count){
    naConvertArrayN128(buffer, count);
  }

#else
  // Other cases are not implemented yet.
#endif








NAEndiannessConverter naMakeEndiannessConverter(NAInt end1, NAInt end2){
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
    converter.convertArray128  = &naConvertArrayNative128;
    converter.convertArray64   = &naConvertArrayNative64;
    converter.convertArray32   = &naConvertArrayNative32;
    converter.convertArray16   = &naConvertArrayNative16;
    converter.convertArray8    = &naConvertArrayNative8;
  }else{
    converter.convert128       = &naConvertLittleBig128;
    converter.convert64        = &naConvertLittleBig64;
    converter.convert32        = &naConvertLittleBig32;
    converter.convert16        = &naConvertLittleBig16;
    converter.convert8         = &naConvertLittleBig8;
    converter.convertArray128  = &naConvertArrayLittleBig128;
    converter.convertArray64   = &naConvertArrayLittleBig64;
    converter.convertArray32   = &naConvertArrayLittleBig32;
    converter.convertArray16   = &naConvertArrayLittleBig16;
    converter.convertArray8    = &naConvertArrayLittleBig8;
  }
  return converter;
}





NABool naIsEndiannessNative(NAInt endianness){
  #ifndef NDEBUG
    if((endianness < 0) || (endianness >= NA_ENDIANNESS_COUNT))
      naError("naIsEndiannessNative", "endianness is invalid. Result is undefined.");
  #endif
  return ( (endianness == NA_ENDIANNESS_NATIVE)
        || (endianness == NA_ENDIANNESS_UNKNOWN)
        || (endianness == NA_SYSTEM_ENDIANNESS));
}





#define NA_NATIVE_ENDIANNESS_MARKER     0x42446462
#if NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_LITTLE
  #define NA_LITTLE_ENDIANNESS_MARKER   0x42446462
  #define NA_BIG_ENDIANNESS_MARKER      0x62644442
#elif NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_BIG
  #define NA_LITTLE_ENDIANNESS_MARKER   0x62644442
  #define NA_BIG_ENDIANNESS_MARKER      0x42446462
#endif


void naFillEndiannessMarker(NAByte marker[4], NAInt endianness){
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

NAInt naParseEndiannessMarker(const NAByte marker[4]){
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

#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_ENDIANNESS_INCLUDED


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
