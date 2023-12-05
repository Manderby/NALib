
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

typedef void(*NAMessageCallback)(void* reciever, void* sender, void* data);



// Starts and stops a notifier. Note that you still need to call naGetCurrentNotifier if you want the new notifier to
// be the current one.
NA_API NANotifier* naAllocNotifier(void);
NA_API void naDeallocNotifier(NANotifier* notifier);

// Get and set the current notifier. The current notifier will be used when calling naPubMessage and naSubMessage.
NA_API NANotifier* naGetCurrentNotifier(void);
NA_API void naSetCurrentNotifier(NANotifier* notifier);

// Performs a full run where all messages including follow-up messages will be distributed. This function usually is
// called in some kind of application loop on a regular basis.
NA_API void naRunNotifier();

// Registers a topic and returns a new topicId. You can have multiple messages per topic, all numbered from 0 to messageCount-1
// This means, it is most practical to use an enum to define the messages.
// By default, all messages have the message type UPDATE.
NA_API size_t naRegisterTopic(size_t messageCount);

// Sets the type of the depicted message. The initial type of any message is UPDATE.
NA_API void naSetMessageType(size_t topicId, size_t messageId, NotifierMessageType type);

// Registers a subscription to a message.
// - object:    Denotes the object to observe. If NULL, any object is observed which broadcast the message.
//              The object actually sending the message will be provided as the sender parameter in the callback.
// - topicID:   The topic the message belongs to.
// - messageId: The message id to listen to.
// - reciever:  A pointer to the reciever. This information will be provided in the callback as the first parameter.
//              Can be NULL if no specific object is needed.
// - callback:  The callback function which will be called.
// Returns a subscription id which can be used to unsubscribe again.
NA_API size_t naSubscribe(
  void* object,
  size_t topicID,
  size_t messageId,
  void* reciever,
  NAMessageCallback callback);

// Unsubscribes the given subscription.
NA_API void naUnsubscribe(size_t subscriptionId);

// Sends a message which will be distributed to all listeners next time the notifier is run.
// - sender:    The object which causes this message. This information will be provided in the callback as the sender
//              parameter. Can be NULL if no specific object is responsible.
// - topicID:   The topic the message belongs to.
// - messageId: The message id to listen to.
// - data:      A data package containing the data which shall be given the callback as the data parameter. Can be NULL.
// Returns a publication id which is only valid until the notifier did send the message.
NA_API size_t naPublish(
  void* sender,
  size_t topicId,
  size_t messageId,
  void* data);

// Unpublishes the given publication
NA_API naUnpublish(size_t publicationID);

// Publishes the given publication including all follow-up messages immediately. This function does not return until
// all these messages are done. Other messages which are still waiting in the notifier to be published remain unpublished
// but messages which are handeled by the now published notification will be removed automatically.
NA_API naPublishNow(size_t publicationID);

#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_NOTIFIER_INCLUDED
