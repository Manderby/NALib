
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "../NADeflate.h"
#include "../NAMemory.h"


typedef struct NAHuffmanCodeTree NAHuffmanCodeTree;
struct NAHuffmanCodeTree{
  uint16 alphabetcount;
  uint16* codes;
  uint16* codelengths;
  int32* indextree;
  int32 curindex;
};


NAHuffmanCodeTree* naAllocHuffmanCodeTree(uint16 alphabetcount){
  int16 i;
  NAHuffmanCodeTree* tree = naAlloc(NAHuffmanCodeTree);
  tree->alphabetcount = alphabetcount;
  tree->codes = naMalloc(alphabetcount * sizeof(uint16));
  tree->codelengths = naMalloc(alphabetcount * sizeof(uint16));
  tree->indextree = naMalloc((2 * alphabetcount - 1) * sizeof(int32));
  for(i=0; i<alphabetcount; i++){tree->codelengths[i] = 0;}
//  for(int16 i=0; i<(2 * alphabetcount - 1); i++){tree->indextree[i] = 0;}
  return tree;
}


void naDeallocHuffmanCodeTree(NAHuffmanCodeTree* tree){
  naFree(tree->codes);
  naFree(tree->codelengths);
  naFree(tree->indextree);
}


void naBuildHuffmanCodeTree(NAHuffmanCodeTree* tree){
  uint16 codelengthcount[17] = {0};
  uint16 nextcodes[17] = {0};
  uint16 a;
  uint16 code;
  uint16 len;
  int32  curindex;
  uint16 newtreeindex;
  uint16 curmask;
  #ifndef NDEBUG
    int32 openends;
  #endif

  // Count the number of code lenghts
  for(a=0; a<tree->alphabetcount; a++){
    #ifndef NDEBUG
      // Literals of the alphabet which are not in use have a codelength
      // of 0 and therefore, we need to allow that value.
//      if(tree->codelengths[a] == 0)
//        naError("naBuildHuffmanCodeTree", "Found a codelength of 0");
      if(tree->codelengths[a] > 16)
        naError("naBuildHuffmanCodeTree", "Found a codelength of more than 16");
    #endif
    codelengthcount[tree->codelengths[a]]++;
  }
  

  // Prepare the next bit codes per codelength
  code = 0;
  codelengthcount[0] = 0;
  for(a = 1; a < 17; a++) {
    #ifndef NDEBUG
      if(codelengthcount[a] > (1<<a))
        // Note that the case of having exaclty 1<<a codelenghts can actually
        // occur. For example with the fixed huffman codes.
        naError("naBuildHuffmanCodeTree", "Too many codelenghts of the same lenght");
    #endif
    code = (uint16)((code + codelengthcount[a - 1]) << 1);
    if(codelengthcount[a]){ // this if is not necessary but is easier to debug
      nextcodes[a] = code;
    }
  }
  
  // Compute the actual codes of the alphabet
  for(a = 0; a < tree->alphabetcount; a++) {
    if(tree->codelengths[a]){
      #ifndef NDEBUG
        if(tree->codelengths[a] > 16)
          naError("naBuildHuffmanCodeTree", "Suddenly found a codelength of more than 16");
      #endif
      tree->codes[a] = nextcodes[tree->codelengths[a]];
      nextcodes[tree->codelengths[a]]++;
    }
  }
  
  // Create the tree out of the codes.
  #ifndef NDEBUG
    openends = 1;
  #endif
  tree->indextree[0] = 0;
  newtreeindex = 1;
  for(a = 0; a < tree->alphabetcount; a++){
    code = tree->codes[a];
    len = tree->codelengths[a];
    if(!len){continue;}
    curmask = (uint16)(1 << (len - 1));
    curindex = 0;
    while(curmask){
      // If there is no entry in the tree, create two branches.
      if(tree->indextree[curindex] == 0){
        #ifndef NDEBUG
          if(newtreeindex >= (2 * tree->alphabetcount - 1))
            naError("naBuildHuffmanCodeTree", "tree index suddenly overflows");
        #endif
        tree->indextree[curindex] = newtreeindex;
        tree->indextree[newtreeindex] = 0;
        tree->indextree[newtreeindex+1] = 0;
        newtreeindex += 2;
        #ifndef NDEBUG
          openends++;
        #endif
      }

      curindex = tree->indextree[curindex];

      if(code & curmask){curindex++;}

      curmask >>= 1;
    }
    #ifndef NDEBUG
      if(curindex >= (2 * tree->alphabetcount - 1))
        naError("naBuildHuffmanCodeTree", "tree index overflow");
    #endif
    // reaching here, curindex denotes the position of where the alphabet must
    // be stored. We store the alphabet as negative value.
    tree->indextree[curindex] = -1 - (int32)a;
    #ifndef NDEBUG
      if(tree->indextree[curindex] >= 0)
        naError("naBuildHuffmanCodeTree", "code can not be stored as a negative number");
    #endif

    #ifndef NDEBUG
      openends --;
      if(openends < 0)
        naError("naBuildHuffmanCodeTree", "Tree overfilled");
    #endif

  }

  #ifndef NDEBUG
    if(openends != 0)
      naError("naBuildHuffmanCodeTree", "Tree incomplete");

    for(a = 0; a < newtreeindex; a++) {
      if(tree->indextree[a] == 0)
        naError("naBuildHuffmanCodeTree", "undefined index in tree");
//      printf("%d,", tree->indextree[a]);
    }
//    printf("\n");
  #endif

}



