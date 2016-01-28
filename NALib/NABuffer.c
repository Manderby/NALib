
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "NABuffer.h"
#include "NAEndianness.h"
#include "NAList.h"


#define NA_BUFFER_SIZE 4096
#if (NA_BUFFER_SIZE < 16)
  #error "Invalid file buffer size"
  // buffer must not be smaller than 16 as reader may require 16 Bytes.
#endif


// In NALib, Buffers are either Input-Buffers or Output-Buffers.
//
// They can never be both and they are implemented quite differently although
// the API looks very similar. Here is the main explanation of how the two
// kinds of buffers work:
//
// Input buffers:
//
// Input buffers are implemented using a list of NAByteArrays. The current
// array stores its current position in its iterator.
//
// Output buffers:
//
// Output buffers are basically a growing list of memory blocks partially or
// completely filled with bytes. This is implemented using a list of NABuf
// structs. These structs get flushed to the destination when filled enough
// or upon request by the programmer with a call to naFlushBuffer.
//
// Depending on what the output is destined to fill, the buffer might just
// accumulate byte after byte until finally a destination is given and the
// buffer can be flushed. If the output destination is a file, the buffer
// will write to the file based on the given autoflush parameter which can be
// none, auto, text, multibyte or all.
//


// Defintion of basic buffer flags
#define NA_BUFFER_FLAG_CAN_INPUT            0x01
#define NA_BUFFER_FLAG_CAN_OUTPUT           0x02
#define NA_BUFFER_FLAG_CAN_SEEK             0x04

// If maxpos is known, extend MUST be unset!
#define NA_BUFFER_FLAG_CAN_EXTEND           0x08
#define NA_BUFFER_FLAG_MAXPOS_KNOWN         0x10

#define NA_BUFFER_FLAG_SECURE               0x20

//// Definition of the different kind of src and dst
#define NA_BUFFER_FLAG_SRCDST_BITSHIFT     8
#define NA_BUFFER_FLAG_SRCDST_BITSIZE      2
#define NA_BUFFER_FLAG_SRCDST_MASK         (((1 << NA_BUFFER_FLAG_SRCDST_BITSIZE) - 1) << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
#define NA_BUFFER_FLAG_SRCDST_NONE         (0x00 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
#define NA_BUFFER_FLAG_SRCDST_VOID         (0x01 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
#define NA_BUFFER_FLAG_SRCDST_FILE         (0x02 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
#define NA_BUFFER_FLAG_SRCDST_BYTEARRAY    (0x03 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)

// Redefinition of the newline options as flags. See naSetBufferNewLineEncoding for
// more information why this is done like this.
#define NA_BUFFER_FLAG_NEWLINE_BITSHIFT     12
#define NA_BUFFER_FLAG_NEWLINE_BITSIZE      2
#define NA_BUFFER_FLAG_NEWLINE_MASK         (((1 << NA_BUFFER_FLAG_NEWLINE_BITSIZE) - 1) << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
#define NA_BUFFER_FLAG_NEWLINE_UNIX         (NA_NEWLINE_UNIX << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
#define NA_BUFFER_FLAG_NEWLINE_MAC9         (NA_NEWLINE_MAC9 << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
#define NA_BUFFER_FLAG_NEWLINE_WIN          (NA_NEWLINE_WIN  << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #define NA_BUFFER_FLAG_NEWLINE_NATIVE NA_BUFFER_FLAG_NEWLINE_WIN
#else
  #define NA_BUFFER_FLAG_NEWLINE_NATIVE NA_BUFFER_FLAG_NEWLINE_UNIX
#endif

// Redefinition of the autoflush options as flags. See naSetBufferAutoflush for
// more information why this is done like this.
#define NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT  14
#define NA_BUFFER_FLAG_AUTOFLUSH_BITSIZE   3
#define NA_BUFFER_FLAG_AUTOFLUSH_MASK      (((1 << NA_BUFFER_FLAG_AUTOFLUSH_BITSIZE) - 1) << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
#define NA_BUFFER_FLAG_AUTOFLUSH_NONE      (NA_BUFFER_AUTOFLUSH_NONE      << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
#define NA_BUFFER_FLAG_AUTOFLUSH_AUTO      (NA_BUFFER_AUTOFLUSH_AUTO      << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
#define NA_BUFFER_FLAG_AUTOFLUSH_TEXT      (NA_BUFFER_AUTOFLUSH_TEXT      << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
#define NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE (NA_BUFFER_AUTOFLUSH_MULTIBYTE << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
#define NA_BUFFER_FLAG_AUTOFLUSH_ALL       (NA_BUFFER_AUTOFLUSH_ALL       << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)



// Every NABuffer stores a list of buffer parts.
typedef struct NABufferPart NABufferPart;
struct NABufferPart{
  NABuf buf;
  NABufInt abspos;
};



NA_HDEF NABufInt naGetBufferPartEndIndex(NABufferPart* part){
  return part->abspos + naGetBufMaxSize(&(part->buf));
}

NA_HDEF NABufInt naGetBufferPartMaxIndex(NABufferPart* part){
  return naEndToMaxi64(naGetBufferPartEndIndex(part));
}

NA_HDEF NABool naIsPosInBufferPart(NABufferPart* part, NABufInt abspos){
  return naInsidei((NAInt)part->abspos, (NAInt)naGetBufferPartMaxIndex(part), (NAInt)abspos); // todo: different int types
}

NA_HDEF void naAdjustBufferPartFilledTillEndIndex(NABufferPart* part, NABufInt endindex){
  NABufInt curend;
  #ifndef NDEBUG
    if(!naIsPosInBufferPart(part, endindex-1))
      naError("naAdjustBufferPartFilledTillEndIndex", "end index overflows this buffer");
  #endif
  curend = part->abspos + naGetBufUsedSize(&(part->buf));
  if(curend < endindex){
    naAdvanceBuf(&(part->buf), (NAUInt)(endindex - curend));
  }
}

NA_HDEF NAByte* naGetBufferPartDataPointer(NABufferPart* part, NABufInt index){
  return &(((NAByte*)(naGetBufMutableFirstPointer(&(part->buf))))[index - part->abspos]);
}

NA_HDEF void naClearBufferPart(NABufferPart* part){
  naClearBuf(&(part->buf));
}

NA_HDEF void naClearBufferPartList(NAList* buflist){
  naForeachList(buflist, (NAFunc)naClearBufferPart);
  naClearList(buflist);
}




NA_DEF NABuffer* naInitBufferInputtingFromFile(NABuffer* buffer, const char* filename){
  NAFile* readfile;
  NAList* buflist = naInitList(naAlloc(NAList));
  buffer->storage = naNewPointerMutable(buflist, NA_POINTER_CLEANUP_FREE, (NAFunc)naClearBufferPartList);
  
  readfile = naAlloc(NAFile);
  *readfile = naMakeFileReadingFilename(filename);
  buffer->srcdst = naNewPointerMutable(readfile, NA_POINTER_CLEANUP_FREE, (NAFunc)naCloseFile);

  buffer->minpos = 0;
  buffer->maxpos = -1;

  buffer->readpos = buffer->minpos;
  buffer->writepos = buffer->minpos;
  buffer->readpartposition = NA_NULL;
  buffer->writepartposition = NA_NULL;
  buffer->readbit = 0;
  buffer->writebit = 0;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
  
  buffer->flags = 0;
  buffer->flags |= NA_BUFFER_FLAG_CAN_INPUT;
  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;

  return buffer;
}



