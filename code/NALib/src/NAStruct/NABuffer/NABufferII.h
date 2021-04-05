
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABuffer.h"

// How NABuffers work internally:
// 
// Consider the following C-like string written in source code and how
// we easily turn that into an NABuffer:
//
// const NAUTF8Char* myString = "This is an exciting trend";
// NABuffer* buf0 = naNewBufferWithConstData(myString, naStrlen(myString));
//
// This results in the following structure:
//
//                 +==== BUFFER PART =+
//                 | blockOffset :  0 |    +====== MEMORY BLOCK =======+
// NABuffer buf0 = | byteSize    : 25 |    |     referenceCount: 1     |
//                 | memBlock ------------>| This is an exciting trend |
//                 +==================+    +===========================+
// 
// So, a simple buffer just contains one part which references one memory
// block which in this case contains the constant string as a pointer.
//
// For simplicity, here, the string is directly shown in the memory block but
// actually, the data is stored as a pointer. In this case, the memory block
// also remembers that this was a const char array and therefore will NOT try
// to deallocate it if it is no longer in use.
// 
// Buffers can be far more than just a simple storage. They are implemented
// in a highly granular way. Each buffer is a series of buffer parts stitched
// together.
// 
// Consider the following three sentences as buffers:
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
// Let's look at a single buffer part in more detail: Imagine the buffer part
// containing the word "everywhere". Let's assume, it looks like this:
//
// +==== BUFFER PART ====+
// | blockOffset :  0    |       +==== MEMORY BLOCK ====+
// | byteSize    : 10    |       |   referenceCount: 1  |
// | memBlock ---------------->  |      everywhere      |
// +=====================+       +======================+
//
// As seen in the topmost example, the memory block not only stores the word
// "everywhere" but it also knows how many buffer parts need the memory block
// to exist.
//
// A buffer part is referencing such a memory block. It also defines which
// bytes of that memory block are used. In this case 10 bytes starting with
// index 0.
//
// Now, consider the words "very" and "her" of above examples. These strings
// can of course exist in their own memory block, each with a reference count
// of 1. But consider now that we want to save some memory. Both of the words
// "very" and "her" are substrings of the word "everywhere" (e-VERY-w-HER-e).
// Therefore, we can just reference that one memory block and the buffer parts
// then look like this:
//
// +==== BUFFER PART ====+
// | blockOffset :  1    |
// | byteSize    :  4    |
// | memBlock --------------\
// +=====================+   \
//                            -->  +==== MEMORY BLOCK ====+
//                                 |   referenceCount: 3  |
// +==== BUFFER PART ====+         |      everywhere      |
// | blockOffset :  6    |    -->  +======================+
// | byteSize    :  3    |   /
// | memBlock --------------/
// +=====================+
//
// As you can see, the buffer parts share the same memory block which now has
// a reference count of 3. It is very important to understand that it is the
// referenced memery blocks which are shared amongst NABuffers, NOT the parts!
//
// Now, let's go back to buf1, and see in full detail, how it is stiched
// together:
//
//                 +=== BUFFER PART ==+=== BUFFER PART ==+=== BUFFER PART ==+
// NABuffer buf1 = |     This is      |       very       |     exciting.    |
//                 | blockOffset :  0 | blockOffset :  1 | blockOffset : 12 |
//                 | byteSize    :  7 | byteSize    :  4 | byteSize    :  8 |
//             +---- memBlock         | memBlock  --+    | memBlock  --+    |
//             |   +------------------+-------------|----+-------------|----+
//             |                                    |                  |
//             |      +==== MEMORY BLOCK ====+      |                  |
//             |      |   referenceCount: 3  |      |                  |
//             |      |      everywhere      | <----+                  |
//             |      +======================+                         |
//             |                                                       |
//             |            +======= MEMORY BLOCK ========+            |
//             |            |     referenceCount: 3       |            |
//             +----------> |  This is an exciting trend  | <----------+
//                          +=============================+
//
// So it becomes clear that the content of buf1 is comprised of three parts
// "This is", "very" and "exciting" which in this case come from two sources.
// 
// A source can for for example be an existing array in memory, a file, or
// another NABuffer. You can create custom sources with naNewBufferSource and
// the corresponding buffers by calling naNewBufferWithCustomSource.
//
// As we have seen, buf0 was a constant string. Imagine, buf3 being read from
// a file. In NALib, the file content stays on disk as long as it is not
// explicitely used. Therefore at the beginning when first creating buf3,
// it looks something like this:
//
//        +==================== BUFFER PART ============================+
// buf3 = | blockOffset:  0                                             |
//        | byteSize:    37                                             |
//        | memBlock:  NULL                                             |
//        +-------------------------------------------------------------+
//
// That single buffer part knows that the file content has in total 37 bytes
// but it has not yet referenced any memory. This is called a "sparse part".
// When reading the file, such sparse parts will subsequentially be turned
// into filled parts. For example when reading the file byte by byte, it
// might look somewhat like this:
//
//        +======================== BUFFER PART ========================+
//   |    |                            NULL                             |
//   |    |-------------------------------------------------------------|
//   |
//   |    +== BUFFER PART ==+   +============= BUFFER PART =============+
//   |    | You can see ads |   |                 NULL                  |
//   |    |-----------------+   +---------------------------------------+
//   |
//   |    +== BUFFER PART ==+   +== BUFFER PART ==+   +== BUFFER PART ==+
//   |    | You can see ads |   |    everywhere   |   |      NULL       |
//   |    |-----------------+   +-----------------+   +-----------------+
//   |
//   |    +== BUFFER PART ==+   +== BUFFER PART ==+   +== BUFFER PART ==+
//   V    | You can see ads |   |    everywhere   |   |   these days    |
//        +-----------------+   +-----------------+   +-----------------+
//
// The obvious advantage of that is that memory is only allocated when really
// needed. This also works when for example only the first few bytes of a
// large file need to be read and the last few bytes. Therefore, the middle
// buffer part is sparse and in our little example, look like this:
//
//        +== BUFFER PART ==+   +== BUFFER PART ==+   +== BUFFER PART ==+
//        | You can see ads |   |      NULL       |   |   these days    |
//        +-----------------+   +-----------------+   +-----------------+
//
// Now, as good as the advantages sound, this poses a problem. Imagine, the
// example buf3 has not yet been read from the disk, so the buffer is
// completely sparse. Buffers buf1 and buf2 though with the words "very" and
// "her" require the word "everywhere" to be somewhere in memory.
//
// NALib solves this problem by remembering for every sparse buffer part (in
// this case the two parts wich want to reference "very" and "her"), which
// bytes from which source should be stored in that part. And whenever needed,
// the sparse part will "ask" the source to provide the desired content.
// The source itself manages its own buffer and will look for the desired
// content. If the source buffer does not have that content available, it is
// freshly read from disk into a new memory block which in turn will then be
// referenced both by the source buffer as well as the buffer which requested
// the content.
//
// Therefore, sources also have a reference counter which store how many
// sparse parts still require the source to be active in case they need the
// content.


