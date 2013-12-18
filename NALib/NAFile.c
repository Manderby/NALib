
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAFile.h"
#include <stdlib.h>



NAString* naCreateStringFromFileContents(NAString* string, const char* filename, NATextEncoding encoding){
  NAFile file;
  NAFileSize totalsize;
  file = naOpenFileForReading(filename);
  naSetFileTextEncoding(&file, encoding);
  totalsize = naComputeFileSize(&file);
  #ifndef NDEBUG
    if(totalsize > NA_INT32_MAX){
      naError("naCreateStringFromFileContents", "Trying to read more than 2 GiB of data from file");
    }
  #endif
  // todo: encoding.
  string = naCreateStringWithSize(string, (NAInt)totalsize);
  naRead(file.desc, naGetStringMutableUTF8Pointer(string), totalsize);
  naCloseFile(&file);
  return string;
}



NAByteArray* naCreateByteArrayFromFileContents(NAByteArray* bytearray, const char* filename){
  NAFile file;
  NAFileSize totalsize;
  file = naOpenFileForReading(filename);
  totalsize = naComputeFileSize(&file);
  #ifndef NDEBUG
    if(totalsize > NA_INT32_MAX){
      naError("naCreateByteArrayFromFileContents", "Trying to read more than 2 GiB of data from file");
    }
  #endif
  bytearray = naCreateByteArrayFromFile(bytearray, &file, totalsize);
  naCloseFile(&file);
  return bytearray;
}




NAFile naOpenFileForReading(const char* filename){
  NAFile file;
  file.desc = naOpen(filename, NA_FILE_OPEN_FLAGS_READ, NA_FILE_MODE_DEFAULT);
  file.pos = 0;
  file.converter = naMakeEndiannessConverter(NA_ENDIANNESS_NATIVE, NA_ENDIANNESS_NATIVE);
  file.textencoding = NA_ENCODING_UTF_8;
  file.flags = NA_FILE_FLAG_NEWLINE_NATIVE;
  if(file.desc < 0){
    file.flags |= NA_FILE_FLAG_EOF;
    #ifndef NDEBUG
      naError("naOpenFileForReading", "Could not open file.");
    #endif
  }
  file.remainingbytesinbuffer = 0;  // the buffer is empty at the moment.
  file.bufptr = file.buffer;
  return file;
}


NAFile naOpenFileForWriting(const char* filename, NAFileMode mode){
  NAFile file;
  file.desc = naOpen(filename, NA_FILE_OPEN_FLAGS_WRITE, mode);
  file.pos = 0;
  file.converter = naMakeEndiannessConverter(NA_ENDIANNESS_NATIVE, NA_ENDIANNESS_NATIVE);
  file.textencoding = NA_ENCODING_UTF_8;
  file.flags = NA_FILE_FLAG_WRITING | NA_FILE_FLAG_NEWLINE_NATIVE;
  if(file.desc < 0){
    file.flags |= NA_FILE_FLAG_EOF;
    #ifndef NDEBUG
      naError("naOpenFileForWriting", "Could not create file.");
    #endif
  }
  file.remainingbytesinbuffer = NA_FILE_BUFFER_SIZE;
  file.bufptr = file.buffer;
  return file;
}


NAFile naOpenFileForAppending(const char* filename, NAFileMode mode){
  NAFile file;
  file.desc = naOpen(filename, NA_FILE_OPEN_FLAGS_APPEND, mode);
  file.pos = 0;
  file.textencoding = NA_ENCODING_UTF_8;
  file.flags = NA_FILE_FLAG_WRITING | NA_FILE_FLAG_NEWLINE_NATIVE | NA_FILE_FLAG_AUTOFLUSH_TEXT;
  if(file.desc < 0){
    file.flags |= NA_FILE_FLAG_EOF;
    #ifndef NDEBUG
      naError("naOpenFileForAppending", "Could not create file.");
    #endif
  }
  file.remainingbytesinbuffer = NA_FILE_BUFFER_SIZE;
  file.bufptr = file.buffer;
  return file;
}


