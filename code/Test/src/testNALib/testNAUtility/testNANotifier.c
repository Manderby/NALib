
#include "NATest.h"
#include "NAUtility/NANotifier.h"
#include <stdio.h>



void testNANotifier(void){
  naTestGroup("Start and Stop"){
    naTest(naGetCurrentNotifier() == NA_NULL);
    NANotifier* notifier = naAllocNotifier();
    naTest(naGetCurrentNotifier() == NA_NULL);
    naSetCurrentNotifier(notifier);
    naTest(naGetCurrentNotifier() == notifier);
    naDeallocNotifier(notifier);
    naTest(naGetCurrentNotifier() == NA_NULL);
  }

  naTestGroup("Define Topic and messages") {
    // No notifier running -> crash
    naTestCrash(naRegisterTopic(1234));
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);

    // Zero messages not allowed.
    naTestCrash(naRegisterTopic(0));

    enum MyMessages{
      msg0,
      msg1,
      count
    };
    size_t topicId = 0;

    naTestVoid(topicId = naRegisterTopic(count));
    naTest(topicId == 0);
    naTestVoid(naSetMessageType(topicId, msg0, NA_MESSAGE_TYPE_CREATE));

    naTestCrash(naSetMessageType(1234, 1234, 1234));
    naTestCrash(naSetMessageType(topicId, 1234, 1234));
    naTestError(naSetMessageType(topicId, msg0, 1234));

    naTestVoid(topicId = naRegisterTopic(count));
    naTest(topicId == 1);
    naTestVoid(topicId = naRegisterTopic(count));
    naTest(topicId == 2);

    naDeallocNotifier(notifier);
  }
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
