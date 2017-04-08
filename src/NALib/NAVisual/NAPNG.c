
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "../NAPNG.h"
#include "../NABuffer.h"
#include "../NADeflate.h"
#include "../NAFile.h"

// Reference: http://www.w3.org/TR/PNG

#define NA_PNG_FLAGS_IHDR_AVAILABLE       0x01
#define NA_PNG_FLAGS_sRGB_AVAILABLE       0x02


#include "../NAList.h"
#include "../NAVectorAlgebra.h"

typedef enum{
  NA_PIXEL_UNIT_UNDEFINED,
  NA_PIXEL_UNIT_RATIO,
  NA_PIXEL_UNIT_PER_INCH,
  NA_PIXEL_UNIT_PER_METER,
} NAPixelUnit;

typedef enum{
  NA_PNG_INTERLACE_NONE = 0,
  NA_PNG_INTERLACE_ADAM7 = 1,
} NAPNGInterlaceMethod;

struct NAPNG{
  NASizei size;
  NAList chunks;
  uint32 flags;
  int8 bitdepth;
  NAPNGColorType colortype;
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

  NAByte* pixeldata;
  NABuffer* compresseddata;
  NABuffer* filtereddata;
};


NA_HAPI void naDestructPNG(NAPNG* png);
NA_RUNTIME_TYPE(NAPNG, naDestructPNG);


typedef enum{
  NA_PNG_FILTER_TYPE_NONE     = 0,
  NA_PNG_FILTER_TYPE_SUB      = 1,
  NA_PNG_FILTER_TYPE_UP       = 2,
  NA_PNG_FILTER_TYPE_AVERAGE  = 3,
  NA_PNG_FILTER_TYPE_PAETH    = 4,
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
  NA_PNG_CHUNK_TYPE_COUNT,
} NAChunkType;


