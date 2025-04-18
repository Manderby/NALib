
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!

//#include "../NAPreferences.h"



HKEY na_nativePreferences = NA_NULL;
//#include "../../NAUtility/NAMemory.h"
//#include "../../NAUtility/NAString.h"



NA_HDEF void* na_GetNativePreferences() {
  #if NA_DEBUG
  if(!naGetApplication())
    naError("No application running. Use naStartApplication.");
  #endif
  if(!na_nativePreferences) {
    NAString* appName = naNewApplicationName();
    NAString* companyName = naNewApplicationCompanyName();

    NAString* fullKeyName;
    if(companyName) {
      fullKeyName = naNewStringWithFormat("Software\\%s\\%s", naGetStringUTF8Pointer(companyName), naGetStringUTF8Pointer(appName));
    }else{
      fullKeyName = naNewStringWithFormat("Software\\%s", naGetStringUTF8Pointer(appName));
    }

    WCHAR* systemFullKeyName = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(fullKeyName));
    LSTATUS errorCode = RegOpenKeyW(HKEY_CURRENT_USER, systemFullKeyName, &na_nativePreferences);
    if(errorCode != ERROR_SUCCESS) {
      errorCode = RegCreateKeyW(HKEY_CURRENT_USER, systemFullKeyName, &na_nativePreferences);
    }

    naFree(systemFullKeyName);
    naDelete(appName);
    if(companyName) {
      naDelete(companyName);
    }
    naDelete(fullKeyName);
  }
  return na_nativePreferences;
}



NA_HDEF void na_ShutdownPreferences() {
  if(na_nativePreferences) {
    RegCloseKey(na_nativePreferences);
    na_nativePreferences = NA_NULL;
  }
}



NA_HDEF void* na_GetWINRegistryVariableEntry(
  HKEY rootKey,
  const NAUTF8Char* path,
  const NAUTF8Char* key,
  size_t* valueSize)
{
  void* retValue = NA_NULL;

  HKEY registry;
  LSTATUS errorCode;
  wchar_t* pathKey = naAllocWideCharStringWithUTF8String(path);
  errorCode = RegOpenKeyW(
    rootKey,
    pathKey,
    &registry);

  if(errorCode == ERROR_SUCCESS) {
    wchar_t* systemKey = naAllocWideCharStringWithUTF8String(key);
    DWORD expectedSize;
    DWORD type;
    errorCode = RegQueryValueExW(registry, systemKey, NULL, &type, NULL, &expectedSize);

    if(errorCode == ERROR_SUCCESS) {
      retValue = naMalloc(expectedSize);
      if(valueSize) { *valueSize = expectedSize; }

      errorCode = RegGetValueW(registry, NULL, systemKey, RRF_RT_ANY, &type, retValue, (LPDWORD)&expectedSize);

      if(errorCode != ERROR_SUCCESS) {
        naFree(retValue);
        retValue = NA_NULL;
      }
    }

    naFree(systemKey);
  }

  RegCloseKey(registry);
  naFree(pathKey);
  return retValue;
}



NA_HDEF void* na_GetWINRegistryFixedEntry(
  HKEY rootKey,
  const NAUTF8Char* path,
  const NAUTF8Char* key,
  void* value,
  size_t valueSize)
{
  #if NA_DEBUG
  if(!value)
    naError("You must provide a place to store the value.");
  if(!valueSize)
    naError("You must provide the memory size of the given value");
  #endif

  void* retValue = NA_NULL;

  HKEY registry;
  LSTATUS errorCode;
  wchar_t* pathKey = naAllocWideCharStringWithUTF8String(path);
  errorCode = RegOpenKeyW(
    rootKey,
    pathKey,
    &registry);

  if(errorCode == ERROR_SUCCESS) {
    wchar_t* systemKey = naAllocWideCharStringWithUTF8String(key);
    DWORD expectedSize;
    DWORD type;
    errorCode = RegQueryValueExW(registry, systemKey, NULL, &type, NULL, &expectedSize);

    if(errorCode == ERROR_SUCCESS) {
      if(expectedSize == valueSize) {
        errorCode = RegGetValueW(registry, NULL, systemKey, RRF_RT_ANY, &type, value, (LPDWORD)&expectedSize);

        if(errorCode == ERROR_SUCCESS) {
          retValue = value;
        }
      }
    }

    naFree(systemKey);
  }

  RegCloseKey(registry);
  naFree(pathKey);
  return retValue;
}



NA_HDEF int64 na_GetRawPreferencesBool(void* prefs, const char* key) {
  int64 valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : NA_ZERO_i64;
}

NA_HDEF void na_SetRawPreferencesBool(void* prefs, const char* key, int64 valueStorage) {
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
  if(errorCode != ERROR_SUCCESS)
    naError("Could not set bool preference");
  #endif
}



NA_HDEF int64 na_GetRawPreferencesi64(void* prefs, const char* key) {
  int64 valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : NA_ZERO_i64;
}

NA_HDEF void na_SetRawPreferencesi64(void* prefs, const char* key, int64 valueStorage) {
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
  if(errorCode != ERROR_SUCCESS)
    naError("Could not set int preference");
  #endif
}



NA_HDEF int64 na_GetRawPreferencesEnum(void* prefs, const char* key) {
  int64 valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : NA_ZERO_i64;
}

NA_HDEF void na_SetRawPreferencesEnum(void* prefs, const char* key, int64 valueStorage) {
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
  if(errorCode != ERROR_SUCCESS)
    naError("Could not set enum preference");
  #endif
}



NA_HDEF double na_GetRawPreferencesDouble(void* prefs, const char* key) {
  double valueStorage;
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorCode = RegGetValueA(registry, NULL, key, RRF_RT_ANY, &type, &valueStorage, (LPDWORD)&valueSize);
  return (errorCode == ERROR_SUCCESS)
    ? valueStorage
    : 0.;
}

NA_HDEF void na_SetRawPreferencesDouble(void* prefs, const char* key, double valueStorage) {
  HKEY registry = (HKEY)prefs;
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorCode = RegSetKeyValueA(registry, NULL, key, REG_QWORD, &valueStorage, valueSize);
  #if NA_DEBUG
  if(errorCode != ERROR_SUCCESS)
    naError("Could not set double preference");
  #endif
}



NA_HDEF NAString* na_GetRawPreferencesString(void* prefs, const char* key) {
  NAString* valueStorage;
  HKEY registry = (HKEY)prefs;
  wchar_t* systemKey = naAllocWideCharStringWithUTF8String(key);
  DWORD valueSize;
  DWORD type;
  LSTATUS errorCode = RegQueryValueExW(registry, systemKey, NULL, &type, NULL, &valueSize);
  if(errorCode != ERROR_SUCCESS) {
    naFree(systemKey);
    return NA_NULL;
  }

  wchar_t* storedValue = naMalloc(valueSize);
  errorCode = RegGetValueW(registry, NULL, systemKey, RRF_RT_ANY, &type, storedValue, (LPDWORD)&valueSize);
  naFree(systemKey);

  if(errorCode == ERROR_SUCCESS) {
    valueStorage = naNewStringWithWideCharString(storedValue);
    naFree(storedValue);
  }else{
    valueStorage = NA_NULL;
  }
  return valueStorage;
}

NA_HDEF void na_SetRawPreferencesString(void* prefs, const char* key, NAString* valueStorage) {
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
  if(errorCode != ERROR_SUCCESS)
    naError("Could not set string preference");
  #endif
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
