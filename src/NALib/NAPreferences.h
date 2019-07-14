
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_PREFERENCES_INCLUDED
#define NA_PREFERENCES_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



#include "NAString.h"



// If the given key does not exists yet, creates a preference for it and sets
// the value.
NA_API void naInitPreferencesBool      (const char* key, NABool value);
NA_API void naInitPreferencesInt       (const char* key, NAInt value);
NA_API void naInitPreferencesEnum      (const char* key, NAInt value);
NA_API void naInitPreferencesDouble    (const char* key, double value);
NA_API void naInitPreferencesString    (const char* key, NAString* value);

// Sets or toggles the preference
NA_API void   naSetPreferencesBool     (const char* key, NABool value);
NA_API void   naSetPreferencesInt      (const char* key, NAInt value);
NA_API void   naSetPreferencesEnum     (const char* key, NAInt value);
NA_API void   naSetPreferencesDouble   (const char* key, double value);
NA_API void   naSetPreferencesString   (const char* key, NAString* value);
NA_API NABool naTogglePreferencesBool  (const char* key);

// Gets the preference. Beware for strings, naDelete must be called afterwards.
NA_API NABool    naGetPreferencesBool  (const char* key);
NA_API NAInt     naGetPreferencesInt   (const char* key);
NA_API NAInt     naGetPreferencesEnum  (const char* key);
NA_API double    naGetPreferencesDouble(const char* key);
NA_API NAString* naNewPreferencesString(const char* key);




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
