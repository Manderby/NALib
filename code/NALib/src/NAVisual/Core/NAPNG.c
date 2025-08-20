
#include "../NAPNG.h"
#include "../../NAStruct/NABuffer.h"
#include "../NADeflate.h"
#include "../../NAUtility/NADateTime.h"
#include "../../NAUtility/NAFile.h"
#include "../NAImage.h"

// Reference: http://www.w3.org/TR/PNG

#define NA_PNG_FLAGS_IHDR_AVAILABLE       0x01
#define NA_PNG_FLAGS_sRGB_AVAILABLE       0x02


#include "../../NAStruct/NAList.h"
#include "../../NAMath/NAVectorAlgebra.h"

typedef enum{
  NA_PIXEL_UNIT_UNDEFINED,
  NA_PIXEL_UNIT_RATIO,
  NA_PIXEL_UNIT_PER_INCH,
  NA_PIXEL_UNIT_PER_METER
} NAPixelUnit;

typedef enum{
  NA_PNG_INTERLACE_NONE = 0,
  NA_PNG_INTERLACE_ADAM7 = 1
} NAPNGInterlaceMethod;

struct NAPNG{
  NASizes size;
  NAList chunks;
  uint32 flags;
  int8 bitDepth;
  NAPNGColorType colorType;
  int8 compressionMethod;
  int8 filterMethod;
  float gamma;
  NAVec2f whitePoint;
  NAVec2f redPrimary;
  NAVec2f greenPrimary;
  NAVec2f bluePrimary;
  uint8 significantBits[4];
  NAVec2f pixelDimensions;
  NAPixelUnit pixelUnit;
  NADateTime modificationDate;
  NAPNGInterlaceMethod interlaceMethod;

  NAByte* pixelData;  // stored from top to bottom
  NABuffer* compressedData;
  NABuffer* filteredData;
};


NA_HAPI void na_DestructPNG(NAPNG* png);
NA_RUNTIME_TYPE(NAPNG, na_DestructPNG, NA_FALSE);


typedef enum{
  NA_PNG_FILTER_TYPE_NONE     = 0,
  NA_PNG_FILTER_TYPE_SUB      = 1,
  NA_PNG_FILTER_TYPE_UP       = 2,
  NA_PNG_FILTER_TYPE_AVERAGE  = 3,
  NA_PNG_FILTER_TYPE_PAETH    = 4
} NAPNGFilterTypes;


typedef enum{
  NA_PNG_CHUNK_TYPE_IHDR = 0,
  NA_PNG_CHUNK_TYPE_PLTE,
  NA_PNG_CHUNK_TYPE_IDAT,
  NA_PNG_CHUNK_TYPE_IEND,

  NA_PNG_CHUNK_TYPE_cHRM,
  NA_PNG_CHUNK_TYPE_gAMA,
  NA_PNG_CHUNK_TYPE_iCCP,
  NA_PNG_CHUNK_TYPE_sBIT,
  NA_PNG_CHUNK_TYPE_sRGB,
  NA_PNG_CHUNK_TYPE_bKGD,
  NA_PNG_CHUNK_TYPE_hIST,
  NA_PNG_CHUNK_TYPE_tRNS,
  NA_PNG_CHUNK_TYPE_pHYs,
  NA_PNG_CHUNK_TYPE_sPLT,
  NA_PNG_CHUNK_TYPE_tIME,
  NA_PNG_CHUNK_TYPE_iTXt,
  NA_PNG_CHUNK_TYPE_tEXt,
  NA_PNG_CHUNK_TYPE_zTXt,

  NA_PNG_CHUNK_TYPE_UNKNOWN,
  NA_PNG_CHUNK_TYPE_COUNT
} NAChunkType;


