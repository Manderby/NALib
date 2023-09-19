
#ifndef NA_DEFLATE_INCLUDED
#define NA_DEFLATE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// ZLIB Format specification:
// http://www.ietf.org/rfc/rfc1950.txt
// Deflate algorithm specification:
// http://www.ietf.org/rfc/rfc1951.txt

#include "../NABase/NABase.h"
#include "../NAStruct/NABuffer.h"


// NA_DEFLATE_COMPRESSION_FASTEST:  No compression at all. Storing bytes as is.
// The other compression levels have not been implemented yet.
typedef enum{
  NA_DEFLATE_COMPRESSION_FASTEST  = 0,
  NA_DEFLATE_COMPRESSION_FAST     = 1,
  NA_DEFLATE_COMPRESSION_DEFAULT  = 2,
  NA_DEFLATE_COMPRESSION_MAX      = 3
} NADeflateCompressionLevel;


// Decompresses a bit stream which is encoded in the ZLIB format and stores it
// at the current writing position in the given buffer.
NA_API void naFillBufferWithZLIBDecompression(NABuffer* output,
                                              NABuffer* input);

// Compresses a bit stream with ZLIB and stores it at the current writing
// position in the given buffer. The compression level denotes, how strong
// the compression shall be.
NA_API void naFillBufferWithZLIBCompression(  NABuffer* output,
                                              NABuffer* input,
                              NADeflateCompressionLevel level);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_DEFLATE_INCLUDED



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