NAFile naMakeFileAsStdin(){
  NAFile file;
  file.desc = 0;
  file.pos = 0;
  file.converter = naMakeEndiannessConverter(NA_ENDIANNESS_NATIVE, NA_ENDIANNESS_NATIVE);
  file.textencoding = NA_ENCODING_UTF_8;
  file.flags = NA_FILE_FLAG_STREAM | NA_FILE_FLAG_NEWLINE_NATIVE | NA_FILE_FLAG_AUTOFLUSH_ALL;
  file.remainingbytesinbuffer = 0;  // the buffer is empty at the moment.
  file.bufptr = file.buffer;
  return file;
}


NAFile naMakeFileAsStdout(){
  NAFile file;
  file.desc = 1;
  file.pos = 0;
  file.converter = naMakeEndiannessConverter(NA_ENDIANNESS_NATIVE, NA_ENDIANNESS_NATIVE);
  file.textencoding = NA_ENCODING_UTF_8;
  file.flags = NA_FILE_FLAG_WRITING | NA_FILE_FLAG_STREAM | NA_FILE_FLAG_NEWLINE_NATIVE | NA_FILE_FLAG_AUTOFLUSH_ALL;
  file.remainingbytesinbuffer = NA_FILE_BUFFER_SIZE;
  file.bufptr = file.buffer;
  return file;
}


NAFile naMakeFileAsStderr(){
  NAFile file;
  file.desc = 2;
  file.pos = 0;
  file.converter = naMakeEndiannessConverter(NA_ENDIANNESS_NATIVE, NA_ENDIANNESS_NATIVE);
  file.textencoding = NA_ENCODING_UTF_8;
  file.flags = NA_FILE_FLAG_WRITING | NA_FILE_FLAG_STREAM | NA_FILE_FLAG_NEWLINE_NATIVE | NA_FILE_FLAG_AUTOFLUSH_ALL;
  file.remainingbytesinbuffer = NA_FILE_BUFFER_SIZE;
  file.bufptr = file.buffer;
  return file;
}


void naCloseFile(NAFile* file){
  if(file->flags & NA_FILE_FLAG_WRITING){naFlushFileBuffer(file);}
  // Note that stdin, stdout and stderr can be closed as well.
  naClose(file->desc);
  file->desc = -1;
}


void naFlushFileBuffer(NAFile* file){
  if(NA_FILE_BUFFER_SIZE - file->remainingbytesinbuffer){
    naWrite(file->desc, file->buffer, NA_FILE_BUFFER_SIZE - file->remainingbytesinbuffer);
    file->bufptr = file->buffer;
    file->remainingbytesinbuffer = NA_FILE_BUFFER_SIZE;
  }
}


void naSetFileTextEncoding(NAFile* file, NATextEncoding textencoding){
  file->textencoding = textencoding;
}


void naSetFileEndianness(NAFile* file, NAInt endianness){
  file->converter = naMakeEndiannessConverter(endianness, NA_ENDIANNESS_NATIVE);
}


void naSetFileAutoFlush(NAFile* file, uint16 autoflushflag){
  // todo: Only useful for writing files
  file->flags &= ~NA_FILE_FLAG_AUTOFLUSH_MASK;
  file->flags |= (autoflushflag & NA_FILE_FLAG_AUTOFLUSH_MASK);
}


void naSetFileNewLine(NAFile* file, uint16 newlineflag){
  // todo: Only useful for writing files
  file->flags &= ~NA_FILE_FLAG_NEWLINE_MASK;
  file->flags |= (newlineflag & NA_FILE_FLAG_NEWLINE_MASK);
}


NAFileSize naComputeFileSize(const NAFile* file){
  // todo: only useful for non-streams
  NAFileSize curpos = naLseek(file->desc, 0, SEEK_CUR);
  NAFileSize filesize = naLseek(file->desc, 0, SEEK_END);
  naLseek(file->desc, curpos, SEEK_SET);
  return filesize;
}


NABool naIsFileOpen(const NAFile* file){
  return (file->desc >= 0);
}


NABool naHasFileEnded(const NAFile* file){
  // todo: only partially useful for non-streams
  return file->flags & NA_FILE_FLAG_EOF;
}


void naJumpFileOffsetAbsolute(NAFile* file, NAFileSize offset){
  // todo: only useful for non-streams
  naLseek(file->desc, offset, SEEK_SET);
  file->pos = offset;
  // The buffer must be flushed.
  file->bufptr = file->buffer;
  file->remainingbytesinbuffer = 0;
  // todo: the buffer might be reused if the offset is near the current position
  // todo: flush buffer on write.
}

