
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_MONEY_INCLUDED
#define NA_MONEY_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



#include "NABase.h"

// These codes denote the standardized ISO 639-3 codes.
typedef enum{
  NA_LANG_DEU,  // German
  NA_LANG_ENG,  // English
  NA_LANG_GSW   // Swiss German
} NALanguageCode;

#define NA_LOC_LANG(lang)   naSetTranslatorLanguage(lang)
#define NA_LOC_GROUP(group) naSetTranslatorGroup(group)
#define NA_LOC(id, str)     naSetTranslatorString(id, str)

typedef struct NATranslator NATranslator;



void naStartTranslator(void);
void naStopTranslator(void);

void naSetTranslatorLanguage(NAInt lang);

void naSetTranslatorGroup(NAInt group);

void naSetTranslatorString(NAInt id, const char* str);

void naLoadTranslatorFile(const char* filepath);

const char* naTranslate(NAInt group, NAInt id);



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_MONEY_INCLUDED


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
