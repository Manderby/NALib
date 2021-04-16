
#include "../NAPNG.h"
#include "../NABuffer.h"
#include "../NADeflate.h"
#include "../NAFile.h"
#include "../NABabyImage.h"

// Reference: http://www.w3.org/TR/PNG

#define NA_PNG_FLAGS_IHDR_AVAILABLE       0x01
#define NA_PNG_FLAGS_sRGB_AVAILABLE       0x02


#include "../NAList.h"
#include "../NAVectorAlgebra.h"

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
  NASizei size;
  NAList chunks;
  uint32 flags;
  int8 bitDepth;
  NAPNGColorType colorType;
  int8 compressionmethod;
  int8 filtermethod;
  float gamma;
  NAVec2f whitepoint;
  NAVec2f redprimary;
  NAVec2f greenprimary;
  NAVec2f blueprimary;
  uint8 significantbits[4];
  NAVec2f pixeldimensions;
  NAPixelUnit pixelunit;
  NADateTime modificationdate;
  NAPNGInterlaceMethod interlacemethod;

  NAByte* pixeldata;  // stored from top to bottom
  NABuffer* compresseddata;
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


typedef struct NAPNGChunk NAPNGChunk;
struct NAPNGChunk{
  NABuffer* data;      // All data without the preceding chunk type
  NAChunkType type;   // The type as an enum.
  uint32 length;
  uint8 typeName[4];  // The type as 4 uint8
  uint32 crc;
};


NA_HDEF void na_SetPNGsRGBColorimetry(NAPNG* png){
  // This corresponds to the sRGB space.
  png->gamma = 45455.f / 100000.f;
  naFillV2f(png->whitepoint,    31270.f / 100000.f, 32900.f / 100000.f);
  naFillV2f(png->redprimary,    64000.f / 100000.f, 33000.f / 100000.f);
  naFillV2f(png->greenprimary,  30000.f / 100000.f, 60000.f / 100000.f);
  naFillV2f(png->blueprimary,   15000.f / 100000.f,  6000.f / 100000.f);
}


