
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NABitArray.h"




struct NABitArray{
  struct NAByteArray fullstorage;
  struct NAByteArray bits;
};



NABitArray* naCreateBitArray(NABitArray* bitarray){
  bitarray = naAllocateIfNull(bitarray, sizeof(NABitArray));
  // Create two empty byte arrays
  naCreateByteArray(&(bitarray->fullstorage));
  naCreateByteArray(&(bitarray->bits));
  return bitarray;
}



NABitArray* naCreateBitArrayWithCount(NABitArray* bitarray, NAInt count){
  if(!count){return naCreateBitArray(bitarray);}
  bitarray = naAllocateIfNull(bitarray, sizeof(NABitArray));
  if(count > 0){
    naCreateByteArrayWithSize(&(bitarray->fullstorage), count);
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, -1);
  }else{
    naCreateByteArrayWithSize(&(bitarray->fullstorage), -count);
    naCreateByteArray(&(bitarray->bits));
  }
  return bitarray;
}


NABitArray * naCreateBitArrayExtension( NABitArray* dstarray,
                          NABitArray* srcarray,
                          NAInt offset,
                          NAInt size){
  NABit leadarithmetic = NA_BIT0;
  NABit trailarithmetic = NA_BIT0;
  NAInt i;
  NABit* dstptr;
  NABit leadbit;
  NABit trailbit;
  NAInt srccount;

  #ifndef NDEBUG
    if(!srcarray)
      naError("naCreateBitArrayExtension", "srcarray is Null-Pointer");
  #endif
  if(offset<0){leadarithmetic = NA_BIT1; offset = -offset;}
  if(size<0){trailarithmetic = NA_BIT1; size = -size;}
  if(size==0){size = naGetBitArrayCount(srcarray);}
  
  dstarray = naCreateBitArrayWithCount(dstarray, size);
  if(!size){return dstarray;}
  i=0;
  dstptr = naGetByteArrayMutablePointer(&(dstarray->fullstorage));
  trailbit = NA_BIT0;
  srccount = naGetBitArrayCount(srcarray);
  
  if(srccount){
    NABit* srcptr = naGetBitArrayBit(srcarray, 0);
    leadbit = leadarithmetic & (srcptr?(*srcptr):NA_BIT0);
    trailbit = trailarithmetic & (srcptr?(*naGetBitArrayBit(srcarray, -1)):NA_BIT0);
    while(i<offset){
      *dstptr++ = leadbit;
      i++;
    }
    while(srcptr && (i-offset)<srccount){
      *dstptr++ = *srcptr++;
      i++;
    }
  }
  
  while(i<size){
    *dstptr++ = trailbit;
    i++;
  }
  return dstarray;
}



NABitArray* naCreateBitArrayExtraction( NABitArray* dstarray,
                          NABitArray* srcarray,
                          NAInt offset,
                          NAInt size){
  dstarray = naAllocateIfNull(dstarray, sizeof(NABitArray));
  // An extractions fullstorage corresponds to the source bits not the source
  // fullstorage. This is for safety reasons such that an extracted array can
  // not change bits of its originated array outside of the extracted area.
  naCreateByteArrayExtraction(&(dstarray->fullstorage), &(srcarray->bits), offset, size);
  // Set the bits field to the whole fullstorage.
  naCreateByteArrayExtraction(&(dstarray->bits), &(dstarray->fullstorage), 0, -1);
  return dstarray;
}


// Helper function.
// Returns the needed size of the array given the count parameter of the
// createBitArrayFromXXXString functions and the expected count to store the
// full bit array.
NA_IHLP NAInt getBitArraySizeHintCount(NAInt sizehint, NAInt desiredcount){
  if(sizehint > 0){
    // When sizehint is positive, sizehint shall be used.
    return sizehint;
  }else if(sizehint == 0){
    // If sizehint is 0, all bites need to be stored.
    return desiredcount;
  }else{
    // When sizehint is negative, it denotes an alignment. Sufficient bits must
    // be allocated to store all bits including potential padding bits. Will
    // never return zero.
    NAInt retcount = desiredcount - (sizehint + (desiredcount % sizehint)) % sizehint;
    if(!retcount){retcount = -sizehint;}
    return retcount;
  }
}


