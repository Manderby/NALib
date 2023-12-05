
#include "../NANotifier.h"

#include "../NABinaryData.h"
#include "../NAMemory.h"



typedef struct NAMessage NAMessage;
struct NAMessage{
  NotifierMessageType type;
};

typedef struct NATopic NATopic;
struct NATopic{
  size_t messageCount;
  NAMessage* messages;
};

struct NANotifier{
  size_t nextTopicId;
  size_t topicsCount;
  NATopic** topics;
};

NANotifier* na_notifier = NA_NULL;



NA_HDEF naInitMessage(NAMessage* message){
  message->type = NA_MESSAGE_TYPE_UPDATE;
}



NA_HDEF NATopic* naAllocTopic(size_t messageCount){
  NATopic* topic = naAlloc(NATopic);
  topic->messageCount = messageCount;
  topic->messages = naMalloc(sizeof(NAMessage) * messageCount);
  for(size_t i = 0; i < messageCount; ++i){
    naInitMessage(&topic->messages[i]);
  }
  return topic;
}

NA_HDEF void naDeallocTopic(NATopic* topic){
  naFree(topic->messages);
  naFree(topic);
}




NA_DEF NANotifier* naAllocNotifier(void){
  NANotifier* notifier = naAlloc(NANotifier);

  notifier->nextTopicId = 0;
  notifier->topicsCount = 1;
  size_t topicsMemSize = sizeof(NATopic*) * notifier->topicsCount;
  notifier->topics = naMalloc(topicsMemSize);
  naZeron(notifier->topics, topicsMemSize);

  return notifier;
}

NA_DEF void naDeallocNotifier(NANotifier* notifier){
  if(notifier == na_notifier){na_notifier = NA_NULL;}
  for(size_t topicId = 0; topicId < notifier->nextTopicId; ++topicId){
    naDeallocTopic(notifier->topics[topicId]);
  }
  naFree(notifier->topics);
  naFree(notifier);
}



NA_DEF NANotifier* naGetCurrentNotifier(void){
  return na_notifier;
}

NA_DEF void naSetCurrentNotifier(NANotifier* notifier){
  na_notifier = notifier;
}



NA_DEF void naRunNotifier(){
  // todo
}



NA_DEF size_t naRegisterTopic(size_t messageCount){
  #if NA_DEBUG
    if(!na_notifier)
      naCrash("No current notifier present.");
    if(!messageCount)
      naCrash("messageCount is zero.");
  #endif
  size_t newTopicId = na_notifier->nextTopicId;
  na_notifier->nextTopicId += 1;
  if(newTopicId >= na_notifier->topicsCount){
    // The array is too small. Double the array size and copy everything over.
    size_t topicsMemSize = sizeof(NATopic*) * na_notifier->topicsCount;
    NATopic** newArray = naMalloc(topicsMemSize * 2);
    naCopyn(newArray, na_notifier->topics, topicsMemSize);
    naZeron(newArray + na_notifier->topicsCount, topicsMemSize);
    na_notifier->topicsCount *= 2;
    naFree(na_notifier->topics);
    na_notifier->topics = newArray;
  }
  na_notifier->topics[newTopicId] = naAllocTopic(messageCount);
  return newTopicId;
}



NA_DEF void naSetMessageType(size_t topicId, size_t messageId, NotifierMessageType type){
  #if NA_DEBUG
    if (!na_notifier)
      naCrash("No current notifier present.");
    if (topicId >= na_notifier->topicsCount)
      naCrash("Unknown topicId.");
    if (messageId >= na_notifier->topics[topicId]->messageCount)
      naCrash("Unknown message id.");
    if(type < NA_MESSAGE_TYPE_UPDATE || type > NA_MESSAGE_TYPE_DELETE)
      naError("Unknown type");
#endif
  na_notifier->topics[topicId]->messages[messageId].type = type;
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
