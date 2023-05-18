
#include "NAJSON.h"
#include <ctype.h>

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
  NA_JSON_RULE_ARRAY,
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

typedef struct NA_JSONArrayRule NA_JSONArrayRule;
struct NA_JSONArrayRule{
  NAJSONRule baseRule;
  size_t arrayOffset;
  size_t countOffset;
  NA_JSONDataType contentType;
  size_t structSize;
  NABool storeAsPointer;
  NAJSONRule* subRule;
};

typedef struct NA_JSONFixedArrayRule NA_JSONFixedArrayRule;
struct NA_JSONFixedArrayRule{
  NAJSONRule baseRule;
  size_t arrayOffset;
  NA_JSONDataType contentType;
  size_t structSize;
  size_t elementCount;
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
  NAStack rules;
};

struct NAJSONParser{
  const void* buf;
  size_t bufLen;
  const NAByte* curPtr;
  NA_JSONParseStatus parseStatus;
  NAStack stackStatus;
  void* object;
  
  NAStack ruleSets;
  const NAJSONRuleSet* initialRuleSet;
  
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
      naError("src is Nullptr");
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
  NA_JSONArrayRule* arrayRule;
  NA_JSONFixedArrayRule* fixedArrayRule;
  
  switch(rule->type){
  case NA_JSON_RULE_ARRAY:
    arrayRule = (NA_JSONArrayRule*)rule;
    if(arrayRule->subRule){
      na_DeallocJSONRule(arrayRule->subRule);
    }
    break;
  case NA_JSON_RULE_FIXED_ARRAY:
    fixedArrayRule = (NA_JSONFixedArrayRule*)rule;
    if(fixedArrayRule->subRule){
      na_DeallocJSONRule(fixedArrayRule->subRule);
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
  #endif

  NAJSONRuleSet* ruleSet = naAlloc(NAJSONRuleSet);
  naInitStack(&ruleSet->rules, sizeof(NAJSONRule*), 0, 0);
  
  // Store the ruleset as part of the parser so it can be erased automatically.
  NAJSONRuleSet** ruleSetPtr = naPushStack(&parser->ruleSets);
  *ruleSetPtr = ruleSet;
  
  return ruleSet;
}

NA_HDEF void na_DeallocJSONRuleSet(NAJSONRuleSet* ruleSet){
  naForeachStackpMutable(&ruleSet->rules, (NAMutator)na_DeallocJSONRule);
  naClearStack(&ruleSet->rules);
  naFree(ruleSet);
}

NA_DEF void naSetJSONParserInitialRules(NAJSONParser* parser, const NAJSONRuleSet* ruleSet){
  #if NA_DEBUG
    if(!parser)
      naCrash("parser is Nullptr");
    if(!ruleSet)
      naError("ruleSet is Nullptr");
  #endif

  parser->initialRuleSet = ruleSet;
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
      naError("rule is Nullptr");
    if(rule->hasBeenAdded)
      naError("rule has already been added to another ruleSet. This is not allowed and will likely result in memory corruption.");
  #endif

  NAJSONRule** rulePtr = naPushStack(&ruleSet->rules);
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

NA_DEF NAJSONRule* naNewJSONRuleArray(
  size_t arrayOffset,
  size_t countOffset,
  size_t structSize,
  NABool storeAsPointer,
  NAJSONRule* subRule)
{
  NA_JSONArrayRule* rule = naAlloc(NA_JSONArrayRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_ARRAY);
  rule->arrayOffset = arrayOffset;
  rule->countOffset = countOffset;
  rule->contentType = NA_JSON_RULE_UNDEFINED;
  rule->structSize = structSize;
  rule->storeAsPointer = storeAsPointer;
  rule->subRule = subRule;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedArray(
  size_t arrayOffset,
  size_t structSize,
  size_t elementCount,
  NAJSONRule* subRule)
{
  NA_JSONFixedArrayRule* rule = naAlloc(NA_JSONFixedArrayRule);
  na_initJSONRule(&rule->baseRule, NA_JSON_RULE_FIXED_ARRAY);
  rule->arrayOffset = arrayOffset;
  rule->contentType = NA_JSON_RULE_UNDEFINED;
  rule->structSize = structSize;
  rule->elementCount = elementCount;
  rule->subRule = subRule;

  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleArrayInt32(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONArrayRule* rule = (NA_JSONArrayRule*)naNewJSONRuleArray(
    arrayOffset,
    countOffset,
    sizeof(int32),
    NA_FALSE,
    NA_NULL);
  rule->contentType = NA_JSON_RULE_INT32;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleArrayInt64(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONArrayRule* rule = (NA_JSONArrayRule*)naNewJSONRuleArray(
    arrayOffset,
    countOffset,
    sizeof(int64),
    NA_FALSE,
    NA_NULL);
  rule->contentType = NA_JSON_RULE_INT64;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleArrayDouble(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONArrayRule* rule = (NA_JSONArrayRule*)naNewJSONRuleArray(
    arrayOffset,
    countOffset,
    sizeof(double),
    NA_FALSE,
    NA_NULL);
  rule->contentType = NA_JSON_RULE_DOUBLE;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleArrayFloat(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONArrayRule* rule = (NA_JSONArrayRule*)naNewJSONRuleArray(
    arrayOffset,
    countOffset,
    sizeof(float),
    NA_FALSE,
    NA_NULL);
  rule->contentType = NA_JSON_RULE_FLOAT;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleArrayString(
  size_t arrayOffset,
  size_t countOffset)
{
  NA_JSONArrayRule* rule = (NA_JSONArrayRule*)naNewJSONRuleArray(
    arrayOffset,
    countOffset,
    sizeof(NAUTF8Char*),
    NA_FALSE,
    NA_NULL);
  rule->contentType = NA_JSON_RULE_STRING;
  return &rule->baseRule;
}

NA_DEF NAJSONRule* naNewJSONRuleFixedArrayInt32(
  size_t arrayOffset,
  size_t elementCount)
{
  NA_JSONFixedArrayRule* rule = (NA_JSONFixedArrayRule*)naNewJSONRuleFixedArray(
    arrayOffset,
    sizeof(int32),
    elementCount,
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
    sizeof(int64),
    elementCount,
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
    sizeof(double),
    elementCount,
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
    sizeof(float),
    elementCount,
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
    sizeof(NAUTF8Char*),
    elementCount,
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
    NAStackIterator iter = naMakeStackAccessor(&ruleSet->rules);
    while(naIterateStack(&iter)){
      const NAJSONRule* const * rule = naGetStackCurConst(&iter);
      if((*rule)->type == type && na_EqualJSONString(&(*rule)->key, key))
      {
        naClearStackIterator(&iter);
        return *rule;
      }
    }
    naClearStackIterator(&iter);
  }
  return NA_NULL;
}



// ///////////////////
// Stack Status

NA_HIDEF void na_JSONPushStackStatus(NAJSONParser* parser, NA_JSONStackStatus status){
  *(NA_JSONStackStatus*)naPushStack(&parser->stackStatus) = status;
}

NA_HIDEF NA_JSONStackStatus na_JSONTopStackStatus(NAJSONParser* parser){
  return *(NA_JSONStackStatus*)naTopStack(&parser->stackStatus);
}

NA_HIDEF void na_JSONPopStackStatus(NAJSONParser* parser){
  naPopStack(&parser->stackStatus);
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
  parser->buf = NA_NULL;
  parser->bufLen = 0;
  parser->curPtr = NA_NULL;
  naInitStack(&parser->stackStatus, sizeof(NA_JSONStackStatus), 0, 0);
  na_JSONPushStackStatus(parser, NA_JSON_FILE_EXPECTING_VALUE);
  parser->parseStatus = NA_JSON_PARSE_STATUS_UNDEFINED;
  parser->object = NA_NULL;
  
  naInitStack(&parser->ruleSets, sizeof(NAJSONRuleSet*), 0, 0);
  parser->initialRuleSet = NA_NULL;

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

  naClearStack(&parser->stackStatus);
  
  naForeachStackpMutable(&parser->ruleSets, (NAMutator)na_DeallocJSONRuleSet);
  naClearStack(&parser->ruleSets);
  
  naFree(parser);
}

// Prototype:
void naParseJSONObject(NAJSONParser* parser, const NAJSONRuleSet* ruleSet);

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
      naError("buf is Nullptr");
  #endif

  parser->object = object;
  parser->buf = buf;
  parser->bufLen = byteCount ? byteCount : strlen(buf);
  parser->curPtr = buf;

  naParseJSONObject(parser, parser->initialRuleSet);
}



// ///////////////////
// Bytewise operation

NA_HIDEF NAByte na_CurJSONByte(NAJSONParser* parser){
  return *(const NAByte*)parser->curPtr;
}

NA_HIDEF NAByte na_NextJSONByte(NAJSONParser* parser){
  if (parser->curPtr - (const NAByte*)parser->buf < parser->bufLen){
    ++parser->curPtr;
    return *(const NAByte*)parser->curPtr;
  }
  return '\0';
}



// ///////////////////
// Parsing entities

NA_HDEF NA_JSONParseStatus na_ParseJSONNumber(NAJSONParser* parser){
  NAByte curByte = na_CurJSONByte(parser);
  
  int32 decimalShift = 0;
  int64 decimalSign = 1.;
  int32 exponentSign = 1;
  int64 decimals = 0;
  int32 exponent = 0;
  
  if(curByte == '+'){
    curByte = na_NextJSONByte(parser);
  }else if(curByte == '-'){
    decimalSign = -1;
    curByte = na_NextJSONByte(parser);
  }
  
  while(curByte > '\0' && isdigit(curByte)){
    decimals = decimals * 10 + curByte - '0';
    curByte = na_NextJSONByte(parser);
  }
  
  if(curByte == '.'){
    curByte = na_NextJSONByte(parser);

    while(curByte > '\0' && isdigit(curByte)){
      decimals = decimals * 10 + curByte - '0';
      decimalShift++;
      curByte = na_NextJSONByte(parser);
    }
  }

  if(curByte == 'e' || curByte == 'E'){
    curByte = na_NextJSONByte(parser);
    
    if(curByte == '+'){
      curByte = na_NextJSONByte(parser);
    }else if(curByte == '-'){
      exponentSign = -1;
      curByte = na_NextJSONByte(parser);
    }

    while(curByte > '\0' && isdigit(curByte)){
      exponent  = exponent * 10 + curByte - '0';
      curByte = na_NextJSONByte(parser);
    }
  }

  decimals = decimals * decimalSign;
  exponent = exponentSign * (exponent - decimalShift);
  parser->number = (double)(decimals) * naExp10((double)(exponent));
  
  parser->parseStatus = NA_JSON_PARSE_NUMBER;
  return parser->parseStatus;
}

NA_HDEF NA_JSONParseStatus na_ParseJSONIdentifier(NAJSONParser* parser){
  const NAByte* firstByte = parser->curPtr;
  NAByte curByte = na_NextJSONByte(parser);
  while (curByte > '\0' && isalpha(curByte)){
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
      curByte = na_NextJSONByte(parser);
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
    
    }else if(isalpha(curByte)){
      parser->parseStatus = na_ParseJSONIdentifier(parser);
      ns_JSONAdjustStackStatusAfterValueRead(parser);
      return parser->parseStatus;

    }else if(isdigit(curByte) || curByte == '+' || curByte == '-' || curByte == '.'){
      parser->parseStatus = na_ParseJSONNumber(parser);
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
      if(naGetStackCount(&parser->stackStatus) == 1){
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
      if(parser->parseStatus == NA_JSON_PARSE_UNEXPECTED_ARRAY_END)
        #if NA_DEBUG
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
      if(naGetStackCount(&parser->stackStatus) == 1){
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
      if(parser->parseStatus == NA_JSON_PARSE_UNEXPECTED_OBJECT_END)
        #if NA_DEBUG
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
        naParseJSONObject(parser, NA_NULL);
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
}

// Prototype:
NA_HAPI void na_ParseJSONArray(NAJSONParser* parser, const NAJSONRule* elementRule);

NA_HDEF void na_ParseJSONDynamicArray(NAJSONParser* parser, const NA_JSONArrayRule* elementRule){
  NABool unexpectedEnd = NA_FALSE;
  void* prevObject = parser->object;
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
        parser->object = *curObjectPtrPtr;
      }else{
        void* curObjectPtr = naPushStack(&elementStack);
        parser->object = curObjectPtr;
      }
      na_ParseJSONArray(parser, elementRule->subRule);
      parser->object = prevObject;
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
      if(elementRule->storeAsPointer){
        void** curObjectPtrPtr = naPushStack(&elementStack);
        *curObjectPtrPtr = naMalloc(elementRule->structSize);
        parser->object = *curObjectPtrPtr;
      }else{
        void* curObjectPtr = naPushStack(&elementStack);
        parser->object = curObjectPtr;
      }
      naParseJSONObject(parser, subObjectRule->ruleSet);
      parser->object = prevObject;
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
  *((size_t*)(parser->object + elementRule->countOffset)) = elementCount;
  
  if(elementCount){
    void** array = (void**)(parser->object + elementRule->arrayOffset);
    if(elementRule->storeAsPointer){
      *array = naMalloc(sizeof(void*) * elementCount);
    }else{
      *array = naMalloc(elementRule->structSize * elementCount);
    }
    naDumpStack(&elementStack, *array);
  }else{
    *((void**)(parser->object + elementRule->arrayOffset)) = NA_NULL;
  }
  
  naClearStack(&elementStack);
}

#if NA_DEBUG
  #define na_incArrayCount()\
    count++;\
    if(count > elementRule->elementCount)\
      naError("Array overflow");
#else
  #define na_incArrayCount()
#endif

NA_HDEF void na_ParseJSONFixedArray(NAJSONParser* parser, const NA_JSONFixedArrayRule* elementRule){
  NABool unexpectedEnd = NA_FALSE;
  void* prevObject = parser->object;
  parser->object = parser->object + elementRule->arrayOffset;
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
        *(int32*)parser->object = (int32)parser->boolean;
        break;
      case NA_JSON_RULE_INT64:
        na_incArrayCount();
        *(int64*)parser->object = (int64)parser->boolean;
        break;
      default:
        break;
      }
      parser->object = parser->object + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_NUMBER:
      switch(elementRule->contentType){
      case NA_JSON_RULE_INT32:
        na_incArrayCount();
        *(int32*)parser->object = (int32)parser->number;
        break;
      case NA_JSON_RULE_INT64:
        na_incArrayCount();
        *(int64*)parser->object = (int64)parser->number;
        break;
      case NA_JSON_RULE_DOUBLE:
        na_incArrayCount();
        *(double*)parser->object = parser->number;
        break;
      case NA_JSON_RULE_FLOAT:
        na_incArrayCount();
        *(float*)parser->object = (float)parser->number;
        break;
      default:
        break;
      }
      parser->object = parser->object + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_STRING:
      if(elementRule->contentType == NA_JSON_RULE_STRING){
        na_incArrayCount();
        na_StoreJSONString(parser, (NAUTF8Char**)parser->object);
      }
      parser->object = parser->object + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      na_incArrayCount();
      na_ParseJSONArray(parser, elementRule->subRule);
      parser->object = parser->object + elementRule->structSize;
      break;
      
    case NA_JSON_PARSE_OBJECT_START:
      na_incArrayCount();
      naParseJSONObject(parser, subObjectRule->ruleSet);
      parser->object = parser->object + elementRule->structSize;
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
  
  parser->object = prevObject;
}

NA_HDEF void na_ParseJSONArray(NAJSONParser* parser, const NAJSONRule* elementRule){
  if(!elementRule){
    // In case no rule is given, we just read the contents without storing
    // anything.
    na_ParseJSONDummyArray(parser);
  }else if(elementRule->type == NA_JSON_RULE_ARRAY){
    na_ParseJSONDynamicArray(parser, (NA_JSONArrayRule*)elementRule);
  }else if(elementRule->type == NA_JSON_RULE_FIXED_ARRAY){
    na_ParseJSONFixedArray(parser, (NA_JSONFixedArrayRule*)elementRule);
  }
}



// ///////////////////
// Parsing Objects

NA_HDEF void naParseJSONObject(NAJSONParser* parser, const NAJSONRuleSet* ruleSet){
  NABool unexpectedEnd = NA_FALSE;
  void* prevObject = parser->object;
  const NAJSONRuleSet* subRuleSet = NA_NULL;

  while(!unexpectedEnd && na_ParseJSONPrimitives(parser) != NA_JSON_PARSE_OBJECT_END){
    const NAJSONRule* rule = NA_NULL;
        
    switch(parser->parseStatus){
    case NA_JSON_PARSE_NULL:
      #if NA_DEBUG
        if(  (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &parser->key))
          || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &parser->key))
          || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_DOUBLE, &parser->key))
          || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FLOAT, &parser->key))
          || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_STRING, &parser->key))
          || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_ARRAY, &parser->key))
          || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FIXED_ARRAY, &parser->key)))
        {
          naError("Read object is null but type does not allow this. Member stays uninitialized.");
        }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_OBJECT, &parser->key))){
          naError("Read object is null but is not stored as a pointer. Members stay uninitialized.");
        }else
      #endif
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_POINTER_OBJECT, &parser->key))){
        const NA_JSONPointerObjectRule* pointerObjectRule = (const NA_JSONPointerObjectRule*)rule;
        *((void**)(parser->object + pointerObjectRule->memberOffset)) = NA_NULL;
      }
      break;

    case NA_JSON_PARSE_BOOLEAN:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int32*)(parser->object + memberRule->memberOffset)) = parser->boolean;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int64*)(parser->object + memberRule->memberOffset)) = parser->boolean;
      }
      break;

    case NA_JSON_PARSE_NUMBER:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT32, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int32*)(parser->object + memberRule->memberOffset)) = (int32)parser->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_INT64, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((int64*)(parser->object + memberRule->memberOffset)) = (int64)parser->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_DOUBLE, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((double*)(parser->object + memberRule->memberOffset)) = parser->number;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FLOAT, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        *((float*)(parser->object + memberRule->memberOffset)) = (float)parser->number;
      }
      break;

    case NA_JSON_PARSE_STRING:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_STRING, &parser->key))){
        const NA_JSONMemberRule* memberRule = (const NA_JSONMemberRule*)rule;
        na_StoreJSONString(parser, (parser->object + memberRule->memberOffset));
      }
      break;
      
    case NA_JSON_PARSE_ARRAY_START:
      if(  (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_ARRAY, &parser->key))
        || (rule = na_findJSONRule(ruleSet, NA_JSON_RULE_FIXED_ARRAY, &parser->key))){
        na_ParseJSONArray(parser, rule);
      }
      break;

    case NA_JSON_PARSE_OBJECT_START:
      if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_OBJECT, &parser->key))){
        const NA_JSONObjectRule* objectRule = (const NA_JSONObjectRule*)rule;
        subRuleSet = objectRule->ruleSet;
      }else if((rule = na_findJSONRule(ruleSet, NA_JSON_RULE_POINTER_OBJECT, &parser->key))){
        const NA_JSONPointerObjectRule* pointerObjectRule = (const NA_JSONPointerObjectRule*)rule;
        void* subObject = naMalloc(pointerObjectRule->structSize);
        *((void**)(parser->object + pointerObjectRule->memberOffset)) = subObject;
        parser->object = subObject;
        subRuleSet = pointerObjectRule->ruleSet;
      }
      naParseJSONObject(parser, subRuleSet);
      parser->object = prevObject;
      break;
      
    case NA_JSON_PARSE_BUFFER_END:
      unexpectedEnd = NA_TRUE;
      break;
    
    default:
      break;
    }
  }
}



