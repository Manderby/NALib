
#include "../../NAMath/NAMathOperators.h"



// Simplified conversion from and to perceyved space uses the following
// formulas:   y = .25x / (1 - .75x)   and   y = x / (.75x + .25)
// Both formulas try to anneal the Lab L* response curve standard, hence are
// visually pleasing but also have no numerical problems around 0 and 1.
// And they are very fast to compute.



// This factor is nice and easy to remember. There is no greater mathematical
// purpose in this number. It's just nice.
#define NA_ABY_FACTOR 0.75f

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
  outColor->r = naConvertToPerceptualColorValue(inColor->r);
  outColor->g = naConvertToPerceptualColorValue(inColor->g);
  outColor->b = naConvertToPerceptualColorValue(inColor->b);
//  outColor->a = inColor->a;
//  outColor->b = inColor->b;
//  outColor->y = inColor->y;
}

NA_HIDEF void na_ConvertToRadiometricRGB(NAColor* outColor, const NAColor* inColor) {
//  outColor->a = naPowf(inColor->a, 2.2f);
//  outColor->b = naPowf(inColor->b, 2.2f);
//  outColor->y = naPowf(inColor->y, 2.2f);
  outColor->r = naConvertToRadiometricColorValue(inColor->r);
  outColor->g = naConvertToRadiometricColorValue(inColor->g);
  outColor->b = naConvertToRadiometricColorValue(inColor->b);
//  outColor->a = inColor->a;
//  outColor->b = inColor->b;
//  outColor->y = inColor->y;
}