// Helper function.
// Looks at the given sizehint and expands or extracts the given bitarray and
// fills the padding bits with zero if necessary. Warning: expects fullstorage
// to have enough bits allocated.
NA_IHLP void naEnsureBitArraySizeHint(NABitArray* bitarray, NAInt sizehint){
  NAInt bitcount = naGetByteArraySize(&(bitarray->bits));
  NAInt arraycount = getBitArraySizeHintCount(sizehint, bitcount);

  NAInt sizediff = arraycount - bitcount;
  if(!bitcount){
    // if the bitarray was completely empty, just fill it with zeros.
    naNulln(naGetByteArrayMutablePointer(&(bitarray->fullstorage)), sizediff);
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
  }else if(sizediff > 0){
    // The bitarray has fewer bits than needed. Fill with zero.
    naNulln(naGetByteArrayMutableByte(&(bitarray->bits), -1) + 1, sizediff);
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, arraycount);
  }else if(sizediff < 0){
    // The bit array has more bits stored than needed. Extract the relevant
    // part.
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->bits), 0, arraycount);
  }else{
    // The Bit array already has the precise size needed. Do nothing.
  }
}



NABitArray* naCreateBitArrayFromBinString(NABitArray* bitarray,
                                            NAString* string,
                                                NAInt sizehint){
  NAInt stringsize;
  NAInt arraycount;
  const NAUTF8Char* curchar;
  NABit* curbit;
  NAInt bitcount;
  NAInt i;

  bitarray = naAllocateIfNull(bitarray, sizeof(NABitArray));
  // We assume that the string does not contain many garbage characters, so
  // we just take the string size as our guess how long the bin array will be.
  stringsize = naGetStringSize(string);
  arraycount = getBitArraySizeHintCount(sizehint, stringsize);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!stringsize){return bitarray;}

  // todo: Add caching
  // Fill the bit array from lsb to msb.
  curchar = naGetStringConstChar(string, -1);
  curbit = naGetByteArrayMutablePointer(&(bitarray->fullstorage));
  bitcount = 0;
  curchar++;
  for(i = 0; i < stringsize; i++){
    curchar--;
    switch (*curchar){
    case '0':           *curbit = NA_BIT0; break;
    case '1':           *curbit = NA_BIT1; break;
    case 'o':           *curbit = NA_BIT0; break;
    case 'i':           *curbit = NA_BIT1; break;
    case 'O':           *curbit = NA_BIT0; break;
    case 'I':           *curbit = NA_BIT1; break;
    case 'L':           *curbit = NA_BIT1; break;
    default: continue;
    }
    curbit++;
    bitcount++;
    if(bitcount >= arraycount){break;}  // stop if count is reached.
  }
  if(bitcount){
    // Reaching here, the bits array is empty and the clear call is somewhat
    // unneccessary. But we should be consistend anyhow. Just think about what
    // would happen if one day, the implementation of ByteArray changes...
    naClearByteArray(&(bitarray->bits));
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, bitcount);
  }
  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}