NAByte na_png_chunk_type_names[NA_PNG_CHUNK_TYPE_COUNT][4] = {
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


uint8 na_png_magic[8] = {137, 80, 78, 71, 13, 10, 26, 10};


typedef struct NAPNGChunk NAPNGChunk;
struct NAPNGChunk{
  NABuffer* data;      // All data without the preceding chunk type
  NAChunkType type;   // The type as an enum.
  NAInt length;
  uint8 typename[4];  // The type as 4 uint8
  uint32 crc;
};


NA_HDEF void naSetPNGsRGBColorimetry(NAPNG* png){
  // This corresponds to the sRGB space.
  png->gamma = 45455.f / 100000.f;
  naFillV2f(png->whitepoint,    31270.f / 100000.f, 32900.f / 100000.f);
  naFillV2f(png->redprimary,    64000.f / 100000.f, 33000.f / 100000.f);
  naFillV2f(png->greenprimary,  30000.f / 100000.f, 60000.f / 100000.f);
  naFillV2f(png->blueprimary,   15000.f / 100000.f,  6000.f / 100000.f);
}


NA_HDEF NAPNGChunk* naAllocPNGChunkFromBuffer(NABufferIterator* iter){
  NAInt i;
  NAChecksum checksum;
  uint32 crc;

  NAPNGChunk* chunk = naAlloc(NAPNGChunk);

  chunk->length = (NAInt)naReadBufferu32(iter);
  #ifndef NDEBUG
    if((NAUInt)chunk->length > (1U<<31)-1U)
      naError("naAllocPNGChunkFromBuffer", "length should not exceed 2^31-1.");
  #endif

  naReadBufferBytes(iter, chunk->typename, 4);
  if(chunk->length){
    chunk->data = naReadBufferBuffer(iter, chunk->length, NA_FALSE, NA_FALSE);
  }else{
    chunk->data = naCreateBuffer(NA_FALSE);
  }
  naSeekBufferRelative(iter, chunk->length);

  chunk->type = NA_PNG_CHUNK_TYPE_UNKNOWN;
  for(i=0; i<NA_PNG_CHUNK_TYPE_COUNT; i++){
    if(naEqual32(na_png_chunk_type_names[i], chunk->typename)){
      chunk->type = (NAChunkType)i;
      break;
    }
  }
  #ifndef NDEBUG
//    if(chunk->type == NA_PNG_CHUNK_TYPE_UNKNOWN){
//      printf("Undefined Chunkname %c%c%c%c\n", chunkname[0], chunkname[1], chunkname[2], chunkname[3]);
//    }
  #endif
  
  if(chunk->length){
    naCacheBufferRange(chunk->data, naGetBufferRange(chunk->data), NA_FALSE);
  }
  
  naInitChecksum(&checksum, NA_CHECKSUM_TYPE_CRC_PNG);
  naAccumulateChecksum(&checksum, chunk->typename, 4);
  naAccumulateBufferToChecksum(chunk->data, &checksum);
  crc = naGetChecksumResult(&checksum);
  naClearChecksum(&checksum);
  
  chunk->crc = naReadBufferu32(iter);
  
  #ifndef NDEBUG
    if(chunk->crc != crc)
      naError("naAllocPNGChunkFromBuffer", "Checksum validation failed.");
  #else
    NA_UNUSED(crc);
  #endif

  return chunk;
}



NA_HDEF void naDeallocPNGChunk(NAPNGChunk* chunk){
  naReleaseBuffer(chunk->data);
  naFree(chunk);
}




NA_API NAInt naGetPNGBytesPerPixel(NAPNGColorType colortype){
  switch(colortype){
  case NA_PNG_COLORTYPE_GREYSCALE:
    return 1;
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR:
    return 3;
    break;
  case NA_PNG_COLORTYPE_INDEXEDCOLOR:
    return 1;
    break;
  case NA_PNG_COLORTYPE_GREYSCALE_ALPHA:
    return 2;
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR_ALPHA:
    return 4;
    break;
  default:
    #ifndef NDEBUG
      naError("naGetPNGBytesPerPixel", "Invalid colortype given");
    #endif
    return 0;
    break;
  }
}



NA_DEF NAByte naGetPaethPredictor(NAByte a, NAByte b, NAByte c){
  NAInt p = (NAInt)a + (NAInt)b - (NAInt)c;
  NAInt pa = naAbsi(p - a);
  NAInt pb = naAbsi(p - b);
  NAInt pc = naAbsi(p - c);
  if((pa <= pb) && (pa <= pc)){return a;}
  else if(pb <= pc){return b;}
  else{return c;}
}



NA_DEF void naReconstructFilterData(NAPNG* png){
  NAByte* curbyte;
  NAByte* upbuffer;
  NAByte* upbufptr;
  NAInt x, y;
  NABufferIterator iterfilter;

  NAInt bpp = naGetPNGBytesPerPixel(png->colortype);
  NAInt bytesperline = png->size.width * bpp;
  
  png->pixeldata = naMalloc(naSizeof(NAByte) * png->size.width * png->size.height * bpp);
  curbyte = png->pixeldata;
  
  upbuffer = naMalloc(bytesperline);
  naNulln(upbuffer, (NAUInt)bytesperline);
  upbufptr = upbuffer;
  
  iterfilter = naMakeBufferIteratorMutator(png->filtereddata);

  for(y=0; y<png->size.height; y++){

    NAByte filtertype = naReadBufferu8(&iterfilter);
    naReadBufferu8v(&iterfilter, curbyte, bytesperline);

    switch(filtertype){
    case NA_PNG_FILTER_TYPE_NONE:
      // nothing to do.
      curbyte += bytesperline;
      break;
    case NA_PNG_FILTER_TYPE_SUB:
      for(x=0; x<bpp; x++){
        curbyte++;  // The first byte adds value 0 from the virtual left byte.
      }
      for(x=bpp; x<(png->size.width * bpp); x++){
        *curbyte += *(curbyte - bpp);
        curbyte++;
      }
      break;
    case NA_PNG_FILTER_TYPE_UP:
      for(x=0; x<(png->size.width * bpp); x++){
        *curbyte += *upbufptr;
        curbyte++;
        upbufptr++;
      }
      break;
    case NA_PNG_FILTER_TYPE_AVERAGE:
      for(x=0; x<bpp; x++){
        *curbyte += (NAByte)(((NAUInt)*upbufptr) / 2);
        curbyte++;
        upbufptr++;
      }
      for(x=bpp; x<(png->size.width * bpp); x++){
        *curbyte += (NAByte)(((NAUInt)*(curbyte - bpp) + (NAUInt)*upbufptr) / 2);
        curbyte++;
        upbufptr++;
      }
      break;
    case NA_PNG_FILTER_TYPE_PAETH:
      for(x=0; x<bpp; x++){
        *curbyte += naGetPaethPredictor(0, *upbufptr, 0);
        curbyte++;
        upbufptr++;
      }
      for(x=bpp; x<(png->size.width * bpp); x++){
        *curbyte += naGetPaethPredictor(*(curbyte - bpp), *upbufptr, *(upbufptr - bpp));
        curbyte++;
        upbufptr++;
      }
      break;
    default:
      printf("invalid filter\n");
      curbyte += bytesperline;
      break;
    }
    
    upbufptr = curbyte - bytesperline;
  }

//  outfile = naCreateFileWritingFilename("test.raw", NA_FILEMODE_DEFAULT);
//  naWriteFileBytes(outfile, png->pixeldata, png->size.width * png->size.height * bpp);
//  naReleaseFile(outfile);

  naClearBufferIterator(&iterfilter);
  naFree(upbuffer);
}




NA_DEF void naFilterData(NAPNG* png){
  NAInt bpp;
  NAByte* pixeldata;
  NAInt y;

  png->filtereddata = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(png->filtereddata, NA_ENDIANNESS_NETWORK);
  
  bpp = naGetPNGBytesPerPixel(png->colortype);
  pixeldata = naGetPNGPixelData(png);
  
  NABufferIterator iter = naMakeBufferIteratorModifier(png->filtereddata);
  for(y=0; y<png->size.height; y++){
    naWriteBufferu8(&iter, NA_PNG_FILTER_TYPE_NONE);
    naWriteBufferBytes(&iter, pixeldata, png->size.width * bpp);
    pixeldata += png->size.width * bpp;
  }
  naClearBufferIterator(&iter);

  naFixBufferRange(png->filtereddata);
}





NA_HDEF void naReadPNGIHDRChunk(NAPNG* png, NAPNGChunk* ihdr){

  NABufferIterator iter;
  
  #ifndef NDEBUG
    if(png->flags & NA_PNG_FLAGS_IHDR_AVAILABLE)
      naError("naReadPNGIHDRChunk", "IHDR chunk already read.");
  #endif

  iter = naMakeBufferIteratorAccessor(ihdr->data);

  png->size.width = (NAInt)naReadBufferu32(&iter);
  png->size.height = (NAInt)naReadBufferu32(&iter);
  png->bitdepth = naReadBufferi8(&iter);
  png->colortype = (NAPNGColorType)naReadBufferi8(&iter);
  png->compressionmethod = naReadBufferi8(&iter);
  #ifndef NDEBUG
    if(png->compressionmethod != 0)
      naError("naReadPNGIHDRChunk", "Invalid compression method in IHDR chunk");
  #endif
  png->filtermethod = naReadBufferi8(&iter);
  #ifndef NDEBUG
    if(png->filtermethod != 0)
      naError("naReadPNGIHDRChunk", "Invalid filter method in IHDR chunk");
  #endif
  png->interlacemethod = (NAPNGInterlaceMethod)naReadBufferi8(&iter);
  #ifndef NDEBUG
    if(png->interlacemethod == 1)
      naError("naReadPNGIHDRChunk", "This version of NALib can not read interlaced PNG files. Has not been implemented yet. Sorry.");
  #endif
  
  naClearBufferIterator(&iter);
  
  png->significantbits[0] = (uint8)png->bitdepth;
  png->significantbits[1] = (uint8)png->bitdepth;
  png->significantbits[2] = (uint8)png->bitdepth;
  png->significantbits[3] = (uint8)png->bitdepth;
  
  png->flags &= NA_PNG_FLAGS_IHDR_AVAILABLE;
  
}



NA_HDEF NAPNGChunk* naAllocPNGIHDRChunk(NAPNG* png){
  NABufferIterator iter;
  
  NAPNGChunk* ihdr = naAlloc(NAPNGChunk);
  ihdr->data = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(ihdr->data, NA_ENDIANNESS_NETWORK);
  ihdr->type = NA_PNG_CHUNK_TYPE_IHDR;
  
  iter = naMakeBufferIteratorModifier(ihdr->data);

  naWriteBufferu32(&iter, (uint32)png->size.width);
  naWriteBufferu32(&iter, (uint32)png->size.height);
  naWriteBufferi8(&iter, png->bitdepth);
  naWriteBufferi8(&iter, png->colortype);
  naWriteBufferi8(&iter, png->compressionmethod);
  naWriteBufferi8(&iter, png->filtermethod);
  naWriteBufferi8(&iter, png->interlacemethod);

  naClearBufferIterator(&iter);
    
  return ihdr;
}



NA_HDEF void naReadPNGPLTEChunk(NAPNG* png, NAPNGChunk* plte){
  // todo
//  printf("plte");
  NA_UNUSED(png);
  NA_UNUSED(plte);
}



NA_HDEF void naReadPNGIDATChunk(NAPNG* png, NAPNGChunk* idat){
  naAppendBufferToBuffer(png->compresseddata, idat->data);


//  NAFile outfile = naCreateFileWritingFilename("test.raw", NA_FILEMODE_DEFAULT);
//  naWriteBufferToFile(&(png->filtereddata), &outfile);
//  naReleaseFile(&outfile);
}



NA_HDEF NAPNGChunk* naAllocPNGIDATChunk(NAPNG* png){
  NAPNGChunk* idat = naAlloc(NAPNGChunk);
  idat->data = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(idat->data, NA_ENDIANNESS_NETWORK);
  
  naFilterData(png);
  naFillBufferWithZLIBCompression(idat->data, png->filtereddata, NA_DEFLATE_COMPRESSION_FASTEST);

  idat->type = NA_PNG_CHUNK_TYPE_IDAT;

  return idat;
}



NA_HDEF void naReadPNGIENDChunk(NAPNG* png, NAPNGChunk* iend){
  NA_UNUSED(png);
  NA_UNUSED(iend);
  // nothing to do. This chunk is empty and marks the end of the png stream.
}



NA_HDEF NAPNGChunk* naAllocPNGIENDChunk(NAPNG* png){
  NAPNGChunk* iend;
  NA_UNUSED(png);

  iend = naAlloc(NAPNGChunk);
  iend->data = naCreateBuffer(NA_FALSE);
  naSetBufferEndianness(iend->data, NA_ENDIANNESS_NETWORK);
  iend->type = NA_PNG_CHUNK_TYPE_IEND;

  return iend;
}



NA_HDEF void naReadPNGcHRMChunk(NAPNG* png, NAPNGChunk* chrm){
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE){
    // Ignoring when sRGB is available
  }else{
    NABufferIterator iter = naMakeBufferIteratorAccessor(chrm->data);

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



NA_HDEF void naReadPNGgAMAChunk(NAPNG* png, NAPNGChunk* gama){
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE){
    // Ignoring when sRGB is available
  }else{
    NABufferIterator iter = naMakeBufferIteratorAccessor(gama->data);
    png->gamma = (float)naReadBufferu32(&iter) / 100000.f;
    naClearBufferIterator(&iter);
  }
}



NA_HDEF void naReadPNGiCCPChunk(NAPNG* png, NAPNGChunk* iccp){
  // todo
//  printf("iccp");
  NA_UNUSED(png);
  NA_UNUSED(iccp);
}



NA_HDEF void naReadPNGsBITChunk(NAPNG* png, NAPNGChunk* sbit){
  NABufferIterator iter = naMakeBufferIteratorAccessor(sbit->data);
  switch(png->colortype){
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



NA_HDEF void naReadPNGsRGBChunk(NAPNG* png, NAPNGChunk* srgb){
  NABufferIterator iter = naMakeBufferIteratorAccessor(srgb->data);
  uint8 intent = naReadBufferu8(&iter);
  naClearBufferIterator(&iter);
  
  // As this implementation is not yet capable of color management, we ignore
  // the gAMA and cHRM values and set them to the following:
  NA_UNUSED(intent);
  naSetPNGsRGBColorimetry(png);
  png->flags |= NA_PNG_FLAGS_sRGB_AVAILABLE;
}



NA_HDEF void naReadPNGbKGDChunk(NAPNG* png, NAPNGChunk* bkgd){
  // todo
//  printf("bkgd");
  NA_UNUSED(png);
  NA_UNUSED(bkgd);
}



NA_HDEF void naReadPNGhISTChunk(NAPNG* png, NAPNGChunk* hist){
  // todo
//  printf("hist");
  NA_UNUSED(png);
  NA_UNUSED(hist);
}



NA_HDEF void naReadPNGtRNSChunk(NAPNG* png, NAPNGChunk* trns){
  // todo
//  printf("trns");
  NA_UNUSED(png);
  NA_UNUSED(trns);
}



NA_HDEF void naReadPNGpHYsChunk(NAPNG* png, NAPNGChunk* phys){
  uint8 unit;
  NABufferIterator iter = naMakeBufferIteratorAccessor(phys->data);
  
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
    #ifndef NDEBUG
      naError("naReadPNGpHYsChunk", "Undefined pixel unit");
    #endif
    break;
  }
}



NA_HDEF void naReadPNGsPLTChunk(NAPNG* png, NAPNGChunk* splt){
  // todo
//  printf("splt");
  NA_UNUSED(png);
  NA_UNUSED(splt);
}



NA_HDEF void naReadPNGtIMEChunk(NAPNG* png, NAPNGChunk* time){
  png->modificationdate = naMakeDateTimeFromBuffer(time->data, NA_DATETIME_FORMAT_PNG);
}



NA_HDEF void naReadPNGiTXtChunk(NAPNG* png, NAPNGChunk* itxt){
  // todo
//  printf("itxt");
  NA_UNUSED(png);
  NA_UNUSED(itxt);
}



NA_HDEF void naReadPNGtEXtChunk(NAPNG* png, NAPNGChunk* text){
  // todo
//  printf("text");
  NA_UNUSED(png);
  NA_UNUSED(text);
}



NA_HDEF void naReadPNGzTXtChunk(NAPNG* png, NAPNGChunk* ztxt){
  // todo
//  printf("ztxt");
  NA_UNUSED(png);
  NA_UNUSED(ztxt);
}











NA_DEF NAPNG* naNewPNG(NASizei size, NAPNGColorType colortype, NAUInt bitdepth){
  NAInt bpp;
  NAPNG* png = naNew(NAPNG);
  
  #ifndef NDEBUG
    if(bitdepth != 8)
      naError("naInitPNG", "Sorry, bitdepth must be 8 for now. Everything else will be implemented later.");
    if((colortype != NA_PNG_COLORTYPE_TRUECOLOR) && (colortype != NA_PNG_COLORTYPE_TRUECOLOR_ALPHA))
      naError("naInitPNG", "Sorry, colortype must be truecolor with or without alpha. Everything else will be implemented later.");
  #endif
  naInitList(&(png->chunks));
  png->flags = 0;
  png->bitdepth = (int8)bitdepth;
  png->compressionmethod = 0;
  png->interlacemethod = NA_PNG_INTERLACE_NONE;
  png->filtermethod = 0;
  naSetPNGsRGBColorimetry(png);
  png->pixeldimensions[0] = 1.f;
  png->pixeldimensions[1] = 1.f;
  png->pixelunit = NA_PIXEL_UNIT_RATIO;
  png->size = size;
  png->colortype = colortype;
  
  bpp = naGetPNGBytesPerPixel(colortype);
  png->pixeldata = naMalloc(size.width * size.height * bpp);
  png->filtereddata = NA_NULL;
  
  return png;
}



NA_DEF NAPNG* naNewPNGWithFile(const char* filename){
  NABuffer* buffer;
  NAByte magic[8];
  NAListIterator iter;
  NABufferIterator bufiter;

  NAPNG* png = naNew(NAPNG);
  naInitList(&(png->chunks));
  
  // Set the default values. Needed if no appropriate chunk is available.
  png->flags = 0;
  naSetPNGsRGBColorimetry(png);
  png->pixeldimensions[0] = 1.f;
  png->pixeldimensions[1] = 1.f;
  png->pixelunit = NA_PIXEL_UNIT_RATIO;
  png->filtereddata = NA_NULL;
  
  buffer = naCreateBufferWithInpuFile(filename);
  bufiter = naMakeBufferIteratorModifier(buffer);
  
  // If the buffer is empty, there is no png to read.
  if(naIsBufferAtEnd(&bufiter)){
    goto NAEndReadingPNG;
  }
  
  // Important! RFC 1950 is big endianed (network endianness)
  naSetBufferEndianness(buffer, NA_ENDIANNESS_NETWORK);

  // Read the magic numbers. If they do not match, this is no png file.
  naReadBufferBytes(&bufiter, magic, 8);
  if(!naEqual64(magic, na_png_magic)){
    #ifndef NDEBUG
      naError("naInitPNGWithFile", "File is not a PNG file.");
    #endif
    goto NAEndReadingPNG;
  }
  
  // Read the chunks until the IEND chunk is read.
  while(1){
    NAPNGChunk* curchunk = naAllocPNGChunkFromBuffer(&bufiter);
    naAddListLastMutable(&(png->chunks), curchunk);
    if(curchunk->type == NA_PNG_CHUNK_TYPE_IEND){break;}
  }
  
  // Create the buffer to hold the compressed and decompressed data
  png->compresseddata = naCreateBufferCollector();
  naSetBufferEndianness(png->compresseddata, NA_ENDIANNESS_NETWORK);
  png->filtereddata = naCreateBuffer(NA_FALSE);
  
  iter = naMakeListIteratorMutator(&(png->chunks));
  while(naIterateList(&iter, 1)){
    NAPNGChunk* curchunk = naGetListCurrentMutable(&iter);
    switch(curchunk->type){
    case NA_PNG_CHUNK_TYPE_IHDR:  naReadPNGIHDRChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_PLTE:  naReadPNGPLTEChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_IDAT:  naReadPNGIDATChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_IEND:  naReadPNGIENDChunk(png, curchunk);  break;

    case NA_PNG_CHUNK_TYPE_cHRM:  naReadPNGcHRMChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_gAMA:  naReadPNGgAMAChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_iCCP:  naReadPNGiCCPChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_sBIT:  naReadPNGsBITChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_sRGB:  naReadPNGsRGBChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_bKGD:  naReadPNGbKGDChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_hIST:  naReadPNGhISTChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_tRNS:  naReadPNGtRNSChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_pHYs:  naReadPNGpHYsChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_sPLT:  naReadPNGsPLTChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_tIME:  naReadPNGtIMEChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_iTXt:  naReadPNGiTXtChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_tEXt:  naReadPNGtEXtChunk(png, curchunk);  break;
    case NA_PNG_CHUNK_TYPE_zTXt:  naReadPNGzTXtChunk(png, curchunk);  break;
    default:
      break;
    }
  }
  naClearListIterator(&iter);
  
  naFixBufferRange(png->compresseddata);
  naFillBufferWithZLIBDecompression(png->filtereddata, png->compresseddata);
  naReconstructFilterData(png);
  
  NAEndReadingPNG:
  naClearBufferIterator(&bufiter);
  naReleaseBuffer(buffer);

  return png;
}




NA_DEF void* naGetPNGPixelData(NAPNG* png){
  return png->pixeldata;
}



NA_DEF NAInt naGetPNGPixelDataBytesize(NAPNG* png){
  NAInt bpp = naGetPNGBytesPerPixel(png->colortype);
  return png->size.width * png->size.height * bpp;
}



NA_DEF NASizei naGetPNGSize(NAPNG* png){
  return png->size;
}



NA_DEF NAPNGColorType naGetPNGColorType(NAPNG* png){
  return png->colortype;
}



NA_DEF NAInt naGetPNGBitDepth(NAPNG* png){
  return (NAInt)png->bitdepth;
}



NA_DEF void naWritePNGToFile(NAPNG* png, const char* filename){

  NABuffer* outbuffer;
  NAChecksum checksum;
  NAFile* outfile;
  NAListIterator iter;
  NABufferIterator iterout;

  naAddListLastMutable(&(png->chunks), naAllocPNGIHDRChunk(png));
  naAddListLastMutable(&(png->chunks), naAllocPNGIDATChunk(png));
  naAddListLastMutable(&(png->chunks), naAllocPNGIENDChunk(png));

  outbuffer = naCreateBuffer(NA_FALSE);
  iterout = naMakeBufferIteratorMutator(outbuffer);
  naSetBufferEndianness(outbuffer, NA_ENDIANNESS_NETWORK);

  naWriteBufferBytes(&iterout, na_png_magic, 8);

  iter = naMakeListIteratorMutator(&(png->chunks));
  while(naIterateList(&iter, 1)){

    NAPNGChunk* curchunk = naGetListCurrentMutable(&iter);
    naFixBufferRange(curchunk->data);

    curchunk->length = naGetBufferRange(curchunk->data).length;
    naWriteBufferu32(&iterout, (uint32)curchunk->length);
    
    naCopy32(curchunk->typename, na_png_chunk_type_names[curchunk->type]);
    naWriteBufferBytes(&iterout, curchunk->typename, 4);
    
    if(!naIsBufferEmpty(curchunk->data)){
//      naSeekBufferAbsolute(curchunk->data, 0);
      naWriteBufferBuffer(&iterout, curchunk->data, naGetBufferRange(curchunk->data));
    }
    
    naInitChecksum(&checksum, NA_CHECKSUM_TYPE_CRC_PNG);
    naAccumulateChecksum(&checksum, curchunk->typename, 4);
    if(curchunk->length){
//      naSeekBufferAbsolute(curchunk->data, 0);
      naAccumulateBufferToChecksum(curchunk->data, &checksum);
    }
    curchunk->crc = naGetChecksumResult(&checksum);
    naClearChecksum(&checksum);
    naWriteBufferu32(&iterout, curchunk->crc);
  }
  naClearListIterator(&iter);
  naClearBufferIterator(&iterout);

  outfile = naCreateFileWritingFilename(filename, NA_FILEMODE_DEFAULT);
  naFixBufferRange(outbuffer);
  naWriteBufferToFile(outbuffer, outfile);
  naReleaseFile(outfile);
  naReleaseBuffer(outbuffer);
}



// This is the destructor for a PNG. It is marked as a helper as it should
// only be called by the runtime system
NA_HDEF void naDestructPNG(NAPNG* png){
  naForeachListMutable(&(png->chunks), (NAMutator)naDeallocPNGChunk);
  naClearList(&(png->chunks));
  if(png->pixeldata){naFree(png->pixeldata);}
  if(png->filtereddata){naReleaseBuffer(png->filtereddata);}
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
