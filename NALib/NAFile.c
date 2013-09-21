
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAFile.h"
#include <stdlib.h>


NAByteArray* naCreateByteArrayFromFilename(NAByteArray* array, const char* filename){
  NAFile file;
  NAFileSize totalsize;
  naFileOpen(&file, filename, NA_FALSE);
  totalsize = naFileComputeSize(&file);
  #ifndef NDEBUG
    if(totalsize > NA_INT32_MAX){
      naError("naCreateByteArrayFromFilename", "Trying to read more than 2 GiB of data from file");
    }
  #endif
  array = naCreateByteArrayWithSize(array, (NAInt)totalsize);
  naFileReadBytes(&file, naGetByteArrayMutablePointer(array), totalsize);
  naFileClose(&file);
  return array;
}



NAString* naCreateStringFromFilename(NAString* string, const char* filename, NATextEncoding encoding){
  NAFile file;
  NAFileSize totalsize;
  naFileOpen(&file, filename, NA_FALSE);
  totalsize = naFileComputeSize(&file);
  #ifndef NDEBUG
    if(totalsize > NA_INT32_MAX){
      naError("naCreateStringFromFilename", "Trying to read more than 2 GiB of data from file");
    }
  #endif
  string = naCreateStringWithSize(string, (NAInt)totalsize);
  naFileReadBytes(&file, naGetByteArrayMutablePointer(&(string->array)), totalsize);
  naFileClose(&file);
  return string;
}



void naFileOpen(NAFile* file, const char* filename, NABool forwriting){
  file->desc = -1;
//  path = filename;
  if(forwriting){
  }else{
    file->desc = naOpen(filename, NA_FILE_READ_MODE, NA_STANDARD_FILE_MODE);
  }
  if(file->desc >= 0){
    file->eof = NA_FALSE;
  }else{
    file->eof = NA_TRUE;
    #ifndef NDEBUG
      naError("naFileOpen", "Could not open file.");
    #endif
  }
//  remainingbytesinbuffer = 0;
//  bufptr = buffer;
  file->pos = 0;
}

void naFileClose(NAFile* file){
  naClose(file->desc);
  file->desc = -1;
}

void naFileReadBytes(NAFile* file, void* buffer, NAFileSize count){
  if(!count){return;}
  // use the rest of the buffer.
//  if(count > remainingbytesinbuffer){
//    memcpy(retbuffer, bufptr, remainingbytesinbuffer);
//    retbuffer = &(((NAByte*)retbuffer)[remainingbytesinbuffer]);
//    count -= remainingbytesinbuffer;
//    remainingbytesinbuffer = 0;
//  }
  // now, if count does not fits into the buffer, read it without the buffer.
//  if(count > NA_FILE_BUFFER_SIZE){
    file->pos += naRead(file->desc, buffer, count);
//  }else{
//    // else, use the buffer.
//    requireBytes(count);
//    if(remainingbytesinbuffer < count){
//      memcpy(retbuffer, bufptr, (size_t)remainingbytesinbuffer);
//      return;
//    }else{
//      memcpy(retbuffer, bufptr, (size_t)count);
//      bufptr += count;
//      remainingbytesinbuffer -= (uint16)count;
//    }
//  }
}


NAFileSize naFileComputeSize(const NAFile* file){
  NAFileSize curpos = naLseek(file->desc, 0, SEEK_CUR);
  NAFileSize filesize = naLseek(file->desc, 0, SEEK_END);
  naLseek(file->desc, curpos, SEEK_SET);
  return filesize;
}



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
