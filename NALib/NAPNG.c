
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "NAPNG.h"
#include "NABuffer.h"
#include "NADeflate.h"

// Reference: http://www.w3.org/TR/PNG

#define NA_PNG_FLAGS_IHDR_AVAILABLE       0x01
#define NA_PNG_FLAGS_sRGB_AVAILABLE       0x02


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
  NABuffer data;      // All data without the preceding chunk type
  NAChunkType type;   // The type as an enum.
  NABufInt length;
  uint8 typename[4];  // The type as 4 uint8
  uint32 crc;
};


NA_HDEF void naSetPNGDefaultColorimetry(NAPNG* png){
  png->gamma = 45455.f / 100000.f;
  naFillV2f(png->whitepoint,    31270.f / 100000.f, 32900.f / 100000.f);
  naFillV2f(png->redprimary,    64000.f / 100000.f, 64000.f / 100000.f);
  naFillV2f(png->greenprimary,  30000.f / 100000.f, 60000.f / 100000.f);
  naFillV2f(png->blueprimary,   15000.f / 100000.f,  6000.f / 100000.f);
}


NA_HDEF NAPNGChunk* naAllocPNGChunkFromBuffer(NABuffer* buffer){
  NAInt i;
  NAChecksum checksum;
  uint32 crc;

  NAPNGChunk* chunk = naAlloc(NAPNGChunk);

  chunk->length = naReadBufferUInt32(buffer);
  #ifndef NDEBUG
    if(chunk->length > (1U<<31)-1U)
      naError("naAllocPNGChunkFromBuffer", "length should not exceed 2^32-1.");
  #endif

  naReadBufferBytes(buffer, chunk->typename, 4);
  naInitBufferInputtingFromBufferExtraction(&(chunk->data), buffer, chunk->length);

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
  
  naInitChecksum(&checksum, NA_CHECKSUM_TYPE_CRC_PNG);
  naAccumulateChecksum(&checksum, chunk->typename, 4);
  naAccumulateBufferToChecksum(&(chunk->data), &checksum);
  crc = naGetChecksumResult(&checksum);
  naClearChecksum(&checksum);
  
  chunk->crc = naReadBufferUInt32(buffer);
  
  #ifndef NDEBUG
    if(chunk->crc != crc)
      naError("naAllocPNGChunkFromBuffer", "Checksum validation failed.");
  #else
    NA_UNUSED(crc);
  #endif

  return chunk;
}



NA_HDEF void naDeallocPNGChunk(NAPNGChunk* chunk){
  naClearBuffer(&(chunk->data));
  naFree(chunk);
}




