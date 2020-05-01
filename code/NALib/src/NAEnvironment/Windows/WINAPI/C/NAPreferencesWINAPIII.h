
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!

#include "../../../../NAPreferences.h"
#include "../../../../NAUI.h"


NA_HIDEF HKEY naGetNativePreferences(){
  HKEY resultKey;
  WCHAR* systemfullkeyname;
  LSTATUS errorcode;
  NAString* appname;
  NAString* companyname;
  NAString* fullkeyname;

  #ifndef NDEBUG
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
  NAInt64 storedvalue;
  HKEY hKey = naGetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = (value ? naMakeInt64WithLo(1) : naMakeInt64WithLo(-1));
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #ifndef NDEBUG
        naError("Could not init bool to Registry");
      #endif
    }
  }
  return ((naEqualInt64(storedvalue, naMakeInt64WithLo(1))) ? NA_TRUE : NA_FALSE);
}
NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt value){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAInt64 storedvalue;
  HKEY hKey = naGetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = ((value == 0) ? NA_MIN_64 : naCastIntToInt64(value));
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #ifndef NDEBUG
        naError("Could not init int to Registry");
      #endif
    }
  }
  return (NAInt)((naEqualInt64(storedvalue, NA_MIN_64) ? 0 : naCastInt64ToInt(storedvalue)));
}
NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt value){
  DWORD valuesize;
  DWORD type;
  NAInt64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  #ifndef NDEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  valuesize = NA_TYPE64_BYTES;
  hKey = naGetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = naAddInt64(naCastIntToInt64(value), naMakeInt64WithLo(1));
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #ifndef NDEBUG
        naError("Could not init enum to Registry");
      #endif
    }
  }
  return naCastInt64ToInt(naSubInt64(storedvalue, naMakeInt64WithLo(1)));
}
NA_DEF double naInitPreferencesDouble(const char* key, double value){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  double storedvalue;
  HKEY hKey = naGetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    storedvalue = ((value == 0.) ? NA_NAN : value);
    errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #ifndef NDEBUG
        naError("Could not init double to Registry");
      #endif
    }
  }
  return (naIsNaN(storedvalue) ? 0. : storedvalue);
}
NA_DEF NAString* naInitPreferencesString(const char* key, NAString* value){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  // Stupid dog. You make this look bad.
  DWORD valuesize;
  DWORD type;
  wchar_t* storedvalue;
  NAString* retString;

  HKEY hKey = naGetNativePreferences();
  wchar_t* systemkey = naAllocWideCharStringWithUTF8String(key);
  LSTATUS errorcode = RegQueryValueExW(hKey, systemkey, NULL, &type, NULL, &valuesize);
  if(errorcode == ERROR_SUCCESS){
    storedvalue = naMalloc(valuesize);
    errorcode = RegGetValueW(hKey, NULL, systemkey, RRF_RT_ANY, &type, storedvalue, (LPDWORD)&valuesize);
    if(errorcode != ERROR_SUCCESS){
      #ifndef NDEBUG
        naError("Could not read string from Registry");
      #endif
    }
  }else{
    storedvalue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(value));
    valuesize = ((DWORD)wcslen(storedvalue) + 1) * sizeof(wchar_t);
    errorcode = RegSetKeyValueW(hKey, NULL, systemkey, REG_SZ, storedvalue, valuesize);
    if(errorcode != ERROR_SUCCESS){
      #ifndef NDEBUG
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
  NAInt64 storedvalue = (value ? naMakeInt64WithLo(1) : naMakeInt64WithLo(-1));
  HKEY hKey = naGetNativePreferences();

  LSTATUS errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #ifndef NDEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesInt(const char* key, NAInt value){
  DWORD valuesize = NA_TYPE64_BYTES;
  NAInt64 storedvalue = ((value == 0) ? NA_MIN_64 : naCastIntToInt64(value));
  HKEY hKey = naGetNativePreferences();
  LSTATUS errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #ifndef NDEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesEnum(const char* key, NAInt value){
  DWORD valuesize;
  NAInt64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  #ifndef NDEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  valuesize = NA_TYPE64_BYTES;
  storedvalue = naAddInt64(naCastIntToInt64(value), naMakeInt64WithLo(1));
  hKey = naGetNativePreferences();
  errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #ifndef NDEBUG
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
  hKey = naGetNativePreferences();
  errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #ifndef NDEBUG
      naError("Could not store value in registry.");
    #endif
  }
}
NA_DEF void naSetPreferencesString(const char* key, NAString* value){
  // Note that in this function it is necessary to convert the key to wchar_t
  // format as only the W functions can store unicode strings.
  // Stupid dog. You make this look bad.
  DWORD valuesize;
  wchar_t* storedvalue;
  LSTATUS errorcode;
  HKEY hKey;
  wchar_t* systemkey;

  hKey = naGetNativePreferences();
  systemkey = naAllocWideCharStringWithUTF8String(key);
  storedvalue = naAllocWideCharStringWithUTF8String(naGetStringUTF8Pointer(value));
  valuesize = ((DWORD)wcslen(storedvalue) + 1) * sizeof(wchar_t);
  errorcode = RegSetKeyValueW(hKey, NULL, systemkey, REG_SZ, storedvalue, valuesize);
  if(errorcode != ERROR_SUCCESS){
    #ifndef NDEBUG
      naError("Could not store value in registry");
    #endif
  }
  naFree(storedvalue);
  naFree(systemkey);
}



