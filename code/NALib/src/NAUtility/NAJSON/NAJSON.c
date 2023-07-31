
#include "../NAJSON.h"
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

typedef struct NA_JSONString NA_JSONString;
struct NA_JSONString{
  const NAUTF8Char* ptr;
  size_t strLen;
};

typedef struct NAJSONRule NAJSONRule;
struct NAJSONRule{
  NA_JSONString key;
  NA_JSONDataType type;
  #if NA_DEBUG
    NABool hasBeenAdded;
  #endif
};

typedef struct NA_JSONMemberRule NA_JSONMemberRule;
struct NA_JSONMemberRule{
  NAJSONRule baseRule;
  size_t memberOffset;
};

typedef struct NA_JSONFixedArrayRule NA_JSONFixedArrayRule;
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

typedef struct NA_JSONDynamicArrayRule NA_JSONDynamicArrayRule;
struct NA_JSONDynamicArrayRule{
  NAJSONRule baseRule;
  size_t arrayOffset;
  size_t countOffset;
  NA_JSONDataType contentType;
  size_t structSize;
  NABool storeAsPointer;
  NAJSONRule* subRule;
};

typedef struct NA_JSONObjectRule NA_JSONObjectRule;
struct NA_JSONObjectRule{
  NAJSONRule baseRule;
  const NAJSONRuleSet* ruleSet;
};

typedef struct NA_JSONPointerObjectRule NA_JSONPointerObjectRule;
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

