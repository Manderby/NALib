
#ifndef NA_JSON_INCLUDED
#define NA_JSON_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include <stdio.h>
#include "../NABase/NABase.h"
#include "../NAStruct/NAStack.h"

// This file provides a very simple JSON worker: Parser and writer.
//
// You define rules where to store data and just let the worker run. In the
// end, your storage will be filled with the contents desired. If you haven't
// defined a rule for a JSON entry, it will not be stored.
//
// Note that this is by no means a standard compliant parser/writer.
// Many things are allowed which should not be.



NA_PROTOTYPE(NAJSONWorker);
NA_PROTOTYPE(NAJSONRule);
NA_PROTOTYPE(NAJSONRuleSet);
NA_PROTOTYPE(NABuffer);



// Allocate and deallocate JSON workers.
// After allocation, you create RuleSets to define, where to place the parsed
// items. See further below.

NAJSONWorker* naAllocateJSONWorker(void);
void naDeallocateJSONWorker(NAJSONWorker* worker);

// Parses the given buffer into the given object.
// You can send byteCount = 0 to automatically determine the buffer size.
// Important: The buffer must end with a '\0'
void naParseJSONBuffer(
  NAJSONWorker* worker,
  void* object,
  const void* buf,
  size_t byteCount);

// Creates a new buffer containing the given object transformed into a JSON
// file using the given worker.
NABuffer* naCreateBufferWithJSON(
  NAJSONWorker* worker,
  void* object);



// /////////////////
// RuleSets and Rules

// Creates a new RuleSet stored in the given worker. When the worker gets
// deallocated, all rule sets will be deallocated too.
//
// The last ruleSet being added to a worker is automatically defined as the
// initial ruleSet.

NA_API NAJSONRuleSet* naRegisterJSONRuleSet(
  NAJSONWorker* worker);

// Adds a rule to the given ruleSet.
// For example, to store an Integer which comes from a JSON entry with the key
// "elementcount" in a member named "count" defined in a struct with the type
// "ElementList", you would write:
//
// naAddJSONRule(myRuleSet, "elementcount", naNewJSONRuleInt64(
//   offsetof(ElementList, count)));
//
// Note that the key will not be copied, but only referenced.

NA_API void naAddJSONRule(
  NAJSONRuleSet* ruleSet,
  const NAUTF8Char* key,
  NAJSONRule* rule);

// Following are the rules you can create. Note that the returned pointer shall
// be used for one naAddJSONRule only. The rule will automatically deallocate
// when the ruleSet is deallocated, which in turn deallocates when the worker
// is deallocated.
//
// The offset can be computed by offsetof(MyStruct, member).
//
// JSON Booleans can be stored in Int32 or Int64
// JSON Numbers can be stored in Int32, Int64, Double and Float
// JSON Strings are stored as C-strings. Its memory is allocated with malloc.

NA_API NAJSONRule* naNewJSONRuleBool(size_t memberOffset);
NA_API NAJSONRule* naNewJSONRuleInt32(size_t memberOffset);
NA_API NAJSONRule* naNewJSONRuleInt64(size_t memberOffset);
NA_API NAJSONRule* naNewJSONRuleDouble(size_t memberOffset);
NA_API NAJSONRule* naNewJSONRuleFloat(size_t memberOffset);
NA_API NAJSONRule* naNewJSONRuleString(size_t memberOffset);

// Defines a sub object to be read.
//
// The Object function allows to store an object directly as part of another
// object. For example MyContainer{..., MyObject obj, ...}. In that case, no
// memory needs to be allocated but the rules of the given subRuleSet must use
// an appropriate member offset like offsetof(MyContainer, obj.value).
//
// The PointerObject function allows to store an object indirectly in a pointer.
// The memberOffset gives the offset to that member and the structSize is the
// size to be allocated and used in case the read object is not null. If it is
// null, the member will be set to NULL, otherwise the memory gets allocated
// with malloc.

NA_API NAJSONRule* naNewJSONRuleObject(
  const NAJSONRuleSet* subRuleSet);
  
NA_API NAJSONRule* naNewJSONRulePointerObject(
  size_t memberOffset,
  size_t structSize,
  const NAJSONRuleSet* subRuleSet);

// Reading arrays can be done by various means. Note that although JSON allows
// any kind of subtypes in an array, in NALib, an array can only have one
// subtype. You define the kind by providing a rule on how to parse each
// element.
//
// When you know the size of the array, for example with type MyObject array[5],
// then you can use the FixedArray function. If each object shall be allocated
// and only the pointers are stored (MyObject* array[5]), then use the function
// FixedPointerArray.
//
// If you do not know the size of the array in advance, you can parse using the
// functions DynamicArray or DynamicPointerArray respectively. The according
// types would be MyObject* array or MyObject** array. The number of elements
// in the array will be stored at the specified countOffset.
//
// Internally for the dynamic arrays, all elements are read into a buffer and
// are copied to the desired array at the end. It shall be noted that copying
// just pointers is generally faster than copying hole objects.
//
// Also note that due to the fact that no memory allocation is needed, fixed
// arrays perform faster.

NA_API NAJSONRule* naNewJSONRuleFixedArray(
  size_t arrayOffset,
  size_t elementCount,
  size_t structSize,
  NAJSONRule* subRule);
NA_API NAJSONRule* naNewJSONRuleFixedPointerArray(
  size_t arrayOffset,
  size_t elementCount,
  size_t structSize,
  NAJSONRule* subRule);
NA_API NAJSONRule* naNewJSONRuleDynamicArray(
  size_t arrayOffset,
  size_t countOffset,
  size_t structSize,
  NAJSONRule* subRule);
NA_API NAJSONRule* naNewJSONRuleDynamicPointerArray(
  size_t arrayOffset,
  size_t countOffset,
  size_t structSize,
  NAJSONRule* subRule);

// Shortcuts for fixed arrays of basic types stored as objects.
// You can construct all these rules yourself but it is easier to rely on the
// following functions:

NA_API NAJSONRule* naNewJSONRuleDynamicArrayInt32(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleDynamicArrayInt64(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleDynamicArrayDouble(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleDynamicArrayFloat(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleDynamicArrayString(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleFixedArrayInt32(
  size_t arrayOffset,
  size_t elementCount);
NA_API NAJSONRule* naNewJSONRuleFixedArrayInt64(
  size_t arrayOffset,
  size_t elementCount);
NA_API NAJSONRule* naNewJSONRuleFixedArrayDouble(
  size_t arrayOffset,
  size_t elementCount);
NA_API NAJSONRule* naNewJSONRuleFixedArrayFloat(
  size_t arrayOffset,
  size_t elementCount);
NA_API NAJSONRule* naNewJSONRuleFixedArrayString(
  size_t arrayOffset,
  size_t elementCount);


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_JSON_INCLUDED
