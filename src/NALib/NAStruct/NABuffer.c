
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "../NABuffer.h"
#include "../NAList.h"


#define NA_BUFFER_BYTESIZE 4096
#if (NA_BUFFER_BYTESIZE < 16)
  #error "Invalid file buffer bytesize"
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

// If maxpos is known, extend MUST be unset!
#define NA_BUFFER_FLAG_CAN_EXTEND           0x08
#define NA_BUFFER_FLAG_MAXPOS_KNOWN         0x10






// Defintion of basic buffer flags
#define NA_BUFFER_FLAG_CAN_SEEK             0x04


#define NA_BUFFER_FLAG_SECURE               0x20

//// Definition of the different kind of src and dst
#define NA_BUFFER_FLAG_SRCDST_BITSHIFT     8
#define NA_BUFFER_FLAG_SRCDST_BITSIZE      2
#define NA_BUFFER_FLAG_SOURCE_MASK         (((1 << NA_BUFFER_FLAG_SRCDST_BITSIZE) - 1) << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
#define NA_BUFFER_FLAG_SOURCE_NONE         (0x00 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
//#define NA_BUFFER_FLAG_SRCDST_VOID         (0x01 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
#define NA_BUFFER_FLAG_SOURCE_FILE         (0x02 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)
//#define NA_BUFFER_FLAG_SRCDST_BYTEARRAY    (0x03 << NA_BUFFER_FLAG_SRCDST_BITSHIFT)

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
  NAUInt refcount;
  NARangei bufferrange;
  NARangei datarange;
  NAByte* data;
};



NABufferPart* naAllocBufferPart(NARangei range, NABufInt datastartindex){
  NABufferPart* part = naAlloc(NABufferPart);
  part->refcount = 0;
  part->bufferrange = range;
  part->datarange = naMakeRangeiWithStartAndEnd(datastartindex, datastartindex);  // Note that this results in an empty range.
  part->data = naMalloc(part->bufferrange.length);
  return part;
}



NABufferPart* naAllocBufferPartSparse(NABufInt bufferstartindex, NABufInt bufferendindex){
  #ifndef NDEBUG
    if(bufferstartindex > bufferendindex)
      naError("naAllocBufferPartSparse", "end should be greater than start");
  #endif
  NABufferPart* part = naAlloc(NABufferPart);
  part->refcount = 0;
  part->bufferrange = naMakeRangeiWithStartAndEnd(bufferstartindex, bufferendindex);
  part->datarange = naMakeRangeiWithStartAndEnd(0, 0);
  part->data = NA_NULL;
  return part;
}



NA_HDEF NABool naIsBufferPartSparse(NABufferPart* part){
  return (part->data == NA_NULL);
}



void naDeallocBufferPart(NABufferPart* part){
  #ifndef NDEBUG
    if(!naIsBufferPartSparse(part) && part->refcount != 0)
      naError("naDeallocBufferPart", "Part is still in use");
  #endif
  naFree(part->data);
}



// Returns the start index of the buffer part.
NA_HDEF NABufInt naGetBufferPartStartBufferIndex(NABufferPart* part){
  return part->bufferrange.origin;
}



// Returns the end index of the buffer part.
NA_HDEF NABufInt naGetBufferPartEndBufferIndex(NABufferPart* part){
  return naGetRangeiEnd(part->bufferrange);
}


// Returns the end index of the buffer part.
NA_HDEF NABufInt naGetBufferPartEndDataIndex(NABufferPart* part){
  return naGetRangeiEnd(part->datarange);
}



// Returns the max index of the buffer part.
NA_HDEF NABufInt naGetBufferPartMaxBufferIndex(NABufferPart* part){
  return naGetRangeiMax(part->bufferrange);
}



// Returns true, if the given absolute position is inside the buffer part.
NA_HDEF NABool naIsOffsetInBufferPart(NABufferPart* part, NABufInt offset){
  return naContainsRangeiOffset(part->bufferrange, offset);
}



// Advances the number of bytes being used in the buffer storage.
NA_HDEF void naAdjustBufferPartToContainDataRange(NABufferPart* part, NARangei range){
  #ifndef NDEBUG
    if(!naContainsRangeiRange(part->bufferrange, range))
      naError("naAdjustBufferPartToContainDataRange", "dataindex not contained in this buffer range");
  #endif
  if(naIsRangeiEmpty(part->datarange)){
    part->datarange = range;
  }else if(naContainsRangeiRange(part->datarange, range)){
    // Nothing to do.
  }else if(part->datarange.origin < range.origin){
    // Appending after.
    if(naGetRangeiEnd(part->datarange) < range.origin){
      // There is a gap.
      #ifndef NDEBUG
        naError("naAdjustBufferPartToContainDataRange", "Warning: Gap between used dataranges in buffer part apending after");
      #endif
    }
    part->datarange = naMakeRangeiWithStartAndEnd(part->datarange.origin, naGetRangeiEnd(range));
  }else{
    // Appending before
    if(naGetRangeiEnd(range) < part->datarange.origin){
      // There is a gap.
      #ifndef NDEBUG
        naError("naAdjustBufferPartToContainDataRange", "Warning: Gap between used dataranges in buffer part appending before");
      #endif
    }
    part->datarange = naMakeRangeiWithStartAndEnd(range.origin, naGetRangeiEnd(part->datarange));
  }
}



// Returns a direct pointer to the raw data of this buffer part, given its
// absolute address.
NA_HDEF NAByte* naGetBufferPartDataPointer(NABufferPart* part, NABufInt offset){
  #ifndef NDEBUG
    if(!naIsOffsetInBufferPart(part, offset))
      naError("naGetBufferPartDataPointer", "abspos not inside this buffer part");
    if(!naContainsRangeiOffset(part->datarange, offset))
      naError("naGetBufferPartDataPointer", "abspos not inside this buffers data part");
  #endif
  return &(part->data[offset - part->bufferrange.origin]);
}



// Clears a buffer part.
NA_HIDEF void naClearBufferPart(NABufferPart* part){
  naFree(part->data);
}



// Clears all buffer parts in the list and clears the list itself.
NA_HIDEF void naClearBufferPartList(NAList* partlist){
  #ifndef NDEBUG
    if(naGetListCount(partlist) > 1)
      naError("naClearBufferPartList", "there are still parts active");
    if((naGetListCount(partlist) == 1) && !naIsBufferPartSparse(naGetListFirstMutable(partlist)))
      naError("naClearBufferPartList", "internal error: The remaining part is non-sparse.");
  #endif
  naRewindList(partlist);
  while(naIterateList(partlist, 1)){
    NABufferPart* part = naGetListCurrentMutable(partlist);
    naClearBufferPart(part);
  }
  naClearList(partlist);
}



