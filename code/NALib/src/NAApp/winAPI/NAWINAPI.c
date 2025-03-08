
// This file combines all necessary implementations regarding the WINAPI
// framework. It is a collection of #include directives including code from
// inline implementation II files. These files are .h files and hence will not
// be compiled by the IDEs by default. They do contain non-inlineable code
// though so don't treat them like normal .h files. Do not include these
// files anywhere else but here in this file.

// This file itself is a .c file which is an implementation file.

// The whole WINAPI implementation has been combined into this single file
// because using only one single file speeds up the compilation considerably!


#include "../../NABase/NAConfiguration.h"
#if NA_COMPILE_GUI == 1

#include "../../NABase/NABase.h"
#if NA_OS == NA_OS_WINDOWS



#if(NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1)
  #pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif



#include <windows.h>
#include <windowsx.h>

#include "../Core/NAAppCore.h"
#include "../../NAUtility/NAString.h"


#include "NAWINAPIClipboardII.h"
#include "NAWINAPIFontII.h"
#include "NAWINAPIImageSetII.h"
#include "NAWINAPIKeyboardII.h"
#include "NAWINAPIMouseII.h"
#include "NAWINAPIPreferencesII.h"

#include "NAWINAPIII.h"
#include "NAWINAPIColorII.h"
#include "NAWINAPIInterfaces.h"

void naFillSpaceBackgroundColor(NAColor* color, const NASpace* space);
NAWINAPIColor* naGetWINAPISpaceBackgroundColor(const NAWINAPISpace* winapiSpace);

#include "UIElements/NAWINAPIApplicationII.h"
#include "UIElements/NAWINAPIButtonII.h"
#include "UIElements/NAWINAPICheckBoxII.h"
#include "UIElements/NAWINAPIImageSpaceII.h"
#include "UIElements/NAWINAPILabelII.h"
#include "UIElements/NAWINAPIMenuII.h"
#include "UIElements/NAWINAPIMenuItemII.h"
#include "UIElements/NAWINAPIMetalSpaceII.h"
#include "UIElements/NAWINAPIOpenGLSpaceII.h"
#include "UIElements/NAWINAPIRadioII.h"
#include "UIElements/NAWINAPISelectII.h"
#include "UIElements/NAWINAPISliderII.h"
#include "UIElements/NAWINAPISpaceII.h"
#include "UIElements/NAWINAPITextBoxII.h"
#include "UIElements/NAWINAPITextFieldII.h"
#include "UIElements/NAWINAPIUIElementII.h"
#include "UIElements/NAWINAPIWindowII.h"



#endif // NA_COMPILE_GUI == 1
#endif // NA_OS == NA_OS_WINDOWS



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
