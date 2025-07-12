
#include "../NAJSON.h"
#include "../../NAStruct/NABuffer.h"
#include <ctype.h>

#define NA_JSON_PARSE_STACK_DEPTH 32

typedef enum {
  NA_JSON_PARSE_STATUS_UNDEFINED,
  NA_JSON_PARSE_BUFFER_END,
  NA_JSON_PARSE_NULL,
  NA_JSON_PARSE_BOOLEAN,
  NA_JSON_PARSE_NUMBER,
  NA_JSON_PARSE_STRING,
  NA_JSON_PARSE_ARRAY_START,
  NA_JSON_PARSE_ARRAY_END,
  NA_JSON_PARSE_OBJECT_START,
  NA_JSON_PARSE_OBJECT_END,
  NA_JSON_PARSE_UNEXPECTED_COMMA,
  NA_JSON_PARSE_UNEXPECTED_COLON,
  NA_JSON_PARSE_UNEXPECTED_ARRAY_END,
  NA_JSON_PARSE_UNEXPECTED_OBJECT_END,
  NA_JSON_PARSE_UNKNOWN_CHARACTER,
  NA_JSON_PARSE_UNKNOWN_IDENTIFIER,
} NA_JSONParseStatus;

typedef enum{
  NA_JSON_FILE_EXPECTING_VALUE,
  NA_JSON_ARRAY_EXPECTING_VALUE_OR_END,
  NA_JSON_OBJECT_EXPECTING_KEY_OR_END,
  NA_JSON_OBJECT_EXPECTING_COLON,
  NA_JSON_OBJECT_EXPECTING_VALUE,
} NA_JSONStackStatus;

typedef enum{
  NA_JSON_RULE_UNDEFINED,
  NA_JSON_RULE_BOOL,
  NA_JSON_RULE_INT32,
  NA_JSON_RULE_INT64,
  NA_JSON_RULE_DOUBLE,
  NA_JSON_RULE_FLOAT,
  NA_JSON_RULE_STRING,
  NA_JSON_RULE_DYNAMIC_ARRAY,
  NA_JSON_RULE_FIXED_ARRAY,
  NA_JSON_RULE_OBJECT,
  NA_JSON_RULE_POINTER_OBJECT,
} NA_JSONDataType;

NA_PROTOTYPE(NA_JSONString);
struct NA_JSONString{
  const NAUTF8Char* ptr;
  size_t strLen;
};

NA_PROTOTYPE(NAJSONRule);
struct NAJSONRule{
  NA_JSONString key;
  NA_JSONDataType type;
  #if NA_DEBUG
    NABool hasBeenAdded;
  #endif
};

NA_PROTOTYPE(NA_JSONMemberRule);
struct NA_JSONMemberRule{
  NAJSONRule baseRule;
  size_t memberOffset;
};

NA_PROTOTYPE(NA_JSONFixedArrayRule);
struct NA_JSONFixedArrayRule{
  NAJSONRule baseRule;
  size_t arrayOffset;
  #if NA_DEBUG
    size_t elementCount;
  #endif
  NA_JSONDataType contentType;
  size_t structSize;
  NABool storeAsPointer;
  NAJSONRule* subRule;
};

NA_PROTOTYPE(NA_JSONDynamicArrayRule);
struct NA_JSONDynamicArrayRule{
  NAJSONRule baseRule;
  size_t arrayOffset;
  size_t countOffset;
  NA_JSONDataType contentType;
  size_t structSize;
  NABool storeAsPointer;
  NAJSONRule* subRule;
};

NA_PROTOTYPE(NA_JSONObjectRule);
struct NA_JSONObjectRule{
  NAJSONRule baseRule;
  const NAJSONRuleSet* ruleSet;
};

NA_PROTOTYPE(NA_JSONPointerObjectRule);
struct NA_JSONPointerObjectRule{
  NAJSONRule baseRule;
  size_t memberOffset;
  size_t structSize;
  const NAJSONRuleSet* ruleSet;
};

struct NAJSONRuleSet{
  NAStack ruleStack;
  NAJSONRule** rules;
  #if NA_DEBUG
    NABool prepared;
  #endif
};

struct NAJSONWorker{
  const NAByte* curPtr;
  NA_JSONParseStatus parseStatus;
  size_t curStackStatus;
  NA_JSONStackStatus* stackStatusStack;
  NABool prepared;
  
  NAStack ruleSetStack;
  
  NABool boolean;
  double number;
  NA_JSONString string;
  NA_JSONString key;
};



// ///////////////////
// Strings

NA_HIDEF void na_FillJSONString(NA_JSONString* dst, const NAUTF8Char* src) {
  #if NA_DEBUG
    if(!src)
      naCrash("src is nullptr");
  #endif
  
  dst->ptr = src;
  dst->strLen = strlen(src);
}

NA_HIDEF void na_ResetJSONString(NA_JSONString* dst) {
  dst->ptr = "";
  dst->strLen = 0;
}

NA_HIDEF NABool na_EqualJSONString(const NA_JSONString* string1, const NA_JSONString* string2) {
  if(string1->strLen != string2->strLen)
    return NA_FALSE;
  return !memcmp(string1->ptr, string2->ptr, string1->strLen);
}

NA_HIDEF void na_StoreJSONString(const NAJSONWorker* worker, NAUTF8Char** storage) {
  *storage = naMalloc(worker->string.strLen + 1);
  naCopyn(*storage, worker->string.ptr, worker->string.strLen);
  (*storage)[worker->string.strLen] = '\0';
}



// ///////////////////
// Rules and Rule Sets

NA_HDEF void na_DeallocJSONRule(NAJSONRule* rule) {
  NA_JSONFixedArrayRule* fixedArrayRule;
  NA_JSONDynamicArrayRule* dynamicArrayRule;
  
  switch(rule->type) {
  case NA_JSON_RULE_FIXED_ARRAY:
    fixedArrayRule = (NA_JSONFixedArrayRule*)rule;
    if(fixedArrayRule->subRule) {
      na_DeallocJSONRule(fixedArrayRule->subRule);
    }
    break;
  case NA_JSON_RULE_DYNAMIC_ARRAY:
    dynamicArrayRule = (NA_JSONDynamicArrayRule*)rule;
    if(dynamicArrayRule->subRule) {
      na_DeallocJSONRule(dynamicArrayRule->subRule);
    }
    break;
  default:
    break;
  }
  naFree(rule);
}