NA_DEF NABuffer* naInitBufferInputtingFromByteArray(NABuffer* buffer, const NAByteArray* array){
  NAList* buflist;
  NAByteArray* srcdstarray;

  #ifndef NDEBUG
    if(naIsByteArrayEmpty(array))
      naError("naInitBufferInputtingFromByteArray", "Array is empty");
  #endif
  buflist = naInitList(naAlloc(NAList));
  buffer->storage = naNewPointerMutable(buflist, NA_POINTER_CLEANUP_FREE, (NAFunc)naClearBufferPartList);

  srcdstarray = naAlloc(NAByteArray);
  naInitByteArrayExtraction(srcdstarray, array, 0, -1);
  buffer->srcdst = naNewPointerMutable(srcdstarray, NA_POINTER_CLEANUP_FREE, (NAFunc)naClearByteArray);

  buffer->minpos = 0;
  buffer->maxpos = (NABufInt)naEndToMaxi(naGetByteArraySize(array));

  buffer->readpos = buffer->minpos;
  buffer->writepos = buffer->minpos;
  buffer->readpartposition = NA_NULL;
  buffer->writepartposition = NA_NULL;
  buffer->readbit = 0;
  buffer->writebit = 0;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  buffer->flags = 0;
  buffer->flags |= NA_BUFFER_FLAG_CAN_INPUT;
  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
  buffer->flags |= NA_BUFFER_FLAG_MAXPOS_KNOWN;
  buffer->flags |= NA_BUFFER_FLAG_SRCDST_BYTEARRAY;

  return buffer;
}



NA_DEF NABuffer* naInitBufferInputtingFromBufferExtraction(NABuffer* buffer, NABuffer* srcbuffer, NABufInt size){
  #ifndef NDEBUG
    if(!naCanBufferInput(srcbuffer))
      naError("naInitBufferInputtingFromBufferExtraction", "src buffer can not input");
    if(!naCanBufferSeek(srcbuffer))
      naError("naInitBufferInputtingFromBufferExtraction", "src buffer bust be able to seek");
  #endif

  buffer->storage = naRetainPointer(srcbuffer->storage);

  buffer->srcdst = naRetainPointer(srcbuffer->srcdst);

  buffer->minpos = srcbuffer->readpos;
  buffer->maxpos = (NABufInt)naEndToMaxi((NAInt)srcbuffer->readpos + (NAInt)size);
  
  // Advance the src buffer by the given size and invalidate the curpart to
  // search it anew in the next step.
  srcbuffer->readpos += size;
  srcbuffer->readpartposition = NA_NULL;

  buffer->readpos = buffer->minpos;
  buffer->writepos = buffer->minpos;
  buffer->readpartposition = NA_NULL;
  buffer->writepartposition = NA_NULL;
  buffer->readbit = 0;
  buffer->writebit = 0;

  buffer->endianness = srcbuffer->endianness;
  buffer->converter = srcbuffer->converter;

  // Copy the flags but delete the output flag.
  buffer->flags = srcbuffer->flags;
  buffer->flags |= NA_BUFFER_FLAG_MAXPOS_KNOWN;
  buffer->flags &= ~NA_BUFFER_FLAG_CAN_OUTPUT;
  buffer->flags &= ~NA_BUFFER_FLAG_CAN_EXTEND;

  return buffer;
}



NA_DEF NABuffer* naInitBuffer(NABuffer* buffer){
  NAList* buflist = naInitList(naAlloc(NAList));
  buffer->storage = naNewPointerMutable(buflist, NA_POINTER_CLEANUP_FREE, (NAFunc)naClearBufferPartList);

  buffer->srcdst = naNewPointerConst(NA_NULL);

  buffer->minpos = 0;
  buffer->maxpos = -1;

  buffer->readpos = buffer->minpos;
  buffer->writepos = buffer->minpos;
  buffer->readpartposition = NA_NULL;
  buffer->writepartposition = NA_NULL;
  buffer->readbit = 0;
  buffer->writebit = 0;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  buffer->flags = 0;
  buffer->flags |= NA_BUFFER_FLAG_CAN_INPUT;
  buffer->flags |= NA_BUFFER_FLAG_CAN_OUTPUT;
  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
  buffer->flags |= NA_BUFFER_FLAG_CAN_EXTEND;
  buffer->flags |= NA_BUFFER_FLAG_SRCDST_NONE;

  return buffer;
}



//NA_DEF NABuffer* naInitBufferOutputtingToFile(NABuffer* buffer, const char* filename, NAFileMode mode){
//  NAList* buflist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointerMutable(buflist, NA_POINTER_CLEANUP_FREE, naClearList);
//
//  buffer->srcdst = naAlloc(NAFile);
//  *((NAFile*)(buffer->srcdst)) = naMakeFileWritingFilename(filename, mode);
//
//  buffer->minpos = 0;
//  buffer->maxpos = -1;  // todo: check if better solution
//
//  buffer->readpos = buffer->minpos;
//  buffer->writepos = buffer->minpos;
//  buffer->readpartposition = NA_NULL;
//  buffer->writepartposition = NA_NULL;
//  buffer->readbit = 0;
//  buffer->writebit = 0;
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  buffer->flags = 0;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_OUTPUT;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_EXTEND;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferOutputtingToStdout(NABuffer* buffer){
//  NAList* buflist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointerMutable(buflist, NA_POINTER_CLEANUP_FREE, naClearList);
//
//  buffer->srcdst = naAlloc(NAFile);
//  *((NAFile*)(buffer->srcdst)) = naMakeFileWritingStdout();
//
//  buffer->minpos = 0;
//  buffer->maxpos = -1;  // todo: check if better solution
//
//  buffer->readpos = buffer->minpos;
//  buffer->writepos = buffer->minpos;
//  buffer->readpartposition = NA_NULL;
//  buffer->writepartposition = NA_NULL;
//  buffer->readbit = 0;
//  buffer->writebit = 0;
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  buffer->flags = 0;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_OUTPUT;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferOutputtingToStderr(NABuffer* buffer){
//  NAList* buflist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointerMutable(buflist, NA_POINTER_CLEANUP_FREE, naClearList);
//
//  buffer->srcdst = naAlloc(NAFile);
//  *((NAFile*)(buffer->srcdst)) = naMakeFileWritingStderr();
//
//  buffer->minpos = 0;
//  buffer->maxpos = -1;  // todo: check if better solution
//
//  buffer->readpos = buffer->minpos;
//  buffer->writepos = buffer->minpos;
//  buffer->readpartposition = NA_NULL;
//  buffer->writepartposition = NA_NULL;
//  buffer->readbit = 0;
//  buffer->writebit = 0;
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  buffer->flags = 0;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_OUTPUT;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferOutputtingToVoid(NABuffer* buffer){
//  NAList* buflist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointerMutable(buflist, NA_POINTER_CLEANUP_FREE, naClearList);
//
//  buffer->srcdst = NA_NULL;
//
//  buffer->minpos = 0;
//  buffer->maxpos = -1;  // todo: check if better solution
//
//  buffer->readpos = buffer->minpos;
//  buffer->writepos = buffer->minpos;
//  buffer->readpartposition = NA_NULL;
//  buffer->writepartposition = NA_NULL;
//  buffer->readbit = 0;
//  buffer->writebit = 0;
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  buffer->flags = 0;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_OUTPUT;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_VOID;
//
//  return buffer;
//}





NA_DEF void naFlushBuffer(NABuffer* buffer){
//  if(naGetBufferKind(buffer) == NA_BUFFER_KIND_OUTPUT){
//    if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_NONE){
//      return;
//    }
//    naFirstList(&(buffer->buflist));
//    NABuf* curbuf;
//    while((curbuf = naIterateListMutable(&(buffer->buflist), 1))){
//      #ifndef NDEBUG
//        if(!naGetBufUsedSize(curbuf))
//          naError("naFlushBuffer", "Internal error: NABuf is empty.");
//      #endif
//      switch(buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK){
//      case NA_BUFFER_FLAG_SRCDST_NONE:
//        // this is handeled in the if statement above.
//        break;
//      case NA_BUFFER_FLAG_SRCDST_VOID:
//        // Nothing to do. Flushing to nowhere.
//        break;
//      case NA_BUFFER_FLAG_SRCDST_FILE:
//        naWriteFileBytes(buffer->srcdst, naGetBufConstFirstPointer(curbuf), naGetBufUsedSize(curbuf));
//        break;
//      default:
//        #ifndef NDEBUG
//          naError("naFlushBuffer", "Invalid srcdst field.");
//        #endif
//        break;
//      }
//      naRemoveListPrevMutable(&(buffer->buflist));
//      naFree(naGetBufMutableFirstPointer(curbuf));
//      naFree(curbuf);
//    }
//
//  }else{
//
//    NAByteArray* curarray;
//    
//    naFirstList(&(buffer->buflist));
//    while((curarray = naIterateListMutable(&(buffer->buflist), 1))){
//      naRemoveListPrevMutable(&(buffer->buflist));
//      naClearByteArray(curarray);
//      naFree(curarray);
//    }
//
//  }
}



//NA_DEF void naVoidBuffer(NABuffer* buffer){
////  #ifndef NDEBUG
////    if(naGetBufferKind(buffer) != NA_BUFFER_KIND_OUTPUT)
////      naError("naVoidBuffer", "Buffer is not an output-buffer.");
////  #endif
////  switch(buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK){
////  case NA_BUFFER_FLAG_SRCDST_NONE:
////    // Switch to void immediately and lose all previous buffers.
////    buffer->flags &= ~NA_BUFFER_FLAG_SRCDST_MASK;
////    buffer->flags |= NA_BUFFER_FLAG_SRCDST_VOID;
////    naFlushBuffer(buffer);
////    break;
////  case NA_BUFFER_FLAG_SRCDST_VOID:
////    // nothing to do
////    break;
////  case NA_BUFFER_FLAG_SRCDST_FILE:
////    // Write the remaining buffers and close the file
////    naFlushBuffer(buffer);
////    naCloseFile(buffer->srcdst);
////    // Then switch to void.
////    buffer->flags &= ~NA_BUFFER_FLAG_SRCDST_MASK;
////    buffer->flags |= NA_BUFFER_FLAG_SRCDST_VOID;
////    break;
////  default:
////    #ifndef NDEBUG
////      naError("naVoidBuffer", "Invalid srcdst field.");
////    #endif
////    break;
////  }
//}



NA_DEF void naClearBuffer(NABuffer* buffer){
//  switch(buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK){
//  case NA_BUFFER_FLAG_SRCDST_NONE:
//    break;
//  case NA_BUFFER_FLAG_SRCDST_VOID:
//    break;
//  case NA_BUFFER_FLAG_SRCDST_FILE:
//    naCloseFile(naGetPointerMutable(buffer->srcdst));
//    break;
//  case NA_BUFFER_FLAG_SRCDST_BYTEARRAY:
//    naClearByteArray(naGetPointerMutable(buffer->srcdst));
//    break;
//  default:
//    #ifndef NDEBUG
//      naError("naClearBuffer", "Invalid srcdst field.");
//    #endif
//    break;
//  }
  naReleasePointer(buffer->srcdst);
  naReleasePointer(buffer->storage);
//  
//  NAList* buflist = naGetPointerMutable(buffer->storage);
//  naForeachList(buflist, (NAFunc)naClearBufferPart);
//  naClearList(buflist);
}



NA_DEF NABool naCanBufferInput(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_CAN_INPUT) == NA_BUFFER_FLAG_CAN_INPUT);
}



NA_DEF NABool naCanBufferOutput(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_CAN_OUTPUT) == NA_BUFFER_FLAG_CAN_OUTPUT);
}