void naJumpFileOffsetRelative(NAFile* file, NAFileSize offset){
  // todo: only useful for non-streams
  naJumpFileOffsetAbsolute(file, file->pos - file->remainingbytesinbuffer + offset);
}




// Helper function not visible to the programmer. Will make sure that there
// are enough bytes in the buffer. Note that this function only works for
// counts in uint16 range.
// This function is hidden to the user as it is just a supplementary helper
// function which prepares the NAFile struct internally for an appropriate
// action. Therefore the user shall not use it. Of course, in NALib hiding
// something has not much use but it shows where the hidden attribute makes
// sense.
NA_IDEF NA_HIDDEN void naRequireFileReadBufferBytes(NAFile* file, uint16 count){
  // Declaration before implementation. Needed for C90.
  NAFileSize bytesread;
  if(file->remainingbytesinbuffer >= count){return;}  // enough bytes available
  // copy the remaining bytes to the beginning of the buffer.
  if(file->remainingbytesinbuffer){
    naCpyn(file->buffer, file->bufptr, file->remainingbytesinbuffer);
  }
  // Place the bufferpointer right after these bytes...
  file->bufptr = file->buffer + file->remainingbytesinbuffer;
  // ... and refill the remaining buffer
  bytesread = naRead(file->desc, file->bufptr, NA_FILE_BUFFER_SIZE - file->remainingbytesinbuffer);
  // adjust all internal counters to reflect the new state.
  file->remainingbytesinbuffer += (uint16)bytesread;
  file->pos += bytesread;
  file->bufptr = file->buffer;
  if(file->remainingbytesinbuffer == 0){file->flags |= NA_FILE_FLAG_EOF;}
}



void naReadFileBytes(NAFile* file, void* buf, NAFileSize count){
  // todo make checks for write files.
  if(!count){return;}

  // use the rest of the buffer, if available.
  if((file->remainingbytesinbuffer) && (count > file->remainingbytesinbuffer)){
    naCpyn(buf, file->bufptr, file->remainingbytesinbuffer);
    buf = ((NAByte*)buf) + file->remainingbytesinbuffer;
    count -= file->remainingbytesinbuffer;
    file->remainingbytesinbuffer = 0;
  }
  // now, if count does not fits into the buffer, read it without the buffer.
  if(count > NA_FILE_BUFFER_SIZE){
    file->pos += naRead(file->desc, buf, (NAInt)count);
  }else{
    // else, use the buffer.
    naRequireFileReadBufferBytes(file, (uint16)count);
    if(file->remainingbytesinbuffer < count){
      // The file must have ended and has less bytes stored than needed.
      naCpyn(buf, file->bufptr, (size_t)file->remainingbytesinbuffer);
      file->remainingbytesinbuffer = 0;
    }else{
      naCpyn(buf, file->bufptr, (size_t)count);
      file->bufptr += count;
      file->remainingbytesinbuffer -= (uint16)count;
    }
  }
}




int8 naReadFileInt8(NAFile* file){
  int8 value;
  naRequireFileReadBufferBytes(file, 1);
  if(naHasFileEnded(file)){return 0;}
  value = *((int8*)(file->bufptr));
  file->converter.convert8(&value);
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  return value;
}
int16 naReadFileInt16(NAFile* file){
  int16 value;
  naRequireFileReadBufferBytes(file, 2);
  if(naHasFileEnded(file)){return 0;}
  value = *((int16*)(file->bufptr));
  file->converter.convert16(&value);
  file->bufptr += 2;
  file->remainingbytesinbuffer -= 2;
  return value;
}
int32 naReadFileInt32(NAFile* file){
  int32 value;
  naRequireFileReadBufferBytes(file, 4);
  if(naHasFileEnded(file)){return 0;}
  value = *((int32*)(file->bufptr));
  file->converter.convert32(&value);
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  return value;
}
int64 naReadFileInt64(NAFile* file){
  int64 value;
  naRequireFileReadBufferBytes(file, 8);
  if(naHasFileEnded(file)){return 0;}
  value = *((int64*)(file->bufptr));
  file->converter.convert64(&value);
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  return value;
}