NA_DEF NAJSONRuleSet* naRegisterJSONRuleSet(NAJSONWorker* worker) {
  #if NA_DEBUG
    if(!worker)
      naCrash("worker is nullptr");
    if(worker->prepared)
      naError("The worker has already been used. Adding more rule sets results in undefined behaviour.");
  #endif

  NAJSONRuleSet* ruleSet = naAlloc(NAJSONRuleSet);
  naInitStack(&ruleSet->ruleStack, sizeof(NAJSONRule*), 0, 0);
  ruleSet->rules = NA_NULL;
  #if NA_DEBUG
    ruleSet->prepared = NA_FALSE;
  #endif
  
  // Store the ruleset as part of the worker so it can be erased automatically.
  NAJSONRuleSet** ruleSetPtr = naPushStack(&worker->ruleSetStack);
  *ruleSetPtr = ruleSet;
  
  return ruleSet;
}

NA_HDEF void na_DeallocJSONRuleSet(NAJSONRuleSet* ruleSet) {
  if(ruleSet->rules) {
    naFree(ruleSet->rules);
  }
  naForeachStackpMutable(&ruleSet->ruleStack, (NAMutator)na_DeallocJSONRule);
  naClearStack(&ruleSet->ruleStack);
  naFree(ruleSet);
}



// ///////////////////
// Rules

NA_DEF void naAddJSONRule(
  NAJSONRuleSet* ruleSet,
  const NAUTF8Char* key,
  NAJSONRule* rule)
{
  #if NA_DEBUG
    if(!ruleSet)
      naCrash("ruleSet is nullptr");
    if(!rule)
      naCrash("rule is nullptr");
    if(rule->hasBeenAdded)
      naError("rule has already been added to another ruleSet. This is not allowed and will likely result in memory corruption.");
    if(ruleSet->prepared)
      naError("The ruleSet is part of a worker which has already been used. Adding more rules results in undefined behaviour.");
  #endif

  NAJSONRule** rulePtr = naPushStack(&ruleSet->ruleStack);
  *rulePtr = rule;
  na_FillJSONString(&rule->key, key);
  #if NA_DEBUG
    rule->hasBeenAdded = NA_TRUE;
  #endif
}

NA_DEF void na_initJSONRule(NAJSONRule* rule, NA_JSONDataType type) {
  rule->type = type;
  #if NA_DEBUG
    rule->hasBeenAdded = NA_FALSE;
  #endif
}