NAByte na_PngChunkTypeNames[NA_PNG_CHUNK_TYPE_COUNT][4] = {
  {'I', 'H', 'D', 'R'},       // NA_PNG_CHUNK_TYPE_IHDR
  {'P', 'L', 'T', 'E'},       // NA_PNG_CHUNK_TYPE_PLTE
  {'I', 'D', 'A', 'T'},       // NA_PNG_CHUNK_TYPE_IDAT
  {'I', 'E', 'N', 'D'},       // NA_PNG_CHUNK_TYPE_IEND

  {'c', 'H', 'R', 'M'},       // NA_PNG_CHUNK_TYPE_cHRM
  {'g', 'A', 'M', 'A'},       // NA_PNG_CHUNK_TYPE_gAMA
  {'i', 'C', 'C', 'P'},       // NA_PNG_CHUNK_TYPE_iCCP
  {'s', 'B', 'I', 'T'},       // NA_PNG_CHUNK_TYPE_sBIT
  {'s', 'R', 'G', 'B'},       // NA_PNG_CHUNK_TYPE_sRGB
  {'b', 'K', 'G', 'D'},       // NA_PNG_CHUNK_TYPE_bKGD
  {'h', 'I', 'S', 'T'},       // NA_PNG_CHUNK_TYPE_hIST
  {'t', 'R', 'N', 'S'},       // NA_PNG_CHUNK_TYPE_tRNS
  {'p', 'H', 'Y', 's'},       // NA_PNG_CHUNK_TYPE_pHYs
  {'s', 'P', 'L', 'T'},       // NA_PNG_CHUNK_TYPE_sPLT
  {'t', 'I', 'M', 'E'},       // NA_PNG_CHUNK_TYPE_tIME
  {'i', 'T', 'X', 't'},       // NA_PNG_CHUNK_TYPE_iTXt
  {'t', 'E', 'X', 't'},       // NA_PNG_CHUNK_TYPE_tEXt
  {'z', 'T', 'X', 't'},       // NA_PNG_CHUNK_TYPE_zTXt

  {'x', 'x', 'X', 'x'},       // NA_PNG_CHUNK_TYPE_UNKNOWN
};


uint8 na_PngMagic[8] = {137, 80, 78, 71, 13, 10, 26, 10};


NA_PROTOTYPE(NAPNGChunk);
struct NAPNGChunk{
  NABuffer* data;      // All data without the preceding chunk type
  NAChunkType type;   // The type as an enum.
  uint32 length;
  uint8 typeName[4];  // The type as 4 uint8
  uint32 crc;
};


NA_HDEF void na_SetPNGsRGBColorimetry(NAPNG* png) {
  // This corresponds to the sRGB space.
  png->gamma = 45455.f / 100000.f;
  naFillV2f(png->whitePoint,    31270.f / 100000.f, 32900.f / 100000.f);
  naFillV2f(png->redPrimary,    64000.f / 100000.f, 33000.f / 100000.f);
  naFillV2f(png->greenPrimary,  30000.f / 100000.f, 60000.f / 100000.f);
  naFillV2f(png->bluePrimary,   15000.f / 100000.f,  6000.f / 100000.f);
}


NA_HDEF NAPNGChunk* na_AllocPNGChunkWithBuffer(NABufferIterator* iter) {
  NAChecksum checksum;
  uint32 crc;

  NAPNGChunk* chunk = naAlloc(NAPNGChunk);

  chunk->length = naReadBufferu32(iter);
  #if NA_DEBUG
    if(chunk->length > (1U<<31)-1U)
      naError("length should not exceed 2^31-1.");
  #endif

  naReadBufferBytes(iter, chunk->typeName, 4);
  if(chunk->length) {
    chunk->data = naReadBufferBuffer(iter, naCastu32Toi64(chunk->length));
  }else{
    chunk->data = naCreateBuffer(NA_FALSE);
  }

  chunk->type = NA_PNG_CHUNK_TYPE_UNKNOWN;
  for(size_t i = 0; i < NA_PNG_CHUNK_TYPE_COUNT; ++i) {
    if(naEqual32(na_PngChunkTypeNames[i], chunk->typeName)) {
      chunk->type = (NAChunkType)i;
      break;
    }
  }
  #if NA_DEBUG
//    if(chunk->type == NA_PNG_CHUNK_TYPE_UNKNOWN) {
//      printf("Undefined Chunkname %c%c%c%c" NA_NL, chunkname[0], chunkname[1], chunkname[2], chunkname[3]);
//    }
  #endif

  if(chunk->length) {
    // We cache the data to compute the checksum.
    naCacheBufferRange(chunk->data, naGetBufferRange(chunk->data));
  }

  naInitChecksum(&checksum, NA_CHECKSUM_TYPE_CRC_PNG);
  naAccumulateChecksum(&checksum, chunk->typeName, 4);
  naAccumulateChecksumBuffer(&checksum, chunk->data);
  crc = naGetChecksumResult(&checksum);
  naClearChecksum(&checksum);

  chunk->crc = naReadBufferu32(iter);

  #if NA_DEBUG
    if(chunk->crc != crc)
      naError("Checksum validation failed.");
  #else
    NA_UNUSED(crc);
  #endif

  return chunk;
}



