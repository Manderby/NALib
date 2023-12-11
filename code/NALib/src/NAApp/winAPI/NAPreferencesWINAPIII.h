
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!

#include "../NAPreferences.h"
#include "../../NAApp/NAApp.h"



typedef HKEY NA_PreferencesPtr;
NA_PreferencesPtr na_nativePreferences = NA_NULL;



NA_HIDEF NA_PreferencesPtr na_GetNativePreferences(){
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
    LSTATUS errorcode = RegOpenKeyW(HKEY_CURRENT_USER, systemFullKeyName, &na_nativePreferences);
    if(errorcode != ERROR_SUCCESS){
      errorcode = RegCreateKeyW(HKEY_CURRENT_USER, systemFullKeyName, &na_nativePreferences);
    }

    naFree(systemFullKeyName);
    naDelete(appname);
    if(companyname){naDelete(companyname);}
    naDelete(fullKeyName);
  }
  return na_nativePreferences;
}



NA_HIDEF NABool na_GetRawPreferencesBool(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  LSTATUS errorcode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, valueStorage, (LPDWORD)&valueSize);
  return (errorcode == ERROR_SUCCESS);
}

NA_HIDEF NABool na_SetRawPreferencesBool(NA_PreferencesPtr prefs, const char* key, NAi64* valueStorage){
  DWORD valueSize = NA_TYPE64_BYTES;
  LSTATUS errorcode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, valueStorage, valueSize);
  return (errorcode == ERROR_SUCCESS);
}





NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt value){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedValue;
  NA_PreferencesPtr prefs = na_GetNativePreferences();
  LSTATUS errorcode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, &storedValue, (LPDWORD)&valueSize);
  if(errorcode != ERROR_SUCCESS){
    storedValue = ((value == 0) ? NA_MIN_i64 : naCastIntToi64(value));
    errorcode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, &storedValue, valueSize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init int to Registry");
      #endif
    }
  }
  return (NAInt)((naEquali64(storedValue, NA_MIN_i64) ? 0 : naCasti64ToInt(storedValue)));
}
NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt value){
  DWORD valueSize;
  DWORD type;
  NAi64 storedValue;
  NA_PreferencesPtr prefs;
  LSTATUS errorcode;

  #if NA_DEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  valueSize = NA_TYPE64_BYTES;
  prefs = na_GetNativePreferences();
  errorcode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, &storedValue, (LPDWORD)&valueSize);
  if(errorcode != ERROR_SUCCESS){
    storedValue = naAddi64(naCastIntToi64(value), naMakei64WithLo(1));
    errorcode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, &storedValue, valueSize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init enum to Registry");
      #endif
    }
  }
  return naCasti64ToInt(naSubi64(storedValue, naMakei64WithLo(1)));
}
NA_DEF double naInitPreferencesDouble(const char* key, double value){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  double storedValue;
  NA_PreferencesPtr prefs = na_GetNativePreferences();
  LSTATUS errorcode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, &storedValue, (LPDWORD)&valueSize);
  if(errorcode != ERROR_SUCCESS){
    storedValue = ((value == 0.) ? NA_NAN : value);
    errorcode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, &storedValue, valueSize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init double to Registry");
      #endif
    }
  }
  return (naIsNaN(storedValue) ? 0. : storedValue);
}
NA_DEF NAString* naInitPreferencesString(const char* key, const NAString* value){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  // Stupid dog. You make this look bad.
  DWORD valueSize;
  DWORD type;
  wchar_t* storedValue;
  NAString* retString;

  NA_PreferencesPtr prefs = na_GetNativePreferences();
  wchar_t* systemkey = naAllocWideCharStringWithUTF8String(key);
  LSTATUS errorcode = RegQueryValueExW(prefs, systemkey, NULL, &type, NULL, &valueSize);
  if(errorcode == ERROR_SUCCESS){
    storedValue = naMalloc(valueSize);
    errorcode = RegGetValueW(prefs, NULL, systemkey, RRF_RT_ANY, &type, storedValue, (LPDWORD)&valueSize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not read string from Registry");
      #endif
    }
  }else{
    storedValue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(value));
    valueSize = ((DWORD)wcslen(storedValue) + 1) * sizeof(wchar_t);
    errorcode = RegSetKeyValueW(prefs, NULL, systemkey, REG_SZ, storedValue, valueSize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init string to Registry");
      #endif
    }
  }
  retString = naNewStringFromWideCharString(storedValue);
  naFree(storedValue);
  naFree(systemkey);
  return retString;
}



