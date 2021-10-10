
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!

#include "../../NAPreferences.h"
#include "../../NAApp.h"


NA_HIDEF HKEY na_GetNativePreferences(){
  HKEY resultKey;
  WCHAR* systemfullkeyname;
  LSTATUS errorcode;
  NAString* appname;
  NAString* companyname;
  NAString* fullkeyname;

  #if NA_DEBUG
    if(!naGetApplication())
      naError("No application running. Use naStartApplication.");
  #endif
  appname = naNewApplicationName();
  companyname = naNewApplicationCompanyName();
  fullkeyname;
  if(companyname){
    fullkeyname = naNewStringWithFormat("Software\\%s\\%s", naGetStringUTF8Pointer(companyname), naGetStringUTF8Pointer(appname));
  }else{
    fullkeyname = naNewStringWithFormat("Software\\%s", naGetStringUTF8Pointer(appname));
  }
  systemfullkeyname = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(fullkeyname));
  errorcode = RegOpenKeyW(HKEY_CURRENT_USER, systemfullkeyname, &resultKey);
  if(errorcode != ERROR_SUCCESS){
    errorcode = RegCreateKeyW(HKEY_CURRENT_USER, systemfullkeyname, &resultKey);
  }
  naFree(systemfullkeyname);
  naDelete(appname);
  if(companyname){naDelete(companyname);}
  naDelete(fullkeyname);
  return resultKey;
}



NA_DEF NABool naInitPreferencesBool(const char* key, NABool value){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedvalue;
  HKEY hKey = na_GetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = (value ? naMakei64WithLo(1) : naMakei64WithLo(-1));
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init bool to Registry");
      #endif
    }
  }
  return ((naEquali64(storedvalue, naMakei64WithLo(1))) ? NA_TRUE : NA_FALSE);
}
NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt value){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedvalue;
  HKEY hKey = na_GetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = ((value == 0) ? NA_MIN_i64 : naCastIntToi64(value));
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init int to Registry");
      #endif
    }
  }
  return (NAInt)((naEquali64(storedvalue, NA_MIN_i64) ? 0 : naCasti64ToInt(storedvalue)));
}
NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt value){
  DWORD valuesize;
  DWORD type;
  NAi64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  #if NA_DEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  valuesize = NA_TYPE64_BYTES;
  hKey = na_GetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = naAddi64(naCastIntToi64(value), naMakei64WithLo(1));
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init enum to Registry");
      #endif
    }
  }
  return naCasti64ToInt(naSubi64(storedvalue, naMakei64WithLo(1)));
}
NA_DEF double naInitPreferencesDouble(const char* key, double value){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  double storedvalue;
  HKEY hKey = na_GetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = ((value == 0.) ? NA_NAN : value);
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init double to Registry");
      #endif
    }
  }
  return (naIsNaN(storedvalue) ? 0. : storedvalue);
}
NA_DEF NAString* naInitPreferencesString(const char* key, const NAString* value){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  // Stupid dog. You make this look bad.
  DWORD valuesize;
  DWORD type;
  wchar_t* storedvalue;
  NAString* retString;

  HKEY hKey = na_GetNativePreferences();
  wchar_t* systemkey = naAllocWideCharStringWithUTF8String(key);
  LSTATUS errorcode = RegQueryValueExW(hKey, systemkey, NULL, &type, NULL, &valuesize);
  if(errorcode == ERROR_SUCCESS){
    storedvalue = naMalloc(valuesize);
    errorcode = RegGetValueW(hKey, NULL, systemkey, RRF_RT_ANY, &type, storedvalue, (LPDWORD)&valuesize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not read string from Registry");
      #endif
    }
  }else{
    storedvalue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(value));
    valuesize = ((DWORD)wcslen(storedvalue) + 1) * sizeof(wchar_t);
    errorcode = RegSetKeyValueW(hKey, NULL, systemkey, REG_SZ, storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #if NA_DEBUG
        naError("Could not init string to Registry");
      #endif
    }
  }
  retString = naNewStringFromWideCharString(storedvalue);
  naFree(storedvalue);
  naFree(systemkey);
  return retString;
}



