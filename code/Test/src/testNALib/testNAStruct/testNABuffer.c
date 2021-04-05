
#include "NATesting.h"
#include <stdio.h>

#include "NABuffer.h"


void testMemoryBlock(){
  naTestGroup("New and release"){
    NAMemoryBlock* block = NA_NULL;
    naTestVoid(block = na_NewMemoryBlock(10));
    naTestVoid(naRelease(block));
    naTestCrash(block = na_NewMemoryBlock(0));
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


  
void testNABuffer(){
  naTestGroupFunction(MemoryBlock);  
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
