
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
  //  naTestError(block = na_NewMemoryBlockWithData(constPtr, sizeof(int), naFree));
  //  naTestCrash(naRelease(block));
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



//
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


  
void testNABuffer(){
  naTestGroupFunction(MemoryBlock);  
  naTestGroupFunction(BufferSource);  
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
