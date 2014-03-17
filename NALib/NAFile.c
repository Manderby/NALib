
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAFile.h"
#include <stdlib.h>


// Note that some of the following flags reflect the enum-types found in
// NAFile.h and NAString.h. They are redefined here as macro flags as a
// programmer might want to hide the actual implementation of something like
// flags. An API on the other hand is much easier to use with an enum type.
// The user of such an API makes less errors, the compiler can check for
// errors and the debugger can print useful information.

#define NA_FILE_FLAG_EOF                  0x01
#define NA_FILE_FLAG_WRITING              0x02
#define NA_FILE_FLAG_STREAM               0x04

#define NA_FILE_FLAG_NEWLINE_MASK         0x30
#define NA_FILE_FLAG_NEWLINE_UNIX         0x10
#define NA_FILE_FLAG_NEWLINE_MAC9         0x20
#define NA_FILE_FLAG_NEWLINE_WIN          0x30
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #define NA_FILE_FLAG_NEWLINE_NATIVE NA_FILE_FLAG_NEWLINE_WIN
#else
  #define NA_FILE_FLAG_NEWLINE_NATIVE NA_FILE_FLAG_NEWLINE_UNIX
#endif

#define NA_FILE_FLAG_AUTOFLUSH_MASK       0xc0
#define NA_FILE_FLAG_AUTOFLUSH_NONE       0x00
#define NA_FILE_FLAG_AUTOFLUSH_TEXT       0x40
#define NA_FILE_FLAG_AUTOFLUSH_MULTIBYTE  0x80
#define NA_FILE_FLAG_AUTOFLUSH_ALL        0xc0



NAString* naCreateStringFromFileContents(NAString* string, const char* filename, NATextEncoding encoding){
  NAFile file;
  NAFileSize totalsize;
  file = naOpenFileForReading(filename);
  naSetFileTextEncoding(&file, encoding);
  totalsize = naComputeFileSize(&file);
  #ifndef NDEBUG
    if(totalsize > NA_INT32_MAX)
      naError("naCreateStringFromFileContents", "Trying to read more than 2 GiB of data from file");
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
    if(totalsize > NA_INT32_MAX)
      naError("naCreateByteArrayFromFileContents", "Trying to read more than 2 GiB of data from file");
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
  file.textencoding = NA_TEXT_ENCODING_UTF_8;
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
  file.textencoding = NA_TEXT_ENCODING_UTF_8;
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
  file.textencoding = NA_TEXT_ENCODING_UTF_8;
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
  file.textencoding = NA_TEXT_ENCODING_UTF_8;
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
  file.textencoding = NA_TEXT_ENCODING_UTF_8;
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
  file.textencoding = NA_TEXT_ENCODING_UTF_8;
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naFlushFileBuffer", "Flushing buffer of read-file. Undefined behaviour.");
  #endif
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

void naSetFileAutoFlush(NAFile* file, NAFileAutoFlushing autoflushing){
  uint16 autoflushflag;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naSetFileAutoFlush", "Setting the flushing behaviour is only useful for write-files.");
  #endif
  file->flags &= ~NA_FILE_FLAG_AUTOFLUSH_MASK;
  switch(autoflushing){
  case NA_FILE_AUTOFLUSH_NONE:      autoflushflag = NA_FILE_FLAG_AUTOFLUSH_NONE;      break;
  case NA_FILE_AUTOFLUSH_TEXT:      autoflushflag = NA_FILE_FLAG_AUTOFLUSH_TEXT;      break;
  case NA_FILE_AUTOFLUSH_MULTIBYTE: autoflushflag = NA_FILE_FLAG_AUTOFLUSH_MULTIBYTE; break;
  case NA_FILE_AUTOFLUSH_ALL:       autoflushflag = NA_FILE_FLAG_AUTOFLUSH_ALL;       break;
  }
  file->flags |= autoflushflag;
}


void naSetFileNewLine(NAFile* file, NANewlineEncoding newlineencoding){
  uint16 newlineflag;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naSetFileNewLine", "Setting the newline encoding is only useful for write-files.");
  #endif
  file->flags &= ~NA_FILE_FLAG_NEWLINE_MASK;
  switch(newlineencoding){
  case NA_NEWLINE_UNIX:   newlineflag = NA_FILE_FLAG_NEWLINE_UNIX;   break;
  case NA_NEWLINE_MAC9:   newlineflag = NA_FILE_FLAG_NEWLINE_MAC9;   break;
  case NA_NEWLINE_WIN:    newlineflag = NA_FILE_FLAG_NEWLINE_WIN;    break;
  case NA_NEWLINE_NATIVE: newlineflag = NA_FILE_FLAG_NEWLINE_NATIVE; break;
  }
  file->flags |= newlineflag;
}


NAFileSize naComputeFileSize(const NAFile* file){
  // Declaration before implementation. Needed for C90.
  NAFileSize curpos;
  NAFileSize filesize;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_STREAM))
      naError("naComputeFileSize", "Computing the file-size not defined for streams. Undefined behaviour.");
  #endif
  curpos = naLseek(file->desc, 0, SEEK_CUR);
  filesize = naLseek(file->desc, 0, SEEK_END);
  naLseek(file->desc, curpos, SEEK_SET);
  return filesize;
}


