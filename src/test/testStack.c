

#include "stdio.h"
#include "NAStack.h"
#include "../NALib/NADateTime.h"



#define NA_TEST_STACK_M_COUNT 10
#define NA_TEST_STACK_COUNT (NA_TEST_STACK_M_COUNT * 1000000)

void testStack(void){
  NAStack stack1;
  int i;
  int* v;
  NADateTime time1;
  NADateTime time2;
  
  printf("\n");
  printf("Stack Test\n");
  printf("==========\n");
  printf("Init stack with space for 2 integers. Pushing %d M values from 0 to %d... ", NA_TEST_STACK_M_COUNT, NA_TEST_STACK_COUNT - 1);
  time1 = naMakeDateTimeNow();
  naInitStack(&stack1, sizeof(int), 2);
  for(i=0; i<NA_TEST_STACK_COUNT; i++){
    *(int*)naPushStack(&stack1) = i;
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  printf("Top of stack: %d\n", *(int*)naTopStack(&stack1));

  printf("Used / Reserved space in stack: %d / %d Elements\n", naGetStackCount(&stack1), naGetStackReservedCount(&stack1));

  printf("Pop'ing half of the values... ");
  time1 = naMakeDateTimeNow();
  for(i=0; i<NA_TEST_STACK_COUNT / 2; i++){
    naPopStack(&stack1);
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  printf("Top of stack: %d\n", *(int*)naTopStack(&stack1));
}