NA_HIDEF NAListPos naLocateBufferPartList(NAList* partlist, NABufInt offset){
  NABufferPart* part;

  // If the list is empty, we simply return NA_NULL.
  if(naIsListEmpty(partlist)){return NA_NULL;}

  part = naGetListCurrentMutable(partlist);
  if(!naContainsRangeiOffset(((NABufferPart*)(part))->bufferrange, offset)){
    // We expect the current part to be above the desired offset or the pointer
    // to be NA_NULL. If that is not the case, we search forward, otherwise,
    // se search backward.
    if(part && (naGetBufferPartMaxBufferIndex(part) < offset)){
      // Search forward.
      do{
        naIterateList(partlist, 1);
        part = naGetListCurrentMutable(partlist);
      } while(part && (naGetBufferPartMaxBufferIndex(part) < offset));
      // When searching forward and reaching the end of the list, we need to
      // iterate back one step to really have the last part before the desired
      // part.
      if(!part){
        naIterateList(partlist, -1);
        part = naGetListCurrentMutable(partlist);
      }
    }else{
      // Search backward.
      do{
        naIterateList(partlist, -1);
        part = naGetListCurrentMutable(partlist);
      } while(part && (naGetBufferPartStartBufferIndex(part) > offset));
    }
  }

  // Now, we are sure that curpart either contains abspos or is the one element
  // before that (which even might be NA_NULL). We return the list pos.
  return naGetListCurrentPosition(partlist);
}




// //////////////////////////////////////////////////
// NABuffer
// //////////////////////////////////////////////////




NA_DEF NABuffer* naInitBufferInputtingFromFile(NABuffer* buffer, const char* filename){
  
  buffer->flags = NA_BUFFER_FLAG_SOURCE_FILE;
  buffer->source = naAlloc(NAFile);
  *((NAFile*)(buffer->source)) = naMakeFileReadingFilename(filename);
  buffer->storage = naNewPointer(naAlloc(NAList), NA_MEMORY_CLEANUP_FREE, (NAFunc)naClearBufferPartList);
  naInitList(naGetPointerMutable(buffer->storage));
  
  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);

  buffer->curlistpos = NA_NULL;
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

//  buffer->flags = 0;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;

  return buffer;
}



//NA_DEF NABuffer* naInitBufferInputtingFromByteArray(NABuffer* buffer, const NAByteArray* array){
//  NAList* bufpartlist;
//  NAByteArray* srcdstarray;
//
//  #ifndef NDEBUG
//    if(naIsByteArrayEmpty(array))
//      naError("naInitBufferInputtingFromByteArray", "Array is empty");
//  #endif
//  bufpartlist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointer(bufpartlist, NA_MEMORY_CLEANUP_FREE, (NAFunc)naClearBufferPartList);
//
//  srcdstarray = naAlloc(NAByteArray);
//  naInitByteArrayExtraction(srcdstarray, array, 0, -1);
//  buffer->srcdst = naNewPointer(srcdstarray, NA_MEMORY_CLEANUP_FREE, (NAFunc)naClearByteArray);
//
//  buffer->minpos = 0;
//  buffer->maxpos = (NABufInt)naMakeMaxWithEndi(naGetByteArrayBytesize(array));
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
//  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
//  buffer->flags |= NA_BUFFER_FLAG_MAXPOS_KNOWN;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_BYTEARRAY;
//
//  return buffer;
//}



NA_DEF NABuffer* naInitBufferInputtingFromBufferExtraction(NABuffer* buffer, NABuffer* srcbuffer, NABufInt bytesize){
  NAList* partlist;

  buffer->flags = srcbuffer->flags;
  buffer->source = srcbuffer->source;
  buffer->storage = naRetainPointer(srcbuffer->storage);
  partlist = naGetPointerMutable(buffer->storage);
  
  buffer->range = naMakeRangeiE(srcbuffer->curoffset, bytesize);

  buffer->curlistpos = naLocateBufferPartList(partlist, buffer->range.origin);
  buffer->curoffset = buffer->range.origin;
  buffer->curbit = 0;

  buffer->endianness = srcbuffer->endianness;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

//  buffer->flags = 0;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;

  return buffer;
}



NA_DEF NABuffer* naInitBuffer(NABuffer* buffer){
  buffer->flags = NA_BUFFER_FLAG_SOURCE_NONE | NA_BUFFER_FLAG_CAN_EXTEND;
  buffer->source = NA_NULL;
  buffer->storage = naNewPointer(naAlloc(NAList), NA_MEMORY_CLEANUP_FREE, (NAFunc)naClearBufferPartList);
  naInitList(naGetPointerMutable(buffer->storage));

  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);

  buffer->curlistpos = NA_NULL;
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);


//  buffer->flags = 0;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_SEEK;
//  buffer->flags |= NA_BUFFER_FLAG_CAN_EXTEND;
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_NONE;

  return buffer;
}



//NA_DEF NABuffer* naInitBufferOutputtingToFile(NABuffer* buffer, const char* filename, NAFileMode mode){
//  NAList* bufpartlist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointer(bufpartlist, NA_MEMORY_CLEANUP_FREE, naClearList);
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
//  NAList* bufpartlist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointer(bufpartlist, NA_MEMORY_CLEANUP_FREE, naClearList);
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
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferOutputtingToStderr(NABuffer* buffer){
//  NAList* bufpartlist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointer(bufpartlist, NA_MEMORY_CLEANUP_FREE, naClearList);
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
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_FILE;
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferOutputtingToVoid(NABuffer* buffer){
//  NAList* bufpartlist = naInitList(naAlloc(NAList));
//  buffer->storage = naNewPointer(bufpartlist, NA_MEMORY_CLEANUP_FREE, naClearList);
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
//  buffer->flags |= NA_BUFFER_FLAG_SRCDST_VOID;
//
//  return buffer;
//}





NA_DEF void naFlushBuffer(NABuffer* buffer){
  NA_UNUSED(buffer);
//  if(naGetBufferKind(buffer) == NA_BUFFER_KIND_OUTPUT){
//    if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_NONE){
//      return;
//    }
//    naFirstList(&(buffer->buflist));
//    NABuf* curbuf;
//    while((curbuf = naIterateListMutable(&(buffer->buflist), 1))){
//      #ifndef NDEBUG
//        if(!naGetBufUsedBytesize(curbuf))
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
//        naWriteFileBytes(buffer->srcdst, naGetBufConstFirstPointer(curbuf), naGetBufUsedBytesize(curbuf));
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
  switch(buffer->flags & NA_BUFFER_FLAG_SOURCE_MASK){
  case NA_BUFFER_FLAG_SOURCE_NONE:
    // Nothing to do here.
    break;
  case NA_BUFFER_FLAG_SOURCE_FILE:
    naCloseFile(buffer->source);
    naFree(buffer->source);
    break;
  default:
    #ifndef NDEBUG
      naError("naClearBuffer", "Invalid source flag.");
    #endif
    break;
  }
  
  naReleasePointer(buffer->storage);
}




NA_DEF NABool naIsBufferEmpty(const NABuffer* buffer){
  return naIsRangeiEmpty(buffer->range);
}



//NA_DEF NABool naCanBufferSeek(const NABuffer* buffer){
//  return ((buffer->flags & NA_BUFFER_FLAG_CAN_SEEK) == NA_BUFFER_FLAG_CAN_SEEK);
//}

NA_DEF NABool naCanBufferExtend(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_CAN_EXTEND) == NA_BUFFER_FLAG_CAN_EXTEND);
}
//
//
//
//
//
//
//NA_API NABool naIsBufferSecure(const NABuffer* buffer){
//  return ((buffer->flags & NA_BUFFER_FLAG_SECURE) == NA_BUFFER_FLAG_SECURE);
//}
//
//
//
NA_DEF NABufInt naGetBufferCurOffsetAbsolute(const NABuffer* buffer){
  return buffer->curoffset;
}



