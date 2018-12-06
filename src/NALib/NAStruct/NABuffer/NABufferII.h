
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABuffer.h"

typedef struct NABufferPart NABufferPart;
typedef struct NABufferSourcePart NABufferSourcePart;
typedef struct NAMemoryBlock NAMemoryBlock;
typedef struct NABufferSearchToken NABufferSearchToken;



#include "NATree.h"


struct NABufferIterator{
  NAPtr bufferptr;
  NATreeIterator partiter;
  NAInt partoffset;  // The current byte offset in the referenced part.
  uint8 curbit;      // The current bit number
  NAUInt linenum;    // The line number, starting with 1 after first line read.
};

struct NAMemoryBlock{
  NAPtr            data;
  NAMutator        deallocator;
  #ifndef NDEBUG
    NAInt          bytesize;
  #endif
};
NA_EXTERN_RUNTIME_TYPE(NAMemoryBlock);

struct NABufferPart{
  NABufferSource*     source;       // The referenced source.
  NAInt               sourceoffset; // The source offset of the first byte
  NAInt               blockoffset;  // The byte offset in the block.
  NAInt               bytesize;     // The number of bytes referenced.
  NAMemoryBlock*      memblock;     // The referenced memory block.
};

struct NABuffer{
  NABufferSource* enhancesource;
  NAInt enhancesourceoffset; // Offset of source relative to this buffers
                             // origin. Add this offset to the desired pos to
                             // get the position within the source.

  NAUInt flags;
  NARangei range;

  NANewlineEncoding newlineencoding;  // The current newline encoding
  NAInt endianness;                   // The current endianness
  NAEndiannessConverter converter;    // The endianness converter.

  NATree parts;             // Tree with all parts in this buffer

  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NABufferSearchToken{
  NAInt searchoffset;
  NAInt curoffset;
};

typedef struct NABufferTreeNodeData NABufferTreeNodeData;
struct NABufferTreeNodeData{
  NAInt len1;
  NAInt len2;
};


// NAMemoryBlock
NA_HDEF NAMemoryBlock* naNewMemoryBlock(NAInt bytesize);

// NABufferPart
NA_HAPI NABufferPart* naNewBufferPartSparse(NABufferSource* source, NARangei range);
NA_HAPI NABufferPart* naNewBufferPartWithConstData(const void* data, NAInt bytesize);
NA_HAPI NABufferPart* naNewBufferPartWithMutableData(void* data, NAInt bytesize, NAMutator deallocator);
NA_HAPI NAInt naGetBufferPartNormedStart(NAInt start);
NA_HAPI NAInt naGetBufferPartNormedEnd(NAInt end);
NA_HAPI NAInt naPrepareBufferPart(NABufferIterator* iter, NAInt bytecount, NABool forcevolatile);
NA_HAPI NATreeIterator* naGetBufferIteratorPartIterator(NABufferIterator* iter);
NA_HAPI NAInt naGetBufferIteratorPartOffset(NABufferIterator* iter);
NA_HAPI NABool naIsBufferPartSparse(NABufferIterator* iter);

//NA_HAPI void naAllocateBufferPartMemory(NABufferPart* part);
NA_HAPI void naDestructBufferPart(NABufferPart* part);
NA_HAPI void naSplitBufferSparsePart(NABufferIterator* iter, NAInt start, NAInt end);
NA_HAPI const void* naGetBufferPartDataPointerConst(NABufferIterator* iter);

// NABufferHelper
NA_HAPI void naEnsureBufferRange(NABuffer* buffer, NAInt start, NAInt end);
NA_HAPI void naPrepareBuffer(NABufferIterator* iter, NAInt bytecount, NABool forcevolatile);

// NABufferIteration
NA_HAPI const NABuffer* naGetBufferIteratorBufferConst(const NABufferIterator* iter);
NA_HAPI NABuffer* naGetBufferIteratorBufferMutable(NABufferIterator* iter);
NA_HAPI void naRetrieveBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize, NABool advance);
NA_HAPI NABufferPart* naGetBufferPart(NABufferIterator* iter);
NA_HAPI NABool naIterateBufferPart(NABufferIterator* iter);
NA_API NABool naLocateBufferFirstPart(NABufferIterator* iter);
NA_API NABool naLocateBufferLastPart(NABufferIterator* iter);

// NABufferSource
NA_HAPI NARangei naGetBufferSourceLimit(NABufferSource* source);
NA_HAPI NABool naIsBufferSourceVolatile(const NABufferSource* source);
#ifndef NDEBUG
  NA_HAPI NABool naIsBufferSourceLimited(const NABufferSource* source);
#endif
NA_HAPI NAMemoryBlock* naPrepareBufferSource(NABufferSource* source, NAInt sourceoffset, NAInt* blockoffset);
NA_HAPI NABool naHasBufferSourceUnderlyingBuffer(const NABufferSource* source);
NA_HAPI NABuffer* naGetBufferSourceUnderlyingBuffer(NABufferSource* source);



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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