NA_DEF NAJSONRule* naNewJSONRuleBool(size_t memberOffset) {
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_BOOL);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleInt32(size_t memberOffset) {
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_INT32);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleInt64(size_t memberOffset) {
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_INT64);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDouble(size_t memberOffset) {
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_DOUBLE);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFloat(size_t memberOffset) {
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_FLOAT);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleString(size_t memberOffset) {
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_STRING);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedArray(
  size_t arrayOffset,
  size_t elementCount,
  size_t structSize,
  NAJSONRule* subRule)
{
  #if !NA_DEBUG
    NA_UNUSED(elementCount);
  #endif
  NA_JSONFixedArrayRule* rule = naAlloc(NA_JSONFixedArrayRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_FIXED_ARRAY);
  rule->arrayOffset = arrayOffset;
  #if NA_DEBUG
    rule->elementCount = elementCount;
  #endif
  rule->contentType = NA_JSON_RULE_UNDEFINED;
  rule->structSize = structSize;
  rule->storeAsPointer = NA_FALSE;
  rule->subRule = subRule;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedPointerArray(
  size_t arrayOffset,
  size_t elementCount,
  size_t structSize,
  NAJSONRule* subRule)
{
  #if !NA_DEBUG
    NA_UNUSED(elementCount);
  #endif
  NA_JSONFixedArrayRule* rule = naAlloc(NA_JSONFixedArrayRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_FIXED_ARRAY);
  rule->arrayOffset = arrayOffset;
  #if NA_DEBUG
    rule->elementCount = elementCount;
  #endif
  rule->contentType = NA_JSON_RULE_UNDEFINED;
  rule->structSize = structSize;
  rule->storeAsPointer = NA_TRUE;
  rule->subRule = subRule;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDynamicArray(
  size_t arrayOffset,
  size_t countOffset,
  size_t structSize,
  NAJSONRule* subRule)
{
  NA_JSONDynamicArrayRule* rule = naAlloc(NA_JSONDynamicArrayRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_DYNAMIC_ARRAY);
  rule->arrayOffset = arrayOffset;
  rule->countOffset = countOffset;
  rule->contentType = NA_JSON_RULE_UNDEFINED;
  rule->structSize = structSize;
  rule->storeAsPointer = NA_FALSE;
  rule->subRule = subRule;

  return &rule->baseRule;
}
NA_DEF NAJSONRule* naNewJSONRuleDynamicPointerArray(
  size_t arrayOffset,
  size_t countOffset,
  size_t structSize,
  NAJSONRule* subRule)
{
  NA_JSONDynamicArrayRule* rule = naAlloc(NA_JSONDynamicArrayRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_DYNAMIC_ARRAY);
  rule->arrayOffset = arrayOffset;
  rule->countOffset = countOffset;
  rule->contentType = NA_JSON_RULE_UNDEFINED;
  rule->structSize = structSize;
  rule->storeAsPointer = NA_TRUE;
  rule->subRule = subRule;

  return &rule->baseRule;
}



NA_DEF NAJSONRule* naNewJSONRuleFixedArrayInt32(
  size_t arrayOffset,
  size_t elementCount)
{
  NA_JSONFixedArrayRule* rule = (NA_JSONFixedArrayRule*)naNewJSONRuleFixedArray(
    arrayOffset,
    elementCount,
    sizeof(int32),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_INT32;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedArrayInt64(
  size_t arrayOffset,
  size_t elementCount)
{
  NA_JSONFixedArrayRule* rule = (NA_JSONFixedArrayRule*)naNewJSONRuleFixedArray(
    arrayOffset,
    elementCount,
    sizeof(int64),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_INT64;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedArrayDouble(
  size_t arrayOffset,
  size_t elementCount)
{
  NA_JSONFixedArrayRule* rule = (NA_JSONFixedArrayRule*)naNewJSONRuleFixedArray(
    arrayOffset,
    elementCount,
    sizeof(double),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_DOUBLE;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedArrayFloat(
  size_t arrayOffset,
  size_t elementCount)
{
  NA_JSONFixedArrayRule* rule = (NA_JSONFixedArrayRule*)naNewJSONRuleFixedArray(
    arrayOffset,
    elementCount,
    sizeof(float),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_FLOAT;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedArrayString(
  size_t arrayOffset,
  size_t elementCount)
{
  NA_JSONFixedArrayRule* rule = (NA_JSONFixedArrayRule*)naNewJSONRuleFixedArray(
    arrayOffset,
    elementCount,
    sizeof(NAUTF8Char*),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_STRING;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDynamicArrayInt32(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONDynamicArrayRule* rule = (NA_JSONDynamicArrayRule*)naNewJSONRuleDynamicArray(
    arrayOffset,
    countOffset,
    sizeof(int32),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_INT32;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDynamicArrayInt64(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONDynamicArrayRule* rule = (NA_JSONDynamicArrayRule*)naNewJSONRuleDynamicArray(
    arrayOffset,
    countOffset,
    sizeof(int64),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_INT64;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDynamicArrayDouble(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONDynamicArrayRule* rule = (NA_JSONDynamicArrayRule*)naNewJSONRuleDynamicArray(
    arrayOffset,
    countOffset,
    sizeof(double),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_DOUBLE;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDynamicArrayFloat(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONDynamicArrayRule* rule = (NA_JSONDynamicArrayRule*)naNewJSONRuleDynamicArray(
    arrayOffset,
    countOffset,
    sizeof(float),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_FLOAT;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDynamicArrayString(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONDynamicArrayRule* rule = (NA_JSONDynamicArrayRule*)naNewJSONRuleDynamicArray(
    arrayOffset,
    countOffset,
    sizeof(NAUTF8Char*),
    NA_NULL);
  rule->contentType = NA_JSON_RULE_STRING;
  return &rule->baseRule;
}



NA_DEF NAJSONRule* naNewJSONRuleObject(const NAJSONRuleSet* subRuleSet) {
  NA_JSONObjectRule* rule = naAlloc(NA_JSONObjectRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_OBJECT);
  rule->ruleSet = subRuleSet;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRulePointerObject(
  size_t memberOffset,
  size_t structSize,
  const NAJSONRuleSet* subRuleSet)
{
  NA_JSONPointerObjectRule* rule = naAlloc(NA_JSONPointerObjectRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_POINTER_OBJECT);
  rule->memberOffset = memberOffset;
  rule->structSize = structSize;
  rule->ruleSet = subRuleSet;

  return &rule->baseRule;
}



NA_HDEF const NAJSONRule* na_findJSONRule(const NAJSONRuleSet* ruleSet, NA_JSONDataType type, const NA_JSONString* key) {
  if(ruleSet) {
    for(size_t i = 0; i < naGetStackCount(&ruleSet->ruleStack); ++i) {
      const NAJSONRule* rule = ruleSet->rules[i];
      if(rule->type == type && na_EqualJSONString(&rule->key, key))
      {
        return rule;
      }
    }
  }
  return NA_NULL;
}



// ///////////////////
// Stack Status

NA_HIDEF void na_JSONPushStackStatus(NAJSONWorker* worker, NA_JSONStackStatus status) {
  #if NA_DEBUG
    if(worker->curStackStatus == NA_JSON_PARSE_STACK_DEPTH - 1)
      naError("Stack overflow. Heap corruption imminent...");
  #endif
  worker->curStackStatus++;
  worker->stackStatusStack[worker->curStackStatus] = status;
}

NA_HIDEF NA_JSONStackStatus na_JSONTopStackStatus(NAJSONWorker* worker) {
  return worker->stackStatusStack[worker->curStackStatus];
}

NA_HIDEF void na_JSONPopStackStatus(NAJSONWorker* worker) {
  #if NA_DEBUG
    if(worker->curStackStatus == 0)
      naError("Stack underflow. Heap corruption imminent...");
  #endif
  worker->curStackStatus--;
}

void ns_JSONAdjustStackStatusAfterValueRead(NAJSONWorker* worker) {
  NA_JSONStackStatus stackStatus = na_JSONTopStackStatus(worker);
  if(stackStatus == NA_JSON_OBJECT_EXPECTING_VALUE) {
    na_JSONPopStackStatus(worker);
    na_JSONPushStackStatus(worker, NA_JSON_OBJECT_EXPECTING_KEY_OR_END);
  }
}



// ///////////////////
// Worker

NA_DEF NAJSONWorker* naAllocateJSONWorker() {
  NAJSONWorker* worker = naAlloc(NAJSONWorker);
  worker->curPtr = NA_NULL;
  
  worker->stackStatusStack = naMalloc(NA_JSON_PARSE_STACK_DEPTH * sizeof(NA_JSONStackStatus));
  worker->curStackStatus = 0;
  worker->stackStatusStack[worker->curStackStatus] = NA_JSON_FILE_EXPECTING_VALUE;
  worker->prepared = NA_FALSE;
  
  worker->parseStatus = NA_JSON_PARSE_STATUS_UNDEFINED;
  
  naInitStack(&worker->ruleSetStack, sizeof(NAJSONRuleSet*), 0, 0);

  worker->boolean = NA_FALSE;
  worker->number = 0.;
  na_ResetJSONString(&worker->string);
  na_ResetJSONString(&worker->key);

  return worker;
}

NA_DEF void naDeallocateJSONWorker(NAJSONWorker* worker) {
  #if NA_DEBUG
    if(!worker)
      naCrash("worker is nullptr");
  #endif

  naFree(worker->stackStatusStack);

  naForeachStackpMutable(&worker->ruleSetStack, (NAMutator)na_DeallocJSONRuleSet);
  naClearStack(&worker->ruleSetStack);
    
  naFree(worker);
}



// Prototypes:
NA_HAPI void na_ParseJSONObject(NAJSONWorker* worker, void* object, const NAJSONRuleSet* ruleSet);
NA_HAPI void na_WriteJSONObject(NAJSONWorker* worker, void* object, NABufferIterator* bufIt, const NAJSONRuleSet* ruleSet);



NA_HAPI void na_PrepareJSONWorker(NAJSONWorker* worker) {
  if(!worker->prepared) {
    // Prepare all rules for faster access.
    NAStackIterator ruleSetIt = naMakeStackMutator(&worker->ruleSetStack);
    while(naIterateStack(&ruleSetIt)) {
      NAJSONRuleSet* ruleSet = naGetStackCurpMutable(&ruleSetIt);
      if(ruleSet->rules) {
        naFree(ruleSet->rules);
        ruleSet->rules = NA_NULL;
      }
      size_t ruleCount = naGetStackCount(&ruleSet->ruleStack);
      if(ruleCount) {
        ruleSet->rules = naMalloc(ruleCount * sizeof(NAJSONRule*));
        naDumpStack(&ruleSet->ruleStack, ruleSet->rules);
      }
      #if NA_DEBUG
        ruleSet->prepared = NA_TRUE;
      #endif
    }
    naClearStackIterator(&ruleSetIt);
    worker->prepared = NA_TRUE;
  }
}

void naParseJSONBuffer(
  NAJSONWorker* worker,
  void* object,
  const void* buf,
  size_t byteCount)
{
  #if NA_DEBUG
    if(!worker)
      naCrash("worker is nullptr");
    if(!buf)
      naCrash("buf is nullptr");
    if(((NAByte*)buf)[byteCount - 1] != '\0')
      naError("buffer must end with a zero byte.");
  #else
    NA_UNUSED(byteCount);
  #endif

  worker->curPtr = buf;
  
  na_PrepareJSONWorker(worker);

  NAJSONRuleSet** initialRuleSet = naTopStack(&worker->ruleSetStack);
  na_ParseJSONObject(worker, object, *initialRuleSet);
}



NABuffer* naCreateBufferWithJSON(
  NAJSONWorker* worker,
  void* object)
{
  NABuffer* buffer = naCreateBuffer(NA_FALSE);
  NABufferIterator bufIt = naMakeBufferModifier(buffer);
  
  na_PrepareJSONWorker(worker);

  NAJSONRuleSet** initialRuleSet = naTopStack(&worker->ruleSetStack);
  na_WriteJSONObject(worker, object, &bufIt, *initialRuleSet);
  
  naClearBufferIterator(&bufIt);
  
  NAByte data[10000];
  naWriteBufferToData(buffer, data);
  
  return buffer;
}


// ///////////////////
// Bytewise operation

NA_HIDEF NAByte na_CurJSONByte(NAJSONWorker* worker) {
  return *(const NAByte*)worker->curPtr;
}

NA_HIDEF NAByte na_NextJSONByte(NAJSONWorker* worker) {
  ++worker->curPtr;
  return *(const NAByte*)worker->curPtr;
}



// ///////////////////
// Parsing entities

NA_HDEF NA_JSONParseStatus na_ParseJSONNumber(NAJSONWorker* worker) {
  NAByte curByte = na_CurJSONByte(worker);
  
  int32 decimalShift = 0;
  int64 decimalSign = NA_ONE_i64;
  int32 exponentSign = 1;
  int64 decimals = NA_ZERO_i64;
  int32 exponent = 0;
  
  if(curByte == '-') {
    decimalSign = NA_MINUS_ONE_i64;
    curByte = na_NextJSONByte(worker);
  }else if(curByte == '+') {
    curByte = na_NextJSONByte(worker);
  }
  
  while(isdigit(curByte)) {
    decimals = naAddi64(naAddi64(naShli64(decimals, 1), naShli64(decimals, 3)), naCastu8Toi64(curByte & 0x0f));
    curByte = na_NextJSONByte(worker);
  }
  
  if(curByte == '.') {
    curByte = na_NextJSONByte(worker);
    const NAByte* firstDecimalPtr = worker->curPtr;

    while(isdigit(curByte)) {
      decimals = naAddi64(naAddi64(naShli64(decimals, 1), naShli64(decimals, 3)), naCastu8Toi64(curByte & 0x0f));
      curByte = na_NextJSONByte(worker);
    }

    const NAByte* lastDecimalPtr = worker->curPtr;
    decimalShift = (int32)(lastDecimalPtr - firstDecimalPtr);
  }

  if((curByte | 0x20) == 'e') {
    curByte = na_NextJSONByte(worker);
    
    if(curByte == '-') {
      exponentSign = -1;
      curByte = na_NextJSONByte(worker);
    }else if(curByte == '+') {
      curByte = na_NextJSONByte(worker);
    }

    while(isdigit(curByte)) {
      exponent = (exponent << 1) + (exponent << 3) + (curByte & 0x0f);
      curByte = na_NextJSONByte(worker);
    }
  }

  decimals = naMuli64(decimals, decimalSign);
  exponent = exponentSign * exponent - decimalShift;
  worker->number = naCasti64ToDouble(decimals) * naExp10((double)(exponent));
  
  worker->parseStatus = NA_JSON_PARSE_NUMBER;
  return worker->parseStatus;
}

NA_HDEF NA_JSONParseStatus na_ParseJSONIdentifier(NAJSONWorker* worker) {
  const NAByte* firstByte = worker->curPtr;
  NAByte curByte = na_NextJSONByte(worker);
  while(isalpha(curByte)) {
    curByte = na_NextJSONByte(worker);
  }

  const size_t strLen = (size_t)(worker->curPtr - firstByte);
  if(strLen == 4 && !memcmp(firstByte, "null", 4)) {
    worker->parseStatus = NA_JSON_PARSE_NULL;
    return worker->parseStatus;
  }else if(strLen == 4 && !memcmp(firstByte, "true", 4)) {
    worker->boolean = NA_TRUE;
    worker->parseStatus = NA_JSON_PARSE_BOOLEAN;
    return worker->parseStatus;
  }else if(strLen == 5 && !memcmp(firstByte, "false", 5)) {
    worker->boolean = NA_FALSE;
    worker->parseStatus = NA_JSON_PARSE_BOOLEAN;
    return worker->parseStatus;
  }

  worker->parseStatus = (curByte == '\0')
    ? NA_JSON_PARSE_BUFFER_END
    : NA_JSON_PARSE_UNKNOWN_IDENTIFIER;
  #if NA_DEBUG
    if(worker->parseStatus == NA_JSON_PARSE_UNKNOWN_IDENTIFIER)
      naError("Unknown Identifier\n");
  #endif
  return worker->parseStatus;
}

NA_HDEF NA_JSONParseStatus na_ParseJSONString(NAJSONWorker* worker) {
  NAByte curByte = na_CurJSONByte(worker);

  NA_JSONStackStatus stackStatus = na_JSONTopStackStatus(worker);
  if(stackStatus == NA_JSON_OBJECT_EXPECTING_KEY_OR_END) {
    worker->key.ptr = (const NAUTF8Char*)worker->curPtr;
  }else{
    worker->string.ptr = (const NAUTF8Char*)worker->curPtr;
  }

  while(curByte > '\0') {
    if(curByte == '\\') {
      na_NextJSONByte(worker);
    }else if(curByte == '\"') {
      if(stackStatus == NA_JSON_OBJECT_EXPECTING_KEY_OR_END) {
        worker->key.strLen = (size_t)((const NAUTF8Char*)worker->curPtr - worker->key.ptr);
      }else{
        worker->string.strLen = (size_t)((const NAUTF8Char*)worker->curPtr - worker->string.ptr);
      }
      na_NextJSONByte(worker);
      worker->parseStatus = NA_JSON_PARSE_STRING;
      return worker->parseStatus;
    }
    curByte = na_NextJSONByte(worker);
  }
  
  worker->parseStatus = NA_JSON_PARSE_BUFFER_END;
  return worker->parseStatus;
}



// ///////////////////
// Parsing primitives

NA_HDEF NA_JSONParseStatus na_ParseJSONPrimitives(NAJSONWorker* worker) {
  while(1) {
    NAByte curByte = na_CurJSONByte(worker);
    
    // Skip any whitespace.
    while(curByte > '\0' && curByte <= ' ') {
      curByte = na_NextJSONByte(worker);
    }
    
    // Reset the key string, except while reading an object entry.
    NA_JSONStackStatus stackStatus = na_JSONTopStackStatus(worker);
    if(stackStatus != NA_JSON_OBJECT_EXPECTING_COLON && stackStatus != NA_JSON_OBJECT_EXPECTING_VALUE) {
      na_ResetJSONString(&worker->key);
    }

    if(curByte == '\0') {
      worker->parseStatus = NA_JSON_PARSE_BUFFER_END;
      return worker->parseStatus;
    
    }else if(isdigit(curByte) || curByte == '-' || curByte == '+' || curByte == '.') {
      worker->parseStatus = na_ParseJSONNumber(worker);
      ns_JSONAdjustStackStatusAfterValueRead(worker);
      return worker->parseStatus;

    }else if(isalpha(curByte)) {
      worker->parseStatus = na_ParseJSONIdentifier(worker);
      ns_JSONAdjustStackStatusAfterValueRead(worker);
      return worker->parseStatus;

    }else if(curByte == '\"') {
      na_NextJSONByte(worker);
      worker->parseStatus = na_ParseJSONString(worker);
      if(stackStatus == NA_JSON_OBJECT_EXPECTING_KEY_OR_END) {
        // This was a key
        na_JSONPopStackStatus(worker);
        na_JSONPushStackStatus(worker, NA_JSON_OBJECT_EXPECTING_COLON);
        continue;
      }else{
        // This was a value
        ns_JSONAdjustStackStatusAfterValueRead(worker);
        return worker->parseStatus;
      }
      
    }else if(curByte == '[') {
      na_NextJSONByte(worker);
      na_JSONPushStackStatus(worker, NA_JSON_ARRAY_EXPECTING_VALUE_OR_END);
      worker->parseStatus = NA_JSON_PARSE_ARRAY_START;
      return worker->parseStatus;
      
    }else if(curByte == ']') {
      na_NextJSONByte(worker);
      if(worker->curStackStatus == 0) {
        worker->parseStatus = NA_JSON_PARSE_UNEXPECTED_ARRAY_END;
        #if NA_DEBUG
          naError("Unexpected Array End\n");
        #endif
        return worker->parseStatus;
      }
      na_JSONPopStackStatus(worker);
      ns_JSONAdjustStackStatusAfterValueRead(worker);
      worker->parseStatus = (stackStatus != NA_JSON_ARRAY_EXPECTING_VALUE_OR_END)
        ? NA_JSON_PARSE_UNEXPECTED_ARRAY_END
        : NA_JSON_PARSE_ARRAY_END;
      #if NA_DEBUG
      if(worker->parseStatus == NA_JSON_PARSE_UNEXPECTED_ARRAY_END)
        naError("Unexpected Array End\n");
      #endif
      return worker->parseStatus;
      
    }else if(curByte == '{') {
      na_NextJSONByte(worker);
      na_JSONPushStackStatus(worker, NA_JSON_OBJECT_EXPECTING_KEY_OR_END);
      worker->parseStatus = NA_JSON_PARSE_OBJECT_START;
      return worker->parseStatus;
      
    }else if(curByte == '}') {
      na_NextJSONByte(worker);
      if(worker->curStackStatus == 0) {
        worker->parseStatus = NA_JSON_PARSE_UNEXPECTED_OBJECT_END;
        #if NA_DEBUG
          naError("Unexpected Object End\n");
        #endif
        return worker->parseStatus;
      }
      na_JSONPopStackStatus(worker);
      ns_JSONAdjustStackStatusAfterValueRead(worker);
      worker->parseStatus = (stackStatus != NA_JSON_OBJECT_EXPECTING_KEY_OR_END)
        ? NA_JSON_PARSE_UNEXPECTED_OBJECT_END
        : NA_JSON_PARSE_OBJECT_END;
      #if NA_DEBUG
        if(worker->parseStatus == NA_JSON_PARSE_UNEXPECTED_OBJECT_END)
          naError("Unexpected Object End\n");
      #endif
      return worker->parseStatus;
      
    }else if(curByte == ',') {
      na_NextJSONByte(worker);
      if(stackStatus != NA_JSON_ARRAY_EXPECTING_VALUE_OR_END
        && stackStatus != NA_JSON_OBJECT_EXPECTING_KEY_OR_END) {
        worker->parseStatus = NA_JSON_PARSE_UNEXPECTED_COMMA;
        #if NA_DEBUG
          naError("Unexpected Comma\n");
        #endif
        return worker->parseStatus;
      }
      continue;

    }else if(curByte == ':') {
      na_NextJSONByte(worker);
      if(stackStatus != NA_JSON_OBJECT_EXPECTING_COLON) {
        worker->parseStatus = NA_JSON_PARSE_UNEXPECTED_COLON;
        #if NA_DEBUG
          naError("Unexpected Colon\n");
        #endif
        return worker->parseStatus;
      }
      na_JSONPopStackStatus(worker);
      na_JSONPushStackStatus(worker, NA_JSON_OBJECT_EXPECTING_VALUE);
      continue;
    }
  
    worker->parseStatus = NA_JSON_PARSE_UNKNOWN_CHARACTER;
    #if NA_DEBUG
      naError("Unknown Character\n");
    #endif
    na_NextJSONByte(worker);
    return worker->parseStatus;
  }
}
    
    

// Prototype:
NA_HAPI void na_ParseJSONArray(NAJSONWorker* worker, void* object, const NAJSONRule* elementRule);
NA_HAPI void na_ParseJSONDummyObject(NAJSONWorker* worker);


// ///////////////////
// Parsing Arrays

NA_HDEF void na_ParseJSONDummyArray(NAJSONWorker* worker) {
  NABool unexpectedEnd = NA_FALSE;
  while(!unexpectedEnd && na_ParseJSONPrimitives(worker) != NA_JSON_PARSE_ARRAY_END) {

    switch(worker->parseStatus) {
    case NA_JSON_PARSE_ARRAY_START:
        na_ParseJSONDummyArray(worker);
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
        na_ParseJSONDummyObject(worker);
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
}

#if NA_DEBUG
  #define na_incArrayCount()\
    count++;\
    if(count > elementRule->elementCount)\
      naError("Array overflow");
#else
  #define na_incArrayCount()
#endif

NA_HDEF void na_ParseJSONFixedArray(NAJSONWorker* worker, void* object, const NA_JSONFixedArrayRule* elementRule) {
  NABool unexpectedEnd = NA_FALSE;
  void* subObject;
  void* arrayPtr = (NAByte*)object + elementRule->arrayOffset;
  NA_JSONObjectRule* subObjectRule = (NA_JSONObjectRule*)elementRule->subRule;

  #if NA_DEBUG
    size_t count = 0;
  #endif

  while(!unexpectedEnd && na_ParseJSONPrimitives(worker) != NA_JSON_PARSE_ARRAY_END) {

    switch(worker->parseStatus) {
    case NA_JSON_PARSE_BOOLEAN:
      switch(elementRule->contentType) {
      case NA_JSON_RULE_BOOL:
        na_incArrayCount();
        *(NABool*)arrayPtr = worker->boolean;
        break;
      case NA_JSON_RULE_INT32:
        na_incArrayCount();
        *(int32*)arrayPtr = (int32)worker->boolean;
        break;
      case NA_JSON_RULE_INT64:
        na_incArrayCount();
        *(int64*)arrayPtr = naCastBoolToi64(worker->boolean);
        break;
      default:
        break;
      }
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_NUMBER:
      switch(elementRule->contentType) {
      case NA_JSON_RULE_BOOL:
        na_incArrayCount();
        *(NABool*)arrayPtr = (NABool)worker->number;
        break;
      case NA_JSON_RULE_INT32:
        na_incArrayCount();
        *(int32*)arrayPtr = (int32)worker->number;
        break;
      case NA_JSON_RULE_INT64:
        na_incArrayCount();
        *(int64*)arrayPtr = naCastDoubleToi64(worker->number);
        break;
      case NA_JSON_RULE_DOUBLE:
        na_incArrayCount();
        *(double*)arrayPtr = worker->number;
        break;
      case NA_JSON_RULE_FLOAT:
        na_incArrayCount();
        *(float*)arrayPtr = (float)worker->number;
        break;
      default:
        break;
      }
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_STRING:
      if(elementRule->contentType == NA_JSON_RULE_STRING) {
        na_incArrayCount();
        na_StoreJSONString(worker, (NAUTF8Char**)arrayPtr);
      }
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      na_incArrayCount();
      na_ParseJSONArray(worker, arrayPtr, elementRule->subRule);
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
      na_incArrayCount();
      if(elementRule->storeAsPointer) {
        subObject = naMalloc(elementRule->structSize);
        *((void**)(arrayPtr)) = subObject;
      }else{
        subObject = arrayPtr;
      }
      na_ParseJSONObject(worker, subObject, subObjectRule->ruleSet);
      if(elementRule->storeAsPointer) {
        arrayPtr = (NAByte*)arrayPtr + sizeof(void*);
      }else{
        arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      }
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }

  #if NA_DEBUG
    if(count < elementRule->elementCount)
      naError("Not all elements of the array are filled. Remainder is uninitialized.");
  #endif
}

NA_HDEF void na_ParseJSONDynamicArray(NAJSONWorker* worker, void* object, const NA_JSONDynamicArrayRule* elementRule) {
  NABool unexpectedEnd = NA_FALSE;
  void* subObject;
  NAStack elementStack;
  NA_JSONObjectRule* subObjectRule = (NA_JSONObjectRule*)elementRule->subRule;

  if(elementRule->storeAsPointer) {
    naInitStack(&elementStack, sizeof(void*), 0, 0);
  }else{
    naInitStack(&elementStack, elementRule->structSize, 0, 0);
  }

  while(!unexpectedEnd && na_ParseJSONPrimitives(worker) != NA_JSON_PARSE_ARRAY_END) {

    switch(worker->parseStatus) {
    case NA_JSON_PARSE_BOOLEAN:
      switch(elementRule->contentType) {
      case NA_JSON_RULE_BOOL:
        if(elementRule->storeAsPointer) {
          NABool** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(NABool));
          **curNumberPtrPtr = worker->boolean;
        }else{
          NABool* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = worker->boolean;
        }
        break;
      case NA_JSON_RULE_INT32:
        if(elementRule->storeAsPointer) {
          int32** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(int32));
          **curNumberPtrPtr = (int32)worker->boolean;
        }else{
          int32* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = (int32)worker->boolean;
        }
        break;
      case NA_JSON_RULE_INT64:
        if(elementRule->storeAsPointer) {
          int64** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(int64));
          **curNumberPtrPtr = naCastDoubleToi64(worker->boolean);
        }else{
          int64* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = naCastDoubleToi64(worker->boolean);
        }
        break;
      default:
        break;
      }
      break;
      
    case NA_JSON_PARSE_NUMBER:
      switch(elementRule->contentType) {
      case NA_JSON_RULE_BOOL:
        if(elementRule->storeAsPointer) {
          NABool** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(NABool));
          **curNumberPtrPtr = (NABool)worker->number;
        }else{
          NABool* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = (NABool)worker->number;
        }
        break;
      case NA_JSON_RULE_INT32:
        if(elementRule->storeAsPointer) {
          int32** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(int32));
          **curNumberPtrPtr = (int32)worker->number;
        }else{
          int32* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = (int32)worker->number;
        }
        break;
      case NA_JSON_RULE_INT64:
        if(elementRule->storeAsPointer) {
          int64** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(int64));
          **curNumberPtrPtr = naCastDoubleToi64(worker->number);
        }else{
          int64* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = naCastDoubleToi64(worker->number);
        }
        break;
      case NA_JSON_RULE_DOUBLE:
        if(elementRule->storeAsPointer) {
          double** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(double));
          **curNumberPtrPtr = worker->number;
        }else{
          double* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = worker->number;
        }
        break;
      case NA_JSON_RULE_FLOAT:
        if(elementRule->storeAsPointer) {
          float** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(float));
          **curNumberPtrPtr = (float)worker->number;
        }else{
          float* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = (float)worker->number;
        }
        break;
      default:
        break;
      }
      break;
      
    case NA_JSON_PARSE_STRING:
      if(elementRule->storeAsPointer) {
        #if NA_DEBUG
          naError("Storing a pointer to a C-string in a dynamic array is not allowed. Would result in char*** and that's just stupid.");
        #endif
      }else{
        NAUTF8Char** curStringPtr = naPushStack(&elementStack);
        na_StoreJSONString(worker, curStringPtr);
      }
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      if(elementRule->storeAsPointer) {
        void** curObjectPtrPtr = naPushStack(&elementStack);
        *curObjectPtrPtr = naMalloc(elementRule->structSize);
        subObject = *curObjectPtrPtr;
      }else{
        void* curObjectPtr = naPushStack(&elementStack);
        subObject = curObjectPtr;
      }
      na_ParseJSONArray(worker, subObject, elementRule->subRule);
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
      if(elementRule->storeAsPointer) {
        void** curObjectPtrPtr = naPushStack(&elementStack);
        *curObjectPtrPtr = naMalloc(elementRule->structSize);
        subObject = *curObjectPtrPtr;
      }else{
        void* curObjectPtr = naPushStack(&elementStack);
        subObject = curObjectPtr;
      }
      na_ParseJSONObject(worker, subObject, subObjectRule->ruleSet);
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
  
  // Store the array elements in the container object.
  size_t elementCount = naGetStackCount(&elementStack);
  *((size_t*)((NAByte*)object + elementRule->countOffset)) = elementCount;
  
  if(elementCount) {
    void** array = (void**)((NAByte*)object + elementRule->arrayOffset);
    if(elementRule->storeAsPointer) {
      *array = naMalloc(sizeof(void*) * elementCount);
    }else{
      *array = naMalloc(elementRule->structSize * elementCount);
    }
    naDumpStack(&elementStack, *array);
  }else{
    *((void**)((NAByte*)object + elementRule->arrayOffset)) = NA_NULL;
  }
  
  naClearStack(&elementStack);
}

NA_HDEF void na_ParseJSONArray(NAJSONWorker* worker, void* object, const NAJSONRule* elementRule) {
  if(!elementRule) {
    // In case no rule is given, we just read the contents without storing
    // anything.
    na_ParseJSONDummyArray(worker);
  }else if(elementRule->type == NA_JSON_RULE_FIXED_ARRAY) {
    na_ParseJSONFixedArray(worker, object, (NA_JSONFixedArrayRule*)elementRule);
  }else if(elementRule->type == NA_JSON_RULE_DYNAMIC_ARRAY) {
    na_ParseJSONDynamicArray(worker, object, (NA_JSONDynamicArrayRule*)elementRule);
  }
}



// ///////////////////
// Parsing Objects

NA_HDEF void na_ParseJSONDummyObject(NAJSONWorker* worker) {
  NABool unexpectedEnd = NA_FALSE;
  while(!unexpectedEnd && na_ParseJSONPrimitives(worker) != NA_JSON_PARSE_OBJECT_END) {

    switch(worker->parseStatus) {
    case NA_JSON_PARSE_ARRAY_START:
        na_ParseJSONDummyArray(worker);
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
        na_ParseJSONDummyObject(worker);
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
}

NA_HDEF void na_ParseJSONRuleSetObject(NAJSONWorker* worker, void* object, const NAJSONRuleSet* ruleSet) {
  NABool unexpectedEnd = NA_FALSE;
  void* subObject;
  const NAJSONRuleSet* subRuleSet = NA_NULL;

  while(!unexpectedEnd && na_ParseJSONPrimitives(worker) != NA_JSON_PARSE_OBJECT_END) {
    const NAJSONRule* rule = NA_NULL;
        
    switch(worker->parseStatus) {
    case NA_JSON_PARSE_NULL:
      #if NA_DEBUG
        if(  (na_findJSONRule(ruleSet, NA_JSON_RULE_BOOL, &worker->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &worker->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &worker->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_DOUBLE, &worker->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_FLOAT, &worker->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_STRING, &worker->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_DYNAMIC_ARRAY, &worker->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_FIXED_ARRAY, &worker->key)))
        {
          naError("Read object is null but type does not allow this. Member stays uninitialized.");
        }else if((na_findJSONRule(ruleSet, NA_JSON_RULE_OBJECT, &worker->key))) {
          naError("Read object is null but is not stored as a pointer. Members stay uninitialized.");
        }else
      #endif
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_POINTER_OBJECT, &worker->key))) {
        const NA_JSONPointerObjectRule* pointerObjectRule = (const NA_JSONPointerObjectRule*)rule;
        *((void**)((NAByte*)object + pointerObjectRule->memberOffset)) = NA_NULL;
      }
      break;

    case NA_JSON_PARSE_BOOLEAN:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_BOOL, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((NABool*)((NAByte*)object + memberRule->memberOffset)) = worker->boolean;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int32*)((NAByte*)object + memberRule->memberOffset)) = (int32)worker->boolean;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int64*)((NAByte*)object + memberRule->memberOffset)) = naCastBoolToi64(worker->boolean);
      }
      break;

    case NA_JSON_PARSE_NUMBER:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_BOOL, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((NABool*)((NAByte*)object + memberRule->memberOffset)) = (NABool)worker->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int32*)((NAByte*)object + memberRule->memberOffset)) = (int32)worker->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int64*)((NAByte*)object + memberRule->memberOffset)) = naCastDoubleToi64(worker->number);
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_DOUBLE, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((double*)((NAByte*)object + memberRule->memberOffset)) = worker->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FLOAT, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((float*)((NAByte*)object + memberRule->memberOffset)) = (float)worker->number;
      }
      break;

    case NA_JSON_PARSE_STRING:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_STRING, &worker->key))) {
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        na_StoreJSONString(worker, (NAUTF8Char**)((NAByte*)object + memberRule->memberOffset));
      }
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      if(  (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_DYNAMIC_ARRAY, &worker->key))
        || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FIXED_ARRAY, &worker->key))) {
        na_ParseJSONArray(worker, object, rule);
      }
      break;

    case NA_JSON_PARSE_OBJECT_START:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_OBJECT, &worker->key))) {
        const NA_JSONObjectRule* objectRule = (const NA_JSONObjectRule*)rule;
        subObject = object;
        subRuleSet = objectRule->ruleSet;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_POINTER_OBJECT, &worker->key))) {
        const NA_JSONPointerObjectRule* pointerObjectRule = (const NA_JSONPointerObjectRule*)rule;
        subObject = naMalloc(pointerObjectRule->structSize);
        *((void**)((NAByte*)object + pointerObjectRule->memberOffset)) = subObject;
        subRuleSet = pointerObjectRule->ruleSet;
      }else{
        subObject = NA_NULL;
        subRuleSet = NA_NULL;
      }
      na_ParseJSONObject(worker, subObject, subRuleSet);
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
}