NABitArray* naCreateBitArrayFromDecString(NABitArray* bitarray,
                                            NAString* string,
                                                NAInt sizehint){
  NAInt stringsize;
  NAInt arraycount;
  const NAUTF8Char* curchar;
  NABitArray nibble;
  NANibble* nibbleptr;
  NABitArray tenarray;
  NAInt i;

  bitarray = naAllocateIfNull(bitarray, sizeof(NABitArray));
  // We assume that the string does not contain many garbage characters, so
  // we just take the string size times 4 as our guess how long the bin array
  // will be.
  stringsize = naGetStringSize(string);
  arraycount = getBitArraySizeHintCount(sizehint, stringsize * 4);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!stringsize){return bitarray;}

  // todo: Add caching
  // Fill the bit array from lsb to msb by parsing the string from msb to lsb.
  curchar = naGetStringConstChar(string, 0);
  
  naCreateBitArrayWithCount(&nibble, 4); // todo: use existing storage.
  nibbleptr = (NANibble*)naGetBitArrayBit(&nibble, 0);
  naCreateBitArrayWithCount(&tenarray, stringsize * -4);
  
  curchar--;
  for(i = 0; i < stringsize; i++){
    curchar++;
    switch (*curchar){
    case '0': *nibbleptr = NA_NIBBLE_0; break;
    case '1': *nibbleptr = NA_NIBBLE_1; break;
    case '2': *nibbleptr = NA_NIBBLE_2; break;
    case '3': *nibbleptr = NA_NIBBLE_3; break;
    case '4': *nibbleptr = NA_NIBBLE_4; break;
    case '5': *nibbleptr = NA_NIBBLE_5; break;
    case '6': *nibbleptr = NA_NIBBLE_6; break;
    case '7': *nibbleptr = NA_NIBBLE_7; break;
    case '8': *nibbleptr = NA_NIBBLE_8; break;
    case '9': *nibbleptr = NA_NIBBLE_9; break;
    default: continue;
    }
    naComputeBitArrayMulTen(&tenarray, bitarray);
    naComputeBitArrayAddBitArray(bitarray, &tenarray, &nibble);
    // Note that you have to parse the whole string before having determined
    // the least significant bits.
  }
  naClearBitArray(&nibble);
  naClearBitArray(&tenarray);
  if(naGetBitArrayCount(bitarray)){
  }
  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}


NABitArray* naCreateBitArrayFromHexString(NABitArray* bitarray,
                                            NAString* string,
                                                NAInt sizehint){
  NAInt stringsize;
  NAInt arraycount;
  const NAUTF8Char* curchar;
  NABit* startbit;
  uint32* curnibble;
  NAInt bitcount;
  NAInt i;

  bitarray = naAllocateIfNull(bitarray, sizeof(NABitArray));
  // We assume that the string does not contain many garbage characters, so
  // we just take the string size as our guess how long the bin array will be.
  stringsize = naGetStringSize(string);
  arraycount = getBitArraySizeHintCount(sizehint, stringsize * 4);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!stringsize){return bitarray;}

  // Fill the bit array from lsb to msb.
  curchar = naGetStringConstChar(string, -1);
  startbit = naGetByteArrayMutablePointer(&(bitarray->fullstorage));
  curnibble = (uint32*)(startbit);
  bitcount = 0;
  curchar++;
  for(i = 0; i < stringsize; i++){
    curchar--;
    switch (*curchar){
    case '0':           *curnibble++ = NA_NIBBLE_0; break;
    case '1':           *curnibble++ = NA_NIBBLE_1; break;
    case '2':           *curnibble++ = NA_NIBBLE_2; break;
    case '3':           *curnibble++ = NA_NIBBLE_3; break;
    case '4':           *curnibble++ = NA_NIBBLE_4; break;
    case '5':           *curnibble++ = NA_NIBBLE_5; break;
    case '6':           *curnibble++ = NA_NIBBLE_6; break;
    case '7':           *curnibble++ = NA_NIBBLE_7; break;
    case '8':           *curnibble++ = NA_NIBBLE_8; break;
    case '9':           *curnibble++ = NA_NIBBLE_9; break;
    case 'a': case 'A': *curnibble++ = NA_NIBBLE_A; break;
    case 'b': case 'B': *curnibble++ = NA_NIBBLE_B; break;
    case 'c': case 'C': *curnibble++ = NA_NIBBLE_C; break;
    case 'd': case 'D': *curnibble++ = NA_NIBBLE_D; break;
    case 'e': case 'E': *curnibble++ = NA_NIBBLE_E; break;
    case 'f': case 'F': *curnibble++ = NA_NIBBLE_F; break;
    default: continue;
    }
    bitcount+=4;
    if(bitcount >= arraycount){break;}
  }
  if(bitcount){
    // Reaching here, the bits array is empty and the clear call is somewhat
    // unneccessary. But we should be consistend anyhow. Just think about what
    // would happen if one day, the implementation of ByteArray changes...
    naClearByteArray(&(bitarray->bits));
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, bitcount);
  }
  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}




