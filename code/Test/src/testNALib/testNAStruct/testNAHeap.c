
#include "NATest.h"
#include <stdio.h>

#include "NAStruct/NAHeap.h"


#define HEAP_TEST_SIZE 10
int values[HEAP_TEST_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int keys[HEAP_TEST_SIZE] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

void test_FillHeapWithConstElements(NAHeap* heap, size_t count) {
  for(int i = 0; i < count; ++i) {
    naInsertHeapElementConst(heap, &values[i], &keys[i], NA_NULL);
  }
}
void test_FillHeapWithMutableElements(NAHeap* heap, size_t count) {
  for(int i = 0; i < count; ++i) {
    naInsertHeapElementMutable(heap, &values[i], &keys[i], NA_NULL);
  }
}

void testHeapConstructionAndDestruction() {
  naTestGroup("Faulty construction and destruction") {
    NAHeap heap;
    // Crashes:
    naTestCrash(naInitHeap(NA_NULL, 0, 0));
    naTestCrash(naInitHeap(&heap, NA_MAX_s, 0));
    naTestCrash(naClearHeap(NA_NULL));
  }
  
  naTestGroup("Creating and destroying fixed heap without flags") {
    NAHeap heap;
    naTest(naInitHeap(&heap, HEAP_TEST_SIZE, 0));
    naTestVoid(naClearHeap(&heap));
  }

  naTestGroup("Creating and destroying dynamic heap without flags") {
    NAHeap heap;
    naTest(naInitHeap(&heap, 0, 0));
    naTestVoid(naClearHeap(&heap));
  }
}

void testHeapGetters() {
  naTestGroup("Getter crashes") {
    naTestCrash(naGetHeapMaxCount(NA_NULL));
    naTestCrash(naGetHeapCount(NA_NULL));
  }

  naTestGroup("Fixed sizes") {
    NAHeap heap;
    naInitHeap(&heap, HEAP_TEST_SIZE, 0);
    naTest(naGetHeapMaxCount(&heap) == HEAP_TEST_SIZE);
    naTest(naGetHeapCount(&heap) == 0);
    naClearHeap(&heap);
  }

  naTestGroup("Dynamic sizes") {
    NAHeap heap;
    naInitHeap(&heap, 0, 0);
    naTest(naGetHeapMaxCount(&heap) == 1);
    naTest(naGetHeapCount(&heap) == 0);
    naClearHeap(&heap);
  }
}

void testHeapFilling() {
  naTestGroup("adding and removing crashes") {
    naTestCrash(naInsertHeapElementConst(NA_NULL, NA_NULL, NA_NULL, NA_NULL));
    naTestCrash(naInsertHeapElementMutable(NA_NULL, NA_NULL, NA_NULL, NA_NULL));
    naTestCrash(naGetHeapRootConst(NA_NULL));
    naTestCrash(naGetHeapRootMutable(NA_NULL));
    naTestCrash(naRemoveHeapRootConst(NA_NULL));
    naTestCrash(naRemoveHeapRootMutable(NA_NULL));
  }

  naTestGroup("Adding and removing const elements") {
    NAHeap heap;
    int dummy = 1234;
    naInitHeap(&heap, HEAP_TEST_SIZE, 0);
    naTestVoid(naInsertHeapElementConst(&heap, &dummy, &dummy, NA_NULL));
    naTest(naGetHeapCount(&heap) == 1);
    naTest(naGetHeapRootConst(&heap) == &dummy);
    naTest(naGetHeapCount(&heap) == 1);
    naTest(naRemoveHeapRootConst(&heap) == &dummy);
    naTest(naGetHeapCount(&heap) == 0);
    test_FillHeapWithConstElements(&heap, HEAP_TEST_SIZE);
    naTest(naGetHeapCount(&heap) == 10);
    naClearHeap(&heap);
  }

  naTestGroup("Adding and removing mutable elements") {
    NAHeap heap;
    int dummy = 1234;
    naInitHeap(&heap, HEAP_TEST_SIZE, 0);
    naTestVoid(naInsertHeapElementMutable(&heap, &dummy, &dummy, NA_NULL));
    naTest(naGetHeapCount(&heap) == 1);
    naTest(naGetHeapRootMutable(&heap) == &dummy);
    naTest(naGetHeapCount(&heap) == 1);
    naTest(naRemoveHeapRootMutable(&heap) == &dummy);
    naTest(naGetHeapCount(&heap) == 0);
    test_FillHeapWithMutableElements(&heap, HEAP_TEST_SIZE);
    naTest(naGetHeapCount(&heap) == 10);
    naClearHeap(&heap);
  }

  naTestGroup("Adding too many elements") {
    NAHeap heap;
    int dummy = 1234;
    naInitHeap(&heap, HEAP_TEST_SIZE, 0);
    test_FillHeapWithConstElements(&heap, HEAP_TEST_SIZE);
    naTestCrash(naInsertHeapElementMutable(&heap, &dummy, &dummy, NA_NULL));
    naClearHeap(&heap);
  }

  naTestGroup("Removing too many elements") {
    NAHeap heap;
    naInitHeap(&heap, HEAP_TEST_SIZE, 0);
    naTestCrash(naRemoveHeapRootConst(&heap));
    naTestCrash(naRemoveHeapRootMutable(&heap));
    naClearHeap(&heap);
  }

  naTestGroup("trying to mutate const") {
    NAHeap heap;
    int dummy = 1234;
    naInitHeap(&heap, HEAP_TEST_SIZE, 0);
    naInsertHeapElementConst(&heap, &dummy, &dummy, NA_NULL);
    naTestError(naGetHeapRootMutable(&heap));
    naTestError(naRemoveHeapRootMutable(&heap));
    naClearHeap(&heap);
  }

  naTestGroup("Emptying Heap") {
    naTestCrash(naEmptyHeap(NA_NULL));
    NAHeap heap;
    naInitHeap(&heap, HEAP_TEST_SIZE, 0);
    test_FillHeapWithConstElements(&heap, HEAP_TEST_SIZE);
    naTestVoid(naEmptyHeap(&heap));
    naTest(naGetHeapCount(&heap) == 0);
    naClearHeap(&heap);
  }

  naTestGroup("Growing dynamic heap") {
    NAHeap heap;
    naInitHeap(&heap, 0, 0);
    test_FillHeapWithConstElements(&heap, HEAP_TEST_SIZE);
    naTest(naGetHeapMaxCount(&heap) >= HEAP_TEST_SIZE);
    naEmptyHeap(&heap);
    naTest(naGetHeapMaxCount(&heap) >= HEAP_TEST_SIZE);
    naTestCrash(naShrinkHeapIfNecessary(NA_NULL));
    naTestVoid(naShrinkHeapIfNecessary(&heap));
    naTest(naGetHeapMaxCount(&heap) == 1);
  }
}





void testNAHeap(void) {
  naTestFunction(testHeapConstructionAndDestruction);  
  naTestFunction(testHeapGetters);  
  naTestFunction(testHeapFilling);  
}



void printNAHeap(void) {
  printf("NAHeap.h:" NA_NL);

  naPrintMacroux16(NA_HEAP_USES_DOUBLE_KEY);
  naPrintMacroux16(NA_HEAP_USES_FLOAT_KEY);
  naPrintMacroux16(NA_HEAP_USES_NAINT_KEY);
  naPrintMacroux16(NA_HEAP_USES_DATETIME_KEY);
  naPrintMacroux16(NA_HEAP_DATATYPE_MASK);
  
  naPrintMacroux16(NA_HEAP_IS_MIN_HEAP);
  naPrintMacroux16(NA_HEAP_IS_MAX_HEAP);
  
  naPrintMacroux16(NA_HEAP_STORES_BACKPOINTERS);
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
