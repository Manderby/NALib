
#ifndef NA_PREFERENCES_INCLUDED
#define NA_PREFERENCES_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "../NAUtility/NAString.h"

// This file allows a programmer to store user preferences for an application.
// This is a system dependent implementation. On Mac, the NSUserDefaults class
// is used and on Win, the Registry is used.
//
// Please only use ASCII-7 characters to encode keys. Otherwise, it might not
// work.



// Init functions:
// If the given key does not exist yet, creates a preference for it and sets
// the value. If it already exists, it checkes whether the value is within
// the given range as described further below and sets the value if not.
// Returns the value which is stored afterwards.

// Bool Range check: Value must be true or false.
NA_API NABool naInitPreferencesBool(
  const char* key,
  NABool newValue);

// Int Range check: Value must be within [min, max].
NA_API NAInt naInitPreferencesInt(
  const char* key,
  NAInt newValue,
  NAInt min,
  NAInt max);

// Enum Range check: The value must be non-negative and below count.
NA_API NAInt naInitPreferencesEnum(
  const char* key,
  NAInt newValue,
  NAInt count);

// Int Range check: Value must be within [min, max].
NA_API double naInitPreferencesDouble(
  const char* key,
  double newValue,
  double min,
  double max);

// String range check: No range check.
// Note: The string is always a new string, never the given value pointer. You
// must use naDelete afterwards.
NA_API NAString* naInitPreferencesString (
  const char* key,
  NAString* newValue);

// Sets the preference. Returns true, if the preference was different before
// or did not exist.
NA_API NABool naSetPreferencesBool       (const char* key, NABool newValue);
NA_API NABool naSetPreferencesInt        (const char* key, NAInt newValue);
NA_API NABool naSetPreferencesEnum       (const char* key, NAInt newValue);
NA_API NABool naSetPreferencesDouble     (const char* key, double newValue);
NA_API NABool naSetPreferencesString     (const char* key, NAString* newValue);

// Toggles the boolean preference. Returns the value which is stored afterwards.
NA_API NABool naTogglePreferencesBool    (const char* key);

// Gets the preference. Beware for strings, naDelete must be called afterwards.
NA_API NABool    naGetPreferencesBool    (const char* key);
NA_API NAInt     naGetPreferencesInt     (const char* key);
NA_API NAInt     naGetPreferencesEnum    (const char* key);
NA_API double    naGetPreferencesDouble  (const char* key);
NA_API NAString* naNewPreferencesString  (const char* key);



// Inline implementations are in a separate file: 
#include "Core/NAPreferencesII.h"


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
