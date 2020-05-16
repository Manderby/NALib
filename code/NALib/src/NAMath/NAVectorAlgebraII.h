
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebra.h"



// /////////////////////////////////////
// Warning: This is one huge collection!
//
// Vector algebra functions are not especially difficult to understand or
// implement but take a lot of code writing. Therefore it is split into 
// several files. You are free to explore all the inline code but there
// really isn't much to see.
// /////////////////////////////////////



#include "NARandom.h"
#include "NAMathOperators.h"

// ////////////////////////////////////////////
// Vectors
// ////////////////////////////////////////////

#include "NAVectorAlgebra/NAVectorFiller.h"
#include "NAVectorAlgebra/NAVectorBaseOperations.h"
#include "NAVectorAlgebra/NAVectorComponentwise.h"
#include "NAVectorAlgebra/NAVectorComparison.h"
#include "NAVectorAlgebra/NAVectorProducts.h"
#include "NAVectorAlgebra/NAVectorGeometry.h"

// ////////////////////////////////////////////
// Matrix, stored in COLUMN-FIRST order
// ////////////////////////////////////////////

#include "NAVectorAlgebra/NAMatrixFiller.h"
#include "NAVectorAlgebra/NAMatrixBaseOperations.h"
#include "NAVectorAlgebra/NAMatrixComponentwise.h"
#include "NAVectorAlgebra/NAMatrixMultiplication.h"
#include "NAVectorAlgebra/NAMatrixComparison.h"
#include "NAVectorAlgebra/NAHomogeneousCoordinates.h"
#include "NAVectorAlgebra/NAMatrixAlgebraOperations.h"
#include "NAVectorAlgebra/NAMatrixRotations.h"



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