NA_DEF NABool naGetPreferencesBool(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAInt64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = naGetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, &valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = naMakeInt64WithLo(0);}
  return ((naEqualInt64(storedvalue, naMakeInt64WithLo(1))) ? NA_TRUE : NA_FALSE);
}
NA_DEF NAInt naGetPreferencesInt(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAInt64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = naGetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, &valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = NA_MIN_64;}
  return (NAInt)((naEqualInt64(storedvalue, NA_MIN_64)) ? 0 : naCastInt64ToInt(storedvalue));
}
NA_DEF NAInt naGetPreferencesEnum(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  NAInt64 storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = naGetNativePreferences();
  errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, &valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = naMakeInt64WithLo(1);}
  return naCastInt64ToInt(naSubInt64(storedvalue, naMakeInt64WithLo(1)));
}
NA_DEF double naGetPreferencesDouble(const char* key){
  DWORD valuesize = NA_TYPE64_BYTES;
  DWORD type;
  double storedvalue;
  HKEY hKey;
  LSTATUS errorcode;

  hKey = naGetNativePreferences();
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

  hKey = naGetNativePreferences();
  systemkey = naAllocWideCharStringWithUTF8String(key);
  errorcode = RegQueryValueExW(hKey, systemkey, NULL, &type, NULL, &valuesize);
  if(errorcode != ERROR_SUCCESS){
    return NA_NULL;
  }
  storedvalue = naMalloc(valuesize);
  errorcode = RegGetValueW(hKey, NULL, systemkey, RRF_RT_ANY, &type, storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){
    #ifndef NDEBUG
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
  NAInt64 storedvalue;
  HKEY hKey = naGetNativePreferences();
  LSTATUS errorcode = RegGetValueA(hKey, NULL, key, RRF_RT_ANY, &type, &storedvalue, (LPDWORD)&valuesize);
  if(errorcode != ERROR_SUCCESS){storedvalue = naMakeInt64WithLo(-1);}
  storedvalue = (!naEqualInt64(storedvalue, naMakeInt64WithLo(0)) ? naMakeInt64WithLo(-1) : naMakeInt64WithLo(1));
  errorcode = RegSetKeyValueA(hKey, NULL, key, REG_QWORD, &storedvalue, valuesize);
  return ((naEqualInt64(storedvalue, naMakeInt64WithLo(1))) ? NA_TRUE : NA_FALSE);
}



// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
