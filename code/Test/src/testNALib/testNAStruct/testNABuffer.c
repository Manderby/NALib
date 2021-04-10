
#include "NATesting.h"
#include <stdio.h>

#include "NABuffer.h"


void testMemoryBlock(){
  naTestGroup("New and release"){
    NAMemoryBlock* block = NA_NULL;
    naTestVoid(block = na_NewMemoryBlock(10));
    naTestVoid(naRelease(block));
    //naTestCrash(block = na_NewMemoryBlock(0));
    naRelease(block);
  }

  naTestGroup("New and release with data"){
    int constData = 1234;
    NAPtr constPtr = naMakePtrWithDataConst(&constData);
    NAPtr mutablePtr = naMakePtrWithDataMutable(naAlloc(int));
    NAMemoryBlock* block = NA_NULL;

    naTestVoid(block = na_NewMemoryBlockWithData(constPtr, sizeof(int), NA_NULL));
    naTestVoid(naRelease(block));
    naTestVoid(block = na_NewMemoryBlockWithData(mutablePtr, sizeof(int), naFree));
    naRelease(block);

    naTestError(block = na_NewMemoryBlockWithData(naMakePtrNull(), sizeof(int), NA_NULL));
    naRelease(block);
    naTestError(block = na_NewMemoryBlockWithData(constPtr, 0, NA_NULL));
    naRelease(block);
    //naTestError(block = na_NewMemoryBlockWithData(constPtr, sizeof(int), naFree));
    //naTestCrash(naRelease(block));
  }

  naTestGroup("Accessing and Mutating"){
    NAPtr mutablePtr = naMakePtrWithDataMutable(naAlloc(int));
    NAMemoryBlock* block = na_NewMemoryBlockWithData(mutablePtr, sizeof(int), naFree);
    naTest(na_GetMemoryBlockDataPointerConst(block, 0) != NULL);
    naTest(na_GetMemoryBlockDataPointerConst(block, sizeof(int) - 1) != NULL);
    naTest(na_GetMemoryBlockDataPointerMutable(block, 0) != NULL);
    naTest(na_GetMemoryBlockDataPointerMutable(block, sizeof(int) - 1) != NULL);
    //naTestCrash(na_GetMemoryBlockDataPointerConst(NA_NULL, 0));
    naTestError(na_GetMemoryBlockDataPointerConst(block, 1000));
    //naTestCrash(na_GetMemoryBlockDataPointerMutable(NA_NULL, 0));
    naTestError(na_GetMemoryBlockDataPointerMutable(block, 1000));
    naRelease(block);
  }
}



// dummy function to fill a buffer source
void na_DummyBufferFiller(void* dst, NARangei sourceRange, void* sourceData){
}

