//
//// This file is part of NALib, a collection of C and C++ source code
//// intended for didactical purposes. Full license notice at the bottom.
//
//
//
int isoctest69834759;
//#include "../NAList.h"
//#include "../NAOldBuffer.h"
//
//
//#define NA_BUFFER_BYTESIZE 4096
//#if (NA_BUFFER_BYTESIZE < 16)
//  #error "Invalid file buffer bytesize"
//  // buffer must not be smaller than 16 as reader may require 16 Bytes.
//#endif
//
//
//// In NALib, Buffers are either Input-Buffers or Output-Buffers.
////
//// They can never be both and they are implemented quite differently although
//// the API looks very similar. Here is the main explanation of how the two
//// kinds of buffers work:
////
//// Input buffers:
////
//// Input buffers are implemented using a list of NAByteArrays. The current
//// array stores its current position in its iterator.
////
//// Output buffers:
////
//// Output buffers are basically a growing list of memory blocks partially or
//// completely filled with bytes. This is implemented using a list of todo
//// structs. These structs get flushed to the destination when filled enough
//// or upon request by the programmer with a call to naFlushBuffer.
////
//// Depending on what the output is destined to fill, the buffer might just
//// accumulate byte after byte until finally a destination is given and the
//// buffer can be flushed. If the output destination is a file, the buffer
//// will write to the file based on the given autoflush parameter which can be
//// none, auto, text, multibyte or all.
////
//
//// If maxpos is known, extend MUST be unset!
//#define NA_BUFFER_FLAG_OWNS_SOURCE         0x01
//#define NA_BUFFER_FLAG_MAXPOS_KNOWN        0x10
//#define NA_BUFFER_FLAG_CAN_EXTEND          0x04
//
//
//
//
//
//// Defintion of basic buffer flags
//#define NA_BUFFER_FLAG_CAN_SEEK             0x04
//
//
//#define NA_BUFFER_FLAG_SECURE               0x20
//
//
//
//
//////// Definition of the different kind of src and dst
////#define NA_BUFFER_STORAGE_FLAG_SOURCE_MASK         0x03
////#define NA_BUFFER_STORAGE_FLAG_SOURCE_NONE         0x00
////#define NA_BUFFER_STORAGE_FLAG_SOURCE_FILE         0x01
////#define NA_BUFFER_STORAGE_FLAG_SOURCE_MEMORY       0x02
//
//typedef enum{
//  NA_BUFFER_STORAGE_SOURCE_NONE = 0,
//  NA_BUFFER_STORAGE_SOURCE_FILE,
//  NA_BUFFER_STORAGE_SOURCE_MEMORY,
//} NABufferStorageSourceType;
//
//// Redefinition of the newline options as flags. See naSetBufferNewLineEncoding for
//// more information why this is done like this.
//#define NA_BUFFER_FLAG_NEWLINE_BITSHIFT     12
//#define NA_BUFFER_FLAG_NEWLINE_BITSIZE      2
//#define NA_BUFFER_FLAG_NEWLINE_MASK         (((1 << NA_BUFFER_FLAG_NEWLINE_BITSIZE) - 1) << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
//#define NA_BUFFER_FLAG_NEWLINE_UNIX         (NA_NEWLINE_UNIX << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
//#define NA_BUFFER_FLAG_NEWLINE_MAC9         (NA_NEWLINE_MAC9 << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
//#define NA_BUFFER_FLAG_NEWLINE_WIN          (NA_NEWLINE_WIN  << NA_BUFFER_FLAG_NEWLINE_BITSHIFT)
//#if NA_SYSTEM == NA_SYSTEM_WINDOWS
//  #define NA_BUFFER_FLAG_NEWLINE_NATIVE NA_BUFFER_FLAG_NEWLINE_WIN
//#else
//  #define NA_BUFFER_FLAG_NEWLINE_NATIVE NA_BUFFER_FLAG_NEWLINE_UNIX
//#endif
//
//// Redefinition of the autoflush options as flags. See naSetBufferAutoflush for
//// more information why this is done like this.
//#define NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT  14
//#define NA_BUFFER_FLAG_AUTOFLUSH_BITSIZE   3
//#define NA_BUFFER_FLAG_AUTOFLUSH_MASK      (((1 << NA_BUFFER_FLAG_AUTOFLUSH_BITSIZE) - 1) << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
//#define NA_BUFFER_FLAG_AUTOFLUSH_NONE      (NA_BUFFER_AUTOFLUSH_NONE      << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
//#define NA_BUFFER_FLAG_AUTOFLUSH_AUTO      (NA_BUFFER_AUTOFLUSH_AUTO      << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
//#define NA_BUFFER_FLAG_AUTOFLUSH_TEXT      (NA_BUFFER_AUTOFLUSH_TEXT      << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
//#define NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE (NA_BUFFER_AUTOFLUSH_MULTIBYTE << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
//#define NA_BUFFER_FLAG_AUTOFLUSH_ALL       (NA_BUFFER_AUTOFLUSH_ALL       << NA_BUFFER_FLAG_AUTOFLUSH_BITSHIFT)
//
//
//
//
//
//
//
//
//// //////////////////////////////////
//// BUFFER PART
//// //////////////////////////////////
//
//
//typedef struct NABufferPart NABufferPart;
//struct NABufferPart{
//  NASmartPtr asdfdata;
//  NARangei bufferrange;
//  NARangei datarange;
//  NAUInt refcount;
//  NAByte* data;
//};
//
//
//// Dallocates a buffer part
////NA_HIDEF void naDestructBufferPartData(NABufferPart* part){
////  #ifndef NDEBUG
////    if(part->refcount != NA_ZERO)
////      naError("naDeallocBufferPart", "Part is still in use");
////  #endif
////  naFree(part->data);
////  naFree(part);
////}
//
//
//// Allocates a buffer part which is sparse
//NA_HIDEF NABufferPart* naAllocBufferPartSparse(NARangei bufferrange){
//  return NA_NULL;
//  //NABufferPart* part;
//  //#ifndef NDEBUG
//  //  if(!naIsRangeiUseful(bufferrange))
//  //    naError("naAllocBufferPartSparse", "buffer range not useful");
//  //#endif
//  //part = naAlloc(NABufferPart);
//  //part->bufferrange = bufferrange;
//  //part->datarange = naMakeRangeiWithStartAndEnd(0, 0);
//  //naInitSmartPtrMutable(&(part->asdfdata), NA_MEMORY_CLEANUP_NONE, NA_NULL, NA_NULL);
//  //return part;
//}
//
//
//
//
//
//
//// Retains a buffer part
//NA_HIDEF void naRetainBufferPart(NABufferPart* part){
////  part->refcount++;
//  naRetainSmartPtr(&(part->asdfdata));
//}
//
//
//
//// Releases a buffer part.
//NA_HIDEF void naReleaseBufferPart(NABufferPart* part, NAListIterator* iter){
//  if(naReleaseSmartPtr(&(part->asdfdata), NA_NULL)){
////  part->refcount--;
////  if(part->refcount == NA_ZERO){
//    #ifndef NDEBUG
//      if(naGetListCurrentMutable(iter) != part)
//        naError("naReleaseBufferPart", "Iterator not placed at part to delete");
//    #endif
//    naRemoveListCurrentConst(iter, NA_FALSE);
////    naDestructBufferPartData(part);
////*    naFree(part);
//  }
//}
//
//
//
//// Returns NA_TRUE if this buffer is a sparse buffer
//NA_HIDEF NABool naIsBufferPartSparse(NABufferPart* part){
//  return (naGetSmartPtrConst(&(part->asdfdata)) == NA_NULL);
//}
//
//
//
//// Returns the buffer start index of the buffer part.
//NA_HIDEF NAInt naGetBufferPartStartBufferIndex(NABufferPart* part){
//  return part->bufferrange.origin;
//}
//
//
//
//// Returns the buffer end index of the buffer part.
//NA_HIDEF NAInt naGetBufferPartEndBufferIndex(NABufferPart* part){
//  return 0;
////*  return naGetRangeiEnd(part->bufferrange);
//}
//
//
//
//// Returns the buffer max index of the buffer part.
//NA_HIDEF NAInt naGetBufferPartMaxBufferIndex(NABufferPart* part){
//  return naGetRangeiMax(part->bufferrange);
//}
//
//
//
////// Returns the data start index of the buffer part.
////NA_HIDEF NAInt naGetBufferPartStartDataIndex(NABufferPart* part){
////  return part->datarange.origin;
////}
////
////
////// Returns the data end index of the buffer part.
////NA_HIDEF NAInt naGetBufferPartEndDataIndex(NABufferPart* part){
////  return naGetRangeiEnd(part->datarange);
////}
//
//
//
//#ifndef NDEBUG
//  // Returns true, if the given absolute position is inside the buffer part.
//  NA_HIDEF NABool naIsOffsetInBufferPart(NABufferPart* part, NAInt offset){
//    return naContainsRangeiOffset(part->bufferrange, offset);
//  }
//#endif
//
//
//
//// Advances the number of bytes being used in the buffer storage.
//NA_HIDEF void naAdjustBufferPartToContainDataRange(NABufferPart* part, NARangei range){
//  #ifndef NDEBUG
//    if(naIsBufferPartSparse(part))
//      naError("naAdjustBufferPartToContainDataRange", "buffer part is sparse");
//    if(!naIsRangeiUseful(range))
//      naError("naAdjustBufferPartToContainDataRange", "data range not useful");
//    if(!naContainsRangeiRange(part->bufferrange, range))
//      naError("naAdjustBufferPartToContainDataRange", "data range not contained in this buffer range");
//  #endif
//
//  if(naIsRangeiEmpty(part->datarange)){
//    part->datarange = range;
//  }else{
//  
//    // Extend the range in the leading part
//    if(range.origin < part->datarange.origin){
//      if(naGetRangeiEnd(range) < part->datarange.origin){
//        // There is a gap.
//        #ifndef NDEBUG
//          naError("naAdjustBufferPartToContainDataRange", "Warning: Gap between datarange in buffer part appending before");
//        #endif
//      }
//      part->datarange = naMakeRangeiWithStartAndEnd(range.origin, naGetRangeiEnd(part->datarange));
//    }
//    
//    // Extend the range in the trailing part
//    if(naGetRangeiEnd(range) > naGetRangeiEnd(part->datarange)){
//      if(range.origin > naGetRangeiEnd(part->datarange)){
//        // There is a gap.
//        #ifndef NDEBUG
//          naError("naAdjustBufferPartToContainDataRange", "Warning: Gap between datarange in buffer part appending after");
//        #endif
//      }
//      part->datarange = naMakeRangeiWithStartAndEnd(part->datarange.origin, naGetRangeiEnd(range));
//    }
//  }
//}
//
//
//
//// Returns a direct pointer to the raw data of this buffer part, given its
//// absolute address.
//NA_HIDEF NAByte* naGetBufferPartDataPointerAbsolute(NABufferPart* part, NAInt offset){
//  return NA_NULL;
////  #ifndef NDEBUG
////    if(naIsBufferPartSparse(part))
////      naError("naGetBufferPartDataPointerAbsolute", "buffer part is sparse");
////    if(!naContainsRangeiOffset(part->bufferrange, offset))
////      naError("naGetBufferPartDataPointerAbsolute", "offset not inside buffer part");
////    if(!naContainsRangeiOffset(part->datarange, offset))
////      naError("naGetBufferPartDataPointerAbsolute", "offset not inside data part");
////  #endif
////  return &(naGetSmartPtrMutable(&(part->asdfdata))[offset - part->bufferrange.origin]);
//}
//
//
//
//// Returns a direct pointer to the raw data of this buffer part, given its
//// absolute address.
//NA_HIDEF NAByte* naGetBufferPartDataPointer(NABufferPart* part){
//  #ifndef NDEBUG
//    if(naIsBufferPartSparse(part))
//      naError("naGetBufferPartDataPointerAbsolute", "buffer part is sparse");
//  #endif
//  return naGetSmartPtrMutable(&(part->asdfdata));
//}
//
//
//
//
//
//
//// //////////////////////////////////////
//// Buffer Storage
//// //////////////////////////////////////
//
//
//
//typedef struct NABufferStorage NABufferStorage;
//struct NABufferStorage{
//  NAList partlist;
//  void* source;
//  NABufferStorageSourceType sourcetype;
//};
//
//
//
//// Returns the storage of the given buffer
//NA_HIDEF NABufferStorage* naGetBufferStorage(NABuffer* buffer){
//  return (NABufferStorage*)naGetPointerMutable(buffer->storage);
//}
//
//
//
//// Returns the buffer part list of the given buffer
//NA_HIDEF NAList* naGetBufferPartList(NABuffer* buffer){
//  return &(naGetBufferStorage(buffer)->partlist);
//}
//
//
//
//// Removes any previous source.
//NA_HIDEF void naVoidBufferStorageSource(NABufferStorage* storage){
//  switch(storage->sourcetype){
//  case NA_BUFFER_STORAGE_SOURCE_NONE:
//    break;
//  case NA_BUFFER_STORAGE_SOURCE_FILE:
//    naReleaseFile(storage->source);
//    naFree(storage->source);
//    break;
//  case NA_BUFFER_STORAGE_SOURCE_MEMORY:
//    break;
//  default:
//    #ifndef NDEBUG
//      naError("naVoidBufferStorageSource", "Invalid Source type");
//    #endif
//    break;
//  }
//  storage->source = NA_NULL;
//  storage->sourcetype = NA_BUFFER_STORAGE_SOURCE_NONE;
//}
//
//
//
//// Allocates a new buffer storage
//NA_HIDEF NABufferStorage* naAllocBufferStorage(){
//  NABufferStorage* storage = naAlloc(NABufferStorage);
//  naInitList(&(storage->partlist));
//  storage->source = NA_NULL;
//  storage->sourcetype = NA_BUFFER_STORAGE_SOURCE_NONE;
//  return storage;
//}
//
//
//
//// Clears all buffer parts in the list and clears the list itself.
//NA_HIDEF void naDeallocBufferStorage(NABufferStorage* storage){
//  #ifndef NDEBUG
//    if(!naIsListEmpty(&(storage->partlist)))
//      naError("naClearBufferStorage", "part list is not empty");
//    if(storage->sourcetype != NA_BUFFER_STORAGE_SOURCE_NONE)
//      naError("naClearBufferStorage", "buffer storage source is still present");
//  #endif
//  naClearList(&(storage->partlist));
//}
//
//
//
//// Sets the storage source to NULL, meaning, there is no source.
////NA_HIDEF void naSetBufferStorageSourceNone(NABufferStorage* storage){
////  naVoidBufferStorageSource(storage);
////  storage->source = NA_NULL;
////  storage->sourcetype = NA_BUFFER_STORAGE_SOURCE_NONE;
////}
//
//
//
//// Sets the storage source to an input file.
//NA_HIDEF void naSetBufferStorageSourceFile(NABufferStorage* storage, const char* filename){
//  naVoidBufferStorageSource(storage);
//  storage->source = naAlloc(NAFile);
//  *((NAFile*)(storage->source)) = naCreateFileReadingFilename(filename);
//  storage->sourcetype = NA_BUFFER_STORAGE_SOURCE_FILE;
//}
//
//
//
//// Sets the storage source to memory which can be allocated.
//NA_HIDEF void naSetBufferStorageSourceMemory(NABufferStorage* storage){
//  naVoidBufferStorageSource(storage);
//  storage->source = NA_NULL;
//  storage->sourcetype = NA_BUFFER_STORAGE_SOURCE_MEMORY;
//}
//
//
//
//// Enlarges the storage such that range is contained in the part list.
//NA_HIDEF void naEnlargeBufferStorage(NABuffer* buffer, NARangei range){
//  NABufferStorage* storage = naGetBufferStorage(buffer);
//  NABufferPart* part;
//  if(naIsListEmpty(&(storage->partlist))){
//    part = naAllocBufferPartSparse(range);
//    naAddListFirstMutable(&(storage->partlist), part);
//  }else{
//    // Enlarge at the beginning if necessary
//    part = naGetListFirstMutable(&(storage->partlist));
//    if(range.origin < part->bufferrange.origin){
//      part = naAllocBufferPartSparse(naMakeRangeiWithStartAndEnd(range.origin, part->bufferrange.origin));
//      naAddListFirstMutable(&(storage->partlist), part);
//    }
//    // Enlarge at the end if necessary
//    part = naGetListLastMutable(&(storage->partlist));
//    if(naGetRangeiEnd(range) > naGetRangeiEnd(part->bufferrange)){
//      part = naAllocBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(part->bufferrange), naGetRangeiEnd(range)));
//      naAddListLastMutable(&(storage->partlist), part);
//    }
//  }
//  if(buffer->curlistpos == naGetListInitialPosition(&(storage->partlist))){
//    buffer->curlistpos = naGetListFirstPosition(&(storage->partlist));
//  }
//}
//
//
//
//// This function adjusts iter so that the part it points to contains offset.
//NA_HIDEF void naLocateBufferPartList(NAListIterator* iter, NAInt offset){
//  NABufferPart* part = naGetListCurrentMutable(iter);
//
//  // If the list is empty, we simply return.
//  if(!part){
//    naLocateListFirst(iter);
//    part = naGetListCurrentMutable(iter);
//  }
//
//  while(part){
//    if(naGetBufferPartStartBufferIndex(part) > offset){
//      naIterateList(iter, -1);
//      part = naGetListCurrentMutable(iter);
//    }
//    if(naGetBufferPartMaxBufferIndex(part) < offset){
//      naIterateList(iter, 1);
//      part = naGetListCurrentMutable(iter);
//    }
//    if(naContainsRangeiOffset(part->bufferrange, offset)){return;}
//  }
//  #ifndef NDEBUG
//    naError("naLocateBufferPartList", "Buffer Part containing given offset not found.");
//  #endif
//}
//
//
//
//
//// Goes from first to last part and retains them.
//NA_HDEF void naRetainBufferParts(NABufferStorage* storage, NAListPos firstpos, NAListPos lastpos){
//  NAListIterator iter;
//  if((firstpos == naGetListInitialPosition(&(storage->partlist))) || (lastpos == naGetListInitialPosition(&(storage->partlist)))){return;}
//  iter = naMakeListIteratorMutator(&(storage->partlist));
//  naLocateListPosition(&iter, firstpos);
//  while(NA_TRUE){
//    naRetainBufferPart(naGetListCurrentMutable(&iter));
//    if(naGetListCurrentPosition(&iter) == lastpos){break;}
//    naIterateList(&iter, 1);
//  }
//  naClearListIterator(&iter);
//}
//
//
//
//// Looks at the buffer part currently at iter which must be sparse and
//// transforms it into a read buffer part, splitting the part if necessary.
//// iter will point to the filled part after this function.
//NA_HDEF void naFillBufferSparsePart(NABufferStorage* storage, NAListIterator* iter, NARangei newbufferrange){
//  NABufferPart* part = naGetListCurrentMutable(iter);
//  #ifndef NDEBUG
//    if(!naIsBufferPartSparse(part))
//      naError("naFillBufferSparsePart", "Internal error: Buffer part must be sparse.");
//  #endif
//  if(naContainsRangeiRange(newbufferrange, part->bufferrange)){
//    // This sparse buffer will be transformed completely.
//    part->bufferrange = newbufferrange;
//  }else{
//    NABufferPart* newpart = naAllocBufferPartSparse(newbufferrange);
//    newpart->refcount = part->refcount;
//    if(newbufferrange.origin == naGetBufferPartStartBufferIndex(part)){
//      // The new buffer must be placed before the sparse buffer.
//      // We adjust the sparse part and iterate backward.
//      naAddListBeforeMutable(iter, newpart);
//      part->bufferrange = naClampRangeiToMin(part->bufferrange, naGetRangeiEnd(newbufferrange));
//      naIterateList(iter, -1);
//    
//    }else if(naGetRangeiEnd(newbufferrange) == naGetBufferPartEndBufferIndex(part)){
//      // The new buffer must be placed after the sparse buffer
//      naAddListAfterMutable(iter, newpart);
//      part->bufferrange = naClampRangeiToEnd(part->bufferrange, newbufferrange.origin);
//      naIterateList(iter, 1);
//    
//    }else{
//      // In this situation, the new buffer must be placed in the middle
//      // of the existing sparse buffer part.
//
//      NABufferPart* sparsepart = naAllocBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetBufferPartStartBufferIndex(part), newbufferrange.origin));
//      sparsepart->refcount = part->refcount;
//      naAddListBeforeMutable(iter, sparsepart);
//      // Then we add the new part.
//      naAddListBeforeMutable(iter, newpart);
//      part->bufferrange = naClampRangeiToMin(part->bufferrange, naGetRangeiEnd(newbufferrange));
//      naIterateList(iter, -1);
//    }
//    part = naGetListCurrentMutable(iter);
//  }
//
//  // Fill the newly aquired part.
//  part->data = naMalloc(part->bufferrange.length);
//  switch(storage->sourcetype){
//  case NA_BUFFER_STORAGE_SOURCE_NONE:
//    // Nothing to do here.
//    break;
//  case NA_BUFFER_STORAGE_SOURCE_FILE:
//    {
//      NAFile* file;
//      #ifndef NDEBUG
//        if(part->bufferrange.origin < 0)
//          naError("naFillBufferSourcePart", "Trying to read from a file with negative offset");
//      #endif
//      file = (NAFile*)(storage->source);
//      if(naTell(file->desc) != part->bufferrange.origin){
//        naSeekFileAbsolute(file, part->bufferrange.origin);
//      }
//      naReadFileBytes(file, naGetBufferPartDataPointer(part), part->bufferrange.length);
//    }
//    break;
//  case NA_BUFFER_STORAGE_SOURCE_MEMORY:
//    // Nothing to do here.
//    break;
//  default:
//    #ifndef NDEBUG
//      naError("naFillBufferSourcePart", "Invalid source flag.");
//    #endif
//    break;
//  }
//}
//
//
//
//
//
//
//
//
//
//
//// //////////////////////////////////////////////////
//// NABuffer
//// //////////////////////////////////////////////////
//
//
//NA_DEF NABuffer* naInitBuffer(NABuffer* buffer){
//  
//  NABufferStorage* storage = naAllocBufferStorage();
//  naSetBufferStorageSourceMemory(storage);
//  buffer->storage = naNewPointerMutable(storage, NA_MEMORY_CLEANUP_NONE, (NAMutator)naDeallocBufferStorage);
//  buffer->flags = NA_BUFFER_FLAG_OWNS_SOURCE | NA_BUFFER_FLAG_CAN_EXTEND;
//
//  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);
//  buffer->curoffset = 0;
//  buffer->curbit = 0;
//
//  buffer->curlistpos = naGetListInitialPosition(naGetBufferPartList(buffer));
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferWithConstBytes(NABuffer* buffer, const NAByte* bytes, NAInt bytesize){
//  
//  NABufferStorage* storage = naAllocBufferStorage();
//  naSetBufferStorageSourceMemory(storage);
//  buffer->storage = naNewPointerMutable(storage, NA_MEMORY_CLEANUP_NONE, (NAMutator)naDeallocBufferStorage);
//  buffer->flags = NA_BUFFER_FLAG_OWNS_SOURCE;
//
//  if(bytesize < 0){
//    buffer->range = naMakeRangeiWithStartAndEnd(0, -bytesize);
//  }else{
//    buffer->range = naMakeRangeiWithStartAndEnd(0, bytesize);
//  }
//  buffer->curoffset = 0;
//  buffer->curbit = 0;
//
//  naEnlargeBufferStorage(buffer, buffer->range);
//
//  buffer->curlistpos = naGetListInitialPosition(naGetBufferPartList(buffer));
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferWithFile(NABuffer* buffer, const char* filename){
//  
//  NABufferStorage* storage = naAllocBufferStorage();
//  naSetBufferStorageSourceFile(storage, filename);
//  buffer->storage = naNewPointerMutable(storage, NA_MEMORY_CLEANUP_NONE, (NAMutator)naDeallocBufferStorage);
//  buffer->flags = NA_BUFFER_FLAG_OWNS_SOURCE | NA_BUFFER_FLAG_CAN_EXTEND;
//  
//  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);
//  buffer->curoffset = 0;
//  buffer->curbit = 0;
//
//  buffer->curlistpos = naGetListInitialPosition(naGetBufferPartList(buffer));
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  return buffer;
//}
//
//
//
//NA_DEF NABuffer* naInitBufferWithBufferExtraction(NABuffer* buffer, NABuffer* srcbuffer, NAInt bytesize){
//  
//  return NA_NULL;
//  
////  NABufferStorage* storage = naGetBufferStorage(srcbuffer);
////
////  buffer->storage = naRetainPointer(srcbuffer->storage);
////  buffer->flags = NA_BUFFER_FLAG_MAXPOS_KNOWN;
////  
////  buffer->range = naMakeRangeiE(srcbuffer->curoffset, bytesize);
////  buffer->curoffset = buffer->range.origin;
////  buffer->curbit = 0;
////  
////  if(bytesize){
////    NAListPos lastlistpos;
////    NAListIterator iter = naMakeListIteratorMutator(&(storage->partlist));
////    naLocateListPosition(&iter, srcbuffer->curlistpos);
////    naLocateBufferPartList(&iter, buffer->range.origin);
////    buffer->curlistpos = naGetListCurrentPosition(&iter);
////    naLocateBufferPartList(&iter, naGetRangeiMax(buffer->range));
////    lastlistpos = naGetListCurrentPosition(&iter);
////    naRetainBufferParts(storage, buffer->curlistpos, lastlistpos);
////    naClearListIterator(&iter);
////  }
////
////  buffer->endianness = srcbuffer->endianness;
////  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
////
////  return buffer;
//}
//
//
//
//
//
//NA_DEF void naFlushBuffer(NABuffer* buffer){
//  NA_UNUSED(buffer);
////  if(naGetBufferKind(buffer) == NA_BUFFER_KIND_OUTPUT){
////    if((buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK) == NA_BUFFER_FLAG_SRCDST_NONE){
////      return;
////    }
////    naFirstList(&(buffer->buflist));
////    NABuf* curbuf;
////    while((curbuf = naIterateListMutable(&(buffer->buflist), 1))){
////      #ifndef NDEBUG
////        if(!naGetBufUsedBytesize(curbuf))
////          naError("naFlushBuffer", "Internal error: NABuf is empty.");
////      #endif
////      switch(buffer->flags & NA_BUFFER_FLAG_SRCDST_MASK){
////      case NA_BUFFER_FLAG_SRCDST_NONE:
////        // this is handeled in the if statement above.
////        break;
////      case NA_BUFFER_FLAG_SRCDST_VOID:
////        // Nothing to do. Flushing to nowhere.
////        break;
////      case NA_BUFFER_FLAG_SRCDST_FILE:
////        naWriteFileBytes(buffer->srcdst, naGetBufConstFirstPointer(curbuf), naGetBufUsedBytesize(curbuf));
////        break;
////      default:
////        #ifndef NDEBUG
////          naError("naFlushBuffer", "Invalid srcdst field.");
////        #endif
////        break;
////      }
////      naRemoveListPrevMutable(&(buffer->buflist));
////      naFree(naGetBufMutableFirstPointer(curbuf));
////      naFree(curbuf);
////    }
////
////  }else{
////
////    NAByteArray* curarray;
////    
////    naFirstList(&(buffer->buflist));
////    while((curarray = naIterateListMutable(&(buffer->buflist), 1))){
////      naRemoveListPrevMutable(&(buffer->buflist));
////      naClearByteArray(curarray);
////      naFree(curarray);
////    }
////
////  }
//}
//
//
//
//
//NA_DEF void naClearBuffer(NABuffer* buffer){
//  NABufferPart* curpart;
//  NABufferStorage* storage = naGetBufferStorage(buffer);
//  
//  if(buffer->flags & NA_BUFFER_FLAG_OWNS_SOURCE){
//    naVoidBufferStorageSource(storage);
//  }
//  
//  #ifndef NDEBUG
//    if(!naIsRangeiEmpty(buffer->range) && naIsListEmpty(&(storage->partlist)))
//      naError("naClearBuffer", "List is empty although it should not be.");
//  #endif
//  
//  if(!naIsListEmpty(&(storage->partlist))){
//    // Release all parts of the list which are covered by this buffer.
//    NAListIterator iter = naMakeListIteratorModifier(&(storage->partlist));
//    naLocateBufferPartList(&iter, buffer->range.origin);
//    curpart = naGetListCurrentMutable(&iter);
//    while(curpart){
//      naReleaseBufferPart(curpart, &iter);
//      naIterateList(&iter, 1);
//      curpart = naGetListCurrentMutable(&iter);
//    }
//    naClearListIterator(&iter);
//  }
//  
//  naReleasePointer(buffer->storage);
//}
//
//
//
//
////NA_DEF NABool naCanBufferSeek(const NABuffer* buffer){
////  return ((buffer->flags & NA_BUFFER_FLAG_CAN_SEEK) == NA_BUFFER_FLAG_CAN_SEEK);
////}
//
//NA_DEF NABool naCanBufferExtend(const NABuffer* buffer){
//  return ((buffer->flags & NA_BUFFER_FLAG_CAN_EXTEND) == NA_BUFFER_FLAG_CAN_EXTEND);
//}
////
////
////
////
////
////
////NA_API NABool naIsBufferSecure(const NABuffer* buffer){
////  return ((buffer->flags & NA_BUFFER_FLAG_SECURE) == NA_BUFFER_FLAG_SECURE);
////}
////
////
////
//NA_DEF NAInt naGetBufferCurOffsetAbsolute(const NABuffer* buffer){
//  return buffer->curoffset;
//}
//
//
//NA_API NABool naHasBufferDeterminedBytesize(const NABuffer* buffer){
//  return ((buffer->flags & NA_BUFFER_FLAG_MAXPOS_KNOWN) == NA_BUFFER_FLAG_MAXPOS_KNOWN);
//}
//
//
////NA_API NAInt naDetermineBufferBytesize(NABuffer* buffer){
////  NAFilesize filesize;
////  if(!naHasBufferDeterminedBytesize(buffer)){
////    NABufferStorage* storage = naGetBufferStorage(buffer);
////    #ifndef NDEBUG
////      if(!(buffer->flags & NA_BUFFER_FLAG_OWNS_SOURCE))
////        naError("naDetermineBufferBytesize", "Undetermined size fixed by buffer which does not own the source.");
////    #endif
////    switch(storage->sourcetype){
////    case NA_BUFFER_STORAGE_SOURCE_NONE:
////      break;
////    case NA_BUFFER_STORAGE_SOURCE_FILE:
////      filesize = naComputeFileBytesize((NAFile*)(storage->source));
////      #ifndef NDEBUG
////        #if (NA_FILESIZE_BITS > NA_SYSTEM_ADDRESS_BITS)
////          if(filesize > NA_INT_MAX)
////            naError("naDetermineBufferBytesize", "Can not store full file with %d bits", NA_SYSTEM_ADDRESS_BITS);
////        #endif
////      #endif
////      buffer->range.length = (NAInt)filesize;
////      buffer->range.origin = 0;
////      naEnlargeBufferStorage(buffer, buffer->range);
////      break;
////    case NA_BUFFER_STORAGE_SOURCE_MEMORY:
////      break;
////    }
////    buffer->flags |= NA_BUFFER_FLAG_MAXPOS_KNOWN;
////    buffer->flags &= ~NA_BUFFER_FLAG_CAN_EXTEND;
////  }
////  return buffer->range.length;
////}
//
//
//
////NA_DEF NABool naIsBufferReadAtEnd(const NABuffer* buffer){
////  #ifndef NDEBUG
////    if(!naHasBufferDeterminedBytesize(buffer))
////      naError("naIsBufferReadAtEnd", "Buffer has no fixed max position. Use naDetermineBufferBytesize.");
////  #endif
////  return (buffer->curoffset > naGetRangeiMax(buffer->range));
////}
//
//
//
////NA_DEF void naWriteBufferToFile(NABuffer* buffer, NAFile* file){
////  NAList* buflist;
////  NAListIterator iter;
////  NAInt prevend;
////  NAInt curend;
////
////  buflist = naGetBufferPartList(buffer);;
////  iter = naMakeListIteratorMutator(buflist);
////  naLocateBufferPartList(&iter, buffer->range.origin);
////  prevend = buffer->range.origin;
////  
////  while(NA_TRUE){
////    NABufferPart* curpart = naGetListCurrentMutable(&iter);
////    if(!curpart || (naGetRangeiMax(buffer->range) < naGetBufferPartStartDataIndex(curpart))){break;}
////    #ifndef NDEBUG
////      if(naGetBufferPartStartDataIndex(curpart) < 0)
////        naError("naWriteBufferToFile", "Can not write a file with negative offset.");
////    #endif
////    if(naIsBufferPartSparse(curpart)){naIterateList(&iter, 1); continue;}
////    if(prevend < naGetBufferPartStartDataIndex(curpart)){
////      #ifndef NDEBUG
////        naError("naWriteBufferToFile", "Sparse Buffers can not be written currently.");
////      #endif
////      naSeekFileAbsolute(file, (NAFilesize)naGetBufferPartStartDataIndex(curpart));
////    }
////    curend = naMini(naGetBufferPartEndDataIndex(curpart), naGetRangeiEnd(buffer->range));
////    naWriteFileBytes(file, naGetBufferPartDataPointerAbsolute(curpart, prevend), (curend - prevend));
////    naIterateList(&iter, 1);
////    prevend = curend;
////  }
////  
////  naClearListIterator(&iter);
////}
//
//
//
////NA_DEF void naSetBufferEndianness(NABuffer* buffer, NAInt endianness){
////  buffer->endianness = endianness;
////  buffer->converter = naMakeEndiannessConverter(endianness, NA_ENDIANNESS_NATIVE);
////}
////
////
////NA_DEF NAInt naGetBufferEndianness(NABuffer* buffer){
////  return buffer->endianness;
////}
//
//
////// You may ask yourself the question why the author decided to reimplement the
////// different flags again and use a case to set them correctly. This is because
////// the autoflush flags are actually an enumeration and not macros. Mixing the
////// two can lead to hard to understand compile errors especially if the enum
////// will be enhanced in the future (who knows). By reimplementing the flags
////// again, we can be sure that the NABuffer implementation is independent of
////// the enumeration.
////NA_DEF void naSetBufferAutoFlushing(NABuffer* buffer, NABufferAutoFlushing autoflushing){
//////  NAUInt autoflushflag;
//////  #ifndef NDEBUG
//////    if(naGetBufferKind(buffer) != NA_BUFFER_KIND_OUTPUT)
//////      naError("naSetBufferAutoFlushing", "Setting the flushing behaviour is only useful for output buffers.");
//////  #endif
//////  buffer->flags &= ~NA_BUFFER_FLAG_AUTOFLUSH_MASK;
//////  switch(autoflushing){
//////  case NA_BUFFER_AUTOFLUSH_NONE:      autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_NONE;      break;
//////  case NA_BUFFER_AUTOFLUSH_AUTO:      autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_AUTO;      break;
//////  case NA_BUFFER_AUTOFLUSH_TEXT:      autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_TEXT;      break;
//////  case NA_BUFFER_AUTOFLUSH_MULTIBYTE: autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE; break;
//////  case NA_BUFFER_AUTOFLUSH_ALL:       autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_ALL;       break;
//////  default:
//////    #ifndef NDEBUG
//////      naError("naSetBufferNewLineEncoding", "Unhandeled case.");
//////    #endif
//////    autoflushflag = NA_BUFFER_FLAG_AUTOFLUSH_AUTO;
//////    break;
//////  }
//////  buffer->flags |= autoflushflag;
////}
////
////
////
////// You may ask yourself the question why the author decided to reimplement the
////// different flags again and use a case to set them correctly. This is because
////// the newline flags are actually an enumeration and not macros. Mixing the
////// two can lead to hard to understand compile errors especially if the enum
////// will be enhanced in the future (who knows). By reimplementing the flags
////// again, we can be sure that the NABuffer implementation is independent of
////// the enumeration.
////NA_DEF void naSetBufferNewLineEncoding(NABuffer* buffer, NANewlineEncoding newlineencoding){
//////  NAUInt newlineflag;
//////  #ifndef NDEBUG
//////    if(naGetBufferKind(buffer) != NA_BUFFER_KIND_OUTPUT)
//////      naError("naSetBufferNewLineEncoding", "Setting the newline encoding is only useful for output buffers.");
//////  #endif
//////  buffer->flags &= ~NA_BUFFER_FLAG_NEWLINE_MASK;
//////  switch(newlineencoding){
//////  case NA_NEWLINE_UNIX:   newlineflag = NA_BUFFER_FLAG_NEWLINE_UNIX;   break;
//////  case NA_NEWLINE_MAC9:   newlineflag = NA_BUFFER_FLAG_NEWLINE_MAC9;   break;
//////  case NA_NEWLINE_WIN:    newlineflag = NA_BUFFER_FLAG_NEWLINE_WIN;    break;
//////  case NA_NEWLINE_NATIVE: newlineflag = NA_BUFFER_FLAG_NEWLINE_NATIVE; break;
//////  default:
//////    #ifndef NDEBUG
//////      naError("naSetBufferNewLineEncoding", "Unhandeled case.");
//////    #endif
//////    break;
//////  }
//////  buffer->flags |= newlineflag;
////}
//
//
////NA_DEF void naSetBufferSecure(NABuffer* buffer, NABool secure){
////  if(secure){
////    buffer->flags |= NA_BUFFER_FLAG_SECURE;
////  }else{
////    buffer->flags &= ~NA_BUFFER_FLAG_SECURE;
////  }
////}
//
//
//
//NA_DEF NABool naIsBufferEmpty(const NABuffer* buffer){
//  return naIsRangeiEmpty(buffer->range);
//}
//
//
//
//NA_DEF void naSeekBufferAbsolute(NABuffer* buffer, NAInt offset){
//  NAInt absoffset = offset;
//  #ifndef NDEBUG
//    NABufferStorage* storage = naGetBufferStorage(buffer);
//    if((storage->sourcetype == NA_BUFFER_STORAGE_SOURCE_FILE) && (absoffset < 0))
//      naError("naSeekBufferAbsolute", "absolute offset of file buffer is negative");
//    if(naHasBufferDeterminedBytesize(buffer) && !naContainsRangeiOffset(buffer->range, absoffset))
//      naError("naSeekBufferAbsolute", "absolute offset lies outside of this buffer");
//  #endif
//  buffer->curoffset = absoffset;
//}
//
//
//
//NA_DEF void naSeekBufferLocal(NABuffer* buffer, NAInt offset){
//  NAInt absoffset = buffer->range.origin + offset;
//  #ifndef NDEBUG
//    NABufferStorage* storage = naGetBufferStorage(buffer);
//    if((storage->sourcetype == NA_BUFFER_STORAGE_SOURCE_FILE) && (absoffset < 0))
//      naError("naSeekBufferLocal", "absolute offset of file buffer is negative");
//    if(naHasBufferDeterminedBytesize(buffer) && !naContainsRangeiOffset(buffer->range, absoffset))
//      naError("naSeekBufferLocal", "absolute offset lies outside of this buffer");
//    if((offset < 0) && (absoffset > buffer->curoffset))
//      naError("naSeekBufferLocal", "position underflow");
//    if((offset > 0) && (absoffset < buffer->curoffset))
//      naError("naSeekBufferLocal", "position overflow");
//  #endif
//  naSeekBufferAbsolute(buffer, absoffset);
//}
//
//
//
//NA_DEF void naSeekBufferRelative(NABuffer* buffer, NAInt offset){
//  NAInt absoffset = buffer->curoffset + offset;
//  #ifndef NDEBUG
//    NABufferStorage* storage = naGetBufferStorage(buffer);
//    if((storage->sourcetype == NA_BUFFER_STORAGE_SOURCE_FILE) && (absoffset < 0))
//      naError("naSeekBufferRelative", "absolute offset of file buffer is negative");
//    if(naHasBufferDeterminedBytesize(buffer) && !naContainsRangeiOffset(buffer->range, absoffset))
//      naError("naSeekBufferRelative", "absolute offset lies outside of this buffer");
//    if((offset < 0) && (absoffset > buffer->curoffset))
//      naError("naSeekBufferRelative", "position underflow");
//    if((offset > 0) && (absoffset < buffer->curoffset))
//      naError("naSeekBufferRelative", "position overflow");
//  #endif
//  naSeekBufferAbsolute(buffer, absoffset);
//}
//
//
//
//
//
//// Helper function not visible to the programmer. Will make sure that there
//// are enough bytes in the buffer.
//// Note that this is quite a heavy function. But in 99% of all cases, this
//// function only executes about 10-20 statements. Nonteless, it ensures safe
//// usage of any buffer.
//NA_DEF void naEnhanceBuffer(NABuffer* buffer, NAInt bytesize){
//  NABufferPart* curpart;
//  NAInt curoffset;
//  NARangei desiredrange = naMakeRangei(buffer->curoffset, bytesize);
//  NABufferStorage* storage = naGetBufferStorage(buffer);
//  NAList* partlist = naGetBufferPartList(buffer);
//  NAListIterator iter = naMakeListIteratorModifier(partlist);
//
//  #ifndef NDEBUG
//    if(!bytesize)
//      naError("naEnhanceBuffer", "bytesize is zero.");
//    if(bytesize < 0)
//      naError("naEnhanceBuffer", "bytesize is negative.");
//  #endif
//  
//  // Adjust the total buffer range if applicable and necessary
//  if(naCanBufferExtend(buffer) && !naHasBufferDeterminedBytesize(buffer)){
//    naEnlargeBufferStorage(buffer, desiredrange);
//  }else{
//    #ifndef NDEBUG
//      if(!naContainsRangeiRange(buffer->range, desiredrange))
//        naError("naEnhanceBuffer", "enhancement goes beyound fixed buffer boundary.");
//    #endif
//  }
//
//  // Set curpos to the current byte position.
//  curoffset = buffer->curoffset;
//  naLocateListPosition(&iter, buffer->curlistpos);
//  naLocateBufferPartList(&iter, buffer->curoffset);
//  curpart = naGetListCurrentMutable(&iter);
//  
//  // While there are still bytes to be enhanced...
//  while(bytesize){
//  
//    #ifndef NDEBUG
//      if(!curpart)
//        naError("naEnhanceBuffer", "Internal error: No buffer part available");
//      if(!naIsOffsetInBufferPart(curpart, curoffset))
//        naError("naEnhanceBuffer", "Internal error: cur offset not in cur buffer part");
//    #endif
//  
//    // If there is a current part and the current position is within and it
//    // is no sparse buffer part...
//    if(!naIsBufferPartSparse(curpart)){
//      // We are sure that curpart contains the start of our enhancement.
//      
//      // We compute the remaining bytesize in the buffer part.
//      NAInt remainingbytesize = naGetBufferPartEndBufferIndex(curpart) - curoffset;
//      if(bytesize <= remainingbytesize){
//        // We have sufficient space in this buf.
//        // Adjust the data range of the buffer part.
//        naAdjustBufferPartToContainDataRange(curpart, naMakeRangei(curoffset, bytesize));
//        // We are done. No more bytes to enhance, all buffer parts updated.
//        // We set bytesize to 0 to end the loop.
//        bytesize = 0;
//      
//      }else{
//        // The desired content does not fit completely. We use all of the
//        // remaining bytes in the current buffer.
//        // Adjust the data range of the buffer part.
//        naAdjustBufferPartToContainDataRange(curpart, naMakeRangei(curoffset, remainingbytesize));
//        curoffset += remainingbytesize;
//        bytesize -= remainingbytesize;
//        
//        // If there is a next bufferpart, we iterate to it so that in the
//        // next loop, we are already in the correct position if available.
//        naIterateList(&iter, 1);
//        curpart = naGetListCurrentMutable(&iter);
//      }
//    
//    }else{
//      // Reaching here, the current buffer is a sparse buffer.
//      NABufferPart* prevpart;
//      NABufferPart* nextpart;
//
//      // Reaching here, we either have no part or the current part is a
//      // sparse part. Either way, we will create a new buffer part.
//      NARangei newbufferrange;
//      // We define the default buffer part boundaries.
//      if(curoffset < 0){
//        newbufferrange = naMakeRangei(((curoffset / NA_BUFFER_BYTESIZE) - 1) * NA_BUFFER_BYTESIZE, NA_BUFFER_BYTESIZE);
//      }else{
//        newbufferrange = naMakeRangei(((curoffset / NA_BUFFER_BYTESIZE) - 0) * NA_BUFFER_BYTESIZE, NA_BUFFER_BYTESIZE);
//      }
//      
//      // We clamp the boundaries to the current buffer if applicable.
//      // Theoretically, this should not be necessary, but we do it to be sure.
//      prevpart = naGetListPrevMutable(&iter);
//      if(prevpart){newbufferrange = naClampRangeiToMin(newbufferrange, naGetRangeiEnd(prevpart->bufferrange));}
//      nextpart = naGetListNextMutable(&iter);
//      if(nextpart){newbufferrange = naClampRangeiToEnd(newbufferrange, nextpart->bufferrange.origin);}
//      
//      #ifndef NDEBUG
//        if(!naIsRangeiUseful(newbufferrange))
//          naError("naEnhanceBuffer", "New buffer range not useful");
//      #endif
//      
//      naFillBufferSparsePart(storage, &iter, newbufferrange);
//      curpart = naGetListCurrentMutable(&iter);
//
//    }
//  }
//  // Reaching here, the buffer is enhanced with all desired bytes and
//  // ready to be used.
//
//  // Adjust the total buffer range if applicable and necessary
//  if(naCanBufferExtend(buffer) && !naHasBufferDeterminedBytesize(buffer)){
//    NAListPos firstpos;
//    NAListPos lastpos;
//
//    if(naIsRangeiEmpty(buffer->range)){
//      naLocateBufferPartList(&iter, desiredrange.origin);
//      firstpos = naGetListCurrentPosition(&iter);
//      naLocateBufferPartList(&iter, naGetRangeiMax(desiredrange));
//      lastpos = naGetListCurrentPosition(&iter);
//      naRetainBufferParts(storage, firstpos, lastpos);
//    }else{
//      if(desiredrange.origin < buffer->range.origin){
//        naLocateBufferPartList(&iter, desiredrange.origin);
//        firstpos = naGetListCurrentPosition(&iter);
//        naLocateBufferPartList(&iter, buffer->range.origin);
//        naIterateList(&iter, -1);
//        lastpos = naGetListCurrentPosition(&iter);
//        naRetainBufferParts(storage, firstpos, lastpos);
//      }
//      if(naGetRangeiMax(desiredrange) > naGetRangeiMax(buffer->range)){
//        naLocateBufferPartList(&iter, naGetRangeiMax(buffer->range));
//        naIterateList(&iter, 1);
//        firstpos = naGetListCurrentPosition(&iter);
//        naLocateBufferPartList(&iter, naGetRangeiMax(desiredrange));
//        lastpos = naGetListCurrentPosition(&iter);
//        naRetainBufferParts(storage, firstpos, lastpos);
//      }
//    }
//    
//    buffer->range = naMakeRangeiWithRangeAndRange(buffer->range, desiredrange);
//
//  }
//
//  // Finally, set the current position to the one holding the first byte.
//  naLocateListPosition(&iter, buffer->curlistpos);
//  naLocateBufferPartList(&iter, buffer->curoffset);
//  buffer->curlistpos = naGetListCurrentPosition(&iter);
//
//  naClearListIterator(&iter);
//
//}
//
//
//
//
//
//NA_DEF void naReadBuffer (NABuffer* buffer, NAFilesize bytecount){
//  if(bytecount){
//    naEnhanceBuffer(buffer, bytecount);
//    buffer->curoffset += bytecount;
//  }
//}
//
//
//
//
////NA_DEF void naAccumulateBufferToChecksum(NABuffer* buffer, NAChecksum* checksum){
////  NAInt bytesize;
////  NAInt curoffset;
////  NAList* partlist;
////  NAListIterator iter;
////
////  #ifndef NDEBUG
////    if(!naHasBufferDeterminedBytesize(buffer))
////      naError("naAccumulateBufferToChecksum", "Buffer has no known max position. Use naDetermineBufferBytesize or naDetermineBufferBytesize");
////  #endif
////
////  bytesize = buffer->range.length;
////  if(!bytesize){return;}
////
////  partlist = naGetBufferPartList(buffer);
////  curoffset = buffer->range.origin;
////  iter = naMakeListIteratorMutator(partlist);
////  naLocateBufferPartList(&iter, curoffset);
////  
////  while(bytesize){
////    NABufferPart* curpart;
////    NAInt remainingbytes;
////    NAByte* src;
////
////    curpart = naGetListCurrentMutable(&iter);
////    remainingbytes = naGetBufferPartEndDataIndex(curpart) - curoffset;
////    src = naGetBufferPartDataPointerAbsolute(curpart, curoffset);
////    
////    #ifndef NDEBUG
////      if(naIsBufferPartSparse(curpart))
////        naError("naAccumulateBufferToChecksum", "Buffer contains sparse parts. Can not compute checksum");
////    #endif
////    
////    if(bytesize > remainingbytes){
////      naAccumulateChecksum(checksum, src, remainingbytes);
////      naIterateList(&iter, 1);
////      curoffset += remainingbytes;
////      bytesize -= remainingbytes;
////    }else{
////      naAccumulateChecksum(checksum, src, bytesize);
////      bytesize = 0;
////    }
////  }
////  
////  naClearListIterator(&iter);
////}
//
//
//// ///////////////////////////////////////////////////////////////
//// READING
//// ///////////////////////////////////////////////////////////////
//
//
//
//
//
//NA_HDEF void naRetrieveBufferBytes(NABuffer* buffer, void* ptr, NAInt bytesize, NAUInt lastbytemodulobits){
//  NABufferPart* curpart;
//  NAList* partlist = naGetBufferPartList(buffer);
//  NAListIterator iter;
//
//  #ifndef NDEBUG
//    if(!bytesize)
//      naError("naRetrieveBufferBytes", "Reading zero bytes.");
//    if(bytesize < 0)
//      naError("naRetrieveBufferBytes", "Negative count.");
//  #endif
//  if(!bytesize){return;}
//
//  iter = naMakeListIteratorMutator(partlist);
//  naLocateListPosition(&iter, buffer->curlistpos);
//  curpart = naGetListCurrentMutable(&iter);
//
//  while(bytesize){
//    NAInt remainingbytes;
//    NAByte* src;
//
//    #ifndef NDEBUG
//      if(!buffer->curlistpos)
//        naError("naRetrieveBufferBytes", "Internal error: No buffer part available.");
//      if(!curpart || !naIsOffsetInBufferPart(curpart, buffer->curoffset))
//        {naCrash("naRetrieveBufferBytes", "Internal error: Current buffer part unavailable or not containing current position."); return;}
//    #endif
//    
//    remainingbytes = naGetBufferPartEndBufferIndex(curpart) - buffer->curoffset;
//    src = naGetBufferPartDataPointerAbsolute(curpart, buffer->curoffset);
//    if(bytesize > remainingbytes){
//      naCopyn(ptr, src, (NAUInt)remainingbytes);
//      buffer->curoffset += remainingbytes;
//      bytesize -= remainingbytes;
//      
//      *((NAByte**)(&ptr)) += remainingbytes;
//      naIterateList(&iter, 1);
//      buffer->curlistpos = naGetListCurrentPosition(&iter);
//      curpart = naGetListCurrentMutable(&iter);
//    }else{
//      naCopyn(ptr, src, (NAUInt)bytesize);
//      buffer->curoffset += bytesize;
//      bytesize = NA_ZERO;
//    }
//  }
//  
//  buffer->curbit = (uint8)lastbytemodulobits;
//  if(lastbytemodulobits){buffer->curoffset--;}
//  
//  naClearListIterator(&iter);
//}
//
//
//
//
//NA_DEF void naReadBufferBytes(NABuffer* buffer, void* buf, NAInt bytesize){
//  #ifndef NDEBUG
//    if(!bytesize)
//      naError("naReadBufferBytes", "Reading zero bytes.");
//    if(bytesize < 0)
//      naError("naReadBufferBytes", "Negative count.");
//    if(buffer->curbit != 0)
//      naError("naReadBufferBytes", "Bit offset not 0.");
//  #endif
//  naEnhanceBuffer(buffer, bytesize);
//  naRetrieveBufferBytes(buffer, buf, bytesize, 0);
//}
//
//
//
////NA_DEF NAByteArray* naInitByteArrayFromBufferInput(NAByteArray* array, NABuffer* buffer, NAInt bytesize){
////  #ifndef NDEBUG
////    if(!count)
////      naError("naInitByteArrayFromBufferInput", "Reading zero bytes.");
////    if(count < 0)
////      naError("naInitByteArrayFromBufferInput", "Negative count.");
////    if(buffer->curbit != 0)
////      naError("naInitByteArrayFromBufferInput", "Bit offset not 0.");
////  #endif
////  if(!count){
////    naInitByteArray(array);
////  }else{
////    naInitByteArrayWithBytesize(array, count);
////    naReadBufferBytes(buffer, naGetByteArrayMutablePointer(array), count);
////  }
////  return array;
////}
////
////
////
////NA_DEF NABool naReadBufferBit (NABuffer* buffer){
////  uint8 curbit;
////  NAUInt requiredbitcount;
////  NAUInt lastbytemodulobits;
////  NAByte value;
////  NABool bit;
////
////  curbit = buffer->curbit;
////  requiredbitcount = curbit + 1;
////  lastbytemodulobits = requiredbitcount % 8;
////
////  naEnhanceBuffer(buffer, 1);
////  naRetrieveBufferBytes(buffer, &value, 1, lastbytemodulobits);
////
////  bit = (value >> curbit) & 0x01;
////  return bit;
////}
//
//
//
////NA_DEF NAUInt naReadBufferBits (NABuffer* buffer, uint8 count){
////  NAUInt retint = 0;
////  NABool curbit;
////  uint32 curmask = 1;
////  #ifndef NDEBUG
////    if(count > NA_SYSTEM_ADDRESS_BITS)
////      naError("naReadBufferBits", "Can read %d bits at max.", NA_SYSTEM_ADDRESS_BITS);
////  #endif
////  while(count){
////    curbit = naReadBufferBit(buffer);
////    retint |= curmask * (uint32)curbit;
////    curmask <<= 1;
////    count--;
////  }
////  return retint;
////
//////  #ifndef NDEBUG
//////    if(!count)
//////      naError("naReadBufferBits", "Reading zero bits.");
//////    if(count > 32)
//////      naError("naReadBufferBits", "Can not read more than 32 bits at once.");
//////  #endif
//////  uint8 curbit = buffer->curbit;
//////  uint8 lastbytebits = count % 8;
//////  NAUInt requiredbitcount = curbit + count;
//////  NAUInt lastbytemodulobits = requiredbitcount % 8;
//////  NAUInt requiredbytecount = requiredbitcount / 8;
//////  if(lastbytemodulobits){requiredbytecount++;}
//////  const NAByte* src = naRequireBufferReadBytes(buffer, requiredbytecount, NA_TRUE, lastbytemodulobits);
//////
//////  uint32 retint = 0;
//////  NAByte* buf = (NAByte*)(&retint);
//////  buf += 3;
//////
//////  while(1){
//////    *buf = (*src >> curbit);
//////    if(count <= (8 - curbit)){break;}
//////    count -= (8 - curbit);
//////    src++;
//////    *buf |= (*src << (8 - curbit));
//////    if(count <= curbit){break;}
//////    count -= curbit;
//////    buf--;
//////  }
//////
//////  if(lastbytebits){
//////    *buf = ((*buf << (8 - lastbytebits)) & 0xff) >> (8 - lastbytebits);
//////  }
//////  
//////  naConvertNativeBig32(&retint);
//////  return retint;
////}
//
//
////NA_DEF void naPadBufferReadBits(NABuffer* buffer){
////  if(buffer->curbit != 0){
////    #ifndef NDEBUG
////      if(buffer->flags & NA_BUFFER_FLAG_SECURE){
////        NAByte testbyte = naReadBufferu8(buffer);
////        testbyte >>= buffer->curbit;
////        if(testbyte != 0)
////          naError("naPadBufferReadBits", "Padding Bits not Null in secure buffer.");
////      }
////    #endif
////    buffer->curbit = 0;
////    buffer->curoffset++;
////  }
////}
//
//
//
//// Note that creating a helper function for reading small number of bytes is
//// not really useful as there are too many things which can go wrong. Instead,
//// the author decided to simply implement all functions plainly.
//
////NA_DEF int8 naReadBufferi8(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  int8 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferi8", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 1);
////  naRetrieveBufferBytes(buffer, &value, 1, 0);
////  buffer->converter.convert8(&value);
////  return value;
////}
////NA_DEF int16 naReadBufferi16(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  int16 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferi16", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 2);
////  naRetrieveBufferBytes(buffer, &value, 2, 0);
////  buffer->converter.convert16(&value);
////  return value;
////}
////NA_DEF int32 naReadBufferi32(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  int32 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferi32", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 4);
////  naRetrieveBufferBytes(buffer, &value, 4, 0);
////  buffer->converter.convert32(&value);
////  return value;
////}
////NA_DEF int64 naReadBufferi64(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  int64 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferi64", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 8);
////  naRetrieveBufferBytes(buffer, &value, 8, 0);
////  buffer->converter.convert64(&value);
////  return value;
////}
////
////
////
////NA_DEF uint8 naReadBufferu8(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  uint8 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferu8", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 1);
////  naRetrieveBufferBytes(buffer, &value, 1, 0);
////  buffer->converter.convert8(&value);
////  return value;
////}
////NA_DEF uint16 naReadBufferu16(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  uint16 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferu16", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 2);
////  naRetrieveBufferBytes(buffer, &value, 2, 0);
////  buffer->converter.convert16(&value);
////  return value;
////}
////NA_DEF uint32 naReadBufferu32(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  uint32 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferu32", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 4);
////  naRetrieveBufferBytes(buffer, &value, 4, 0);
////  buffer->converter.convert32(&value);
////  return value;
////}
////NA_DEF uint64 naReadBufferu64(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  uint64 value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferu64", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 8);
////  naRetrieveBufferBytes(buffer, &value, 8, 0);
////  buffer->converter.convert64(&value);
////  return value;
////}
//
//
//
////NA_DEF float naReadBufferf(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  float value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferf", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 4);
////  naRetrieveBufferBytes(buffer, &value, 4, 0);
////  buffer->converter.convert32(&value);
////  return value;
////}
////NA_DEF double naReadBufferd(NABuffer* buffer){
////  // Declaration before implementation. Needed for C90.
////  double value;
////  #ifndef NDEBUG
////    if(buffer->curbit != 0)
////      naError("naReadBufferd", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, 8);
////  naRetrieveBufferBytes(buffer, &value, 8, 0);
////  buffer->converter.convert64(&value);
////  return value;
////}
//
//
//
////NA_DEF void naReadBufferi8v(NABuffer* buffer, int8* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferi8v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferi8v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 1);
////  naRetrieveBufferBytes(buffer, dst, count * 1, 0);
////  buffer->converter.convert8v(dst, (NAUInt)count);
////}
////NA_DEF void naReadBufferi16v(NABuffer* buffer, int16* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferi16v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferi16v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 2);
////  naRetrieveBufferBytes(buffer, dst, count * 2, 0);
////  buffer->converter.convert16v(dst, (NAUInt)count);
////}
////NA_DEF void naReadBufferi32v(NABuffer* buffer, int32* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferi32v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferi32v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 4);
////  naRetrieveBufferBytes(buffer, dst, count * 4, 0);
////  buffer->converter.convert32v(dst, (NAUInt)count);
////}
////NA_DEF void naReadBufferi64v(NABuffer* buffer, int64* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferi64v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferi64v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 8);
////  naRetrieveBufferBytes(buffer, dst, count * 8, 0);
////  buffer->converter.convert64v(dst, (NAUInt)count);
////}
////
////
////
////NA_DEF void naReadBufferu8v(NABuffer* buffer, uint8* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferu8v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferu8v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 1);
////  naRetrieveBufferBytes(buffer, dst, count * 1, 0);
////  buffer->converter.convert8v(dst, (NAUInt)count);
////}
////NA_DEF void naReadBufferu16v(NABuffer* buffer, uint16* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferu16v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferu16v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 2);
////  naRetrieveBufferBytes(buffer, dst, count * 2, 0);
////  buffer->converter.convert16v(dst, (NAUInt)count);
////}
////NA_DEF void naReadBufferu32v(NABuffer* buffer, uint32* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferu32v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferu32v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 4);
////  naRetrieveBufferBytes(buffer, dst, count * 4, 0);
////  buffer->converter.convert32v(dst, (NAUInt)count);
////}
////NA_DEF void naReadBufferu64v(NABuffer* buffer, uint64* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferu64v", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferu64v", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 8);
////  naRetrieveBufferBytes(buffer, dst, count * 8, 0);
////  buffer->converter.convert64v(dst, (NAUInt)count);
////}
////
////
////
////NA_DEF void naReadBufferfv(NABuffer* buffer, float* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      naError("naReadBufferfv", "dst is Null pointer.");
////    if(buffer->curbit != 0)
////      naError("naReadBufferfv", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 4);
////  naRetrieveBufferBytes(buffer, dst, count * 4, 0);
////  buffer->converter.convert32v(dst, (NAUInt)count);
////}
////NA_DEF void naReadBufferdv(NABuffer* buffer, double* dst, NAInt count){
////  #ifndef NDEBUG
////    if(!dst)
////      {naCrash("naReadBufferdv", "dst is Null pointer."); return;}
////    if(buffer->curbit != 0)
////      naError("naReadBufferdv", "Bit offset not 0.");
////  #endif
////  naEnhanceBuffer(buffer, count * 8);
////  naRetrieveBufferBytes(buffer, dst, count * 8, 0);
////  buffer->converter.convert64v(dst, (NAUInt)count);
////}
//
//
//
//
//
//// /////////////////////////////////////////////////////////////////
//// WRITING
//// /////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//NA_DEF void naStoreBufferBytes(NABuffer* buffer, const void* ptr, NAInt bytesize){
//  NABufferPart* curpart;
//  NAList* partlist;
//  NAListIterator iter;
//
//  #ifndef NDEBUG
//    if(!bytesize)
//      naError("naStoreBufferBytes", "Writing zero bytes.");
//    if(bytesize < 0)
//      naError("naStoreBufferBytes", "Negative count.");
//  #endif
//  if(!bytesize){return;}
//
//  partlist = naGetBufferPartList(buffer);
//  iter = naMakeListIteratorMutator(partlist);
//  naLocateListPosition(&iter, buffer->curlistpos);
//  curpart = naGetListCurrentMutable(&iter);
//  
//  while(bytesize){
//    NAInt remainingbytes;
//    NAByte* dst;
//    
//    #ifndef NDEBUG
//      if(!buffer->curlistpos)
//        naError("naStoreBufferBytes", "Internal error: No buffer part available.");
//      if(!curpart || !naIsOffsetInBufferPart(curpart, buffer->curoffset))
//        naError("naStoreBufferBytes", "Internal error: Current buffer part inavailable or not containing current position.");
//    #endif
//    
//    remainingbytes = naGetBufferPartEndBufferIndex(curpart) - buffer->curoffset;
//    dst = naGetBufferPartDataPointerAbsolute(curpart, buffer->curoffset);
//
//    if(bytesize > remainingbytes){
//      naCopyn(dst, ptr, (NAUInt)remainingbytes);
//      buffer->curoffset += remainingbytes;
//      bytesize -= remainingbytes;
//      
//      *((NAByte**)(&ptr)) += remainingbytes;
//      naIterateList(&iter, 1);
//      buffer->curlistpos = naGetListCurrentPosition(&iter);
//      curpart = naGetListCurrentMutable(&iter);
//    }else{
//      naCopyn(dst, ptr, (NAUInt)bytesize);
//      buffer->curoffset += bytesize;
//      bytesize = NA_ZERO;
//    }
//  }
//  
////  buffer->curbit = (uint8)lastbytemodulobits;
////  if(lastbytemodulobits){buffer->curoffset--;}
//
//  naClearListIterator(&iter);
//}
//
//
//
//NA_DEF void naWriteBufferBytes(NABuffer* buffer, const void* ptr, NAInt bytesize){
//  #ifndef NDEBUG
//    if(!bytesize)
//      naError("naReadBufferBytes", "Writing zero bytes.");
//    if(bytesize < 0)
//      naError("naReadBufferBytes", "Negative count.");
//  #endif
//  if(!bytesize){return;}
//  naEnhanceBuffer(buffer, bytesize);
//  naStoreBufferBytes(buffer, ptr, bytesize);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
//}
//
//
//
//NA_DEF void naWriteBufferByteArray(NABuffer* buffer, const NAByteArray* bytearray){
//  naWriteBufferBytes(buffer, naGetByteArrayConstPointer(bytearray), naGetByteArrayBytesize(bytearray));
//}
//
//
//
//NA_DEF void naWriteBufferString(NABuffer* buffer, const NAString* string){
//  naEnhanceBuffer(buffer, naGetStringBytesize(string));
//  naStoreBufferBytes(buffer, naGetStringUTF8Pointer(string), naGetStringBytesize(string));
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
//}
//
//
//
//NA_DEF void naWriteBufferStringWithFormat(NABuffer* buffer,
//                       const NAUTF8Char* format,
//                                         ...){
//  // Declaration before implementation. Needed for C90.
//  va_list argumentlist;
//  va_start(argumentlist, format);
//  naWriteBufferStringWithArguments(buffer, format, argumentlist);
//  va_end(argumentlist);
//}
//
//
//
//NA_DEF void naWriteBufferStringWithArguments(NABuffer* buffer,
//                          const NAUTF8Char* format,
//                                    va_list argumentlist){
//  // Declaration before implementation. Needed for C90.
//  NAString* string;
//  string = naNewStringWithArguments(format, argumentlist);
//  naWriteBufferString(buffer, string);
//  naDelete(string);
//}
//
//
//
////NA_DEF void naWriteBufferBuffer(NABuffer* dstbuffer, NABuffer* srcbuffer, NAInt bytesize){
////  NAList* srcbuflist = naGetBufferPartList(srcbuffer);
////  NAListIterator iter;
////  void* srcptr;
////
////  #ifndef NDEBUG
////    if(bytesize < 0)
////      naError("naWriteBufferBuffer", "bytesize should be >= 0.");
////  #endif
////  if(bytesize == 0){return;}
////  
////  iter = naMakeListIteratorMutator(srcbuflist);
////  naLocateListPosition(&iter, srcbuffer->curlistpos);
////  naLocateBufferPartList(&iter, srcbuffer->curoffset);
////  while(bytesize != NA_ZERO){
////    NABufferPart* curpart = naGetListCurrentMutable(&iter);
////    NAInt remainingsize = naGetBufferPartEndDataIndex(curpart) - srcbuffer->curoffset;
////    remainingsize = naMini(remainingsize, bytesize);
////    srcptr = naGetBufferPartDataPointerAbsolute(curpart, srcbuffer->curoffset);
////    naWriteBufferBytes(dstbuffer, srcptr, remainingsize);
////    srcbuffer->curoffset += remainingsize;
////    bytesize -= remainingsize;
////    naIterateList(&iter, 1);
////  }
////  naClearListIterator(&iter);
////}
////
//
//
////NA_DEF void naRepeatBufferBytes(NABuffer* buffer, NAInt distance, NAInt bytesize){
////  NAList* partlist;
////  NAInt finalpos;
////  NAInt writeoffset;
////  NAInt readoffset;
////  NABufferPart* writepart;
////  NABufferPart* readpart;
////  NAInt remainingwrite;
////  NAInt remainingread;
////  NAByte* writebyte;
////  NAByte* readbyte;
////  NAListIterator writeiter;
////  NAListIterator readiter;
////
////  #ifndef NDEBUG
////    if(bytesize <= 0)
////      naError("naRepeatBufferBytes", "count should be > 0.");
////    if(distance <= 0)
////      naError("naRepeatBufferBytes", "distance should be > 0.");
////  #endif
////  partlist = naGetBufferPartList(buffer);
////  finalpos = buffer->curoffset + bytesize;
////
////  // Prepare the write part:
////  // Enhance the buffer so that the write part can be stored:
////  naEnhanceBuffer(buffer, bytesize);
////
////  // Position the write iterator.
////  writeiter = naMakeListIteratorMutator(partlist);
////  naLocateListPosition(&writeiter, buffer->curlistpos);
////  writeoffset = buffer->curoffset;
////  naLocateBufferPartList(&writeiter, writeoffset);
////  writepart = naGetListCurrentMutable(&writeiter);
////  remainingwrite = naGetBufferPartEndDataIndex(writepart) - writeoffset;
////  writebyte = naGetBufferPartDataPointerAbsolute(writepart, writeoffset);
////  
////  // Prepare the read part:
////  // Enhance the buffer so that the read part is available:
////  naSeekBufferRelative(buffer, -distance);
////  naEnhanceBuffer(buffer, bytesize);
////
////  // Position the read iterator.
////  readiter = naMakeListIteratorMutator(partlist);
////  naLocateListPosition(&readiter, buffer->curlistpos);
////  readoffset = buffer->curoffset;
////  naLocateBufferPartList(&readiter, readoffset);
////  readpart = naGetListCurrentMutable(&readiter);
////  remainingread = naGetBufferPartEndDataIndex(readpart) - readoffset;
////  remainingread = naMini(remainingread, distance);
////  readbyte = naGetBufferPartDataPointerAbsolute(readpart, readoffset);
////  
////  // Now start copying the buffers.
////  while(1){
////    if(remainingwrite < remainingread){
////      // Limit the writing to the actual bytesize requested, copy all bytes.
////      remainingwrite = naMini(remainingwrite, bytesize);
////      naCopyn(writebyte, readbyte, remainingwrite);
////      bytesize -= remainingwrite;
////      if(!bytesize){break;}
////      // Adjust the byte oriented variables.
////      writeoffset += remainingwrite;
////      readoffset += remainingwrite;
////      writebyte += remainingwrite;
////      readbyte += remainingwrite;
////    }else{
////      // Limit the reading to the actual bytesize requested, copy all bytes.
////      remainingread = naMini(remainingread, bytesize);
////      naCopyn(writebyte, readbyte, remainingread);
////      bytesize -= remainingread;
////      if(!bytesize){break;}
////      // Adjust the byte oriented variables.
////      writeoffset += remainingread;
////      readoffset += remainingread;
////      writebyte += remainingread;
////      readbyte += remainingread;
////    }
////    
////    remainingwrite = naGetBufferPartEndDataIndex(writepart) - writeoffset;
////    remainingread = naGetBufferPartEndDataIndex(readpart) - readoffset;
////    
////    if(remainingwrite == NA_ZERO){
////      naIterateList(&writeiter, 1);
////      writepart = naGetListCurrentMutable(&writeiter);
////      remainingwrite = naGetBufferPartEndDataIndex(writepart) - writeoffset;
////      writebyte = naGetBufferPartDataPointerAbsolute(writepart, writeoffset);
////    }
////    if(remainingread == NA_ZERO){
////      naIterateList(&readiter, 1);
////      readpart = naGetListCurrentMutable(&readiter);
////      remainingread = naGetBufferPartEndDataIndex(readpart) - readoffset;
////      readbyte = naGetBufferPartDataPointerAbsolute(readpart, readoffset);
////    }
////    remainingread = naMini(remainingread, distance);
//////    remainingwrite = naMini(remainingwrite, distance);
////  }
////
////  naClearListIterator(&readiter);
////  naClearListIterator(&writeiter);
////  naSeekBufferAbsolute(buffer, finalpos);
////}
////
//
//
////NA_DEF void naWriteBufferi8(NABuffer* buffer, int8 value){
////  buffer->converter.convert8(&value);
////  naEnhanceBuffer(buffer, 1);
////  naStoreBufferBytes(buffer, &value, 1);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferi16(NABuffer* buffer, int16 value){
////  buffer->converter.convert16(&value);
////  naEnhanceBuffer(buffer, 2);
////  naStoreBufferBytes(buffer, &value, 2);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferi32(NABuffer* buffer, int32 value){
////  buffer->converter.convert32(&value);
////  naEnhanceBuffer(buffer, 4);
////  naStoreBufferBytes(buffer, &value, 4);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferi64(NABuffer* buffer, int64 value){
////  buffer->converter.convert64(&value);
////  naEnhanceBuffer(buffer, 8);
////  naStoreBufferBytes(buffer, &value, 8);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////
////
////NA_DEF void naWriteBufferu8(NABuffer* buffer, uint8 value){
////  buffer->converter.convert8(&value);
////  naEnhanceBuffer(buffer, 1);
////  naStoreBufferBytes(buffer, &value, 1);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferu16(NABuffer* buffer, uint16 value){
////  buffer->converter.convert16(&value);
////  naEnhanceBuffer(buffer, 2);
////  naStoreBufferBytes(buffer, &value, 2);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferu32(NABuffer* buffer, uint32 value){
////  buffer->converter.convert32(&value);
////  naEnhanceBuffer(buffer, 4);
////  naStoreBufferBytes(buffer, &value, 4);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferu64(NABuffer* buffer, uint64 value){
////  buffer->converter.convert64(&value);
////  naEnhanceBuffer(buffer, 8);
////  naStoreBufferBytes(buffer, &value, 8);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////
////
////NA_DEF void naWriteBufferf(NABuffer* buffer, float value){
////  buffer->converter.convert32(&value);
////  naEnhanceBuffer(buffer, 4);
////  naStoreBufferBytes(buffer, &value, 4);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferd(NABuffer* buffer, double value){
////  buffer->converter.convert64(&value);
////  naEnhanceBuffer(buffer, 8);
////  naStoreBufferBytes(buffer, &value, 8);
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
////}
//
//
//
//
//
//
//
//
//
////NA_DEF void naWriteBufferi8v(NABuffer* buffer, const int8* src, NAInt count){
////  int8 value;
////  naEnhanceBuffer(buffer, count * 1);
////  while(count){
////    value = *src;
////    buffer->converter.convert8(&value);
////    naStoreBufferBytes(buffer, &value, 1);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferi16v(NABuffer* buffer, const int16* src, NAInt count){
////  int16 value;
////  naEnhanceBuffer(buffer, count * 2);
////  while(count){
////    value = *src;
////    buffer->converter.convert16(&value);
////    naStoreBufferBytes(buffer, &value, 2);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferi32v(NABuffer* buffer, const int32* src, NAInt count){
////  int32 value;
////  naEnhanceBuffer(buffer, count * 4);
////  while(count){
////    value = *src;
////    buffer->converter.convert32(&value);
////    naStoreBufferBytes(buffer, &value, 4);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferi64v(NABuffer* buffer, const int64* src, NAInt count){
////  int64 value;
////  naEnhanceBuffer(buffer, count * 8);
////  while(count){
////    value = *src;
////    buffer->converter.convert64(&value);
////    naStoreBufferBytes(buffer, &value, 8);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferu8v(NABuffer* buffer, const uint8* src, NAInt count){
////  uint8 value;
////  naEnhanceBuffer(buffer, count * 1);
////  while(count){
////    value = *src;
////    buffer->converter.convert8(&value);
////    naStoreBufferBytes(buffer, &value, 1);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferu16v(NABuffer* buffer, const uint16* src, NAInt count){
////  uint16 value;
////  naEnhanceBuffer(buffer, count * 2);
////  while(count){
////    value = *src;
////    buffer->converter.convert16(&value);
////    naStoreBufferBytes(buffer, &value, 2);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferu32v(NABuffer* buffer, const uint32* src, NAInt count){
////  uint32 value;
////  naEnhanceBuffer(buffer, count * 4);
////  while(count){
////    value = *src;
////    buffer->converter.convert32(&value);
////    naStoreBufferBytes(buffer, &value, 4);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferu64v(NABuffer* buffer, const uint64* src, NAInt count){
////  uint64 value;
////  naEnhanceBuffer(buffer, count * 8);
////  while(count){
////    value = *src;
////    buffer->converter.convert64(&value);
////    naStoreBufferBytes(buffer, &value, 8);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferfv(NABuffer* buffer, const float* src, NAInt count){
////  float value;
////  naEnhanceBuffer(buffer, count * 4);
////  while(count){
////    value = *src;
////    buffer->converter.convert32(&value);
////    naStoreBufferBytes(buffer, &value, 4);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
////NA_DEF void naWriteBufferdv(NABuffer* buffer, const double* src, NAInt count){
////  double value;
////  naEnhanceBuffer(buffer, count * 8);
////  while(count){
////    value = *src;
////    buffer->converter.convert64(&value);
////    naStoreBufferBytes(buffer, &value, 8);
////    src++;
////    count--;
////  }
////  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
////}
//
//
//
//
//NA_DEF void naWriteBufferTab(NABuffer* buffer){
//  naEnhanceBuffer(buffer, 1);
//  naStoreBufferBytes(buffer, NA_TAB, 1);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
//}
//
//
//
//NA_DEF void naWriteBufferNewLine(NABuffer* buffer){
//  switch(buffer->flags & NA_BUFFER_FLAG_NEWLINE_MASK){
//  case NA_BUFFER_FLAG_NEWLINE_MAC9:
//    naEnhanceBuffer(buffer, 1);
//    naStoreBufferBytes(buffer, NA_NL_MAC9, 1);
//    break;
//  case NA_BUFFER_FLAG_NEWLINE_UNIX:
//    naEnhanceBuffer(buffer, 1);
//    naStoreBufferBytes(buffer, NA_NL_UNIX, 1);
//    break;
//  case NA_BUFFER_FLAG_NEWLINE_WIN:
//    naEnhanceBuffer(buffer, 2);
//    naStoreBufferBytes(buffer, NA_NL_WIN, 2);
//    break;
//  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_TEXT){naFlushBuffer(buffer);}
//}
//
//
//
//
//
//NA_DEF void naWriteBufferLine(NABuffer* buffer, const NAString* string){
//  naWriteBufferString(buffer, string);
//  naWriteBufferNewLine(buffer);
//}
//NA_DEF void naWriteBufferLineWithFormat(NABuffer* buffer, const NAUTF8Char* format, ...){
//  // Declaration before implementation. Needed for C90.
//  va_list argumentlist;
//  va_start(argumentlist, format);
//  naWriteBufferLineWithArguments(buffer, format, argumentlist);
//  va_end(argumentlist);
//}
//NA_DEF void naWriteBufferLineWithArguments(NABuffer* buffer, const NAUTF8Char* format, va_list argumentlist){
//  naWriteBufferStringWithArguments(buffer, format, argumentlist);
//  naWriteBufferNewLine(buffer);
//}
//
//
//
//
//// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the source-code.
////
//// In case the source-code of this software is inaccessible to the end-user,
//// the above copyright notice and this permission notice shall be included
//// in any source-code which is dependent on this software and is accessible
//// to the end-user.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