NA_DEF NABool naCanBufferSeek(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_CAN_SEEK) == NA_BUFFER_FLAG_CAN_SEEK);
}



NA_DEF NABool naCanBufferExtend(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_CAN_EXTEND) == NA_BUFFER_FLAG_CAN_EXTEND);
}



NA_API NABool naHasBufferKnownMaxPos(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_MAXPOS_KNOWN) == NA_BUFFER_FLAG_MAXPOS_KNOWN);
}



NA_API NABool naIsBufferSecure(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_SECURE) == NA_BUFFER_FLAG_SECURE);
}



NA_DEF NABufInt naGetBufferReadPosAbsolute(const NABuffer* buffer){
  return buffer->readpos;
}



NA_DEF NABufInt naGetBufferWritePosAbsolute(const NABuffer* buffer){
  return buffer->writepos;
}



NA_API NABufInt naGetBufferSize(NABuffer* buffer){
  #ifndef NDEBUG
    if(!naHasBufferKnownMaxPos(buffer))
      naError("naGetBufferSize", "Buffer has no fixed max position. Use naComputeBufferMaxPos or naFixBufferMaxPos.");
  #endif
  return naMaxToEndi((NAInt)buffer->maxpos) - buffer->minpos;
}


NA_API NABufInt naComputeBufferMaxPos(NABuffer* buffer){
  #ifndef NDEBUG
    if(naHasBufferKnownMaxPos(buffer))
      naError("naComputeBufferMaxPos", "Max pos already fixed");
    if(     ((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) != NA_BUFFER_FLAG_SRCDST_FILE)
        &&  ((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) != NA_BUFFER_FLAG_SRCDST_BYTEARRAY))
      naError("naComputeBufferMaxPos", "Can compute max pos only for files and bytearrays");
  #endif
  if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_FILE){
    buffer->maxpos = naEndToMaxi(naComputeFileSize((NAFile*)naGetPointerMutable(buffer->srcdst)));
  }
  buffer->flags |= NA_BUFFER_FLAG_MAXPOS_KNOWN;
  buffer->flags &= ~NA_BUFFER_FLAG_CAN_EXTEND;
  return buffer->maxpos;
}



NA_API NABufInt naFixBufferMaxPos(NABuffer* buffer){
  #ifndef NDEBUG
    if(naHasBufferKnownMaxPos(buffer))
      naError("naFixBufferMaxPos", "Max pos already fixed");
  #endif
  buffer->flags |= NA_BUFFER_FLAG_MAXPOS_KNOWN;
  buffer->flags &= ~NA_BUFFER_FLAG_CAN_EXTEND;
  return buffer->maxpos;
}



NA_DEF NABool naIsBufferReadAtEnd(const NABuffer* buffer){
  #ifndef NDEBUG
    if(!naHasBufferKnownMaxPos(buffer))
      naError("naIsBufferReadAtEnd", "Buffer has no fixed max position. Use naComputeBufferMaxPos or naFixBufferMaxPos.");
  #endif
  return (buffer->readpos > buffer->maxpos);
}



NA_DEF void naWriteBufferToFile(NABuffer* buffer, NAFile* file, NABool atabsoluteposition){
  NAList* buflist;
  NABufferPart* curpart;
  #ifndef NDEBUG
    if(atabsoluteposition)
      naError("naWriteBufferToFile", "atabsoluteposition must be NA_FALSE for now. Will be implemented later.");
      // todo.
  #else
    NA_UNUSED(atabsoluteposition);
  #endif
  buflist = naGetPointerMutable(buffer->storage);
  naFirstList(buflist);
  while((curpart = naIterateListMutable(buflist, 1))){
    // todo: This might not be correct in all cases.
    naWriteFileBytes(file, naGetBufConstFirstPointer(&(curpart->buf)), naGetBufUsedSize(&(curpart->buf)));
  }
}



NA_DEF void naSetBufferEndianness(NABuffer* buffer, NAInt endianness){
  buffer->endianness = endianness;
  buffer->converter = naMakeEndiannessConverter(endianness, NA_ENDIANNESS_NATIVE);
}


NA_DEF NAInt naGetBufferEndianness(NABuffer* buffer){
  return buffer->endianness;
}


