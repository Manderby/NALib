
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!

#include "NAPreferences.h"
#include "NAUI.h"


NA_HIDEF NSUserDefaults* na_GetNativePreferences(){
  #ifndef NDEBUG
    if(!naGetApplication())
      naError("No application running. Use naStartApplication.");
  #endif
//  return [[NSUserDefaults alloc] initWithSuiteName:@"XXXXXXX.group.com.mycompany.appidentifier"];
  return [NSUserDefaults standardUserDefaults];
}



NA_DEF NABool naInitPreferencesBool(const char* key, NABool value){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NSInteger curValue = [userDefaults integerForKey:nsKey];
  if(curValue == 0){
    curValue = (value ? 1 : -1);
    [userDefaults setInteger:curValue forKey:nsKey];
  }
  return (NABool)(curValue == 1);
}
NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  NSInteger curValue;
  #ifndef NDEBUG
    if(value == NSIntegerMin)
      naError("Minimal integer value is reserved");
  #endif
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  curValue = [userDefaults integerForKey:nsKey];
  if(curValue == 0){
    curValue = ((value == 0) ? NSIntegerMin : value);
    [userDefaults setInteger:curValue forKey:nsKey];
  }
  return (NAInt)((curValue == NSIntegerMin) ? 0 : curValue);
}
NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  NSInteger curValue;
  #ifndef NDEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  curValue = [userDefaults integerForKey:nsKey];
  if(curValue == 0){
    curValue = value + 1;
    [userDefaults setInteger:curValue forKey:nsKey];
  }
  return (NAInt)curValue - 1;
}
NA_DEF double naInitPreferencesDouble(const char* key, double value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  double curValue;
  #ifndef NDEBUG
    if(naIsNaN(value))
      naError("NaN value is reserved");
  #endif
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  curValue = [userDefaults doubleForKey:nsKey] ;
  if(curValue == 0.){
    curValue = ((value == 0.) ? NA_NAN : value);
    [userDefaults setDouble:curValue forKey:nsKey];
  }
  return (double)(naIsNaN(curValue) ? 0. : curValue);
}
NA_DEF NAString* naInitPreferencesString(const char* key, NAString* value){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  if([userDefaults objectForKey:nsKey] == nil){
    NSString* valueObj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(value)];
    [userDefaults setObject:valueObj forKey:nsKey];
  }
  return naNewPreferencesString(key);
}



NA_DEF void naSetPreferencesBool(const char* key, NABool value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  [userDefaults setInteger:(value ? 1 : -1) forKey:nsKey];
}
NA_DEF void naSetPreferencesInt(const char* key, NAInt value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  #ifndef NDEBUG
    if(value == NSIntegerMin)
      naError("Minimal integer value is reserved");
  #endif
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  [userDefaults setInteger:((value == 0) ? NSIntegerMin : value) forKey:nsKey];
}
NA_DEF void naSetPreferencesEnum(const char* key, NAInt value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  #ifndef NDEBUG
    if(value == -1)
      naError("Value -1 can not be stored correctly.");
  #endif
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  [userDefaults setInteger:value + 1 forKey:nsKey];
}
NA_DEF void naSetPreferencesDouble(const char* key, double value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  #ifndef NDEBUG
    if(naIsNaN(value))
      naError("NaN value is reserved");
  #endif
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  [userDefaults setDouble:((value == 0.) ? NA_NAN : value) forKey:nsKey];
}
NA_DEF void naSetPreferencesString(const char* key, NAString* value){
  NSUserDefaults* userDefaults;
  NSString* nsKey;
  NSString* valueObj;
  userDefaults = na_GetNativePreferences();
  nsKey = [NSString stringWithUTF8String:key];
  valueObj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(value)];
  [userDefaults setObject:valueObj forKey:nsKey];
}



NA_DEF NABool naGetPreferencesBool(const char* key){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  return (([userDefaults integerForKey:nsKey] == 1) ? NA_TRUE : NA_FALSE);
}
NA_DEF NAInt naGetPreferencesInt(const char* key){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NSInteger integer = [userDefaults integerForKey:nsKey];
  return (NAInt)((integer == NSIntegerMin) ? 0 : integer);
}
NA_DEF NAInt naGetPreferencesEnum(const char* key){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  return (NAInt)[userDefaults integerForKey:nsKey] - 1;
}
NA_DEF double naGetPreferencesDouble(const char* key){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  double dbl = [userDefaults doubleForKey:nsKey];
  return (double)(naIsNaN(dbl) ? 0. : dbl);
}
NA_DEF NAString* naNewPreferencesString(const char* key){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NSString* valueObj = [userDefaults stringForKey:nsKey];
  return naNewStringWithFormat("%s", [valueObj UTF8String]);
}



NA_DEF NABool naTogglePreferencesBool(const char* key){
  NSUserDefaults* userDefaults = na_GetNativePreferences();
  NSString* nsKey = [NSString stringWithUTF8String:key];
  NABool value = (([userDefaults integerForKey:nsKey] == 1) ? NA_TRUE : NA_FALSE);
  [userDefaults setInteger:(value ? -1 : 1) forKey:nsKey];
  return !value;
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