NABool naIsFileOpen(const NAFile* file){
  return (file->desc >= 0);
}


NABool naHasFileEnded(const NAFile* file){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_STREAM))
      naError("naHasFileEnded", "End of file not defined in stream. Undefined behaviour.");
  #endif
  return file->flags & NA_FILE_FLAG_EOF;
}


void naJumpFileOffsetAbsolute(NAFile* file, NAFileSize offset){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_STREAM))
      naError("naJumpFileOffsetAbsolute", "Jumping not defined for streams. Undefined behaviour.");
  #endif
  if((file->flags & NA_FILE_FLAG_WRITING)){naFlushFileBuffer(file);}
  naLseek(file->desc, offset, SEEK_SET);
  file->pos = offset;
  // The buffer must be flushed.
  file->bufptr = file->buffer;
  file->remainingbytesinbuffer = 0;
  // todo: the buffer might be reused if the offset is near the current position
}

void naJumpFileOffsetRelative(NAFile* file, NAFileSize offset){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_STREAM))
      naError("naJumpFileOffsetRelative", "Jumping not defined for streams. Undefined behaviour.");
  #endif
  naJumpFileOffsetAbsolute(file, file->pos - file->remainingbytesinbuffer + offset);
}




// Helper function not visible to the programmer. Will make sure that there
// are enough bytes in the buffer. Note that this function only works for
// counts in uint16 range.
// This function should be hidden to the user as it is just a supplementary
// helper function which prepares the NAFile struct internally for an
// appropriate action. Therefore the user shall not use it. Of course, in
// NALib hiding something has not much use but it shows where the hidden
// attribute makes sense.
NA_HLP void naRequireFileReadBufferBytes(NAFile* file, uint16 count){
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
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileBytes", "File is not a read-file.");
  #endif
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
  // Declaration before implementation. Needed for C90.
  int8 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileInt8", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 1);
  if(naHasFileEnded(file)){return 0;}
  value = *((int8*)(file->bufptr));
  file->converter.convert8(&value);
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  return value;
}
int16 naReadFileInt16(NAFile* file){
  // Declaration before implementation. Needed for C90.
  int16 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileInt16", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 2);
  if(naHasFileEnded(file)){return 0;}
  value = *((int16*)(file->bufptr));
  file->converter.convert16(&value);
  file->bufptr += 2;
  file->remainingbytesinbuffer -= 2;
  return value;
}
int32 naReadFileInt32(NAFile* file){
  // Declaration before implementation. Needed for C90.
  int32 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileInt32", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 4);
  if(naHasFileEnded(file)){return 0;}
  value = *((int32*)(file->bufptr));
  file->converter.convert32(&value);
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  return value;
}
int64 naReadFileInt64(NAFile* file){
  // Declaration before implementation. Needed for C90.
  int64 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileInt64", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 8);
  if(naHasFileEnded(file)){return 0;}
  value = *((int64*)(file->bufptr));
  file->converter.convert64(&value);
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  return value;
}


uint8 naReadFileUInt8(NAFile* file){
  // Declaration before implementation. Needed for C90.
  uint8 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileUInt8", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 1);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint8*)(file->bufptr));
  file->converter.convert8(&value);
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  return value;
}
uint16 naReadFileUInt16(NAFile* file){
  // Declaration before implementation. Needed for C90.
  uint16 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileUInt16", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 2);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint16*)(file->bufptr));
  file->converter.convert16(&value);
  file->bufptr += 2;
  file->remainingbytesinbuffer -= 2;
  return value;
}
uint32 naReadFileUInt32(NAFile* file){
  // Declaration before implementation. Needed for C90.
  uint32 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileUInt32", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 4);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint32*)(file->bufptr));
  file->converter.convert32(&value);
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  return value;
}
uint64 naReadFileUInt64(NAFile* file){
  // Declaration before implementation. Needed for C90.
  uint64 value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileUInt64", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 8);
  if(naHasFileEnded(file)){return 0;}
  value = *((uint64*)(file->bufptr));
  file->converter.convert64(&value);
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  return value;
}