void naResetHuffmanCodeTree(NAHuffmanCodeTree* tree){
  tree->curindex = 0;
}



NABool naTraverseHuffmanCodeTree(NAHuffmanCodeTree* tree, NABool bit, uint16* alphabet){
  tree->curindex = tree->indextree[tree->curindex];
  if(bit){tree->curindex++;}
  *alphabet = (uint16)(-(tree->indextree[tree->curindex] + 1));
  return (tree->indextree[tree->curindex] >= 0);
}



uint16 naDecodeHuffman(NAHuffmanCodeTree* tree, NABuffer* buffer){
  uint16 symbol;
  naResetHuffmanCodeTree(tree);
  while(naTraverseHuffmanCodeTree(tree, naReadBufferBit(buffer), &symbol)){}
  return symbol;
}



NAHuffmanCodeTree* naReadCodeLengthHuffman(NAHuffmanCodeTree* codelengthhuffman, NABuffer* zbuffer, uint16 alphabetcount){
  NAHuffmanCodeTree* alphabethuffman = naAllocHuffmanCodeTree(alphabetcount);
  uint16 curalphabetcount = 0;
  while(curalphabetcount < alphabetcount){
    uint16 codelength;
    uint16 repeatcount;
    uint16 i;
    codelength = naDecodeHuffman(codelengthhuffman, zbuffer);
//    printf(", %d", codelength);
    switch(codelength){
    case 16:
      repeatcount = (uint16)naReadBufferBits(zbuffer, 2) + 3;
      #ifndef NDEBUG
        if((curalphabetcount + repeatcount) > alphabetcount)
          naError("naReadCodeLengthHuffman", "Internal Error: Reading too many literals at codelength 16");
      #endif
      for(i=0; i<repeatcount; i++){alphabethuffman->codelengths[curalphabetcount + i] = alphabethuffman->codelengths[curalphabetcount - 1];}
//      printf("(%d)", repeatcount);
      curalphabetcount += repeatcount;
      break;
    case 17:
      repeatcount = (uint16)naReadBufferBits(zbuffer, 3) + 3;
      #ifndef NDEBUG
        if((curalphabetcount + repeatcount) > alphabetcount)
          naError("naReadCodeLengthHuffman", "Internal Error: Reading too many literals at codelength 17");
      #endif
      for(i=0; i<repeatcount; i++){alphabethuffman->codelengths[curalphabetcount + i] = 0;}
//      printf("(%d)", repeatcount);
      curalphabetcount += repeatcount;
      break;
    case 18:
      repeatcount =(uint16) naReadBufferBits(zbuffer, 7) + 11;
      #ifndef NDEBUG
        if((curalphabetcount + repeatcount) > alphabetcount)
          naError("naReadCodeLengthHuffman", "Internal Error: Reading too many literals at codelength 18");
      #endif
      for(i=0; i<repeatcount; i++){alphabethuffman->codelengths[curalphabetcount + i] = 0;}
//      printf("(%d)", repeatcount);
      curalphabetcount += repeatcount;
      break;
    default:
      alphabethuffman->codelengths[curalphabetcount] = codelength;
      curalphabetcount++;
      break;
    }
  }
//  printf("\n");
  
  naBuildHuffmanCodeTree(alphabethuffman);
  return alphabethuffman;
}



