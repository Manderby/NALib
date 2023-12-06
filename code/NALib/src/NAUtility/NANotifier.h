
#ifndef NA_NOTIFIER_INCLUDED
#define NA_NOTIFIER_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include "../NABase/NABase.h"



typedef struct NANotifier NANotifier;

typedef enum{
  NA_SIGNAL_PRIORITY_UPDATE,   // lowest prio
  NA_SIGNAL_PRIORITY_CREATE,
  NA_SIGNAL_PRIORITY_DELETE,   // highest prio
} SignalPriority;

typedef enum{
  void* reciever;
  void* sender;
  void* data;
} NAMessage;

typedef void(*NAMessageCallback)(NAMessage message);



// Starts and stops a notifier. You still need to call naGetCurrentNotifier if
// you want the new notifier to be the current one.
NA_API NANotifier* naAllocNotifier(void);
NA_API void naDeallocNotifier(NANotifier* notifier);

// Get and set the current notifier. The current notifier will be used when
// calling naPublish and naSubscribe.
NA_API NANotifier* naGetCurrentNotifier(void);
NA_API void naSetCurrentNotifier(NANotifier* notifier);

// Performs a full run where all messages including follow-up messages will be
// distributed. This function usually is called in some kind of application
// loop on a regular basis.
NA_API void naRunNotifier();

// Registers a topic and returns a new topicId. You can have multiple signals
// per topic, all numbered from 0 to signalCount - 1. This means, it is most
// practical to use an enum to define the signals. By default, all signals
// have the signal priority UPDATE.
NA_API size_t naRegisterTopic(size_t signalCount);

// Sets the priority of the signal. Initial priority of any signal is UPDATE.
NA_API void naSetSignalPriority(
  size_t topicId,
  size_t signalId,
  SignalPriority priority);

// Registers a subscription
// - object:    Denotes the object to observe. If NULL, any object is observed
//              which broadcast the message. The object actually sending the
//              message will be provided as the sender parameter in the
//              callback function.
// - topicID:   The topic the signal belongs to.
// - signalId:  The signal id to listen to.
// - reciever:  A pointer to the reciever. This information will be provided in
//              the callback as the first parameter. Can be NULL if no specific
//              object is needed.
// - callback:  The callback function which will be called.
// Returns a subscription id which can be used to unsubscribe again.
NA_API size_t naSubscribe(
  void* object,
  size_t topicID,
  size_t signalId,
  void* reciever,
  NAMessageCallback callback);

// Unsubscribes the given subscription.
NA_API void naUnsubscribe(size_t subscriptionId);

// Sends a message which will be distributed to all listeners next time the
// notifier is run.
// - sender:    The object which causes this message. This information will be
//              provided in the callback as the sender parameter. Can be NULL
//              if no specific object is responsible.
// - topicID:   The topic the signal belongs to.
// - signalId:  The signal id to listen to.
// - data:      A data package containing the data which shall be given the
//              callback as the data parameter. Can be NULL.
// Returns a publication id which is only valid until the notifier did send the
// message.
NA_API size_t naPublish(
  void* sender,
  size_t topicId,
  size_t signalId,
  void* data);

// Unpublishes the given publication
NA_API void naUnpublish(size_t publicationID);

// Publishes the message behind the given publication id including all
// follow-up messages immediately. This function does not return until all of
// these messages are done. Other messages which are still waiting in the
// notifier to be published remain unpublished but messages which are handeled
// by this call will be removed automatically.
NA_API void naPublishNow(size_t publicationID);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_NOTIFIER_INCLUDED
