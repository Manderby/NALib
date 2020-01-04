
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_KEY_INCLUDED
#define NA_KEY_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// This macro helps you define the name of the key function in a starndardized
// way. For example:
//
// NA_KEY_OP(Greater, double)  ->  naGreaterKeydouble
//
// You are free to use the evaluated name directly bu beware if the API
// should change!
#define NA_KEY_OP(operator, type) NA_CONCAT4(na, operator, Key, type)



NA_IAPI void   NA_KEY_OP(Assign,       double)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Lower,        double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LowerEqual,   double)(const void* a, const void* b);
NA_IAPI void   NA_KEY_OP(Add,          double)(void* dst, const void* src1, const void* src2);

NA_IAPI void   NA_KEY_OP(Assign,       float)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Lower,        float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LowerEqual,   float)(const void* a, const void* b);
NA_IAPI void   NA_KEY_OP(Add,          float)(void* dst, const void* src1, const void* src2);

NA_IAPI void   NA_KEY_OP(Assign,       NAInt)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Lower,        NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LowerEqual,   NAInt)(const void* a, const void* b);
NA_IAPI void   NA_KEY_OP(Add,          NAInt)(void* dst, const void* src1, const void* src2);

NA_IAPI void   NA_KEY_OP(Assign,       NAPos)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Lower,        NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LowerEqual,   NAPos)(const void* a, const void* b);
NA_IAPI void   NA_KEY_OP(Add,          NAPos)(void* dst, const void* src1, const void* src2);

NA_IAPI void   NA_KEY_OP(Assign,       NAVertex)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Lower,        NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LowerEqual,   NAVertex)(const void* a, const void* b);
NA_IAPI void   NA_KEY_OP(Add,          NAVertex)(void* dst, const void* src1, const void* src2);


#include "NACore/NAKeyII.h"


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_KEY_INCLUDED



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
