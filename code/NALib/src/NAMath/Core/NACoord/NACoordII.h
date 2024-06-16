
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"




// /////////////////////////////////////|
// Coordinate functions are not especially difficult to understand or implement
// but take a lot of code writing. You are free to explore all the inline code
// underneath the API but there really isn't much to see.
// /////////////////////////////////////



#include "../../NAMathOperators.h"
#include "../../../NAUtility/NAValueHelper.h"


#include "NABorderII.h"
#include "NABoxII.h"
#include "NABoxCombinationII.h"
#include "NAContainsTesterII.h"
#include "NACoordAlignII.h"
#include "NACoordArithmeticII.h"
#include "NACoordCalculationII.h"
#include "NACoordClampingII.h"
#include "NACoordComparisonII.h"
#include "NACoordEndMaxII.h"
#include "NACoordIndexII.h"
#include "NACoordMacOSII.h"
#include "NACoordTesterII.h"
#include "NAPosII.h"
#include "NARangeII.h"
#include "NARangeCombinationII.h"
#include "NARectII.h"
#include "NARectCombinationII.h"
#include "NASizeII.h"
#include "NAVertexII.h"
#include "NAVolumeII.h"



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