struct NAJSONParser{
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

NA_HIDEF void na_FillJSONString(NA_JSONString* dst, const NAUTF8Char* src){
  #if NA_DEBUG
    if(!src)
      naCrash("src is Nullptr");
  #endif
  
  dst->ptr = src;
  dst->strLen = strlen(src);
}

NA_HIDEF void na_ResetJSONString(NA_JSONString* dst){
  dst->ptr = "";
  dst->strLen = 0;
}

NA_HIDEF NABool na_EqualJSONString(const NA_JSONString* string1, const NA_JSONString* string2){
  if(string1->strLen != string2->strLen)
    return NA_FALSE;
  return !memcmp(string1->ptr, string2->ptr, string1->strLen);
}

NA_HIDEF void na_StoreJSONString(const NAJSONParser* parser, NAUTF8Char** storage){
  *storage = naMalloc(parser->string.strLen + 1);
  naCopyn(*storage, parser->string.ptr, parser->string.strLen);
  (*storage)[parser->string.strLen] = '\0';
}



// ///////////////////
// Rules and Rule Sets

NA_HDEF void na_DeallocJSONRule(NAJSONRule* rule){
  NA_JSONFixedArrayRule* fixedArrayRule;
  NA_JSONDynamicArrayRule* dynamicArrayRule;
  
  switch(rule->type){
  case NA_JSON_RULE_FIXED_ARRAY:
    fixedArrayRule = (NA_JSONFixedArrayRule*)rule;
    if(fixedArrayRule->subRule){
      na_DeallocJSONRule(fixedArrayRule->subRule);
    }
    break;
  case NA_JSON_RULE_DYNAMIC_ARRAY:
    dynamicArrayRule = (NA_JSONDynamicArrayRule*)rule;
    if(dynamicArrayRule->subRule){
      na_DeallocJSONRule(dynamicArrayRule->subRule);
    }
    break;
  default:
    break;
  }
  naFree(rule);
}

NA_DEF NAJSONRuleSet* naRegisterJSONRuleSet(NAJSONParser* parser){
  #if NA_DEBUG
    if(!parser)
      naCrash("parser is Nullptr");
    if(parser->prepared)
      naError("The parser has already been used. Adding more rule sets results in undefined behaviour.");
  #endif

  NAJSONRuleSet* ruleSet = naAlloc(NAJSONRuleSet);
  naInitStack(&ruleSet->ruleStack, sizeof(NAJSONRule*), 0, 0);
  ruleSet->rules = NA_NULL;
  #if NA_DEBUG
    ruleSet->prepared = NA_FALSE;
  #endif
  
  // Store the ruleset as part of the parser so it can be erased automatically.
  NAJSONRuleSet** ruleSetPtr = naPushStack(&parser->ruleSetStack);
  *ruleSetPtr = ruleSet;
  
  return ruleSet;
}

NA_HDEF void na_DeallocJSONRuleSet(NAJSONRuleSet* ruleSet){
  if(ruleSet->rules){
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
      naCrash("ruleSet is Nullptr");
    if(!rule)
      naCrash("rule is Nullptr");
    if(rule->hasBeenAdded)
      naError("rule has already been added to another ruleSet. This is not allowed and will likely result in memory corruption.");
    if(ruleSet->prepared)
      naError("The ruleSet is part of a parser which has already been used. Adding more rules results in undefined behaviour.");
  #endif

  NAJSONRule** rulePtr = naPushStack(&ruleSet->ruleStack);
  *rulePtr = rule;
  na_FillJSONString(&rule->key, key);
  #if NA_DEBUG
    rule->hasBeenAdded = NA_TRUE;
  #endif
}

NA_DEF void na_initJSONRule(NAJSONRule* rule, NA_JSONDataType type){
  rule->type = type;
  #if NA_DEBUG
    rule->hasBeenAdded = NA_FALSE;
  #endif
}

NA_DEF NAJSONRule* naNewJSONRuleInt32(size_t memberOffset){
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_INT32);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleInt64(size_t memberOffset){
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_INT64);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleDouble(size_t memberOffset){
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_DOUBLE);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFloat(size_t memberOffset){
  NA_JSONMemberRule* rule = naAlloc(NA_JSONMemberRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_FLOAT);
  rule->memberOffset = memberOffset;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleString(size_t memberOffset){
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



NA_DEF NAJSONRule* naNewJSONRuleObject(const NAJSONRuleSet* subRuleSet){
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



NA_HDEF const NAJSONRule* na_findJSONRule(const NAJSONRuleSet* ruleSet, NA_JSONDataType type, const NA_JSONString* key){
  if(ruleSet){
    for(size_t i = 0; i < naGetStackCount(&ruleSet->ruleStack); ++i){
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

NA_HIDEF void na_JSONPushStackStatus(NAJSONParser* parser, NA_JSONStackStatus status){
  #if NA_DEBUG
    if(parser->curStackStatus == NA_JSON_PARSE_STACK_DEPTH - 1)
      naError("Stack overflow. Heap corruption imminent...");
  #endif
  parser->curStackStatus++;
  parser->stackStatusStack[parser->curStackStatus] = status;
}

NA_HIDEF NA_JSONStackStatus na_JSONTopStackStatus(NAJSONParser* parser){
  return parser->stackStatusStack[parser->curStackStatus];
}

NA_HIDEF void na_JSONPopStackStatus(NAJSONParser* parser){
  #if NA_DEBUG
    if(parser->curStackStatus == 0)
      naError("Stack underflow. Heap corruption imminent...");
  #endif
  parser->curStackStatus--;
}

void ns_JSONAdjustStackStatusAfterValueRead(NAJSONParser* parser){
  NA_JSONStackStatus stackStatus = na_JSONTopStackStatus(parser);
  if(stackStatus == NA_JSON_OBJECT_EXPECTING_VALUE){
    na_JSONPopStackStatus(parser);
    na_JSONPushStackStatus(parser, NA_JSON_OBJECT_EXPECTING_KEY_OR_END);
  }
}



// ///////////////////
// Parser

NA_DEF NAJSONParser* naAllocateJSONParser(){
  NAJSONParser* parser = naAlloc(NAJSONParser);
  parser->curPtr = NA_NULL;
  
  parser->stackStatusStack = naMalloc(NA_JSON_PARSE_STACK_DEPTH * sizeof(NA_JSONStackStatus));
  parser->curStackStatus = 0;
  parser->stackStatusStack[parser->curStackStatus] = NA_JSON_FILE_EXPECTING_VALUE;
  parser->prepared = NA_FALSE;
  
  parser->parseStatus = NA_JSON_PARSE_STATUS_UNDEFINED;
  
  naInitStack(&parser->ruleSetStack, sizeof(NAJSONRuleSet*), 0, 0);

  parser->boolean = NA_FALSE;
  parser->number = 0.;
  na_ResetJSONString(&parser->string);
  na_ResetJSONString(&parser->key);

  return parser;
}

NA_DEF void naDeallocateJSONParser(NAJSONParser* parser){
  #if NA_DEBUG
    if(!parser)
      naCrash("parser is Nullptr");
  #endif

  naFree(parser->stackStatusStack);

  naForeachStackpMutable(&parser->ruleSetStack, (NAMutator)na_DeallocJSONRuleSet);
  naClearStack(&parser->ruleSetStack);
    
  naFree(parser);
}

// Prototype:
NA_HAPI void na_ParseJSONObject(NAJSONParser* parser, void* object, const NAJSONRuleSet* ruleSet);

void naParseJSONBuffer(
  NAJSONParser* parser,
  void* object,
  const void* buf,
  size_t byteCount)
{
  #if NA_DEBUG
    if(!parser)
      naCrash("parser is Nullptr");
    if(!buf)
      naCrash("buf is Nullptr");
    if(((NAByte*)buf)[byteCount - 1] != '\0')
      naError("buffer must end with a zero byte.");
  #else
    NA_UNUSED(byteCount);
  #endif

  parser->curPtr = buf;
  
  if(!parser->prepared){
    // Prepare all rules for faster access.
    NAStackIterator ruleSetIt = naMakeStackMutator(&parser->ruleSetStack);
    while(naIterateStack(&ruleSetIt)){
      NAJSONRuleSet* ruleSet = naGetStackCurpMutable(&ruleSetIt);
      if(ruleSet->rules){
        naFree(ruleSet->rules);
        ruleSet->rules = NA_NULL;
      }
      size_t ruleCount = naGetStackCount(&ruleSet->ruleStack);
      if(ruleCount){
        ruleSet->rules = naMalloc(ruleCount * sizeof(NAJSONRule*));
        naDumpStack(&ruleSet->ruleStack, ruleSet->rules);
      }
      #if NA_DEBUG
        ruleSet->prepared = NA_TRUE;
      #endif
    }
    naClearStackIterator(&ruleSetIt);
    parser->prepared = NA_TRUE;
  }

  NAJSONRuleSet** initialRuleSet = naTopStack(&parser->ruleSetStack);
  na_ParseJSONObject(parser, object, *initialRuleSet);
}



// ///////////////////
// Bytewise operation

NA_HIDEF NAByte na_CurJSONByte(NAJSONParser* parser){
  return *(const NAByte*)parser->curPtr;
}

NA_HIDEF NAByte na_NextJSONByte(NAJSONParser* parser){
  ++parser->curPtr;
  return *(const NAByte*)parser->curPtr;
}



// ///////////////////
// Parsing entities

NA_HDEF NA_JSONParseStatus na_ParseJSONNumber(NAJSONParser* parser){
  NAByte curByte = na_CurJSONByte(parser);
  
  int32 decimalShift = 0;
  int64 decimalSign = 1;
  int32 exponentSign = 1;
  int64 decimals = 0;
  int32 exponent = 0;
  
  if(curByte == '-'){
    decimalSign = -1;
    curByte = na_NextJSONByte(parser);
  }else if(curByte == '+'){
    curByte = na_NextJSONByte(parser);
  }
  
  while(isdigit(curByte)){
    decimals = (decimals << 1) + (decimals << 3) + (curByte & 0x0f);
    curByte = na_NextJSONByte(parser);
  }
  
  if(curByte == '.'){
    curByte = na_NextJSONByte(parser);
    const NAByte* firstDecimalPtr = parser->curPtr;

    while(isdigit(curByte)){
      decimals = (decimals << 1) + (decimals << 3) + (curByte & 0x0f);
      curByte = na_NextJSONByte(parser);
    }

    const NAByte* lastDecimalPtr = parser->curPtr;
    decimalShift = (int32)(lastDecimalPtr - firstDecimalPtr);
  }

  if((curByte | 0x20) == 'e'){
    curByte = na_NextJSONByte(parser);
    
    if(curByte == '-'){
      exponentSign = -1;
      curByte = na_NextJSONByte(parser);
    }else if(curByte == '+'){
      curByte = na_NextJSONByte(parser);
    }

    while(isdigit(curByte)){
      exponent = (exponent << 1) + (exponent << 3) + (curByte & 0x0f);
      curByte = na_NextJSONByte(parser);
    }
  }

  decimals = decimals * decimalSign;
  exponent = exponentSign * exponent - decimalShift;
  parser->number = (double)(decimals) * naExp10((double)(exponent));
  
  parser->parseStatus = NA_JSON_PARSE_NUMBER;
  return parser->parseStatus;
}

NA_HDEF NA_JSONParseStatus na_ParseJSONIdentifier(NAJSONParser* parser){
  const NAByte* firstByte = parser->curPtr;
  NAByte curByte = na_NextJSONByte(parser);
  while (isalpha(curByte)){
    curByte = na_NextJSONByte(parser);
  }

  const size_t strLen = parser->curPtr - firstByte;
  if(strLen == 4 && !memcmp(firstByte, "null", 4)){
    parser->parseStatus = NA_JSON_PARSE_NULL;
    return parser->parseStatus;
  }else if(strLen == 4 && !memcmp(firstByte, "true", 4)){
    parser->boolean = NA_TRUE;
    parser->parseStatus = NA_JSON_PARSE_BOOLEAN;
    return parser->parseStatus;
  }else if(strLen == 5 && !memcmp(firstByte, "false", 5)){
    parser->boolean = NA_FALSE;
    parser->parseStatus = NA_JSON_PARSE_BOOLEAN;
    return parser->parseStatus;
  }

  parser->parseStatus = (curByte == '\0')
    ? NA_JSON_PARSE_BUFFER_END
    : NA_JSON_PARSE_UNKNOWN_IDENTIFIER;
  #if NA_DEBUG
    if(parser->parseStatus == NA_JSON_PARSE_UNKNOWN_IDENTIFIER)
      naError("Unknown Identifier\n");
  #endif
  return parser->parseStatus;
}

NA_HDEF NA_JSONParseStatus na_ParseJSONString(NAJSONParser* parser){
  NAByte curByte = na_CurJSONByte(parser);

  NA_JSONStackStatus stackStatus = na_JSONTopStackStatus(parser);
  if(stackStatus == NA_JSON_OBJECT_EXPECTING_KEY_OR_END){
    parser->key.ptr = (const NAUTF8Char*)parser->curPtr;
  }else{
    parser->string.ptr = (const NAUTF8Char*)parser->curPtr;
  }

  while(curByte > '\0'){
    if(curByte == '\\'){
      na_NextJSONByte(parser);
    }else if(curByte == '\"'){
      if(stackStatus == NA_JSON_OBJECT_EXPECTING_KEY_OR_END){
        parser->key.strLen = (const NAUTF8Char*)parser->curPtr - parser->key.ptr;
      }else{
        parser->string.strLen = (const NAUTF8Char*)parser->curPtr - parser->string.ptr;
      }
      na_NextJSONByte(parser);
      parser->parseStatus = NA_JSON_PARSE_STRING;
      return parser->parseStatus;
    }
    curByte = na_NextJSONByte(parser);
  }
  
  parser->parseStatus = NA_JSON_PARSE_BUFFER_END;
  return parser->parseStatus;
}



// ///////////////////
// Parsing primitives

NA_HDEF NA_JSONParseStatus na_ParseJSONPrimitives(NAJSONParser* parser){
  while(1){
    NAByte curByte = na_CurJSONByte(parser);
    
    // Skip any whitespace.
    while (curByte > '\0' && curByte <= ' ') {
      curByte = na_NextJSONByte(parser);
    }
    
    // Reset the key string, except while reading an object entry.
    NA_JSONStackStatus stackStatus = na_JSONTopStackStatus(parser);
    if(stackStatus != NA_JSON_OBJECT_EXPECTING_COLON && stackStatus != NA_JSON_OBJECT_EXPECTING_VALUE){
      na_ResetJSONString(&parser->key);
    }

    if(curByte == '\0'){
      parser->parseStatus = NA_JSON_PARSE_BUFFER_END;
      return parser->parseStatus;
    
    }else if(isdigit(curByte) || curByte == '-' || curByte == '+' || curByte == '.'){
      parser->parseStatus = na_ParseJSONNumber(parser);
      ns_JSONAdjustStackStatusAfterValueRead(parser);
      return parser->parseStatus;

    }else if(isalpha(curByte)){
      parser->parseStatus = na_ParseJSONIdentifier(parser);
      ns_JSONAdjustStackStatusAfterValueRead(parser);
      return parser->parseStatus;

    }else if(curByte == '\"'){
      na_NextJSONByte(parser);
      parser->parseStatus = na_ParseJSONString(parser);
      if(stackStatus == NA_JSON_OBJECT_EXPECTING_KEY_OR_END){
        // This was a key
        na_JSONPopStackStatus(parser);
        na_JSONPushStackStatus(parser, NA_JSON_OBJECT_EXPECTING_COLON);
        continue;
      }else{
        // This was a value
        ns_JSONAdjustStackStatusAfterValueRead(parser);
        return parser->parseStatus;
      }
      
    }else if(curByte == '['){
      na_NextJSONByte(parser);
      na_JSONPushStackStatus(parser, NA_JSON_ARRAY_EXPECTING_VALUE_OR_END);
      parser->parseStatus = NA_JSON_PARSE_ARRAY_START;
      return parser->parseStatus;
      
    }else if(curByte == ']'){
      na_NextJSONByte(parser);
      if(parser->curStackStatus == 0){
        parser->parseStatus = NA_JSON_PARSE_UNEXPECTED_ARRAY_END;
        #if NA_DEBUG
          naError("Unexpected Array End\n");
        #endif
        return parser->parseStatus;
      }
      na_JSONPopStackStatus(parser);
      ns_JSONAdjustStackStatusAfterValueRead(parser);
      parser->parseStatus = (stackStatus != NA_JSON_ARRAY_EXPECTING_VALUE_OR_END)
        ? NA_JSON_PARSE_UNEXPECTED_ARRAY_END
        : NA_JSON_PARSE_ARRAY_END;
      #if NA_DEBUG
      if(parser->parseStatus == NA_JSON_PARSE_UNEXPECTED_ARRAY_END)
        naError("Unexpected Array End\n");
      #endif
      return parser->parseStatus;
      
    }else if(curByte == '{'){
      na_NextJSONByte(parser);
      na_JSONPushStackStatus(parser, NA_JSON_OBJECT_EXPECTING_KEY_OR_END);
      parser->parseStatus = NA_JSON_PARSE_OBJECT_START;
      return parser->parseStatus;
      
    }else if(curByte == '}'){
      na_NextJSONByte(parser);
      if(parser->curStackStatus == 0){
        parser->parseStatus = NA_JSON_PARSE_UNEXPECTED_OBJECT_END;
        #if NA_DEBUG
          naError("Unexpected Object End\n");
        #endif
        return parser->parseStatus;
      }
      na_JSONPopStackStatus(parser);
      ns_JSONAdjustStackStatusAfterValueRead(parser);
      parser->parseStatus = (stackStatus != NA_JSON_OBJECT_EXPECTING_KEY_OR_END)
        ? NA_JSON_PARSE_UNEXPECTED_OBJECT_END
        : NA_JSON_PARSE_OBJECT_END;
      #if NA_DEBUG
        if(parser->parseStatus == NA_JSON_PARSE_UNEXPECTED_OBJECT_END)
          naError("Unexpected Object End\n");
      #endif
      return parser->parseStatus;
      
    }else if(curByte == ','){
      na_NextJSONByte(parser);
      if(stackStatus != NA_JSON_ARRAY_EXPECTING_VALUE_OR_END
        && stackStatus != NA_JSON_OBJECT_EXPECTING_KEY_OR_END){
        parser->parseStatus = NA_JSON_PARSE_UNEXPECTED_COMMA;
        #if NA_DEBUG
          naError("Unexpected Comma\n");
        #endif
        return parser->parseStatus;
      }
      continue;

    }else if(curByte == ':'){
      na_NextJSONByte(parser);
      if(stackStatus != NA_JSON_OBJECT_EXPECTING_COLON){
        parser->parseStatus = NA_JSON_PARSE_UNEXPECTED_COLON;
        #if NA_DEBUG
          naError("Unexpected Colon\n");
        #endif
        return parser->parseStatus;
      }
      na_JSONPopStackStatus(parser);
      na_JSONPushStackStatus(parser, NA_JSON_OBJECT_EXPECTING_VALUE);
      continue;
    }
  
    parser->parseStatus = NA_JSON_PARSE_UNKNOWN_CHARACTER;
    #if NA_DEBUG
      naError("Unknown Character\n");
    #endif
    na_NextJSONByte(parser);
    return parser->parseStatus;
  }
}
    
    

// Prototype:
NA_HAPI void na_ParseJSONArray(NAJSONParser* parser, void* object, const NAJSONRule* elementRule);
NA_HAPI void na_ParseJSONDummyObject(NAJSONParser* parser);


// ///////////////////
// Parsing Arrays

NA_HDEF void na_ParseJSONDummyArray(NAJSONParser* parser){
  NABool unexpectedEnd = NA_FALSE;
  while(!unexpectedEnd && na_ParseJSONPrimitives(parser) != NA_JSON_PARSE_ARRAY_END){

    switch(parser->parseStatus){
    case NA_JSON_PARSE_ARRAY_START:
        na_ParseJSONDummyArray(parser);
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
        na_ParseJSONDummyObject(parser);
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

NA_HDEF void na_ParseJSONFixedArray(NAJSONParser* parser, void* object, const NA_JSONFixedArrayRule* elementRule){
  NABool unexpectedEnd = NA_FALSE;
  void* subObject;
  void* arrayPtr = (NAByte*)object + elementRule->arrayOffset;
  NA_JSONObjectRule* subObjectRule = (NA_JSONObjectRule*)elementRule->subRule;

  #if NA_DEBUG
    size_t count = 0;
  #endif

  while(!unexpectedEnd && na_ParseJSONPrimitives(parser) != NA_JSON_PARSE_ARRAY_END){

    switch(parser->parseStatus){
    case NA_JSON_PARSE_BOOLEAN:
      switch(elementRule->contentType){
      case NA_JSON_RULE_INT32:
        na_incArrayCount();
        *(int32*)arrayPtr = (int32)parser->boolean;
        break;
      case NA_JSON_RULE_INT64:
        na_incArrayCount();
        *(int64*)arrayPtr = (int64)parser->boolean;
        break;
      default:
        break;
      }
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_NUMBER:
      switch(elementRule->contentType){
      case NA_JSON_RULE_INT32:
        na_incArrayCount();
        *(int32*)arrayPtr = (int32)parser->number;
        break;
      case NA_JSON_RULE_INT64:
        na_incArrayCount();
        *(int64*)arrayPtr = (int64)parser->number;
        break;
      case NA_JSON_RULE_DOUBLE:
        na_incArrayCount();
        *(double*)arrayPtr = parser->number;
        break;
      case NA_JSON_RULE_FLOAT:
        na_incArrayCount();
        *(float*)arrayPtr = (float)parser->number;
        break;
      default:
        break;
      }
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_STRING:
      if(elementRule->contentType == NA_JSON_RULE_STRING){
        na_incArrayCount();
        na_StoreJSONString(parser, (NAUTF8Char**)arrayPtr);
      }
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      na_incArrayCount();
      na_ParseJSONArray(parser, arrayPtr, elementRule->subRule);
      arrayPtr = (NAByte*)arrayPtr + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
      na_incArrayCount();
      if(elementRule->storeAsPointer){
        subObject = naMalloc(elementRule->structSize);
        *((void**)(arrayPtr)) = subObject;
      }else{
        subObject = arrayPtr;
      }
      na_ParseJSONObject(parser, subObject, subObjectRule->ruleSet);
      if(elementRule->storeAsPointer){
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

NA_HDEF void na_ParseJSONDynamicArray(NAJSONParser* parser, void* object, const NA_JSONDynamicArrayRule* elementRule){
  NABool unexpectedEnd = NA_FALSE;
  void* subObject;
  NAStack elementStack;
  NA_JSONObjectRule* subObjectRule = (NA_JSONObjectRule*)elementRule->subRule;

  if(elementRule->storeAsPointer){
    naInitStack(&elementStack, sizeof(void*), 0, 0);
  }else{
    naInitStack(&elementStack, elementRule->structSize, 0, 0);
  }

  while(!unexpectedEnd && na_ParseJSONPrimitives(parser) != NA_JSON_PARSE_ARRAY_END){

    switch(parser->parseStatus){
    case NA_JSON_PARSE_BOOLEAN:
      break;
      
    case NA_JSON_PARSE_NUMBER:
      switch(elementRule->contentType){
      case NA_JSON_RULE_INT32:
        if(elementRule->storeAsPointer){
          int32** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(int32));
          **curNumberPtrPtr = (int32)parser->number;
        }else{
          int32* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = (int32)parser->number;
        }
        break;
      case NA_JSON_RULE_INT64:
        if(elementRule->storeAsPointer){
          int64** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(int64));
          **curNumberPtrPtr = (int64)parser->number;
        }else{
          int64* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = (int64)parser->number;
        }
        break;
      case NA_JSON_RULE_DOUBLE:
        if(elementRule->storeAsPointer){
          double** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(double));
          **curNumberPtrPtr = parser->number;
        }else{
          double* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = parser->number;
        }
        break;
      case NA_JSON_RULE_FLOAT:
        if(elementRule->storeAsPointer){
          float** curNumberPtrPtr = naPushStack(&elementStack);
          *curNumberPtrPtr = naMalloc(sizeof(float));
          **curNumberPtrPtr = (float)parser->number;
        }else{
          float* curNumberPtr = naPushStack(&elementStack);
          *curNumberPtr = (float)parser->number;
        }
        break;
      default:
        break;
      }
      break;
      
    case NA_JSON_PARSE_STRING:
      if(elementRule->storeAsPointer){
        #if NA_DEBUG
          naError("Storing a pointer to a C-string in a dynamic array is not allowed. Would result in char*** and that's just stupid.");
        #endif
      }else{
        NAUTF8Char** curStringPtr = naPushStack(&elementStack);
        na_StoreJSONString(parser, curStringPtr);
      }
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      if(elementRule->storeAsPointer){
        void** curObjectPtrPtr = naPushStack(&elementStack);
        *curObjectPtrPtr = naMalloc(elementRule->structSize);
        subObject = *curObjectPtrPtr;
      }else{
        void* curObjectPtr = naPushStack(&elementStack);
        subObject = curObjectPtr;
      }
      na_ParseJSONArray(parser, subObject, elementRule->subRule);
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
      if(elementRule->storeAsPointer){
        void** curObjectPtrPtr = naPushStack(&elementStack);
        *curObjectPtrPtr = naMalloc(elementRule->structSize);
        subObject = *curObjectPtrPtr;
      }else{
        void* curObjectPtr = naPushStack(&elementStack);
        subObject = curObjectPtr;
      }
      na_ParseJSONObject(parser, subObject, subObjectRule->ruleSet);
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
  
  if(elementCount){
    void** array = (void**)((NAByte*)object + elementRule->arrayOffset);
    if(elementRule->storeAsPointer){
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

NA_HDEF void na_ParseJSONArray(NAJSONParser* parser, void* object, const NAJSONRule* elementRule){
  if(!elementRule){
    // In case no rule is given, we just read the contents without storing
    // anything.
    na_ParseJSONDummyArray(parser);
  }else if(elementRule->type == NA_JSON_RULE_FIXED_ARRAY){
    na_ParseJSONFixedArray(parser, object, (NA_JSONFixedArrayRule*)elementRule);
  }else if(elementRule->type == NA_JSON_RULE_DYNAMIC_ARRAY){
    na_ParseJSONDynamicArray(parser, object, (NA_JSONDynamicArrayRule*)elementRule);
  }
}



// ///////////////////
// Parsing Objects

NA_HDEF void na_ParseJSONDummyObject(NAJSONParser* parser){
  NABool unexpectedEnd = NA_FALSE;
  while(!unexpectedEnd && na_ParseJSONPrimitives(parser) != NA_JSON_PARSE_OBJECT_END){

    switch(parser->parseStatus){
    case NA_JSON_PARSE_ARRAY_START:
        na_ParseJSONDummyArray(parser);
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
        na_ParseJSONDummyObject(parser);
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
}

NA_HDEF void na_ParseJSONRuleSetObject(NAJSONParser* parser, void* object, const NAJSONRuleSet* ruleSet){
  NABool unexpectedEnd = NA_FALSE;
  void* subObject;
  const NAJSONRuleSet* subRuleSet = NA_NULL;

  while(!unexpectedEnd && na_ParseJSONPrimitives(parser) != NA_JSON_PARSE_OBJECT_END){
    const NAJSONRule* rule = NA_NULL;
        
    switch(parser->parseStatus){
    case NA_JSON_PARSE_NULL:
      #if NA_DEBUG
        if(  (na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &parser->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &parser->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_DOUBLE, &parser->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_FLOAT, &parser->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_STRING, &parser->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_DYNAMIC_ARRAY, &parser->key))
          || (na_findJSONRule(ruleSet, NA_JSON_RULE_FIXED_ARRAY, &parser->key)))
        {
          naError("Read object is null but type does not allow this. Member stays uninitialized.");
        }else if((na_findJSONRule(ruleSet, NA_JSON_RULE_OBJECT, &parser->key))){
          naError("Read object is null but is not stored as a pointer. Members stay uninitialized.");
        }else
      #endif
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_POINTER_OBJECT, &parser->key))){
        const NA_JSONPointerObjectRule* pointerObjectRule = (const NA_JSONPointerObjectRule*)rule;
        *((void**)((NAByte*)object + pointerObjectRule->memberOffset)) = NA_NULL;
      }
      break;

    case NA_JSON_PARSE_BOOLEAN:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int32*)((NAByte*)object + memberRule->memberOffset)) = parser->boolean;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int64*)((NAByte*)object + memberRule->memberOffset)) = parser->boolean;
      }
      break;

    case NA_JSON_PARSE_NUMBER:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int32*)((NAByte*)object + memberRule->memberOffset)) = (int32)parser->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int64*)((NAByte*)object + memberRule->memberOffset)) = (int64)parser->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_DOUBLE, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((double*)((NAByte*)object + memberRule->memberOffset)) = parser->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FLOAT, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((float*)((NAByte*)object + memberRule->memberOffset)) = (float)parser->number;
      }
      break;

    case NA_JSON_PARSE_STRING:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_STRING, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        na_StoreJSONString(parser, (NAUTF8Char**)((NAByte*)object + memberRule->memberOffset));
      }
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      if(  (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_DYNAMIC_ARRAY, &parser->key))
        || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FIXED_ARRAY, &parser->key))){
        na_ParseJSONArray(parser, object, rule);
      }
      break;

    case NA_JSON_PARSE_OBJECT_START:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_OBJECT, &parser->key))){
        const NA_JSONObjectRule* objectRule = (const NA_JSONObjectRule*)rule;
        subObject = object;
        subRuleSet = objectRule->ruleSet;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_POINTER_OBJECT, &parser->key))){
        const NA_JSONPointerObjectRule* pointerObjectRule = (const NA_JSONPointerObjectRule*)rule;
        subObject = naMalloc(pointerObjectRule->structSize);
        *((void**)((NAByte*)object + pointerObjectRule->memberOffset)) = subObject;
        subRuleSet = pointerObjectRule->ruleSet;
      }else{
        subObject = NA_NULL;
        subRuleSet = NA_NULL;
      }
      na_ParseJSONObject(parser, subObject, subRuleSet);
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
}


NA_HDEF void na_ParseJSONObject(NAJSONParser* parser, void* object, const NAJSONRuleSet* ruleSet){
  if(!ruleSet){
    // In case no rule is given, we just read the contents without storing
    // anything.
    na_ParseJSONDummyObject(parser);
  }else{
    na_ParseJSONRuleSetObject(parser, object, ruleSet);
  }
}