NA_DEF void naSetPreferencesInt(const char* key, NAInt value){
  DWORD valueSize = NA_TYPE64_BYTES;
  NAi64 storedValue = ((value == 0) ? NA_MIN_i64 : naCastIntToi64(value));
  NA_PreferencesPtr prefs = na_GetNativePreferences();
  LSTATUS errorcode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, &storedValue, valueSize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesEnum(const char* key, NAInt value){
  DWORD valueSize;
  NAi64 storedValue;
  NA_PreferencesPtr prefs;
  LSTATUS errorcode;

  #if NA_DEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  valueSize = NA_TYPE64_BYTES;
  storedValue = naAddi64(naCastIntToi64(value), naMakei64WithLo(1));
  prefs = na_GetNativePreferences();
  errorcode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, &storedValue, valueSize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesDouble(const char* key, double value){
  DWORD valueSize;
  double storedValue;
  NA_PreferencesPtr prefs;
  LSTATUS errorcode;

  valueSize = NA_TYPE64_BYTES;
  storedValue = ((value == 0.) ? NA_NAN : value);
  prefs = na_GetNativePreferences();
  errorcode = RegSetKeyValueA(prefs, NULL, key, REG_QWORD, &storedValue, valueSize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesString(const char* key, const NAString* value){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  // Stupid dog. You make this look bad.
  DWORD valueSize;
  wchar_t* storedValue;
  LSTATUS errorcode;
  NA_PreferencesPtr prefs;
  wchar_t* systemkey;

  prefs = na_GetNativePreferences();
  systemkey = naAllocWideCharStringWithUTF8String(key);
  storedValue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(value));
  valueSize = ((DWORD)wcslen(storedValue) + 1) * sizeof(wchar_t);
  errorcode = RegSetKeyValueW(prefs, NULL, systemkey, REG_SZ, storedValue, valueSize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry");
    #endif
  }
  naFree(storedValue);
  naFree(systemkey);
}



NA_DEF NAInt naGetPreferencesInt(const char* key){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedValue;
  NA_PreferencesPtr prefs;
  LSTATUS errorcode;

  prefs = na_GetNativePreferences();
  errorcode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, &storedValue, &valueSize);
  if(errorcode != ERROR_SUCCESS){storedValue = NA_MIN_i64;}
  return (NAInt)((naEquali64(storedValue, NA_MIN_i64)) ? 0 : naCasti64ToInt(storedValue));
}
NA_DEF NAInt naGetPreferencesEnum(const char* key){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedValue;
  NA_PreferencesPtr prefs;
  LSTATUS errorcode;

  prefs = na_GetNativePreferences();
  errorcode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, &storedValue, &valueSize);
  if(errorcode != ERROR_SUCCESS){storedValue = naMakei64WithLo(1);}
  return naCasti64ToInt(naSubi64(storedValue, naMakei64WithLo(1)));
}
NA_DEF double naGetPreferencesDouble(const char* key){
  DWORD valueSize = NA_TYPE64_BYTES;
  DWORD type;
  double storedValue;
  NA_PreferencesPtr prefs;
  LSTATUS errorcode;

  prefs = na_GetNativePreferences();
  errorcode = RegGetValueA(prefs, NULL, key, RRF_RT_ANY, &type, &storedValue, &valueSize);
  if(errorcode != ERROR_SUCCESS){storedValue = 0.;}
  return (naIsNaN(storedValue) ? 0. : storedValue);
}
NA_DEF NAString* naNewPreferencesString(const char* key){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  DWORD valueSize;
  DWORD type;
  wchar_t* storedValue;
  NAString* retString;
  NA_PreferencesPtr prefs;
  wchar_t* systemkey;
  LSTATUS errorcode;

  prefs = na_GetNativePreferences();
  systemkey = naAllocWideCharStringWithUTF8String(key);
  errorcode = RegQueryValueExW(prefs, systemkey, NULL, &type, NULL, &valueSize);
  if(errorcode != ERROR_SUCCESS){
    return NA_NULL;
  }
  storedValue = naMalloc(valueSize);
  errorcode = RegGetValueW(prefs, NULL, systemkey, RRF_RT_ANY, &type, storedValue, (LPDWORD)&valueSize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not read string from Registry");
    #endif
    return NA_NULL;
  }
  retString = naNewStringFromWideCharString(storedValue);
  naFree(storedValue);
  naFree(systemkey);
  return retString;
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
