
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

typedef struct NAMessage NAMessage;
struct NAMessage {
  void* reciever;
  void* sender;
  void* data;
};

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
NA_API void naRunNotifier(void);

// Registers a topic and returns a new topicId. You can have multiple signals
// per topic, all numbered from 0 to signalCount - 1. This means, it is most
// practical to use an enum to define the signals. By default, all signals
// have the signal priority UPDATE.
// The returned topicId is always > 0.
NA_API size_t naRegisterTopic(size_t signalCount);

// Sets the priority of the signal. Initial priority of any signal is UPDATE.
NA_API void naSetSignalPriority(
  size_t topicId,
  size_t signalId,
  SignalPriority priority);

// Registers a subscription
// - object:    Denotes the object to observe. If NULL, any object is observed
//              which broadcast the message. The object provided here is const
//              The object later actually sending the message will be provided
//              as the sender parameter in the callback function which might
//              be non-const.
// - topicID:   The topic the signal belongs to.
// - signalId:  The signal id to listen to.
// - reciever:  A pointer to the reciever. This information will be provided in
//              the callback as the first parameter. Can be NULL if no specific
//              object is needed.
// - callback:  The callback function which will be called.
// Returns a subscription pointer to an internal opaque type. Use that to
// unsubscribe again using naUnsubscribe.
NA_API void* naSubscribe(
  const void* object,
  size_t topicId,
  size_t signalId,
  void* reciever,
  NAMessageCallback callback);

// Unsubscribes the given subscription.
// You can send Null as a subscription which does nothing.
NA_API void naUnsubscribe(
  void* subscription);

// Sends a message which will be distributed to all listeners next time the
// notifier is run.
// - sender:    The object which causes this message. This information will be
//              provided in the callback as the sender parameter. Can be NULL
//              if no specific object is responsible.
// - topicID:   The topic the signal belongs to.
// - signalId:  The signal id to listen to.
// - data:      A data package containing the data which shall be given the
//              callback as the data parameter. Can be NULL.
NA_API void naPublish(
  void* sender,
  size_t topicId,
  size_t signalId,
  void* data);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_NOTIFIER_INCLUDED
