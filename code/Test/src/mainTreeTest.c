//
//#define NA_TEST_NALIB_PATH(x) #x;
//
//#include NA_TEST_NALIB_PATH(NATree.h)
//#include NA_TEST_NALIB_PATH(NAHeap.h)
//#include NA_TEST_NALIB_PATH(NARandom.h)
//#include NA_TEST_NALIB_PATH(NAMemory.h)
//
//#define TESTSIZE    1000000
//#define SUBTESTSIZE  100000.
//
//int compare(const void* a, const void* b)
//{
//  if(*(double*)a > *(double*)b){return 1;}
//  else if(*(double*)a < *(double*)b){return -1;}
//  return 0;
//}
//
//void testQSort(){
//  NADateTime t1, t2;
//  int i;
//
//  double* keys = naMalloc(TESTSIZE * sizeof(double));
//  for(i = 0; i < TESTSIZE; i++){
//    keys[i] = naUniformRandZE();
//  }
//
//  t1 = naMakeDateTimeNow();
//  qsort(keys, TESTSIZE, sizeof(keys[0]), compare);
//  t2 = naMakeDateTimeNow();
//  printf("QSort: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
//
//  naFree(keys);
//}
//
//
//
//void testHeap(){
//  NAHeap heap;
//  NADateTime t1, t2;
//  int i;
//
//  double* keys = naMalloc(TESTSIZE * sizeof(double*));
//  naInitHeap(&heap, -1, NA_HEAP_USES_DOUBLE_KEY);
//
//  t1 = naMakeDateTimeNow();
////  iter = naMakeTreeModifier(&tree);
//  for(i = 0; i < TESTSIZE; i++){
//    keys[i] = naUniformRandZE();
////    keys[i] = (double)i / TESTSIZE;
////    printf("%f" NA_NL, key);
//    naInsertHeapElementConst(&heap, &(keys[i]), &(keys[i]), NA_NULL);
//  }
////  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Heap Insert: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
//
////  double prevkey = -1;
//  t1 = naMakeDateTimeNow();
//  for(i = 0; i < TESTSIZE; i++){
////    const double* key = naGetHeapRootKey(&heap);
////    if(*key < prevkey){
////      printf("Wrong sorting: %d: %f, %f" NA_NL, i, *key, prevkey);
////    }
////    prevkey = *key;
//    naRemoveHeapRootConst(&heap);
//  }
//  t2 = naMakeDateTimeNow();
//  printf("Heap Iterate: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
//
//  naClearHeap(&heap);
//  naFree(keys);
//}
//
//
//
//void testTree(){
//  NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_DOUBLE | NA_TREE_BALANCE_AVL);
//  NATree tree;
//  NADateTime t1, t2;
//  NATreeIterator iter;
//  int i;
//  double prevkey;
//
//  naInitTree(&tree, config);
//
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  for(i = 0; i < TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
//    double key = naUniformRandZE();
////    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
////    printf("%f" NA_NL, key);
//    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Insert: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
//
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeAccessor(&tree);
//  for(i = 0; i < TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
////    double key = naUniformRandZE();
//    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
////    double key = (i % 10000) / 10000.;
////    printf("%f" NA_NL, key);
////    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//    naLocateTreeKey(&iter, &key, NA_FALSE);
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Locate: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
//
////  NAInt prevkey = -1.;
//  prevkey = -1.;
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  i = 0;
//  while(naIterateTree(&iter, NA_NULL, NA_NULL)){
//    const double* key;
//    i++;
////    const NAInt* key = naGetTreeCurKey(&iter);
////    if(*key < prevkey){
////      printf("Wrong sorting: %d: %"NA_PRIi", %"NA_PRIi NA_NL, i, *key, prevkey);
////    }
////    prevkey = *key;
////    if(i < 5){printf("%"NA_PRIi NA_NL, *key);}
//
//    key = naGetTreeCurLeafKey(&iter);
//    if(*key < prevkey){
//      printf("Wrong sorting: %d: %f, %f" NA_NL, i, *key, prevkey);
//    }
//    prevkey = *key;
////    if(i < 5){printf("%f" NA_NL, *key);}
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Iterate (%d): %f" NA_NL, i, naGetDateTimeDifference(&t2, &t1));
//
//  // ////////////////
//
////  naEmptyTree(&tree);
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  for(i = 0; i < TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
//    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
//    if(i%5 != 0){
//      naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//    }else{
//      NABool found = naLocateTreeKey(&iter, &key, NA_FALSE);
//      if(found){naRemoveTreeCurLeaf(&iter);}
//    }
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Insert/Remove: %f" NA_NL, naGetDateTimeDifference(&t2, &t1));
//
//  naClearTree(&tree);
//  naRelease(config);
//}
//
//
//
//
//#include <stdio.h>
//
//int main(void){
//  printf("NALib Version: %d ", NA_VERSION);
//  #if NA_DEBUG
//    printf("(Debug ");
//  #else
//    printf("(Release ");
//  #endif
//  printf("%d Bits Addresses, %d Bits Integers)" NA_NL, NA_ADDRESS_BITS, NA_TYPE_NAINT_BITS);
//
//  naStartRuntime();
//    testTree();
////    testHeap();
////    testQSort();
//  naStopRuntime();
//
//  #if NA_OS == NA_OS_WINDOWS
//    printf("Finished." NA_NL);
//    NA_UNUSED(getchar());
//  #endif
//
//  return 0;
//}
//
//
//
//// This is free and unencumbered software released into the public domain.
//
//// Anyone is free to copy, modify, publish, use, compile, sell, or
//// distribute this software, either in source code form or as a compiled
//// binary, for any purpose, commercial or non-commercial, and by any
//// means.
//
//// In jurisdictions that recognize copyright laws, the author or authors
//// of this software dedicate any and all copyright interest in the
//// software to the public domain. We make this dedication for the benefit
//// of the public at large and to the detriment of our heirs and
//// successors. We intend this dedication to be an overt act of
//// relinquishment in perpetuity of all present and future rights to this
//// software under copyright law.
//
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//// OTHER DEALINGS IN THE SOFTWARE.
//
//// For more information, please refer to <http://unlicense.org/>
//
