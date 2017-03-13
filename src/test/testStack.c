

#include "tests.h"

#include "../NALib/NAStack.h"
#include "../NALib/NADateTime.h"


#define NA_TEST_STACK_M_COUNT 1
#define NA_TEST_STACK_COUNT (NA_TEST_STACK_M_COUNT * 1000000)


void naTestStackDoubleFunc(void* data){
  int* intdata = (int*)data;
  *intdata = 2 * *intdata;
}


void testStack(void){
  NAStack stack;
  int i;
  int* v;
  NADateTime time1;
  NADateTime time2;
  NAInt stackcount;
  NAInt stackreservedcount;
  int64 sum;
  
  printf("\n");
  printf("Stack Test\n");
  printf("==========\n");
  printf("Init stack with space for 2 integers. Pushing %d M values from 1 to %d... ", NA_TEST_STACK_M_COUNT, NA_TEST_STACK_COUNT);
  time1 = naMakeDateTimeNow();
  naInitStack(&stack, sizeof(int), 2);
  for(i=0; i<NA_TEST_STACK_COUNT; i++){
    *(int*)naPushStack(&stack) = i + 1;
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  printf("Top of stack: %d\n", *(int*)naTopStack(&stack));

  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);

  printf("Pop'ing nine thenth of the values... ");
  time1 = naMakeDateTimeNow();
  for(i=0; i<(int)(NA_TEST_STACK_COUNT * 0.9); i++){
    v = (int*)naPopStack(&stack);
    // you can still use *v after this function as long as the value will not
    // get overwritten or the storage will not be deleted either by clearing the
    // stack or by reducing its memory footprint with naShrinkStackIfNecessary.
    *v = 0;
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  printf("Top of stack: %d\n", *(int*)naTopStack(&stack));

  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);
  
  printf("Shrinking stack normally...\n");
  naShrinkStackIfNecessary(&stack, NA_FALSE);
  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);

  printf("Shrinking stack aggressively...\n");
  naShrinkStackIfNecessary(&stack, NA_TRUE);
  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);
  
  printf("Clearing the stack.\n");
  naClearStack(&stack);

  // ///////////////////////////////////

  printf("\nInitializing a new stack, this time with sufficient initial space.\n");
  naInitStack(&stack, sizeof(int), NA_TEST_STACK_COUNT);
  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);

  printf("Pushing %d M values... ", NA_TEST_STACK_M_COUNT);
  time1 = naMakeDateTimeNow();
  for(i=0; i<NA_TEST_STACK_COUNT; i++){
    *(int*)naPushStack(&stack) = i + 1;
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);

  printf("Pushing 9 times more values... ");
  time1 = naMakeDateTimeNow();
  for(i=0; i<NA_TEST_STACK_COUNT * 9; i++){
    *(int*)naPushStack(&stack) = i + NA_TEST_STACK_COUNT + 1;
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);

  printf("Pop'ing 99/100 of the values... ");
  time1 = naMakeDateTimeNow();
  for(i=0; i<(int)(NA_TEST_STACK_COUNT * 9.9); i++){
    v = (int*)naPopStack(&stack);
    *v = 0;
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);

  printf("Creating iterator and computing sum. Should be %lld...\n", ((int64)NA_TEST_STACK_COUNT / 10 + 1) * ((int64)NA_TEST_STACK_COUNT / 20));
  time1 = naMakeDateTimeNow();
  sum = 0;
  NAStackIterator iter = naMakeStackIteratorAccessor(&stack);
  while(naIterateStack(&iter)){
    sum += (int64)*(int*)naGetStackCurrentConst(&iter);
  }
  naClearStackIterator(&iter);
  time2 = naMakeDateTimeNow();
  printf("Result is %lld, took %f seconds\n", sum, naGetDateTimeDifference(&time2, &time1));

  printf("Foreaching with a function doubling all numbers... ");
  time1 = naMakeDateTimeNow();
  naForeachStackMutable(&stack, naTestStackDoubleFunc);
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  printf("Top of stack: %d\n", *(int*)naTopStack(&stack));

  printf("Shrinking stack (should not go below the initial element count)...\n");
  naShrinkStackIfNecessary(&stack, NA_TRUE);
  stackcount = naGetStackCount(&stack);
  stackreservedcount = naGetStackReservedCount(&stack);
  printf("Used / Reserved space in stack: %" NA_PRIu " / %" NA_PRIu " Elements (%.2f%%)\n", stackcount, stackreservedcount, ((float)stackcount / stackreservedcount) * 100.);

  printf("Clearing the stack.\n");
  naClearStack(&stack);
}


