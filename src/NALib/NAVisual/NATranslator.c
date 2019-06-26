
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATranslator.h"
#include "NAStack.h"
#include "NATree.h"



NATranslator* na_translator = NA_NULL;

typedef struct NALanguagePack NALanguagePack;
struct NALanguagePack{
  NALanguageCode code;
  NAStack* NATranslatorGroup;
};

struct NATranslator{
  NATree languagepacks;
  NAInt lang;
  NAInt group;
};



NAPtr naConstructGroupPack(const void* key, NAPtr content){
  NA_UNUSED(key);
  NA_UNUSED(content);
  NATree* strings = naAlloc(NATree);
  NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naInitTree(strings, config);
  naReleaseTreeConfiguration(config);
  return naMakePtrWithDataMutable(strings);
}

void naDestructGroupPack(NAPtr leafdata){
  NATree* strings = naGetPtrMutable(leafdata);
  naClearTree(strings);
  naFree(strings);
}



NAPtr naConstructLanguagePack(const void* key, NAPtr content){
  NA_UNUSED(key);
  NA_UNUSED(content);
  NATree* grouppacks = naAlloc(NATree);
  NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(config, naConstructGroupPack, naDestructGroupPack);
  naInitTree(grouppacks, config);
  naReleaseTreeConfiguration(config);
  return naMakePtrWithDataMutable(grouppacks);
}

void naDestructLanguagePack(NAPtr leafdata){
  NATree* grouppacks = naGetPtrMutable(leafdata);
  naClearTree(grouppacks);
  naFree(grouppacks);
}



void naStartTranslator(void){
  na_translator = naAlloc(NATranslator);
  NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_NAINT | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(config, naConstructLanguagePack, naDestructLanguagePack);
  naInitTree(&(na_translator->languagepacks), config);
  naReleaseTreeConfiguration(config);
}

void naStopTranslator(void){
  naClearTree(&(na_translator->languagepacks));
  naFree(na_translator);
}

void naSetTranslatorLanguage(NAInt lang){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  na_translator->lang = lang;
}

void naSetTranslatorGroup(NAInt group){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  na_translator->group = group;
}

void naSetTranslatorString(NAInt id, const char* str){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  
  // Search for the language pack and create it if necessary.
  NATreeIterator languageiter = naMakeTreeModifier(&(na_translator->languagepacks));
  naAddTreeKeyConst(&languageiter, &(na_translator->lang), NA_NULL, NA_FALSE);
  NATree* languagepack = naGetTreeCurLeafMutable(&languageiter);
  naClearTreeIterator(&languageiter);
  
  // Search for the group pack and create it if necessary.
  NATreeIterator groupiter = naMakeTreeModifier(languagepack);
  naAddTreeKeyConst(&groupiter, &(na_translator->group), NA_NULL, NA_FALSE);
  NATree* grouppack = naGetTreeCurLeafMutable(&groupiter);
  naClearTreeIterator(&groupiter);
  
  // Search for the string entry and replace it if necessary.
  NATreeIterator stringiter = naMakeTreeModifier(grouppack);
  naAddTreeKeyConst(&stringiter, &id, str, NA_FALSE);
  naClearTreeIterator(&stringiter);
}

void naLoadTranslatorFile(const char* filepath){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif

  naError("Not implemented yet");
}

const char* naTranslate(NAInt group, NAInt id){
  #ifndef NDEBUG
    if(!na_translator)
      naCrash("No translator running. Please use naStartTranslator.");
  #endif
  const char* retvalue = "String not found";
  
  // Search for the language pack.
  NATreeIterator languageiter = naMakeTreeModifier(&(na_translator->languagepacks));
  NABool langfound = naLocateTreeKey(&languageiter, &(na_translator->lang), NA_TRUE);
  if(langfound){
    NATree* languagepack = naGetTreeCurLeafMutable(&languageiter);
  
    // Search for the group pack.
    NATreeIterator groupiter = naMakeTreeModifier(languagepack);
    NABool groupfound = naLocateTreeKey(&groupiter, &group, NA_TRUE);
    if(groupfound){
      NATree* grouppack = naGetTreeCurLeafMutable(&groupiter);
      
      // Search for the string entry.
      NATreeIterator stringiter = naMakeTreeModifier(grouppack);
      NABool stringfound = naLocateTreeKey(&stringiter, &id, NA_FALSE);
      if(stringfound){
        retvalue = naGetTreeCurLeafConst(&stringiter);
      }
      naClearTreeIterator(&stringiter);
    }
    naClearTreeIterator(&groupiter);
  }
  naClearTreeIterator(&languageiter);

  return retvalue;
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