NABitArray* naCreateBitArrayFromASCIIString(NABitArray* bitarray,
                                              NAString* string,
                                                  NAInt sizehint){
  NAInt stringsize;
  NAInt arraycount;
  NAInt bitcount = 0;
  const NAUTF8Char* curcharptr;
  NABit* curbit;
  NAInt i;

  bitarray = naAllocateIfNull(bitarray, sizeof(NABitArray));
  stringsize = naGetStringSize(string);
  arraycount = getBitArraySizeHintCount(sizehint, stringsize * 8);
  naCreateBitArrayWithCount(bitarray, -arraycount);
  if(!stringsize){return bitarray;}

  // todo: caching.
  curcharptr = naGetStringConstChar(string, -1);
  curbit = naGetByteArrayMutablePointer(&(bitarray->fullstorage));
  for(i = 0; i < stringsize; i++){
    NAUTF8Char curchar = *curcharptr;
    *curbit++ = (curchar & 0x01) >> 0;
    *curbit++ = (curchar & 0x02) >> 1;
    *curbit++ = (curchar & 0x04) >> 2;
    *curbit++ = (curchar & 0x08) >> 3;
    *curbit++ = (curchar & 0x10) >> 4;
    *curbit++ = (curchar & 0x20) >> 5;
    *curbit++ = (curchar & 0x40) >> 6;
    *curbit++ = (curchar & 0x80) >> 7;
    curcharptr--;
    bitcount += 8;
  }
  
  if(bitcount){
    naClearByteArray(&(bitarray->bits));
    naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, bitcount);
  }

  naEnsureBitArraySizeHint(bitarray, sizehint);
  return bitarray;
}


void naClearBitArray(NABitArray* array){
  naClearByteArray(&(array->bits));
  naClearByteArray(&(array->fullstorage));
}


void naDestroyBitArray(NABitArray* array){
  naClearBitArray(array);
  free(array);
}


void naDecoupleBitArray(NABitArray* bitarray){
  if(naIsByteArrayEmpty(&(bitarray->bits))){
    // This array points to an empty bit array. Make the fullstorage empty
    // as well to be consistent.
    naClearByteArray(&(bitarray->fullstorage));
    naCreateByteArray(&(bitarray->fullstorage));
    return;
  }
  naClearByteArray(&(bitarray->fullstorage));
  naCreateByteArrayExtraction(&(bitarray->fullstorage), &(bitarray->bits), 0, -1);
  naDecoupleByteArray(&(bitarray->fullstorage), NA_FALSE);
  naClearByteArray(&(bitarray->bits));
  naCreateByteArrayExtraction(&(bitarray->bits), &(bitarray->fullstorage), 0, -1);
}







NABit* naGetBitArrayBit(NABitArray* bitarray, NAInt indx){
  #ifndef NDEBUG
    if(!bitarray)
      {naCrash("naGetBitArrayBit", "bitarray is Null-Pointer."); return NA_NULL;}
  #endif
  return naGetByteArrayMutableByte(&(bitarray->bits), indx);
}

NAInt naGetBitArrayCount(NABitArray* bitarray){
  #ifndef NDEBUG
    if(!bitarray)
      {naCrash("naGetBitArrayCount", "bitarray is Null-Pointer."); return 0;}
  #endif
  return naGetByteArraySize(&(bitarray->bits));
}











