
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Rotation Matrix
// ///////////////////////////////

NA_IDEF void naFillM33dWithRotationAlongAxis(double* NA_RESTRICT d, const double* NA_RESTRICT axis, double angle) {
  double sinang    = naSin(.5 * angle);
  double sinang2   = sinang * sinang;
  double sinangT2  = 2. * sinang;
  double cosang    = naCos(.5 * angle);
  double ax0       = axis[0] * cosang;
  double ax00      = axis[0] * axis[0];
  double ax01      = axis[0] * axis[1] * sinang;
  double ax02      = axis[0] * axis[2] * sinang;
  double ax1       = axis[1] * cosang;
  double ax11      = axis[1] * axis[1];
  double ax12      = axis[1] * axis[2] * sinang;
  double ax2       = axis[2] * cosang;
  double ax22      = axis[2] * axis[2];

  naFillM33dColumnFirst(d,
          1. + sinang2 * (ax00 - ax11 - ax22 - 1.),
          sinangT2 * (ax01 + ax2),
          sinangT2 * (ax02 - ax1),
          sinangT2 * (ax01 - ax2),
          1. + sinang2 * (-ax00 + ax11 - ax22 - 1.),
          sinangT2 * (ax12 + ax0),
          sinangT2 * (ax02 + ax1),
          sinangT2 * (ax12 - ax0),
          1. + sinang2 * (-ax00 - ax11 + ax22 - 1.));
}


NA_IDEF void naFillM33fWithRotationAlongAxis(float* NA_RESTRICT d, const float* NA_RESTRICT axis, float angle) {
  float sinang    = naSinf(.5f * angle);
  float sinang2   = sinang * sinang;
  float sinangT2  = 2.f * sinang;
  float cosang    = naCosf(.5f * angle);
  float ax0       = axis[0] * cosang;
  float ax00      = axis[0] * axis[0];
  float ax01      = axis[0] * axis[1] * sinang;
  float ax02      = axis[0] * axis[2] * sinang;
  float ax1       = axis[1] * cosang;
  float ax11      = axis[1] * axis[1];
  float ax12      = axis[1] * axis[2] * sinang;
  float ax2       = axis[2] * cosang;
  float ax22      = axis[2] * axis[2];

  naFillM33fColumnFirst(d,
          1.f + sinang2 * (ax00 - ax11 - ax22 - 1.f),
          sinangT2 * (ax01 + ax2),
          sinangT2 * (ax02 - ax1),
          sinangT2 * (ax01 - ax2),
          1.f + sinang2 * (-ax00 + ax11 - ax22 - 1.f),
          sinangT2 * (ax12 + ax0),
          sinangT2 * (ax02 + ax1),
          sinangT2 * (ax12 - ax0),
          1.f + sinang2 * (-ax00 - ax11 + ax22 - 1.f));
}



NA_IDEF void naFillM33dWithRotationAngles(double* d, double roll, double pitch, double yaw) {
  double q1 = - naCos(.5 * (roll - yaw)) * naSin(.5 * pitch);
  double q2 = naSin(.5 * (roll - yaw)) * naSin(.5 * pitch);
  double q3 = - naSin(.5 * (roll + yaw)) * naCos(.5 * pitch);
  double q4 = naCos(.5 * (roll + yaw)) * naCos(.5 * pitch);

  naFillM33dColumnFirst(d,
          1. - 2. * ( q2*q2 - q3*q3),
          2. * (q1*q2 + q3*q4),
          2. * (q1*q3 - q2*q4),
          2. * (q1*q2 - q3*q4),
          1. - 2. * ( q1*q1 - q3*q3),
          2. * (q1*q4 + q2*q3),
          2. * (q1*q3 + q2*q4),
          2. * (q2*q3 - q1*q4),
          1. - 2. * ( q1*q1 - q2*q2));
}

NA_IDEF void naFillM33fWithRotationAngles(float* d, float roll, float pitch, float yaw) {
  float q1 = - naCosf(.5f * (roll - yaw)) * naSinf(.5f * pitch);
  float q2 = naSinf(.5f * (roll - yaw)) * naSinf(.5f * pitch);
  float q3 = - naSinf(.5f * (roll + yaw)) * naCosf(.5f * pitch);
  float q4 = naCosf(.5f * (roll + yaw)) * naCosf(.5f * pitch);

  naFillM33fColumnFirst(d,
          1.f - 2.f * ( q2*q2 - q3*q3),
          2.f * (q1*q2 + q3*q4),
          2.f * (q1*q3 - q2*q4),
          2.f * (q1*q2 - q3*q4),
          1.f - 2.f * ( q1*q1 - q3*q3),
          2.f * (q1*q4 + q2*q3),
          2.f * (q1*q3 + q2*q4),
          2.f * (q2*q3 - q1*q4),
          1.f - 2.f * ( q1*q1 - q2*q2));
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