NA_HDEF void na_DeallocPNGChunk(NAPNGChunk* chunk) {
  naRelease(chunk->data);
  naFree(chunk);
}




NA_API size_t naGetPNGBytesPerPixel(NAPNGColorType colorType) {
  size_t retValue;
  switch(colorType) {
  case NA_PNG_COLORTYPE_GREYSCALE:
    retValue = 1;
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR:
    retValue = 3;
    break;
  case NA_PNG_COLORTYPE_INDEXEDCOLOR:
    retValue = 1;
    break;
  case NA_PNG_COLORTYPE_GREYSCALE_ALPHA:
    retValue = 2;
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR_ALPHA:
    retValue = 4;
    break;
  default:
    #if NA_DEBUG
      naError("Invalid colorType given");
    #endif
    retValue = 0;
    break;
  }
  return retValue;
}



NA_DEF NAByte naGetPaethPredictor(NAByte a, NAByte b, NAByte c) {
  NAByte retValue;
  int32 p = (int32)a + (int32)b - (int32)c;
  int32 pa = naAbsi32(p - (int32)a);
  int32 pb = naAbsi32(p - (int32)b);
  int32 pc = naAbsi32(p - (int32)c);
  if((pa <= pb) && (pa <= pc)) {
    retValue = a;
  }else if(pb <= pc) {
    retValue = b;
  }else{
    retValue = c;
  }
  return retValue;
}



NA_DEF void naReconstructFilterData(NAPNG* png) {
  NAByte* curByte;
  NAByte* upBuffer;
  NAByte* upBufPtr;
  NABufferIterator iterFilter;

  size_t bpp = naGetPNGBytesPerPixel(png->colorType);
  size_t bytesPerLine = (size_t)png->size.width * bpp;

  png->pixelData = naMalloc(sizeof(NAByte) * naGetSizesIndexCount(png->size) * bpp);
  curByte = png->pixelData;

  upBuffer = naMalloc(bytesPerLine);
  naZeron(upBuffer, bytesPerLine);
  upBufPtr = upBuffer;

  iterFilter = naMakeBufferMutator(png->filteredData);

  for(size_t y = 0; y < (size_t)png->size.height; y++) {

    NAByte filtertype = naReadBufferu8(&iterFilter);
    naReadBufferu8v(&iterFilter, curByte, bytesPerLine);

    switch(filtertype) {
    case NA_PNG_FILTER_TYPE_NONE:
      // nothing to do.
      curByte += bytesPerLine;
      break;
    case NA_PNG_FILTER_TYPE_SUB:
      for(size_t x = 0; x < bpp; x++) {
        curByte++;  // The first byte adds value 0 from the virtual left byte.
      }
      for(size_t x = bpp; x < ((size_t)png->size.width * bpp); x++) {
        *curByte += *(curByte - bpp);
        curByte++;
      }
      break;
    case NA_PNG_FILTER_TYPE_UP:
      for(size_t x = 0; x < ((size_t)png->size.width * bpp); x++) {
        *curByte += *upBufPtr;
        curByte++;
        upBufPtr++;
      }
      break;
    case NA_PNG_FILTER_TYPE_AVERAGE:
      for(size_t x = 0; x < bpp; x++) {
        *curByte += (NAByte)(((size_t)*upBufPtr) / 2);
        curByte++;
        upBufPtr++;
      }
      for(size_t x = bpp; x < ((size_t)png->size.width * bpp); x++) {
        *curByte += (NAByte)(((size_t)*(curByte - bpp) + (size_t)*upBufPtr) / 2);
        curByte++;
        upBufPtr++;
      }
      break;
    case NA_PNG_FILTER_TYPE_PAETH:
      for(size_t x = 0; x < bpp; x++) {
        *curByte += naGetPaethPredictor(0, *upBufPtr, 0);
        curByte++;
        upBufPtr++;
      }
      for(size_t x = bpp; x < ((size_t)png->size.width * bpp); x++) {
        *curByte += naGetPaethPredictor(*(curByte - bpp), *upBufPtr, *(upBufPtr - bpp));
        curByte++;
        upBufPtr++;
      }
      break;
    default:
      #if NA_DEBUG
        naError("Invalid Filter");
      #endif
      curByte += bytesPerLine;
      break;
    }

    upBufPtr = curByte - bytesPerLine;
  }

//  outFile = naCreateFileWritingUrl("test.raw", NA_FILEMODE_DEFAULT);
//  naWriteFileBytes(outFile, png->pixelData, png->size.width * png->size.height * bpp);
//  naRelease(outFile);

  naClearBufferIterator(&iterFilter);
  naFree(upBuffer);
}