NAString* naCreateStringDecFromBitArray(NAString* string,
                                    NABitArray* bitarray){
  NAUTF8Char* charptr;
  NAInt outputlen;
  NAInt finalstringcount;
  NABitArray* work;
  NAInt i;
  NAInt j;

  NAInt bitcount = naGetBitArrayCount(bitarray);
  string = naCreateStringWithSize(string, bitcount);
  if(naIsStringEmpty(string)){return string;}
  charptr = naGetStringMutableChar(string, -1);
  outputlen = 0;
  finalstringcount = 0;
  work = naCreateBitArrayExtraction(NULL, bitarray, 0, -1);
  naDecoupleBitArray(work);

  while(bitcount){
    // as long as a remaining value exists
    NABit lead = NA_BIT0;
    NABit bit0;
    NABit bit1;
    NABit bit2;
    NABit bit3;
    uint32 value;
    i = 0;
    j = naGetBitArrayCount(bitarray) - 1;
    while(j >= (bitarray->bits.size - bitcount) + 3){
      // walk through the remaining value
      bit0 = naGetByteArrayConstPointer(&(work->bits))[j-3];
      bit1 = naGetByteArrayConstPointer(&(work->bits))[j-2];
      bit2 = naGetByteArrayConstPointer(&(work->bits))[j-1];
      bit3 = naGetByteArrayConstPointer(&(work->bits))[j-0];
      value = (bit3<<24) | (bit2<<16) | (bit1<<8) | (bit0<<0);
      if(value >= NA_NIBBLE_A){
        // For nibbles greaterequal than the value 10, adjust the bits accordingly.
        naGetByteArrayMutablePointer(&(work->bits))[j-0] = NA_BIT1;  // division result
        naGetByteArrayMutablePointer(&(work->bits))[j-1] = bit2 && bit1;
        naGetByteArrayMutablePointer(&(work->bits))[j-2] = !bit1;
      }else{
        naGetByteArrayMutablePointer(&(work->bits))[j-0] = lead;
        if(lead){
          // When the previous nibble was 8 or 9, adjust the bits accordingly
          naGetByteArrayMutablePointer(&(work->bits))[j-1] = !bit1;
          naGetByteArrayMutablePointer(&(work->bits))[j-2] = !bit1;
          lead = bit1;
        }else{
          // otherwise, just leave the bits as they are.
          if(value >= NA_NIBBLE_8){lead = NA_BIT1;}
        }
      }
      i++;
      j--;
    }
    // extract the decimal value of the remaining bits
    if(bitcount==1){
      bit0 = naGetByteArrayConstPointer(&(work->bits))[bitarray->bits.size - 1];
      bit1 = NA_BIT0;
      bit2 = NA_BIT0;
    }else if(bitcount==2){
      bit0 = naGetByteArrayConstPointer(&(work->bits))[bitarray->bits.size - 2];
      bit1 = naGetByteArrayConstPointer(&(work->bits))[bitarray->bits.size - 1];
      bit2 = NA_BIT0;
    }else{
      bit0 = naGetByteArrayConstPointer(&(work->bits))[j-2];
      bit1 = naGetByteArrayConstPointer(&(work->bits))[j-1];
      bit2 = naGetByteArrayConstPointer(&(work->bits))[j-0];
    }
    bit3 = lead;
    value = (bit3<<3) | (bit2<<2) | (bit1<<1) | (bit0<<0);
    if((outputlen > 0) && !(outputlen%3)){*charptr-- = ' '; finalstringcount++;}
    *charptr-- = value + '0';
    outputlen++;
    finalstringcount++;
    bitcount = i;
  }
  // Remove zeros
  i = naGetStringSize(string) - finalstringcount;
  charptr++;
  while((i < naGetStringSize(string)) && ((*charptr == '0') || (*charptr == ' '))){
    charptr++;
    i++;
  }
  if(i == naGetStringSize(string)){
    naClearString(string);
    naCreateStringWithUTF8CString(string, "0");
  }else{
    naCreateStringExtraction(string, string, i, -1);
  }

  naDestroyBitArray(work);
  return string;
}



NAString* naCreateStringHexFromBitArray(NAString* string,
                                    NABitArray* bitarray){
  NAUTF8Char* charptr;
  NABit* bitptr;
  uint8 nibble;

  NAInt bitcount = naGetBitArrayCount(bitarray);
  NAInt nibblecount = (bitcount % 4)?(bitcount / 4 + 1):(bitcount / 4);
  NAInt delimiters = (nibblecount - 1) / 2;
  string = naCreateStringWithSize(string, nibblecount + delimiters);
  if(naIsStringEmpty(string)){return string;}

  charptr = naGetStringMutableChar(string, 0);
  bitptr = naGetBitArrayBit(bitarray, -1);
  nibble = 0;
  while(bitcount){
    nibble <<= 1;
    nibble |= *bitptr;
    bitptr--;
    bitcount--;
    if(!(bitcount % 4)){
      if(nibble < 10){
        *charptr++ = '0' + nibble;
      }else{
        *charptr++ = 'a' + nibble - 10;
      }
      nibble = 0;
      if(bitcount && !(bitcount % 8)){
        *charptr++ = ' ';
      }
    }
  }
  return string;
}



