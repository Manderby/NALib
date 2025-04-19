
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



// Each type defines helper conversion methods with NA_HIDEF which convert
// between the values stored in the preferences and those desired during
// runtime. They convert the values such that the value 0 is not a valid
// entry and therefore can serve to detect whether an entry already exists
// in the preferences.
//
// The public methods with NA_DEF are the actual getters and setters.



// ////////////////////////
// Bool
// ////////////////////////

// Boolean values are stored as 1 for TRUE and -1 for FALSE
NA_HIDEF int64 na_ConvertNABoolToPreferencesBool(NABool value) {
  #if NA_DEBUG
  if(!naEquali64(naCastBoolToi64(value), NA_ZERO_i64) && !naEquali64(naCastBoolToi64(value), NA_ONE_i64))
    naError("Trying to store invalid boolean value in preferences.");
  #endif
  return (value) ? NA_ONE_i64 : naCasti32Toi64(-1);
}

NA_HIDEF NABool na_ConvertPreferencesBoolToNABool(int64 value) {
  #if NA_DEBUG
  if(naEquali64(value, NA_ZERO_i64))
    naError("Value stored in preferences invalid or uninitialized.");
  if(!naEquali64(value, NA_ONE_i64) && !naEquali64(value, naCasti32Toi64(-1)))
    naError("Invalid boolean value stored in preferences.");
  #endif
  return ((naEquali64(value, NA_ONE_i64)) ? NA_TRUE : NA_FALSE);
}

NA_HIDEF NABool na_IsPreferencesBoolValid(NABool value) {
  return value == NA_TRUE || value == NA_FALSE;
}

NA_DEF NABool naInitPreferencesBool(const char* key, NABool newValue) {
  void* prefs = na_GetNativePreferences();
  int64 value = na_GetRawPreferencesBool(prefs, key);
  
  if(!naEquali64(value, NA_ZERO_i64)) {
    NABool retValue = na_ConvertPreferencesBoolToNABool(value);
    if(na_IsPreferencesBoolValid(retValue)) {
      return retValue;
    }
  }
  
  na_SetRawPreferencesBool(prefs, key, na_ConvertNABoolToPreferencesBool(newValue));
  return newValue;
}

NA_DEF NABool naGetPreferencesBool(const char* key) {
  void* prefs = na_GetNativePreferences();
  int64 value = na_GetRawPreferencesBool(prefs, key);
  #if NA_DEBUG
    if(naEquali64(value, NA_ZERO_i64))
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesBoolToNABool(value);
}