NA_API NAUInt naGetPNGBytesPerPixel(NAPNGColorType colortype){
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

  NAInt bpp = naGetPNGBytesPerPixel(png->colortype);
  NAUInt bytesperline = png->size.width * bpp;
  
  png->pixeldata = naMalloc(sizeof(NAByte) * png->size.width * png->size.height * bpp);
  naSeekBufferReadAbsolute(&(png->filtereddata), 0);
  curbyte = png->pixeldata;
  
  upbuffer = naMalloc(bytesperline);
  naNulln(upbuffer, bytesperline);
  upbufptr = upbuffer;

  for(y=0; y<png->size.height; y++){

    NAByte filtertype = naReadBufferUInt8(&(png->filtereddata));
    naReadBufferArrayUInt8(&(png->filtereddata), curbyte, bytesperline);

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

  naFree(upbuffer);
}




NA_DEF void naFilterData(NAPNG* png){
  NAUInt bpp;
  NAByte* pixeldata;
  NAInt y;

  naInitBuffer(&(png->filtereddata));
  naSetBufferEndianness(&(png->filtereddata), NA_ENDIANNESS_NETWORK);
  
  bpp = naGetPNGBytesPerPixel(png->colortype);
  pixeldata = naGetPNGPixelData(png);
  
  for(y=0; y<png->size.height; y++){
    naWriteBufferUInt8(&(png->filtereddata), NA_PNG_FILTER_TYPE_NONE);
    naWriteBufferBytes(&(png->filtereddata), pixeldata, png->size.width * bpp);
    pixeldata += png->size.width * bpp;
  }

  naFixBufferMaxPos(&(png->filtereddata));
}





NA_HDEF void naReadPNGIHDRChunk(NAPNG* png, NAPNGChunk* ihdr){

  #ifndef NDEBUG
    if(png->flags & NA_PNG_FLAGS_IHDR_AVAILABLE)
      naError("naReadPNGIHDRChunk", "IHDR chunk already read.");
  #endif

  png->size.width = naReadBufferUInt32(&(ihdr->data));
  png->size.height = naReadBufferUInt32(&(ihdr->data));
  png->bitdepth = naReadBufferInt8(&(ihdr->data));
  png->colortype = (NAPNGColorType)naReadBufferInt8(&(ihdr->data));
  png->compressionmethod = naReadBufferInt8(&(ihdr->data));
  #ifndef NDEBUG
    if(png->compressionmethod != 0)
      naError("naReadPNGIHDRChunk", "Invalid compression method in IHDR chunk");
  #endif
  png->filtermethod = naReadBufferInt8(&(ihdr->data));
  #ifndef NDEBUG
    if(png->filtermethod != 0)
      naError("naReadPNGIHDRChunk", "Invalid filter method in IHDR chunk");
  #endif
  png->interlacemethod = (NAPNGInterlaceMethod)naReadBufferInt8(&(ihdr->data));
  #ifndef NDEBUG
    if(png->interlacemethod == 1)
      naError("naReadPNGIHDRChunk", "This version of NALib can not read interlaced PNG files. Has not been implemented yet. Sorry.");
  #endif
  
  
  png->significantbits[0] = png->bitdepth;
  png->significantbits[1] = png->bitdepth;
  png->significantbits[2] = png->bitdepth;
  png->significantbits[3] = png->bitdepth;
  
  png->flags &= NA_PNG_FLAGS_IHDR_AVAILABLE;
  
}



NA_HDEF NAPNGChunk* naAllocPNGIHDRChunk(NAPNG* png){
  NAPNGChunk* ihdr = naAlloc(NAPNGChunk);
  naInitBuffer(&(ihdr->data));
  naSetBufferEndianness(&(ihdr->data), NA_ENDIANNESS_NETWORK);
  ihdr->type = NA_PNG_CHUNK_TYPE_IHDR;
  
  naWriteBufferUInt32(&(ihdr->data), (uint32)png->size.width);
  naWriteBufferUInt32(&(ihdr->data), (uint32)png->size.height);
  naWriteBufferInt8(&(ihdr->data), png->bitdepth);
  naWriteBufferInt8(&(ihdr->data), png->colortype);
  naWriteBufferInt8(&(ihdr->data), png->compressionmethod);
  naWriteBufferInt8(&(ihdr->data), png->filtermethod);
  naWriteBufferInt8(&(ihdr->data), png->interlacemethod);
  
  return ihdr;
}



NA_HDEF void naReadPNGPLTEChunk(NAPNG* png, NAPNGChunk* plte){
  // todo
//  printf("plte");
  NA_UNUSED(png);
  NA_UNUSED(plte);
}



NA_HDEF void naReadPNGIDATChunk(NAPNG* png, NAPNGChunk* idat){
  naFillBufferWithDeflateDecompression(&(png->filtereddata), &(idat->data));
}



NA_HDEF NAPNGChunk* naAllocPNGIDATChunk(NAPNG* png){
  NAPNGChunk* idat = naAlloc(NAPNGChunk);
  naInitBuffer(&(idat->data));
  naSetBufferEndianness(&(idat->data), NA_ENDIANNESS_NETWORK);
  
  naFilterData(png);
  naFillBufferWithDeflateCompression(&(idat->data), &(png->filtereddata));

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
  naInitBuffer(&(iend->data));
  naSetBufferEndianness(&(iend->data), NA_ENDIANNESS_NETWORK);
  iend->type = NA_PNG_CHUNK_TYPE_IEND;
  return iend;
}



NA_HDEF void naReadPNGcHRMChunk(NAPNG* png, NAPNGChunk* chrm){
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE){
    // Ignoring when sRGB is available
  }else{
    png->whitepoint[0]    = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
    png->whitepoint[1]    = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
    png->redprimary[0]    = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
    png->redprimary[1]    = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
    png->greenprimary[0]  = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
    png->greenprimary[1]  = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
    png->blueprimary[0]   = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
    png->blueprimary[1]   = (float)naReadBufferUInt32(&(chrm->data)) / 100000.f;
  }
}