NAString* naCreateStringBinFromBitArray(NAString* string,
                                    NABitArray* bitarray){
  NAUTF8Char* charptr;
  NABit* bitptr;

  NAInt bitcount = naGetBitArrayCount(bitarray);
  NAInt delimiters = (bitcount - 1) / 8;
  string = naCreateStringWithSize(string, bitcount + delimiters);
  if(naIsStringEmpty(string)){return string;}

  charptr = naGetStringMutableChar(string, 0);
  bitptr = naGetBitArrayBit(bitarray, -1);
  while(bitcount){
    *charptr++ = '0' + *bitptr--;
    bitcount--;
    if(bitcount && !(bitcount % 8)){
      *charptr++ = ' ';
    }
  }
  return string;
}



NAByteArray* naCreateByteArrayFromBitArray(NAByteArray* bytearray,
                                            NABitArray* bitarray){
  NAByte* curbyte;
  NABit* curbit;
  int b;
  NAInt bytecount = naGetBitArrayCount(bitarray) / 8;
  bytearray = naCreateByteArrayWithSize(bytearray, -bytecount);
  if(!bytecount){return bytearray;}
  
  curbyte = naGetByteArrayMutablePointer(bytearray);
  curbit = naGetBitArrayBit(bitarray, (bytecount - 1) * 8) + 7;

  while(bytecount){
    *curbyte = 0;
    
    for(b=0; b<8; b++){
      *curbyte <<= 1;
      *curbyte |= *curbit;
      curbit--;
    }
    
    curbyte++;
    bytecount--;
  }

  return bytearray;
}




NABit naComputeBitArrayAddOne(NABitArray* array){
  NABit carry;
  NABit* curbit;

  NAInt bitcount = naGetBitArrayCount(array);
  if(!bitcount){return NA_BIT1;}

  curbit = naGetByteArrayMutablePointer(&(array->bits));

  carry = NA_BIT1;
  while(carry && bitcount){
    carry = *curbit;
    *curbit ^= NA_BIT1;
    bitcount--;
    curbit++;
  }
  return carry;
}



NABit naComputeBitArrayAddBitArray( NABitArray* dstarray,
                                    NABitArray* srcarray1,
                                    NABitArray* srcarray2){
  NABit* curbitd;
  const NABit* curbit1 = NA_NULL;
  const NABit* curbit2 = NA_NULL;
  NABit carry;  // Note that carry can be 0, 1, 2 or 3 during the computation
                // but will be 0 or 1 in the end.

  NAInt dstcount = naGetByteArraySize(&(dstarray->fullstorage));
  NAInt srccount1 = naGetByteArraySize(&(srcarray1->bits));
  NAInt srccount2 = naGetByteArraySize(&(srcarray2->bits));

  if(!dstcount){return NA_BIT0;}

  // The destination bit pointer will point to a bit with lesser significance
  // than the bits of src1 and src2. Therefore, it is save to say that both
  // src1 and src2 can be the same as dst and might therefore be overwritten
  // during this computation without alienating the result.
  curbitd = naGetByteArrayMutablePointer(&(dstarray->fullstorage));
  if(srccount1){curbit1 = naGetByteArrayConstPointer(&(srcarray1->bits));}
  if(srccount2){curbit2 = naGetByteArrayConstPointer(&(srcarray2->bits));}

  carry = 0;
  
  // Add the two arrays as long as there exist elements in both arrays
  while(dstcount && srccount1 && srccount2){
    carry += *curbit1++;
    carry += *curbit2++;
    srccount1--;
    srccount2--;
    *curbitd++ = carry & NA_BIT1;
    carry >>= 1;
    dstcount--;
  }

  // Reaching here, one of the counts became 0. Whoever it was, src1 will
  // now become the one still possibly having valid bits.
  if(srccount2){srccount1 = srccount2; curbit1 = curbit2;}
  
  // Now, add up the remaining bits.
  while(dstcount && srccount1){
    carry += *curbit1++;
    srccount1--;
    *curbitd++ = carry & NA_BIT1;
    carry >>= 1;
    dstcount--;
  }
  
  // Add the remaining carry, if needed.
  if(dstcount && carry){
    *curbitd = carry & NA_BIT1;
    carry >>= 1;
    dstcount--;
  }
  
  naCreateByteArrayExtraction(&(dstarray->bits), &(dstarray->fullstorage), 0, -1 - dstcount);
  return carry;
}