NA_DEF NABool naSetPreferencesBool(const char* key, NABool newValue) {
  int64 value = na_ConvertNABoolToPreferencesBool(newValue);
  void* prefs = na_GetNativePreferences();
  int64 existingValue = na_GetRawPreferencesBool(prefs, key);
  if(naEquali64(existingValue, NA_ZERO_i64) || !naEquali64(value, existingValue)) {
    na_SetRawPreferencesBool(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}

NA_DEF NABool naTogglePreferencesBool(const char* key) {
  void* prefs = na_GetNativePreferences();
  int64 value = na_GetRawPreferencesBool(prefs, key);
  #if NA_DEBUG
    if(naEquali64(value, NA_ZERO_i64))
      naError("Preferences value not initialized.");
  #endif
  NABool boolValue = na_ConvertPreferencesBoolToNABool(value);
  value = na_ConvertNABoolToPreferencesBool(!boolValue);
  na_SetRawPreferencesBool(prefs, key, value);
  return na_ConvertPreferencesBoolToNABool(value);
}



// ////////////////////////
// int64
// ////////////////////////

// Int values are stored as is but the value 0 is stored as min_i64
NA_HIDEF int64 na_Converti64ToPreferencesInt(int64 value) {
  #if NA_DEBUG
  if(naEquali64(value, NA_MIN_i64))
    naError("min_i64 is not a valid value which can be stored in preferences.");
  #endif
  return naEquali64(value, NA_ZERO_i64) ? NA_MIN_i64 : value;
}

NA_HIDEF int64 na_ConvertPreferencesIntToi64(int64 value) {
  #if NA_DEBUG
  if(naEquali64(value, NA_ZERO_i64))
    naError("Value stored in preferences invalid or uninitialized.");
  #endif
  return naEquali64(value, NA_MIN_i64) ? NA_ZERO_i64 : value;
}

NA_HIDEF NABool na_IsPreferencesi64Valid(int64 value, int64 min, int64 max) {
  return naGreaterEquali64(value, min) && naSmallerEquali64(value, max);
}

NA_DEF int64 naInitPreferencesi64(const char* key, int64 newValue, int64 min, int64 max) {
  void* prefs = na_GetNativePreferences();
  int64 value = na_GetRawPreferencesi64(prefs, key);
  
  if(!naEquali64(value, NA_ZERO_i64)) {
    int64 retValue = na_ConvertPreferencesIntToi64(value);
    if(na_IsPreferencesi64Valid(retValue, min, max)) {
      return retValue;
    }
  }

  na_SetRawPreferencesi64(prefs, key, na_Converti64ToPreferencesInt(newValue));
  return newValue;
}

NA_DEF int64 naGetPreferencesi64(const char* key) {
  void* prefs = na_GetNativePreferences();
  int64 value = na_GetRawPreferencesi64(prefs, key);
  #if NA_DEBUG
    if(naEquali64(value, NA_ZERO_i64))
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesIntToi64(value);
}

NA_DEF NABool naSetPreferencesi64(const char* key, int64 newValue) {
  int64 value = na_Converti64ToPreferencesInt(newValue);
  void* prefs = na_GetNativePreferences();
  int64 existingValue = na_GetRawPreferencesi64(prefs, key);
  if(naEquali64(existingValue, NA_ZERO_i64) || !naEquali64(value, existingValue)) {
    na_SetRawPreferencesi64(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}



// ////////////////////////
// Enum
// ////////////////////////

// Enum values are stored with their value increased by 1
NA_HIDEF int64 na_ConvertNAEnumToPreferencesEnum(uint32 value) {
  return naAddi64(naCastu32Toi64(value), NA_ONE_i64);
}

NA_HIDEF uint32 na_ConvertPreferencesEnumToNAEnum(int64 value) {
  #if NA_DEBUG
    if(naEquali64(value, NA_ZERO_i64))
      naError("Value stored in preferences invalid or uninitialized.");
  #endif
  return naCasti64Tou32(naSubi64(value, NA_ONE_i64));
}

NA_HIDEF NABool na_IsPreferencesEnumValid(int64 value, int64 count) {
  return naGreaterEquali64(value, NA_ZERO_i64) && naSmalleri64(value, count);
}

NA_DEF uint32 naInitPreferencesEnum(const char* key, uint32 newValue, uint32 count) {
  void* prefs = na_GetNativePreferences();
  int64 value = na_GetRawPreferencesEnum(prefs, key);
  
  if(!naEquali64(value, NA_ZERO_i64)) {
    uint32 retValue = na_ConvertPreferencesEnumToNAEnum(value);
    if(na_IsPreferencesEnumValid(naCastu32Toi64(retValue), naCastu32Toi64(count))) {
      return retValue;
    }
  }

  na_SetRawPreferencesEnum(prefs, key, na_ConvertNAEnumToPreferencesEnum(newValue));
  return newValue;
}

NA_DEF uint32 naGetPreferencesEnum(const char* key) {
  void* prefs = na_GetNativePreferences();
  int64 value = na_GetRawPreferencesEnum(prefs, key);
  #if NA_DEBUG
    if(naEquali64(value, NA_ZERO_i64))
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesEnumToNAEnum(value);
}

NA_DEF NABool naSetPreferencesEnum(const char* key, uint32 newValue) {
  int64 value = na_ConvertNAEnumToPreferencesEnum(newValue);
  void* prefs = na_GetNativePreferences();
  int64 existingValue = na_GetRawPreferencesEnum(prefs, key);
  if(naEquali64(existingValue, NA_ZERO_i64) || !naEquali64(value, existingValue)) {
    na_SetRawPreferencesEnum(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}



// ////////////////////////
// Double
// ////////////////////////

// double values are stored as is but zero is converted to NaN.
NA_HIDEF double na_ConvertNADoubleToPreferencesDouble(double value) {
  #if NA_DEBUG
  if(naIsNaN(value))
    naError("The value NaN can not be stored in preferences.");
  #endif
  return (value == 0.) ? NA_NAN : value;
}

NA_HIDEF double na_ConvertPreferencesDoubleToNADouble(double value) {
  #if NA_DEBUG
  if(value == 0.)
    naError("Value stored in preferences invalid or uninitialized.");
  #endif
  return naIsNaN(value) ? 0. : value;
}

NA_HIDEF NABool na_IsPreferencesDoubleValid(double value, double min, double max) {
  return value >= min && value <= max;
}

NA_DEF double naInitPreferencesDouble(const char* key, double newValue, double min, double max) {
  void* prefs = na_GetNativePreferences();
  double value = na_GetRawPreferencesDouble(prefs, key);
  
  if(value != 0.) {
    double retValue = na_ConvertPreferencesDoubleToNADouble(value);
    if(na_IsPreferencesDoubleValid(retValue, min, max)) {
      return retValue;
    }
  }
  
  na_SetRawPreferencesDouble(prefs, key, na_ConvertNADoubleToPreferencesDouble(newValue));
  return newValue;
}

NA_DEF double naGetPreferencesDouble(const char* key) {
  void* prefs = na_GetNativePreferences();
  double value = na_GetRawPreferencesDouble(prefs, key);
  #if NA_DEBUG
    if(value == 0.)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesDoubleToNADouble(value);
}

NA_DEF NABool naSetPreferencesDouble(const char* key, double newValue) {
  double value = na_ConvertNADoubleToPreferencesDouble(newValue);
  void* prefs = na_GetNativePreferences();
  double existingValue = na_GetRawPreferencesDouble(prefs, key);
  if(existingValue == 0. || value != existingValue) {
    na_SetRawPreferencesDouble(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}



// ////////////////////////
// String
// ////////////////////////

// string values are returned as a copy and are checked for NULL.
NA_HIDEF NAString* na_ConvertNAStringToPreferencesString(NAString* value) {
  #if NA_DEBUG
  if(value == NA_NULL)
    naError("A null string can not be stored in preferences.");
  #endif
  return naNewStringExtraction(value, NA_ZERO_i64, NA_MINUS_ONE_i64);
}
NA_HIDEF NAString* na_ConvertPreferencesStringToNAString(NAString* value) {
  #if NA_DEBUG
  if(value == NA_NULL)
    naError("Value stored in preferences invalid or uninitialized.");
  #endif
  return naNewStringExtraction(value, NA_ZERO_i64, NA_MINUS_ONE_i64);
}

NA_DEF NAString* naInitPreferencesString(const char* key, NAString* newValue) {
  void* prefs = na_GetNativePreferences();
  NAString* value = na_GetRawPreferencesString(prefs, key);
  
  if(value == NA_NULL) {
    value = na_ConvertNAStringToPreferencesString(newValue);
    na_SetRawPreferencesString(prefs, key, value);
  }
  
  return value;
}

NA_DEF NAString* naNewPreferencesString(const char* key) {
  void* prefs = na_GetNativePreferences();
  NAString* value = na_GetRawPreferencesString(prefs, key);
  #if NA_DEBUG
    if(value == NA_NULL)
      naError("Preferences value not initialized.");
  #endif
  NAString* returnValue = na_ConvertPreferencesStringToNAString(value);
  
  if(value)
    naDelete(value);
  
  return returnValue;
}

NA_DEF NABool naSetPreferencesString(const char* key, NAString* newValue) {
  NAString* value = na_ConvertNAStringToPreferencesString(newValue);
  void* prefs = na_GetNativePreferences();
  NAString* existingValue = na_GetRawPreferencesString(prefs, key);
  NABool isDifferent = existingValue == NA_NULL || !naEqualStringToString(value, existingValue, NA_TRUE);
  if(isDifferent) {
    na_SetRawPreferencesString(prefs, key, value);
  }
  
  if(value)
    naDelete(value);
  if(existingValue)
    naDelete(existingValue);
    
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
