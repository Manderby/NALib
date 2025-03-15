
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NA_DEF NAUIColor* naAllocUIColor(const NAColor* color, const NAColor* bgColor) {
  NSColor* nsColor;

  if(bgColor) {
    NAColor blendedColor;
    naBlendColors(&blendedColor, bgColor, color, 1.f, NA_BLEND_OVERLAY, 1, NA_FALSE, NA_FALSE);
    
    nsColor = [NSColor colorWithDeviceRed: blendedColor.r
      green: blendedColor.g
      blue: blendedColor.b
      alpha: blendedColor.alpha];

  }else{
    nsColor = [NSColor colorWithDeviceRed: color->r
      green: color->g
      blue: color->b
      alpha: color->alpha];
  }

  return nsColor;
}

NA_DEF void naDeallocUIColor(NAUIColor* uiColor) {
  NA_UNUSED(uiColor);
//  NSColor* nsColor = (NSColor*)uiColor;
//  NA_COCOA_RELEASE(nsColor);
}



NA_DEF void naFillColorWithDefaultWindowBackgroundColor(NAColor* color) {
  NSColor* backColor = naGetWindowBackColor();
  naFillColorWithSRGB(
    color,
    (float)[backColor redComponent],
    (float)[backColor greenComponent],
    (float)[backColor blueComponent],
    1.f);
}



NA_DEF void naFillColorWithDefaultTextColor(NAColor* color) {
  NSColor* labelColor = naGetLabelColor();
  naFillColorWithSRGB(
    color,
    (float)[labelColor redComponent],
    (float)[labelColor greenComponent],
    (float)[labelColor blueComponent],
    1.f);
}



NA_DEF void naFillColorWithDefaultLinkColor(NAColor* color) {
  NSColor* linkColor = naGetLinkColor();
  naFillColorWithSRGB(
    color,
    (float)[linkColor redComponent],
    (float)[linkColor greenComponent],
    (float)[linkColor blueComponent],
    1.f);
}



NA_DEF void naFillColorWithDefaultAccentColor(NAColor* color) {
  NSColor* accentColor = naGetAccentColor();
  naFillColorWithSRGB(
    color,
    (float)[accentColor redComponent],
    (float)[accentColor greenComponent],
    (float)[accentColor blueComponent],
    1.f);
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