NA_DEF void naFilterData(NAPNG* png) {
  NAByte* pixelData;
  NABufferIterator iter;

  png->filteredData = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(png->filteredData, NA_ENDIANNESS_NETWORK);

  size_t bpp = naGetPNGBytesPerPixel(png->colorType);
  pixelData = naGetPNGPixelData(png);

  iter = naMakeBufferModifier(png->filteredData);
  for(size_t y = 0; y < png->size.height; y++) {
    naWriteBufferu8(&iter, NA_PNG_FILTER_TYPE_NONE);
    naWriteBufferBytes(&iter, pixelData, (size_t)png->size.width * bpp);
    pixelData += (size_t)png->size.width * bpp;
  }
  naClearBufferIterator(&iter);

  naFixBufferRange(png->filteredData);
}





NA_HDEF void na_ReadPNGIHDRChunk(NAPNG* png, NAPNGChunk* ihdr) {

  NABufferIterator iter;

  #if NA_DEBUG
    if(png->flags & NA_PNG_FLAGS_IHDR_AVAILABLE)
      naError("IHDR chunk already read.");
  #endif

  iter = naMakeBufferModifier(ihdr->data);

  png->size.width = (size_t)naReadBufferu32(&iter);
  png->size.height = (size_t)naReadBufferu32(&iter);
  png->bitDepth = naReadBufferi8(&iter);
  png->colorType = (NAPNGColorType)naReadBufferi8(&iter);
  png->compressionMethod = naReadBufferi8(&iter);
  #if NA_DEBUG
    if(png->compressionMethod != 0)
      naError("Invalid compression method in IHDR chunk");
  #endif
  png->filterMethod = naReadBufferi8(&iter);
  #if NA_DEBUG
    if(png->filterMethod != 0)
      naError("Invalid filter method in IHDR chunk");
  #endif
  png->interlaceMethod = (NAPNGInterlaceMethod)naReadBufferi8(&iter);
  #if NA_DEBUG
    if(png->interlaceMethod == 1)
      naError("This version of NALib can not read interlaced PNG files. Has not been implemented yet. Sorry.");
  #endif

  naClearBufferIterator(&iter);

  png->significantBits[0] = (uint8)png->bitDepth;
  png->significantBits[1] = (uint8)png->bitDepth;
  png->significantBits[2] = (uint8)png->bitDepth;
  png->significantBits[3] = (uint8)png->bitDepth;

  png->flags &= NA_PNG_FLAGS_IHDR_AVAILABLE;

}



NA_HDEF NAPNGChunk* na_AllocPNGIHDRChunk(NAPNG* png) {
  NABufferIterator iter;

  NAPNGChunk* ihdr = naAlloc(NAPNGChunk);
  ihdr->data = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(ihdr->data, NA_ENDIANNESS_NETWORK);
  ihdr->type = NA_PNG_CHUNK_TYPE_IHDR;

  iter = naMakeBufferModifier(ihdr->data);

  naWriteBufferu32(&iter, (uint32)png->size.width);
  naWriteBufferu32(&iter, (uint32)png->size.height);
  naWriteBufferi8(&iter, (int8)png->bitDepth);
  naWriteBufferi8(&iter, (int8)png->colorType);
  naWriteBufferi8(&iter, (int8)png->compressionMethod);
  naWriteBufferi8(&iter, (int8)png->filterMethod);
  naWriteBufferi8(&iter, (int8)png->interlaceMethod);

  naClearBufferIterator(&iter);

  return ihdr;
}



NA_HDEF void na_ReadPNGPLTEChunk(NAPNG* png, NAPNGChunk* plte) {
  // todo
//  printf("plte");
  NA_UNUSED(png);
  NA_UNUSED(plte);
}



