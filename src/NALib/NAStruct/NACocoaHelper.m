
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is a .m-File which is an implementation file in Objective-C.
// This is required on Macintosh systems to link against the Cocoa-Framework.
// If you do not have an Objective-C compiler such as in windows, this file
// will simply be ignored and hence nothing is linked. If this is not the case,
// you can simply delete the .m File from your source tree.


#include "NAUICocoa.h"


#ifdef __OBJC__
#if NA_OS == NA_OS_MAC_OS_X
// Now, we are sure, we compile with Objective-C and on MacOSX. The two
// #if directives will be closed at the very bottom of this file.

#import <Cocoa/Cocoa.h>
#include "NAString.h"


NA_DEF NSString* naAllocNSStringWithNAString(const NAString* string){
  return [NSString stringWithUTF8String:naGetStringUTF8Pointer(string)];
}




#endif // NA_OS == NA_OS_MAC_OS_X
#endif // __OBJC__

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
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
