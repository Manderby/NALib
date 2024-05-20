
#include "../../NABase/NABase.h"

#if NA_OS == NA_OS_MAC_OS_X
#if defined __OBJC__
#if NA_COMPILE_GUI == 1

#import <Cocoa/Cocoa.h>
#include "../NAPreferences.h"
#include "../../NAApp/NAApp.h"



NA_HDEF void* na_GetNativePreferences(){
  #if NA_DEBUG
    if(!naGetApplication())
      naError("No application running. Use naStartApplication.");
  #endif
//  return [[NSUserDefaults alloc] initWithSuiteName:@"XXXXXXX.group.com.mycompany.appidentifier"];
  return NA_COCOA_PTR_OBJC_TO_C([NSUserDefaults standardUserDefaults]);
}



NA_HDEF NAi64 na_GetRawPreferencesBool(void* prefs, const char* key){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  return (NAi64)[userDefault integerForKey:nsKey];
}

NA_HDEF void na_SetRawPreferencesBool(void* prefs, const char* key, NAi64 valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setInteger:(NSInteger)valueStorage forKey:nsKey];
}



NA_HDEF NAi64 na_GetRawPreferencesInt(void* prefs, const char* key){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  return (NAi64)[userDefault integerForKey:nsKey];
}

NA_HDEF void na_SetRawPreferencesInt(void* prefs, const char* key, NAi64 valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setInteger:(NSInteger)valueStorage forKey:nsKey];
}



NA_HDEF NAi64 na_GetRawPreferencesEnum(void* prefs, const char* key){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  return (NAi64)[userDefault integerForKey:nsKey];
}

NA_HDEF void na_SetRawPreferencesEnum(void* prefs, const char* key, NAi64 valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setInteger:(NSInteger)valueStorage forKey:nsKey];
}



NA_HDEF double na_GetRawPreferencesDouble(void* prefs, const char* key){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  return [userDefault doubleForKey:nsKey];
}

NA_HDEF void na_SetRawPreferencesDouble(void* prefs, const char* key, double valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  [userDefault setDouble:valueStorage forKey:nsKey];
}



NA_HDEF NAString* na_GetRawPreferencesString(void* prefs, const char* key){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NSString* valueObj = [userDefault stringForKey:nsKey];
  return valueObj
    ? naNewStringWithFormat("%s", [valueObj UTF8String])
    : NA_NULL;
}

NA_HDEF void na_SetRawPreferencesString(void* prefs, const char* key, NAString* valueStorage){
  NSUserDefaults* userDefault = (NSUserDefaults*)NA_COCOA_PTR_C_TO_OBJC(prefs);
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NSString* valueObj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(valueStorage)];
  [userDefault setObject:valueObj forKey:nsKey];
}



#endif // NA_COMPILE_GUI == 1
#endif // defined __OBJC__
#endif // NA_OS == NA_OS_MAC_OS_X



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