NA_HDEF void na_ReadPNGIDATChunk(NAPNG* png, NAPNGChunk* idat) {
  naAppendBufferToBuffer(png->compressedData, idat->data);
}



NA_HDEF NAPNGChunk* na_AllocPNGIDATChunk(NAPNG* png) {
  NAPNGChunk* idat = naAlloc(NAPNGChunk);
  idat->data = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(idat->data, NA_ENDIANNESS_NETWORK);

  naFilterData(png);
  naFillBufferWithZLIBCompression(idat->data, png->filteredData, NA_DEFLATE_COMPRESSION_FASTEST);

  idat->type = NA_PNG_CHUNK_TYPE_IDAT;

  return idat;
}



NA_HDEF void na_ReadPNGIENDChunk(NAPNG* png, NAPNGChunk* iend) {
  NA_UNUSED(png);
  NA_UNUSED(iend);
  // nothing to do. This chunk is empty and marks the end of the png stream.
}



NA_HDEF NAPNGChunk* na_AllocPNGIENDChunk(NAPNG* png) {
  NAPNGChunk* iend;
  NA_UNUSED(png);

  iend = naAlloc(NAPNGChunk);
  iend->data = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(iend->data, NA_ENDIANNESS_NETWORK);
  iend->type = NA_PNG_CHUNK_TYPE_IEND;

  return iend;
}



NA_HDEF void na_ReadPNGcHRMChunk(NAPNG* png, NAPNGChunk* chrm) {
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE) {
    // Ignoring when sRGB is available
  }else{
    NABufferIterator iter = naMakeBufferAccessor(chrm->data);

    png->whitePoint[0]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->whitePoint[1]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->redPrimary[0]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->redPrimary[1]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->greenPrimary[0]  = (float)naReadBufferu32(&iter) / 100000.f;
    png->greenPrimary[1]  = (float)naReadBufferu32(&iter) / 100000.f;
    png->bluePrimary[0]   = (float)naReadBufferu32(&iter) / 100000.f;
    png->bluePrimary[1]   = (float)naReadBufferu32(&iter) / 100000.f;

    naClearBufferIterator(&iter);
  }
}



NA_HDEF void na_ReadPNGgAMAChunk(NAPNG* png, NAPNGChunk* gama) {
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE) {
    // Ignoring when sRGB is available
  }else{
    NABufferIterator iter = naMakeBufferAccessor(gama->data);
    png->gamma = (float)naReadBufferu32(&iter) / 100000.f;
    naClearBufferIterator(&iter);
  }
}



NA_HDEF void na_ReadPNGiCCPChunk(NAPNG* png, NAPNGChunk* iccp) {
  // todo
//  printf("iccp");
  NA_UNUSED(png);
  NA_UNUSED(iccp);
}



NA_HDEF void na_ReadPNGsBITChunk(NAPNG* png, NAPNGChunk* sbit) {
  NABufferIterator iter = naMakeBufferAccessor(sbit->data);
  switch(png->colorType) {
  case NA_PNG_COLORTYPE_GREYSCALE:
    png->significantBits[1] = naReadBufferu8(&iter);
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR:
  case NA_PNG_COLORTYPE_INDEXEDCOLOR:
    png->significantBits[0] = naReadBufferu8(&iter);
    png->significantBits[1] = naReadBufferu8(&iter);
    png->significantBits[2] = naReadBufferu8(&iter);
    break;
  case NA_PNG_COLORTYPE_GREYSCALE_ALPHA:
    png->significantBits[1] = naReadBufferu8(&iter);
    png->significantBits[3] = naReadBufferu8(&iter);
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR_ALPHA:
    png->significantBits[0] = naReadBufferu8(&iter);
    png->significantBits[1] = naReadBufferu8(&iter);
    png->significantBits[2] = naReadBufferu8(&iter);
    png->significantBits[3] = naReadBufferu8(&iter);
    break;
  }
  naClearBufferIterator(&iter);
}



NA_HDEF void na_ReadPNGsRGBChunk(NAPNG* png, NAPNGChunk* srgb) {
  NABufferIterator iter = naMakeBufferAccessor(srgb->data);
  uint8 intent = naReadBufferu8(&iter);
  naClearBufferIterator(&iter);

  // As this implementation is not yet capable of color management, we ignore
  // the gAMA and cHRM values and set them to the following:
  NA_UNUSED(intent);
  na_SetPNGsRGBColorimetry(png);
  png->flags |= NA_PNG_FLAGS_sRGB_AVAILABLE;
}