NA_DEF void naSetPreferencesBool(const char* key, NABool value){
  DWORD valuesize = NA_TYPE64_BYTES;
  NAi64 storedvalue = (value ? naMakei64WithLo(1) : naMakei64WithLo(-1));
  HKEY hKey = na_GetNativePreferences();

  LSTATUS errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesInt(const char* key, NAInt value){
  DWORD valuesize = NA_TYPE64_BYTES;
  NAi64 storedvalue = ((value == 0) ? NA_MIN_i64 : naCastIntToi64(value));
  HKEY hKey = na_GetNativePreferences();
  LSTATUS errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesEnum(const char* key, NAInt value){
  DWORD valuesize;
  NAi64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  #if NA_DEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  valuesize = NA_TYPE64_BYTES;
  storedvalue = naAddi64(naCastIntToi64(value), naMakei64WithLo(1));
  hKey = na_GetNativePreferences();
  errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesDouble(const char* key, double value){
  DWORD valuesize;
  double storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  valuesize = NA_TYPE64_BYTES;
  storedvalue = ((value == 0.) ? NA_NAN : value);
  hKey = na_GetNativePreferences();
  errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
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
  DWORD valuesize;
  wchar_t* storedvalue;
  LSTATUS errorcode;
  HKEY hKey;
  wchar_t* systemkey;

  hKey = na_GetNativePreferences();
  systemkey = naAllocWideCharStringWithUTF8String(key);
  storedvalue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(value));
  valuesize = ((DWORD)wcslen(storedvalue) + 1) * sizeof(wchar_t);
  errorcode = RegSetKeyValueW(hKey, NULL, systemkey, REG_SZ, storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not store value in registry");
    #endif
  }
  naFree(storedvalue);
  naFree(systemkey);
}



NA_DEF NABool naGetPreferencesBool(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = na_GetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, &valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = naMakei64WithLo(0);}
  return ((naEquali64(storedvalue, naMakei64WithLo(1))) ? NA_TRUE : NA_FALSE);
}
NA_DEF NAInt naGetPreferencesInt(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = na_GetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, &valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = NA_MIN_i64;}
  return (NAInt)((naEquali64(storedvalue, NA_MIN_i64)) ? 0 : naCasti64ToInt(storedvalue));
}
NA_DEF NAInt naGetPreferencesEnum(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = na_GetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, &valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = naMakei64WithLo(1);}
  return naCasti64ToInt(naSubi64(storedvalue, naMakei64WithLo(1)));
}
NA_DEF double naGetPreferencesDouble(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  double storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = na_GetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, &valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = 0.;}
  return (naIsNaN(storedvalue) ? 0. : storedvalue);
}
NA_DEF NAString* naNewPreferencesString(const char* key){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  DWORD valuesize;
  DWORD type;
  wchar_t* storedvalue;
  NAString* retString;
  HKEY hKey;
  wchar_t* systemkey;
  LSTATUS errorcode;

  hKey = na_GetNativePreferences();
  systemkey = naAllocWideCharStringWithUTF8String(key);
  errorcode = RegQueryValueExW(hKey, systemkey, NULL, &type, NULL, &valuesize);
  if(errorcode != ERROR_SUCCESS){
    return NA_NULL;
  }
  storedvalue = naMalloc(valuesize);
  errorcode = RegGetValueW(hKey, NULL, systemkey, RRF_RT_ANY, &type, storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    #if NA_DEBUG
      naError("Could not read string from Registry");
    #endif
    return NA_NULL;
  }
  retString = naNewStringFromWideCharString(storedvalue);
  naFree(storedvalue);
  naFree(systemkey);
  return retString;
}



NA_DEF NABool naTogglePreferencesBool(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAi64 storedvalue;
  HKEY hKey = na_GetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = naMakei64WithLo(-1);}
  storedvalue = (!naEquali64(storedvalue, naMakei64WithLo(0)) ? naMakei64WithLo(-1) : naMakei64WithLo(1));
  errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  return ((naEquali64(storedvalue, naMakei64WithLo(1))) ? NA_TRUE : NA_FALSE);
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