uint8 naReadFileUInt8(NAFile* file){
  uint8 value;
  naRequireFileReadBufferBytes(file, 1);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint8*)(file->bufptr));
  file->converter.convert8(&value);
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  return value;
}
uint16 naReadFileUInt16(NAFile* file){
  uint16 value;
  naRequireFileReadBufferBytes(file, 2);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint16*)(file->bufptr));
  file->converter.convert16(&value);
  file->bufptr += 2;
  file->remainingbytesinbuffer -= 2;
  return value;
}
uint32 naReadFileUInt32(NAFile* file){
  uint32 value;
  naRequireFileReadBufferBytes(file, 4);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint32*)(file->bufptr));
  file->converter.convert32(&value);
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  return value;
}
uint64 naReadFileUInt64(NAFile* file){
  uint64 value;
  naRequireFileReadBufferBytes(file, 8);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint64*)(file->bufptr));
  file->converter.convert64(&value);
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  return value;
}


float naReadFileFloat(NAFile* file){
  float value;
  naRequireFileReadBufferBytes(file, 4);
  if(naHasFileEnded(file)){return 0.f;}
  value = *((float*)(file->bufptr));
  file->converter.convert32(&value);
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  return value;
}
double naReadFileDouble(NAFile* file){
  double value;
  naRequireFileReadBufferBytes(file, 8);
  if(naHasFileEnded(file)){return 0.;}
  value = *((double*)(file->bufptr));
  file->converter.convert64(&value);
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  return value;
}



NAByteArray* naCreateByteArrayFromFile(NAByteArray* bytearray, NAFile* file, NAFileSize count){
  bytearray = naCreateByteArrayWithSize(bytearray, (NAInt)count);
  naReadFileBytes(file, naGetByteArrayMutablePointer(bytearray), count);
  return bytearray;
}


NAString* naCreateStringFromFile(NAString* string, NAFile* file, NAFileSize bytecount){
  string = naCreateStringWithSize(string, (NAInt)bytecount);
  naReadFileBytes(file, naGetStringMutableUTF8Pointer(string), bytecount);
  return string;
  // todo: encoding.
}








// Helper function not visible to the programmer. Will make sure that there
// are enough bytes in the buffer. Note that this function only works for
// counts in uint16 range.
// This function is hidden to the user as it is just a supplementary helper
// function which prepares the NAFile struct internally for an appropriate
// action. Therefore the user shall not use it. Of course, in NALib hiding
// something has not much use but it shows where the hidden attribute makes
// sense.
NA_IDEF NA_HIDDEN void naRequireFileWriteBufferBytes(NAFile* file, uint16 count){
  if(file->remainingbytesinbuffer >= count){
    return;
  }else{
    naFlushFileBuffer(file);
  }
}



void naWriteFileBytes(NAFile* file, const void* ptr, NAFileSize count){
  // todo make checks for read files.
  if(!count){return;}
  if( ((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) >= NA_FILE_FLAG_AUTOFLUSH_MULTIBYTE)
      || (count >= NA_FILE_BUFFER_SIZE)){
    naFlushFileBuffer(file);
    naWrite(file->desc, ptr, count);
  }else{
    naRequireFileWriteBufferBytes(file, (uint16)count);
    naCpyn(file->bufptr, ptr, (size_t)count);
    file->bufptr += count;
    file->remainingbytesinbuffer -= (uint16)count;
  }
}


void naWriteFileInt8(NAFile* file, int8 value){
  naRequireFileWriteBufferBytes(file, 1);
  file->converter.convert8(&value);
  *((int8*)file->bufptr) = value;
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}
void naWriteFileInt16(NAFile* file, int16 value){
  naRequireFileWriteBufferBytes(file, 2);
  file->converter.convert16(&value);
  *((int16*)file->bufptr) = value;
  file->bufptr += 2;
  file->remainingbytesinbuffer -= 2;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}
void naWriteFileInt32(NAFile* file, int32 value){
  naRequireFileWriteBufferBytes(file, 4);
  file->converter.convert32(&value);
  *((int32*)file->bufptr) = value;
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}
void naWriteFileInt64(NAFile* file, int64 value){
  naRequireFileWriteBufferBytes(file, 8);
  file->converter.convert64(&value);
  *((int64*)file->bufptr) = value;
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}


void naWriteFileUInt8(NAFile* file, uint8 value){
  naRequireFileWriteBufferBytes(file, 1);
  file->converter.convert8(&value);
  *((uint8*)file->bufptr) = value;
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}
void naWriteFileUInt16(NAFile* file, uint16 value){
  naRequireFileWriteBufferBytes(file, 2);
  file->converter.convert16(&value);
  *((uint16*)file->bufptr) = value;
  file->bufptr += 2;
  file->remainingbytesinbuffer -= 2;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}
