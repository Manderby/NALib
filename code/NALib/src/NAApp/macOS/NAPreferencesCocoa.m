
#include "../NAPreferences.h"

#if NA_COMPILE_GUI == 1

#include "../../NAApp/NAApp.h"
#import <Cocoa/Cocoa.h>



NA_HDEF void* na_GetNativePreferences(){
  #if NA_DEBUG
    if(!naGetApplication())
      naError("No application running. Use naStartApplication.");
  #endif
//  return [[NSUserDefaults alloc] initWithSuiteName:@"XXXXXXX.group.com.mycompany.appidentifier"];
  return NA_COCOA_PTR_OBJC_TO_C([NSUserDefaults standardUserDefaults]);
}



NA_HDEF NABool na_GetRawPreferencesBool(void* prefs, const char* key, NAi64* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  *valueStorage = (NAi64)[userDefault integerForKey:nsKey];
  return NA_TRUE;
}

NA_HDEF NABool na_SetRawPreferencesBool(void* prefs, const char* key, NAi64* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setInteger:(NSInteger)*valueStorage forKey:nsKey];
  return NA_TRUE;
}



NA_HDEF NABool na_GetRawPreferencesInt(void* prefs, const char* key, NAi64* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  *valueStorage = (NAi64)[userDefault integerForKey:nsKey];
  return NA_TRUE;
}

NA_HDEF NABool na_SetRawPreferencesInt(void* prefs, const char* key, NAi64* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setInteger:(NSInteger)*valueStorage forKey:nsKey];
  return NA_TRUE;
}



NA_HDEF NABool na_GetRawPreferencesEnum(void* prefs, const char* key, NAi64* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  *valueStorage = (NAi64)[userDefault integerForKey:nsKey];
  return NA_TRUE;
}

NA_HDEF NABool na_SetRawPreferencesEnum(void* prefs, const char* key, NAi64* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setInteger:(NSInteger)*valueStorage forKey:nsKey];
  return NA_TRUE;
}



NA_HDEF NABool na_GetRawPreferencesDouble(void* prefs, const char* key, double* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  *valueStorage = [userDefault doubleForKey:nsKey];
  return NA_TRUE;
}

NA_HDEF NABool na_SetRawPreferencesDouble(void* prefs, const char* key, double* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setDouble:*valueStorage forKey:nsKey];
  return NA_TRUE;
}



NA_HDEF NABool na_GetRawPreferencesString(void* prefs, const char* key, NAString** valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NSString* valueObj = [userDefault stringForKey:nsKey];
  *valueStorage = naNewStringWithFormat("%s", [valueObj UTF8String]);
  return NA_TRUE;
}

NA_HDEF NABool na_SetRawPreferencesString(void* prefs, const char* key, NAString** valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NSString* valueObj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(*valueStorage)];
  [userDefault setObject:valueObj forKey:nsKey];
  return NA_TRUE;
}



#endif // NA_COMPILE_GUI == 1



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
