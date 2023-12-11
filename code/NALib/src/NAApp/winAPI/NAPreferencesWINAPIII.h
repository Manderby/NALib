
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!

#include "../NAPreferences.h"



NA_HIDEF NABool na_GetRawPreferencesBool(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS);
}

NA_HIDEF NABool na_SetRawPreferencesBool(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, valueStorage, valueSize);
  return (errorCode == ERROR_SUCCESS);
}



NA_HIDEF NABool na_GetRawPreferencesInt(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS);
}

NA_HIDEF NABool na_SetRawPreferencesInt(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, valueStorage, valueSize);
  return (errorCode == ERROR_SUCCESS);
}



NA_HIDEF NABool na_GetRawPreferencesEnum(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS);
}

NA_HIDEF NABool na_SetRawPreferencesEnum(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, valueStorage, valueSize);
  return (errorCode == ERROR_SUCCESS);
}



NA_HIDEF NABool na_GetRawPreferencesDouble(NA_PreferencesPtr prefs, const char* key, double* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS);
}

NA_HIDEF NABool na_SetRawPreferencesDouble(NA_PreferencesPtr prefs, const char* key, double* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, valueStorage, valueSize);
  return (errorCode == ERROR_SUCCESS);
}



NA_HIDEF NABool na_GetRawPreferencesString(NA_PreferencesPtr prefs, const char* key, NAString** valueStorage){
  wchar_t* systemKey = naAllocWideCharStringWithUTF8String(key);
  DWORD valueSize;
  DWORD type;
  LSTATUS errorCode = RegQueryValueExW(prefs, systemKey, NULL, &type, NULL, &valueSize);
  if(errorCode != ERROR_SUCCESS){
    naFree(systemKey);
    return NA_FALSE;
  }

  wchar_t* storedValue = naMalloc(valueSize);
  errorCode = RegGetValueW(prefs, NULL, systemKey, RRF_RT_ANY, &type, storedValue, (LPDWORD)&valueSize);
  naFree(systemKey);

  if(errorCode == ERROR_SUCCESS){
    *valueStorage = naNewStringFromWideCharString(storedValue);
    naFree(storedValue);
    return NA_TRUE;
  }else{
    *valueStorage = NA_NULL;
    return NA_FALSE;
  }
}

NA_HIDEF NABool na_SetRawPreferencesString(NA_PreferencesPtr prefs, const char* key, NAString** valueStorage){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  // Stupid dog. You make this look bad.

  wchar_t* systemKey = naAllocWideCharStringWithUTF8String(key);
  wchar_t* storedValue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(*valueStorage));
  DWORD valueSize = ((DWORD)wcslen(storedValue) + 1) * sizeof(wchar_t);
  LSTATUS errorCode = RegSetKeyValueW(prefs, NULL, systemKey, REG_SZ, storedValue, valueSize);
  naFree(storedValue);
  naFree(systemKey);
  return errorCode == ERROR_SUCCESS;
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