NA_HDEF void na_ReadPNGbKGDChunk(NAPNG* png, NAPNGChunk* bkgd) {
  // todo
//  printf("bkgd");
  NA_UNUSED(png);
  NA_UNUSED(bkgd);
}



NA_HDEF void na_ReadPNGhISTChunk(NAPNG* png, NAPNGChunk* hist) {
  // todo
//  printf("hist");
  NA_UNUSED(png);
  NA_UNUSED(hist);
}



NA_HDEF void na_ReadPNGtRNSChunk(NAPNG* png, NAPNGChunk* trns) {
  // todo
//  printf("trns");
  NA_UNUSED(png);
  NA_UNUSED(trns);
}



NA_HDEF void na_ReadPNGpHYsChunk(NAPNG* png, NAPNGChunk* phys) {
  uint8 unit;
  NABufferIterator iter = naMakeBufferAccessor(phys->data);

  png->pixelDimensions[0] = (float)naReadBufferu32(&iter);
  png->pixelDimensions[1] = (float)naReadBufferu32(&iter);
  unit = naReadBufferu8(&iter);

  naClearBufferIterator(&iter);

  switch(unit) {
  case 0:
    png->pixelUnit = NA_PIXEL_UNIT_RATIO;
    break;
  case 1:
    png->pixelUnit = NA_PIXEL_UNIT_PER_METER;
    break;
  default:
    #if NA_DEBUG
      naError("Undefined pixel unit");
    #endif
    break;
  }
}



NA_HDEF void na_ReadPNGsPLTChunk(NAPNG* png, NAPNGChunk* splt) {
  // todo
//  printf("splt");
  NA_UNUSED(png);
  NA_UNUSED(splt);
}



NA_HDEF void na_ReadPNGtIMEChunk(NAPNG* png, NAPNGChunk* time) {
  png->modificationDate = naMakeDateTimeWithBuffer(time->data, NA_DATETIME_FORMAT_PNG);
}



NA_HDEF void na_ReadPNGiTXtChunk(NAPNG* png, NAPNGChunk* itxt) {
  // todo
//  printf("itxt");
  NA_UNUSED(png);
  NA_UNUSED(itxt);
}



NA_HDEF void na_ReadPNGtEXtChunk(NAPNG* png, NAPNGChunk* text) {
  // todo
//  printf("text");
  NA_UNUSED(png);
  NA_UNUSED(text);
}



NA_HDEF void na_ReadPNGzTXtChunk(NAPNG* png, NAPNGChunk* ztxt) {
  // todo
//  printf("ztxt");
  NA_UNUSED(png);
  NA_UNUSED(ztxt);
}











NA_DEF NAPNG* naNewPNG(NASizes size, NAPNGColorType colorType, int8 bitDepth) {
  NAPNG* png = naNew(NAPNG);

  #if NA_DEBUG
    if(bitDepth != 8)
      naError("Sorry, bitDepth must be 8 for now. Everything else will be implemented later.");
    if((colorType != NA_PNG_COLORTYPE_TRUECOLOR) && (colorType != NA_PNG_COLORTYPE_TRUECOLOR_ALPHA))
      naError("Sorry, colorType must be truecolor with or without alpha. Everything else will be implemented later.");
  #endif
  naInitList(&png->chunks);
  png->flags = 0;
  png->bitDepth = (int8)bitDepth;
  png->compressionMethod = 0;
  png->interlaceMethod = NA_PNG_INTERLACE_NONE;
  png->filterMethod = 0;
  na_SetPNGsRGBColorimetry(png);
  png->pixelDimensions[0] = 1.f;
  png->pixelDimensions[1] = 1.f;
  png->pixelUnit = NA_PIXEL_UNIT_RATIO;
  png->size = size;
  png->colorType = colorType;

  size_t bpp = naGetPNGBytesPerPixel(colorType);
  png->pixelData = naMalloc(naGetSizesIndexCount(size) * bpp);
  png->filteredData = NA_NULL;

  return png;
}



