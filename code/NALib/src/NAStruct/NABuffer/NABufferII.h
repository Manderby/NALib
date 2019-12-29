
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABuffer.h"


// Buffers are implemented in an highly granular way. Each buffer is a series
// of buffer parts stitched together. Consider the following three sentences
// as buffers:
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf1 = |     This is     |      very       |    exciting     |
//                 +-----------------+-----------------+-----------------+
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf2 = | Don't question  |       her       |    authority    |
//                 +-----------------+-----------------+-----------------+
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf3 = | You can see ads |    everywhere   |   these days    |
//                 +-----------------+-----------------+-----------------+
//
// Let's look at a single buffer part in more detail: The buffer part
// containing the word "everywhere" looks like this:
//
// +==== BUFFER PART ====+
// | blockoffset :  0    |
// | bytesize    : 10    |       +==== MEMORY BLOCK ====+
// | memblock ---------------->  |      everywhere      |
// +=====================+       +======================+
//
// It stores its content in a memory block which is self-organizing. This means
// the memory block knows how and where to store or deallocate the memory
// necessary to store the word "everywhere" and knows how many instances still
// need the memory block to exist.
//
// A buffer part is only referencing such a memory block. It also defines
// which bytes of that memory block are used. In this case 10 bytes starting
// with index 0.
//
// Now, consider the words "very" and "her" which both are substrings of the
// word "everywhere" (e-VERY-w-HER-e). The buffer parts can look like this:
//
// +==== BUFFER PART ====+
// | blockoffset :  1    |
// | bytesize    :  4    |
// | memblock --------------\
// +=====================+   \
//                            -->  +==== MEMORY BLOCK ====+
// +==== BUFFER PART ====+         |      everywhere      |
// | blockoffset :  6    |    -->  +======================+
// | bytesize    :  3    |   /
// | memblock --------------/
// +=====================+
//
// As you can see, the buffer parts share the same memory block. It is
// important to understand that it is the referenced memery blocks which
// are shared amongst NABuffers, NOT the buffer parts!
//
// Now consider the following C-like string written in source code and how
// we easily turn that into an NABuffer:
//
// const NAUTF8Char* mystring = "This is an exciting trend these days.";
// NABuffer* buf4 = naNewBufferWithConstData(mystring, naStrlen(mystring));
//
// This results in a buf4 containing exactly one buffer part:
//
// +==== BUFFER PART ====+
// | blockoffset :  0    |
// | bytesize    : 37    |       +============ MEMORY BLOCK =============+
// | memblock ---------------->  | This is an exciting trend these days. |
// +=====================+       +=======================================+
//
// Note that the data stored in mystring will not be copied but referenced
// and the memory block remembers that this was a const array and therefore
// will NOT try to deallocate it if it is no longer in use.
//
// Going back to buf1, we can show the buffer a little more in detail:
//
//                 +=== BUFFER PART ==+=== BUFFER PART ==+=== BUFFER PART ==+
// NABuffer buf1 = |     This is      |       very       |     exciting.    |
//                 | blockoffset :  0 | blockoffset :  1 | blockoffset : 12 |
//                 | bytesize    :  7 | bytesize    :  4 | bytesize    :  8 |
//               +-- memblock         | memblock  --+    | memblock  --+    |
//               | +------------------+-------------|----+-------------|----+
//               |                                  |                  |
//               |    +==== MEMORY BLOCK ====+      |                  |
//               |    |      everywhere      | <----+                  |
//               |    +======================+                         |
//               |                                                     |
//               |       +============ MEMORY BLOCK =============+     |
//               +-----> | This is an exciting trend these days. | <---+
//                       +=======================================+
//
// So far so good but let's go further.
//
// Imagine, buf3 being read from a file. In NALib, the file content stays on
// disk as long as it is not explicitely used. Therefore at the beginning
// when first creating buf3, it looks something like this:
//
//        +==================== BUFFER PART ====================+
// buf3 = |                                                     |
//        +-----------------------------------------------------+
//
// That single buffer part knows that the file content has in total 37 bytes
// but it has not yet allocated a memory block. This is called a "sparse part".
// When reading the file, such sparse parts will subsequentially be turned
// into filled parts. For example when reading the file byte by byte, it
// might looke somewhat like this:
//
//        +==================== BUFFER PART ====================+
//   |    |                                                     |
//   |    +== BUFFER PART ==+=========== BUFFER PART ===========+
//   |    | You can see ads |                                   |
//   |    +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
//   |    | You can see ads |    everywhere   |                 |
//   |    +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
//   V    | You can see ads |    everywhere   |   these days    |
//        +-----------------+-----------------+-----------------+
//
// The obvious advantage of that is that memory is only allocated when really
// needed. This also works when for example only the first few bytes of a
// large file need to be read and the last few bytes. In our little example,
// this might look like this:
//
//        +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
//        | You can see ads |                 |   these days    |
//        +-----------------+-----------------+-----------------+
//
// The middle buffer part is sparse. Now, as good as the advantages sound,
// this poses a problem. When looking above, buf1 and buf2 require the middle
// buffer part to be filled with the word "everywhere".
//
// NALib solves this problem automatically by remembering, what the original
// source of a buffer part should be. This is not shown here in the diagrams
// of a buffer part, but whenever a buffer part is sparse, it remembers which
// bytes from which source should be stored in that part. And whenever needed,
// the sparse part will "ask" the source to fill up its own buffer part with
// content, hence creating a memory block which then in turn will finally be
// referenced.
//
// A source can for example be a file, a const char* array or another NABuffer.
// You can create custom sources with naNewBufferSource and the corresponding
// buffers by calling naNewBufferWithCustomSource.



