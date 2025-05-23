
#include "../../NAUtility/NATranslator.h"
#include "../../NAStruct/NATree.h"
#include "../../NAStruct/NAList.h"
#include "../../NAUtility/NAString.h"
#include "../../NAApp/NAApp.h"
#include "../../NAApp/Core/NAAppCore.h"
#include <ctype.h>



struct NATranslator{
  NATree groups;
  NATreeConfiguration* groupsConfig;
  NATreeConfiguration* languagesConfig;
  NATreeConfiguration* stringsConfig;
  NAList languagePreferences;
  uint32 curGroup;
  uint32 curLang;
};

#if NA_COMPILE_GUI == 1
  #define NA_TRANSLATOR naGetApplication()->translator
#else
  NATranslator* na_Translator = NA_NULL;
  #define NA_TRANSLATOR na_Translator
#endif


NA_HDEF NAPtr na_ConstructLanguages(const void* key, NAPtr content) {
  NATree* strings;
  NA_UNUSED(key);
  NA_UNUSED(content);
  strings = naAlloc(NATree);
  naInitTree(strings, NA_TRANSLATOR->stringsConfig);
  return naMakePtrWithDataMutable(strings);
}

NA_HDEF void na_DestructLanguages(NAPtr leafData) {
  NATree* strings = naGetPtrMutable(leafData);
  naClearTree(strings);
  naFree(strings);
}



NA_HDEF NAPtr na_ConstructGroups(const void* key, NAPtr content) {
  NATree* languages;
  NA_UNUSED(key);
  NA_UNUSED(content);
  languages = naAlloc(NATree);
  naInitTree(languages, NA_TRANSLATOR->languagesConfig);
  return naMakePtrWithDataMutable(languages);
}

NA_HDEF void na_DestructGroups(NAPtr leafData) {
  NATree* languages = naGetPtrMutable(leafData);
  naClearTree(languages);
  naFree(languages);
}



NA_DEF void naStartTranslator(void) {
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!naGetApplication())
        naCrash("Translator requires application to run. Please use naStartApplication.");
      if(naGetApplication() && NA_TRANSLATOR)
        naCrash("Application translator already started.");
    #else
      if(NA_TRANSLATOR)
        naCrash("Translator already started.");
    #endif
  #endif
  NA_TRANSLATOR = naAlloc(NATranslator);

  NA_TRANSLATOR->groupsConfig = naCreateTreeConfiguration(NA_TREE_KEY_u32 | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(NA_TRANSLATOR->groupsConfig, na_ConstructGroups, na_DestructGroups);
  NA_TRANSLATOR->languagesConfig = naCreateTreeConfiguration(NA_TREE_KEY_u32 | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(NA_TRANSLATOR->languagesConfig, na_ConstructLanguages, na_DestructLanguages);
  NA_TRANSLATOR->stringsConfig = naCreateTreeConfiguration(NA_TREE_KEY_u32 | NA_TREE_BALANCE_AVL);

  naInitTree(&NA_TRANSLATOR->groups, NA_TRANSLATOR->groupsConfig);
  naInitList(&NA_TRANSLATOR->languagePreferences);
  NA_TRANSLATOR->curGroup = NA_MAX_u32;
  NA_TRANSLATOR->curLang = 0;
}



NA_DEF void naStopTranslator(void) {
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!naGetApplication())
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  naClearTree(&NA_TRANSLATOR->groups);
  naClearList(&NA_TRANSLATOR->languagePreferences, (NAMutator)naFree);

  naRelease(NA_TRANSLATOR->groupsConfig);
  naRelease(NA_TRANSLATOR->languagesConfig);
  naRelease(NA_TRANSLATOR->stringsConfig);

  naFree(NA_TRANSLATOR);
}



NA_DEF uint32 naRegisterTranslatorGroup(void) {
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!naGetApplication())
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  NA_TRANSLATOR->curGroup = (NA_TRANSLATOR->curGroup == NA_MAX_u32)
    ? 0
    : NA_TRANSLATOR->curGroup + 1;
  return NA_TRANSLATOR->curGroup;
}



