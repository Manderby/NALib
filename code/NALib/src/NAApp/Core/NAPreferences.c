
#include "../NAPreferences.h"


NA_DEF NABool naInitPreferencesBool(const char* key, NABool newValue){
  NAi64 value;
  void* prefs = na_GetNativePreferences();
  if(!na_GetRawPreferencesBool(prefs, key, &value)){
    value = na_ConvertNABoolToPreferencesBool(newValue);
    if(!na_SetRawPreferencesBool(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not init bool preference");
      #endif
    }
  }
  return na_ConvertPreferencesBoolToNABool(value);
}

NA_DEF NABool naGetPreferencesBool(const char* key){
  NAi64 value;
  void* prefs = na_GetNativePreferences();
  return na_GetRawPreferencesBool(prefs, key, &value)
    ? na_ConvertPreferencesBoolToNABool(value)
    : NA_FALSE;
}

NA_DEF NABool naSetPreferencesBool(const char* key, NABool newValue){
  NAi64 value = na_ConvertNABoolToPreferencesBool(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue;
  NABool isDifferent = na_GetRawPreferencesBool(prefs, key, &existingValue)
    ? value != existingValue
    : NA_TRUE;

  if(isDifferent){
    if(!na_SetRawPreferencesBool(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not store bool preference.");
      #endif
    }
  }

  return isDifferent;
}

NA_DEF NABool naTogglePreferencesBool(const char* key){
  void* prefs = na_GetNativePreferences();
  NAi64 value;
  na_GetRawPreferencesBool(prefs, key, &value);
  NABool boolValue = na_ConvertPreferencesBoolToNABool(value);
  value = na_ConvertNABoolToPreferencesBool(!boolValue);
  if(!na_SetRawPreferencesBool(prefs, key, &value)){
    #if NA_DEBUG
      naError("Could not toggle bool preference.");
    #endif
  }
  return na_ConvertPreferencesBoolToNABool(value);
}



NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt newValue){
  NAi64 value;
  void* prefs = na_GetNativePreferences();
  if(!na_GetRawPreferencesInt(prefs, key, &value)){
    value = na_ConvertNAIntToPreferencesInt(newValue);
    if(!na_SetRawPreferencesInt(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not init int preference");
      #endif
    }
  }
  return na_ConvertPreferencesIntToNAInt(value);
}

NA_DEF NAInt naGetPreferencesInt(const char* key){
  NAi64 value;
  void* prefs = na_GetNativePreferences();
  return na_GetRawPreferencesInt(prefs, key, &value)
    ? na_ConvertPreferencesIntToNAInt(value)
    : NA_MIN_i64;
}

NA_DEF NABool naSetPreferencesInt(const char* key, NAInt newValue){
  NAi64 value = na_ConvertNAIntToPreferencesInt(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue;
  NABool isDifferent = na_GetRawPreferencesInt(prefs, key, &existingValue)
    ? value != existingValue
    : NA_TRUE;

  if(isDifferent){
    if(!na_SetRawPreferencesInt(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not store int preference.");
      #endif
    }
  }

  return isDifferent;
}



NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt newValue){
  NAi64 value;
  void* prefs = na_GetNativePreferences();
  if(!na_GetRawPreferencesEnum(prefs, key, &value)){
    value = na_ConvertNAEnumToPreferencesEnum(newValue);
    if(!na_SetRawPreferencesEnum(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not init enum preference");
      #endif
    }
  }
  return na_ConvertPreferencesEnumToNAEnum(value);
}

NA_DEF NAInt naGetPreferencesEnum(const char* key){
  NAi64 value;
  void* prefs = na_GetNativePreferences();
  return na_GetRawPreferencesEnum(prefs, key, &value)
    ? na_ConvertPreferencesEnumToNAEnum(value)
    : NA_MIN_i64;
}

NA_DEF NABool naSetPreferencesEnum(const char* key, NAInt newValue){
  NAi64 value = na_ConvertNAEnumToPreferencesEnum(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue;
  NABool isDifferent = na_GetRawPreferencesEnum(prefs, key, &existingValue)
    ? value != existingValue
    : NA_TRUE;

  if(isDifferent){
    if(!na_SetRawPreferencesEnum(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not store enum preference.");
      #endif
    }
  }

  return isDifferent;
}



NA_DEF double naInitPreferencesDouble(const char* key, double newValue){
  double value;
  void* prefs = na_GetNativePreferences();
  if(!na_GetRawPreferencesDouble(prefs, key, &value)){
    value = na_ConvertNADoubleToPreferencesDouble(newValue);
    if(!na_SetRawPreferencesDouble(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not init double preference");
      #endif
    }
  }
  return na_ConvertPreferencesDoubleToNADouble(value);
}

NA_DEF double naGetPreferencesDouble(const char* key){
  double value;
  void* prefs = na_GetNativePreferences();
  return na_GetRawPreferencesDouble(prefs, key, &value)
    ? na_ConvertPreferencesDoubleToNADouble(value)
    : NA_MIN_i64;
}

NA_DEF NABool naSetPreferencesDouble(const char* key, double newValue){
  double value = na_ConvertNADoubleToPreferencesDouble(newValue);
  void* prefs = na_GetNativePreferences();
  double existingValue;
  NABool isDifferent = na_GetRawPreferencesDouble(prefs, key, &existingValue)
    ? value != existingValue
    : NA_TRUE;

  if(isDifferent){
    if(!na_SetRawPreferencesDouble(prefs, key, &value)){
      #if NA_DEBUG
        naError("Could not store double preference.");
      #endif
    }
  }

  return isDifferent;
}



NA_DEF NAString* naInitPreferencesString(const char* key, NAString* newValue){
  NAString* value;
  void* prefs = na_GetNativePreferences();
  if(!na_GetRawPreferencesString(prefs, key, &value)){
    value = naNewStringExtraction(newValue, 0, -1);
    if(!na_SetRawPreferencesString(prefs, key, &value)){
      #if NA_DEBUG
      naError("Could not init string preference");
      #endif
    }
  }
  return value;
}

NA_DEF NAString* naNewPreferencesString(const char* key){
  NAString* value;
  void* prefs = na_GetNativePreferences();
  return na_GetRawPreferencesString(prefs, key, &value)
    ? value
    : NA_NULL;
}

NA_DEF NABool naSetPreferencesString(const char* key, NAString* newValue){
  void* prefs = na_GetNativePreferences();
  NAString* existingValue;
  NABool isDifferent = na_GetRawPreferencesString(prefs, key, &existingValue)
    ? !naEqualStringToString(newValue, existingValue, NA_TRUE)
    : NA_TRUE;
  if(existingValue){naDelete(existingValue);}

  if(isDifferent){
    if(!na_SetRawPreferencesString(prefs, key, &newValue)){
      #if NA_DEBUG
      naError("Could not store string preference.");
      #endif
    }
  }

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
