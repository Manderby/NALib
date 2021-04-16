
#include "../NATranslator.h"
#include "../NATree.h"
#include "../NAList.h"
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

#if NA_COMPILE_GUI == 1
  #define NA_TRANSLATOR na_App->translator
#else
  NATranslator* na_Translator = NA_NULL;
  #define NA_TRANSLATOR na_Translator
#endif


NA_HDEF NAPtr na_ConstructLanguages(const void* key, NAPtr content){
  NATree* strings;
  NA_UNUSED(key);
  NA_UNUSED(content);
  strings = naAlloc(NATree);
  naInitTree(strings, NA_TRANSLATOR->stringsconfig);
  return naMakePtrWithDataMutable(strings);
}

NA_HDEF void na_DestructLanguages(NAPtr leafData){
  NATree* strings = naGetPtrMutable(leafData);
  naClearTree(strings);
  naFree(strings);
}



NA_HDEF NAPtr na_ConstructGroups(const void* key, NAPtr content){
  NATree* languages;
  NA_UNUSED(key);
  NA_UNUSED(content);
  languages = naAlloc(NATree);
  naInitTree(languages, NA_TRANSLATOR->languagesconfig);
  return naMakePtrWithDataMutable(languages);
}

NA_HDEF void na_DestructGroups(NAPtr leafData){
  NATree* languages = naGetPtrMutable(leafData);
  naClearTree(languages);
  naFree(languages);
}



NA_DEF void naStartTranslator(void){
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!na_App)
        naCrash("Translator requires application to run. Please use naStartApplication.");
      if(na_App && NA_TRANSLATOR)
        naCrash("Application translator already started.");
    #else
      if(NA_TRANSLATOR)
        naCrash("Translator already started.");
    #endif
  #endif
  NA_TRANSLATOR = naAlloc(NATranslator);

  NA_TRANSLATOR->groupsconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(NA_TRANSLATOR->groupsconfig, na_ConstructGroups, na_DestructGroups);
  NA_TRANSLATOR->languagesconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(NA_TRANSLATOR->languagesconfig, na_ConstructLanguages, na_DestructLanguages);
  NA_TRANSLATOR->stringsconfig = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);

  naInitTree(&(NA_TRANSLATOR->groups), NA_TRANSLATOR->groupsconfig);
  naInitList(&(NA_TRANSLATOR->languagepreferences));
  NA_TRANSLATOR->curgroup = -1;
  NA_TRANSLATOR->curlang = 0;
}



NA_DEF void naStopTranslator(void){
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!na_App)
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
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!na_App)
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
  NABool codefound;
  NAListIterator it;
  
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!na_App)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  NA_TRANSLATOR->curlang = code;
  
  codefound = NA_FALSE;
  it = naMakeListAccessor(&(NA_TRANSLATOR->languagepreferences));
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
  NATreeIterator groupiter;
  NATree* grouppack;
  NATreeIterator languageiter;
  NATree* languagepack;
  NATreeIterator stringiter;
  
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!na_App)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  
  // Search for the group pack and create it if necessary.
  groupiter = naMakeTreeModifier(&(NA_TRANSLATOR->groups));
  naAddTreeKeyConst(&groupiter, &(NA_TRANSLATOR->curgroup), NA_NULL, NA_FALSE);
  grouppack = naGetTreeCurLeafMutable(&groupiter);
  naClearTreeIterator(&groupiter);

  // Search for the language pack and create it if necessary.
  languageiter = naMakeTreeModifier(grouppack);
  naAddTreeKeyConst(&languageiter, &(NA_TRANSLATOR->curlang), NA_NULL, NA_FALSE);
  languagepack = naGetTreeCurLeafMutable(&languageiter);
  naClearTreeIterator(&languageiter);
  
  // Search for the string entry and replace it if necessary.
  stringiter = naMakeTreeModifier(languagepack);
  naAddTreeKeyConst(&stringiter, &id, str, NA_TRUE);
  naClearTreeIterator(&stringiter);
}



NA_DEF void naSetTranslatorLanguagePreference(NALanguageCode3 code){
  NABool codefound;
  NAListIterator it;
  
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!na_App)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  codefound = NA_FALSE;
  it = naMakeListModifier(&(NA_TRANSLATOR->languagepreferences));
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
  const NAUTF8Char* retValue = "String not found";
  NATreeIterator groupiter;
  NABool groupfound;
  
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!na_App)
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  
  // Search for the group pack.
  groupiter = naMakeTreeModifier(&(NA_TRANSLATOR->groups));
  groupfound = naLocateTreeKey(&groupiter, &group, NA_TRUE);
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
          retValue = naGetTreeCurLeafConst(&stringiter);
        }
        naClearTreeIterator(&stringiter);
      }
      naClearTreeIterator(&languageiter);
    }
    naClearListIterator(&preflangit);
  }
  naClearTreeIterator(&groupiter);

  return retValue;
}



NA_DEF NALanguageCode3 naGetLanguageCode(const NAUTF8Char* str){
  size_t strlength = naStrlen(str);
  NAInt code = 0;
  size_t i = 0;
  while(i < strlength && i < 3 && isalpha((unsigned char)str[i])){
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
