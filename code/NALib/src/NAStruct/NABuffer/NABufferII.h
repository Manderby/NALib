
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
// | blockOffset :  0    |
// | byteSize    : 10    |       +==== MEMORY BLOCK ====+
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
// | blockOffset :  1    |
// | byteSize    :  4    |
// | memblock --------------\
// +=====================+   \
//                            -->  +==== MEMORY BLOCK ====+
// +==== BUFFER PART ====+         |      everywhere      |
// | blockOffset :  6    |    -->  +======================+
// | byteSize    :  3    |   /
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
// | blockOffset :  0    |
// | byteSize    : 37    |       +============ MEMORY BLOCK =============+
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
//                 | blockOffset :  0 | blockOffset :  1 | blockOffset : 12 |
//                 | byteSize    :  7 | byteSize    :  4 | byteSize    :  8 |
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
  NAPtr bufferPtr;
  NATreeIterator partIter;
  size_t partOffset;  // The current byte offset in the referenced part.
  uint8 curBit;      // The current bit number
  NAUInt lineNum;    // The line number, starting with 1 after first line read.
};

struct NABuffer{
  NABufferSource* source;
  NAInt sourceOffset; // Offset of source relative to this buffers
                             // origin. Add this offset to the desired pos to
                             // get the position within the source.
  NAUInt flags;
  NARangei range;

  NANewlineEncoding newlineEncoding;  // The current newline encoding
  NAInt endianness;                   // The current endianness

  NATree parts;             // Tree with all parts in this buffer

  #ifndef NDEBUG
    NAInt iterCount;
  #endif
};

struct NABufferSearchToken{
  NAInt searchOffset;
  NAInt curOffset;
};

typedef struct NABufferTreeNodeData NABufferTreeNodeData;
struct NABufferTreeNodeData{
  size_t len1;
  size_t len2;
};


// NABufferHelper
NA_HAPI void na_EnsureBufferRange(NABuffer* buffer, NAInt start, NAInt end);

// NABufferIteration
NA_HIAPI const NABuffer* na_GetBufferIteratorBufferConst(const NABufferIterator* iter);
NA_HIAPI NABuffer* na_GetBufferIteratorBufferMutable(NABufferIterator* iter);

NA_HAPI NABool na_LocateBufferStart(NABufferIterator* iter);
NA_HAPI NABool na_LocateBufferLastPart(NABufferIterator* iter);
NA_HAPI NABool na_LocateBufferNextPart(NABufferIterator* iter);
NA_HAPI NABool na_LocateBufferPrevPartMax(NABufferIterator* iter);
NA_HAPI NABool na_LocateBufferMax(NABufferIterator* iter);
NA_HAPI NABool na_LocateBufferEnd(NABufferIterator* iter);
NA_HAPI NABool na_IterateBufferPart(NABufferIterator* iter);

NA_HAPI NABuffer* na_GetBufferIteratorSourceBuffer(NABufferIterator* iter);
NA_HIAPI size_t na_GetBufferIteratorPartOffset(NABufferIterator* iter);
NA_HAPI NABool na_IsBufferIteratorSparse(NABufferIterator* iter);
NA_HIAPI NABufferPart* na_GetBufferPart(NABufferIterator* iter);
NA_HAPI void na_PrepareBuffer(NABufferIterator* iter, size_t byteCount);

// NABufferPart
NA_HAPI NABufferPart* na_NewBufferPartSparse(NABufferSource* source, NARangei sourceRange);
NA_HAPI NABufferPart* na_NewBufferPartWithConstData(const void* data, size_t byteSize);
NA_HAPI NABufferPart* na_NewBufferPartWithMutableData(void* data, size_t byteSize, NAMutator destructor);
NA_HAPI void na_SeparateBufferPart(NABufferPart* part);

NA_HIAPI NABufferSource* na_GetBufferPartSource(const NABufferPart* part);
NA_HIAPI size_t na_GetBufferPartByteSize(const NABufferPart* part);
NA_HIAPI NAMemoryBlock* na_GetBufferPartMemoryBlock(const NABufferPart* part);
NA_HIAPI NABool na_IsBufferPartSparse(const NABufferPart* part);

NA_HAPI NABufferPart* na_SplitBufferPart(NATreeIterator* partIter, size_t start, size_t end);
NA_HAPI NABufferPart* na_PrepareBufferPartSourceBuffer(NATreeIterator* partIter, NARangei partrange);
NA_HAPI NABufferPart* na_PrepareBufferPartMemory(NATreeIterator* partIter, NARangei partrange);
NA_HAPI size_t na_PrepareBufferPart(NABufferIterator* iter, size_t byteCount);

NA_HIAPI void na_EnlargeBufferPart(NABufferPart* part, size_t bytesAtStart, size_t bytesAtEnd);
NA_HIAPI size_t na_GetBufferPartRemainingBytes(NABufferIterator* iter);
NA_HIAPI const void* na_GetBufferPartDataPointerConst(NABufferIterator* iter);
NA_HIAPI void* na_GetBufferPartDataPointerMutable(NABufferIterator* iter);

// NABufferSource
NA_HIAPI NABuffer* na_GetBufferSourceUnderlyingBuffer(NABufferSource* source);
NA_HIAPI NABool na_IsBufferSourceLimited(const NABufferSource* source);
NA_HIAPI NARangei na_GetBufferSourceLimit(const NABufferSource* source);
NA_HIAPI void na_FillSourceBuffer(const NABufferSource* source, void* dst, NARangei range);

// NABufferRead and NABufferWrite
NA_HAPI void na_RetrieveBufferBytes(NABufferIterator* iter, void* data, size_t byteSize, NABool advance);
NA_HAPI void na_StoreBufferBytes(NABufferIterator* iter, const void* data, size_t byteSize, NABool prepare, NABool advance);

// NAMemoryBlock
NA_HIAPI NAMemoryBlock* na_NewMemoryBlock(size_t byteSize);
NA_HIAPI NAMemoryBlock* na_NewMemoryBlockWithData(NAPtr data, size_t byteSize, NAMutator destructor);
NA_HIAPI const void* na_GetMemoryBlockDataPointerConst(NAMemoryBlock* block, size_t index);
NA_HIAPI void* na_GetMemoryBlockDataPointerMutable(NAMemoryBlock* block, size_t index);


#include "NABufferHelperII.h"
#include "NABufferIterationII.h"
#include "NABufferParseII.h"
#include "NABufferPartII.h"
#include "NABufferReadII.h"
#include "NABufferSourceII.h"
#include "NABufferWriteII.h"
#include "NAMemoryBlockII.h"



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
