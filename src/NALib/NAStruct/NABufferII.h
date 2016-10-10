
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABuffer.h"



#include "NAMemory.h"
#include "NAList.h"
#include "NACoord.h"

struct NABuffer{
  NAPointer* storage;               // The storage of the whole buffer.
  NAPointer* srcdst;                // The source or dest of the buffer.
  
  NABufInt minpos;                  // The minimal absolute position
  NABufInt maxpos;                  // THe maximal absolute position
  
  NABufInt readpos;                 // The current absolute reading position
  NABufInt writepos;                // The current absolute writing position.
  NAListPos readpartposition;       // The part of the buffer currently reading
  NAListPos writepartposition;      // The part of the buffer currently writing
  uint8 readbit;                    // The current bit number for reading
  uint8 writebit;                   // The current bit number for writing

  NAUInt flags;                     // various flags
  NAInt endianness;                 // The current endianness
  NAEndiannessConverter converter;  // The endianness converter.
};






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