uint16 naDecodeLiteralLength(NABuffer* zbuffer, uint16 code){
  switch(code){
  case 257: return 3; break;
  case 258: return 4; break;
  case 259: return 5; break;
  case 260: return 6; break;
  case 261: return 7; break;
  case 262: return 8; break;
  case 263: return 9; break;
  case 264: return 10; break;
  case 265: return 11 + (uint16)naReadBufferBits(zbuffer, 1); break;
  case 266: return 13 + (uint16)naReadBufferBits(zbuffer, 1); break;
  case 267: return 15 + (uint16)naReadBufferBits(zbuffer, 1); break;
  case 268: return 17 + (uint16)naReadBufferBits(zbuffer, 1); break;
  case 269: return 19 + (uint16)naReadBufferBits(zbuffer, 2); break;
  case 270: return 23 + (uint16)naReadBufferBits(zbuffer, 2); break;
  case 271: return 27 + (uint16)naReadBufferBits(zbuffer, 2); break;
  case 272: return 31 + (uint16)naReadBufferBits(zbuffer, 2); break;
  case 273: return 35 + (uint16)naReadBufferBits(zbuffer, 3); break;
  case 274: return 43 + (uint16)naReadBufferBits(zbuffer, 3); break;
  case 275: return 51 + (uint16)naReadBufferBits(zbuffer, 3); break;
  case 276: return 59 + (uint16)naReadBufferBits(zbuffer, 3); break;
  case 277: return 67 + (uint16)naReadBufferBits(zbuffer, 4); break;
  case 278: return 83 + (uint16)naReadBufferBits(zbuffer, 4); break;
  case 279: return 99 + (uint16)naReadBufferBits(zbuffer, 4); break;
  case 280: return 115 + (uint16)naReadBufferBits(zbuffer, 4); break;
  case 281: return 131 + (uint16)naReadBufferBits(zbuffer, 5); break;
  case 282: return 163 + (uint16)naReadBufferBits(zbuffer, 5); break;
  case 283: return 195 + (uint16)naReadBufferBits(zbuffer, 5); break;
  case 284: return 227 + (uint16)naReadBufferBits(zbuffer, 5); break;
  case 285: return 258; break;
  default:
    #ifndef NDEBUG
      naError("naDecodeLiteralLength", "Invalid literal length code receyved");
    #endif
    return 0;
    break;
  }
}


uint16 naDecodeDistance(NABuffer* zbuffer, uint16 code){
  switch(code){
  case 0: return 1; break;
  case 1: return 2; break;
  case 2: return 3; break;
  case 3: return 4; break;
  case 4: return 5 + (uint16)naReadBufferBits(zbuffer, 1); break;
  case 5: return 7 + (uint16)naReadBufferBits(zbuffer, 1); break;
  case 6: return 9 + (uint16)naReadBufferBits(zbuffer, 2); break;
  case 7: return 13 + (uint16)naReadBufferBits(zbuffer, 2); break;
  case 8: return 17 + (uint16)naReadBufferBits(zbuffer, 3); break;
  case 9: return 25 + (uint16)naReadBufferBits(zbuffer, 3); break;
  case 10: return 33 + (uint16)naReadBufferBits(zbuffer, 4); break;
  case 11: return 49 + (uint16)naReadBufferBits(zbuffer, 4); break;
  case 12: return 65 + (uint16)naReadBufferBits(zbuffer, 5); break;
  case 13: return 97 + (uint16)naReadBufferBits(zbuffer, 5); break;
  case 14: return 129 + (uint16)naReadBufferBits(zbuffer, 6); break;
  case 15: return 193 + (uint16)naReadBufferBits(zbuffer, 6); break;
  case 16: return 257 + (uint16)naReadBufferBits(zbuffer, 7); break;
  case 17: return 385 + (uint16)naReadBufferBits(zbuffer, 7); break;
  case 18: return 513 + (uint16)naReadBufferBits(zbuffer, 8); break;
  case 19: return 769 + (uint16)naReadBufferBits(zbuffer, 8); break;
  case 20: return 1025 + (uint16)naReadBufferBits(zbuffer, 9); break;
  case 21: return 1537 + (uint16)naReadBufferBits(zbuffer, 9); break;
  case 22: return 2049 + (uint16)naReadBufferBits(zbuffer, 10); break;
  case 23: return 3073 + (uint16)naReadBufferBits(zbuffer, 10); break;
  case 24: return 4097 + (uint16)naReadBufferBits(zbuffer, 11); break;
  case 25: return 6145 + (uint16)naReadBufferBits(zbuffer, 11); break;
  case 26: return 8193 + (uint16)naReadBufferBits(zbuffer, 12); break;
  case 27: return 12289 + (uint16)naReadBufferBits(zbuffer, 12); break;
  case 28: return 16385 + (uint16)naReadBufferBits(zbuffer, 13); break;
  case 29: return 24577 + (uint16)naReadBufferBits(zbuffer, 13); break;
  default:
    #ifndef NDEBUG
      naError("naDecodeDistance", "Invalid distance code receyved");
    #endif
    return 0;
    break;
  }
}



