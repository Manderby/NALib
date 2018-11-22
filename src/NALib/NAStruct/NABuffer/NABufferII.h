
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABuffer.h"

typedef struct NABufferSource NABufferSource;
typedef struct NABufferPart NABufferPart;



#include "NATree.h"



struct NABufferPart{
  NAUInt     byteoffset;  // Start-index in the referenced data
  NAUInt     bytesize;    // Number of bytes being referenced
  NAPointer* pointer;     // A reference counting pointer referencing the data.
};

struct NABufferSource{
  NABufferSourceDescriptor desc;  
};

struct NABufferIterator{
  NAPtr bufferptr;
  NAInt curoffset;
  uint8 curbit;             // The current bit number
//  NAUInt linenum;    // The line number, starting with 1 after first line read.
  NATreeIterator partiter;
};

struct NABuffer{
  NABufferSource* source;
  NAInt srcoffset;       // Offset of source relative to this buffers origin.
                         // Add this offset to the desired pos to get the
                         // position within the source.
  
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




// NABufferPart
NA_HAPI NABufferPart* naNewBufferPartSparse(NAUInt bytesize);
NA_HAPI NABufferPart* naNewBufferPartConstData(const NAByte* data, NAUInt bytesize);
NA_HAPI NABufferPart* naNewBufferPartMutableData(NAByte* data, NAUInt bytesize, NAMutator destructor);
NA_HAPI NABool naIsBufferPartSparse(const NABufferPart* part);

// NABufferHelper
NA_HAPI void naEnsureBufferRange(NABuffer* buffer, NARangei range);


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
