
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file combines all necessary implementations regarding the WINAPI
// framework. It is a collection of #include directives including code from
// inline implementation files. These files are .h files and hence will not
// be compiled by the IDEs by default. They do contain non-inlineable code
// though so don't threat them like normal .h files. Do not include them
// files anywhere else but here in this file.

// This file itself is a .c file which is an implementation file.

// The whole WINAPI implementation has been combined into this single file
// because compiling with WINAPI is very slow. Using only
// one single file speeds up the compilation considerably!


#include "../../../NABase.h"

#if NA_OS == NA_OS_WINDOWS
// Now, we are sure, we compile on Windows. The #if directives will be closed
// at the very bottom of this file.

#if NA_CONFIG_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  #pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <windows.h>
#include <windowsx.h>

#include "C/NAUIImageWINAPIII.h"

#if (NA_CONFIG_COMPILE_GUI == 1)

#include "C/NAPreferencesWINAPIII.h"

#include "C/UI/NAUIWINAPIII.h"
#include "C/UI/NAApplicationII.h"
#include "C/UI/NAWindowII.h"
#include "C/UI/NASpaceII.h"
#include "C/UI/NAImageSpaceII.h"
#include "C/UI/NAOpenGLSpaceII.h"
#include "C/UI/NAButtonII.h"
#include "C/UI/NARadioII.h"
#include "C/UI/NACheckBoxII.h"
#include "C/UI/NALabelII.h"
#include "C/UI/NATextFieldII.h"
#include "C/UI/NATextBoxII.h"

#endif // (NA_CONFIG_COMPILE_GUI == 1)
#endif // NA_OS == NA_OS_WINDOWS


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
