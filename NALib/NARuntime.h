
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_RUNTIME_INCLUDED
#define NA_RUNTIME_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"



typedef struct NATypeInfo NATypeInfo;
struct NATypeInfo{
  NAUInt typesize;
  NADestructor desctructor;
};


typedef void* NATypeIdentifier;
extern NATypeIdentifier na_NAString_identifier;


NA_API void               naStartRuntime();
NA_API NATypeIdentifier   naManageRuntimeType(NATypeInfo* typeinfo);
NA_API void               naStopRuntime();

#define naNew(type)       naNewStruct(na_ ## type ## _identifier)
NA_API void*              naNewStruct(NATypeIdentifier typeidentifier);
NA_API void               naDelete(void* pointer);



// Here, the author decided to put everything into implementation files
// and not provide inline functions.




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_RUNTIME_INCLUDED


// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