typedef struct NABufferPart NABufferPart;
typedef struct NAMemoryBlock NAMemoryBlock;
typedef struct NABufferSearchToken NABufferSearchToken;


#include "../../NATree.h"


struct NABuffer{
  NABufferSource* source;
  NAInt sourceOffset; // Offset of source relative to this buffers
                             // origin. Add this offset to the desired pos to
                             // get the position within the source.
  uint32 flags;
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



// NAMemoryBlock
NA_HAPI NAMemoryBlock* na_NewMemoryBlock(size_t byteSize);
NA_HAPI NAMemoryBlock* na_NewMemoryBlockWithData(NAPtr data, size_t byteSize, NAMutator destructor);
NA_HIAPI const void* na_GetMemoryBlockDataPointerConst(NAMemoryBlock* block, size_t index);
NA_HIAPI void* na_GetMemoryBlockDataPointerMutable(NAMemoryBlock* block, size_t index);



// NABufferSource
NA_HIAPI NABool na_HasBufferSourceCache(NABufferSource* source);
NA_HIAPI NABuffer* na_GetBufferSourceCache(NABufferSource* source);
NA_HIAPI NABool na_HasBufferSourceLimit(const NABufferSource* source);
NA_HIAPI NARangei na_GetBufferSourceLimit(const NABufferSource* source);
NA_HIAPI void na_FillSourceBuffer(const NABufferSource* source, void* dst, NARangei range);



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
NA_HIAPI NAInt na_GetBufferIteratorPartOffset(NABufferIterator* iter);
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

// NABufferRead and NABufferWrite
NA_HAPI void na_RetrieveBufferBytes(NABufferIterator* iter, void* data, size_t byteSize, NABool advance);
NA_HAPI void na_StoreBufferBytes(NABufferIterator* iter, const void* data, size_t byteSize, NABool prepare, NABool advance);



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
