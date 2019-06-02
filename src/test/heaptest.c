//
//// This file is part of NALib, a collection of C source code.
//// Full license notice at the bottom.
//
//
////
//// This is just a small file to get you started using NALib.
////
//// This file should compile and run and print some version notes on the screen.
////
//
//#include "../NALib/NATree.h"
//#include "../NALib/NAHeap.h"
//#include "../NALib/NAHeap.h"
//#include "../NALib/NARandom.h"
//
//#define TESTSIZE    100000
//#define SUBTESTSIZE  10000.
//
//int compare(const void* a, const void* b)
//{
//  if(*(double*)a > *(double*)b){return 1;}
//  else if(*(double*)a < *(double*)b){return -1;}
//  return 0;  
//}
//
//void testQSort(double* keys){
//  NADateTime t1, t2;
//
//  t1 = naMakeDateTimeNow();
//  qsort(keys, TESTSIZE, sizeof(keys[0]), compare);
//  t2 = naMakeDateTimeNow();
//  printf("QSort: %f\n", naGetDateTimeDifference(&t2, &t1));
//
//}
//
//
//
//void testHeap(const double* keys){
//  NAHeap heap;
//  NADateTime t1;
//  NADateTime t2;
//  NADateTime t3;
//  int i;
//  double prevkey = 0.;
//
//  naInitHeap(&heap, -1, NA_HEAP_USES_DOUBLE_KEY);
//  
//  t1 = naMakeDateTimeNow();
//  for(i=0; i<TESTSIZE; i++){
//    naInsertHeapElementConst(&heap, &(keys[i]), &(keys[i]), NA_NULL);
//  }
//
//  t2 = naMakeDateTimeNow();
//
//  for(i=0; i<TESTSIZE; i++){
//    const double* curkey = naRemoveHeapRootConst(&heap);
//    if(prevkey > *curkey){
//      printf("Error in sorting");
//    }
//    prevkey = *curkey;
////    printf("%f\n", *curkey);
//  }
//
//  t3 = naMakeDateTimeNow();
//
//  printf("Heap Insert: %f\n", naGetDateTimeDifference(&t2, &t1));
//  printf("Heap Iterate: %f\n", naGetDateTimeDifference(&t3, &t2));
//
//  naClearHeap(&heap);
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
//  double prevkey = -1.;
//
//  naInitTree(&tree, config); 
//
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  for(i=0; i<TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
//    double key = naUniformRandZE();
////    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
////    printf("%f\n", key);
//    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Insert: %f\n", naGetDateTimeDifference(&t2, &t1));
//
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeAccessor(&tree);
//  for(i=0; i<TESTSIZE; i++){
////    NAInt key = (NAInt)(naUniformRandZE() * TESTSIZE);
////    double key = naUniformRandZE();
//    double key = (int)(naUniformRandZE() * SUBTESTSIZE) / SUBTESTSIZE;
////    double key = (double)i / TESTSIZE;
////    double key = (i % 10000) / 10000.;
////    printf("%f\n", key);
////    naAddTreeKeyConst(&iter, &key, NA_NULL, NA_TRUE);
//    naLocateTreeKey(&iter, &key, NA_FALSE);
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Locate: %f\n", naGetDateTimeDifference(&t2, &t1));
//
////  NAInt prevkey = -1.;
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  i = 0;
//  while(naIterateTree(&iter, NA_NULL, NA_NULL)){
//    const double* key;
//    i++;
////    const NAInt* key = naGetTreeCurKey(&iter);
////    if(*key < prevkey){
////      printf("Wrong sorting: %d: %"NA_PRIi", %"NA_PRIi"\n", i, *key, prevkey);
////    }
////    prevkey = *key;
////    if(i < 5){printf("%"NA_PRIi"\n", *key);}
//
//    key = naGetTreeCurLeafKey(&iter);
//    if(*key < prevkey){
//      printf("Wrong sorting: %d: %f, %f\n", i, *key, prevkey);
//    }
//    prevkey = *key;
////    if(i < 50){printf("%f\n", *key);}
//  }
//  naClearTreeIterator(&iter);
//  t2 = naMakeDateTimeNow();
//  printf("Tree Iterate (%d): %f\n", i, naGetDateTimeDifference(&t2, &t1));
//
//  // ////////////////
//
////  naEmptyTree(&tree);
//  t1 = naMakeDateTimeNow();
//  iter = naMakeTreeModifier(&tree);
//  for(i=0; i<TESTSIZE; i++){
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
//  printf("Tree Insert/Remove: %f\n", naGetDateTimeDifference(&t2, &t1));
//
//  naClearTree(&tree);
//  naReleaseTreeConfiguration(config);
//}
//
//
//
//
//#include "../NALib/NABase.h"
//#include <stdio.h>
//
//int main(void){
//  double* keys;
//  int i;
//
//  printf("NALib Version: %d ", NA_VERSION);
//  #ifndef NDEBUG
//    printf("(Debug ");
//  #else
//    printf("(Release ");
//  #endif
//  printf("%d Bits Addresses, %d Bits Integers)\n", NA_SYSTEM_ADDRESS_BITS, NA_TYPE_NAINT_BITS);
//
//  naStartRuntime();
//    keys = (double*)naMalloc(TESTSIZE * sizeof(double));
//    for(i=0; i<TESTSIZE; i++){
//      keys[i] = naUniformRandZE();
////      keys[i] = TESTSIZE - i;
////      keys[i] = i;
//    }
//    testTree();
//    testHeap(keys);
//    testQSort(keys);
//    naFree(keys);
//  naStopRuntime();
//
//  #if NA_OS == NA_OS_WINDOWS
//    printf("Finished.\n");
//    NA_UNUSED(getchar());
//  #endif
//
//  return 0;
//}
//
//
//// Copyright (c) NALib, Tobias Stamm
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
