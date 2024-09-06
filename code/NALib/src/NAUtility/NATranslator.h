
#ifndef NA_TRANSLATOR_INCLUDED
#define NA_TRANSLATOR_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

// ////////////////
// NATranslator is a very basic way of adding multilingual support to an
// application. It links arbitrary defined enum ids with strings defined in
// external files. These files get included into the source code directly,
// therefore, no additional tools or preprocessors are needed.
//
// A translator file is a simple UTF-8 encoded text file with contents like
// the following:
//
// // English
// NA_LOC_LANG("eng");
// NA_LOC(HELLO_WORLD,            "Hello World");
// NA_LOC(99_BEER,                "99 Bottles of beer");
// NA_LOC(BREADCRUMBS_BEAVERSPIT, "Bread crumbs and beaver spit");
//
// // German
// NA_LOC_LANG("deu");
// NA_LOC(HELLO_WORLD,            "Hallo Welt");
// NA_LOC(99_BEER,                "99 Flaschen Bier");
// NA_LOC(BREADCRUMBS_BEAVERSPIT, "Brotkrumen und Biberspucke");
//
// // Swiss German
// NA_LOC_LANG("gsw");
// NA_LOC(HELLO_WORLD,            "Sali Welt");
// NA_LOC(99_BEER,                "99 Fläsche Bier");
// NA_LOC(BREADCRUMBS_BEAVERSPIT, "Brösmeli und Biberspeuz");
//
// You can use // comments and split the content into as many files you like.
// These textfiles are then simply included into your sourcecode like for
// example:
// 
// void myInitTranslations() {
//   myTranslatorGroup = naRegisterTranslatorGroup();
//   #include "../translations/eng.txt"
//   #include "../translations/deu.txt"
// }
//
// The myTranslatorGroup is an uint32 identifying a group of strings belonging
// to the same execution unit. For example, your application has a group
// "myAppTranslations" but you might compile this project together with a
// separate project which definies and includes its own strings in a group
// called "MyConverterTranslations". Later, you will identify the strings
// using this uint32 group identifier.
//
// In order for the #includes to work, somewhere in your code, integer ids
// must be provided, defining the identifiers of the translation items.
// Most probably, you want to do this using an enum:
//
//typedef enum{
//  HELLO_WORLD,
//  99_BEER,
//  BREADCRUMBS_BEAVERSPIT
//} SomeTranslationsNeededInMyApplication;
//
// Be sure to have called naStartTranslator() somewere before including your
// translator files. Note that when using naStartApplication(), a translator
// will already have been started for you.
//
// Finally, when you want to have a translated string, you call naTranslate
// with the desired group and string identifier.
//
// printf ("%s", naTranslate(myTranslatorGroup, BREADCRUMBS_BEAVERSPIT));
//
// To switch the language, you can use naSetTranslatorLanguagePreference but
// when working with an naApplication, you will not need to switch at all, as
// the user-preferred language of the system will automatically be detected.
//
// If some string is undefined in one language, the next available string is
// taken according to the defined language preference. See the function
// naSetTranslatorLanguagePreference for more information.
//
// If a string is not available in any language, a default string is returned.



typedef struct NATranslator NATranslator;

#include "../NABase/NABase.h"

// These codes denote the standardized ISO 639-3 codes.
typedef enum{
  NA_LANG_DEU = 0x646575,  // German
  NA_LANG_ENG = 0x656e67,  // English
  NA_LANG_FRA = 0x667261,  // French
  NA_LANG_GSW = 0x677377,  // Swiss German
  NA_LANG_JPN = 0x6a706e,  // Japanese
  NA_LANG_SPA = 0x737061,  // Spanish
  NA_LANG_TLH = 0x746c68,  // Klingon
  NA_LANG_ZHO = 0x7a686f,  // Chinese
} NALanguageCode3;

// These codes denote the standardized ISO 639-1 codes. They should not be
// used, please use the 3 variant whenever possible or use the conversion
// function naConvertLanguageCode1To3
typedef enum{
  NA_LANG_DE = 0x6465,  // German
  NA_LANG_EN = 0x656e,  // English
  NA_LANG_ES = 0x6573,  // Spanish
  NA_LANG_FR = 0x6672,  // French
  NA_LANG_JA = 0x6a61,  // Japanese
  NA_LANG_ZH = 0x7a68,  // Chinese
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
NA_API uint32 naRegisterTranslatorGroup(void);

// Sets the given language as the current language for string insertion and
// registers its availability in the current translator.
NA_API void naSwitchTranslatorInsertionLanguage(NALanguageCode3 code);

// Stores a given string for the given id in the current language and the
// current group. If there already was a string, it will be replaced.
NA_API void naInsertTranslatorString(uint32 id, NAUTF8Char* str);

// Usually, when working with translator files, you will be using the following
// macros with the strings being encoded in UTF-8:
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
// Also note that when using naStartApplication, the user preferences of the
// system will automatically be set for you.
NA_API void naSetTranslatorLanguagePreference(NALanguageCode3 code);

// Returns the UTF8-String of the given id in the given group, according to
// the language preferences.
NA_API const NAUTF8Char* naTranslate(uint32 group, uint32 id);



// ////////////////////////////////
// Helper functions for language codes

// Returns an enum denoting the provided ISO 639-3 string. For example "eng".
NA_API NALanguageCode3 naGetLanguageCode(const NAUTF8Char* str);
#if NA_OS == NA_OS_WINDOWS
  #include "windows.h"
  NA_API NALanguageCode3 naGetLanguageCodeWithLANGID(LANGID langId);
#endif

// Converts a code of ISO 639-1 into ISO 639-3. For example, converts en to eng
// or de to deu. Try to avoid this function, it's bloated. Use ISO 639-3.
NA_API NALanguageCode3 naConvertLanguageCode1To3(NALanguageCode1 code1);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_TRANSLATOR_INCLUDED



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
