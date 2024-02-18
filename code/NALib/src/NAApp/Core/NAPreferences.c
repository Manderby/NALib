
#include "../NAPreferences.h"


NA_DEF NABool naInitPreferencesBool(const char* key, NABool newValue){
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesBool(prefs, key);
  if(value == NA_ZERO_i64){
    value = na_ConvertNABoolToPreferencesBool(newValue);
    na_SetRawPreferencesBool(prefs, key, value);
  }
  return na_ConvertPreferencesBoolToNABool(value);
}

NA_DEF NABool naGetPreferencesBool(const char* key){
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesBool(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesBoolToNABool(value);
}

NA_DEF NABool naSetPreferencesBool(const char* key, NABool newValue){
  NAi64 value = na_ConvertNABoolToPreferencesBool(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue = na_GetRawPreferencesBool(prefs, key);
  if(existingValue == NA_ZERO_i64 || value != existingValue){
    na_SetRawPreferencesBool(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}

NA_DEF NABool naTogglePreferencesBool(const char* key){
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesBool(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  NABool boolValue = na_ConvertPreferencesBoolToNABool(value);
  value = na_ConvertNABoolToPreferencesBool(!boolValue);
  na_SetRawPreferencesBool(prefs, key, value);
  return na_ConvertPreferencesBoolToNABool(value);
}



NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt newValue){
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesInt(prefs, key);
  if(value == NA_ZERO_i64){
    value = na_ConvertNAIntToPreferencesInt(newValue);
    na_SetRawPreferencesInt(prefs, key, value);
  }
  return na_ConvertPreferencesIntToNAInt(value);
}

NA_DEF NAInt naGetPreferencesInt(const char* key){
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesInt(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesIntToNAInt(value);
}

NA_DEF NABool naSetPreferencesInt(const char* key, NAInt newValue){
  NAi64 value = na_ConvertNAIntToPreferencesInt(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue = na_GetRawPreferencesInt(prefs, key);
  if(existingValue == NA_ZERO_i64 || value != existingValue){
    na_SetRawPreferencesInt(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt newValue){
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesEnum(prefs, key);
  if(value == NA_ZERO_i64){
    value = na_ConvertNAEnumToPreferencesEnum(newValue);
    na_SetRawPreferencesEnum(prefs, key, value);
  }
  return na_ConvertPreferencesEnumToNAEnum(value);
}

NA_DEF NAInt naGetPreferencesEnum(const char* key){
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesEnum(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesEnumToNAEnum(value);
}

NA_DEF NABool naSetPreferencesEnum(const char* key, NAInt newValue){
  NAi64 value = na_ConvertNAEnumToPreferencesEnum(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue = na_GetRawPreferencesEnum(prefs, key);
  if(existingValue == NA_ZERO_i64 || value != existingValue){
    na_SetRawPreferencesEnum(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF double naInitPreferencesDouble(const char* key, double newValue){
  void* prefs = na_GetNativePreferences();
  double value = na_GetRawPreferencesDouble(prefs, key);
  if(value == NA_ZERO_i64){
    value = na_ConvertNADoubleToPreferencesDouble(newValue);
    na_SetRawPreferencesDouble(prefs, key, value);
  }
  return na_ConvertPreferencesDoubleToNADouble(value);
}

NA_DEF double naGetPreferencesDouble(const char* key){
  void* prefs = na_GetNativePreferences();
  double value = na_GetRawPreferencesDouble(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesDoubleToNADouble(value);
}

NA_DEF NABool naSetPreferencesDouble(const char* key, double newValue){
  double value = na_ConvertNADoubleToPreferencesDouble(newValue);
  void* prefs = na_GetNativePreferences();
  double existingValue = na_GetRawPreferencesDouble(prefs, key);
  if(existingValue == 0. || value != existingValue){
    na_SetRawPreferencesDouble(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF NAString* naInitPreferencesString(const char* key, NAString* newValue){
  void* prefs = na_GetNativePreferences();
  NAString* value = na_GetRawPreferencesString(prefs, key);
  if(value == NA_NULL){
    value = na_ConvertNAStringToPreferencesString(newValue);
    na_SetRawPreferencesString(prefs, key, value);
  }
  return value;
}

NA_DEF NAString* naNewPreferencesString(const char* key){
  void* prefs = na_GetNativePreferences();
  NAString* value = na_GetRawPreferencesString(prefs, key);
  #if NA_DEBUG
    if(value == NA_NULL)
      naError("Preferences value not initialized.");
  #endif
  NAString* returnValue = na_ConvertPreferencesStringToNAString(value);
  if(value){naDelete(value);}
  return returnValue;
}

NA_DEF NABool naSetPreferencesString(const char* key, NAString* newValue){
  NAString* value = na_ConvertNAStringToPreferencesString(newValue);
  void* prefs = na_GetNativePreferences();
  NAString* existingValue = na_GetRawPreferencesString(prefs, key);
  NABool isDifferent = existingValue == NA_NULL || !naEqualStringToString(value, existingValue, NA_TRUE);
  if(isDifferent){
    na_SetRawPreferencesString(prefs, key, value);
  }
  if(value){naDelete(value);}
  if(existingValue){naDelete(existingValue);}
  return isDifferent;
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
