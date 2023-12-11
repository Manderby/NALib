
// This file contains inline implementations of the file NAPreferences
// Do not include this file directly! It will automatically be included when
// including "NAPreferences.h"


#if NA_OS == NA_OS_WINDOWS
  #include "../winAPI/NAPreferencesWINAPIII.h"
#else if NA_OS == NA_OS_MAC_OS_X
  #include "../macOS/NAPreferencesCocoaII.h"
#endif



NA_HIDEF NABool na_ConvertPreferencesBoolToNABool(NAi64 value){
  return ((naEquali64(value, naMakei64WithLo(1))) ? NA_TRUE : NA_FALSE);
}

NA_HIDEF NAi64 na_ConvertNABoolToPreferencesBool(NABool value){
  return value ? naMakei64WithLo(1) : naMakei64WithLo(-1);
}

NA_DEF NABool naInitPreferencesBool(const char* key, NABool newValue){
  NAi64 value;
  NA_PreferencesPtr prefs = na_GetNativePreferences();
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
  NA_PreferencesPtr prefs = na_GetNativePreferences();
  return na_GetRawPreferencesBool(prefs, key, &value)
    ? na_ConvertPreferencesBoolToNABool(value)
    : NA_FALSE;
}

NA_DEF NABool naSetPreferencesBool(const char* key, NABool newValue){
  NAi64 value = na_ConvertNABoolToPreferencesBool(newValue);
  NA_PreferencesPtr prefs = na_GetNativePreferences();
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
  NA_PreferencesPtr prefs = na_GetNativePreferences();
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
