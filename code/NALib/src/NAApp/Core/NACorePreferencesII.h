
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
NA_HIDEF NAi64 na_ConvertNABoolToPreferencesBool(NABool value) {
  #if NA_DEBUG
  if(!naEquali64(value, NA_ZERO_i64) && !naEquali64(value, NA_ONE_i64))
    naError("Trying to store invalid boolean value in preferences.");
  #endif
  return (value) ? NA_ONE_i64 : naMakei64WithLo(-1);
}

NA_HIDEF NABool na_ConvertPreferencesBoolToNABool(NAi64 value) {
  #if NA_DEBUG
  if(naEquali64(value, NA_ZERO_i64))
    naError("Value stored in preferences invalid or uninitialized.");
  if(!naEquali64(value, NA_ONE_i64) && !naEquali64(value, naMakei64WithLo(-1)))
    naError("Invalid boolean value stored in preferences.");
  #endif
  return ((naEquali64(value, NA_ONE_i64)) ? NA_TRUE : NA_FALSE);
}

NA_HIDEF NABool na_IsPreferencesBoolValid(NABool value) {
  return value == NA_TRUE || value == NA_FALSE;
}

NA_DEF NABool naInitPreferencesBool(const char* key, NABool newValue) {
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesBool(prefs, key);
  
  if(value != NA_ZERO_i64) {
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
  NAi64 value = na_GetRawPreferencesBool(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesBoolToNABool(value);
}

NA_DEF NABool naSetPreferencesBool(const char* key, NABool newValue) {
  NAi64 value = na_ConvertNABoolToPreferencesBool(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue = na_GetRawPreferencesBool(prefs, key);
  if(existingValue == NA_ZERO_i64 || value != existingValue) {
    na_SetRawPreferencesBool(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}

NA_DEF NABool naTogglePreferencesBool(const char* key) {
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



// ////////////////////////
// NAInt
// ////////////////////////

// Int values are stored as is but the value 0 is stored as min_i64
NA_HIDEF NAi64 na_ConvertNAIntToPreferencesInt(NAInt value) {
  #if NA_DEBUG
  if(naEquali64(value, NA_MIN_i64))
    naError("min_i64 is not a valid value which can be stored in preferences.");
  #endif
  return (value == 0) ? NA_MIN_i64 : naCastIntToi64(value);
}

NA_HIDEF NAInt na_ConvertPreferencesIntToNAInt(NAi64 value) {
  #if NA_DEBUG
  if(naEquali64(value, NA_ZERO_i64))
    naError("Value stored in preferences invalid or uninitialized.");
  #endif
  return (NAInt)((naEquali64(value, NA_MIN_i64)) ? 0 : naCasti64ToInt(value));
}

NA_HIDEF NABool na_IsPreferencesNAIntValid(NAi64 value, NAi64 min, NAi64 max) {
  return value >= min && value <= max;
}

NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt newValue, NAInt min, NAInt max) {
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesInt(prefs, key);
  
  if(value != NA_ZERO_i64) {
    NAInt retValue = na_ConvertPreferencesIntToNAInt(value);
    if(na_IsPreferencesNAIntValid(retValue, min, max)) {
      return retValue;
    }
  }

  na_SetRawPreferencesInt(prefs, key, na_ConvertNAIntToPreferencesInt(newValue));
  return newValue;
}

NA_DEF NAInt naGetPreferencesInt(const char* key) {
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesInt(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesIntToNAInt(value);
}

NA_DEF NABool naSetPreferencesInt(const char* key, NAInt newValue) {
  NAi64 value = na_ConvertNAIntToPreferencesInt(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue = na_GetRawPreferencesInt(prefs, key);
  if(existingValue == NA_ZERO_i64 || value != existingValue) {
    na_SetRawPreferencesInt(prefs, key, value);
    return NA_TRUE;
  }
  return NA_FALSE;
}



// ////////////////////////
// Enum
// ////////////////////////

// Enum values are stored with their value increased by 1
NA_HIDEF NAi64 na_ConvertNAEnumToPreferencesEnum(NAInt value) {
  #if NA_DEBUG
  if(naSmalleri64(value, 0))
    naError("negative enum values can not be stored in preferences.");
  #endif
  return naAddi64(naCastIntToi64(value), NA_ONE_i64);
}

NA_HIDEF NAInt na_ConvertPreferencesEnumToNAEnum(NAi64 value) {
  #if NA_DEBUG
  if(naEquali64(value, NA_ZERO_i64))
    naError("Value stored in preferences invalid or uninitialized.");
  #endif
  return naCasti64ToInt(naSubi64(value, NA_ONE_i64));
}

NA_HIDEF NABool na_IsPreferencesEnumValid(NAi64 value, NAi64 count) {
  return value >= 0 && value < count;
}

NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt newValue, NAInt count) {
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesEnum(prefs, key);
  
  if(value != NA_ZERO_i64) {
    NAInt retValue = na_ConvertPreferencesEnumToNAEnum(value);
    if(na_IsPreferencesEnumValid(retValue, count)) {
      return retValue;
    }
  }

  na_SetRawPreferencesEnum(prefs, key, na_ConvertNAEnumToPreferencesEnum(newValue));
  return newValue;
}

NA_DEF NAInt naGetPreferencesEnum(const char* key) {
  void* prefs = na_GetNativePreferences();
  NAi64 value = na_GetRawPreferencesEnum(prefs, key);
  #if NA_DEBUG
    if(value == NA_ZERO_i64)
      naError("Preferences value not initialized.");
  #endif
  return na_ConvertPreferencesEnumToNAEnum(value);
}

NA_DEF NABool naSetPreferencesEnum(const char* key, NAInt newValue) {
  NAi64 value = na_ConvertNAEnumToPreferencesEnum(newValue);
  void* prefs = na_GetNativePreferences();
  NAi64 existingValue = na_GetRawPreferencesEnum(prefs, key);
  if(existingValue == NA_ZERO_i64 || value != existingValue) {
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
    if(value == NA_ZERO_i64)
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
  return naNewStringExtraction(value, 0, -1);
}
NA_HIDEF NAString* na_ConvertPreferencesStringToNAString(NAString* value) {
  #if NA_DEBUG
  if(value == NA_NULL)
    naError("Value stored in preferences invalid or uninitialized.");
  #endif
  return naNewStringExtraction(value, 0, -1);
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
