#include "../../NALib/src/NALib.h"

int listExample(void){

  naStartRuntime();
  
  int a = 12;
  int b = 34;
  int c = 56;

  NAList list;
  naInitList(&list);
  naAddListLastConst(&list, &a);
  naAddListLastConst(&list, &b);
  naAddListLastConst(&list, &c);
  printf("Listing elements: ");
  NAListIterator listIter = naMakeListAccessor(&list);
  while(naIterateList(&listIter)){
    printf("%d ", *(const int*)naGetListCurConst(&listIter));
  }
  naClearListIterator(&listIter);
  naClearList(&list);

  printf(NA_NL NA_NL);
  
  naStopRuntime();

  return 0;
}