NA_HDEF NAPNGChunk* na_AllocPNGChunkFromBuffer(NABufferIterator* iter){
  NAInt i;
  NAChecksum checksum;
  uint32 crc;

  NAPNGChunk* chunk = naAlloc(NAPNGChunk);

  chunk->length = naReadBufferu32(iter);
  #if NA_DEBUG
    if(chunk->length > (1U<<31)-1U)
      naError("length should not exceed 2^31-1.");
  #endif

  naReadBufferBytes(iter, chunk->typeName, 4);
  if(chunk->length){
    chunk->data = naReadBufferBuffer(iter, chunk->length);
  }else{
    chunk->data = naNewBuffer(NA_FALSE);
  }

  chunk->type = NA_PNG_CHUNK_TYPE_UNKNOWN;
  for(i = 0; i < NA_PNG_CHUNK_TYPE_COUNT; i++){
    if(naEqual32(na_PngChunkTypeNames[i], chunk->typeName)){
      chunk->type = (NAChunkType)i;
      break;
    }
  }
  #if NA_DEBUG
//    if(chunk->type == NA_PNG_CHUNK_TYPE_UNKNOWN){
//      printf("Undefined Chunkname %c%c%c%c" NA_NL, chunkname[0], chunkname[1], chunkname[2], chunkname[3]);
//    }
  #endif

  if(chunk->length){
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



NA_HDEF void na_DeallocPNGChunk(NAPNGChunk* chunk){
  naRelease(chunk->data);
  naFree(chunk);
}




NA_API size_t naGetPNGBytesPerPixel(NAPNGColorType colorType){
  size_t retValue;
  switch(colorType){
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



NA_DEF NAByte naGetPaethPredictor(NAByte a, NAByte b, NAByte c){
  NAByte retValue;
  NAInt p = (NAInt)a + (NAInt)b - (NAInt)c;
  NAInt pa = naAbsi(p - (NAInt)a);
  NAInt pb = naAbsi(p - (NAInt)b);
  NAInt pc = naAbsi(p - (NAInt)c);
  if((pa <= pb) && (pa <= pc)){retValue = a;}
  else if(pb <= pc){retValue = b;}
  else{retValue = c;}
  return retValue;
}



NA_DEF void naReconstructFilterData(NAPNG* png){
  NAByte* curByte;
  NAByte* upBuffer;
  NAByte* upBufPtr;
  NABufferIterator iterFilter;

  size_t bpp = naGetPNGBytesPerPixel(png->colorType);
  size_t bytesPerLine = (size_t)png->size.width * bpp;

  png->pixeldata = naMalloc(sizeof(NAByte) * naGetSizeiIndexCount(png->size) * bpp);
  curByte = png->pixeldata;

  upBuffer = naMalloc(bytesPerLine);
  naZeron(upBuffer, bytesPerLine);
  upBufPtr = upBuffer;

  iterFilter = naMakeBufferMutator(png->filteredData);

  for(size_t y = 0; y < (size_t)png->size.height; y++){

    NAByte filtertype = naReadBufferu8(&iterFilter);
    naReadBufferu8v(&iterFilter, curByte, bytesPerLine);

    switch(filtertype){
    case NA_PNG_FILTER_TYPE_NONE:
      // nothing to do.
      curByte += bytesPerLine;
      break;
    case NA_PNG_FILTER_TYPE_SUB:
      for(size_t x = 0; x < bpp; x++){
        curByte++;  // The first byte adds value 0 from the virtual left byte.
      }
      for(size_t x = bpp; x < ((size_t)png->size.width * bpp); x++){
        *curByte += *(curByte - bpp);
        curByte++;
      }
      break;
    case NA_PNG_FILTER_TYPE_UP:
      for(size_t x = 0; x < ((size_t)png->size.width * bpp); x++){
        *curByte += *upBufPtr;
        curByte++;
        upBufPtr++;
      }
      break;
    case NA_PNG_FILTER_TYPE_AVERAGE:
      for(size_t x = 0; x < bpp; x++){
        *curByte += (NAByte)(((size_t)*upBufPtr) / 2);
        curByte++;
        upBufPtr++;
      }
      for(size_t x = bpp; x < ((size_t)png->size.width * bpp); x++){
        *curByte += (NAByte)(((size_t)*(curByte - bpp) + (size_t)*upBufPtr) / 2);
        curByte++;
        upBufPtr++;
      }
      break;
    case NA_PNG_FILTER_TYPE_PAETH:
      for(size_t x = 0; x < bpp; x++){
        *curByte += naGetPaethPredictor(0, *upBufPtr, 0);
        curByte++;
        upBufPtr++;
      }
      for(size_t x = bpp; x < ((size_t)png->size.width * bpp); x++){
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

//  outfile = naCreateFileWritingPath("test.raw", NA_FILEMODE_DEFAULT);
//  naWriteFileBytes(outfile, png->pixeldata, png->size.width * png->size.height * bpp);
//  naReleaseFile(outfile);

  naClearBufferIterator(&iterFilter);
  naFree(upBuffer);
}




NA_DEF void naFilterData(NAPNG* png){
  NAByte* pixeldata;
  NAInt y;
  NABufferIterator iter;

  png->filteredData = naNewBuffer(NA_FALSE);
  naSetBufferEndianness(png->filteredData, NA_ENDIANNESS_NETWORK);

  size_t bpp = naGetPNGBytesPerPixel(png->colorType);
  pixeldata = naGetPNGPixelData(png);

  iter = naMakeBufferModifier(png->filteredData);
  for(y = 0; y < png->size.height; y++){
    naWriteBufferu8(&iter, NA_PNG_FILTER_TYPE_NONE);
    naWriteBufferBytes(&iter, pixeldata, (size_t)png->size.width * bpp);
    pixeldata += (size_t)png->size.width * bpp;
  }
  naClearBufferIterator(&iter);

  naFixBufferRange(png->filteredData);
}





NA_HDEF void na_ReadPNGIHDRChunk(NAPNG* png, NAPNGChunk* ihdr){

  NABufferIterator iter;

  #if NA_DEBUG
    if(png->flags & NA_PNG_FLAGS_IHDR_AVAILABLE)
      naError("IHDR chunk already read.");
  #endif

  iter = naMakeBufferModifier(ihdr->data);

  png->size.width = (NAInt)naReadBufferu32(&iter);
  png->size.height = (NAInt)naReadBufferu32(&iter);
  png->bitDepth = naReadBufferi8(&iter);
  png->colorType = (NAPNGColorType)naReadBufferi8(&iter);
  png->compressionmethod = naReadBufferi8(&iter);
  #if NA_DEBUG
    if(png->compressionmethod != 0)
      naError("Invalid compression method in IHDR chunk");
  #endif
  png->filtermethod = naReadBufferi8(&iter);
  #if NA_DEBUG
    if(png->filtermethod != 0)
      naError("Invalid filter method in IHDR chunk");
  #endif
  png->interlacemethod = (NAPNGInterlaceMethod)naReadBufferi8(&iter);
  #if NA_DEBUG
    if(png->interlacemethod == 1)
      naError("This version of NALib can not read interlaced PNG files. Has not been implemented yet. Sorry.");
  #endif

  naClearBufferIterator(&iter);

  png->significantbits[0] = (uint8)png->bitDepth;
  png->significantbits[1] = (uint8)png->bitDepth;
  png->significantbits[2] = (uint8)png->bitDepth;
  png->significantbits[3] = (uint8)png->bitDepth;

  png->flags &= NA_PNG_FLAGS_IHDR_AVAILABLE;

}



NA_HDEF NAPNGChunk* na_AllocPNGIHDRChunk(NAPNG* png){
  NABufferIterator iter;

  NAPNGChunk* ihdr = naAlloc(NAPNGChunk);
  ihdr->data = naNewBuffer(NA_FALSE);
  naSetBufferEndianness(ihdr->data, NA_ENDIANNESS_NETWORK);
  ihdr->type = NA_PNG_CHUNK_TYPE_IHDR;

  iter = naMakeBufferModifier(ihdr->data);

  naWriteBufferu32(&iter, (uint32)png->size.width);
  naWriteBufferu32(&iter, (uint32)png->size.height);
  naWriteBufferi8(&iter, (int8)png->bitDepth);
  naWriteBufferi8(&iter, (int8)png->colorType);
  naWriteBufferi8(&iter, (int8)png->compressionmethod);
  naWriteBufferi8(&iter, (int8)png->filtermethod);
  naWriteBufferi8(&iter, (int8)png->interlacemethod);

  naClearBufferIterator(&iter);

  return ihdr;
}



NA_HDEF void na_ReadPNGPLTEChunk(NAPNG* png, NAPNGChunk* plte){
  // todo
//  printf("plte");
  NA_UNUSED(png);
  NA_UNUSED(plte);
}



NA_HDEF void na_ReadPNGIDATChunk(NAPNG* png, NAPNGChunk* idat){
  naAppendBufferToBuffer(png->compresseddata, idat->data);
}



NA_HDEF NAPNGChunk* na_AllocPNGIDATChunk(NAPNG* png){
  NAPNGChunk* idat = naAlloc(NAPNGChunk);
  idat->data = naNewBuffer(NA_FALSE);
  naSetBufferEndianness(idat->data, NA_ENDIANNESS_NETWORK);

  naFilterData(png);
  naFillBufferWithZLIBCompression(idat->data, png->filteredData, NA_DEFLATE_COMPRESSION_FASTEST);

  idat->type = NA_PNG_CHUNK_TYPE_IDAT;

  return idat;
}



NA_HDEF void na_ReadPNGIENDChunk(NAPNG* png, NAPNGChunk* iend){
  NA_UNUSED(png);
  NA_UNUSED(iend);
  // nothing to do. This chunk is empty and marks the end of the png stream.
}



NA_HDEF NAPNGChunk* na_AllocPNGIENDChunk(NAPNG* png){
  NAPNGChunk* iend;
  NA_UNUSED(png);

  iend = naAlloc(NAPNGChunk);
  iend->data = naNewBuffer(NA_FALSE);
  naSetBufferEndianness(iend->data, NA_ENDIANNESS_NETWORK);
  iend->type = NA_PNG_CHUNK_TYPE_IEND;

  return iend;
}



NA_HDEF void na_ReadPNGcHRMChunk(NAPNG* png, NAPNGChunk* chrm){
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE){
    // Ignoring when sRGB is available
  }else{
    NABufferIterator iter = naMakeBufferAccessor(chrm->data);

    png->whitepoint[0]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->whitepoint[1]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->redprimary[0]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->redprimary[1]    = (float)naReadBufferu32(&iter) / 100000.f;
    png->greenprimary[0]  = (float)naReadBufferu32(&iter) / 100000.f;
    png->greenprimary[1]  = (float)naReadBufferu32(&iter) / 100000.f;
    png->blueprimary[0]   = (float)naReadBufferu32(&iter) / 100000.f;
    png->blueprimary[1]   = (float)naReadBufferu32(&iter) / 100000.f;

    naClearBufferIterator(&iter);
  }
}



NA_HDEF void na_ReadPNGgAMAChunk(NAPNG* png, NAPNGChunk* gama){
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE){
    // Ignoring when sRGB is available
  }else{
    NABufferIterator iter = naMakeBufferAccessor(gama->data);
    png->gamma = (float)naReadBufferu32(&iter) / 100000.f;
    naClearBufferIterator(&iter);
  }
}



NA_HDEF void na_ReadPNGiCCPChunk(NAPNG* png, NAPNGChunk* iccp){
  // todo
//  printf("iccp");
  NA_UNUSED(png);
  NA_UNUSED(iccp);
}



NA_HDEF void na_ReadPNGsBITChunk(NAPNG* png, NAPNGChunk* sbit){
  NABufferIterator iter = naMakeBufferAccessor(sbit->data);
  switch(png->colorType){
  case NA_PNG_COLORTYPE_GREYSCALE:
    png->significantbits[1] = naReadBufferu8(&iter);
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR:
  case NA_PNG_COLORTYPE_INDEXEDCOLOR:
    png->significantbits[0] = naReadBufferu8(&iter);
    png->significantbits[1] = naReadBufferu8(&iter);
    png->significantbits[2] = naReadBufferu8(&iter);
    break;
  case NA_PNG_COLORTYPE_GREYSCALE_ALPHA:
    png->significantbits[1] = naReadBufferu8(&iter);
    png->significantbits[3] = naReadBufferu8(&iter);
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR_ALPHA:
    png->significantbits[0] = naReadBufferu8(&iter);
    png->significantbits[1] = naReadBufferu8(&iter);
    png->significantbits[2] = naReadBufferu8(&iter);
    png->significantbits[3] = naReadBufferu8(&iter);
    break;
  }
  naClearBufferIterator(&iter);
}



NA_HDEF void na_ReadPNGsRGBChunk(NAPNG* png, NAPNGChunk* srgb){
  NABufferIterator iter = naMakeBufferAccessor(srgb->data);
  uint8 intent = naReadBufferu8(&iter);
  naClearBufferIterator(&iter);

  // As this implementation is not yet capable of color management, we ignore
  // the gAMA and cHRM values and set them to the following:
  NA_UNUSED(intent);
  na_SetPNGsRGBColorimetry(png);
  png->flags |= NA_PNG_FLAGS_sRGB_AVAILABLE;
}



NA_HDEF void na_ReadPNGbKGDChunk(NAPNG* png, NAPNGChunk* bkgd){
  // todo
//  printf("bkgd");
  NA_UNUSED(png);
  NA_UNUSED(bkgd);
}



NA_HDEF void na_ReadPNGhISTChunk(NAPNG* png, NAPNGChunk* hist){
  // todo
//  printf("hist");
  NA_UNUSED(png);
  NA_UNUSED(hist);
}



NA_HDEF void na_ReadPNGtRNSChunk(NAPNG* png, NAPNGChunk* trns){
  // todo
//  printf("trns");
  NA_UNUSED(png);
  NA_UNUSED(trns);
}



NA_HDEF void na_ReadPNGpHYsChunk(NAPNG* png, NAPNGChunk* phys){
  uint8 unit;
  NABufferIterator iter = naMakeBufferAccessor(phys->data);

  png->pixeldimensions[0] = (float)naReadBufferu32(&iter);
  png->pixeldimensions[1] = (float)naReadBufferu32(&iter);
  unit = naReadBufferu8(&iter);

  naClearBufferIterator(&iter);

  switch(unit){
  case 0:
    png->pixelunit = NA_PIXEL_UNIT_RATIO;
    break;
  case 1:
    png->pixelunit = NA_PIXEL_UNIT_PER_METER;
    break;
  default:
    #if NA_DEBUG
      naError("Undefined pixel unit");
    #endif
    break;
  }
}



NA_HDEF void na_ReadPNGsPLTChunk(NAPNG* png, NAPNGChunk* splt){
  // todo
//  printf("splt");
  NA_UNUSED(png);
  NA_UNUSED(splt);
}



NA_HDEF void na_ReadPNGtIMEChunk(NAPNG* png, NAPNGChunk* time){
  png->modificationdate = naMakeDateTimeFromBuffer(time->data, NA_DATETIME_FORMAT_PNG);
}



NA_HDEF void na_ReadPNGiTXtChunk(NAPNG* png, NAPNGChunk* itxt){
  // todo
//  printf("itxt");
  NA_UNUSED(png);
  NA_UNUSED(itxt);
}



NA_HDEF void na_ReadPNGtEXtChunk(NAPNG* png, NAPNGChunk* text){
  // todo
//  printf("text");
  NA_UNUSED(png);
  NA_UNUSED(text);
}



NA_HDEF void na_ReadPNGzTXtChunk(NAPNG* png, NAPNGChunk* ztxt){
  // todo
//  printf("ztxt");
  NA_UNUSED(png);
  NA_UNUSED(ztxt);
}











NA_DEF NAPNG* naNewPNG(NASizei size, NAPNGColorType colorType, int8 bitDepth){
  NAPNG* png = naNew(NAPNG);

  #if NA_DEBUG
    if(bitDepth != 8)
      naError("Sorry, bitDepth must be 8 for now. Everything else will be implemented later.");
    if((colorType != NA_PNG_COLORTYPE_TRUECOLOR) && (colorType != NA_PNG_COLORTYPE_TRUECOLOR_ALPHA))
      naError("Sorry, colorType must be truecolor with or without alpha. Everything else will be implemented later.");
  #endif
  naInitList(&(png->chunks));
  png->flags = 0;
  png->bitDepth = (int8)bitDepth;
  png->compressionmethod = 0;
  png->interlacemethod = NA_PNG_INTERLACE_NONE;
  png->filtermethod = 0;
  na_SetPNGsRGBColorimetry(png);
  png->pixeldimensions[0] = 1.f;
  png->pixeldimensions[1] = 1.f;
  png->pixelunit = NA_PIXEL_UNIT_RATIO;
  png->size = size;
  png->colorType = colorType;

  size_t bpp = naGetPNGBytesPerPixel(colorType);
  png->pixeldata = naMalloc(naGetSizeiIndexCount(size) * bpp);
  png->filteredData = NA_NULL;

  return png;
}



NA_DEF NAPNG* naNewPNGWithPath(const char* filePath){
  NABuffer* buffer;
  NAByte magic[8];
  NAListIterator iter;
  NABufferIterator bufiter;

  NAPNG* png = naNew(NAPNG);
  naInitList(&(png->chunks));

  // Set the default values. Needed if no appropriate chunk is available.
  png->flags = 0;
  na_SetPNGsRGBColorimetry(png);
  png->pixeldimensions[0] = 1.f;
  png->pixeldimensions[1] = 1.f;
  png->pixelunit = NA_PIXEL_UNIT_RATIO;
  png->filteredData = NA_NULL;

  buffer = naNewBufferWithInputPath(filePath);
  bufiter = naMakeBufferModifier(buffer);

  // If the buffer is empty, there is no png to read.
  if(naIsBufferEmpty(buffer)){
    goto NAEndReadingPNG;
  }

  // Important! RFC 1950 is big endianed (network endianness)
  naSetBufferEndianness(buffer, NA_ENDIANNESS_NETWORK);

  // Read the magic numbers. If they do not match, this is no png file.
  naReadBufferBytes(&bufiter, magic, 8);
  if(!naEqual64(magic, na_PngMagic)){
    #if NA_DEBUG
      naError("File is not a PNG file.");
    #endif
    goto NAEndReadingPNG;
  }

  // Read the chunks until the IEND chunk is read.
  while(1){
    NAPNGChunk* chunk = na_AllocPNGChunkFromBuffer(&bufiter);
    naAddListLastMutable(&(png->chunks), chunk);
    if(chunk->type == NA_PNG_CHUNK_TYPE_IEND){break;}
  }
  naClearBufferIterator(&bufiter);

  // Create the buffer to hold the compressed and decompressed data
  png->compresseddata = naNewBuffer(NA_FALSE);
  naSetBufferEndianness(png->compresseddata, NA_ENDIANNESS_NETWORK);
  png->filteredData = naNewBuffer(NA_FALSE);

  naBeginListMutatorIteration(NAPNGChunk* chunk, &(png->chunks), iter);
    switch(chunk->type){
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
  naEndListIteration(iter);

//  NAFile* outfile = naCreateFileWritingPath("test.raw", NA_FILEMODE_DEFAULT);
//  naWriteBufferToFile(png->compresseddata, outfile);
//  naReleaseFile(outfile);

  naFixBufferRange(png->compresseddata);
  naFillBufferWithZLIBDecompression(png->filteredData, png->compresseddata);
  naReconstructFilterData(png);

  NAEndReadingPNG:
  naRelease(buffer);

  return png;
}



NA_API NAPNG* naNewPNGWithBabyImage(NABabyImage* babyImage){
  NAPNG* png = naNewPNG(naGetBabyImageSize(babyImage), NA_PNG_COLORTYPE_TRUECOLOR_ALPHA, 8);
  naConvertBabyImageTou8(babyImage, png->pixeldata, NA_TRUE, NA_COLOR_BUFFER_RGBA);
  return png;
}



NA_DEF void* naGetPNGPixelData(NAPNG* png){
  return png->pixeldata;
}



NA_DEF size_t naGetPNGPixelDataByteSize(NAPNG* png){
  size_t bpp = naGetPNGBytesPerPixel(png->colorType);
  return naGetSizeiIndexCount(png->size) * bpp;
}



NA_DEF NABabyImage* naCreateBabyImageFromPNG(NAPNG* png){
  NABabyImage* babyImage = naCreateBabyImage(png->size, NA_NULL);
  NAByte* pngptr;
  float* babyptr;
  uint8 inbuf[4];
  NAInt x, y;

  switch(png->colorType){
  case NA_PNG_COLORTYPE_TRUECOLOR:
    pngptr = png->pixeldata;
    inbuf[3] = 255;
    for(y = 0; y < png->size.height; y++){
      babyptr = &(naGetBabyImageData(babyImage)[(png->size.height - y - 1) * naGetBabyImageValuesPerLine(babyImage)]);
      for(x = 0; x < png->size.width; x++){
        inbuf[0] = pngptr[0];
        inbuf[1] = pngptr[1];
        inbuf[2] = pngptr[2];
        naFillBabyColorWithu8(babyptr, inbuf, NA_COLOR_BUFFER_RGB);
        babyptr += 4;
        pngptr += 3;
      }
    }
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR_ALPHA:
    naFillBabyImageWithu8(babyImage, png->pixeldata, NA_TRUE, NA_COLOR_BUFFER_RGBA);
    break;
  default:
    #if NA_DEBUG
      naError("Not implemented yet");
    #endif
    break;
  }
  return babyImage;
}



NA_DEF NASizei naGetPNGSize(NAPNG* png){
  return png->size;
}



NA_DEF NAPNGColorType naGetPNGColorType(NAPNG* png){
  return png->colorType;
}



NA_DEF NAInt naGetPNGBitDepth(NAPNG* png){
  return (NAInt)png->bitDepth;
}



NA_DEF void naWritePNGToPath(NAPNG* png, const char* filePath){

  NABuffer* outbuffer;
  NAChecksum checksum;
  NAFile* outfile;
  NAListIterator iter;
  NABufferIterator iterout;

  naAddListLastMutable(&(png->chunks), na_AllocPNGIHDRChunk(png));
  naAddListLastMutable(&(png->chunks), na_AllocPNGIDATChunk(png));
  naAddListLastMutable(&(png->chunks), na_AllocPNGIENDChunk(png));

  outbuffer = naNewBuffer(NA_FALSE);
  iterout = naMakeBufferMutator(outbuffer);
  naSetBufferEndianness(outbuffer, NA_ENDIANNESS_NETWORK);

  naWriteBufferBytes(&iterout, na_PngMagic, 8);

  naBeginListMutatorIteration(NAPNGChunk* chunk, &(png->chunks), iter);
    naFixBufferRange(chunk->data);

    chunk->length = (uint32)naGetBufferRange(chunk->data).length;
    naWriteBufferu32(&iterout, chunk->length);

    naCopy32(chunk->typeName, na_PngChunkTypeNames[chunk->type]);
    naWriteBufferBytes(&iterout, chunk->typeName, 4);

    if(!naIsBufferEmpty(chunk->data)){
      naWriteBufferBuffer(&iterout, chunk->data, naGetBufferRange(chunk->data));
    }

    naInitChecksum(&checksum, NA_CHECKSUM_TYPE_CRC_PNG);
    naAccumulateChecksum(&checksum, chunk->typeName, 4);
    if(chunk->length){
      naAccumulateChecksumBuffer(&checksum, chunk->data);
    }
    chunk->crc = naGetChecksumResult(&checksum);
    naClearChecksum(&checksum);
    naWriteBufferu32(&iterout, chunk->crc);
  naEndListIteration(iter);

  naClearBufferIterator(&iterout);

  outfile = naCreateFileWritingPath(filePath, NA_FILEMODE_DEFAULT);
  naFixBufferRange(outbuffer);
  naWriteBufferToFile(outbuffer, outfile);
  naReleaseFile(outfile);
  naRelease(outbuffer);
}



// This is the destructor for a PNG. It is marked as a helper as it should
// only be called by the runtime system
NA_HDEF void na_DestructPNG(NAPNG* png){
  naForeachListMutable(&(png->chunks), (NAMutator)na_DeallocPNGChunk);
  naClearList(&(png->chunks));
  if(png->pixeldata){naFree(png->pixeldata);}
  if(png->compresseddata){naRelease(png->compresseddata);}
  if(png->filteredData){naRelease(png->filteredData);}
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
