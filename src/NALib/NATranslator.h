
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_TRANSLATOR_INCLUDED
#define NA_TRANSLATOR_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

typedef struct NATranslator NATranslator;

#include "NABase.h"

// These codes denote the standardized ISO 639-3 codes.
typedef enum{
  NA_LANG_DEU = 6579573,  // German
  NA_LANG_ENG = 6647399,  // English
  NA_LANG_GSW = 6779767,  // Swiss German
} NALanguageCode3;

// These codes denote the standardized ISO 639-1 codes. They should not be
// used, please use the 3 variant whenever possible or use the conversion
// function naConvertLanguageCode1To3
typedef enum{
  NA_LANG_DE = 25701,  // German
  NA_LANG_EN = 25966,  // English
} NALanguageCode1;



// Starts and stops the translator.
NA_API void naStartTranslator(void);
NA_API void naStopTranslator(void);



// ////////////////////////////////
// Inserting strings in languages and groups

// Registers a new group and sets it as the current group for string insertion.
// You have to store the returning value somewhere in your program for later
// use. How you do this is up to you.
// Note: Once you register a group, you have to insert all strings of that
// group. You can not come back to it later.
NA_API NAInt naRegisterTranslatorGroup(void);

// Sets the given language as the current language for string insertion and
// registers its availability in the current translator.
NA_API void naSwitchTranslatorInsertionLanguage(NALanguageCode3 code);

// Stores a given string for the given id in the current language and the
// current group. If there already was a string, it will be replaced.
NA_API void naInsertTranslatorString(NAInt id, NAUTF8Char* str);

// Usually, when working with translator files, you will be using the following
// macros:
#define NA_LOC_LANG(langstr) naSwitchTranslatorInsertionLanguage(naGetLanguageCode(langstr))
#define NA_LOC(id, str)      naInsertTranslatorString(id, str)



// ////////////////////////////////
// Getting translated strings in preferred languages

// Declares the given language to be the topmost preferred language.
// If you call this function multiple times, the previous languages simply
// move back in preference. So, if you like english first, then french,
// call this function in backwards order with "fra" first, and then "eng".
// Note that if you never call this function, the existing language from
// string registering will automatically be available in order of insertion.
NA_API void naSetTranslatorLanguagePreference(NALanguageCode3 code);

// Returns the UTF8-String of the given id in the given group, according to
// the language preferences.
NA_API const NAUTF8Char* naTranslate(NAInt group, NAInt id);



// ////////////////////////////////
// Helper functions for language codes

// Returns an enum denoting the provided ISO 639-3 string. For example "eng".
NA_API NALanguageCode3 naGetLanguageCode(const char* str);

// Converts a code of ISO 639-1 into ISO 639-3. For example, converts en to eng
// or de to deu. Try to avoid this function, it's bloated. Use ISO 639-3.
NA_API NALanguageCode3 naConvertLanguageCode1To3(NALanguageCode1 code1);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_TRANSLATOR_INCLUDED


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