NA_HDEF void naReadPNGgAMAChunk(NAPNG* png, NAPNGChunk* gama){
  if(png->flags & NA_PNG_FLAGS_sRGB_AVAILABLE){
    // Ignoring when sRGB is available
  }else{
    png->gamma = (float)naReadBufferUInt32(&(gama->data)) / 100000.f;
  }
}



NA_HDEF void naReadPNGiCCPChunk(NAPNG* png, NAPNGChunk* iccp){
  // todo
//  printf("iccp");
  NA_UNUSED(png);
  NA_UNUSED(iccp);
}



NA_HDEF void naReadPNGsBITChunk(NAPNG* png, NAPNGChunk* sbit){
  switch(png->colortype){
  case NA_PNG_COLORTYPE_GREYSCALE:
    png->significantbits[1] = naReadBufferUInt8(&(sbit->data));
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR:
  case NA_PNG_COLORTYPE_INDEXEDCOLOR:
    png->significantbits[0] = naReadBufferUInt8(&(sbit->data));
    png->significantbits[1] = naReadBufferUInt8(&(sbit->data));
    png->significantbits[2] = naReadBufferUInt8(&(sbit->data));
    break;
  case NA_PNG_COLORTYPE_GREYSCALE_ALPHA:
    png->significantbits[1] = naReadBufferUInt8(&(sbit->data));
    png->significantbits[3] = naReadBufferUInt8(&(sbit->data));
    break;
  case NA_PNG_COLORTYPE_TRUECOLOR_ALPHA:
    png->significantbits[0] = naReadBufferUInt8(&(sbit->data));
    png->significantbits[1] = naReadBufferUInt8(&(sbit->data));
    png->significantbits[2] = naReadBufferUInt8(&(sbit->data));
    png->significantbits[3] = naReadBufferUInt8(&(sbit->data));
    break;
  }
}



