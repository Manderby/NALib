
#ifndef NA_NOTIFIER_INCLUDED
#define NA_NOTIFIER_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include "../NABase/NABase.h"


typedef struct NANotifier NANotifier;

typedef enum{
  NA_MESSAGE_TYPE_UPDATE,   // lowest prio
  NA_MESSAGE_TYPE_CREATE,
  NA_MESSAGE_TYPE_DELETE,   // highest prio
} NotifierMessageType;



// Starts and stops a notifier. Note that you still need to call naGetCurrentNotifier if you want the new notifier to
// be the current one.
NA_API NANotifier* naAllocNotifier(void);
NA_API void naDeallocNotifier(NANotifier* notifier);

// Get and set the current notifier. The current notifier will be used when calling naPubMessage and naSubMessage.
NA_API NANotifier* naGetCurrentNotifier(void);
NA_API void naSetCurrentNotifier(NANotifier* notifier);

// Registers a topic and returns a new topicId. You can have multiple messages per topic, all numbered from 0 to messageCount-1
// This means, it is most practical to use an enum to define the messages.
// By default, all messages have the message type UPDATE.
NA_API size_t naRegisterTopic(size_t messageCount);

// Sets the type of the depicted message. The initial type of any message is UPDATE.
NA_API void naSetMessageType(size_t topicId, size_t message, NotifierMessageType type);




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_NOTIFIER_INCLUDED
