
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_MONEY_INCLUDED
#define NA_MONEY_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAString.h"

typedef struct NAMoney NAMoney;

typedef enum{
  NA_CURRENCY_CHF = 0,
  NA_CURRENCY_COUNT
} NACurrency;


NA_IAPI NAMoney naMakeMoneyWithCurrency(double amount, NACurrency currency);

NA_IAPI NAMoney naNegMoney(NAMoney money);
NA_IAPI NAMoney naAddMoney(NAMoney moneyA, NAMoney moneyB);
NA_IAPI NAMoney naSubMoney(NAMoney moneyA, NAMoney moneyB);

NA_IAPI NAString* naNewStringWithMoney(NAMoney money);




// Inline implementations are in a separate file:
#include "NAMath/NAMoneyII.h"






#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_MONEY_INCLUDED


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