NA_HDEF void naReadDymanicHuffmanCodes(NABuffer* zbuffer, NAHuffmanCodeTree** literalhuffman, NAHuffmanCodeTree** distancehuffman){
  uint8 codeorder[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
  int c;

  uint16 hlit = (uint16)naReadBufferBits(zbuffer, 5) + 257;
  uint8 hdist = (uint8)naReadBufferBits(zbuffer, 5) + 1;
  uint8 hclen = (uint8)naReadBufferBits(zbuffer, 4) + 4;
  
//  naAlloc(NABuffer);
  NAHuffmanCodeTree* codelengthhuffman = naAllocHuffmanCodeTree(19);
  for(c=0; c<hclen; c++){
    codelengthhuffman->codelengths[codeorder[c]] = (uint16)naReadBufferBits(zbuffer, 3);
//    printf("(%d, %d),", codeorder[c], codelengthhuffman->codelengths[codeorder[c]]);
  }
//  printf("\n");
  naBuildHuffmanCodeTree(codelengthhuffman);
  
  *literalhuffman = naReadCodeLengthHuffman(codelengthhuffman, zbuffer, hlit);
  *distancehuffman = naReadCodeLengthHuffman(codelengthhuffman, zbuffer, hdist);

  // Now, the literal and distance codes are read.
  naDeallocHuffmanCodeTree(codelengthhuffman);

}



NA_HDEF void naCreateFixedHuffmanCodes(NAHuffmanCodeTree** literalhuffman, NAHuffmanCodeTree** distancehuffman){
  uint16 i;
  
  *literalhuffman = naAllocHuffmanCodeTree(288);
  for(i=0; i<=143; i++){
    (*literalhuffman)->codelengths[i] = 8;
  }
  for(i=144; i<=255; i++){
    (*literalhuffman)->codelengths[i] = 9;
  }
  for(i=256; i<=279; i++){
    (*literalhuffman)->codelengths[i] = 7;
  }
  for(i=280; i<=287; i++){
    (*literalhuffman)->codelengths[i] = 8;
  }
  naBuildHuffmanCodeTree(*literalhuffman);

  // Note that only the values up to 29 are used but we fill up the remaining
  // thre values anyway as otherwise, the huffman tree will be incomplete.
  *distancehuffman = naAllocHuffmanCodeTree(32);
  for(i=0; i<32; i++){
    (*distancehuffman)->codelengths[i] = 5;
  }
  naBuildHuffmanCodeTree(*distancehuffman);
}


#define NA_ZLIB_CMF_COMPRESSION_DEFLATE 8
#define NA_ZLIB_CMF_MAX_WINDOW_SIZE 7     // 2 ^ MAX_WINDOWSIZE + 8


NA_DEF void naFillBufferWithZLIBDecompression(NABuffer* output, NABuffer* input){
  uint8 compressionmethodflags;
  uint8 compressionmethod;
  uint8 compressioninfo;
  uint16 windowsize;
  uint8 compressionadditionalflags;
  uint32 flagcheck;
  NABool haspresetdict;
  NADeflateCompressionLevel compressionlevel;
  uint32 dictadler;
  NABuffer* zbuffer;
  uint32 zbufferadler;
  NAChecksum checksum;
  uint32 adler;

  // First, read RFC 1950
  NAInt zbuffersize = naGetBufferRange(input).length - 6;
  // The 6 Bytes are the CMF and FLG Bytes as well as the Adler number.
  // If there is a DICTID, zbuffersize will be reduced by 4 more bytes later.
  
  #ifndef NDEBUG
    if(naGetBufferEndianness(input) != NA_ENDIANNESS_NETWORK)
      naError("naInitBufferFromDeflateDecompression", "Input buffer should be big endianed");
  #endif
  compressionmethodflags = naReadBufferu8(input);
  compressionmethod = compressionmethodflags & 0x0f;
  #ifndef NDEBUG
    if(compressionmethod != NA_ZLIB_CMF_COMPRESSION_DEFLATE)
      naError("naInitBufferFromDeflateDecompression", "Compression method of Deflate buffer unknown");
  #endif
  compressioninfo = (compressionmethodflags & 0xf0) >> 4;
  #ifndef NDEBUG
    if(compressioninfo > NA_ZLIB_CMF_MAX_WINDOW_SIZE)
      naError("naInitBufferFromDeflateDecompression", "Window size too big");
  #endif
  windowsize = (uint16)(1 << (compressioninfo + 8));
  compressionadditionalflags = naReadBufferu8(input);
  flagcheck = compressionmethodflags * 256 + compressionadditionalflags;
  #ifndef NDEBUG
    if(flagcheck % 31)
      naError("naInitBufferFromDeflateDecompression", "Flag check failed");
  #endif
  haspresetdict = (compressionadditionalflags & (1 << 5)) ? NA_TRUE : NA_FALSE;
  compressionlevel = (NADeflateCompressionLevel) ((compressionadditionalflags >> 6) & 0x03);

  NA_UNUSED(windowsize);
  NA_UNUSED(compressionlevel);
  #ifndef NDEBUG
  #else
    NA_UNUSED(compressionmethod);
    NA_UNUSED(flagcheck);
  #endif

  if(haspresetdict){
    dictadler = naReadBufferu32(input);
    zbuffersize -= 4;
  }
  NA_UNUSED(dictadler);

  zbuffer = naCreateBufferExtraction(input, naMakeRangei(naTellBuffer(input), zbuffersize));
  naSeekBufferRelative(input, zbuffersize);
  zbufferadler = naReadBufferu32(input);
  
  // Now start RFC 1951

  // Important! RFC 1951 is Little-endianed, whereas RFC 1950 is big endianed!
  naSetBufferEndianness(zbuffer, NA_ENDIANNESS_LITTLE);
  
  while(1){
    NAByte isblockfinal = (NAByte)naReadBufferBits(zbuffer, 1);
    NAByte blocktype = (NAByte)naReadBufferBits(zbuffer, 2);
    #ifndef NDEBUG
      if(blocktype == 0x03)
      naError("naInitBufferFromDeflateDecompression", "Block compression invalid");
    #endif
    if(blocktype == 0x00){
      // Blocktype 0x00 is an uncompressed block.
      uint16 len;
      uint16 nlen;
      NAByte* tmpbuf;

      naPadBufferBits(zbuffer);
      len = naReadBufferu16(zbuffer);
      nlen = naReadBufferu16(zbuffer);
      #ifndef NDEBUG
        if((uint16)len != (uint16)(~nlen))
        naError("naInitBufferFromDeflateDecompression", "Len and NLen do not match in one's complement");
      #else
        NA_UNUSED(nlen);
      #endif
      // the len denotes the number of bytes in the whole block. Todo: Check
      // how this behaves if the first three bits are not at the first bit
      // position.
      tmpbuf = naMalloc(len);
      naReadBufferBytes(zbuffer, tmpbuf, len);
      naWriteBufferBytes(output, tmpbuf, len);
      naFree(tmpbuf);
    }else{
      NAHuffmanCodeTree* literalhuffman;
      NAHuffmanCodeTree* distancehuffman;
      if(blocktype == 0x02){
        naReadDymanicHuffmanCodes(zbuffer, &literalhuffman, &distancehuffman);
      }else{
        naCreateFixedHuffmanCodes(&literalhuffman, &distancehuffman);
      }
      // Now, we have the literal and distance codes. Start reading until we
      // find the value 256.
      while(1){
      
        uint16 curcode;
        uint16 length;
        uint16 dist;
        curcode = naDecodeHuffman(literalhuffman, zbuffer);
        if(curcode < 256){
          naWriteBufferu8(output, (uint8)curcode);
          //printf("%u\n", (uint32)curcode);
        }else if(curcode == 256){
          break;
        }else{
          uint16 distcode;
          length = naDecodeLiteralLength(zbuffer, curcode);
          distcode = naDecodeHuffman(distancehuffman, zbuffer);
          dist = naDecodeDistance(zbuffer, distcode);
          naRepeatBufferBytes(output, dist, length);
          //printf("(d %d l %d)\n", dist, length);
        }
      }
      
      naDeallocHuffmanCodeTree(literalhuffman);
      naDeallocHuffmanCodeTree(distancehuffman);
    }
    if(isblockfinal){break;}
//    printf("Another block\n");
  }
  
  naReleaseBuffer(zbuffer);
  
  naFixBufferRange(output);
//  naExpandBufferRangeToSource(output);
  naInitChecksum(&checksum, NA_CHECKSUM_TYPE_ADLER_32);
  naAccumulateBufferToChecksum(output, &checksum);
  adler = naGetChecksumResult(&checksum);
  naClearChecksum(&checksum);
  
  #ifndef NDEBUG
    if(adler != zbufferadler)
      naError("naInitBufferFromDeflateDecompression", "Adler code does not correspond to decompression result");
  #else
    NA_UNUSED(zbufferadler);
    NA_UNUSED(adler);
  #endif
}



#define NA_ZLIB_PRESET_DICT_AVAILABLE 0

NA_DEF void naFillBufferWithZLIBCompression(NABuffer* buffer, NABuffer* input, NADeflateCompressionLevel level){

  uint8 cmf;
  uint8 flg;
  NAInt bytesize;
  NAChecksum checksum;
  uint32 adler;
  
  #ifndef NDEBUG
    if(naGetBufferEndianness(buffer) != NA_ENDIANNESS_NETWORK)
      naError("naInitBufferFromDeflateDecompression", "Output buffer should be big endianed");
    if(naGetBufferEndianness(input) != NA_ENDIANNESS_NETWORK)
      naError("naInitBufferFromDeflateDecompression", "Input buffer should be big endianed");
  #endif

  cmf = (NA_ZLIB_CMF_MAX_WINDOW_SIZE<<4 | NA_ZLIB_CMF_COMPRESSION_DEFLATE);
  flg = (uint8)((level << 6 | NA_ZLIB_PRESET_DICT_AVAILABLE << 5));
  flg |= 31 - ((cmf * 256 + flg) % 31); // Check-bits
  naWriteBufferu8(buffer, cmf);
  naWriteBufferu8(buffer, flg);

  // Now, for the actual content, we change to little endian due to RFC 1951!
  naSetBufferEndianness(buffer, NA_ENDIANNESS_LITTLE);
  
  bytesize = naGetBufferRange(input).length;
  naSeekBufferAbsolute(input, 0);
  
  
  NAInt curoffset = 0;
  while(bytesize > 0){
    uint16 curbytesize;
    NAByte headbyte = (0 << 1);
    if(bytesize >= (1<<15)){
      curbytesize = (1<<15) - 1;
    }else{
      curbytesize = (uint16)bytesize;
      headbyte |= 1;
    }
    naWriteBufferu8(buffer, headbyte);
    naWriteBufferu16(buffer, curbytesize);
    naWriteBufferu16(buffer, ~curbytesize);
    naWriteBufferBuffer(buffer, input, naMakeRangei(curoffset, curbytesize));
    bytesize -= curbytesize;
    curoffset += curbytesize;
  }
  
  // We write the adler number. Note that this must be in network byte order
  // again as it belongs to RFC 1950!
  naSetBufferEndianness(buffer, NA_ENDIANNESS_NETWORK);
  naInitChecksum(&checksum, NA_CHECKSUM_TYPE_ADLER_32);
  naSeekBufferAbsolute(input, 0);
  naAccumulateBufferToChecksum(input, &checksum);
  adler = naGetChecksumResult(&checksum);
  naClearChecksum(&checksum);
  naWriteBufferu32(buffer, adler);
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