NA_DEF void naSwitchTranslatorInsertionLanguage(NALanguageCode3 code) {
  NABool codeFound;
  NAListIterator it;
  
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!naGetApplication())
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  NA_TRANSLATOR->curLang = code;
  
  codeFound = NA_FALSE;
  it = naMakeListAccessor(&NA_TRANSLATOR->languagePreferences);
  while(naIterateList(&it)) {
    const NALanguageCode3* curCode = naGetListCurConst(&it);
    if(*curCode == code) {
      codeFound = NA_TRUE;
    }
  }
  naClearListIterator(&it);
  
  if(!codeFound) {
    uint32* newCode = naAlloc(uint32);  // No, not TranslatorCode3, enums may have not the same size!!!
    *newCode = code;
    naAddListLastMutable(&NA_TRANSLATOR->languagePreferences, newCode);
  }
}



NA_DEF void naInsertTranslatorString(uint32 id, NAUTF8Char* str) {
  NATreeIterator groupIter;
  NATree* groupPack;
  NATreeIterator languageIter;
  NATree* languagePack;
  NATreeIterator stringIter;
  
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!naGetApplication())
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  
  // Search for the group pack and create it if necessary.
  groupIter = naMakeTreeModifier(&NA_TRANSLATOR->groups);
  naAddTreeKeyConst(&groupIter, &NA_TRANSLATOR->curGroup, NA_NULL, NA_FALSE);
  groupPack = naGetTreeCurLeafMutable(&groupIter);
  naClearTreeIterator(&groupIter);

  // Search for the language pack and create it if necessary.
  languageIter = naMakeTreeModifier(groupPack);
  naAddTreeKeyConst(&languageIter, &NA_TRANSLATOR->curLang, NA_NULL, NA_FALSE);
  languagePack = naGetTreeCurLeafMutable(&languageIter);
  naClearTreeIterator(&languageIter);
  
  // Search for the string entry and replace it if necessary.
  stringIter = naMakeTreeModifier(languagePack);
  naAddTreeKeyConst(&stringIter, &id, str, NA_TRUE);
  naClearTreeIterator(&stringIter);
}



NA_DEF void naSetTranslatorLanguagePreference(NALanguageCode3 code) {
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!naGetApplication())
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  
  NABool codeFound = NA_FALSE;
  
  NAListIterator it = naMakeListModifier(&NA_TRANSLATOR->languagePreferences);
  while(!codeFound && naIterateList(&it)) {
    const NALanguageCode3* curCode = naGetListCurConst(&it);
    if(*curCode == code) {
      codeFound = NA_TRUE;
      naMoveListCurToFirst(&it, NA_FALSE, &NA_TRANSLATOR->languagePreferences);
    }
  }
  naClearListIterator(&it);
  
  // Try to find a replacement (People often have dialects selected but are
  // fine with a more general language)
  if(!codeFound) {
    NALanguageCode3 replacingCode = naGetLanguageReplacement(code);
    NAListIterator it2 = naMakeListModifier(&NA_TRANSLATOR->languagePreferences);
    while(!codeFound && naIterateList(&it2)) {
      const NALanguageCode3* curCode = naGetListCurConst(&it2);
      if(*curCode == replacingCode) {
        codeFound = NA_TRUE;
        naMoveListCurToFirst(&it2, NA_FALSE, &NA_TRANSLATOR->languagePreferences);
      }
    }
    naClearListIterator(&it2);
  }
  
  if(!codeFound) {
    uint32* newCode = naAlloc(uint32);  // No, not type TranslatorCode3, enums may have not the same size!!!
    *newCode = code;
    naAddListFirstMutable(&NA_TRANSLATOR->languagePreferences, newCode);
  }
}



