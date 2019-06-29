
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATranslator.h"
#include "NAStack.h"
#include "NATree.h"
#include "NAString.h"
#include <ctype.h>



typedef struct NATranslator NATranslator;
struct NATranslator{
  NATree groups;
  NATreeConfiguration* groupsconfig;
  NATreeConfiguration* languagesconfig;
  NATreeConfiguration* stringsconfig;
  NAList languagepreferences;
  NAInt curgroup;
  NAInt curlang;
};

NATranslator* na_translator = NA_NULL;



NA_HDEF NAPtr naConstructLanguages(const void* key, NAPtr content){
  NA_UNUSED(key);
  NA_UNUSED(content);
  NATree* strings = naAlloc(NATree);
  naInitTree(strings, na_translator->stringsconfig);
  return naMakePtrWithDataMutable(strings);
}

NA_HDEF void naDestructLanguages(NAPtr leafdata){
  NATree* strings = naGetPtrMutable(leafdata);
  naClearTree(strings);
  naFree(strings);
}



NA_HDEF NAPtr naConstructGroups(const void* key, NAPtr content){
  NA_UNUSED(key);
  NA_UNUSED(content);
  NATree* languages = naAlloc(NATree);
  naInitTree(languages, na_translator->languagesconfig);
  return naMakePtrWithDataMutable(languages);
}

NA_HDEF void naDestructGroups(NAPtr leafdata){
  NATree* languages = naGetPtrMutable(leafdata);
  naClearTree(languages);
  naFree(languages);
}



NA_DEF void naStartTranslator(void){
  na_translator = naAlloc(NATranslator);

  na_translator->groupsconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(na_translator->groupsconfig, naConstructGroups, naDestructGroups);
  na_translator->languagesconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(na_translator->languagesconfig, naConstructLanguages, naDestructLanguages);
  na_translator->stringsconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);

  naInitTree(&(na_translator->groups), na_translator->groupsconfig);
  naInitList(&(na_translator->languagepreferences));
  na_translator->curgroup = -1;
  na_translator->curlang = 0;
}



NA_DEF void naStopTranslator(void){
  naClearTree(&(na_translator->groups));
  naForeachListMutable(&(na_translator->languagepreferences), naFree);
  naClearList(&(na_translator->languagepreferences));

  naReleaseTreeConfiguration(na_translator->groupsconfig);
  naReleaseTreeConfiguration(na_translator->languagesconfig);
  naReleaseTreeConfiguration(na_translator->stringsconfig);

  naFree(na_translator);
}



NA_DEF NAInt naRegisterTranslatorGroup(void){
  na_translator->curgroup++;
  return na_translator->curgroup;
}



NA_DEF void naSwitchTranslatorInsertionLanguage(NALanguageCode3 code){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  na_translator->curlang = code;
  
  NABool codefound = NA_FALSE;
  NAListIterator it = naMakeListAccessor(&(na_translator->languagepreferences));
  while(naIterateList(&it)){
    const NALanguageCode3* curcode = naGetListCurConst(&it);
    if(*curcode == code){codefound = NA_TRUE;}
  }
  naClearListIterator(&it);
  
  if(!codefound){
    NAInt* newcode = naAlloc(NAInt);  // No, not TranslatorCode3, enums may have not the same size!!!
    *newcode = code;
    naAddListLastMutable(&(na_translator->languagepreferences), newcode);
  }
}



NA_DEF void naInsertTranslatorString(NAInt id, NAUTF8Char* str){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  
  // Search for the group pack and create it if necessary.
  NATreeIterator groupiter = naMakeTreeModifier(&(na_translator->groups));
  naAddTreeKeyConst(&groupiter, &(na_translator->curgroup), NA_NULL, NA_FALSE);
  NATree* grouppack = naGetTreeCurLeafMutable(&groupiter);
  naClearTreeIterator(&groupiter);

  // Search for the language pack and create it if necessary.
  NATreeIterator languageiter = naMakeTreeModifier(grouppack);
  naAddTreeKeyConst(&languageiter, &(na_translator->curlang), NA_NULL, NA_FALSE);
  NATree* languagepack = naGetTreeCurLeafMutable(&languageiter);
  naClearTreeIterator(&languageiter);
  
  // Search for the string entry and replace it if necessary.
  NATreeIterator stringiter = naMakeTreeModifier(languagepack);
  naAddTreeKeyConst(&stringiter, &id, str, NA_TRUE);
  naClearTreeIterator(&stringiter);
}



NA_DEF void naSetTranslatorLanguagePreference(NALanguageCode3 code){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  NABool codefound = NA_FALSE;
  NAListIterator it = naMakeListModifier(&(na_translator->languagepreferences));
  while(!codefound && naIterateList(&it)){
    const NALanguageCode3* curcode = naGetListCurConst(&it);
    if(*curcode == code){
      codefound = NA_TRUE;
      naMoveListCurToFirst(&it, NA_FALSE, &(na_translator->languagepreferences));
    }
  }
  naClearListIterator(&it);
  
  if(!codefound){
    NAInt* newcode = naAlloc(NAInt);  // No, not TranslatorCode3, enums may have not the same size!!!
    *newcode = code;
    naAddListFirstMutable(&(na_translator->languagepreferences), newcode);
  }
}



NA_DEF const NAUTF8Char* naTranslate(NAInt group, NAInt id){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  const NAUTF8Char* retvalue = "String not found";
  
  // Search for the group pack.
  NATreeIterator groupiter = naMakeTreeModifier(&(na_translator->groups));
  NABool groupfound = naLocateTreeKey(&groupiter, &group, NA_TRUE);
  if(groupfound){
    NATree* grouppack = naGetTreeCurLeafMutable(&groupiter);

    // Go through the list of preferred languages
    NAListIterator preflangit = naMakeListAccessor(&(na_translator->languagepreferences));
    NABool found = NA_FALSE;
    while(!found && naIterateList(&preflangit)){
      const NALanguageCode3* curlang = naGetListCurConst(&preflangit);
    
      // Search for the language pack.
      NATreeIterator languageiter = naMakeTreeModifier(grouppack);
      NABool langfound = naLocateTreeKey(&languageiter, curlang, NA_TRUE);
      if(langfound){
        NATree* languagepack = naGetTreeCurLeafMutable(&languageiter);
        
        // Search for the string entry.
        NATreeIterator stringiter = naMakeTreeModifier(languagepack);
        found = naLocateTreeKey(&stringiter, &id, NA_FALSE);
        if(found){
          retvalue = naGetTreeCurLeafConst(&stringiter);
        }
        naClearTreeIterator(&stringiter);
      }
      naClearTreeIterator(&languageiter);
    }
    naClearListIterator(&preflangit);
  }
  naClearTreeIterator(&groupiter);

  return retvalue;
}



NA_DEF NALanguageCode3 naGetLanguageCode(const char* str){
  NAInt strlength = naStrlen(str);
  NAInt code = 0;
  int i = 0;
  while(i < strlength && i < 3 && isalpha(str[i])){
    code = code << 8;
    code |= tolower(str[i]);
    i++;
  }
  if(code <= 0xffff){code = naConvertLanguageCode1To3((NALanguageCode1)code);}
  return (NALanguageCode3)code;
}



NA_DEF NALanguageCode3 naConvertLanguageCode1To3(NALanguageCode1 code1){
  switch(code1){
  case NA_LANG_DE: return NA_LANG_DEU;
  case NA_LANG_EN: return NA_LANG_ENG;
  }
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