//// You may ask yourself the question why the author decided to reimplement the
//// different flags again and use a case to set them correctly. This is because
//// the autoflush flags are actually an enumeration and not macros. Mixing the
//// two can lead to hard to understand compile errors especially if the enum
//// will be enhanced in the future (who knows). By reimplementing the flags
//// again, we can be sure that the NABuffer implementation is independent of
//// the enumeration.
//NA_DEF void naSetBufferAutoFlushing(NABuffer* buffer, NABufferAutoFlushing autoflushing){
////  NAUInt autoflushflag;
////  #ifndef NDEBUG
////    if(naGetBufferKind(buffer) != NA_BUFFER_KIND_OUTPUT)
////      naError("naSetBufferAutoFlushing", "Setting the flushing behaviour is only useful for output buffers.");
////  #endif
////  buffer->flags &= ~NA_BUFFER_FLAG_AUTOFLUSH_MASK;
////  switch(autoflushing){
////  case NA_BUFFER_AUTOFLUSH_NONE:      autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_NONE;      break;
////  case NA_BUFFER_AUTOFLUSH_AUTO:      autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_AUTO;      break;
////  case NA_BUFFER_AUTOFLUSH_TEXT:      autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_TEXT;      break;
////  case NA_BUFFER_AUTOFLUSH_MULTIBYTE: autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE; break;
////  case NA_BUFFER_AUTOFLUSH_ALL:       autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_ALL;       break;
////  default:
////    #ifndef NDEBUG
////      naError("naSetBufferNewLineEncoding", "Unhandeled case.");
////    #endif
////    autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_AUTO;
////    break;
////  }
////  buffer->flags |= autoflushflag;
//}
//
//
//
//// You may ask yourself the question why the author decided to reimplement the
//// different flags again and use a case to set them correctly. This is because
//// the newline flags are actually an enumeration and not macros. Mixing the
//// two can lead to hard to understand compile errors especially if the enum
//// will be enhanced in the future (who knows). By reimplementing the flags
//// again, we can be sure that the NABuffer implementation is independent of
//// the enumeration.
//NA_DEF void naSetBufferNewLineEncoding(NABuffer* buffer, NANewlineEncoding newlineencoding){
////  NAUInt newlineflag;
////  #ifndef NDEBUG
////    if(naGetBufferKind(buffer) != NA_BUFFER_KIND_OUTPUT)
////      naError("naSetBufferNewLineEncoding", "Setting the newline encoding is only useful for output buffers.");
////  #endif
////  buffer->flags &= ~NA_BUFFER_FLAG_NEWLINE_MASK;
////  switch(newlineencoding){
////  case NA_NEWLINE_UNIX:   newlineflag = NA_BUFFER_FLAG_NEWLINE_UNIX;   break;
////  case NA_NEWLINE_MAC9:   newlineflag = NA_BUFFER_FLAG_NEWLINE_MAC9;   break;
////  case NA_NEWLINE_WIN:    newlineflag = NA_BUFFER_FLAG_NEWLINE_WIN;    break;
////  case NA_NEWLINE_NATIVE: newlineflag = NA_BUFFER_FLAG_NEWLINE_NATIVE; break;
////  default:
////    #ifndef NDEBUG
////      naError("naSetBufferNewLineEncoding", "Unhandeled case.");
////    #endif
////    break;
////  }
////  buffer->flags |= newlineflag;
//}


NA_DEF void naSetBufferSecure(NABuffer* buffer, NABool secure){
  if(secure){
    buffer->flags |= NA_BUFFER_FLAG_SECURE;
  }else{
    buffer->flags &= ~NA_BUFFER_FLAG_SECURE;
  }
}



NA_DEF void naSeekBufferReadAbsolute(NABuffer* buffer, NABufInt pos){
  #ifndef NDEBUG
    if(!naCanBufferSeek(buffer))
      naError("naSeekBufferReadAbsolute", "Buffer can not seek");
    if(pos < buffer->minpos)
      naError("naSeekBufferReadAbsolute", "pos underflows buffer range.");
    if(naHasBufferKnownMaxPos(buffer) && (pos > buffer->maxpos))
      naError("naSeekBufferReadAbsolute", "pos overflows buffer range.");
  #endif
  buffer->readpos = pos;
}



NA_DEF void naSeekBufferReadLocal(NABuffer* buffer, NABufInt pos){
  NABufInt newpos = buffer->minpos + pos;
  #ifndef NDEBUG
    if(!naCanBufferSeek(buffer))
      naError("naSeekBufferReadLocal", "Buffer can not seek");
    if(newpos < buffer->minpos)
      naError("naSeekBufferReadLocal", "pos underflows buffer range.");
    if(naHasBufferKnownMaxPos(buffer) && (pos > buffer->maxpos))
      naError("naSeekBufferReadLocal", "pos overflows buffer range.");
  #endif
  buffer->readpos = newpos;
}



NA_DEF void naSeekBufferReadRelative(NABuffer* buffer, NABufInt pos){
  NABufInt newpos = buffer->readpos + pos;
  #ifndef NDEBUG
    if(!naCanBufferSeek(buffer))
      naError("naSeekBufferReadRelative", "Buffer can not seek");
    if(newpos < buffer->minpos)
      naError("naSeekBufferReadRelative", "pos underflows buffer range.");
    if(naHasBufferKnownMaxPos(buffer) && (pos > buffer->maxpos))
      naError("naSeekBufferReadRelative", "pos overflows buffer range.");
  #endif
  buffer->readpos = newpos;
}



NA_DEF void naSeekBufferWriteAbsolute(NABuffer* buffer, NABufInt pos){
  #ifndef NDEBUG
    if(!naCanBufferSeek(buffer))
      naError("naSeekBufferWriteAbsolute", "Buffer can not seek");
    if(pos < buffer->minpos)
      naError("naSeekBufferWriteAbsolute", "pos underflows buffer range.");
    if((pos > buffer->maxpos) && (!naCanBufferExtend(buffer)))
      naError("naSeekBufferWriteAbsolute", "pos overflows buffer range but buffer can not extend.");
  #endif
  buffer->writepos = pos;
}



NA_DEF void naSeekBufferWriteLocal(NABuffer* buffer, NABufInt pos){
  NABufInt newpos = buffer->minpos + pos;
  #ifndef NDEBUG
    if(!naCanBufferSeek(buffer))
      naError("naSeekBufferWriteLocal", "Buffer can not seek");
    if(newpos < buffer->minpos)
      naError("naSeekBufferWriteLocal", "pos underflows buffer range.");
    if((pos > buffer->maxpos) && (!naCanBufferExtend(buffer)))
      naError("naSeekBufferWriteLocal", "pos overflows buffer range but buffer can not extend.");
  #endif
  buffer->writepos = newpos;
}



NA_DEF void naSeekBufferWriteRelative(NABuffer* buffer, NABufInt pos){
  NABufInt newpos = buffer->writepos + pos;
  #ifndef NDEBUG
    if(!naCanBufferSeek(buffer))
      naError("naSeekBufferWriteRelative", "Buffer can not seek");
    if(newpos < buffer->minpos)
      naError("naSeekBufferWriteRelative", "pos underflows buffer range.");
    if((pos > buffer->maxpos) && (!naCanBufferExtend(buffer)))
      naError("naSeekBufferWriteRelative", "pos overflows buffer range but buffer can not extend.");
  #endif
  buffer->writepos = newpos;
}







NA_HDEF void naReadBufferPart(NABuffer* buffer, NABufferPart* part){
  NAInt desiredbytes = naGetBufMaxSize(&(part->buf));
  NAFileSize readbytes = 0;
  if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_FILE){
    readbytes = naReadFileBytes(naGetPointerMutable(buffer->srcdst), naGetBufMutableFirstPointer(&(part->buf)), desiredbytes);
  }else if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_NONE){
    // Don't do anything
  }else{
    // todo
    printf("not yet implemented");
  }
  if(naIsBufferSecure(buffer) && (readbytes < desiredbytes)){
    naNulln(&(((NAByte*)naGetBufMutableFirstPointer(&(part->buf)))[readbytes]), desiredbytes - readbytes);
  }
}



NA_HIDEF NABufferPart* naLocateBufferList(NAList* buflist, NABufInt abspos){
  NABufferPart* curpart;
  
  // todo: This can be much better if stored as a tree.
  if(naGetListCount(buflist) == 0){return NA_NULL;}
  
  curpart = naGetListCurrentMutable(buflist);
  if(!curpart){
    naFirstList(buflist);
    curpart = naGetListCurrentMutable(buflist);
  }
  // Now we are sure, curpart is a part of the list
  
  // Search backward.
  while(curpart && ((NABufInt)curpart->abspos > abspos)){
    naIterateList(buflist, -1);
    curpart = naGetListCurrentMutable(buflist);
  }
  // Search forward.
  while(curpart && (naGetBufferPartMaxIndex(curpart) < abspos)){
    naIterateList(buflist, +1);
    curpart = naGetListCurrentMutable(buflist);
  }
  // Now, we are sure that curpart either contains abspos or is the one element
  // after that.

  return curpart;
}