NA_API NABool naHasBufferDeterminedBytesize(const NABuffer* buffer){
  return ((buffer->flags & NA_BUFFER_FLAG_MAXPOS_KNOWN) == NA_BUFFER_FLAG_MAXPOS_KNOWN);
}


NA_API NABufInt naDetermineBufferBytesize(NABuffer* buffer){
  if(!naHasBufferDeterminedBytesize(buffer)){
    switch(buffer->flags & NA_BUFFER_FLAG_SOURCE_MASK){
    case NA_BUFFER_FLAG_SOURCE_NONE:
      break;
    case NA_BUFFER_FLAG_SOURCE_FILE:
      buffer->range.length = naComputeFileBytesize((NAFile*)(buffer->source));
      buffer->range.origin = 0;
      break;
    }
    buffer->flags |= NA_BUFFER_FLAG_MAXPOS_KNOWN;
    buffer->flags &= ~NA_BUFFER_FLAG_CAN_EXTEND;
  }
  return buffer->range.length;
}



NA_DEF NABool naIsBufferReadAtEnd(const NABuffer* buffer){
  #ifndef NDEBUG
    if(!naHasBufferDeterminedBytesize(buffer))
      naError("naIsBufferReadAtEnd", "Buffer has no fixed max position. Use naDetermineBufferBytesize.");
  #endif
  return (buffer->curoffset > naGetRangeiMax(buffer->range));
}