float naReadFileFloat(NAFile* file){
  // Declaration before implementation. Needed for C90.
  float value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileFloat", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 4);
  if(naHasFileEnded(file)){return 0.f;}
  value = *((float*)(file->bufptr));
  file->converter.convert32(&value);
  file->bufptr += 4;
  file->remainingbytesinbuffer -= 4;
  return value;
}
double naReadFileDouble(NAFile* file){
  // Declaration before implementation. Needed for C90.
  double value;
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileDouble", "File is not a read-file.");
  #endif
  naRequireFileReadBufferBytes(file, 8);
  if(naHasFileEnded(file)){return 0.;}
  value = *((double*)(file->bufptr));
  file->converter.convert64(&value);
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  return value;
}



int8* naReadFileArrayInt8(NAFile* file, int8* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayInt8", "File is not a read-file.");
  #endif
  buf = (int8*)naAllocateIfNull(buf, count * sizeof(int8));
  while(count){
    *buf++ = naReadFileInt8(file);
    count--;
  }
  return buf;
}
int16* naReadFileArrayInt16(NAFile* file, int16* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayInt16", "File is not a read-file.");
  #endif
  buf = (int16*)naAllocateIfNull(buf, count * sizeof(int16));
  while(count){
    *buf++ = naReadFileInt16(file);
    count--;
  }
  return buf;
}
int32* naReadFileArrayInt32(NAFile* file, int32* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayInt32", "File is not a read-file.");
  #endif
  buf = (int32*)naAllocateIfNull(buf, count * sizeof(int32));
  while(count){
    *buf++ = naReadFileInt32(file);
    count--;
  }
  return buf;
}
int64* naReadFileArrayInt64(NAFile* file, int64* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayInt64", "File is not a read-file.");
  #endif
  buf = (int64*)naAllocateIfNull(buf, count * sizeof(int64));
  while(count){
    *buf++ = naReadFileInt64(file);
    count--;
  }
  return buf;
}
uint8* naReadFileArrayUInt8(NAFile* file, uint8* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayUInt8", "File is not a read-file.");
  #endif
  buf = (uint8*)naAllocateIfNull(buf, count * sizeof(uint8));
  while(count){
    *buf++ = naReadFileUInt8(file);
    count--;
  }
  return buf;
}
uint16* naReadFileArrayUInt16(NAFile* file, uint16* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayUInt16", "File is not a read-file.");
  #endif
  buf = (uint16*)naAllocateIfNull(buf, count * sizeof(uint16));
  while(count){
    *buf++ = naReadFileUInt16(file);
    count--;
  }
  return buf;
}
uint32* naReadFileArrayUInt32(NAFile* file, uint32* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayUInt32", "File is not a read-file.");
  #endif
  buf = (uint32*)naAllocateIfNull(buf, count * sizeof(uint32));
  while(count){
    *buf++ = naReadFileUInt32(file);
    count--;
  }
  return buf;
}
uint64* naReadFileArrayUInt64(NAFile* file, uint64* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayUInt64", "File is not a read-file.");
  #endif
  buf = (uint64*)naAllocateIfNull(buf, count * sizeof(uint64));
  while(count){
    *buf++ = naReadFileUInt64(file);
    count--;
  }
  return buf;
}
float* naReadFileArrayFloat(NAFile* file, float* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayFloat", "File is not a read-file.");
  #endif
  buf = (float*)naAllocateIfNull(buf, count * sizeof(float));
  while(count){
    *buf++ = naReadFileFloat(file);
    count--;
  }
  return buf;
}
double* naReadFileArrayDouble(NAFile* file, double* buf, NAInt count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naReadFileArrayDouble", "File is not a read-file.");
  #endif
  buf = (double*)naAllocateIfNull(buf, count * sizeof(double));
  while(count){
    *buf++ = naReadFileDouble(file);
    count--;
  }
  return buf;
}




NAByteArray* naCreateByteArrayFromFile(NAByteArray* bytearray, NAFile* file, NAFileSize count){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naCreateByteArrayFromFile", "File is not a read-file.");
  #endif
  bytearray = naCreateByteArrayWithSize(bytearray, (NAInt)count);
  naReadFileBytes(file, naGetByteArrayMutablePointer(bytearray), count);
  return bytearray;
}


