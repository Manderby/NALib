
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!

#include "../../../../NAPreferences.h"


//NA_HIDEF NSUserDefaults* naGetNativePreferences(){
////  return [[NSUserDefaults alloc] initWithSuiteName:@"XXXXXXX.group.com.mycompany.appidentifier"];
//  return [NSUserDefaults standardUserDefaults];
//}



NA_DEF NABool naInitPreferencesBool(const char* key, NABool value){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  NSInteger curValue = [userdefaults integerForKey:nskey];
//  if(curValue == 0){
//    curValue = (value ? 1 : -1);
//    [userdefaults setInteger:curValue forKey:nskey];
//  }
//  return (NABool)(curValue == 1);
  return NA_FALSE;
}
NA_DEF NAInt naInitPreferencesInt(const char* key, NAInt value){
//  #ifndef NDEBUG
//    if(value == NSIntegerMin)
//      naError("Minimal integer value is reserved");
//  #endif
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  NSInteger curValue = [userdefaults integerForKey:nskey];
//  if(curValue == 0){
//    curValue = ((value == 0) ? NSIntegerMin : value);
//    [userdefaults setInteger:curValue forKey:nskey];
//  }
//  return (NAInt)((curValue == NSIntegerMin) ? 0 : curValue);
  return 0;
}
NA_DEF NAInt naInitPreferencesEnum(const char* key, NAInt value){
//  #ifndef NDEBUG
//    if(value == -1)
//      naError("Value -1 can not be stored correctly.");
//  #endif
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  NSInteger curValue = [userdefaults integerForKey:nskey];
//  if(curValue == 0){
//    curValue = value + 1;
//    [userdefaults setInteger:curValue forKey:nskey];
//  }
//  return (NAInt)curValue - 1;
  return 0;
}
NA_DEF double naInitPreferencesDouble(const char* key, double value){
//  #ifndef NDEBUG
//    if(naIsNaN(value))
//      naError("NaN value is reserved");
//  #endif
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  double curValue = [userdefaults doubleForKey:nskey] ;
//  if(curValue == 0.){
//    curValue = ((value == 0) ? NA_NAN : value);
//    [userdefaults setDouble:curValue forKey:nskey];
//  }
//  return (double)(naIsNaN(curValue) ? 0. : curValue);
  return 0.0;
}
NA_DEF NAString* naInitPreferencesString(const char* key, NAString* value){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  if([userdefaults objectForKey:nskey] == nil){
//    NSString* valueobj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(value)];
//    [userdefaults setObject:valueobj forKey:nskey];
//  }
//  return naNewPreferencesString(key);
  return NA_NULL;
}



NA_DEF void naSetPreferencesBool(const char* key, NABool value){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  [userdefaults setInteger:(value ? 1 : -1) forKey:nskey];
}
NA_DEF void naSetPreferencesInt(const char* key, NAInt value){
//  #ifndef NDEBUG
//    if(value == NSIntegerMin)
//      naError("Minimal integer value is reserved");
//  #endif
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  [userdefaults setInteger:((value == 0) ? NSIntegerMin : value) forKey:nskey];
}
NA_DEF void naSetPreferencesEnum(const char* key, NAInt value){
//  #ifndef NDEBUG
//    if(value == -1)
//      naError("Value -1 can not be stored correctly.");
//  #endif
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  [userdefaults setInteger:value + 1 forKey:nskey];
}
NA_DEF void naSetPreferencesDouble(const char* key, double value){
//  #ifndef NDEBUG
//    if(naIsNaN(value))
//      naError("NaN value is reserved");
//  #endif
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  [userdefaults setDouble:((value == 0) ? NA_NAN : value) forKey:nskey];
}
NA_DEF void naSetPreferencesString(const char* key, NAString* value){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  NSString* valueobj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(value)];
//  [userdefaults setObject:valueobj forKey:nskey];
}



NA_DEF NABool naGetPreferencesBool(const char* key){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  return (([userdefaults integerForKey:nskey] == 1) ? NA_TRUE : NA_FALSE);
  return NA_FALSE;
}
NA_DEF NAInt naGetPreferencesInt(const char* key){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  NSInteger integer = [userdefaults integerForKey:nskey];
//  return (NAInt)((integer == NSIntegerMin) ? 0 : integer);
  return 0;
}
NA_DEF NAInt naGetPreferencesEnum(const char* key){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  return (NAInt)[userdefaults integerForKey:nskey] - 1;
  return 0;
}
NA_DEF double naGetPreferencesDouble(const char* key){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  double dbl = [userdefaults doubleForKey:nskey];
//  return (double)(naIsNaN(dbl) ? 0. : dbl);
  return 0.0;
}
NA_DEF NAString* naNewPreferencesString(const char* key){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  NSString* valueobj = [userdefaults stringForKey:nskey];
//  return naNewStringWithFormat("%s", [valueobj UTF8String]);
  return NA_NULL;
}



NA_DEF NABool naTogglePreferencesBool(const char* key){
//  NSUserDefaults* userdefaults = naGetNativePreferences();
//  NSString* nskey = [NSString stringWithUTF8String:key];
//  NABool value = (([userdefaults integerForKey:nskey] == 1) ? NA_TRUE : NA_FALSE);
//  [userdefaults setInteger:(value ? -1 : 1) forKey:nskey];
//  return !value;
  return NA_FALSE;
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