void testBufferSource(){
  naTestGroup("New and release"){
    NABuffer* cache = naNewBuffer(NA_FALSE);
    NABufferSource* source = NA_NULL;
    naTestVoid(source = naNewBufferSource(NA_NULL, NA_NULL));
    naTestVoid(naRelease(source));
    naTestVoid(source = naNewBufferSource(na_DummyBufferFiller, NA_NULL));
    naRelease(source);
    naTestVoid(source = naNewBufferSource(NA_NULL, cache));
    naRelease(source);
    naTestVoid(source = naNewBufferSource(na_DummyBufferFiller, cache));
    naRelease(source);

    naRelease(cache);
  }

  naTestGroup("Set data"){
    int* data = naAlloc(int);
    NABufferSource* fillingSource = naNewBufferSource(na_DummyBufferFiller, NA_NULL);
    naTestVoid(naSetBufferSourceData(fillingSource, data, naFree));
    naTestVoid(naRelease(fillingSource));

    NABufferSource* emptySource = naNewBufferSource(NA_NULL, NA_NULL);
    data = naAlloc(int);
    // source is Null:
    //naTestCrash(naSetBufferSourceData(NA_NULL, data, naFree));
    // setting Null data
    naTestError(naSetBufferSourceData(emptySource, NA_NULL, naFree));
    // setting data to non filling source:
    naTestError(naSetBufferSourceData(emptySource, data, naFree));
    naRelease(emptySource);

    // setting data twice:
    fillingSource = naNewBufferSource(na_DummyBufferFiller, NA_NULL);
    data = naAlloc(int);
    naSetBufferSourceData(fillingSource, data, naFree);
    naTestError(naSetBufferSourceData(fillingSource, data, naFree));
    naRelease(fillingSource);
  }

  naTestGroup("Set limit"){
    NABufferSource* source = naNewBufferSource(NA_NULL, NA_NULL);
    naTestVoid(naSetBufferSourceLimit(source, naMakeRangei(0, 10)));
    // trying to set it twice:
    naTestError(naSetBufferSourceLimit(source, naMakeRangei(0, 10)));
    naRelease(source);

    source = naNewBufferSource(NA_NULL, NA_NULL);
    // trying to set a bad range
    naTestError(naSetBufferSourceLimit(source, naMakeRangei(0, 0)));
    // source is Null:
    //naTestCrash(naSetBufferSourceLimit(NA_NULL, naMakeRangei(0, 10)));

    naRelease(source);
  }

  naTestGroup("Accessors"){
    NABufferSource* source = naNewBufferSource(na_DummyBufferFiller, NA_NULL);
    naTest(!na_HasBufferSourceCache(source));
    naRelease(source);

    NABuffer* cache = naNewBuffer(NA_FALSE);
    source = naNewBufferSource(na_DummyBufferFiller, cache);
    naTest(na_HasBufferSourceCache(source));
    naTest(na_GetBufferSourceCache(source) == cache);
    naRelease(cache);

    naTest(!na_HasBufferSourceLimit(source));
    naSetBufferSourceLimit(source, naMakeRangei(0, 10));
    naTest(na_HasBufferSourceLimit(source));
    naTest(naEqualRangei(na_GetBufferSourceLimit(source), naMakeRangei(0, 10)));
    naRelease(source);

    //naTestCrash(na_HasBufferSourceCache(NA_NULL));
    //naTestCrash(na_GetBufferSourceCache(NA_NULL));
    //naTestCrash(na_HasBufferSourceLimit(NA_NULL));
    //naTestCrash(na_GetBufferSourceLimit(NA_NULL));
  }

  naTestGroup("Filling data"){
    NAByte buf[10];

    NABufferSource* source = naNewBufferSource(NA_NULL, NA_NULL);
    naTestVoid(na_FillBufferSourceMemory(source, buf, naMakeRangei(0, 10)));
    naSetBufferSourceLimit(source, naMakeRangei(0, 10));
    naTestVoid(na_FillBufferSourceMemory(source, buf, naMakeRangei(0, 10)));
    naTestError(na_FillBufferSourceMemory(source, buf, naMakeRangei(0, 11)));
    naRelease(source);

    source = naNewBufferSource(na_DummyBufferFiller, NA_NULL);
    naTestVoid(na_FillBufferSourceMemory(source, buf, naMakeRangei(0, 10)));

    //naTestCrash(na_FillBufferSourceMemory(NA_NULL, buf, naMakeRangei(0, 10)));
    //naTestCrash(na_FillBufferSourceMemory(source, NA_NULL, naMakeRangei(0, 10)));
    naTestError(na_FillBufferSourceMemory(source, buf, naMakeRangei(0, 0)));
    naRelease(source);
  }
}


  
void testBufferPart(){
  NABufferSource* source = naNewBufferSource(NA_NULL, NA_NULL);

  naTestGroup("Normed start and end"){
    naTest(na_GetBufferPartNormedStart(0) == 0);
    naTest(na_GetBufferPartNormedStart(NA_INTERNAL_BUFFER_PART_BYTESIZE - 1) == 0);
    naTest(na_GetBufferPartNormedStart(NA_INTERNAL_BUFFER_PART_BYTESIZE) == NA_INTERNAL_BUFFER_PART_BYTESIZE);
    naTest(na_GetBufferPartNormedStart(-1) == -(NAInt)NA_INTERNAL_BUFFER_PART_BYTESIZE);
    naTest(na_GetBufferPartNormedStart(-(NAInt)NA_INTERNAL_BUFFER_PART_BYTESIZE + 1) == -NA_INTERNAL_BUFFER_PART_BYTESIZE);
    naTest(na_GetBufferPartNormedStart(-(NAInt)NA_INTERNAL_BUFFER_PART_BYTESIZE) == -NA_INTERNAL_BUFFER_PART_BYTESIZE);

    naTest(na_GetBufferPartNormedEnd(0) == 0);
    naTest(na_GetBufferPartNormedEnd(1) == NA_INTERNAL_BUFFER_PART_BYTESIZE);
    naTest(na_GetBufferPartNormedEnd(NA_INTERNAL_BUFFER_PART_BYTESIZE) == NA_INTERNAL_BUFFER_PART_BYTESIZE);
    naTest(na_GetBufferPartNormedEnd(-NA_INTERNAL_BUFFER_PART_BYTESIZE + 1) == 0);
    naTest(na_GetBufferPartNormedEnd(-NA_INTERNAL_BUFFER_PART_BYTESIZE) == -NA_INTERNAL_BUFFER_PART_BYTESIZE);
  }

  naTestGroup("New and delete sparse part"){
    NABufferPart* part = NA_NULL;
    naTestVoid(part = na_NewBufferPartSparse(source, naMakeRangei(0, 1)));
    naTestVoid(naDelete(part));

    // no source
    //naTestError(part = na_NewBufferPartSparse(NA_NULL, naMakeRangei(0, 1)));
    //naDelete(part);
    // no useful range
    naTestError(part = na_NewBufferPartSparse(source, naMakeRangei(0, 0)));
    naDelete(part);
  }

  naTestGroup("New and delete data part"){
    NABufferPart* part = NA_NULL;
    NAByte dataConst[] = {0, 1, 2, 3};
    naTestVoid(part = na_NewBufferPartWithConstData(dataConst, 4));
    naTestVoid(naDelete(part));
    NAByte* dataMutable = naMalloc(4);
    naTestVoid(part = na_NewBufferPartWithMutableData(dataMutable, 4, naFree));
    naTestVoid(naDelete(part));
    naTestVoid(part = na_NewBufferPartWithMutableData(dataConst, 4, NA_NULL));
    naTestVoid(naDelete(part));

    naTestError(part = na_NewBufferPartWithConstData(NA_NULL, 4));
    naDelete(part);
    naTestError(part = na_NewBufferPartWithMutableData(NA_NULL, 4, NA_NULL));
    naDelete(part);
    naTestError(part = na_NewBufferPartWithConstData(dataConst, 0));
    naDelete(part);
    naTestError(part = na_NewBufferPartWithMutableData(dataMutable, 0, NA_NULL));
    naDelete(part);
  }

  naTestGroup("Accessors"){
    NAByte dataConst[] = {0, 1, 2, 3};
    NABufferPart* sparsePart = na_NewBufferPartSparse(source, naMakeRangei(2, 8));
    NABufferPart* dataPart = na_NewBufferPartWithConstData(dataConst, 4);
    naTest(na_GetBufferPartSource(sparsePart) == source);
    naTest(na_GetBufferPartSource(dataPart) == NA_NULL);
    naTest(na_GetBufferPartSourceOffset(sparsePart) == 2);
    naTest(na_GetBufferPartByteSize(sparsePart) == 8);
    naTest(na_GetBufferPartByteSize(dataPart) == 4);
    naTest(na_IsBufferPartSparse(sparsePart) == NA_TRUE);
    naTest(na_IsBufferPartSparse(dataPart) == NA_FALSE);
    naTest(na_GetBufferPartMemoryBlock(dataPart) != NA_NULL);

    naTestCrash(na_GetBufferPartSource(NA_NULL));
    naTestCrash(na_GetBufferPartSourceOffset(NA_NULL));
    naTestCrash(na_GetBufferPartByteSize(NA_NULL));
    naTestCrash(na_IsBufferPartSparse(NA_NULL));
    naTestCrash(na_GetBufferPartMemoryBlock(NA_NULL));
    naTestError(na_GetBufferPartSourceOffset(dataPart));
    naTestError(na_GetBufferPartMemoryBlock(sparsePart));
    naDelete(dataPart);
    naDelete(sparsePart);
  }

  naTestGroup("Part enlarging"){
    NABufferPart* sparsePart = na_NewBufferPartSparse(source, naMakeRangei(2, 6));
    naTestVoid(na_EnlargeBufferPart(sparsePart, 0, 0));
    naTest(na_GetBufferPartSourceOffset(sparsePart) == 2);
    naTest(na_GetBufferPartByteSize(sparsePart) == 6);
    naTestVoid(na_EnlargeBufferPart(sparsePart, 5, 7));
    naTest(na_GetBufferPartSourceOffset(sparsePart) == -3);
    naTest(na_GetBufferPartByteSize(sparsePart) == 18);

    NAByte dataConst[] = {0, 1, 2, 3};
    NABufferPart* dataPart = na_NewBufferPartWithConstData(dataConst, 4);
    naTestError(na_EnlargeBufferPart(dataPart, 2, 6));
    naTestCrash(na_EnlargeBufferPart(NA_NULL, 0, 0));
    naDelete(dataPart);
    naDelete(sparsePart);
  }

  naTestGroup("Part decoupling"){
    NAByte dataConst[] = {0, 1, 2, 3};
    NABufferPart* part = na_NewBufferPartWithConstData(dataConst, 2);
    naTest(na_GetMemoryBlockDataPointerMutable(na_GetBufferPartMemoryBlock(part), 0) == dataConst);
    naTestVoid(na_DecoupleBufferPart(part));
    naTest(na_GetMemoryBlockDataPointerMutable(na_GetBufferPartMemoryBlock(part), 0) != dataConst);
    naTestVoid(naDelete(part));

    naTestCrash(na_DecoupleBufferPart(NA_NULL));
  }

  naTestGroup("Sparse Part splitting"){
    //na_SplitBufferPart
  }

  naTestGroup("Preparing buffer part of a cache"){
    //na_PrepareBufferPartCache
  }

  naTestGroup("Preparing memory for a buffer part"){
    //na_PrepareBufferPartMemory
  }

  naTestGroup("Preparing a buffer part"){
    //na_PrepareBufferPart
  }

  
  naRelease(source);
}



void printNABuffer(){
  printf("NABuffer.h:" NA_NL);

  naPrintMacro(NA_BUFFER_SOURCE_RANGE_LIMITED);
  naPrintMacro(NA_INTERNAL_BUFFER_PART_BYTESIZE);

  printf(NA_NL);
}



void testNABuffer(){
  naTestGroupFunction(MemoryBlock);  
  naTestGroupFunction(BufferSource);  
  naTestGroupFunction(BufferPart);  
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