NAString* naCreateStringFromFile(NAString* string, NAFile* file, NAFileSize bytecount){
  #ifndef NDEBUG
    if((file->flags & NA_FILE_FLAG_WRITING))
      naError("naCreateStringFromFile", "File is not a read-file.");
  #endif
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
NA_IHLP void naRequireFileWriteBufferBytes(NAFile* file, uint16 count){
  if(file->remainingbytesinbuffer >= count){
    return;
  }else{
    naFlushFileBuffer(file);
  }
}



void naWriteFileBytes(NAFile* file, const void* ptr, NAFileSize count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileBytes", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileInt8", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileInt16", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileInt32", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileInt64", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileUInt8", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileUInt16", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileUInt32", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileUInt64", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileFloat", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileDouble", "File is not a write-file.");
  #endif
  naRequireFileWriteBufferBytes(file, 8);
  file->converter.convert64(&value);
  *((double*)file->bufptr) = value;
  file->bufptr += 8;
  file->remainingbytesinbuffer -= 8;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) == NA_FILE_FLAG_AUTOFLUSH_ALL){
    naFlushFileBuffer(file);
  }
}



void naWriteFileArrayInt8(NAFile* file, const int8* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayInt8", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileInt8(file, *buf++);
    count--;
  }
}
void naWriteFileArrayInt16(NAFile* file, const int16* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayInt16", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileInt16(file, *buf++);
    count--;
  }
}
void naWriteFileArrayInt32(NAFile* file, const int32* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayInt32", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileInt32(file, *buf++);
    count--;
  }
}
void naWriteFileArrayInt64(NAFile* file, const int64* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayInt64", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileInt64(file, *buf++);
    count--;
  }
}
void naWriteFileArrayUInt8(NAFile* file, const uint8* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayUInt8", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileUInt8(file, *buf++);
    count--;
  }
}
void naWriteFileArrayUInt16(NAFile* file, const uint16* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayUInt16", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileUInt16(file, *buf++);
    count--;
  }
}
void naWriteFileArrayUInt32(NAFile* file, const uint32* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayUInt32", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileUInt32(file, *buf++);
    count--;
  }
}
void naWriteFileArrayUInt64(NAFile* file, const uint64* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayUInt64", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileUInt64(file, *buf++);
    count--;
  }
}
void naWriteFileArrayFloat(NAFile* file, const float* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayFloat", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileFloat(file, *buf++);
    count--;
  }
}
void naWriteFileArrayDouble(NAFile* file, const double* buf, NAInt count){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileArrayDouble", "File is not a write-file.");
  #endif
  while(count){
    naWriteFileDouble(file, *buf++);
    count--;
  }
}



void naWriteFileByteArray(NAFile* file, const NAByteArray* array){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileByteArray", "File is not a write-file.");
  #endif
  naWriteFileBytes(file, naGetByteArrayConstPointer(array), naGetByteArraySize(array));
}



void naWriteFileTab(NAFile* file){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileTab", "File is not a write-file.");
  #endif
  naRequireFileWriteBufferBytes(file, 1);
  *((NAUTF8Char*)file->bufptr) = '\t';
  file->bufptr += 1;
  file->remainingbytesinbuffer -= 1;
  if((file->flags & NA_FILE_FLAG_AUTOFLUSH_MASK) >= NA_FILE_FLAG_AUTOFLUSH_TEXT){
    naFlushFileBuffer(file);
  }
}


void naWriteFileNewLine(NAFile* file){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileNewLine", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileString", "File is not a write-file.");
  #endif
  naWriteFileBytes(file, naGetStringConstUTF8Pointer(string), naGetStringSize(string));
  // todo: encoding.
}


void naWriteFileStringWithFormat(NAFile* file,
                       const NAUTF8Char* format,
                                         ...){
  // Declaration before implementation. Needed for C90.
  va_list argumentlist;
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileStringWithFormat", "File is not a write-file.");
  #endif
  va_start(argumentlist, format);
  naWriteFileStringWithArguments(file, format, argumentlist);
  va_end(argumentlist);
  // todo: encoding.
}

void naWriteFileStringWithArguments(NAFile* file,
                          const NAUTF8Char* format,
                                    va_list argumentlist){
  // Declaration before implementation. Needed for C90.
  NAString string;
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileStringWithArguments", "File is not a write-file.");
  #endif
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
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileLine", "File is not a write-file.");
  #endif
  naWriteFileString(file, string);
  naWriteFileNewLine(file);
}
void naWriteFileLineWithFormat(   NAFile* file,
                        const NAUTF8Char* format,
                                          ...){
  // Declaration before implementation. Needed for C90.
  va_list argumentlist;
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileLineWithFormat", "File is not a write-file.");
  #endif
  va_start(argumentlist, format);
  naWriteFileLineWithArguments(file, format, argumentlist);
  va_end(argumentlist);
}
void naWriteFileLineWithArguments(NAFile* file,
                        const NAUTF8Char* format,
                                  va_list argumentlist){
  #ifndef NDEBUG
    if(!(file->flags & NA_FILE_FLAG_WRITING))
      naError("naWriteFileLineWithArguments", "File is not a write-file.");
  #endif
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
