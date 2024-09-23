
#include "NATest.h"
#include <stdio.h>

#include "NAStruct/NAStack.h"

#define NA_TEST_STACK_INIT_COUNT 10
#define NA_TEST_STACK_TYPE_SIZE 20
#define NA_BENCHMARK_STACK_SIZE 1000

void testStackArray(void) {
  
  naTestGroup("Allocating Stack Array") {
    {
      void* dummyArray = NA_NULL;
      naTest((dummyArray = na_AllocStackArray(NA_TEST_STACK_INIT_COUNT, NA_TEST_STACK_TYPE_SIZE)) != NA_NULL);
      na_DeallocStackArray(dummyArray);
    }
    {
      void* dummyArray = NA_NULL;
      naTestError(dummyArray = na_AllocStackArray(0, NA_TEST_STACK_TYPE_SIZE));
      na_DeallocStackArray(dummyArray);
    }
    {
      void* dummyArray = NA_NULL;
      naTestError(dummyArray = na_AllocStackArray(NA_TEST_STACK_INIT_COUNT, 0));
      na_DeallocStackArray(dummyArray);
    }
  }

  naTestGroup("Freeing Stack Array") {
    void* dummyArray = na_AllocStackArray(NA_TEST_STACK_INIT_COUNT, NA_TEST_STACK_TYPE_SIZE);
    naTestVoid(na_DeallocStackArray(dummyArray); dummyArray = NA_NULL);
    naTestError(na_DeallocStackArray(NA_NULL));
    // We add this check so that static code analysis tools will not report
    // potential memory leaks.
    if(dummyArray) { na_DeallocStackArray(dummyArray); }
  }

  NAList list;
  naInitList(&list);
  void* array = na_AllocStackArray(NA_TEST_STACK_INIT_COUNT, NA_TEST_STACK_TYPE_SIZE);
  naAddListLastMutable(&list, array);
  NAListIterator listIter = naMakeListMutator(&list);
  naIterateList(&listIter);

  naTestGroup("Get Stack Array count") {
    naTest(na_GetStackArrayCount(&listIter) == NA_TEST_STACK_INIT_COUNT);

    // Testing iterator at initial and Null
    naIterateList(&listIter);
    naTestCrash(na_GetStackArrayCount(&listIter));
    naTestCrash(na_GetStackArrayCount(NA_NULL));
  }

  naResetListIterator(&listIter);
  naIterateList(&listIter);

  naTestGroup("Get Stack Array First") {
    naTest(na_GetStackArrayFirstConst(&listIter) == (const NAByte*)array + sizeof(size_t));
    naTest(na_GetStackArrayFirstMutable(&listIter) == (NAByte*)array + sizeof(size_t));

    // Testing iterator at initial and Null
    naIterateList(&listIter);
    naTestCrash(na_GetStackArrayFirstConst(&listIter));
    naTestCrash(na_GetStackArrayFirstMutable(&listIter));
    naTestCrash(na_GetStackArrayFirstConst(NA_NULL)); 
    naTestCrash(na_GetStackArrayFirstMutable(NA_NULL)); 
  }

  naResetListIterator(&listIter);
  naIterateList(&listIter);

  naTestGroup("Get Stack Array at") {
    naTest(na_GetStackArrayAt(&listIter, 0, NA_TEST_STACK_TYPE_SIZE) == (NAByte*)array + sizeof(size_t));
    naTestError(na_GetStackArrayAt(&listIter, NA_TEST_STACK_INIT_COUNT, NA_TEST_STACK_TYPE_SIZE)); // index out of bound
    naTestError(na_GetStackArrayAt(&listIter, 0, 0)); // bad typeSize

    // Testing iterator at initial and Null
    naIterateList(&listIter);
    naTestCrash(na_GetStackArrayAt(&listIter, 0, NA_TEST_STACK_TYPE_SIZE));
    naTestCrash(na_GetStackArrayAt(NA_NULL, 0, NA_TEST_STACK_TYPE_SIZE));
  }
  
  na_DeallocStackArray(array);
  naClearListIterator(&listIter);
  naClearList(&list);
}


  
void testStackInitClear(void) {
  NAStack stack;

  naTestGroup("With initial count") {
    naTest(naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0) != NA_NULL);
    naTestVoid(naClearStack(&stack));
  }

  naTestGroup("With automatic initial count") {
    naTest(naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, 0, 0) != NA_NULL);
    naClearStack(&stack);
  }

  naTestGroup("Null pointer") {
    naTestCrash(naInitStack(NA_NULL, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0));
    naTestCrash(naClearStack(NA_NULL));
  }

  naTestGroup("Bad typeSize") {
    naTestCrash(naInitStack(&stack, 0, NA_TEST_STACK_INIT_COUNT, 0); naClearStack(&stack));
  }

  naTestGroup("Missing initialCount when fixed size") {
    naTestError(naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, 0, NA_STACK_FIXED_SIZE); naClearStack(&stack));
  }

  naTestGroup("Too many iterators") {
    // untested as struct will leak
    naUntested("naClearStack with iterators still running");
  }
}