typedef struct NABufferPart NABufferPart;
typedef struct NAMemoryBlock NAMemoryBlock;
typedef struct NABufferSearchToken NABufferSearchToken;


#include "../../NATree.h"


struct NABufferIterator{
  NAPtr bufferptr;
  NATreeIterator partiter;
  NAInt partoffset;  // The current byte offset in the referenced part.
  uint8 curbit;      // The current bit number
  NAUInt linenum;    // The line number, starting with 1 after first line read.
};

struct NABuffer{
  NABufferSource* source;
  NAInt sourceoffset; // Offset of source relative to this buffers
                             // origin. Add this offset to the desired pos to
                             // get the position within the source.
  NAUInt flags;
  NARangei range;

  NANewlineEncoding newlineencoding;  // The current newline encoding
  NAInt endianness;                   // The current endianness

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


// NABufferHelper
NA_HAPI void naEnsureBufferRange(NABuffer* buffer, NAInt start, NAInt end);

// NABufferIteration
NA_HIAPI const NABuffer* naGetBufferIteratorBufferConst(const NABufferIterator* iter);
NA_HIAPI NABuffer* naGetBufferIteratorBufferMutable(NABufferIterator* iter);

NA_HAPI NABool naLocateBufferStart(NABufferIterator* iter);
NA_HAPI NABool naLocateBufferLastPart(NABufferIterator* iter);
NA_HAPI NABool naLocateBufferNextPart(NABufferIterator* iter);
NA_HAPI NABool naLocateBufferPrevPartMax(NABufferIterator* iter);
NA_HAPI NABool naLocateBufferMax(NABufferIterator* iter);
NA_HAPI NABool naLocateBufferEnd(NABufferIterator* iter);
NA_HAPI NABool naIterateBufferPart(NABufferIterator* iter);

NA_HAPI NABuffer* naGetBufferIteratorSourceBuffer(NABufferIterator* iter);
NA_HIAPI NAInt naGetBufferIteratorPartOffset(NABufferIterator* iter);
NA_HAPI NABool naIsBufferIteratorSparse(NABufferIterator* iter);
NA_HIAPI NABufferPart* naGetBufferPart(NABufferIterator* iter);
NA_HAPI void naPrepareBuffer(NABufferIterator* iter, NAInt bytecount);

// NABufferPart
NA_HAPI NABufferPart* naNewBufferPartSparse(NABufferSource* source, NARangei sourcerange);
NA_HAPI NABufferPart* naNewBufferPartWithConstData(const void* data, NAInt bytesize);
NA_HAPI NABufferPart* naNewBufferPartWithMutableData(void* data, NAInt bytesize, NAMutator destructor);
NA_HAPI void naSeparateBufferPart(NABufferPart* part);

NA_HIAPI NABufferSource* naGetBufferPartSource(const NABufferPart* part);
NA_HIAPI NAInt naGetBufferPartByteSize(const NABufferPart* part);
NA_HIAPI NAMemoryBlock* naGetBufferPartMemoryBlock(const NABufferPart* part);
NA_HIAPI NABool naIsBufferPartSparse(const NABufferPart* part);

NA_HAPI NABufferPart* naSplitBufferPart(NATreeIterator* partiter, NAInt start, NAInt end);
NA_HAPI NABufferPart* naPrepareBufferPartSourceBuffer(NATreeIterator* partiter, NARangei partrange);
NA_HAPI NABufferPart* naPrepareBufferPartMemory(NATreeIterator* partiter, NARangei partrange);
NA_HAPI NAInt naPrepareBufferPart(NABufferIterator* iter, NAInt bytecount);

NA_HIAPI void naEnlargeBufferPart(NABufferPart* part, NAInt bytesatstart, NAInt bytesatend);
NA_HIAPI NAInt naGetBufferPartRemainingBytes(NABufferIterator* iter);
NA_HIAPI const void* naGetBufferPartDataPointerConst(NABufferIterator* iter);
NA_HIAPI void* naGetBufferPartDataPointerMutable(NABufferIterator* iter);

// NABufferSource
NA_HIAPI NABuffer* naGetBufferSourceUnderlyingBuffer(NABufferSource* source);
NA_HIAPI NABool naIsBufferSourceLimited(const NABufferSource* source);
NA_HIAPI NARangei naGetBufferSourceLimit(const NABufferSource* source);
NA_HIAPI void naFillSourceBuffer(const NABufferSource* source, void* dst, NARangei range);

// NABufferRead and NABufferWrite
NA_HAPI void naRetrieveBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize, NABool advance);
NA_HAPI void naStoreBufferBytes(NABufferIterator* iter, const void* data, NAInt bytesize, NABool prepare, NABool advance);

// NAMemoryBlock
NA_HIAPI NAMemoryBlock* naNewMemoryBlock(NAInt bytesize);
NA_HIAPI NAMemoryBlock* naNewMemoryBlockWithData(NAPtr data, NAInt bytesize, NAMutator destructor);
NA_HIAPI const void* naGetMemoryBlockDataPointerConst(NAMemoryBlock* block, NAInt indx);
NA_HIAPI void* naGetMemoryBlockDataPointerMutable(NAMemoryBlock* block, NAInt indx);


#include "NABufferHelperII.h"
#include "NABufferIterationII.h"
#include "NABufferParseII.h"
#include "NABufferPartII.h"
#include "NABufferReadII.h"
#include "NABufferSourceII.h"
#include "NABufferWriteII.h"
#include "NAMemoryBlockII.h"


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