NA_DEF void naWriteBufferToFile(NABuffer* buffer, NAFile* file, NABool atabsoluteposition){
//  NAList* buflist;
//  #ifndef NDEBUG
//    if(atabsoluteposition)
//      naError("naWriteBufferToFile", "atabsoluteposition must be NA_FALSE for now. Will be implemented later.");
//      // todo.
//  #else
//    NA_UNUSED(atabsoluteposition);
//  #endif
//  buflist = naGetPointerMutable(buffer->storage);
//  naRewindList(buflist);
//  while(naIterateList(buflist, 1)){
//    NABufferPart* curpart = naGetListCurrentMutable(buflist);
//    #ifndef NDEBUG
//        if ((NA_SYSTEM_INT_BITS < 64) && (naGetBufUsedBytesize(&(curpart->buf)) > NA_INT_MAX))
//          naError("naWriteBufferToFile", "buf size overflows int range, while int has less than 64 bits in this configuration.");
//    #endif
//    // todo: This might not be correct in all cases.
//    naWriteFileBytes(file, naGetBufConstFirstPointer(&(curpart->buf)), (NAInt)naGetBufUsedBytesize(&(curpart->buf)));
//  }
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


//NA_DEF void naSetBufferSecure(NABuffer* buffer, NABool secure){
//  if(secure){
//    buffer->flags |= NA_BUFFER_FLAG_SECURE;
//  }else{
//    buffer->flags &= ~NA_BUFFER_FLAG_SECURE;
//  }
//}



NA_DEF void naSeekBufferAbsolute(NABuffer* buffer, NABufInt offset){
  NABufInt absoffset = offset;
  #ifndef NDEBUG
    if((buffer->flags & NA_BUFFER_FLAG_SOURCE_FILE) && (absoffset < 0))
      naError("naSeekBufferAbsolute", "absolute offset of file buffer is negative");
    if(naHasBufferDeterminedBytesize(buffer) && !naContainsRangeiOffset(buffer->range, absoffset))
      naError("naSeekBufferAbsolute", "absolute offset lies outside of this buffer");
  #endif
  NAList* partlist = naGetPointerMutable(buffer->storage);
  buffer->curoffset = absoffset;
  naLocateBufferPartList(partlist, buffer->curoffset);
}



NA_DEF void naSeekBufferLocal(NABuffer* buffer, NABufInt offset){
  NABufInt absoffset = buffer->range.origin + offset;
  #ifndef NDEBUG
    if((buffer->flags & NA_BUFFER_FLAG_SOURCE_FILE) && (absoffset < 0))
      naError("naSeekBufferLocal", "absolute offset of file buffer is negative");
    if(naHasBufferDeterminedBytesize(buffer) && !naContainsRangeiOffset(buffer->range, absoffset))
      naError("naSeekBufferLocal", "absolute offset lies outside of this buffer");
  #endif
  naSeekBufferAbsolute(buffer, absoffset);
}



NA_DEF void naSeekBufferRelative(NABuffer* buffer, NABufInt offset){
  NABufInt absoffset = buffer->curoffset + offset;
  #ifndef NDEBUG
    if((buffer->flags & NA_BUFFER_FLAG_SOURCE_FILE) && (absoffset < 0))
      naError("naSeekBufferRelative", "absolute offset of file buffer is negative");
    if(naHasBufferDeterminedBytesize(buffer) && !naContainsRangeiOffset(buffer->range, absoffset))
      naError("naSeekBufferRelative", "absolute offset lies outside of this buffer");
  #endif
  naSeekBufferAbsolute(buffer, absoffset);
}





// Helper functions not visible to the programmer. Will make sure that there
// are enough bytes in the buffer.
// These functions are hidden to the user as they are just supplementary helper
// functions which prepare the NABuffer struct internally for an appropriate
// action. Therefore the user shall not use it. Of course, in NALib hiding
// something has not much use but it shows where the hidden attribute makes
// sense.
// Note that this is quite a heavy function. But in 99% of all cases, this
// function only executes about 10-20 statements. Nonteless, it ensures safe
// usage of any buffer.
NA_DEF void naEnhanceBuffer(NABuffer* buffer, NABufInt bytesize){
  NABufInt curoffset;
  NABufferPart* curpart;
  NAList* partlist = naGetPointerMutable(buffer->storage);
  NAListPos curlistpos;

  #ifndef NDEBUG
    if(!bytesize)
      naError("naEnhanceBuffer", "bytesize is zero.");
    if(bytesize < 0)
      naError("naEnhanceBuffer", "bytesize is negative.");
  #endif

  // Set curpos to the current byte position.
  curoffset = buffer->curoffset;
  curlistpos = buffer->curlistpos;
  
  // Adjust the total buffer range if applicable
  if(naCanBufferExtend(buffer) && !naHasBufferDeterminedBytesize(buffer)){
    buffer->range = naMakeRangeiWithRangeAndRange(buffer->range, naMakeRangei(curoffset, bytesize));
  }else{
    #ifndef NDEBUG
      if(!naContainsRangeiRange(buffer->range, naMakeRangei(curoffset, bytesize)))
        naError("naEnhanceBuffer", "enhancement goes beyound fixed buffer boundary.");
    #endif
  }

  // We move the list to the desired read / write position.
  if(curlistpos){
    naPositionList(partlist, curlistpos);
  }else{
    // There is no information about where to put the new buffer part. This
    // either happends when the bufferpart list is empty or if the buffer
    // was filled up to the last byte and now curpart is NA_NULL because the
    // bufferpart list was iterated beyound the last element or when the
    // a jump has been performed.

    // We try to search for an already existing part.
    curlistpos = naLocateBufferPartList(partlist, curoffset);
    // After this function, curpartpointer either is the part where curoffset
    // is within or the part before the desired part (which might be NA_NULL)
    // if there is no such part.
  }
  curpart = curlistpos ? naGetListCurrentMutable(partlist) : NA_NULL;

  // While there are still bytes to be enhanced...
  while(bytesize){
  
    // If there is a current part and the current position is within and it
    // is no sparse buffer part...
    if(curpart && !naIsBufferPartSparse(curpart) && naIsOffsetInBufferPart(curpart, curoffset)){
      // We are now sure that curpartpointer points to the buffer part which
      // must contain the start of our enhancement.
      
      // We compute the remaining bytesize in the buffer part.
      NABufInt remainingbytesize = naGetBufferPartEndBufferIndex(curpart) - curoffset;
      if(bytesize <= remainingbytesize){
        // We have sufficient space in this buf.
        // Adjust the data range of the buffer part.
        naAdjustBufferPartToContainDataRange(curpart, naMakeRangei(curoffset, bytesize));
        // We are done. No more bytes to enhance, the buffer part is updated.
        // We set bytesize to 0 to end the loop.
        bytesize = 0;
      
      }else{
        // The desired content does not fit completely. We use all of the
        // remaining bytes in the current buffer.
        // Adjust the data range of the buffer part.
        naAdjustBufferPartToContainDataRange(curpart, naMakeRangei(curoffset, remainingbytesize));
        curoffset += remainingbytesize;
        bytesize -= remainingbytesize;
        
        // If there is a next bufferpart, we iterate to it so that in the
        // next loop, we are already in the correct position.
        if(naGetListNextMutable(partlist)){
          naIterateList(partlist, 1);
          curlistpos = naGetListCurrentPosition(partlist);
          curpart = curlistpos ? naGetListCurrentMutable(partlist) : NA_NULL;
          // Note that this works because we always fill up the buffer from
          // leading to trailing. Additionally, sparse buffer parts fill up
          // any space in between of two parts which are not adjacent.
          #ifndef NDEBUG
            if(naGetBufferPartStartBufferIndex(curpart) != curoffset)
              naError("naEnhanceBuffer", "Buffer part is not adjacent to previous buffer part.");
          #endif
        }
      }
    
    }else{
      // Reaching here, curbufferpointer does not point to the buffer part
      // which shall contain the next few bytes. There are several things
      // we need to check.
      
      // We will probably create a new buffer part. We define the start and
      // end index of it.
      NARangei newbufferrange;
    
      // We define the default buffer part boundaries.
      if(curoffset < 0){
        newbufferrange = naMakeRangei(((curoffset / NA_BUFFER_BYTESIZE) - 1) * NA_BUFFER_BYTESIZE, NA_BUFFER_BYTESIZE);
      }else{
        newbufferrange = naMakeRangei(((curoffset / NA_BUFFER_BYTESIZE) - 0) * NA_BUFFER_BYTESIZE, NA_BUFFER_BYTESIZE);
      }
      
      // We clip the boundaries to the surrounding buffer parts if necessary.
      if(curpart){
        if(naIsOffsetInBufferPart(curpart, curoffset)){
          newbufferrange = naClampRangeiToRange(newbufferrange, curpart->bufferrange);
        }else{
          newbufferrange = naClampRangeiToMin(newbufferrange, naGetBufferPartEndBufferIndex(curpart));
        }
      }
      
      #ifndef NDEBUG
        if(!naIsRangeiUseful(newbufferrange))
          naError("naEnhanceBuffer", "New buffer range not useful");
      #endif
      
      if(curpart && naIsBufferPartSparse(curpart) && naIsOffsetInBufferPart(curpart, curoffset)){
        // If the current part contains curoffset but is sparse, we adjust
        // this sparse buffer part such that there is just the right amount
        // of space for a new non-sparse buffer part in the next loop.
        
        #ifndef NDEBUG
          if(!naContainsRangeiRange(curpart->bufferrange, newbufferrange))
            naError("naEnhanceBuffer", "Buffer part boundaries overlap");
        #endif
        
        if(naEqualRangei(newbufferrange, curpart->bufferrange)){
          // In this situation, the new buffer will completely replace the
          // sparse buffer. We just erase the sparse buffer.
          naDeallocBufferPart(naRemoveListCurrentMutable(partlist, NA_FALSE));
          
        }else if(newbufferrange.origin == naGetBufferPartStartBufferIndex(curpart)){
          // The new buffer must be placed before the sparse buffer.
          // We adjust the sparse part and iterate backwards such that
          // curpartpointer and curpart point to the previous part.
          curpart->bufferrange = naClampRangeiToMin(curpart->bufferrange, naGetRangeiEnd(newbufferrange));
          naIterateList(partlist, -1);
        
        }else if(naGetRangeiEnd(newbufferrange) == naGetBufferPartEndBufferIndex(curpart)){
          // The new buffer must be placed after the sparse buffer
          curpart->bufferrange = naClampRangeiToEnd(curpart->bufferrange, newbufferrange.origin);
        
        }else{
          // In this situation, the new buffer must be placed in the middle
          // of the existing sparse buffer part. We solve this by dividing
          // the sparse buffer into two sparse buffers and repeat the loop.

          // We create a new sparse buffer part after the desired part.
          naAddListAfterMutable(partlist, naAllocBufferPartSparse(naGetRangeiEnd(newbufferrange), naGetBufferPartEndBufferIndex(curpart)));
          // Adjust the current part
          curpart->bufferrange = naClampRangeiToEnd(curpart->bufferrange, newbufferrange.origin);
        }
      }
      
      // Reaching here, there surely is enough space in the list to fit the
      // new part in.

      // Before we create the new part and add it to the list, we find
      // out if there are some gaps between this new buffer and the ones
      // surrounding it.

      // First, let's check the gap with the previous part.
      if(curpart && (naGetBufferPartEndBufferIndex(curpart) != newbufferrange.origin)){
        // We create a sparse buffer part right after the curpart and right
        // before the new part.
        naAddListAfterMutable(partlist, naAllocBufferPartSparse(naGetBufferPartEndBufferIndex(curpart), newbufferrange.origin));
        // Now, we adjust the curpartpoitner to the new sparse part which is
        // now the one previous the new part.
        naIterateList(partlist, 1);
      }

      // We do the same thing for the next part.
      NABufferPart* nextpart = naIsListEmpty(partlist) ? NA_NULL : naGetListNextMutable(partlist);
      if(nextpart && (naGetBufferPartStartBufferIndex(nextpart) != naGetRangeiEnd(newbufferrange))){
        // We create a sparse buffer part right before the nextpart and right
        // after the new part.
        naAddListAfterMutable(partlist, naAllocBufferPartSparse(naGetRangeiEnd(newbufferrange), naGetBufferPartStartBufferIndex(nextpart)));
      }

      // Finally, we create a new non-sparse part.
      naAddListAfterMutable(partlist, naAllocBufferPart(newbufferrange, curoffset));
      // The list is currently located at the left position, therefore, we
      // add the new element after and position the list accordingly.
      naIterateList(partlist, 1);
      curlistpos = naGetListCurrentPosition(partlist);
      curpart = curlistpos ? naGetListCurrentMutable(partlist) : NA_NULL;

      // We have created a new buffer part. Now, we need to fill it.
      switch(buffer->flags & NA_BUFFER_FLAG_SOURCE_MASK){
      case NA_BUFFER_FLAG_SOURCE_NONE:
        // Nothing to do here.
        break;
      case NA_BUFFER_FLAG_SOURCE_FILE:
        {
          #ifndef NDEBUG
            if(newbufferrange.origin < 0)
              naError("naEnhanceBuffer", "Trying to read from a file with negative offset");
          #endif
          NAFile* file = (NAFile*)buffer->source;
          if(naTell(file->desc) != newbufferrange.origin){
            naSeekFileAbsolute(file, newbufferrange.origin);
          }
          naAdjustBufferPartToContainDataRange(curpart, newbufferrange);
          naReadFileBytes(file, naGetBufferPartDataPointer(curpart, newbufferrange.origin), newbufferrange.length);
        }
        break;
      default:
        #ifndef NDEBUG
          naError("naEnhanceBuffer", "Invalid source flag.");
        #endif
        break;
      }

    }
  }
  // Reaching here, the buffer is enhanced with all desired bytes and
  // ready to be used.

  // Set the buffer read or write position to the first byte prepared.
  if(buffer->curlistpos){
    naPositionList(partlist, buffer->curlistpos);
    curpart = naGetListCurrentMutable(partlist);
  }
  if(!buffer->curlistpos || !naContainsRangeiOffset(curpart->bufferrange, buffer->curoffset)){
    buffer->curlistpos = naLocateBufferPartList(partlist, buffer->curoffset);
  }

}





NA_DEF void naReadBuffer (NABuffer* buffer, NAFilesize bytecount){
  if(bytecount){
    naEnhanceBuffer(buffer, bytecount);
    buffer->curoffset += bytecount;
  }
  buffer->curlistpos = naLocateBufferPartList(naGetPointerMutable(buffer->storage), buffer->curoffset);
}



NA_HDEF void naReadBufferPart(NABuffer* buffer, NABufferPart* part){
//  NAFilesize desiredbytes;
//  NAFilesize readbytes;
//  #ifndef NDEBUG
//    if ((NA_FILESIZE_BITS < 64) && (naGetBufMaxBytesize(&(part->buf)) > NA_FILESIZE_MAX))
//      naError("naReadBufferPart", "buf size overflows filesize range, while a filesize has less than 64 bits in this configuration.");
//  #endif
//  desiredbytes = (NAFilesize)naGetBufMaxBytesize(&(part->buf));
//  readbytes = 0;
//  if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_FILE){
//    readbytes = naReadFileBytes(naGetPointerMutable(buffer->srcdst), naGetBufMutableFirstPointer(&(part->buf)), desiredbytes);
//  }else if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_NONE){
//    // Don't do anything
//  }else{
//    // todo
//    printf("not yet implemented");
//  }
//  if(naIsBufferSecure(buffer) && (readbytes < desiredbytes)){
//    naNulln64(&(((NAByte*)naGetBufMutableFirstPointer(&(part->buf)))[readbytes]), desiredbytes - readbytes);
//  }
}



NA_DEF void naAccumulateBufferToChecksum(NABuffer* buffer, NAChecksum* checksum){
  NABufInt bytesize;
  NABufInt curoffset;
  NAList* partlist;

  #ifndef NDEBUG
    if(!naHasBufferDeterminedBytesize(buffer))
      naError("naAccumulateBufferToChecksum", "Buffer has no known max position. Use naDetermineBufferBytesize or naDetermineBufferBytesize");
  #endif

  bytesize = buffer->range.length;
  if(!bytesize){return;}

  partlist = naGetPointerMutable(buffer->storage);
  curoffset = buffer->range.origin;
  naLocateBufferPartList(partlist, curoffset);
  
  while(bytesize){
    NABufferPart* curpart;
    NABufInt remainingbytes;
    NAByte* src;

    curpart = naGetListCurrentMutable(partlist);
    remainingbytes = naGetBufferPartEndBufferIndex(curpart) - curoffset;
    src = naGetBufferPartDataPointer(curpart, curoffset);
    
    if(bytesize > remainingbytes){
      naAccumulateChecksum(checksum, src, (NAInt)remainingbytes);
      naIterateList(partlist, 1);
      curoffset += remainingbytes;
      bytesize -= remainingbytes;
    }else{
      naAccumulateChecksum(checksum, src, (NAInt)bytesize);
      bytesize = 0;
    }
  }
  
}


// ///////////////////////////////////////////////////////////////
// READING
// ///////////////////////////////////////////////////////////////





NA_DEF void naRetrieveBufferBytes(NABuffer* buffer, void* ptr, NABufInt bytesize, NAUInt lastbytemodulobits){
  
  NAList* partlist = naGetPointerMutable(buffer->storage);

  #ifndef NDEBUG
    if(!bytesize)
      naError("naRetrieveBufferBytes", "Reading zero bytes.");
    if(bytesize < 0)
      naError("naRetrieveBufferBytes", "Negative count.");
  #endif
  if(!bytesize){return;}

  naPositionList(partlist, buffer->curlistpos);

  while(bytesize){
    NABufInt remainingbytes;
    NAByte* src;
    NABufferPart* curpart;

    #ifndef NDEBUG
      if(!buffer->curlistpos)
        naError("naRetrieveBufferBytes", "Internal error: No buffer part available.");
      if(buffer->curlistpos != naGetListCurrentPosition(partlist))
        naError("naRetrieveBufferBytes", "Internal error: cached read position does not correspond to list position.");
    #endif

    curpart = naGetListCurrentMutable(partlist);

    #ifndef NDEBUG
      if(!curpart || !naIsOffsetInBufferPart(curpart, buffer->curoffset))
        naError("naRetrieveBufferBytes", "Internal error: Current buffer part inavailable or not containing current position.");
    #endif
    
//    if(!buffer->curlistpos){return;} // security return. Only used for code sanity checks.
    remainingbytes = naGetBufferPartEndBufferIndex(curpart) - buffer->curoffset;
    src = naGetBufferPartDataPointer(curpart, buffer->curoffset);
    if(bytesize > remainingbytes){
      naCopyn(ptr, src, (NAUInt)remainingbytes);
      buffer->curoffset += remainingbytes;
      bytesize -= remainingbytes;
      *((NAByte**)(&ptr)) += remainingbytes;
      naIterateList(partlist, 1);
      buffer->curlistpos = naGetListCurrentPosition(partlist);
      curpart = naGetListCurrentMutable(partlist);
    }else{
      naCopyn(ptr, src, (NAUInt)bytesize);
      buffer->curoffset += bytesize;
      bytesize -= bytesize;
    }
  }
  
  buffer->curbit = (uint8)lastbytemodulobits;
  if(lastbytemodulobits){buffer->curoffset--;}
}




NA_DEF void naReadBufferBytes(NABuffer* buffer, void* buf, NABufInt bytesize){
  #ifndef NDEBUG
    if(!bytesize)
      naError("naReadBufferBytes", "Reading zero bytes.");
    if(bytesize < 0)
      naError("naReadBufferBytes", "Negative count.");
    if(buffer->curbit != 0)
      naError("naReadBufferBytes", "Bit offset not 0.");
  #endif
//  if(!count){return;}
  naEnhanceBuffer(buffer, bytesize);
  naRetrieveBufferBytes(buffer, buf, bytesize, 0);
}



//NA_DEF NAByteArray* naInitByteArrayFromBufferInput(NAByteArray* array, NABuffer* buffer, NAInt bytesize){
//  #ifndef NDEBUG
//    if(!count)
//      naError("naInitByteArrayFromBufferInput", "Reading zero bytes.");
//    if(count < 0)
//      naError("naInitByteArrayFromBufferInput", "Negative count.");
//    if(buffer->curbit != 0)
//      naError("naInitByteArrayFromBufferInput", "Bit offset not 0.");
//  #endif
//  if(!count){
//    naInitByteArray(array);
//  }else{
//    naInitByteArrayWithBytesize(array, count);
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

  curbit = buffer->curbit;
  requiredbitcount = curbit + 1;
  lastbytemodulobits = requiredbitcount % 8;

  naEnhanceBuffer(buffer, 1);
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
    retint |= curmask * (uint32)curbit;
    curmask <<= 1;
    count--;
  }
  return retint;

