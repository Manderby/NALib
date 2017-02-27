
#include "stdio.h"

// Note that we link the API with C linkage for testing reasons such that all
// test files can easily be renamed from .c to .cpp and hence being recompiled
// and linked flawlessly.

#ifdef __cplusplus 
extern "C" {
#endif

  void testSystem(void);
  void testMemory(void);
  void testStack(void);
  void testBuffer(void);

#ifdef __cplusplus 
}
#endif