void testStackPushTopPopPeek(void) {
  NAStack stack;
  void* data = NA_NULL;

  naTestGroup("Adding, Removing 1 element") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    naTest((data = naPushStack(&stack)) != NA_NULL);
    naTest(naTopStack(&stack) == data);
    naTest(naPopStack(&stack) == data);
    naClearStack(&stack);
  }

  naTestGroup("Adding, Removing 1 element more than available") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT; i++) {naPushStack(&stack);}
    naTest((data = naPushStack(&stack)) != NA_NULL);
    naTest(naTopStack(&stack) == data);
    naTest(naPopStack(&stack) == data);
    // Pushing even more on the stack and see if clearing works fine.
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT * 10; i++) {naPushStack(&stack);}
    naTestVoid(naClearStack(&stack));
  }

  naTestGroup("Peeking") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT + 1; i++) {naPushStack(&stack);}
    naTest((data = naPeekStack(&stack, 0)) != NA_NULL);
    naTest((data = naPeekStack(&stack, NA_TEST_STACK_INIT_COUNT - 1)) != NA_NULL);
    naTest((data = naPeekStack(&stack, NA_TEST_STACK_INIT_COUNT + 0)) != NA_NULL);
    naTestError(naPeekStack(&stack, NA_TEST_STACK_INIT_COUNT + 1));
    naClearStack(&stack);
  }

  naTestGroup("Null pointer") {
    naTestCrash(naPushStack(NA_NULL));
    naTestCrash(naTopStack(NA_NULL));
    naTestCrash(naPopStack(NA_NULL));
    naTestCrash(naPeekStack(NA_NULL, 0));
  }

  naTestGroup("Empty stack") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    naTestError(naTopStack(&stack));
    naClearStack(&stack);

    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    naTestError(naPopStack(&stack));
    naClearStack(&stack);

    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    naTestError(naPeekStack(&stack, 0));
    naClearStack(&stack);
  }
}



void testStackCount(void) {
  NAStack stack;

  naTestGroup("get count") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    naTest(naGetStackCount(&stack) == 0);
    naPushStack(&stack);
    naTest(naGetStackCount(&stack) == 1);
    naPopStack(&stack);
    naTest(naGetStackCount(&stack) == 0);
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT; i++) {naPushStack(&stack);}
    naTest(naGetStackCount(&stack) == NA_TEST_STACK_INIT_COUNT);
    naPushStack(&stack);
    naTest(naGetStackCount(&stack) == NA_TEST_STACK_INIT_COUNT + 1);
    naClearStack(&stack);
  }

  naTestGroup("get reserved count") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, 0);
    naTest(naGetStackReservedCount(&stack) == NA_TEST_STACK_INIT_COUNT);
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT; i++) {naPushStack(&stack);}
    naTest(naGetStackReservedCount(&stack) == NA_TEST_STACK_INIT_COUNT);
    naPushStack(&stack);
    naTest(naGetStackReservedCount(&stack) > NA_TEST_STACK_INIT_COUNT);
    naClearStack(&stack);
  }

  naTestGroup("Null pointer") {
    naTestCrash(naGetStackCount(NA_NULL));
    naTestCrash(naGetStackReservedCount(NA_NULL));
  }
}



