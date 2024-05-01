
#include "../NANotifier.h"

#include "../NABinaryData.h"
#include "../NAMemory.h"
#include "../../NAStruct/NAList.h"



typedef struct NA_MessageDispatch NA_MessageDispatch;
struct NA_MessageDispatch{
  NAMessage message;
  NAMessageCallback callback;
};
NA_RUNTIME_TYPE(NA_MessageDispatch, NA_NULL, NA_FALSE);

typedef struct NA_Subscription NA_Subscription;
struct NA_Subscription{
  const void* object;
  void* reciever;
  NAMessageCallback callback;
};
NA_RUNTIME_TYPE(NA_Subscription, NA_NULL, NA_FALSE);

typedef struct NA_Signal NA_Signal;
struct NA_Signal{
  SignalPriority priority;
  NAList subscriptions;
};

typedef struct NA_Topic NA_Topic;
struct NA_Topic{
  size_t signalCount;
  NA_Signal* signals;
};

struct NANotifier{
  size_t nextTopicId;
  size_t reservedTopicsCount;
  NA_Topic** topics;
  NAList updateQueue;
  NAList createQueue;
  NAList deleteQueue;
};

NANotifier* na_notifier = NA_NULL;



NA_HDEF NA_MessageDispatch* na_NewMessageDispatch(
  void* reciever,
  void* sender,
  void* data,
  NAMessageCallback callback)
{
  NA_MessageDispatch* messageDispatch = naNew(NA_MessageDispatch);
  messageDispatch->message.reciever = reciever;
  messageDispatch->message.sender = sender;
  messageDispatch->message.data = data;
  messageDispatch->callback = callback;
  return messageDispatch;
}



NA_HDEF NA_Subscription* na_NewSubscription(
  const void* object,
  void* reciever,
  NAMessageCallback callback)
{
  NA_Subscription* subscription = naNew(NA_Subscription);
  subscription->object = object;
  subscription->reciever = reciever;
  subscription->callback = callback;
  return subscription;
}



NA_HDEF void na_InitSignal(NA_Signal* signal){
  signal->priority = NA_SIGNAL_PRIORITY_UPDATE;
  naInitList(&signal->subscriptions);
}

NA_HDEF void na_ClearSignal(NA_Signal* signal){
  naForeachListMutable(&signal->subscriptions, naDelete);
  naClearList(&signal->subscriptions);
}



NA_HDEF NA_Topic* na_AllocTopic(size_t signalCount){
  NA_Topic* topic = naAlloc(NA_Topic);
  topic->signalCount = signalCount;
  topic->signals = naMalloc(sizeof(NA_Signal) * signalCount);
  
  for(size_t i = 0; i < signalCount; ++i){
    na_InitSignal(&topic->signals[i]);
  }
  
  return topic;
}

NA_HDEF void na_DeallocTopic(NA_Topic* topic){
  for(size_t i = 0; i < topic->signalCount; ++i){
    na_ClearSignal(&topic->signals[i]);
  }

  naFree(topic->signals);
  naFree(topic);
}




NA_DEF NANotifier* naAllocNotifier(void){
  NANotifier* notifier = naAlloc(NANotifier);

  notifier->nextTopicId = 1;
  notifier->reservedTopicsCount = 1;
  size_t topicsMemSize = sizeof(NA_Topic*) * notifier->reservedTopicsCount;
  notifier->topics = naMalloc(topicsMemSize);
  naZeron(notifier->topics, topicsMemSize);
  
  naInitList(&notifier->updateQueue);
  naInitList(&notifier->createQueue);
  naInitList(&notifier->deleteQueue);

  return notifier;
}

