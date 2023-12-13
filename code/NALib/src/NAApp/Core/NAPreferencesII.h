
// This file contains inline implementations of the file NAPreferences
// Do not include this file directly! It will automatically be included when
// including "NAPreferences.h"



NA_HAPI void* na_GetNativePreferences();
NA_HAPI NABool na_GetRawPreferencesBool(void* prefs, const char* key, NAi64* valueStorage);
NA_HAPI NABool na_SetRawPreferencesBool(void* prefs, const char* key, NAi64* valueStorage);
NA_HAPI NABool na_GetRawPreferencesInt(void* prefs, const char* key, NAi64* valueStorage);
NA_HAPI NABool na_SetRawPreferencesInt(void* prefs, const char* key, NAi64* valueStorage);
NA_HAPI NABool na_GetRawPreferencesEnum(void* prefs, const char* key, NAi64* valueStorage);
NA_HAPI NABool na_SetRawPreferencesEnum(void* prefs, const char* key, NAi64* valueStorage);
NA_HAPI NABool na_GetRawPreferencesDouble(void* prefs, const char* key, double* valueStorage);
NA_HAPI NABool na_SetRawPreferencesDouble(void* prefs, const char* key, double* valueStorage);
NA_HAPI NABool na_GetRawPreferencesString(void* prefs, const char* key, NAString** valueStorage);
NA_HAPI NABool na_SetRawPreferencesString(void* prefs, const char* key, NAString** valueStorage);



NA_HIDEF NABool na_ConvertPreferencesBoolToNABool(NAi64 value){
  return ((naEquali64(value, naMakei64WithLo(1))) ? NA_TRUE : NA_FALSE);
}

NA_HIDEF NAi64 na_ConvertNABoolToPreferencesBool(NABool value){
  return (value != 0) ? naMakei64WithLo(1) : naMakei64WithLo(-1);
}



NA_HIDEF NAInt na_ConvertPreferencesIntToNAInt(NAi64 value){
  return (NAInt)((naEquali64(value, NA_MIN_i64)) ? 0 : naCasti64ToInt(value));
}

NA_HIDEF NAi64 na_ConvertNAIntToPreferencesInt(NAInt value){
  return (value == 0) ? NA_MIN_i64 : naCastIntToi64(value);
}



NA_HIDEF NAInt na_ConvertPreferencesEnumToNAEnum(NAi64 value){
  return naCasti64ToInt(naSubi64(value, naMakei64WithLo(1)));
}

NA_HIDEF NAi64 na_ConvertNAEnumToPreferencesEnum(NAInt value){
  return naAddi64(naCastIntToi64(value), naMakei64WithLo(1));
}



NA_HIDEF double na_ConvertPreferencesDoubleToNADouble(double value){
  return naIsNaN(value) ? 0. : value;
}

NA_HIDEF double na_ConvertNADoubleToPreferencesDouble(double value){
  return (value == 0.) ? NA_NAN : value;
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
