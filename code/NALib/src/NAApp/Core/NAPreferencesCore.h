
#ifndef NA_PREFERENCES_CORE_INCLUDED
#define NA_PREFERENCES_CORE_INCLUDED



// System dependend methods.

NA_HAPI void* na_GetNativePreferences(void);
NA_HAPI void na_ShutdownPreferences(void);

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

#if NA_OS == NA_OS_WINDOWS

  #include <windows.h>

  // Returns a value from the windows registry.
  // 
  // The "Variable" method will allocate sufficient memory with malloc for you
  // and return the size allocated in valueSize if available.
  // 
  // The "Fixed" method expects a pointer to an existing memory location and
  // the size in bytes available at that location. The requested value will
  // be stored at that location if and only if the value size is equal to the
  // one retrieved from the registry.
  // 
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




#endif // NA_PREFERENCES_CORE_INCLUDED



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