NA_HDEF void naReadPNGsRGBChunk(NAPNG* png, NAPNGChunk* srgb){
  uint8 intent = naReadBufferUInt8(&(srgb->data));
  // As this implementation is not yet capable of color management, we ignore
  // the gAMA and cHRM values and set them to the following:
  NA_UNUSED(intent);
  naSetPNGDefaultColorimetry(png);
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
  
  png->pixeldimensions[0] = (float)naReadBufferUInt32(&(phys->data));
  png->pixeldimensions[1] = (float)naReadBufferUInt32(&(phys->data));
  unit = naReadBufferUInt8(&(phys->data));
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
  png->modificationdate = naMakeDateTimeFromBuffer(&(time->data), NA_DATETIME_FORMAT_PNG);
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














NA_DEF NAPNG* naInitPNGWithFile(NAPNG* png, const char* filename){
  NABuffer* buffer;
  NAByte magic[8];
  NAPNGChunk* curchunk;
  
  naInitList(&(png->chunks));
  png->flags = 0;
  naSetPNGDefaultColorimetry(png);
  png->pixeldimensions[0] = 1.;
  png->pixeldimensions[1] = 1.;
  png->pixelunit = NA_PIXEL_UNIT_RATIO;
  
  buffer = naInitBufferInputtingFromFile(naAlloc(NABuffer), filename);
  naComputeBufferMaxPos(buffer);
  if(naIsBufferReadAtEnd(buffer)){
    goto NAEndReadingPNG;
  }
  naSetBufferEndianness(buffer, NA_ENDIANNESS_NETWORK);

  naReadBufferBytes(buffer, magic, 8);
  if(!naEqual64(magic, na_png_magic)){
    #ifndef NDEBUG
      naError("naInitPNGWithFile", "File is not a PNG file.");
    #endif
    goto NAEndReadingPNG;
  }
  
  while(1){
    curchunk = naAllocPNGChunkFromBuffer(buffer);
    if(curchunk->type == NA_PNG_CHUNK_TYPE_IEND){break;}
    naAddListLastMutable(&(png->chunks), curchunk);
  }
  
  // Create the buffer to hold the decompressed data
  naInitBuffer(&(png->filtereddata));
  
  naFirstList(&(png->chunks));
  while((curchunk = naIterateListMutable(&(png->chunks), 1))){
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
  
  naReconstructFilterData(png);
  
  NAEndReadingPNG:
  naClearBuffer(buffer);
  naFree(buffer);
  return png;
}





NA_DEF NAPNG* naInitPNG(NAPNG* png, NASizei size, NAPNGColorType colortype, NAUInt bitdepth){
  NAUInt bpp;
  
  #ifndef NDEBUG
    if(bitdepth != 8)
      naError("naInitPNG", "Sorry, bitdepth must be 8 for now. Everything else will be implemented later.");
    if((colortype != NA_PNG_COLORTYPE_TRUECOLOR) && (colortype != NA_PNG_COLORTYPE_TRUECOLOR_ALPHA))
      naError("naInitPNG", "Sorry, colortype must be truecolor with or without alpha. Everything else will be implemented later.");
  #endif
  naInitList(&(png->chunks));
  png->flags = 0;
  png->bitdepth = bitdepth;
  png->compressionmethod = 0;
  png->interlacemethod = NA_PNG_INTERLACE_NONE;
  png->filtermethod = 0;
  naSetPNGDefaultColorimetry(png);
  png->pixeldimensions[0] = 1.;
  png->pixeldimensions[1] = 1.;
  png->pixelunit = NA_PIXEL_UNIT_RATIO;
  png->size = size;
  png->colortype = colortype;
  
  bpp = naGetPNGBytesPerPixel(colortype);
  png->pixeldata = naMalloc(size.width * size.height * bpp);
  
  return png;
}



NA_DEF void* naGetPNGPixelData(NAPNG* png){
  return png->pixeldata;
}



NA_DEF NAUInt naGetPNGPixelDataSize(NAPNG* png){
  NAUInt bpp = naGetPNGBytesPerPixel(png->colortype);
  return png->size.width * png->size.height * bpp;
}



NA_DEF NASizei naGetPNGSize(NAPNG* png){
  return png->size;
}



NA_DEF NAPNGColorType naGetPNGColorType(NAPNG* png){
  return png->colortype;
}



NA_DEF NAUInt naGetPNGBitDepth(NAPNG* png){
  return png->bitdepth;
}



NA_DEF void naWritePNGToFile(NAPNG* png, const char* filename){
  NABuffer outbuffer;
  NAPNGChunk* curchunk;
  NAChecksum checksum;
  NAFile outfile;

  naAddListLastMutable(&(png->chunks), naAllocPNGIHDRChunk(png));
  naAddListLastMutable(&(png->chunks), naAllocPNGIDATChunk(png));
  naAddListLastMutable(&(png->chunks), naAllocPNGIENDChunk(png));

  naInitBuffer(&outbuffer);
  naSetBufferEndianness(&outbuffer, NA_ENDIANNESS_NETWORK);

  naWriteBufferBytes(&outbuffer, na_png_magic, 8);

  naFirstList(&(png->chunks));
  while((curchunk = naIterateListMutable(&(png->chunks), 1))){
    naFixBufferMaxPos(&(curchunk->data));
    
    curchunk->length = naGetBufferSize(&(curchunk->data));
    naWriteBufferUInt32(&outbuffer, (uint32)curchunk->length);
    
    naCopy32(curchunk->typename, na_png_chunk_type_names[curchunk->type]);
    naWriteBufferBytes(&outbuffer, curchunk->typename, 4);
    
    naWriteBufferBuffer(&outbuffer, &(curchunk->data), naGetBufferSize(&(curchunk->data)));
    
    naInitChecksum(&checksum, NA_CHECKSUM_TYPE_CRC_PNG);
    naAccumulateChecksum(&checksum, curchunk->typename, 4);
    if(curchunk->length){
      naSeekBufferReadLocal(&(curchunk->data), 0);
      naAccumulateBufferToChecksum(&(curchunk->data), &checksum);
    }
    curchunk->crc = naGetChecksumResult(&checksum);
    naClearChecksum(&checksum);
    naWriteBufferUInt32(&outbuffer, curchunk->crc);
  }

  outfile = naMakeFileWritingFilename(filename, NA_FILEMODE_DEFAULT);
  naWriteBufferToFile(&outbuffer, &outfile, NA_FALSE);
  naCloseFile(&outfile);
  naClearBuffer(&outbuffer);
}



// This is the destructor for a PNG. It is marked as a helper as it should
// only be called by the runtime system
NA_DEF void naClearPNG(NAPNG* png){
  naForeachList(&(png->chunks), (NAFunc)naDeallocPNGChunk);
  naClearList(&(png->chunks));
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