//  #ifndef NDEBUG
//    if(!count)
//      naError("naReadBufferBits", "Reading zero bits.");
//    if(count > 32)
//      naError("naReadBufferBits", "Can not read more than 32 bits at once.");
//  #endif
//  uint8 curbit = buffer->curbit;
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
  if(buffer->curbit != 0){
    #ifndef NDEBUG
      if(buffer->flags & NA_BUFFER_FLAG_SECURE){
        NAByte testbyte = naReadBufferUInt8(buffer);
        testbyte >>= buffer->curbit;
        if(testbyte != 0)
          naError("naPadBufferReadBits", "Padding Bits not Null in secure buffer.");
      }
    #endif
    buffer->curbit = 0;
    buffer->curoffset++;
  }
}



// Note that creating a helper function for reading small number of bytes is
// not really useful as there are too many things which can go wrong. Instead,
// the author decided to simply implement all functions plainly.

NA_DEF int8 naReadBufferInt8(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int8 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferInt8", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 1);
  naRetrieveBufferBytes(buffer, &value, 1, 0);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF int16 naReadBufferInt16(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int16 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 2);
  naRetrieveBufferBytes(buffer, &value, 2, 0);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF int32 naReadBufferInt32(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int32 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 4);
  naRetrieveBufferBytes(buffer, &value, 4, 0);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF int64 naReadBufferInt64(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int64 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 8);
  naRetrieveBufferBytes(buffer, &value, 8, 0);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF uint8 naReadBufferUInt8(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint8 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferUInt8", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 1);
  naRetrieveBufferBytes(buffer, &value, 1, 0);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF uint16 naReadBufferUInt16(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint16 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferUInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 2);
  naRetrieveBufferBytes(buffer, &value, 2, 0);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF uint32 naReadBufferUInt32(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint32 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferUInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 4);
  naRetrieveBufferBytes(buffer, &value, 4, 0);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF uint64 naReadBufferUInt64(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint64 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferUInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 8);
  naRetrieveBufferBytes(buffer, &value, 8, 0);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF float naReadBufferFloat(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  float value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferFloat", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 4);
  naRetrieveBufferBytes(buffer, &value, 4, 0);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF double naReadBufferDouble(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  double value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferDouble", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, 8);
  naRetrieveBufferBytes(buffer, &value, 8, 0);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF void naReadBufferArrayInt8(NABuffer* buffer, int8* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt8", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayInt8", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 1);
  naRetrieveBufferBytes(buffer, buf, count * 1, 0);
  buffer->converter.convertArray8(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayInt16(NABuffer* buffer, int16* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt16", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 2);
  naRetrieveBufferBytes(buffer, buf, count * 2, 0);
  buffer->converter.convertArray16(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayInt32(NABuffer* buffer, int32* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt32", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 4);
  naRetrieveBufferBytes(buffer, buf, count * 4, 0);
  buffer->converter.convertArray32(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayInt64(NABuffer* buffer, int64* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayInt64", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 8);
  naRetrieveBufferBytes(buffer, buf, count * 8, 0);
  buffer->converter.convertArray64(buf, (NAUInt)count);
}



NA_DEF void naReadBufferArrayUInt8(NABuffer* buffer, uint8* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt8", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayUInt8", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 1);
  naRetrieveBufferBytes(buffer, buf, count * 1, 0);
  buffer->converter.convertArray8(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayUInt16(NABuffer* buffer, uint16* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt16", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayUInt16", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 2);
  naRetrieveBufferBytes(buffer, buf, count * 2, 0);
  buffer->converter.convertArray16(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayUInt32(NABuffer* buffer, uint32* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt32", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayUInt32", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 4);
  naRetrieveBufferBytes(buffer, buf, count * 4, 0);
  buffer->converter.convertArray32(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayUInt64(NABuffer* buffer, uint64* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayUInt64", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayUInt64", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 8);
  naRetrieveBufferBytes(buffer, buf, count * 8, 0);
  buffer->converter.convertArray64(buf, (NAUInt)count);
}



NA_DEF void naReadBufferArrayFloat(NABuffer* buffer, float* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayFloat", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayFloat", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 4);
  naRetrieveBufferBytes(buffer, buf, count * 4, 0);
  buffer->converter.convertArray32(buf, (NAUInt)count);
}
NA_DEF void naReadBufferArrayDouble(NABuffer* buffer, double* buf, NABufInt count){
  #ifndef NDEBUG
    if(!buf)
      naError("naReadBufferArrayDouble", "buf is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferArrayDouble", "Bit offset not 0.");
  #endif
  naEnhanceBuffer(buffer, count * 8);
  naRetrieveBufferBytes(buffer, buf, count * 8, 0);
  buffer->converter.convertArray64(buf, (NAUInt)count);
}





// /////////////////////////////////////////////////////////////////
// WRITING
// /////////////////////////////////////////////////////////////////





NA_DEF void naStoreBufferBytes(NABuffer* buffer, const void* ptr, NABufInt bytesize){
  NAList* buflist;
  
  #ifndef NDEBUG
    if(!bytesize)
      naError("naStoreBufferBytes", "Writing zero bytes.");
    if(bytesize < 0)
      naError("naStoreBufferBytes", "Negative count.");
  #endif
  if(!bytesize){return;}

  buflist = naGetPointerMutable(buffer->storage);
  
  while(bytesize){
    NABufferPart* curpart;
    NABufInt remainingbytes;
    NAByte* dst;
    
    #ifndef NDEBUG
      if(!buffer->curlistpos)
        naError("naStoreBufferBytes", "Internal error: No buffer part available.");
    #endif
    curpart = naGetListPositionMutable(buflist, buffer->curlistpos);
    #ifndef NDEBUG
      if(!curpart || !naIsOffsetInBufferPart(curpart, buffer->curoffset))
        naError("naStoreBufferBytes", "Internal error: Current buffer part inavailable or not containing current position.");
    #endif
    
    if(!curpart){return;}
    remainingbytes = naGetBufferPartEndBufferIndex(curpart) - buffer->curoffset;  // todo: this should be the remaining bytesize of used bytes
    dst = naGetBufferPartDataPointer(curpart, buffer->curoffset);
    if(bytesize > remainingbytes){
      naCopyn(dst, ptr, (NAUInt)remainingbytes);
      buffer->curoffset += remainingbytes;
      buffer->curlistpos = naGetListNextPosition(buflist, buffer->curlistpos);
      *((NAByte**)(&ptr)) += remainingbytes;
      bytesize -= remainingbytes;
    }else{
      naCopyn(dst, ptr, (NAUInt)bytesize);
      buffer->curoffset += bytesize;
      *((NAByte**)(&ptr)) += bytesize;
      bytesize -= bytesize;
    }
  }
}



NA_DEF void naWriteBufferBytes(NABuffer* buffer, const void* ptr, NABufInt bytesize){
  #ifndef NDEBUG
    if(!bytesize)
      naError("naReadBufferBytes", "Writing zero bytes.");
    if(bytesize < 0)
      naError("naReadBufferBytes", "Negative count.");
  #endif
  if(!bytesize){return;}
  naEnhanceBuffer(buffer, bytesize);
  naStoreBufferBytes(buffer, ptr, bytesize);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferByteArray(NABuffer* buffer, const NAByteArray* bytearray){
  naWriteBufferBytes(buffer, naGetByteArrayConstPointer(bytearray), naGetByteArrayBytesize(bytearray));
}



NA_DEF void naWriteBufferString(NABuffer* buffer, const NAString* string){
  naEnhanceBuffer(buffer, naGetStringLength(string));
  naStoreBufferBytes(buffer, naGetStringUTF8Pointer(string), naGetStringLength(string));
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferStringWithFormat(NABuffer* buffer,
                       const NAUTF8Char* format,
                                         ...){
  // Declaration before implementation. Needed for C90.
  va_list argumentlist;
  va_start(argumentlist, format);
  naWriteBufferStringWithArguments(buffer, format, argumentlist);
  va_end(argumentlist);
}



NA_DEF void naWriteBufferStringWithArguments(NABuffer* buffer,
                          const NAUTF8Char* format,
                                    va_list argumentlist){
  // Declaration before implementation. Needed for C90.
  NAString* string;
  string = naNewStringWithArguments(format, argumentlist);
  naWriteBufferString(buffer, string);
  naDelete(string);
}



NA_DEF void naWriteBufferBuffer(NABuffer* dstbuffer, NABuffer* srcbuffer, NABufInt bytesize){
//  NAList* srcbuflist;
//  
//  #ifndef NDEBUG
//    if(count < 0)
//      naError("naWriteBufferBuffer", "count should be >= 0.");
//  #endif
//  if(count == 0){return;}
////  srcbuflist = naGetPointerMutable(srcbuffer->storage);
////  NABufInt countcopy = count;
////  #ifndef NDEBUG
////    if(naIsOffsetInBufferPart(naGetListCurrentMutable(srcbuflist), srcbuffer->readpos))
////      naError("naWriteBufferBuffer", "Internal error: position is not in the current buffer part.");
////  #endif
////  naFirstList(srcbuflist);
//  naLocateBufferPartList(srcbuflist, srcbuffer->readpos);
//  while(NA_TRUE){
//    NABufferPart* curpart = naGetListCurrentMutable(srcbuflist);
//    NABufInt remainingsize = naGetBufferPartEndBufferIndex(curpart) - srcbuffer->readpos;
//    void* srcptr = naGetBufferPartDataPointer(curpart, srcbuffer->readpos);
//    if(remainingsize > count){
//      naWriteBufferBytes(dstbuffer, srcptr, count);
//      srcbuffer->readpos += count;
//      count = 0;
//    }else{
//      naWriteBufferBytes(dstbuffer, srcptr, remainingsize);
//      srcbuffer->readpos += remainingsize;
//      count -= remainingsize;
//    }
//    if(count == 0){break;}
//    naIterateList(srcbuflist, 1);
//  }
}



NA_DEF void naRepeatBufferBytes(NABuffer* buffer, NABufInt distance, NABufInt bytesize){
  #ifndef NDEBUG
    if(bytesize <= 0)
      naError("naRepeatBufferBytes", "count should be > 0.");
    if(distance <= 0)
      naError("naRepeatBufferBytes", "distance should be > 0.");
  #endif
  NAList* partlist = naGetPointerMutable(buffer->storage);
  NABufInt finalpos = buffer->curoffset + bytesize;

  // Enhance the buffer so that the write part can be stored:
  naEnhanceBuffer(buffer, bytesize);

  NABufInt writeoffset = buffer->curoffset;
  NAListPos writepos = buffer->curlistpos;
  NABufferPart* writepart = naGetListPositionMutable(partlist, writepos);
  NABufInt remainingwrite = naGetBufferPartEndDataIndex(writepart) - writeoffset;
  remainingwrite = naMini(remainingwrite, bytesize);
  NAByte* writebyte = naGetBufferPartDataPointer(writepart, writeoffset);
  
  // Enhance the buffer so that the read part is available:
  naSeekBufferRelative(buffer, -distance);
  if(buffer->curoffset == 158){
    printf("asdf");
  }
  naEnhanceBuffer(buffer, bytesize);

  NABufInt readoffset = buffer->curoffset;
  NAListPos readpos = buffer->curlistpos;
  NABufferPart* readpart = naGetListPositionMutable(partlist, readpos);
  NABufInt remainingread = naGetBufferPartEndDataIndex(readpart) - readoffset;
  remainingread = naMini(remainingread, distance);
  NAByte* readbyte = naGetBufferPartDataPointer(readpart, readoffset);
  
  // Now start copying the buffers.
  while(1){
    if(remainingwrite < remainingread){
      remainingwrite = naMini(remainingread, bytesize);
      naCopyn(writebyte, readbyte, remainingwrite);
      bytesize -= remainingwrite;
      if(!bytesize){break;}
      writeoffset += remainingwrite;
      writepos = naGetListNextPosition(partlist, writepos);
      writepart = naGetListPositionMutable(partlist, writepos);
      remainingwrite = naGetBufferPartEndDataIndex(writepart) - writeoffset;
      writebyte = naGetBufferPartDataPointer(writepart, writeoffset);
      readoffset += remainingwrite;
      remainingread -= remainingwrite;
    }else{
      remainingread = naMini(remainingread, bytesize);
      naCopyn(writebyte, readbyte, remainingread);
      bytesize -= remainingread;
      if(!bytesize){break;}
      writeoffset += remainingread;
      remainingwrite -= remainingread;
      readoffset += remainingread;
      if(readoffset == naGetBufferPartEndDataIndex(readpart)){
        readpos = naGetListNextPosition(partlist, readpos);
        readpart = naGetListPositionMutable(partlist, readpos);
        readbyte = naGetBufferPartDataPointer(readpart, readoffset);
      }else{
        readbyte += remainingread;
      }
      remainingread = naGetBufferPartEndDataIndex(readpart) - readoffset;
      remainingread = naMini(remainingread, distance);
    }
  }

  naSeekBufferAbsolute(buffer, finalpos);
}



NA_DEF void naWriteBufferInt8(NABuffer* buffer, int8 value){
  buffer->converter.convert8(&value);
  naEnhanceBuffer(buffer, 1);
  naStoreBufferBytes(buffer, &value, 1);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferInt16(NABuffer* buffer, int16 value){
  buffer->converter.convert16(&value);
  naEnhanceBuffer(buffer, 2);
  naStoreBufferBytes(buffer, &value, 2);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferInt32(NABuffer* buffer, int32 value){
  buffer->converter.convert32(&value);
  naEnhanceBuffer(buffer, 4);
  naStoreBufferBytes(buffer, &value, 4);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferInt64(NABuffer* buffer, int64 value){
  buffer->converter.convert64(&value);
  naEnhanceBuffer(buffer, 8);
  naStoreBufferBytes(buffer, &value, 8);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}


NA_DEF void naWriteBufferUInt8(NABuffer* buffer, uint8 value){
  buffer->converter.convert8(&value);
  naEnhanceBuffer(buffer, 1);
  naStoreBufferBytes(buffer, &value, 1);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferUInt16(NABuffer* buffer, uint16 value){
  buffer->converter.convert16(&value);
  naEnhanceBuffer(buffer, 2);
  naStoreBufferBytes(buffer, &value, 2);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferUInt32(NABuffer* buffer, uint32 value){
  buffer->converter.convert32(&value);
  naEnhanceBuffer(buffer, 4);
  naStoreBufferBytes(buffer, &value, 4);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferUInt64(NABuffer* buffer, uint64 value){
  buffer->converter.convert64(&value);
  naEnhanceBuffer(buffer, 8);
  naStoreBufferBytes(buffer, &value, 8);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}


NA_DEF void naWriteBufferFloat(NABuffer* buffer, float value){
  buffer->converter.convert32(&value);
  naEnhanceBuffer(buffer, 4);
  naStoreBufferBytes(buffer, &value, 4);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferDouble(NABuffer* buffer, double value){
  buffer->converter.convert64(&value);
  naEnhanceBuffer(buffer, 8);
  naStoreBufferBytes(buffer, &value, 8);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}









NA_DEF void naWriteBufferArrayInt8(NABuffer* buffer, const int8* buf, NABufInt count){
  int8 value;
  naEnhanceBuffer(buffer, count * 1);
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
  naEnhanceBuffer(buffer, count * 2);
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
  naEnhanceBuffer(buffer, count * 4);
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
  naEnhanceBuffer(buffer, count * 8);
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
  naEnhanceBuffer(buffer, count * 1);
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
  naEnhanceBuffer(buffer, count * 2);
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
  naEnhanceBuffer(buffer, count * 4);
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
  naEnhanceBuffer(buffer, count * 8);
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
  naEnhanceBuffer(buffer, count * 4);
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
  naEnhanceBuffer(buffer, count * 8);
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
  naEnhanceBuffer(buffer, 1);
  naStoreBufferBytes(buffer, NA_TAB, 1);
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferNewLine(NABuffer* buffer){
  switch(buffer->flags & NA_BUFFER_FLAG_NEWLINE_MASK){
  case NA_BUFFER_FLAG_NEWLINE_MAC9:
    naEnhanceBuffer(buffer, 1);
    naStoreBufferBytes(buffer, NA_NL_MAC9, 1);
    break;
  case NA_BUFFER_FLAG_NEWLINE_UNIX:
    naEnhanceBuffer(buffer, 1);
    naStoreBufferBytes(buffer, NA_NL_UNIX, 1);
    break;
  case NA_BUFFER_FLAG_NEWLINE_WIN:
    naEnhanceBuffer(buffer, 2);
    naStoreBufferBytes(buffer, NA_NL_WIN, 2);
    break;
  }
  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
}





NA_DEF void naWriteBufferLine(NABuffer* buffer, const NAString* string){
  naWriteBufferString(buffer, string);
  naWriteBufferNewLine(buffer);
}
NA_DEF void naWriteBufferLineWithFormat(NABuffer* buffer, const NAUTF8Char* format, ...){
  // Declaration before implementation. Needed for C90.
  va_list argumentlist;
  va_start(argumentlist, format);
  naWriteBufferLineWithArguments(buffer, format, argumentlist);
  va_end(argumentlist);
}
NA_DEF void naWriteBufferLineWithArguments(NABuffer* buffer, const NAUTF8Char* format, va_list argumentlist){
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
