
#include "../../NAMath/NAMathOperators.h"


// This factor is nice and easy to remember. There is no greater mathematical
// purpose in this number. It's just nice.
//#define NA_ABY_FACTOR (NA_SQRT2f / 2.f)
//#define NA_ABY_FACTOR 0.75f
#define NA_ABY_FACTOR 0.8f

NA_HIDEF float naConvertToPerceptualColorValue(float value) {
  return value / (NA_ABY_FACTOR * value + (1.f - NA_ABY_FACTOR));
}
NA_HIDEF float naConvertToRadiometricColorValue(float value) {
  return (1.f - NA_ABY_FACTOR) * value / (1.f - NA_ABY_FACTOR * value);
}



NA_HIDEF void na_ConvertToPerceptualRGB(NAColor* outColor, const NAColor* inColor) {
//  outColor->a = naPowf(inColor->a, 1.f / 2.2f);
//  outColor->b = naPowf(inColor->b, 1.f / 2.2f);
//  outColor->y = naPowf(inColor->y, 1.f / 2.2f);
  outColor->a = naConvertToPerceptualColorValue(inColor->a);
  outColor->b = naConvertToPerceptualColorValue(inColor->b);
  outColor->y = naConvertToPerceptualColorValue(inColor->y);
//  outColor->a = inColor->a;
//  outColor->b = inColor->b;
//  outColor->y = inColor->y;
}

NA_HIDEF void na_ConvertToRadiometricRGB(NAColor* outColor, const NAColor* inColor) {
//  outColor->a = naPowf(inColor->a, 2.2f);
//  outColor->b = naPowf(inColor->b, 2.2f);
//  outColor->y = naPowf(inColor->y, 2.2f);
  outColor->a = naConvertToRadiometricColorValue(inColor->a);
  outColor->b = naConvertToRadiometricColorValue(inColor->b);
  outColor->y = naConvertToRadiometricColorValue(inColor->y);
//  outColor->a = inColor->a;
//  outColor->b = inColor->b;
//  outColor->y = inColor->y;
}
