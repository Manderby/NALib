
#include "NATest.h"
#include "NAUtility/NANotifier.h"
#include <stdio.h>



static int testValue;
void testMessageCallback(NAMessage message){
//  printf("Message recieved from object %p with data %p.\n", message.sender, message.data);
  testValue++;
}



void testNANotifier(void){
  enum MySignals {
    msg0,
    msg1,
    SIGNAL_COUNT
  };

  naTestGroup("Start and Stop") {
    naTest(naGetCurrentNotifier() == NA_NULL);
    NANotifier* notifier = naAllocNotifier();
    naTest(naGetCurrentNotifier() == NA_NULL);
    naSetCurrentNotifier(notifier);
    naTest(naGetCurrentNotifier() == notifier);
    naDeallocNotifier(notifier);
    naTest(naGetCurrentNotifier() == NA_NULL);
  }

  naTestGroup("Register Topics") {
    // No notifier running -> crash
    naTestCrash(naRegisterTopic(1234));
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);

    // Zero signals not allowed.
    naTestCrash(naRegisterTopic(0));

    size_t topicId = 0;
    const size_t count = 2;

    naTestVoid(topicId = naRegisterTopic(count));
    naTest(topicId == 0);

    naTestVoid(
      for(size_t i = 0; i < 20; ++i){
        topicId = naRegisterTopic(count);
      }
    )
    naTest(topicId == 20);

    naTestVoid(naDeallocNotifier(notifier));
  }

  naTestGroup("Set signals") {
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);
    size_t topicId = naRegisterTopic(SIGNAL_COUNT);

    naTestVoid(naSetSignalPriority(topicId, msg0, NA_SIGNAL_PRIORITY_CREATE));

    naTestCrash(naSetSignalPriority(1234, 1234, 1234));
    naTestCrash(naSetSignalPriority(topicId, 1234, 1234));
    naTestError(naSetSignalPriority(topicId, msg0, 1234));
    
    naDeallocNotifier(notifier);
  }

  naTestGroup("Simple roundtrip") {
  
    naTestCrash(naSubscribe(NA_NULL, 1234, 1234, NA_NULL, NA_NULL));
    naTestCrash(naPublish(NA_NULL, 1234, 1234, NA_NULL));
    naTestCrash(naRunNotifier());

    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);
    size_t topicId = naRegisterTopic(SIGNAL_COUNT);
    int testObject = 9999;

    naTestCrash(naSubscribe(NA_NULL, 1234, 1234, NA_NULL, NA_NULL));
    naTestCrash(naSubscribe(NA_NULL, topicId, 1234, NA_NULL, NA_NULL));
    naTestCrash(naSubscribe(NA_NULL, topicId, msg0, NA_NULL, NA_NULL));
    naTestCrash(naPublish(NA_NULL, 1234, 1234, NA_NULL));
    naTestCrash(naPublish(NA_NULL, topicId, 1234, NA_NULL));

    testValue = 1000;
    naTestVoid(naSubscribe(NA_NULL, topicId, msg0, NA_NULL, testMessageCallback));
    naTestVoid(naPublish(&testObject, topicId, msg0, NA_NULL));
    naTestVoid(naRunNotifier());
    naTest(testValue == 1001);

    naDeallocNotifier(notifier);
  }

  naTestGroup("Unsubscribing") {
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);
    size_t topicId = naRegisterTopic(SIGNAL_COUNT);
  
    void* subscription = naSubscribe(NA_NULL, topicId, msg0, NA_NULL, testMessageCallback);
    naTestVoid(naUnsubscribe(subscription, topicId, msg0));

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