NA_HDEF void na_WriteJSONKey(NABufferIterator* bufIt, NA_JSONString* key) {
  if(key->strLen) {
    naWriteBufferu8(bufIt, '\"');
    naWriteBufferBytes(bufIt, key->ptr, key->strLen);
    naWriteBufferStringWithFormat(bufIt, "\" : ");
  }
}


#define NA_JSON_INDENT "  "

NA_HDEF void na_WriteJSONRuleSetObject(NAJSONWorker* worker, void* object, NABufferIterator* bufIt, const NAJSONRuleSet* ruleSet, NAUTF8Char* ident) {
  for(size_t i = 0; i < naGetStackCount(&ruleSet->ruleStack); ++i) {

    const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)(ruleSet->rules[i]);
    const NA_JSONFixedArrayRule* fixedArrayRule = (const NA_JSONFixedArrayRule*)(ruleSet->rules[i]);
    const NA_JSONObjectRule* objectRule = (const NA_JSONObjectRule*)(ruleSet->rules[i]);
    const NA_JSONPointerObjectRule* pointerObjectRule = (const NA_JSONPointerObjectRule*)(ruleSet->rules[i]);
    void* subObject = NA_NULL;

    switch(ruleSet->rules[i]->type) {
    case NA_JSON_RULE_BOOL:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      naWriteBufferStringWithFormat(
        bufIt,
        "%s" NA_NL,
        *((NABool*)((NAByte*)object + memberRule->memberOffset)) ? "true" : "false");
      break;
    case NA_JSON_RULE_INT32:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      naWriteBufferStringWithFormat(
        bufIt,
        "%d" NA_NL,
        *((int32*)((NAByte*)object + memberRule->memberOffset)));
      break;
    case NA_JSON_RULE_INT64:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      naWriteBufferStringWithFormat(
        bufIt,
        "%ld" NA_NL,
        *((int64*)((NAByte*)object + memberRule->memberOffset)));
      break;
    case NA_JSON_RULE_DOUBLE:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      naWriteBufferStringWithFormat(
        bufIt,
        "%g" NA_NL,
        *((double*)((NAByte*)object + memberRule->memberOffset)));
      break;
    case NA_JSON_RULE_FLOAT:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      naWriteBufferStringWithFormat(
        bufIt,
        "%g" NA_NL,
        (double)*((float*)((NAByte*)object + memberRule->memberOffset)));
      break;
    case NA_JSON_RULE_STRING:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      naWriteBufferStringWithFormat(
        bufIt,
        "\"%s\"" NA_NL,
        *(NAUTF8Char**)((NAByte*)object + memberRule->memberOffset));
      break;

//    case NA_JSON_RULE_FIXED_ARRAY:
//      naWriteBufferStringWithFormat(bufIt, "%s", ident);
//      naWriteBufferStringWithFormat(
//        bufIt,
//        "[" NA_NL);
//      for(size_t a = 0; a < fixedArrayRule->elementCount; ++a) {
//        na_WriteJSONRuleSetObject(
//          worker,
//          object,
//          bufIt,
//          fixedArrayRule->);
//      }
//      naWriteBufferStringWithFormat(bufIt, "%s", ident);
//      naWriteBufferStringWithFormat(
//        bufIt,
//        "]" NA_NL);
//      break;

    case NA_JSON_RULE_OBJECT:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      naWriteBufferStringWithFormat(
        bufIt,
        "{" NA_NL);
      na_WriteJSONRuleSetObject(
        worker,
        object,
        bufIt,
        objectRule->ruleSet,
        naAllocSprintf(NA_TRUE, "%s" NA_JSON_INDENT, ident));
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      naWriteBufferStringWithFormat(
        bufIt,
        "}" NA_NL);
      break;

    case NA_JSON_RULE_POINTER_OBJECT:
      naWriteBufferStringWithFormat(bufIt, "%s", ident);
      na_WriteJSONKey(bufIt, &ruleSet->rules[i]->key);
      subObject = *(void**)((NAByte*)object + pointerObjectRule->memberOffset);
      if(!subObject) {
        naWriteBufferStringWithFormat(
          bufIt,
          "null" NA_NL);
      }else{
        naWriteBufferStringWithFormat(
          bufIt,
          "{" NA_NL);
        na_WriteJSONRuleSetObject(
          worker,
          subObject,
          bufIt,
          pointerObjectRule->ruleSet,
          naAllocSprintf(NA_TRUE, "%s" NA_JSON_INDENT, ident));
        naWriteBufferStringWithFormat(bufIt, "%s", ident);
        naWriteBufferStringWithFormat(
          bufIt,
          "}" NA_NL);
      }
      break;
      

    default:
      break;
    }
  }
}


NA_HDEF void na_ParseJSONObject(NAJSONWorker* worker, void* object, const NAJSONRuleSet* ruleSet) {
  if(!ruleSet) {
    // In case no rule is given, we just read the contents without storing
    // anything.
    na_ParseJSONDummyObject(worker);
  }else{
    na_ParseJSONRuleSetObject(worker, object, ruleSet);
  }
}

NA_HDEF void na_WriteJSONObject(NAJSONWorker* worker, void* object, NABufferIterator* bufIt, const NAJSONRuleSet* ruleSet) {
  if(!ruleSet) {
    // In case no rule is given, we just write nothing.
  }else{
    na_WriteJSONRuleSetObject(worker, object, bufIt, ruleSet, "");
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