void testStackGrow(void) {
  NAStack stack;

  naTestGroup("Linear Growing") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_LINEAR);

    // push one more onto the stack than initial
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT + 1; i++) {naPushStack(&stack);}
    naTest(naGetStackReservedCount(&stack) == (1 + 1) * NA_TEST_STACK_INIT_COUNT);

    naClearStack(&stack);
  }

  naTestGroup("Fibonacci Growing") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_FIBONACCI);

    // push one more onto the stack than initial
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT + 1; i++) {naPushStack(&stack);}
    naTest((double)naGetStackReservedCount(&stack) > (1. + 1.618) * (double)NA_TEST_STACK_INIT_COUNT);

    naClearStack(&stack);
  }

  naTestGroup("Exponential Growing") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_EXPONENTIAL);

    // push one more onto the stack than initial
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT + 1; i++) {naPushStack(&stack);}
    naTest(naGetStackReservedCount(&stack) == (1 + 2) * NA_TEST_STACK_INIT_COUNT);

    naClearStack(&stack);
  }

  naTestGroup("Fixed size stack") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_FIXED_SIZE);
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT; i++) {naPushStack(&stack);}
    naTestError(naPushStack(&stack));
    naClearStack(&stack);
  }

}  
  
void testStackShrink(void) {
  NAStack stack;

  naTestGroup("Automatic shrinking") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_LINEAR);

    // push 5 times more onto the stack than initial
    for(int i = 0; i < 5 * NA_TEST_STACK_INIT_COUNT; i++) {naPushStack(&stack);}
    // Pop 4 times
    for(int i = 0; i < 4 * NA_TEST_STACK_INIT_COUNT; i++) {naPopStack(&stack);}

    naTest(naGetStackReservedCount(&stack) == 2 * NA_TEST_STACK_INIT_COUNT);
    naClearStack(&stack);
  }

  naTestGroup("No shrinking") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_LINEAR | NA_STACK_NO_SHRINKING);

    // push 5 times more onto the stack than initial
    for(int i = 0; i < 5 * NA_TEST_STACK_INIT_COUNT; i++) {naPushStack(&stack);}
    // Pop 4 times
    for(int i = 0; i < 4 * NA_TEST_STACK_INIT_COUNT; i++) {naPopStack(&stack);}

    naTest(naGetStackReservedCount(&stack) == 5 * NA_TEST_STACK_INIT_COUNT);
    naClearStack(&stack);
  }
}



void testStackShrinkIfNecessary(void) {
  NAStack stack;

  naTestGroup("Nothing to shrink") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_LINEAR | NA_STACK_NO_SHRINKING);

    // push one more onto the stack than initial
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT + 1; i++) {naPushStack(&stack);}

    naTest(naGetStackReservedCount(&stack) == 2 * NA_TEST_STACK_INIT_COUNT);
    naTestVoid(naShrinkStackIfNecessary(&stack, NA_FALSE));
    naTest(naGetStackReservedCount(&stack) == 2 * NA_TEST_STACK_INIT_COUNT);
    naTestVoid(naShrinkStackIfNecessary(&stack, NA_TRUE));
    naClearStack(&stack);
  }

  naTestGroup("Only aggressive shrinking") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_LINEAR | NA_STACK_NO_SHRINKING);

    // push one more onto the stack than initial
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT + 1; i++) {naPushStack(&stack);}
    // Pop one
    naPopStack(&stack);

    naTest(naGetStackReservedCount(&stack) == 2 * NA_TEST_STACK_INIT_COUNT);
    naTestVoid(naShrinkStackIfNecessary(&stack, NA_FALSE));
    naTest(naGetStackReservedCount(&stack) == 2 * NA_TEST_STACK_INIT_COUNT);
    naTestVoid(naShrinkStackIfNecessary(&stack, NA_TRUE));
    naTest(naGetStackReservedCount(&stack) == 1 * NA_TEST_STACK_INIT_COUNT);
    naClearStack(&stack);
  }

  naTestGroup("Full Shrinking") {
    naInitStack(&stack, NA_TEST_STACK_TYPE_SIZE, NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_LINEAR | NA_STACK_NO_SHRINKING);

    // push 5 times more onto the stack than initial
    for(int i = 0; i < 5 * NA_TEST_STACK_INIT_COUNT; i++) {naPushStack(&stack);}
    // Pop 4 times
    for(int i = 0; i < 4 * NA_TEST_STACK_INIT_COUNT; i++) {naPopStack(&stack);}

    naTest(naGetStackReservedCount(&stack) == 5 * NA_TEST_STACK_INIT_COUNT);
    naTestVoid(naShrinkStackIfNecessary(&stack, NA_FALSE));
    naTest(naGetStackReservedCount(&stack) == 2 * NA_TEST_STACK_INIT_COUNT);
    naTestVoid(naShrinkStackIfNecessary(&stack, NA_TRUE));
    naTest(naGetStackReservedCount(&stack) == 1 * NA_TEST_STACK_INIT_COUNT);
    naClearStack(&stack);
  }

  naTestGroup("Null pointer") {
    naTestCrash(naShrinkStackIfNecessary(NA_NULL, NA_FALSE));
  }
}