// Helper functions not visible to the programmer. Will make sure that there
// are enough bytes in the buffer.
// These functions are hidden to the user as they are just supplementary helper
// functions which prepare the NABuffer struct internally for an appropriate
// action. Therefore the user shall not use it. Of course, in NALib hiding
// something has not much use but it shows where the hidden attribute makes
// sense.
NA_HIDEF void naEnhanceBuffer(NABuffer* buffer, NABufInt count, NABool forwriting){
  NAListPos* bufferpartposition;
  NABufInt curpos;
  NAList* buflist;
  NABufferPart* curpart;
  NABufferPart* newpart;

  #ifndef NDEBUG
    if(!count)
      naError("naEnhanceBuffer", "count is zero.");
    if(count < 0)
      naError("naEnhanceBuffer", "count is negative.");
  #endif
  
  if(forwriting){
    bufferpartposition = &(buffer->writepartposition);
    curpos = buffer->writepos;
  }else{
    bufferpartposition = &(buffer->readpartposition);
    curpos = buffer->readpos;
  }
  
  buflist = naGetPointerMutable(buffer->storage);
  curpart = NA_NULL;
  if(*bufferpartposition){
    curpart = naGetListPositionMutable(buflist, *bufferpartposition);
    if(!naIsPosInBufferPart(curpart, curpos)){*bufferpartposition = NA_NULL;}
  }
  
//  if(curpos == 4095){
//    int asdf = 1;
//  }
  
  // Note: newly created parts always start at the precise position defined
  // by curpos. The lenght of the new parts though are adjusted to be of a
  // size of at least NA_BUFFER_SIZE.
  //
  // Ignoring that the buffer could also be aligned to NA_BUFFER_SIZE when
  // looking at curpos makes the (already quite difficult) implementation a
  // little easier. The only drawback is that adding small amounts of bytes
  // backwards can add a lot of buffer parts.
  //
  // This can be ameliorated one day, but it is not a very pressing issue.
  
  while(count){
  
    if(!curpart || !naIsPosInBufferPart(curpart, curpos)){curpart = naLocateBufferList(buflist, curpos);}
    // now, curpart is the element which contains curpos or which is
    // the first element after curpos or Null if none available. The list
    // points to the same element.
    
    if(curpart && naIsPosInBufferPart(curpart, curpos)){
    
      NABufInt endindex;
      NABufInt remainingsize;

      // curpart contains the curpos. If there is not yet a part pointer
      // set, we set it now:
      if(!(*bufferpartposition)){*bufferpartposition = naGetListPosition(buflist);}

      endindex = naGetBufferPartEndIndex(curpart);
      remainingsize = endindex - curpos;
      if(count > remainingsize){
        // The desired content does not fit completely.
        NABufInt newendindex = curpos + remainingsize;
        naAdjustBufferPartFilledTillEndIndex(curpart, newendindex);
        curpos += remainingsize;
        count -= remainingsize;
        if(naCanBufferExtend(buffer) && (buffer->maxpos < naEndToMaxi64(newendindex))){buffer->maxpos = naEndToMaxi64(newendindex);}
      }else{
        // We have sufficient space in this buf.
        NABufInt newendindex = curpos + count;
        naAdjustBufferPartFilledTillEndIndex(curpart, curpos + count);
        curpos += count;
        count = 0;
        if(naCanBufferExtend(buffer) && (buffer->maxpos < naEndToMaxi64(newendindex))){buffer->maxpos = naEndToMaxi64(newendindex);}
      }
    
    }else{
      // curpart is either inexistant or the buffer after the desired buffer
      // which we must create anew.

      if(!curpart){
        NABufInt bufpartsize;
        NABufInt newmaxpos;
        NAListPos newlistposition;

        // If there is no curpart, a new buffer part must be added to the
        // end of the list. Create a new buffer part with all remaining bytes.
        newpart = naAlloc(NABufferPart);
        bufpartsize = ((count + NA_BUFFER_SIZE) / NA_BUFFER_SIZE) * NA_BUFFER_SIZE;
        newpart->buf = naMakeBufWithSize((NAInt)bufpartsize, (NAInt)count); // todo: check different int types
        newpart->abspos = curpos;
        newmaxpos = naEndToMaxi64(curpos + count);
        #ifndef NDEBUG
          if(naHasBufferKnownMaxPos(buffer) && (newmaxpos > buffer->maxpos))
            naError("naEnhanceBuffer", "Buffer overflow at the end. Overflowing bytes are garbage and will be ignored.");
        #endif
        if(naCanBufferExtend(buffer) && (buffer->maxpos < newmaxpos)){buffer->maxpos = newmaxpos;}
        naReadBufferPart(buffer, newpart);
        curpos += count;
        count = 0;

        // Now, We add the new buffer part to the end of the list and set the
        // part pointer to the new list element if necessary.
        newlistposition = naAddListLastMutable(buflist, newpart);
        if(!(*bufferpartposition)){*bufferpartposition = newlistposition;}
      
      }else{
        NAListPos newlistposition;

        // We have a buffer part denoting the next part in the list.
        // We find out, how much we need to add before the next element.
        NABufferPart* nextpart = naGetListCurrentMutable(buflist);
        NABufInt potentialsize = nextpart->abspos - curpos;
        newpart = naAlloc(NABufferPart);
        if(count < potentialsize){
          // We have enough space to fit all required bytes.
          NABufInt bufpartsize = ((count + NA_BUFFER_SIZE) / NA_BUFFER_SIZE) * NA_BUFFER_SIZE;
          // We do not waste too much space:
          if((bufpartsize + NA_BUFFER_SIZE) < potentialsize){
            // A small buffer is sufficient.
            newpart->buf = naMakeBufWithSize((NAInt)bufpartsize, (NAInt)count); // todo: check different int types
            newpart->abspos = curpos;
            #ifndef NDEBUG
              if(naHasBufferKnownMaxPos(buffer) && (naEndToMaxi64(curpos + count) > buffer->maxpos))
              naError("naEnhanceBuffer", "Internal error: Buffer overflow 1 where it should not occur.");
            #endif
            naReadBufferPart(buffer, newpart);
          }else{
            // Instead of creating another potential buffer in between this one
            // and the first one of the list, we simply extend this one to the
            // full potential size.
            newpart->buf = naMakeBufWithSize((NAInt)potentialsize, (NAInt)count); // todo: check different int types
            newpart->abspos = curpos;
            #ifndef NDEBUG
              if(naHasBufferKnownMaxPos(buffer) && (naEndToMaxi64(curpos + count) > buffer->maxpos))
              naError("naEnhanceBuffer", "Internal error: Buffer overflow 2 where it should not occur.");
            #endif
            naReadBufferPart(buffer, newpart);
          }
          curpos += count;
          count = 0;
        }else{
          // The required bytes use equal or more than the space available.
          newpart->buf = naMakeBufWithSize((NAInt)potentialsize, (NAInt)potentialsize); // todo: check different int types
          newpart->abspos = curpos;
          #ifndef NDEBUG
            if(naHasBufferKnownMaxPos(buffer) && (naEndToMaxi64(curpos + count) > buffer->maxpos))
              naError("naEnhanceBuffer", "Internal error: Buffer overflow 3 where it should not occur.");
          #endif
          naReadBufferPart(buffer, newpart);
          curpos += potentialsize;
          count -= potentialsize;
        }

        // Now, We add the first buffer part to the list and hence set the the
        // part pointer to the new list element if necessary.
        newlistposition = naAddListBeforeMutable(buflist, newpart);
        if(!(*bufferpartposition)){*bufferpartposition = newlistposition;}
      }

    }
  }
}




