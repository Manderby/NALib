
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
#include TRYOUT_NALIB_PATH(NAHeap.h)
#include TRYOUT_NALIB_PATH(NARandom.h)

#define TESTSIZE    1000000
#define SUBTESTSIZE  10000.

#define KEYTYPE double

int compare(const void* a, const void* b)
{
  if(*(double*)a > *(double*)b){return 1;}
  else if(*(double*)a < *(double*)b){return -1;}
  return 0;  
}

void testQSort(double* keys){
  NADateTime t1, t2;

  t1 = naMakeDateTimeNow();
  qsort(keys, TESTSIZE, sizeof(keys[0]), compare);
  t2 = naMakeDateTimeNow();
  printf("QSort: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));

}



void testHeap(const double* keys){
  NAHeap heap;
  NADateTime t1;
  NADateTime t2;
  NADateTime t3;
  int i;
  double prevkey = 0.;

  naInitHeap(&heap, -1, NA_HEAP_USES_DOUBLE_KEY);
  
  t1 = naMakeDateTimeNow();
  for(i = 0; i < TESTSIZE; i++){
    naInsertHeapElementConst(&heap, &(keys[i]), &(keys[i]), NA_NULL);
  }

  t2 = naMakeDateTimeNow();

  for(i = 0; i < TESTSIZE; i++){
    const double* curkey = naRemoveHeapRootConst(&heap);
    if(prevkey > *curkey){
      printf("Error in sorting");
    }
    prevkey = *curkey;
//    printf("%f" NA_NL, *curkey);
  }

  t3 = naMakeDateTimeNow();

  printf("Heap Insert: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
  printf("Heap Iterate: %f" NA_NL, naGetDateTimeDifference(&t3, &t2));

  naClearHeap(&heap);
}



void testTree(){
  NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_DOUBLE | NA_TREE_BALANCE_AVL);
  NATree tree;
  NADateTime t1, t2;
  NATreeIterator iter;
  int i;
  double prevkey = -1.;

  naInitTree(&tree, config); 

  t1 = naMakeDateTimeNow();
  iter = naMakeTreeModifier(&tree);
  for(i = 0; i < TESTSIZE; i++){
//    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
    double key = naUniformRandZE();
//    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
//    double key = (double)i / TESTSIZE;
//    printf("%f" NA_NL, key);
    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
  }
  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Tree Insert: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));

  t1 = naMakeDateTimeNow();
  iter = naMakeTreeAccessor(&tree);
  for(i = 0; i < TESTSIZE; i++){
//    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
//    double key = naUniformRandZE();
    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
//    double key = (double)i / TESTSIZE;
//    double key = (i % 10000) / 10000.;
//    printf("%f" NA_NL, key);
//    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
    naLocateTreeKey(&iter, &key, NA_FALSE);
  }
  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Tree Locate: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));

//  NAInt prevkey = -1.;
  t1 = naMakeDateTimeNow();
  iter = naMakeTreeModifier(&tree);
  i = 0;
  while(naIterateTree(&iter, NA_NULL, NA_NULL)){
    const double* key;
    i++;
//    const NAInt* key = naGetTreeCurKey(&iter);
//    if(*key < prevkey){
//      printf("Wrong sorting: %d: %"NA_PRIi", %"NA_PRIi NA_NL, i, *key, prevkey);
//    }
//    prevkey = *key;
//    if(i < 5){printf("%"NA_PRIi NA_NL, *key);}

    key = naGetTreeCurLeafKey(&iter);
    if(*key < prevkey){
      printf("Wrong sorting: %d: %f, %f" NA_NL, i, *key, prevkey);
    }
    prevkey = *key;
//    if(i < 50){printf("%f" NA_NL, *key);}
  }
  naClearTreeIterator(&iter);
  t2 = naMakeDateTimeNow();
  printf("Tree Iterate (%d): %f" NA_NL, i, naGetDateTimeDifference(&t2, &t1));

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
  printf("Tree Insert/Remove: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));

  naClearTree(&tree);
  naReleaseTreeConfiguration(config);
}



#include TRYOUT_NALIB_PATH(NAKey.h)

void testDateTimeHeap(){
  NAHeap heap;
  NADateTime t1;
  NADateTime t2;
  NADateTime t3;
  int i;
  NADateTime prevkey = naMakeDateTimeWithNALibSecondNumber(-1E20);
  NADateTime* keys;

  keys = (NADateTime*)naMalloc(TESTSIZE * sizeof(NADateTime));
  for(i = 0; i < TESTSIZE; i++){
    keys[i] = naMakeDateTimeWithNALibSecondNumber((int64)(naUniformRandZE() * 1.E12 - 1.E11));
  }

  naInitHeap(&heap, -1, NA_HEAP_USES_DATETIME_KEY);
  
  t1 = naMakeDateTimeNow();
  for(i = 0; i < TESTSIZE; i++){
    naInsertHeapElementConst(&heap, &(keys[i]), &(keys[i]), NA_NULL);
  }

  t2 = naMakeDateTimeNow();

  for(i = 0; i < TESTSIZE; i++){
    const NADateTime* curkey = naRemoveHeapRootConst(&heap);
    if(NA_KEY_OP(Greater, NADateTime)(&prevkey, curkey)){
      printf("Error in sorting");
    }
    prevkey = *curkey;
    //NAString* datestring = naNewStringWithDateTime(curkey, NA_DATETIME_FORMAT_NATURAL);
    //printf("%s" NA_NL, naGetStringUTF8Pointer(datestring));
    //naDelete(datestring);
  }

  t3 = naMakeDateTimeNow();

  printf("Heap Insert: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
  printf("Heap Iterate: %f" NA_NL, naGetDateTimeDifference(&t3, &t2));

  naClearHeap(&heap);
  
  naFree(keys);
}




#include <stdio.h>

int main(void){
  double* keys;
  int i;

  printf("NALib Version: %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug ");
  #else
    printf("(Release ");
  #endif
  printf("%d Bits Addresses, %d Bits Integers)" NA_NL, NA_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();

    testDateTimeHeap();

    keys = (double*)naMalloc(TESTSIZE * sizeof(double));
    for(i = 0; i < TESTSIZE; i++){
      keys[i] = naUniformRandZE();
//      keys[i] = TESTSIZE - i;
//      keys[i] = i;
    }
    testTree();
    testHeap(keys);
    testQSort(keys);
    naFree(keys);
  naStopRuntime();

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished." NA_NL);
    NA_UNUSED(getchar());
  #endif

  return 0;
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
