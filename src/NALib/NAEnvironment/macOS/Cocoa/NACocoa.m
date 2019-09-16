
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file combines all necessary implementations regarding the Cocoa
// framework. It is a collection of #include directives including code from
// inline implementation files. These files are .h files and hence will not
// be compiled by the IDEs by default. They do contain non-inlineable code
// though so don't threat them like normal .h files. Do not include them
// files anywhere else but here in this file.

// This file itself is a .m file which is an implementation file in
// Objective-C and links against the Cocoa-Framework. If you do not have an
// Objective-C compiler such as in windows, this file will simply be ignored
// and hence nothing is linked. If this is not the case, you can simply
// remove the .m File from your source tree.

// The whole cocoa implementation has been combined into this single file
// because compiling an .m file is very slow compared to C files. Using only
// one single file speeds up the compilation considerably!


#include "NABase.h"

#if NA_OS == NA_OS_MAC_OS_X
#if defined __OBJC__
// Now, we are sure, we compile with Objective-C and on MacOSX. The
// #if directives will be closed at the very bottom of this file.

#import <Cocoa/Cocoa.h>

#include "ObjectiveC/NAHelperCocoaII.h"
#include "ObjectiveC/NAUIImageCocoaII.h"

#if (NA_CONFIG_COMPILE_GUI == 1)

#include "ObjectiveC/NAPreferencesCocoaII.h"

#include "ObjectiveC/UI/NAUICocoaII.h"
#include "ObjectiveC/UI/NAApplicationII.h"
#include "ObjectiveC/UI/NAWindowII.h"
#include "ObjectiveC/UI/NASpaceII.h"
#include "ObjectiveC/UI/NAImageSpaceII.h"
#include "ObjectiveC/UI/NAOpenGLSpaceII.h"
#include "ObjectiveC/UI/NAButtonII.h"
#include "ObjectiveC/UI/NARadioII.h"
#include "ObjectiveC/UI/NACheckBoxII.h"
#include "ObjectiveC/UI/NALabelII.h"
#include "ObjectiveC/UI/NATextFieldII.h"
#include "ObjectiveC/UI/NATextBoxII.h"

#endif // (NA_CONFIG_COMPILE_GUI == 1)
#endif // defined __OBJC__
#endif // NA_OS == NA_OS_MAC_OS_X


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