void naWriteFileUInt32(NAFile* file, uint32 value){
  naRequireFileWriteBufferBytes(file, 4);
  file->converter.convert32(&value);
  *((uint32*)file->bufptr) = value;
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}
void naWriteFileUInt64(NAFile* file, uint64 value){
  naRequireFileWriteBufferBytes(file, 8);
  file->converter.convert64(&value);
  *((uint64*)file->bufptr) = value;
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}


void naWriteFileFloat(NAFile* file, float value){
  naRequireFileWriteBufferBytes(file, 4);
  file->converter.convert32(&value);
  *((float*)file->bufptr) = value;
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}
void naWriteFileDouble(NAFile* file, double value){
  naRequireFileWriteBufferBytes(file, 8);
  file->converter.convert64(&value);
  *((double*)file->bufptr) = value;
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}


void naWriteFileByteArray(NAFile* file, const NAByteArray* array){
  naWriteFileBytes(file, naGetByteArrayConstPointer(array), naGetByteArraySize(array));
}



void naWriteFileTab(NAFile* file){
  naRequireFileWriteBufferBytes(file, 1);
  *((NAUTF8Char*)file->bufptr) = '\t';
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) >= NA_FILE_FLAG_AUTOFLUSH_TEXT){
    naFlushFileBuffer(file);
  }
}


void naWriteFileNewLine(NAFile* file){
  switch(file->flags & NA_FILE_FLAG_NEWLINE_MASK){
  case NA_FILE_FLAG_NEWLINE_MAC9:
    naRequireFileWriteBufferBytes(file, 1);
    *((NAUTF8Char*)file->bufptr) = '\r';
    file->bufptr += 1;
    file->remainingbytesinbuffer -= 1;
    break;
  case NA_FILE_FLAG_NEWLINE_UNIX:
    naRequireFileWriteBufferBytes(file, 1);
    *((NAUTF8Char*)file->bufptr) = '\n';
    file->bufptr += 1;
    file->remainingbytesinbuffer -= 1;
    break;
  case NA_FILE_FLAG_NEWLINE_WIN:
    naRequireFileWriteBufferBytes(file, 2);
    *((NAUTF8Char*)file->bufptr) = '\r';
    file->bufptr += 1;
    file->remainingbytesinbuffer -= 1;
    *((NAUTF8Char*)file->bufptr) = '\n';
    file->bufptr += 1;
    file->remainingbytesinbuffer -= 1;
    break;
  }
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) >= NA_FILE_FLAG_AUTOFLUSH_TEXT){
    naFlushFileBuffer(file);
  }
}


void naWriteFileString(NAFile* file, const NAString* string){
  naWriteFileBytes(file, naGetStringConstUTF8Pointer(string), naGetStringSize(string));
  // todo: encoding.
}


void naWriteFileStringWithFormat(NAFile* file,
                       const NAUTF8Char* format,
                                         ...){
  va_list argumentlist;
  va_start(argumentlist, format);
  naWriteFileStringWithArguments(file, format, argumentlist);
  va_end(argumentlist);
  // todo: encoding.
}

void naWriteFileStringWithArguments(NAFile* file,
                          const NAUTF8Char* format,
                                    va_list argumentlist){
  NAString string;
  naCreateStringWithArguments(&string, format, argumentlist);
  naWriteFileString(file, &string);
  naClearString(&string);
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) >= NA_FILE_FLAG_AUTOFLUSH_TEXT){
    naFlushFileBuffer(file);
  }
  // todo: encoding.
}



void naWriteFileLine(             NAFile* file,
                          const NAString* string){
  naWriteFileString(file, string);
  naWriteFileNewLine(file);
}
void naWriteFileLineWithFormat(   NAFile* file,
                        const NAUTF8Char* format,
                                          ...){
  va_list argumentlist;
  va_start(argumentlist, format);
  naWriteFileLineWithArguments(file, format, argumentlist);
  va_end(argumentlist);
}
void naWriteFileLineWithArguments(NAFile* file,
                        const NAUTF8Char* format,
                                  va_list argumentlist){
  naWriteFileStringWithArguments(file, format, argumentlist);
  naWriteFileNewLine(file);
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