NA_DEF const NAUTF8Char* naTranslate(uint32 group, uint32 id) {
  const NAUTF8Char* retValue = "String not found";
  NATreeIterator groupIter;
  NABool groupFound;
  
  #if NA_DEBUG
    #if NA_COMPILE_GUI == 1
      if(!naGetApplication())
        naCrash("Translator requires application to run. Please use naStartApplication.");
    #else
      if(!NA_TRANSLATOR)
        naCrash("No translator running. Please use naStartTranslator.");
    #endif
  #endif
  
  // Search for the group pack.
  groupIter = naMakeTreeModifier(&NA_TRANSLATOR->groups);
  groupFound = naLocateTreeKey(&groupIter, &group, NA_TRUE);
  if(groupFound) {
    NATree* groupPack = naGetTreeCurLeafMutable(&groupIter);

    // Go through the list of preferred languages
    NAListIterator prefLangIter = naMakeListAccessor(&NA_TRANSLATOR->languagePreferences);
    NABool found = NA_FALSE;
    while(!found && naIterateList(&prefLangIter)) {
      const NALanguageCode3* curLang = naGetListCurConst(&prefLangIter);
    
      // Search for the language pack.
      NATreeIterator languageIter = naMakeTreeModifier(groupPack);
      NABool langFound = naLocateTreeKey(&languageIter, curLang, NA_TRUE);
      if(langFound) {
        NATree* languagePack = naGetTreeCurLeafMutable(&languageIter);
        
        // Search for the string entry.
        NATreeIterator stringIter = naMakeTreeModifier(languagePack);
        found = naLocateTreeKey(&stringIter, &id, NA_FALSE);
        if(found) {
          retValue = naGetTreeCurLeafConst(&stringIter);
        }
        naClearTreeIterator(&stringIter);
      }
      naClearTreeIterator(&languageIter);
    }
    naClearListIterator(&prefLangIter);
  }
  naClearTreeIterator(&groupIter);

  return retValue;
}



NA_DEF NALanguageCode3 naGetLanguageCode(const NAUTF8Char* str) {
  size_t strlength = naStrlen(str);
  uint32 code = 0;
  size_t i = 0;
  while(i < strlength && i < 3 && isalpha((unsigned char)str[i])) {
    code = code << 8;
    code |= (uint32)tolower(str[i]);
    ++i;
  }
  if(code <= 0xffff) {
    code = naConvertLanguageCode1To3((NALanguageCode1)code);
  }
  return (NALanguageCode3)code;
}



#if NA_OS == NA_OS_WINDOWS

  NA_DEF NALanguageCode3 naGetLanguageCodeWithLANGID(LANGID langId) {
    // One can switch for (langId & 0x01ff) to only get the main languages
    // but not the dialects. But when using naGetLanguageReplacement, one
    // finds the proper display language anyway.

    switch(langId)
    {
    case 0x0004: return NA_LANG_ZHO;
    case 0x0007: return NA_LANG_DEU;
    case 0x0009: return NA_LANG_ENG;
    case 0x000a: return NA_LANG_SPA;
    case 0x000c: return NA_LANG_FRA;
    case 0x0011: return NA_LANG_JPN;
    case 0x0807: return NA_LANG_GSW;
    }

    // no Klingon, sorry.
    return NA_LANG_ENG;
  }

#endif // NA_OS == NA_OS_WINDOWS


NA_DEF NALanguageCode3 naConvertLanguageCode1To3(NALanguageCode1 code1) {
  switch(code1) {
  case NA_LANG_DE: return NA_LANG_DEU;
  case NA_LANG_EN: return NA_LANG_ENG;
  case NA_LANG_ES: return NA_LANG_SPA;
  case NA_LANG_FR: return NA_LANG_FRA;
  case NA_LANG_JA: return NA_LANG_JPN;
  case NA_LANG_ZH: return NA_LANG_ZHO;
  default: return NA_LANG_ENG;
  }
}



NA_DEF NALanguageCode3 naGetLanguageReplacement(NALanguageCode3 code) {
  switch(code) {
  case NA_LANG_GSW: return NA_LANG_DEU;
  default: return code;
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