NA_DEF NAPNG* naNewPNGWithUrhl(const char* fileUrl) {
  NABuffer* buffer;
  NAByte magic[8];
  NABufferIterator bufIter;

  NAPNG* png = naNew(NAPNG);
  naInitList(&png->chunks);

  // Set the default values. Needed if no appropriate chunk is available.
  png->flags = 0;
  na_SetPNGsRGBColorimetry(png);
  png->pixelDimensions[0] = 1.f;
  png->pixelDimensions[1] = 1.f;
  png->pixelUnit = NA_PIXEL_UNIT_RATIO;
  png->filteredData = NA_NULL;

  buffer = naCreateBufferWithInputUrl(fileUrl);

  // If the buffer is empty, there is no png to read.
  if(!naIsBufferEmpty(buffer)) {
    bufIter = naMakeBufferModifier(buffer);

    // Important! RFC 1950 is big endianed (network endianness)
    naSetBufferEndianness(buffer, NA_ENDIANNESS_NETWORK);

    // Read the magic numbers. If they do not match, this is no png file.
    naReadBufferBytes(&bufIter, magic, 8);
    if(!naEqual64(magic, na_PngMagic)) {
      #if NA_DEBUG
        naError("File is not a PNG file.");
      #endif

    }else{

      // Read the chunks until the IEND chunk is read.
      while(1) {
        NAPNGChunk* chunk = na_AllocPNGChunkWithBuffer(&bufIter);
        naAddListLastMutable(&png->chunks, chunk);
        
        if(chunk->type == NA_PNG_CHUNK_TYPE_IEND)
          break;
      }
      naClearBufferIterator(&bufIter);

      // Create the buffer to hold the compressed and decompressed data
      png->compressedData = naCreateBuffer(NA_FALSE);
      naSetBufferEndianness(png->compressedData, NA_ENDIANNESS_NETWORK);
      png->filteredData = naCreateBuffer(NA_FALSE);

      NAListIterator iter = naMakeListMutator(&png->chunks);
      while(naIterateList(&iter)) {
        NAPNGChunk* chunk = naGetListCurMutable(&iter);

        switch(chunk->type) {
        case NA_PNG_CHUNK_TYPE_IHDR:  na_ReadPNGIHDRChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_PLTE:  na_ReadPNGPLTEChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_IDAT:  na_ReadPNGIDATChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_IEND:  na_ReadPNGIENDChunk(png, chunk);  break;

        case NA_PNG_CHUNK_TYPE_cHRM:  na_ReadPNGcHRMChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_gAMA:  na_ReadPNGgAMAChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_iCCP:  na_ReadPNGiCCPChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_sBIT:  na_ReadPNGsBITChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_sRGB:  na_ReadPNGsRGBChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_bKGD:  na_ReadPNGbKGDChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_hIST:  na_ReadPNGhISTChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_tRNS:  na_ReadPNGtRNSChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_pHYs:  na_ReadPNGpHYsChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_sPLT:  na_ReadPNGsPLTChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_tIME:  na_ReadPNGtIMEChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_iTXt:  na_ReadPNGiTXtChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_tEXt:  na_ReadPNGtEXtChunk(png, chunk);  break;
        case NA_PNG_CHUNK_TYPE_zTXt:  na_ReadPNGzTXtChunk(png, chunk);  break;
        default:
          break;
        }
      }
      naClearListIterator(&iter);

    //  NAFile* outFile = naCreateFileWritingUrl("test.raw", NA_FILEMODE_DEFAULT);
    //  naWriteBufferToFile(png->compressedData, outFile);
    //  naRelease(outFile);

      naFixBufferRange(png->compressedData);
      naFillBufferWithZLIBDecompression(png->filteredData, png->compressedData);
      naReconstructFilterData(png);
    }
  }
  
  naRelease(buffer);

  return png;
}



NA_API NAPNG* naNewPNGWithImage(NAImage* image) {
  NAPNG* png = naNewPNG(naGetImageSize(image), NA_PNG_COLORTYPE_TRUECOLOR_ALPHA, 8);
  naConvertImageTou8(image, png->pixelData, NA_TRUE, NA_COLOR_BUFFER_RGBA);
  return png;
}



NA_DEF void* naGetPNGPixelData(NAPNG* png) {
  return png->pixelData;
}



NA_DEF size_t naGetPNGPixelDataByteSize(NAPNG* png) {
  size_t bpp = naGetPNGBytesPerPixel(png->colorType);
  return naGetSizesIndexCount(png->size) * bpp;
}



