
// This file contains inline implementations of the file NAPreferences
// Do not include this file directly! It will automatically be included when
// including "NAPreferences.h"



NA_HAPI void* na_GetNativePreferences(void);
NA_HAPI void na_ShutdownPreferences(void);

#if NA_OS == NA_OS_WINDOWS
  // Returns a value from the windows registry.
  // The "Variable" method will allocate sufficient memory with malloc for you
  // and return the size allocated in valueSize if available.
  // The "Fixed" method expects a pointer to an existing memory location and
  // the size in bytes available at that location. The requested value will
  // be stored at that location if and only if the value size is equal to the
  // one retrieved from the registry.
  // Both methods will return NA_NULL if something went wrong.
  NA_HDEF void* na_GetWINRegistryVariableEntry(
    HKEY rootKey,
    const NAUTF8Char* path,
    const NAUTF8Char* key,
    size_t* valueSize);

  NA_HDEF void* na_GetWINRegistryFixedEntry(
    HKEY rootKey,
    const NAUTF8Char* path,
    const NAUTF8Char* key,
    void* value,
    size_t valueSize);

  #endif

NA_HAPI NAi64     na_GetRawPreferencesBool  (void* prefs, const char* key);
NA_HAPI NAi64     na_GetRawPreferencesInt   (void* prefs, const char* key);
NA_HAPI NAi64     na_GetRawPreferencesEnum  (void* prefs, const char* key);
NA_HAPI double    na_GetRawPreferencesDouble(void* prefs, const char* key);
NA_HAPI NAString* na_GetRawPreferencesString(void* prefs, const char* key);

NA_HAPI void na_SetRawPreferencesBool  (void* prefs, const char* key, NAi64 valueStorage);
NA_HAPI void na_SetRawPreferencesInt   (void* prefs, const char* key, NAi64 valueStorage);
NA_HAPI void na_SetRawPreferencesEnum  (void* prefs, const char* key, NAi64 valueStorage);
NA_HAPI void na_SetRawPreferencesDouble(void* prefs, const char* key, double valueStorage);
NA_HAPI void na_SetRawPreferencesString(void* prefs, const char* key, NAString* valueStorage);



// Following are the conversion methods which convert between the values stored
// in the preferences and those desired during runtime. They convert the values
// such that the value 0 is not a valid entry and therefore can serve to detect
// whether an entry already exists in the preferences.

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
