
#ifndef NA_COLOR_DEFINITIONS_INCLUDED
#define NA_COLOR_DEFINITIONS_INCLUDED


typedef enum {
    NA_COLOR_BUFFER_RGBA,
    NA_COLOR_BUFFER_RGBAPre,
    NA_COLOR_BUFFER_RGB,
    NA_COLOR_BUFFER_BGR0,
    NA_COLOR_BUFFER_BGRA
} NAColorBufferType;



// Note that BLACK_GREEN and WHITE_GREEN only work for opaque images.
typedef enum {
    NA_BLEND_ZERO,          // Does not blend at all. The base remains as it is.
    NA_BLEND_LINEAR,        // Linear interpolation according blend factor.
    NA_BLEND_OVERLAY,       // Blends where top is opaque.
    NA_BLEND_OPAQUE,        // Blends where base is opaque.
    NA_BLEND_MULTIPLY,      // Blends where base is opaque by multiplying the color.
    NA_BLEND_SCREEN,        // Blends where base is opaque by inverse multiplying the color.
    NA_BLEND_ERODE_LIGHT,   // Same as screen but makes light pixels of base transparent.
    // Useful for icon creation.
    NA_BLEND_ERODE_DARK,    // Same as multiply but makes dark pixels of base transparent.
    // Useful for icon creation.
    NA_BLEND_ERASE_HUE      // Decolorizes and makes base transparent based on top hue.
                            // Useful for green-screening or blue-screening.
} NABlendMode;



#endif // NA_COLOR_DEFINITIONS_INCLUDED



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
