
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../NATranslator.h"
#include "../NAStack.h"
#include "../NATree.h"
#include "../NAString.h"
#include "../NAUI.h"
#include "NAUICore.h"
#include <ctype.h>



struct NATranslator{
  NATree groups;
  NATreeConfiguration* groupsconfig;
  NATreeConfiguration* languagesconfig;
  NATreeConfiguration* stringsconfig;
  NAList languagepreferences;
  NAInt curgroup;
  NAInt curlang;
};

#if NA_CONFIG_COMPILE_GUI == 1
  #define NA_TRANSLATOR na_app->translator
#else
  NATranslator* na_translator = NA_NULL;
  #define NA_TRANSLATOR na_translator
#endif


NA_HDEF NAPtr naConstructLanguages(const void* key, NAPtr content){
  NA_UNUSED(key);
  NA_UNUSED(content);
  NATree* strings = naAlloc(NATree);
  naInitTree(strings, NA_TRANSLATOR->stringsconfig);
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
  naInitTree(languages, NA_TRANSLATOR->languagesconfig);
  return naMakePtrWithDataMutable(languages);
}

NA_HDEF void naDestructGroups(NAPtr leafdata){
  NATree* languages = naGetPtrMutable(leafdata);
  naClearTree(languages);
  naFree(languages);
}



NA_DEF void naStartTranslator(void){
  #ifndef NDEBUG
    #if NA_CONFIG_COMPILE_GUI == 1
      if(!na_app)
        naCrash("Translator requires application to run. Please use naStartApplication.");
      if(na_app && NA_TRANSLATOR)
        naCrash("Application translator already started.");
    #else
      if(NA_TRANSLATOR)
        naCrash("Translator already started.");
    #endif
  #endif
  NA_TRANSLATOR = naAlloc(NATranslator);

  NA_TRANSLATOR->groupsconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(NA_TRANSLATOR->groupsconfig, naConstructGroups, naDestructGroups);
  NA_TRANSLATOR->languagesconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(NA_TRANSLATOR->languagesconfig, naConstructLanguages, naDestructLanguages);
  NA_TRANSLATOR->stringsconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);

  naInitTree(&(NA_TRANSLATOR->groups), NA_TRANSLATOR->groupsconfig);
  naInitList(&(NA_TRANSLATOR->languagepreferences));
  NA_TRANSLATOR->curgroup = -1;
  NA_TRANSLATOR->curlang = 0;
}



NA_DEF void naStopTranslator(void){
  #ifndef NDEBUG
    #if NA_CONFIG_COMPILE_GUI == 1
      if(!na_app)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  naClearTree(&(NA_TRANSLATOR->groups));
  naForeachListMutable(&(NA_TRANSLATOR->languagepreferences), naFree);
  naClearList(&(NA_TRANSLATOR->languagepreferences));

  naReleaseTreeConfiguration(NA_TRANSLATOR->groupsconfig);
  naReleaseTreeConfiguration(NA_TRANSLATOR->languagesconfig);
  naReleaseTreeConfiguration(NA_TRANSLATOR->stringsconfig);

  naFree(NA_TRANSLATOR);
}



NA_DEF NAInt naRegisterTranslatorGroup(void){
  #ifndef NDEBUG
    #if NA_CONFIG_COMPILE_GUI == 1
      if(!na_app)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  NA_TRANSLATOR->curgroup++;
  return NA_TRANSLATOR->curgroup;
}



NA_DEF void naSwitchTranslatorInsertionLanguage(NALanguageCode3 code){
  #ifndef NDEBUG
    #if NA_CONFIG_COMPILE_GUI == 1
      if(!na_app)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  NA_TRANSLATOR->curlang = code;
  
  NABool codefound = NA_FALSE;
  NAListIterator it = naMakeListAccessor(&(NA_TRANSLATOR->languagepreferences));
  while(naIterateList(&it)){
    const NALanguageCode3* curcode = naGetListCurConst(&it);
    if(*curcode == code){codefound = NA_TRUE;}
  }
  naClearListIterator(&it);
  
  if(!codefound){
    NAInt* newcode = naAlloc(NAInt);  // No, not TranslatorCode3, enums may have not the same size!!!
    *newcode = code;
    naAddListLastMutable(&(NA_TRANSLATOR->languagepreferences), newcode);
  }
}



NA_DEF void naInsertTranslatorString(NAInt id, NAUTF8Char* str){
  #ifndef NDEBUG
    #if NA_CONFIG_COMPILE_GUI == 1
      if(!na_app)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  
  // Search for the group pack and create it if necessary.
  NATreeIterator groupiter = naMakeTreeModifier(&(NA_TRANSLATOR->groups));
  naAddTreeKeyConst(&groupiter, &(NA_TRANSLATOR->curgroup), NA_NULL, NA_FALSE);
  NATree* grouppack = naGetTreeCurLeafMutable(&groupiter);
  naClearTreeIterator(&groupiter);

  // Search for the language pack and create it if necessary.
  NATreeIterator languageiter = naMakeTreeModifier(grouppack);
  naAddTreeKeyConst(&languageiter, &(NA_TRANSLATOR->curlang), NA_NULL, NA_FALSE);
  NATree* languagepack = naGetTreeCurLeafMutable(&languageiter);
  naClearTreeIterator(&languageiter);
  
  // Search for the string entry and replace it if necessary.
  NATreeIterator stringiter = naMakeTreeModifier(languagepack);
  naAddTreeKeyConst(&stringiter, &id, str, NA_TRUE);
  naClearTreeIterator(&stringiter);
}



NA_DEF void naSetTranslatorLanguagePreference(NALanguageCode3 code){
  #ifndef NDEBUG
    #if NA_CONFIG_COMPILE_GUI == 1
      if(!na_app)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  NABool codefound = NA_FALSE;
  NAListIterator it = naMakeListModifier(&(NA_TRANSLATOR->languagepreferences));
  while(!codefound && naIterateList(&it)){
    const NALanguageCode3* curcode = naGetListCurConst(&it);
    if(*curcode == code){
      codefound = NA_TRUE;
      naMoveListCurToFirst(&it, NA_FALSE, &(NA_TRANSLATOR->languagepreferences));
    }
  }
  naClearListIterator(&it);
  
  if(!codefound){
    NAInt* newcode = naAlloc(NAInt);  // No, not TranslatorCode3, enums may have not the same size!!!
    *newcode = code;
    naAddListFirstMutable(&(NA_TRANSLATOR->languagepreferences), newcode);
  }
}



NA_DEF const NAUTF8Char* naTranslate(NAInt group, NAInt id){
  #ifndef NDEBUG
    #if NA_CONFIG_COMPILE_GUI == 1
      if(!na_app)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  const NAUTF8Char* retvalue = "String not found";
  
  // Search for the group pack.
  NATreeIterator groupiter = naMakeTreeModifier(&(NA_TRANSLATOR->groups));
  NABool groupfound = naLocateTreeKey(&groupiter, &group, NA_TRUE);
  if(groupfound){
    NATree* grouppack = naGetTreeCurLeafMutable(&groupiter);

    // Go through the list of preferred languages
    NAListIterator preflangit = naMakeListAccessor(&(NA_TRANSLATOR->languagepreferences));
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
  default: return NA_LANG_ENG;
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
