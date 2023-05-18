
#ifndef NA_JSON_INCLUDED
#define NA_JSON_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include <stdio.h>
#include "NALib.h"

// This file proveds a very simple JSON parser.
//
// You define rules where to store data and just let the parser run. In the
// end, your storage will be filled with the contents desired. If you haven't
// defined a rule for a JSON entry, it will not be stored.
//
// Note that this is by no means a standard compliant parser. Many things are
// allowed which should not be.



typedef struct NAJSONParser NAJSONParser;
typedef struct NAJSONRule NAJSONRule;
typedef struct NAJSONRuleSet NAJSONRuleSet;



// Allocate and deallocate JSON parsers.
// After allocation, you create RuleSets to define, where to place the parsed
// items. See further below.
NAJSONParser* naAllocateJSONParser(void);
void naDeallocateJSONParser(NAJSONParser* parser);

// Parses the given buffer into the given object.
// You can send byteCount = 0 to automatically determine the buffer size.
// Important: The buffer must end with a '\0'
void naParseJSONBuffer(
  NAJSONParser* parser,
  void* object,
  const void* buf,
  size_t byteCount);



// /////////////////
// RuleSets and Rules

// Creates a new RuleSet stored in the given parser. When the parser gets
// deallocated, all rule sets will be deallocated too.
NA_API NAJSONRuleSet* naRegisterJSONRuleSet(
  NAJSONParser* parser);

// Defines what the initial ruleSet of the parser is.
NA_API void naSetJSONParserInitialRules(
  NAJSONParser* parser,
  const NAJSONRuleSet* ruleSet);

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
// when the ruleSet is deallocated, which in turn deallocates when the parser
// is deallocated.
//
// The offset can be computed by offsetof(MyStruct, member).
//
// JSON Booleans can be stored in Int32 or Int64
// JSON Numbers can be stored in Int32, Int64, Double and Float
// JSON Strings are stored as C-strings. Its memory is allocated with malloc.
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

// Arrays require special treatment. Although JSON allows any kind of subtypes
// in an array, in NALib, an array can only have one subtype. You define the
// kind by providing a rule on how to parse each element.
//
// Internally, all elements are read into a buffer and are copied to the
// desired array at the end. The User can choose whether NALib shall allocate
// a pointer for each element and store the pointers in the desired array or
// whether NALib shall store the contents of the elements directly as an object
// in the desired array.
//
// When storeAsAbject is true, the destination array is considered to be an
// array of multiple Objects, for example MyObject*, whereas if it is false,
// it is considered to be an array of pointers to that Objects, MyObject**.
// In both cases, the number of elements is stored at the given countOffset.
//
// Additionally, certain objects shall be stored in an array with a fixed size,
// for example MyArray[6]. In that case, one should use the FixedArray function.
// Note that due to the fact that no memory allocation is needed, fixed arrays
// perform much faster.

NA_API NAJSONRule* naNewJSONRuleArray(
  size_t arrayOffset,
  size_t countOffset,
  size_t structSize,
  NABool storeAsPointer,
  NAJSONRule* subRule);
NA_API NAJSONRule* naNewJSONRuleFixedArray(
  size_t arrayOffset,
  size_t structSize,
  size_t elementCount,
  NAJSONRule* subRule);

// Shortcuts for fixed arrays of basic types stored as objects.
// You can construct all these rules yourself but it is easier to rely on the
// following functions:
NA_API NAJSONRule* naNewJSONRuleArrayInt32(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleArrayInt64(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleArrayDouble(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleArrayFloat(
  size_t arrayOffset,
  size_t countOffset);
NA_API NAJSONRule* naNewJSONRuleArrayString(
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