void testStackDump(void) {
  NAStack stack;
  int dmp[] = {99, 99, 99, 99, 99};

  naTestGroup("Dumping empty stack") {
    naInitStack(&stack, sizeof(int), 0, 0);
    naDumpStack(&stack, dmp);
    naClearStack(&stack);

    naTest(dmp[0] == 99);
  }

  naTestGroup("Dumping stack") {
    naInitStack(&stack, sizeof(int), 0, 0);
    *(int*)naPushStack(&stack) = 1;
    *(int*)naPushStack(&stack) = 2;
    naDumpStack(&stack, dmp);
    naClearStack(&stack);

    naTest(dmp[0] == 1);
    naTest(dmp[1] == 2);
    naTest(dmp[2] == 99);
  }

  dmp[0] = 99;
  dmp[1] = 99;

  naTestGroup("Dumping stack with multiple arrays") {
    naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
    *(int*)naPushStack(&stack) = 1;
    *(int*)naPushStack(&stack) = 2;
    naDumpStack(&stack, dmp);
    naClearStack(&stack);

    naTest(dmp[0] == 1);
    naTest(dmp[1] == 2);
    naTest(dmp[2] == 99);
  }

  naTestGroup("Null pointer") {
    naInitStack(&stack, sizeof(int), 0, 0);

    naTestCrash(naDumpStack(NA_NULL, dmp));
    naTestCrash(naDumpStack(&stack, NA_NULL));

    naClearStack(&stack);
  }
}