void naComputeBitArrayMulTen(NABitArray* dstarray, NABitArray* srcarray){
  NABit* curbitd;
  const NABit* curbits;
  NABit carry;
  NABit bit0 = NA_BIT0;
  NABit bit1 = NA_BIT0;
  NABit bit2 = NA_BIT0;

  // todo: only correct if src != dst.
  NAInt dstcount = naGetByteArraySize(&(dstarray->fullstorage));
  NAInt srccount = naGetByteArraySize(&(srcarray->bits));

  if(!dstcount){return;}
  if(!srccount){
    naClearByteArray(&(dstarray->bits));
    naCreateByteArray(&(dstarray->bits));
    return;
  }

  curbitd = naGetByteArrayMutablePointer(&(dstarray->fullstorage));
  curbits = naGetByteArrayConstPointer(&(srcarray->bits));

  // Set the first three bits
  if(dstcount){*curbitd++ = NA_BIT0;  dstcount--;}

  if(srccount){bit0 = *curbits++; srccount--;}
  if(dstcount){*curbitd++ = bit0;     dstcount--;}

  if(srccount){bit1 = bit0; bit0 = *curbits++; srccount--;}
  if(dstcount){*curbitd++ = bit0;     dstcount--;}

  // Now go for the main part
  carry = NA_BIT0;
  while(dstcount && srccount){
    bit2 = bit1;
    bit1 = bit0;
    bit0 = *curbits++;
    srccount--;
    
    carry += bit0 + bit2;
    *curbitd++ = carry & NA_BIT1;
    carry >>= 1;
    dstcount--;
  }

  // Add up the remaining bits.
  if(dstcount){
    bit2 = bit1;
    bit1 = bit0;
    bit0 = NA_BIT0;
    
    carry += bit0 + bit2;
    *curbitd++ = carry & NA_BIT1;
    carry >>= 1;
    dstcount--;
  }

  if(dstcount){
    bit2 = bit1;
//    bit1 = bit0;
    bit0 = NA_BIT0;
    
    carry += bit0 + bit2;
    *curbitd++ = carry & NA_BIT1;
    carry >>= 1;
    dstcount--;
  }

  // Add the remaining carry, if needed.
  if(dstcount && carry){
    *curbitd = carry & NA_BIT1;
    dstcount--;
  }

  naCreateByteArrayExtraction(&(dstarray->bits), &(dstarray->fullstorage), 0, -1 - dstcount);
}



void naComputeBitArrayOnesComplement(NABitArray* array){
  NABit* curbit;
  NAInt bitcount = naGetBitArrayCount(array);
  
  if(!bitcount){return;}

  curbit = naGetByteArrayMutablePointer(&(array->bits));
  while(bitcount){
    *curbit ^= NA_BIT1;
    bitcount--;
    curbit++;
  }
}



void naComputeBitArrayTwosComplement(NABitArray* array){
  naComputeBitArrayOnesComplement(array);
  naComputeBitArrayAddOne(array);
}


void naComputeBitArraySwapBytes(NABitArray* array){
  NABit* srcptr;
  NABit* dstptr;
  #ifndef NDEBUG
    if(!array)
      {naCrash("naComputeBitArraySwapBytes", "array is Null-Pointer."); return;}
    if(naGetByteArraySize(&(array->bits)) % 8)
      naError("naComputeBitArraySwapBytes", "size of bitarray can not be divided by 8.");
  #endif

  if(!naGetBitArrayCount(array)){return;}
  srcptr = naGetBitArrayBit(array, -8);
  dstptr = naGetBitArrayBit(array, 0);
  while(srcptr > dstptr){
    naSwap64(srcptr, dstptr);
    srcptr -= 8;
    dstptr += 8;
  }
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
