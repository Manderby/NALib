
#include "../NAPreferences.h"
#include "../../NAApp/NAApp.h"

#if NA_OS == NA_OS_WINDOWS
#if NA_COMPILE_GUI == 1


HKEY na_nativePreferences = NA_NULL;



NA_HDEF void* na_GetNativePreferences(){
  #if NA_DEBUG
    if(!naGetApplication())
      naError("No application running. Use naStartApplication.");
  #endif
  if(!na_nativePreferences){
    NAString* appname = naNewApplicationName();
    NAString* companyname = naNewApplicationCompanyName();

    NAString* fullKeyName;
    if(companyname){
      fullKeyName = naNewStringWithFormat("Software\\%s\\%s", naGetStringUTF8Pointer(companyname), naGetStringUTF8Pointer(appname));
    }else{
      fullKeyName = naNewStringWithFormat("Software\\%s", naGetStringUTF8Pointer(appname));
    }

    WCHAR* systemFullKeyName = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(fullKeyName));
    LSTATUS errorCode = RegOpenKeyW(HKEY_CURRENT_USER, systemFullKeyName, &na_nativePreferences);
    if(errorCode != ERROR_SUCCESS){
      errorCode = RegCreateKeyW(HKEY_CURRENT_USER, systemFullKeyName, &na_nativePreferences);
    }

    naFree(systemFullKeyName);
    naDelete(appname);
    if(companyname){naDelete(companyname);}
    naDelete(fullKeyName);
  }
  return na_nativePreferences;
}



NA_HDEF NAi64 na_GetRawPreferencesBool(void* prefs, const char* key){
  NAi64 valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : NA_ZERO_i64;
}

NA_HDEF void na_SetRawPreferencesBool(void* prefs, const char* key, NAi64 valueStorage){
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
    if (errorCode != ERROR_SUCCESS)
      naError("Could not set bool preference");
  #endif
}



NA_HDEF NAi64 na_GetRawPreferencesInt(void* prefs, const char* key){
  NAi64 valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : NA_ZERO_i64;
}

NA_HDEF void na_SetRawPreferencesInt(void* prefs, const char* key, NAi64 valueStorage){
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
    if (errorCode != ERROR_SUCCESS)
      naError("Could not set int preference");
  #endif
}



NA_HDEF NAi64 na_GetRawPreferencesEnum(void* prefs, const char* key){
  NAi64 valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : NA_ZERO_i64;
}

NA_HDEF void na_SetRawPreferencesEnum(void* prefs, const char* key, NAi64 valueStorage){
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
    if (errorCode != ERROR_SUCCESS)
      naError("Could not set enum preference");
  #endif
}



NA_HDEF double na_GetRawPreferencesDouble(void* prefs, const char* key){
  double valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : 0.;
}

NA_HDEF void na_SetRawPreferencesDouble(void* prefs, const char* key, double valueStorage){
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
    if (errorCode != ERROR_SUCCESS)
      naError("Could not set double preference");
  #endif
}



NA_HDEF NAString* na_GetRawPreferencesString(void* prefs, const char* key){
  NAString* valueStorage;
  HKEY registry = (HKEY)prefs;
  wchar_t* systemKey = naAllocWideCharStringWithUTF8String(key);
  DWORD valueSize;
  DWORD type;
  LSTATUS errorCode = RegQueryValueExW(registry, systemKey, NULL, &type, NULL, &valueSize);
  if(errorCode != ERROR_SUCCESS){
    naFree(systemKey);
    return NA_NULL;
  }

  wchar_t* storedValue = naMalloc(valueSize);
  errorCode = RegGetValueW(registry, NULL, systemKey, RRF_RT_ANY, &type, storedValue, (LPDWORD)&valueSize);
  naFree(systemKey);

  if(errorCode == ERROR_SUCCESS){
    valueStorage = naNewStringFromWideCharString(storedValue);
    naFree(storedValue);
  }else{
    valueStorage = NA_NULL;
  }
  return valueStorage;
}

NA_HDEF void na_SetRawPreferencesString(void* prefs, const char* key, NAString* valueStorage){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  // Stupid dog. You make this look bad.

  HKEY registry = (HKEY)prefs;
  wchar_t* systemKey = naAllocWideCharStringWithUTF8String(key);
  wchar_t* storedValue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(valueStorage));
  DWORD valueSize = ((DWORD)wcslen(storedValue) + 1) * sizeof(wchar_t);
  LSTATUS errorCode = RegSetKeyValueW(registry, NULL, systemKey, REG_SZ, storedValue, valueSize);
  naFree(storedValue);
  naFree(systemKey);
  #if NA_DEBUG
    if (errorCode != ERROR_SUCCESS)
      naError("Could not set string preference");
  #endif
}



#endif // NA_COMPILE_GUI == 1
#endif // NA_OS == NA_OS_WINDOWS

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
