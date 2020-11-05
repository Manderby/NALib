
#ifndef NA_PREFERENCES_INCLUDED
#define NA_PREFERENCES_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "NAString.h"

// This file allows a programmer to store user preferences for an application.
// This is a system dependent implementation. On Mac, the NSUserDefaults class
// is used and on Win, the Registry is used.
//
// Please only use ASCII-7 characters to encode keys. Otherwise, it might not
// work.

// If the given key does not exists yet, creates a preference for it and sets
// the value. Returns either the value stored or the given default value.
// Note: The string is always a new string, never the given value pointer.

NA_API NABool    naInitPreferencesBool   (const char* key, NABool value);
NA_API NAInt     naInitPreferencesInt    (const char* key, NAInt value);
NA_API NAInt     naInitPreferencesEnum   (const char* key, NAInt value);
NA_API double    naInitPreferencesDouble (const char* key, double value);
NA_API NAString* naInitPreferencesString (const char* key, const NAString* value);

// Sets or toggles the preference
NA_API void   naSetPreferencesBool       (const char* key, NABool value);
NA_API void   naSetPreferencesInt        (const char* key, NAInt value);
NA_API void   naSetPreferencesEnum       (const char* key, NAInt value);
NA_API void   naSetPreferencesDouble     (const char* key, double value);
NA_API void   naSetPreferencesString     (const char* key, const NAString* value);
NA_API NABool naTogglePreferencesBool    (const char* key);

// Gets the preference. Beware for strings, naDelete must be called afterwards.
NA_API NABool    naGetPreferencesBool    (const char* key);
NA_API NAInt     naGetPreferencesInt     (const char* key);
NA_API NAInt     naGetPreferencesEnum    (const char* key);
NA_API double    naGetPreferencesDouble  (const char* key);
NA_API NAString* naNewPreferencesString  (const char* key);




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_PREFERENCES_INCLUDED



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
