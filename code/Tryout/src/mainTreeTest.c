
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// To be able to include header files from NALib, you need to either add the
// src directory of NALib as an additional project source path, or you can
// help yourself using the following two lines. Just provide the correct
// relative path to the root directory of the NALib source:

#define TRYOUT_STRINGIFY(A) #A
#define TRYOUT_NALIB_PATH(file) TRYOUT_STRINGIFY(../../NALib/src/file)

// This allows you to reference any NALib file using for example this:
// #include TRYOUT_NALIB_PATH(NABase.h)


#include TRYOUT_NALIB_PATH(NATree.h)
#include TRYOUT_NALIB_PATH(NAHeap.h)
#include TRYOUT_NALIB_PATH(NARandom.h)
#include TRYOUT_NALIB_PATH(NAMemory.h)

#define TESTSIZE    1000000
#define SUBTESTSIZE  100000.

int compare(const void* a, const void* b)
{
  if(*(double*)a > *(double*)b){return 1;}
  else if(*(double*)a < *(double*)b){return -1;}
  return 0;
}

void testQSort(){
  NADateTime t1, t2;
  int i;

  double* keys = naMalloc(TESTSIZE * sizeof(double));
  for(i = 0; i < TESTSIZE; i++){
    keys[i] = naUniformRandZE();
  }

  t1 = naMakeDateTimeNow();
  qsort(keys, TESTSIZE, sizeof(keys[0]), compare);
  t2 = naMakeDateTimeNow();
  printf("QSort: %f\n", naGetDateTimeDifference(&t2, &t1));

  naFree(keys);
}



void testHeap(){
  NAHeap heap;
  NADateTime t1, t2;
  int i;

  double* keys = naMalloc(TESTSIZE * sizeof(double*));
  naInitHeap(&heap, -1, NA_HEAP_USES_DOUBLE_KEY);

  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
  for(i = 0; i < TESTSIZE; i++){
    keys[i] = naUniformRandZE();
//    keys[i] = (double)i / TESTSIZE;
//    printf("%f\n", key);
    naInsertHeapElementConst(&heap, &(keys[i]), &(keys[i]), NA_NULL);
  }
//  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Heap Insert: %f\n", naGetDateTimeDifference(&t2, &t1));

//  double prevkey = -1;
  t1 = naMakeDateTimeNow();
  for(i = 0; i < TESTSIZE; i++){
//    const double* key = naGetHeapRootKey(&heap);
//    if(*key < prevkey){
//      printf("Wrong sorting: %d: %f, %f\n", i, *key, prevkey);
//    }
//    prevkey = *key;
    naRemoveHeapRootConst(&heap);
  }
  t2 = naMakeDateTimeNow();
  printf("Heap Iterate: %f\n", naGetDateTimeDifference(&t2, &t1));

  naClearHeap(&heap);
  naFree(keys);
}



void testTree(){
  NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_DOUBLE | NA_TREE_BALANCE_AVL);
  NATree tree;
  NADateTime t1, t2;
  NATreeIterator iter;
  int i;
  double prevkey;

  naInitTree(&tree, config);

  t1 = naMakeDateTimeNow();
  iter = naMakeTreeModifier(&tree);
  for(i = 0; i < TESTSIZE; i++){
//    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
    double key = naUniformRandZE();
//    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
//    double key = (double)i / TESTSIZE;
//    printf("%f\n", key);
    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
  }
  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Tree Insert: %f\n", naGetDateTimeDifference(&t2, &t1));

  t1 = naMakeDateTimeNow();
  iter = naMakeTreeAccessor(&tree);
  for(i = 0; i < TESTSIZE; i++){
//    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
//    double key = naUniformRandZE();
    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
//    double key = (double)i / TESTSIZE;
//    double key = (i % 10000) / 10000.;
//    printf("%f\n", key);
//    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
    naLocateTreeKey(&iter, &key, NA_FALSE);
  }
  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Tree Locate: %f\n", naGetDateTimeDifference(&t2, &t1));

//  NAInt prevkey = -1.;
  prevkey = -1.;
  t1 = naMakeDateTimeNow();
  iter = naMakeTreeModifier(&tree);
  i = 0;
  while(naIterateTree(&iter, NA_NULL, NA_NULL)){
    const double* key;
    i++;
//    const NAInt* key = naGetTreeCurKey(&iter);
//    if(*key < prevkey){
//      printf("Wrong sorting: %d: %"NA_PRIi", %"NA_PRIi"\n", i, *key, prevkey);
//    }
//    prevkey = *key;
//    if(i < 5){printf("%"NA_PRIi"\n", *key);}

    key = naGetTreeCurLeafKey(&iter);
    if(*key < prevkey){
      printf("Wrong sorting: %d: %f, %f\n", i, *key, prevkey);
    }
    prevkey = *key;
//    if(i < 5){printf("%f\n", *key);}
  }
  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Tree Iterate (%d): %f\n", i, naGetDateTimeDifference(&t2, &t1));

  // ////////////////

//  naEmptyTree(&tree);
  t1 = naMakeDateTimeNow();
  iter = naMakeTreeModifier(&tree);
  for(i = 0; i < TESTSIZE; i++){
//    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
//    double key = (double)i / TESTSIZE;
    if(i%5 != 0){
      naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
    }else{
      NABool found = naLocateTreeKey(&iter, &key, NA_FALSE);
      if(found){naRemoveTreeCurLeaf(&iter);}
    }
  }
  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Tree Insert/Remove: %f\n", naGetDateTimeDifference(&t2, &t1));

  naClearTree(&tree);
  naReleaseTreeConfiguration(config);
}




#include <stdio.h>

int main(void){
  printf("NALib Version: %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug ");
  #else
    printf("(Release ");
  #endif
  printf("%d Bits Addresses, %d Bits Integers)\n", NA_SYSTEM_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();
    testTree();
//    testHeap();
//    testQSort();
  naStopRuntime();

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished.\n");
    NA_UNUSED(getchar());
  #endif

  return 0;
}


// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