void testStackIterator(void) {
  NAStack stack;
  naInitStack(&stack, sizeof(int), 0, 0);
  *(int*)naPushStack(&stack) = 1;
  *(int*)naPushStack(&stack) = 2;

  naTestGroup("Creating and Clearing") {
    NAStackIterator iterConst;
    NAStackIterator iterMutable;

    naTestVoid(iterConst = naMakeStackAccessor(&stack); naClearStackIterator(&iterConst));
    naTestVoid(iterMutable = naMakeStackMutator(&stack); naClearStackIterator(&iterMutable));
  }

  naTestGroup("Iterating") {
    NAStackIterator iter = naMakeStackAccessor(&stack);
    naTest(naIterateStack(&iter));
    naTest(naIterateStack(&iter));
    naTest(!naIterateStack(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Initial position") {
    NAStackIterator iter = naMakeStackAccessor(&stack);
    naTest(naIsStackAtInitial(&iter));
    naIterateStack(&iter);
    naTest(!naIsStackAtInitial(&iter));
    naIterateStack(&iter);
    naTest(!naIsStackAtInitial(&iter));
    naIterateStack(&iter);
    naTest(naIsStackAtInitial(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Resetting") {
    NAStackIterator iter = naMakeStackAccessor(&stack);
    naIterateStack(&iter);
    naTestVoid(naResetStackIterator(&iter))
    naTest(naIsStackAtInitial(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Null pointer") {
    naTestCrash(naMakeStackAccessor(NA_NULL));
    naTestCrash(naMakeStackMutator(NA_NULL));
    naTestCrash(naClearStackIterator(NA_NULL));
    naTestCrash(naIterateStack(NA_NULL));
    naTestCrash(naResetStackIterator(NA_NULL));
    naTestCrash(naIsStackAtInitial(NA_NULL));
  }

  naTestGroup("Too few iterators") {
    naUntested("naClearStackIterator with no iterators on the stack");
  }

  naClearStack(&stack);

  naTestGroup("Out of bound iterator") {
    naInitStack(&stack, sizeof(int), NA_TEST_STACK_INIT_COUNT, 0);
    naPushStack(&stack);
    NAStackIterator iter = naMakeStackAccessor(&stack);
    naIterateStack(&iter);
    naPopStack(&stack);
    naTestError(naIterateStack(&iter));
    naClearStackIterator(&iter);
    naClearStack(&stack);
  }

  naTestGroup("Iterating over more than the initial count") {
    naInitStack(&stack, sizeof(int), NA_TEST_STACK_INIT_COUNT, NA_STACK_GROW_LINEAR);
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT + 1; i++) {naPushStack(&stack);}
    NAStackIterator iter = naMakeStackAccessor(&stack);
    for(int i = 0; i < NA_TEST_STACK_INIT_COUNT; i++) {naIterateStack(&iter);}
    naTest(naIterateStack(&iter));
    naTest(!naIterateStack(&iter));
    naClearStackIterator(&iter);
    naClearStack(&stack);
  }
}



void testStackIteratorAccessAndMutate(void) {
  NAStack stack;
  naInitStack(&stack, sizeof(int), 0, 0);
  *(int*)naPushStack(&stack) = 1;
  *(int*)naPushStack(&stack) = 2;

  NAStack pStack;
  int x1 = 1, x2 = 2;
  naInitStack(&pStack, sizeof(int*), 0, 0);
  *(int**)naPushStack(&pStack) = &x1;
  *(int**)naPushStack(&pStack) = &x2;

  naTestGroup("Accessor") {
    NAStackIterator iter = naMakeStackAccessor(&stack);
    naIterateStack(&iter);
    naTest(*(const int*)naGetStackCurConst(&iter) == 1);
    naTestError(naGetStackCurMutable(&iter));
    naIterateStack(&iter);
    naTest(*(const int*)naGetStackCurConst(&iter) == 2);
    naIterateStack(&iter);
    naTestCrash(naGetStackCurConst(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Mutator") {
    NAStackIterator iter = naMakeStackMutator(&stack);
    naIterateStack(&iter);
    naTest(*(const int*)naGetStackCurConst(&iter) == 1);
    naTest(*(int*)naGetStackCurMutable(&iter) == 1);
    naIterateStack(&iter);
    naTest(*(int*)naGetStackCurMutable(&iter) == 2);
    naIterateStack(&iter);
    naTestCrash(naGetStackCurMutable(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Accessor pointer") {
    NAStackIterator iter = naMakeStackAccessor(&pStack);
    naIterateStack(&iter);
    naTest(*(const int*)naGetStackCurpConst(&iter) == 1);
    naTestError(naGetStackCurpMutable(&iter));
    naIterateStack(&iter);
    naTest(*(const int*)naGetStackCurpConst(&iter) == 2);
    naIterateStack(&iter);
    naTestCrash(naGetStackCurpConst(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Mutator pointer") {
    NAStackIterator iter = naMakeStackMutator(&pStack);
    naIterateStack(&iter);
    naTest(*(const int*)naGetStackCurpConst(&iter) == 1);
    naTest(*(int*)naGetStackCurpMutable(&iter) == 1);
    naIterateStack(&iter);
    naTest(*(int*)naGetStackCurpMutable(&iter) == 2);
    naIterateStack(&iter);
    naTestCrash(naGetStackCurpMutable(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Iterator at initial") {
    NAStackIterator iter = naMakeStackMutator(&pStack);
    naTestCrash(naGetStackCurConst(&iter));
    naTestCrash(naGetStackCurMutable(&iter));
    naTestCrash(naGetStackCurpConst(&iter));
    naTestCrash(naGetStackCurpMutable(&iter));
    naClearStackIterator(&iter);
  }

  naTestGroup("Null pointer") {
    naTestCrash(naGetStackCurConst(NA_NULL));
    naTestCrash(naGetStackCurMutable(NA_NULL));
    naTestCrash(naGetStackCurpConst(NA_NULL));
    naTestCrash(naGetStackCurpMutable(NA_NULL));
  }

  naClearStack(&stack);
  naClearStack(&pStack);

  naTestGroup("Out of bound iterator") {
    naInitStack(&stack, sizeof(int), NA_TEST_STACK_INIT_COUNT, 0);
    naPushStack(&stack);
    NAStackIterator iter = naMakeStackAccessor(&stack);
    naIterateStack(&iter);
    naPopStack(&stack);
    naTestError(naGetStackCurConst(&iter));
    naTestError(naGetStackCurMutable(&iter));
    naTestError(naGetStackCurpConst(&iter));
    naTestError(naGetStackCurpMutable(&iter));
    naClearStackIterator(&iter);
    naClearStack(&stack);
  }
}



int na_DummyStackSum;
void na_DummyIncreaseSumConst(const void* elem) {
  na_DummyStackSum += *(const int*)elem;
}
void na_DummyIncreaseSumMutable(void* elem) {
  na_DummyStackSum += *(int*)elem;
}

void testStackForeach(void) {
  NAStack stack;
  naInitStack(&stack, sizeof(int), 0, 0);
  *(int*)naPushStack(&stack) = 1;
  *(int*)naPushStack(&stack) = 2;

  NAStack pStack;
  int x1 = 1, x2 = 2;
  naInitStack(&pStack, sizeof(int*), 0, 0);
  *(int**)naPushStack(&pStack) = &x1;
  *(int**)naPushStack(&pStack) = &x2;

  naTestGroup("Foreach with int") {
    na_DummyStackSum = 0;
    naTestVoid(naForeachStackConst(&stack, (NAAccessor)na_DummyIncreaseSumConst));
    naTest(na_DummyStackSum == 3);
    na_DummyStackSum = 0;
    naTestVoid(naForeachStackMutable(&stack, (NAMutator)na_DummyIncreaseSumMutable));
    naTest(na_DummyStackSum == 3);
  }

  naTestGroup("Foreach with int pointer") {
    na_DummyStackSum = 0;
    naTestVoid(naForeachStackpConst(&pStack, (NAAccessor)na_DummyIncreaseSumConst));
    naTest(na_DummyStackSum == 3);
    na_DummyStackSum = 0;
    naTestVoid(naForeachStackpMutable(&pStack, (NAMutator)na_DummyIncreaseSumMutable));
    naTest(na_DummyStackSum == 3);
  }

  naTestGroup("Null pointer") {
    naTestCrash(naForeachStackConst(NA_NULL, na_DummyIncreaseSumConst));
    naTestCrash(naForeachStackMutable(NA_NULL, na_DummyIncreaseSumMutable));
    naTestCrash(naForeachStackpConst(NA_NULL, na_DummyIncreaseSumConst));
    naTestCrash(naForeachStackpMutable(NA_NULL, na_DummyIncreaseSumMutable));
    naTestCrash(naForeachStackConst(&stack, NA_NULL));
    naTestCrash(naForeachStackMutable(&stack, NA_NULL));
    naTestCrash(naForeachStackpConst(&pStack, NA_NULL));
    naTestCrash(naForeachStackpMutable(&pStack, NA_NULL));
  }

  naClearStack(&stack);
  naClearStack(&pStack);
}



void printNAStack(void) {
  printf("NAStack.h:" NA_NL);

  naPrintMacro(NA_STACK_GROW_AUTO);
  naPrintMacro(NA_STACK_GROW_LINEAR);
  naPrintMacro(NA_STACK_GROW_FIBONACCI);
  naPrintMacro(NA_STACK_GROW_EXPONENTIAL);
  naPrintMacro(NA_STACK_FIXED_SIZE);

  naPrintMacro(NA_STACK_SHRINK_AUTO);
  naPrintMacro(NA_STACK_NO_SHRINKING);

  naPrintMacro(NA_STACK_GROW_MASK);
  naPrintMacro(NA_STACK_SHRINK_MASK);

  printf(NA_NL);
}



void testNAStack(void) {
  naTestFunction(testStackArray);  
  naTestFunction(testStackInitClear);  
  naTestFunction(testStackPushTopPopPeek);  
  naTestFunction(testStackCount);  
  naTestFunction(testStackGrow);  
  naTestFunction(testStackShrink);  
  naTestFunction(testStackShrinkIfNecessary);  
  naTestFunction(testStackDump);  
  naTestFunction(testStackIterator);  
  naTestFunction(testStackIteratorAccessAndMutate);  
  naTestFunction(testStackForeach);  
}


NA_HIDEF void na_DummyPushAndPopManyItemsOnStack(NAStack* stack) {
  for(size_t i = 0; i < NA_BENCHMARK_STACK_SIZE; i++) {naPushStack(stack);}
  for(size_t i = 0; i < NA_BENCHMARK_STACK_SIZE; i++) {naPopStack(stack);}
}

void benchmarkNAStack(void) {
  printf(NA_NL "NAStack:" NA_NL);

  NAStack stack;
  int* buf;
  NAStackIterator iter;
  int myInteger = 1;

  // Init / Clear
  naBenchmark(naClearStack(naInitStack(&stack, sizeof(int), 0, 0)));

  // Push all automatic
  naInitStack(&stack, sizeof(int), 0, 0);
  naBenchmark(naPushStack(&stack));
  naClearStack(&stack);

  // Push linear starting with 1
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  naBenchmark(naPushStack(&stack));
  naClearStack(&stack);

  // Push fibonacci starting with 1
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  naBenchmark(naPushStack(&stack));
  naClearStack(&stack);

  // Push exponential starting with 1
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_EXPONENTIAL);
  naBenchmark(naPushStack(&stack));
  naClearStack(&stack);

  // Push / Pop
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  naBenchmark(naPushStack(&stack); naPopStack(&stack));
  naClearStack(&stack);

  // Many Push / Pop WITH shrinking
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  naBenchmark(na_DummyPushAndPopManyItemsOnStack(&stack));
  naClearStack(&stack);

  // Many Push / Pop with NO shrinking
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI | NA_STACK_NO_SHRINKING);
  naBenchmark(na_DummyPushAndPopManyItemsOnStack(&stack));
  naClearStack(&stack);

  // Top
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  naPushStack(&stack);
  naBenchmark(naTopStack(&stack));
  naClearStack(&stack);

  // Peek linear
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  for(size_t i = 0; i < NA_BENCHMARK_STACK_SIZE; i++)naPushStack(&stack);
  naBenchmark(naPeekStack(&stack, NA_BENCHMARK_STACK_SIZE - 1));
  naClearStack(&stack);

  // Peek fibonacci
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  for(size_t i = 0; i < NA_BENCHMARK_STACK_SIZE; i++)naPushStack(&stack);
  naBenchmark(naPeekStack(&stack, NA_BENCHMARK_STACK_SIZE - 1));
  naClearStack(&stack);
  
  // get count, get reserved count
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  for(size_t i = 0; i < NA_BENCHMARK_STACK_SIZE; i++)naPushStack(&stack);
  naBenchmark(naGetStackCount(&stack));
  naBenchmark(naGetStackReservedCount(&stack));
  naClearStack(&stack);

  // Shrink fibonacci stack mildly with many items
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  naBenchmark(na_DummyPushAndPopManyItemsOnStack(&stack); naShrinkStackIfNecessary(&stack, NA_FALSE));
  naClearStack(&stack);

  // Shrink fibonacci stack aggressively with many items
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  naBenchmark(na_DummyPushAndPopManyItemsOnStack(&stack); naShrinkStackIfNecessary(&stack, NA_TRUE));
  naClearStack(&stack);

  // Dump linear stack
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  buf = naMalloc(sizeof(int) * NA_BENCHMARK_STACK_SIZE);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {naPushStack(&stack);}
  naBenchmark(naDumpStack(&stack, buf));
  naFree(buf);
  naClearStack(&stack);

  // Dump fibonacci stack
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  buf = naMalloc(sizeof(int) * NA_BENCHMARK_STACK_SIZE);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {naPushStack(&stack);}
  naBenchmark(naDumpStack(&stack, buf));
  naFree(buf);
  naClearStack(&stack);

  // Iterater make and clear
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  NAStackIterator iter1;
  NAStackIterator iter2;
  naBenchmark((iter1 = naMakeStackAccessor(&stack)); naClearStackIterator(&iter1));
  naBenchmark((iter2 = naMakeStackMutator(&stack)); naClearStackIterator(&iter2));
  naClearStack(&stack);

  // Iterate linear
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {naPushStack(&stack);}
  iter = naMakeStackAccessor(&stack);
  naBenchmark(naIterateStack(&iter));
  naClearStackIterator(&iter);
  naClearStack(&stack);

  // Iterate fibonacci
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {naPushStack(&stack);}
  iter = naMakeStackAccessor(&stack);
  naBenchmark(naIterateStack(&iter));
  naClearStackIterator(&iter);
  naClearStack(&stack);

  // Is stack at initial
  naInitStack(&stack, sizeof(int), 1, 0);
  iter = naMakeStackAccessor(&stack);
  naBenchmark(naIsStackAtInitial(&iter));
  naBenchmark(naResetStackIterator(&iter));
  naClearStackIterator(&iter);
  naClearStack(&stack);

  // Get cur element of iterator int
  naInitStack(&stack, sizeof(int), 1, 0);
  naPushStack(&stack);
  iter = naMakeStackMutator(&stack);
  naIterateStack(&iter);
  naBenchmark(naGetStackCurConst(&iter));
  naBenchmark(naGetStackCurMutable(&iter));
  naClearStackIterator(&iter);
  naClearStack(&stack);

  // Get cur element of iterator int*
  naInitStack(&stack, sizeof(int*), 1, 0);
  naPushStack(&stack);
  iter = naMakeStackMutator(&stack);
  naIterateStack(&iter);
  naBenchmark(naGetStackCurpConst(&iter));
  naBenchmark(naGetStackCurpMutable(&iter));
  naClearStackIterator(&iter);
  naClearStack(&stack);

  // foreach on linear stack with int
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_LINEAR);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {*(int*)naPushStack(&stack) = 1;}
  naBenchmark(naForeachStackConst(&stack, (NAAccessor)na_DummyIncreaseSumConst));
  naBenchmark(naForeachStackMutable(&stack, (NAMutator)na_DummyIncreaseSumMutable));
  naClearStack(&stack);

  // foreach on fibonacci stack with int
  naInitStack(&stack, sizeof(int), 1, NA_STACK_GROW_FIBONACCI);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {*(int*)naPushStack(&stack) = 1;}
  naBenchmark(naForeachStackConst(&stack, (NAAccessor)na_DummyIncreaseSumConst));
  naBenchmark(naForeachStackMutable(&stack, (NAMutator)na_DummyIncreaseSumMutable));
  naClearStack(&stack);

    // foreach on linear stack with int
  naInitStack(&stack, sizeof(int*), 1, NA_STACK_GROW_LINEAR);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {*(int**)naPushStack(&stack) = &myInteger;}
  naBenchmark(naForeachStackpConst(&stack, (NAAccessor)na_DummyIncreaseSumConst));
  naBenchmark(naForeachStackpMutable(&stack, (NAMutator)na_DummyIncreaseSumMutable));
  naClearStack(&stack);

    // foreach on fibonacci stack with int
  naInitStack(&stack, sizeof(int*), 1, NA_STACK_GROW_FIBONACCI);
  for(int i=0;i<NA_BENCHMARK_STACK_SIZE;i++) {*(int**)naPushStack(&stack) = &myInteger;}
  naBenchmark(naForeachStackpConst(&stack, (NAAccessor)na_DummyIncreaseSumConst));
  naBenchmark(naForeachStackpMutable(&stack, (NAMutator)na_DummyIncreaseSumMutable));
  naClearStack(&stack);
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
