
// This file combines all necessary implementations regarding the Cocoa
// framework. It is a collection of #include directives including code from
// inline implementation files. These files are .h files and hence will not
// be compiled by the IDEs by default. They do contain non-inlineable code
// though so don't treat them like normal .h files. Do not include them
// files anywhere else but here in this file.

// This file itself is a .m file which is an implementation file in
// Objective-C and links against the Cocoa-Framework. If you do not have an
// Objective-C compiler such as in windows, this file will simply be ignored
// and hence nothing is linked. If this is not the case, you can simply
// remove the .m File from your source tree.

// The whole cocoa implementation has been combined into this single file
// because using only one single file speeds up the compilation considerably!


#include "../../NABase/NABase.h"

#if NA_OS == NA_OS_MAC_OS_X
#if defined __OBJC__
#if NA_COMPILE_GUI == 1
// Now, we are sure, we compile with Objective-C and on MacOSX. The
// #if directives will be closed at the very bottom of this file.

#if NA_NATIVE_INT64_IN_USE

#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#include "NACocoaLegacyII.h"

#include "NACocoaClipboardII.h"
#include "NACocoaFontII.h"
#include "NACocoaImageSetII.h"
#include "NACocoaMouseII.h"
#include "NACocoaKeyboardII.h"
#include "NACocoaPreferencesII.h"

#include "NACocoaII.h"
#include "NACocoaColorII.h"
#include "NACocoaInterfaces.h"

#include "UIElements/NACocoaApplicationII.h"
#include "UIElements/NACocoaButtonII.h"
#include "UIElements/NACocoaCheckBoxII.h"
#include "UIElements/NACocoaImageSpaceII.h"
#include "UIElements/NACocoaLabelII.h"
#include "UIElements/NACocoaMenuII.h"
#include "UIElements/NACocoaMenuItemII.h"
#include "UIElements/NACocoaMetalSpaceII.h"
#include "UIElements/NACocoaOpenGLSpaceII.h"
#include "UIElements/NACocoaRadioII.h"
#include "UIElements/NACocoaScreenII.h"
#include "UIElements/NACocoaSelectII.h"
#include "UIElements/NACocoaSliderII.h"
#include "UIElements/NACocoaSpaceII.h"
#include "UIElements/NACocoaTextBoxII.h"
#include "UIElements/NACocoaTextFieldII.h"
#include "UIElements/NACocoaUIElementII.h"
#include "UIElements/NACocoaWindowII.h"

#else // NA_NATIVE_INT64_IN_USE
  #error Compiling NALib and Cocoa without a native int64 type will not work.
#endif

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
