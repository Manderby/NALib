#include "../../NALib/src/NALib.h"

int stackExample(void){

  naStartRuntime();
  
//  int a = 12;
//  int b = 34;
//  int c = 56;

  NAStack stack;
  naInitStack(&stack, sizeof(int), 0, 0);
  *(int*)naPushStack(&stack) = 3;
  *(int*)naPushStack(&stack) = 2;
  *(int*)naPushStack(&stack) = 1;
  printf("Stacking elements: ");
  printf("%d ", *(int*)naPopStack(&stack));
  printf("%d ", *(int*)naPopStack(&stack));
  printf("%d ", *(int*)naPopStack(&stack));
  naClearStack(&stack);

  printf(NA_NL NA_NL);
  
  naStopRuntime();

  return 0;
}
