
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_DEFLATE_INCLUDED
#define NA_DEFLATE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// ZLIB Format specification:
// http://www.ietf.org/rfc/rfc1950.txt
// Deflate algorithm specification:
// http://www.ietf.org/rfc/rfc1951.txt

#include "NABase.h"
#include "NABuffer.h"


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