NA_DEF NAImage* naCreateImageWithPNG(NAPNG* png) {
  NAImage* image = naCreateImage(png->size, NA_NULL);
  NAByte* pngPtr;
  NAColor* colorPtr;
  uint8 inBuf[4];

  switch(png->colorType) {
  case NA_PNG_COLORTYPE_TRUECOLOR:
    pngPtr = png->pixelData;
    inBuf[3] = 255;
    for(size_t y = 0; y < png->size.height; y++) {
      colorPtr = &(naGetImageData(image)[(png->size.height - y - 1) * naGetImageSize(image).width]);
      for(size_t x = 0; x < png->size.width; x++) {
        inBuf[0] = pngPtr[0];
        inBuf[1] = pngPtr[1];
        inBuf[2] = pngPtr[2];
        naFillColorWithSRGBu8v(colorPtr, inBuf, NA_COLOR_BUFFER_RGB);
        colorPtr += 1;
        pngPtr += 3;
      }
    }
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR_ALPHA:
    naFillImageWithu8(image, png->pixelData, NA_TRUE, NA_COLOR_BUFFER_RGBA);
    break;
  default:
    #if NA_DEBUG
      naError("Not implemented yet");
    #endif
    break;
  }
  return image;
}



NA_DEF NASizes naGetPNGSize(NAPNG* png) {
  return png->size;
}



NA_DEF NAPNGColorType naGetPNGColorType(NAPNG* png) {
  return png->colorType;
}



NA_DEF int8 naGetPNGBitDepth(NAPNG* png) {
  return png->bitDepth;
}



NA_DEF void naWritePNGToUrl(NAPNG* png, const char* fileUrl) {

  NABuffer* outbuffer;
  NAChecksum checksum;
  NAFile* outFile;
  NABufferIterator iterOut;

  naAddListLastMutable(&png->chunks, na_AllocPNGIHDRChunk(png));
  naAddListLastMutable(&png->chunks, na_AllocPNGIDATChunk(png));
  naAddListLastMutable(&png->chunks, na_AllocPNGIENDChunk(png));

  outbuffer = naCreateBuffer(NA_FALSE);
  iterOut = naMakeBufferMutator(outbuffer);
  naSetBufferEndianness(outbuffer, NA_ENDIANNESS_NETWORK);

  naWriteBufferBytes(&iterOut, na_PngMagic, 8);

  NAListIterator iter = naMakeListMutator(&png->chunks);
  while(naIterateList(&iter)) {
    NAPNGChunk* chunk = naGetListCurMutable(&iter);

    naFixBufferRange(chunk->data);

    chunk->length = naCasti64Tou32(naGetBufferRange(chunk->data).length);
    naWriteBufferu32(&iterOut, chunk->length);

    naCopy32(chunk->typeName, na_PngChunkTypeNames[chunk->type]);
    naWriteBufferBytes(&iterOut, chunk->typeName, 4);

    if(!naIsBufferEmpty(chunk->data)) {
      naWriteBufferBuffer(&iterOut, chunk->data, naGetBufferRange(chunk->data));
    }

    naInitChecksum(&checksum, NA_CHECKSUM_TYPE_CRC_PNG);
    naAccumulateChecksum(&checksum, chunk->typeName, 4);
    if(chunk->length) {
      naAccumulateChecksumBuffer(&checksum, chunk->data);
    }
    chunk->crc = naGetChecksumResult(&checksum);
    naClearChecksum(&checksum);
    naWriteBufferu32(&iterOut, chunk->crc);
  }
  naClearListIterator(&iter);

  naClearBufferIterator(&iterOut);

  outFile = naCreateFileWritingUrl(fileUrl, NA_FILEMODE_DEFAULT);
  naFixBufferRange(outbuffer);
  naWriteBufferToFile(outbuffer, outFile);
  naRelease(outFile);
  naRelease(outbuffer);
}



// This is the destructor for a PNG. It is marked as a helper as it should
// only be called by the runtime system
NA_HDEF void na_DestructPNG(NAPNG* png) {
  naClearList(&png->chunks, (NAMutator)na_DeallocPNGChunk);
  
  if(png->pixelData)
    naFree(png->pixelData);
  if(png->compressedData)
    naRelease(png->compressedData);
  if(png->filteredData)
    naRelease(png->filteredData);
}



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