NA_DEF void naAccumulateBufferToChecksum(NABuffer* buffer, NAChecksum* checksum){
  NABufInt count;
  NABufInt curpos;
  NAList* buflist;

  #ifndef NDEBUG
    if(!naHasBufferKnownMaxPos(buffer))
      naError("naAccumulateBufferToChecksum", "Buffer has no known max position. Use naComputeBufferMaxPos or naFixBufferMaxPos");
  #endif
  count = naGetBufferSize(buffer);
  if(!count){return;}

  curpos = naGetBufferReadPosAbsolute(buffer);
  naSeekBufferReadLocal(buffer, 0);
  naEnhanceBuffer(buffer, count, NA_FALSE);
  
  buflist = naGetPointerMutable(buffer->storage);
  
  while(count){
    NABufferPart* curpart;
    NABufInt remainingbytes;
    NAByte* src;

    #ifndef NDEBUG
      if(!buffer->readpartposition)
        naError("naAccumulateBufferToChecksum", "Internal error: No buffer part available.");
    #endif
    curpart = naGetListPositionMutable(buflist, buffer->readpartposition);
    if(!curpart){return;} // security return. Only used for code sanity checks.
    remainingbytes = naGetBufferPartEndIndex(curpart) - buffer->readpos;
    src = naGetBufferPartDataPointer(curpart, buffer->readpos);
    if(count > remainingbytes){
      naAccumulateChecksum(checksum, src, (NAInt)remainingbytes);
      buffer->readpos += remainingbytes;
      buffer->readpartposition = naGetListPositionNext(buflist, buffer->readpartposition);
      count -= remainingbytes;
    }else{
      naAccumulateChecksum(checksum, src, (NAInt)count);
      buffer->readpos += count;
      count -= count;
    }
  }

  naSeekBufferReadAbsolute(buffer, curpos);
  
}


// ///////////////////////////////////////////////////////////////
// READING
// ///////////////////////////////////////////////////////////////





NA_DEF void naRetrieveBufferBytes(NABuffer* buffer, void* ptr, NABufInt count, NAUInt lastbytemodulobits){
  NAList* buflist;
  
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naRetrieveBufferBytes", "Buffer is not an input-buffer.");
    if(!count)
      naError("naRetrieveBufferBytes", "Reading zero bytes.");
    if(count < 0)
      naError("naRetrieveBufferBytes", "Negative count.");
  #endif
  if(!count){return;}

  buflist = naGetPointerMutable(buffer->storage);
  
  while(count){
    NABufferPart* curpart;
    NABufInt remainingbytes;
    NAByte* src;

    #ifndef NDEBUG
      if(!buffer->readpartposition)
        naError("naRetrieveBufferBytes", "Internal error: No buffer part available.");
    #endif
    curpart = naGetListPositionMutable(buflist, buffer->readpartposition);
    #ifndef NDEBUG
      if(!curpart || !naIsPosInBufferPart(curpart, buffer->readpos))
        naError("naRetrieveBufferBytes", "Internal error: Current buffer part inavailable or not containing current position.");
    #endif
    if(!curpart){return;} // security return. Only used for code sanity checks.
    remainingbytes = naGetBufferPartEndIndex(curpart) - buffer->readpos;
    src = naGetBufferPartDataPointer(curpart, buffer->readpos);
    if(count > remainingbytes){
      naCopyn(ptr, src, (NAUInt)remainingbytes);
      buffer->readpos += remainingbytes;
      buffer->readpartposition = naGetListPositionNext(buflist, buffer->readpartposition);
      count -= remainingbytes;
      *((NAByte**)(&ptr)) += remainingbytes;
    }else{
      naCopyn(ptr, src, (NAUInt)count);
      buffer->readpos += count;
      count -= count;
    }
  }
  
  buffer->readbit = lastbytemodulobits;
  if(lastbytemodulobits){buffer->readpos--;}
}




NA_DEF void naReadBufferBytes(NABuffer* buffer, void* buf, NABufInt count){
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferBytes", "Buffer is not a input buffer.");
    if(!count)
      naError("naReadBufferBytes", "Reading zero bytes.");
    if(count < 0)
      naError("naReadBufferBytes", "Negative count.");
    if(buffer->readbit != 0)
      naError("naReadBufferBytes", "Bit offset not 0.");
  #endif
  if(!count){return;}
  naEnhanceBuffer(buffer, count, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count, 0);
}



//NA_DEF NAByteArray* naInitByteArrayFromBufferInput(NAByteArray* array, NABuffer* buffer, NAInt count){
//  #ifndef NDEBUG
//    if(!naCanBufferInput(buffer))
//      naError("naInitByteArrayFromBufferInput", "Buffer is not a input buffer.");
//    if(!count)
//      naError("naInitByteArrayFromBufferInput", "Reading zero bytes.");
//    if(count < 0)
//      naError("naInitByteArrayFromBufferInput", "Negative count.");
//    if(buffer->readbit != 0)
//      naError("naInitByteArrayFromBufferInput", "Bit offset not 0.");
//  #endif
//  if(!count){
//    naInitByteArray(array);
//  }else{
//    naInitByteArrayWithSize(array, count);
//    naReadBufferBytes(buffer, naGetByteArrayMutablePointer(array), count);
//  }
//  return array;
//}



NA_DEF NABool naReadBufferBit (NABuffer* buffer){
  uint8 curbit;
  NAUInt requiredbitcount;
  NAUInt lastbytemodulobits;
  NAByte value;
  NABool bit;

  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferBits", "Buffer is not an input buffer.");
  #endif
  curbit = buffer->readbit;
  requiredbitcount = curbit + 1;
  lastbytemodulobits = requiredbitcount % 8;

  naEnhanceBuffer(buffer, 1, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 1, lastbytemodulobits);

  bit = (value >> curbit) & 0x01;
  return bit;
}



NA_DEF uint32 naReadBufferBits (NABuffer* buffer, NABufInt count){
  uint32 retint = 0;
  NABool curbit;
  uint32 curmask = 1;
  while(count){
    curbit = naReadBufferBit(buffer);
    retint |= curmask * curbit;
    curmask <<= 1;
    count--;
  }
  return retint;

//  #ifndef NDEBUG
//    if(!naCanBufferInput(buffer))
//      naError("naReadBufferBits", "Buffer is not a input buffer.");
//    if(!count)
//      naError("naReadBufferBits", "Reading zero bits.");
//    if(count > 32)
//      naError("naReadBufferBits", "Can not read more than 32 bits at once.");
//  #endif
//  uint8 curbit = buffer->readbit;
//  uint8 lastbytebits = count % 8;
//  NAUInt requiredbitcount = curbit + count;
//  NAUInt lastbytemodulobits = requiredbitcount % 8;
//  NAUInt requiredbytecount = requiredbitcount / 8;
//  if(lastbytemodulobits){requiredbytecount++;}
//  const NAByte* src = naRequireBufferReadBytes(buffer, requiredbytecount, NA_TRUE, lastbytemodulobits);
//
//  uint32 retint = 0;
//  NAByte* buf = (NAByte*)(&retint);
//  buf += 3;
//
//  while(1){
//    *buf = (*src >> curbit);
//    if(count <= (8 - curbit)){break;}
//    count -= (8 - curbit);
//    src++;
//    *buf |= (*src << (8 - curbit));
//    if(count <= curbit){break;}
//    count -= curbit;
//    buf--;
//  }
//
//  if(lastbytebits){
//    *buf = ((*buf << (8 - lastbytebits)) & 0xff) >> (8 - lastbytebits);
//  }
//  
//  naConvertNativeBig32(&retint);
//  return retint;
}


NA_DEF void naPadBufferReadBits(NABuffer* buffer){
  if(buffer->readbit != 0){
    #ifndef NDEBUG
      if(buffer->flags & NA_BUFFER_FLAG_SECURE){
        NAByte testbyte = naReadBufferUInt8(buffer);
        testbyte >>= buffer->readbit;
        if(testbyte != 0)
          naError("naPadBufferReadBits", "Padding Bits not Null in secure buffer.");
      }
    #endif
    buffer->readbit = 0;
    buffer->readpos++;
  }
}



// Note that creating a helper function for reading small number of bytes is
// not really useful as there are too many things which can go wrong. Instead,
// the author decided to simply implement all functions plainly.

