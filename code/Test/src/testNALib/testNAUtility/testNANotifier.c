
#include "NATest.h"
#include "NAUtility/NANotifier.h"
#include <stdio.h>



static int testValue;
void testMessageCallback(NAMessage message){
//  printf("Message recieved from object %p with data %p.\n", message.sender, message.data);
  testValue++;
}

int valueArray[3];
size_t valuePos = 0;
void addValueCallback(NAMessage message){
  int* data = (int*)message.data;
  valueArray[valuePos] = *data;
  valuePos++;
}

enum MySignals {
  msgUpdate,
  msgCreate,
  msgDelete,
  SIGNAL_COUNT
};



void testNANotifierCreationAndSetup(){
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

    // The first id returned must be 1, not 0.
    naTestVoid(topicId = naRegisterTopic(count));
    naTest(topicId == 1);

    naTestVoid(
      for(size_t i = 0; i < 20; ++i){
        topicId = naRegisterTopic(count);
      }
    )
    naTest(topicId == 21);

    naTestVoid(naDeallocNotifier(notifier));
  }
}



void testNANotifierPublishAndSubscribe(){
  naTestCrash(naSubscribe(NA_NULL, 1234, 1234, NA_NULL, NA_NULL));
  naTestCrash(naPublish(NA_NULL, 1234, 1234, NA_NULL));
  naTestCrash(naRunNotifier());

  naTestGroup("Set signals") {
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);
    size_t topicId = naRegisterTopic(SIGNAL_COUNT);

    naTestVoid(naSetSignalPriority(topicId, msgCreate, NA_SIGNAL_PRIORITY_CREATE));
    naTestVoid(naSetSignalPriority(topicId, msgDelete, NA_SIGNAL_PRIORITY_DELETE));

    naTestCrash(naSetSignalPriority(1234, 1234, 1234));
    naTestCrash(naSetSignalPriority(topicId, 1234, 1234));
    naTestError(naSetSignalPriority(topicId, msgCreate, 1234));

    naDeallocNotifier(notifier);
  }

  naTestGroup("Simple roundtrip") {
    int testObject = 9999;
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);
    size_t topicId = naRegisterTopic(SIGNAL_COUNT);

    naTestCrash(naSubscribe(NA_NULL, 1234, 1234, NA_NULL, NA_NULL));
    naTestCrash(naSubscribe(NA_NULL, topicId, 1234, NA_NULL, NA_NULL));
    naTestCrash(naSubscribe(NA_NULL, topicId, msgUpdate, NA_NULL, NA_NULL));
    naTestCrash(naPublish(NA_NULL, 1234, 1234, NA_NULL));
    naTestCrash(naPublish(NA_NULL, topicId, 1234, NA_NULL));

    testValue = 1000;
    naTestVoid(naSubscribe(NA_NULL, topicId, msgUpdate, NA_NULL, testMessageCallback));
    naTestVoid(naPublish(&testObject, topicId, msgUpdate, NA_NULL));
    naTestVoid(naRunNotifier());
    naTest(testValue == 1001);

    naDeallocNotifier(notifier);
  }

  naTestGroup("Unsubscribing") {
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);
    size_t topicId = naRegisterTopic(SIGNAL_COUNT);

    void* subscription = naSubscribe(NA_NULL, topicId, msgUpdate, NA_NULL, testMessageCallback);
    naTestVoid(naUnsubscribe(subscription));

    naDeallocNotifier(notifier);
  }

  naTestGroup("Priority messages") {
    NANotifier* notifier = naAllocNotifier();
    naSetCurrentNotifier(notifier);
    size_t topicId = naRegisterTopic(SIGNAL_COUNT);

    naTestVoid(naSetSignalPriority(topicId, msgUpdate, NA_SIGNAL_PRIORITY_UPDATE));
    naTestVoid(naSetSignalPriority(topicId, msgCreate, NA_SIGNAL_PRIORITY_CREATE));
    naTestVoid(naSetSignalPriority(topicId, msgDelete, NA_SIGNAL_PRIORITY_DELETE));
    naSubscribe(NA_NULL, topicId, msgUpdate, NA_NULL, addValueCallback);
    naSubscribe(NA_NULL, topicId, msgCreate, NA_NULL, addValueCallback);
    naSubscribe(NA_NULL, topicId, msgDelete, NA_NULL, addValueCallback);
    int value1 = 1;
    int value2 = 2;
    int value3 = 3;
    naPublish(NA_NULL, topicId, msgUpdate, &value1);
    naPublish(NA_NULL, topicId, msgCreate, &value2);
    naPublish(NA_NULL, topicId, msgDelete, &value3);
    naRunNotifier();
    naTest(valueArray[0] == 3);
    naTest(valueArray[1] == 2);
    naTest(valueArray[2] == 1);

    naDeallocNotifier(notifier);
  }
}



void testNANotifier(void){
  naTestFunction(testNANotifierCreationAndSetup);
  naTestFunction(testNANotifierPublishAndSubscribe);
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
