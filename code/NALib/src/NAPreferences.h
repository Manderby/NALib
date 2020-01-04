
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

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
NA_API NAString* naInitPreferencesString (const char* key, NAString* value);

// Sets or toggles the preference
NA_API void   naSetPreferencesBool       (const char* key, NABool value);
NA_API void   naSetPreferencesInt        (const char* key, NAInt value);
NA_API void   naSetPreferencesEnum       (const char* key, NAInt value);
NA_API void   naSetPreferencesDouble     (const char* key, double value);
NA_API void   naSetPreferencesString     (const char* key, NAString* value);
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