NA_DEF int8 naReadBufferInt8(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int8 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferInt8", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferInt8", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 1, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 1, 0);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF int16 naReadBufferInt16(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int16 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferInt16", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 2, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 2, 0);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF int32 naReadBufferInt32(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int32 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferInt32", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 4, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 4, 0);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF int64 naReadBufferInt64(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int64 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferInt64", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 8, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 8, 0);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF uint8 naReadBufferUInt8(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint8 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferUInt8", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferUInt8", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 1, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 1, 0);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF uint16 naReadBufferUInt16(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint16 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferUInt16", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferUInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 2, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 2, 0);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF uint32 naReadBufferUInt32(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint32 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferUInt32", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferUInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 4, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 4, 0);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF uint64 naReadBufferUInt64(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint64 value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferUInt64", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferUInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 8, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 8, 0);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF float naReadBufferFloat(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  float value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferFloat", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferFloat", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 4, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 4, 0);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF double naReadBufferDouble(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  double value;
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naReadBufferDouble", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferDouble", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 8, NA_FALSE);
  naRetrieveBufferBytes(buffer, &value, 8, 0);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF void naReadBufferArrayInt8(NABuffer* buffer, int8* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt8", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayInt8", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayInt8", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 1, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 1, 0);
  buffer->converter.convertArray8(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayInt16(NABuffer* buffer, int16* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt16", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayInt16", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 2, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 2, 0);
  buffer->converter.convertArray16(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayInt32(NABuffer* buffer, int32* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt32", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayInt32", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 4, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 4, 0);
  buffer->converter.convertArray32(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayInt64(NABuffer* buffer, int64* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt64", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayInt64", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 8, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 8, 0);
  buffer->converter.convertArray64(buf, (NAUInt)count);
}



NA_DEF void naReadBufferArrayUInt8(NABuffer* buffer, uint8* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt8", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayUInt8", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayUInt8", "Bit offset not 0.");
  #endif
//  if(buffer->readpos + count > 4096){
//    printf("asdf");
//  }
  naEnhanceBuffer(buffer, count * 1, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 1, 0);
  buffer->converter.convertArray8(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayUInt16(NABuffer* buffer, uint16* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt16", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayUInt16", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayUInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 2, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 2, 0);
  buffer->converter.convertArray16(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayUInt32(NABuffer* buffer, uint32* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt32", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayUInt32", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayUInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 4, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 4, 0);
  buffer->converter.convertArray32(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayUInt64(NABuffer* buffer, uint64* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt64", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayUInt64", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayUInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 8, NA_TRUE);
  naRetrieveBufferBytes(buffer, buf, count * 8, 0);
  buffer->converter.convertArray64(buf, (NAUInt)count);
}



NA_DEF void naReadBufferArrayFloat(NABuffer* buffer, float* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayFloat", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayFloat", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayFloat", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 4, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 4, 0);
  buffer->converter.convertArray32(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayDouble(NABuffer* buffer, double* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayDouble", "buf is Null pointer.");
    if(!naCanBufferInput(buffer))
      naError("naReadBufferArrayDouble", "Buffer is not an input-buffer.");
    if(buffer->readbit != 0)
      naError("naReadBufferArrayDouble", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 8, NA_FALSE);
  naRetrieveBufferBytes(buffer, buf, count * 8, 0);
  buffer->converter.convertArray64(buf, (NAUInt)count);
}





// /////////////////////////////////////////////////////////////////
// WRITING
// /////////////////////////////////////////////////////////////////





NA_DEF void naStoreBufferBytes(NABuffer* buffer, const void* ptr, NABufInt count){
  NAList* buflist;
  
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naStoreBufferBytes", "Buffer is not an output-buffer.");
    if(!count)
      naError("naStoreBufferBytes", "Writing zero bytes.");
    if(count < 0)
      naError("naStoreBufferBytes", "Negative count.");
  #endif
  if(!count){return;}

  buflist = naGetPointerMutable(buffer->storage);
  
  while(count){
    NABufferPart* curpart;
    NABufInt remainingbytes;
    NAByte* dst;
    
    #ifndef NDEBUG
      if(!buffer->writepartposition)
        naError("naStoreBufferBytes", "Internal error: No buffer part available.");
    #endif
    curpart = naGetListPositionMutable(buflist, buffer->writepartposition);
    #ifndef NDEBUG
      if(!curpart || !naIsPosInBufferPart(curpart, buffer->writepos))
        naError("naStoreBufferBytes", "Internal error: Current buffer part inavailable or not containing current position.");
    #endif
    if(!curpart){return;}
    remainingbytes = naGetBufferPartEndIndex(curpart) - buffer->writepos;  // todo: this should be the remaining size of used bytes
    dst = naGetBufferPartDataPointer(curpart, buffer->writepos);
    if(count > remainingbytes){
      naCopyn(dst, ptr, (NAUInt)remainingbytes);
      buffer->writepos += remainingbytes;
      buffer->writepartposition = naGetListPositionNext(buflist, buffer->writepartposition);
      *((NAByte**)(&ptr)) += remainingbytes;
      count -= remainingbytes;
    }else{
      naCopyn(dst, ptr, (NAUInt)count);
      buffer->writepos += count;
      *((NAByte**)(&ptr)) += count;
      count -= count;
    }
  }
}



NA_DEF void naWriteBufferBytes(NABuffer* buffer, const void* ptr, NABufInt count){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferBytes", "Buffer is not an output-buffer.");
    if(!count)
      naError("naReadBufferBytes", "Writing zero bytes.");
    if(count < 0)
      naError("naReadBufferBytes", "Negative count.");
  #endif
  if(!count){return;}
  naEnhanceBuffer(buffer, count, NA_TRUE);
  naStoreBufferBytes(buffer, ptr, count);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferByteArray(NABuffer* buffer, const NAByteArray* bytearray){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferByteArray", "Buffer is not an output-buffer.");
  #endif
  naWriteBufferBytes(buffer, naGetByteArrayConstPointer(bytearray), naGetByteArraySize(bytearray));
}



NA_DEF void naWriteBufferString(NABuffer* buffer, const NAString* string){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferString", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, naGetStringSize(string), NA_TRUE);
  naStoreBufferBytes(buffer, naGetStringUTF8Pointer(string), naGetStringSize(string));
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferStringWithFormat(NABuffer* buffer,
                       const NAUTF8Char* format,
                                         ...){
  // Declaration before implementation. Needed for C90.
  va_list argumentlist;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferStringWithFormat", "Buffer is not an output-buffer.");
  #endif
  va_start(argumentlist, format);
  naWriteBufferStringWithArguments(buffer, format, argumentlist);
  va_end(argumentlist);
}



NA_DEF void naWriteBufferStringWithArguments(NABuffer* buffer,
                          const NAUTF8Char* format,
                                    va_list argumentlist){
  // Declaration before implementation. Needed for C90.
  NAString* string;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferStringWithArguments", "Buffer is not an output-buffer.");
  #endif
  string = naNewStringWithArguments(format, argumentlist);
  naWriteBufferString(buffer, string);
  naDelete(string);
}



NA_DEF void naWriteBufferBuffer(NABuffer* dstbuffer, NABuffer* srcbuffer, NABufInt count){
  NAList* srcbuflist;
  NABufferPart* curpart;
  
  #ifndef NDEBUG
    if(count < 0)
      naError("naWriteBufferBuffer", "count should be >= 0.");
  #endif
  if(count == 0){return;}
  srcbuflist = naGetPointerMutable(srcbuffer->storage);
//  NABufInt countcopy = count;
//  #ifndef NDEBUG
//    if(naIsPosInBufferPart(naGetListCurrentMutable(srcbuflist), srcbuffer->readpos))
//      naError("naWriteBufferBuffer", "Internal error: position is not in the current buffer part.");
//  #endif
//  naFirstList(srcbuflist);
  naLocateBufferList(srcbuflist, srcbuffer->readpos);
  while((curpart = naIterateListMutable(srcbuflist, 1))){
    // todo: This might not be correct in all cases.
    NABufInt remainingsize = naGetBufferPartEndIndex(curpart) - srcbuffer->readpos;
    void* srcptr = naGetBufferPartDataPointer(curpart, srcbuffer->readpos);
    if(remainingsize > count){
      naWriteBufferBytes(dstbuffer, srcptr, count);
      srcbuffer->readpos += count;
      count = 0;
    }else{
      naWriteBufferBytes(dstbuffer, srcptr, remainingsize);
      srcbuffer->readpos += remainingsize;
      count -= remainingsize;
    }
    if(count == 0){break;}
  }
}



NA_DEF void naRepeatBufferBytes(NABuffer* buffer, NABufInt count){
  #ifndef NDEBUG
    if(!naCanBufferInput(buffer))
      naError("naRepeatBufferBytes", "Buffer can not read.");
    if(!naCanBufferOutput(buffer))
      naError("naRepeatBufferBytes", "Buffer can not write.");
    if(count <= 0)
      naError("naRepeatBufferBytes", "count should be > 0.");
  #endif
  // todo: make this faster.
  while(count){
//    if(buffer->writepos > 4094){
//      int asdf = 5;
//      printf("%d\n", buffer->writepos);
//    }
    NAByte curbyte = naReadBufferUInt8(buffer);
    naWriteBufferUInt8(buffer, curbyte);
    count--;
  }
}



NA_DEF void naWriteBufferInt8(NABuffer* buffer, int8 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferInt8", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert8(&value);
  naEnhanceBuffer(buffer, 1, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 1);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferInt16(NABuffer* buffer, int16 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferInt16", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert16(&value);
  naEnhanceBuffer(buffer, 2, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 2);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferInt32(NABuffer* buffer, int32 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferInt32", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert32(&value);
  naEnhanceBuffer(buffer, 4, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 4);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferInt64(NABuffer* buffer, int64 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferInt64", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert64(&value);
  naEnhanceBuffer(buffer, 8, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 8);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}


NA_DEF void naWriteBufferUInt8(NABuffer* buffer, uint8 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferUInt8", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert8(&value);
  naEnhanceBuffer(buffer, 1, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 1);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferUInt16(NABuffer* buffer, uint16 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferUInt16", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert16(&value);
  naEnhanceBuffer(buffer, 2, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 2);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferUInt32(NABuffer* buffer, uint32 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferUInt32", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert32(&value);
  naEnhanceBuffer(buffer, 4, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 4);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferUInt64(NABuffer* buffer, uint64 value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferUInt64", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert64(&value);
  naEnhanceBuffer(buffer, 8, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 8);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}


NA_DEF void naWriteBufferFloat(NABuffer* buffer, float value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferFloat", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert32(&value);
  naEnhanceBuffer(buffer, 4, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 4);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferDouble(NABuffer* buffer, double value){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferDouble", "Buffer is not an output-buffer.");
  #endif
  buffer->converter.convert64(&value);
  naEnhanceBuffer(buffer, 8, NA_TRUE);
  naStoreBufferBytes(buffer, &value, 8);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}









NA_DEF void naWriteBufferArrayInt8(NABuffer* buffer, const int8* buf, NABufInt count){
  int8 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayInt8", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 1, NA_TRUE);
  while(count){
    value = *((int8*)buf);
    buffer->converter.convert8(&value);
    naStoreBufferBytes(buffer, &value, 1);
    buf += 1;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayInt16(NABuffer* buffer, const int16* buf, NABufInt count){
  int16 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayInt16", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 2, NA_TRUE);
  while(count){
    value = *((int16*)buf);
    buffer->converter.convert16(&value);
    naStoreBufferBytes(buffer, &value, 2);
    buf += 2;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayInt32(NABuffer* buffer, const int32* buf, NABufInt count){
  int32 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayInt32", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 4, NA_TRUE);
  while(count){
    value = *((int32*)buf);
    buffer->converter.convert32(&value);
    naStoreBufferBytes(buffer, &value, 4);
    buf += 4;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayInt64(NABuffer* buffer, const int64* buf, NABufInt count){
  int64 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayInt64", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 8, NA_TRUE);
  while(count){
    value = *((int64*)buf);
    buffer->converter.convert64(&value);
    naStoreBufferBytes(buffer, &value, 8);
    buf += 8;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayUInt8(NABuffer* buffer, const uint8* buf, NABufInt count){
  uint8 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayUInt8", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 1, NA_TRUE);
  while(count){
    value = *((uint8*)buf);
    buffer->converter.convert8(&value);
    naStoreBufferBytes(buffer, &value, 1);
    buf += 1;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayUInt16(NABuffer* buffer, const uint16* buf, NABufInt count){
  uint16 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayUInt16", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 2, NA_TRUE);
  while(count){
    value = *((uint16*)buf);
    buffer->converter.convert16(&value);
    naStoreBufferBytes(buffer, &value, 2);
    buf += 2;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayUInt32(NABuffer* buffer, const uint32* buf, NABufInt count){
  uint32 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayUInt32", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 4, NA_TRUE);
  while(count){
    value = *((uint32*)buf);
    buffer->converter.convert32(&value);
    naStoreBufferBytes(buffer, &value, 4);
    buf += 4;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayUInt64(NABuffer* buffer, const uint64* buf, NABufInt count){
  uint64 value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayUInt64", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 8, NA_TRUE);
  while(count){
    value = *((uint64*)buf);
    buffer->converter.convert64(&value);
    naStoreBufferBytes(buffer, &value, 8);
    buf += 8;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayFloat(NABuffer* buffer, const float* buf, NABufInt count){
  float value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayFloat", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 4, NA_TRUE);
  while(count){
    value = *((float*)buf);
    buffer->converter.convert32(&value);
    naStoreBufferBytes(buffer, &value, 4);
    buf += 4;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferArrayDouble(NABuffer* buffer, const double* buf, NABufInt count){
  double value;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferArrayDouble", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, count * 8, NA_TRUE);
  while(count){
    value = *((double*)buf);
    buffer->converter.convert64(&value);
    naStoreBufferBytes(buffer, &value, 8);
    buf += 8;
    count--;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}




NA_DEF void naWriteBufferTab(NABuffer* buffer){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferTab", "Buffer is not an output-buffer.");
  #endif
  naEnhanceBuffer(buffer, 1, NA_TRUE);
  naStoreBufferBytes(buffer, NA_TAB, 1);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferNewLine(NABuffer* buffer){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferNewLine", "Buffer is not an output-buffer.");
  #endif
  switch(buffer->flags & NA_BUFFER_FLAG_NEWLINE_MASK){
  case NA_BUFFER_FLAG_NEWLINE_MAC9:
    naEnhanceBuffer(buffer, 1, NA_TRUE);
    naStoreBufferBytes(buffer, NA_NL_MAC9, 1);
    break;
  case NA_BUFFER_FLAG_NEWLINE_UNIX:
    naEnhanceBuffer(buffer, 1, NA_TRUE);
    naStoreBufferBytes(buffer, NA_NL_UNIX, 1);
    break;
  case NA_BUFFER_FLAG_NEWLINE_WIN:
    naEnhanceBuffer(buffer, 2, NA_TRUE);
    naStoreBufferBytes(buffer, NA_NL_WIN, 2);
    break;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
}





NA_DEF void naWriteBufferLine(NABuffer* buffer,
                          const NAString* string){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferLine", "Buffer is not an output-buffer.");
  #endif
  naWriteBufferString(buffer, string);
  naWriteBufferNewLine(buffer);
}
NA_DEF void naWriteBufferLineWithFormat(NABuffer* buffer,
                        const NAUTF8Char* format,
                                          ...){
  // Declaration before implementation. Needed for C90.
  va_list argumentlist;
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferLineWithFormat", "Buffer is not an output-buffer.");
  #endif
  va_start(argumentlist, format);
  naWriteBufferLineWithArguments(buffer, format, argumentlist);
  va_end(argumentlist);
}
NA_DEF void naWriteBufferLineWithArguments(NABuffer* buffer,
                        const NAUTF8Char* format,
                                  va_list argumentlist){
  #ifndef NDEBUG
    if(!naCanBufferOutput(buffer))
      naError("naWriteBufferLineWithArguments", "Buffer is not an output-buffer.");
  #endif
  naWriteBufferStringWithArguments(buffer, format, argumentlist);
  naWriteBufferNewLine(buffer);
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
