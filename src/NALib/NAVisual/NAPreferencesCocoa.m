
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is a .m-File which is an implementation file in Objective-C.
// This is required on Macintosh systems to link against the Cocoa-Framework.
// If you do not have an Objective-C compiler such as in windows, this file
// will simply be ignored and hence nothing is linked. If this is not the case,
// you can simply delete the .m File from your source tree.


#include "NABase.h"


#if (NA_CONFIG_COMPILE_GUI == 1)
#ifdef __OBJC__
#if NA_OS == NA_OS_MAC_OS_X
// Now, we are sure, we compile with Objective-C and on MacOSX. The two
// #if directives will be closed at the very bottom of this file.



#import <Cocoa/Cocoa.h>
#include "NAPreferences.h"



NA_HIDEF NSUserDefaults* naGetNativePreferences(){
//  return [[NSUserDefaults alloc] initWithSuiteName:@"XXXXXXX.group.com.mycompany.appidentifier"];
  return [NSUserDefaults standardUserDefaults];
}



NA_DEF void naInitPreferencesBool(const char* key, NABool value){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  if([userdefaults integerForKey:nskey] == 0){
    [userdefaults setInteger:(value ? 1 : -1) forKey:nskey];
  }
}

NA_DEF void naInitPreferencesInt(const char* key, NAInt value){
  #ifndef NDEBUG
    if(value == NSIntegerMin)
      naError("Minimal integer value is reserved");
  #endif
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  if([userdefaults integerForKey:nskey] == 0){
    [userdefaults setInteger:((value == 0) ? NSIntegerMin : value) forKey:nskey];
  }
}

NA_DEF void naInitPreferencesEnum(const char* key, NAInt value){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  if([userdefaults integerForKey:nskey] == 0){
    [userdefaults setInteger:value + 1 forKey:nskey];
  }
}

NA_DEF void naInitPreferencesDouble(const char* key, double value){
  #ifndef NDEBUG
    if(naIsNaN(value))
      naError("NaN value is reserved");
  #endif
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  if([userdefaults doubleForKey:nskey] == 0.){
    [userdefaults setDouble:((value == 0) ? NA_NAN : value) forKey:nskey];
  }
}

NA_DEF void naInitPreferencesString(const char* key, NAString* value){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  if([userdefaults objectForKey:nskey] == nil){
    NSString* valueobj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(value)];
    [userdefaults setObject:valueobj forKey:nskey];
  }
}



NA_DEF void naSetPreferencesBool(const char* key, NABool value){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  [userdefaults setInteger:(value ? 1 : -1) forKey:nskey];
}

NA_DEF void naSetPreferencesInt(const char* key, NAInt value){
  #ifndef NDEBUG
    if(value == NSIntegerMin)
      naError("Minimal integer value is reserved");
  #endif
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  [userdefaults setInteger:((value == 0) ? NSIntegerMin : value) forKey:nskey];
}

NA_DEF void naSetPreferencesEnum(const char* key, NAInt value){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  [userdefaults setInteger:value + 1 forKey:nskey];
}

NA_DEF void naSetPreferencesDouble(const char* key, double value){
  #ifndef NDEBUG
    if(naIsNaN(value))
      naError("NaN value is reserved");
  #endif
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  [userdefaults setDouble:((value == 0) ? NA_NAN : value) forKey:nskey];
}

NA_DEF void naSetPreferencesString(const char* key, NAString* value){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  NSString* valueobj = [NSString stringWithUTF8String:naGetStringUTF8Pointer(value)];
  [userdefaults setObject:valueobj forKey:nskey];
}



NA_DEF NABool naGetPreferencesBool(const char* key){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  return (([userdefaults integerForKey:nskey] == 1) ? NA_TRUE : NA_FALSE);
}

NA_DEF NAInt naGetPreferencesInt(const char* key){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  NSInteger integer = [userdefaults integerForKey:nskey];
  return (NAInt)((integer == NSIntegerMin) ? 0 : integer);
}

NA_DEF NAInt naGetPreferencesEnum(const char* key){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  return (NAInt)[userdefaults integerForKey:nskey] - 1;
}

NA_DEF double naGetPreferencesDouble(const char* key){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  double dbl = [userdefaults doubleForKey:nskey];
  return (double)(naIsNaN(dbl) ? 0. : dbl);
}

NA_DEF NAString* naNewPreferencesString(const char* key){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  NSString* valueobj = [userdefaults stringForKey:nskey];
  return naNewStringWithFormat("%s", [valueobj UTF8String]);
}



NA_DEF NABool naTogglePreferencesBool(const char* key){
  NSUserDefaults* userdefaults = naGetNativePreferences();
  NSString* nskey = [NSString stringWithUTF8String:key];
  NABool value = (([userdefaults integerForKey:nskey] == 1) ? NA_TRUE : NA_FALSE);
  [userdefaults setInteger:(value ? -1 : 1) forKey:nskey];
  return !value;
}




#endif // NA_OS == NA_OS_MAC_OS_X
#endif // __OBJC__
#endif // (NA_CONFIG_COMPILE_GUI == 1)

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
