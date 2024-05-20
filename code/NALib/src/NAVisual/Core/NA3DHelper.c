
#include "../NA3DHelper.h"



NA_DEF void naFillMatrixPerspective(NAMat44d matrix, double fovy, double aspect, double nearZ, double farZ) {
  double cotan = naInv(naTan(naDegToRad(fovy) * .5));
  matrix[ 0] = cotan / aspect;
  matrix[ 1] = 0.;
  matrix[ 2] = 0.;
  matrix[ 3] = 0.;
  matrix[ 4] = 0.;
  matrix[ 5] = cotan;
  matrix[ 6] = 0.;
  matrix[ 7] = 0.;
  matrix[ 8] = 0.;
  matrix[ 9] = 0.;
  matrix[10] = (farZ + nearZ) / (nearZ - farZ);
  matrix[11] = -1.; // I like right handed systems.
  matrix[12] = 0.;
  matrix[13] = 0.;
  matrix[14] = (2. * farZ * nearZ) / (nearZ - farZ);
  matrix[15] = 0.;
}



NA_DEF void naFillMatrixOrtho(NAMat44d matrix, double left, double right, double bottom, double top, double nearZ, double farZ) {
  double ral = right + left;
  double rsl = right - left;
  double tab = top + bottom;
  double tsb = top - bottom;
  double fan = farZ + nearZ;
  double fsn = farZ - nearZ;
    
  matrix[ 0] = 2. / rsl;
  matrix[ 1] = 0.;
  matrix[ 2] = 0.;
  matrix[ 3] = 0.;
  matrix[ 4] = 0.;
  matrix[ 5] = 2. / tsb;
  matrix[ 6] = 0.;
  matrix[ 7] = 0.;
  matrix[ 8] = 0.;
  matrix[ 9] = 0.;
  matrix[10] = -2. / fsn;
  matrix[11] = 0.;
  matrix[12] = -ral / rsl;
  matrix[13] = -tab / tsb;
  matrix[14] = -fan / fsn;
  matrix[15] = 1.;
}



NA_DEF void naFillMatrixLookAt(NAMat44d matrix, double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ) {
  NAVec3d u;
  NAVec3d v;

  NAVec3d ev = { eyeX, eyeY, eyeZ };
  NAVec3d uv = { upX, upY, upZ };
  NAVec3d n = {eyeX - centerX, eyeY - centerY, eyeZ - centerZ};
  naNormalizeV3dS(n);
  naCrossV3d(u, uv, n);
  naNormalizeV3dS(u);
  naCrossV3d(v, n, u);
  
  matrix[ 0] = u[0];
  matrix[ 1] = v[0];
  matrix[ 2] = n[0];
  matrix[ 3] = 0.;
  matrix[ 4] = u[1];
  matrix[ 5] = v[1];
  matrix[ 6] = n[1];
  matrix[ 7] = 0.;
  matrix[ 8] = u[2];
  matrix[ 9] = v[2];
  matrix[10] = n[2];
  matrix[11] = 0.;
  naNegV3dS(u);
  naNegV3dS(v);
  naNegV3dS(n);
  matrix[12] = naDotV3d(u, ev);
  matrix[13] = naDotV3d(v, ev);
  matrix[14] = naDotV3d(n, ev);
  matrix[15] = 1.;
}



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