NA_DEF void naDeallocNotifier(NANotifier* notifier){
  #if NA_DEBUG
    if(!naIsListEmpty(&notifier->updateQueue))
      naError("There are still unscheduled messages in the update queue");
    if(!naIsListEmpty(&notifier->createQueue))
      naError("There are still unscheduled messages in the create queue");
    if(!naIsListEmpty(&notifier->deleteQueue))
      naError("There are still unscheduled messages in the delete queue");
  #endif
  naForeachListMutable(&notifier->updateQueue, naDelete);
  naForeachListMutable(&notifier->createQueue, naDelete);
  naForeachListMutable(&notifier->deleteQueue, naDelete);
  naClearList(&notifier->updateQueue);
  naClearList(&notifier->createQueue);
  naClearList(&notifier->deleteQueue);
  
  if(notifier == na_notifier){na_notifier = NA_NULL;}
  // topicId 0 is invalid.
  for(size_t topicId = 1; topicId < notifier->nextTopicId; ++topicId){
    na_DeallocTopic(notifier->topics[topicId]);
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
  #if NA_DEBUG
    if(!na_notifier)
      naCrash("No current notifier present.");
  #endif

  while(NA_TRUE){
    if(!naIsListEmpty(&na_notifier->deleteQueue)){
      NA_MessageDispatch* messageDispatch = naRemoveListFirstMutable(&na_notifier->deleteQueue);
      messageDispatch->callback(messageDispatch->message);
      naDelete(messageDispatch);
      continue;
    }
    if(!naIsListEmpty(&na_notifier->createQueue)){
      NA_MessageDispatch* messageDispatch = naRemoveListFirstMutable(&na_notifier->createQueue);
      messageDispatch->callback(messageDispatch->message);
      naDelete(messageDispatch);
      continue;
    }
    if(!naIsListEmpty(&na_notifier->updateQueue)){
      NA_MessageDispatch* messageDispatch = naRemoveListFirstMutable(&na_notifier->updateQueue);
      messageDispatch->callback(messageDispatch->message);
      naDelete(messageDispatch);
      continue;
    }
    break;
  }
}



NA_DEF size_t naRegisterTopic(size_t signalCount){
  #if NA_DEBUG
    if(!na_notifier)
      naCrash("No current notifier present.");
    if(!signalCount)
      naCrash("signalCount is zero.");
  #endif
  size_t newTopicId = na_notifier->nextTopicId;
  na_notifier->nextTopicId += 1;
  if(newTopicId >= na_notifier->reservedTopicsCount){
    // The array is too small. Double the array size and copy everything over.
    size_t topicsMemSize = sizeof(NA_Topic*) * na_notifier->reservedTopicsCount;
    NA_Topic** newArray = naMalloc(topicsMemSize * 2);
    naCopyn(newArray, na_notifier->topics, topicsMemSize);
    naZeron(newArray + na_notifier->reservedTopicsCount, topicsMemSize);
    na_notifier->reservedTopicsCount *= 2;
    naFree(na_notifier->topics);
    na_notifier->topics = newArray;
  }
  na_notifier->topics[newTopicId] = na_AllocTopic(signalCount);
  return newTopicId;
}



NA_DEF void naSetSignalPriority(
  size_t topicId,
  size_t signalId,
  SignalPriority priority)
{
  #if NA_DEBUG
    if (!na_notifier)
      naCrash("No current notifier present.");
    if (topicId >= na_notifier->nextTopicId)
      naCrash("Unknown topicId.");
    if (signalId >= na_notifier->topics[topicId]->signalCount)
      naCrash("Unknown signalId.");
    if(priority < NA_SIGNAL_PRIORITY_UPDATE || priority > NA_SIGNAL_PRIORITY_DELETE)
      naError("Unknown type");
  #endif
  na_notifier->topics[topicId]->signals[signalId].priority = priority;
}



NA_DEF void* naSubscribe(
  const void* object,
  size_t topicId,
  size_t signalId,
  void* reciever,
  NAMessageCallback callback)
{
  #if NA_DEBUG
    if (!na_notifier)
      naCrash("No current notifier present.");
    if (topicId >= na_notifier->nextTopicId)
      naCrash("Unknown topicId.");
    if (signalId >= na_notifier->topics[topicId]->signalCount)
      naCrash("Unknown signalId.");
    if (callback == NA_NULL)
      naCrash("callback is Nullpointer");
  #endif
  
  NA_Signal* signal = &na_notifier->topics[topicId]->signals[signalId];
  NA_Subscription* subscription = na_NewSubscription(
    object,
    reciever,
    callback);

  // Todo: Test whether the subscription already exists.
  
  naAddListLastMutable(&signal->subscriptions, subscription);
  
  return subscription;
}



NA_DEF void naUnsubscribe(
  void* subscription)
{
  #if NA_DEBUG
    if (!na_notifier)
      naCrash("No current notifier present.");
  #endif
  if (!subscription) return;

  NABool found = NA_FALSE;
  for(size_t t = 1; t < na_notifier->nextTopicId; ++t){
    for(size_t s = 0; s < na_notifier->topics[t]->signalCount; ++s){
      NA_Signal* signal = &na_notifier->topics[t]->signals[s];
      NAListIterator it = naMakeListModifier(&signal->subscriptions);
      while(naIterateList(&it)){
        NA_Subscription* sub = naGetListCurMutable(&it);
        if(sub == subscription){
          naRemoveListCurMutable(&it, NA_FALSE);
          naDelete(sub);
          break;
        }
      }
      naClearListIterator(&it);
      if(found) break;
    }
    if(found) break;
  }
}



NA_DEF void naPublish(
  void* sender,
  size_t topicId,
  size_t signalId,
  void* data)
{
  #if NA_DEBUG
    if (!na_notifier)
      naCrash("No current notifier present.");
    if (topicId >= na_notifier->nextTopicId)
      naCrash("Unknown topicId.");
    if (signalId >= na_notifier->topics[topicId]->signalCount)
      naCrash("Unknown signalId.");
  #endif
  
  NA_Signal* signal = &na_notifier->topics[topicId]->signals[signalId];
  
  NAList* list;
  switch(signal->priority){
  case NA_SIGNAL_PRIORITY_UPDATE: list = &na_notifier->updateQueue; break;
  case NA_SIGNAL_PRIORITY_CREATE: list = &na_notifier->createQueue; break;
  case NA_SIGNAL_PRIORITY_DELETE: list = &na_notifier->deleteQueue; break;
  }
  
  NAList* subscriptions = &signal->subscriptions;
  NAListIterator it = naMakeListAccessor(subscriptions);
  while(naIterateList(&it)){
    const NA_Subscription* sub = naGetListCurConst(&it);
    if(sub->object == NA_NULL || sub->object == sender){
      NA_MessageDispatch* message = na_NewMessageDispatch(
        sub->reciever,
        sender,
        data,
        sub->callback);
      naAddListLastMutable(list, message);
    }
  }
  naClearListIterator(&it);
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
