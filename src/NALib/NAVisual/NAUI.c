
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NAUICoreAPI.h"

#if (NA_CONFIG_COMPILE_GUI == 1)

// This file contains function implementations which are directly forwarding
// to a function in the CoreUI implementations.


// ///////////////////////////////////
// APPLICATION
// ///////////////////////////////////


NA_DEF void naStopApplication(void){
  naStopCoreApplication();
}



NA_DEF NAApplication* naGetApplication(void){
  return (NAApplication*)naGetCoreApplication();
}



// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////

NA_DEF NAUIElementType naGetUIElementType(NAUIElement* element){
  return naGetCoreUIElementType((NACoreUIElement*)element);
}



NA_DEF NAUIElement* naGetUIElementParent(NAUIElement* uielement){
  return naGetCoreUIElementParent((NACoreUIElement*)uielement);
}



NA_DEF NAWindow* naGetUIElementWindow(NAUIElement* uielement){
  return naGetCoreUIElementWindow(uielement);
}



NA_DEF NANativeID naGetUIElementNativeID(NAUIElement* element){
  return naGetCoreUIElementNativeID((NACoreUIElement*)element);
}



NA_DEF void naRefreshUIElement(NAUIElement* uielement, double timediff){
  naRefreshCoreUIElement(uielement, timediff);
}



#endif //(NA_CONFIG_COMPILE_GUI == 1)


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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
