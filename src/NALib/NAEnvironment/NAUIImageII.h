
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations of the file NAUIImage
// Do not include this file directly! It will automatically be included when
// including "NAUIImage.h"


struct NAUIImage {
  NASizei size1x;
  NABlendMode tintMode;
  void* imgref[NA_UIIMAGE_RESOLUTION_COUNT * NA_UIIMAGE_KIND_COUNT * NA_UIIMAGE_SKIN_COUNT];
};


NA_HIAPI void* naGetUIImageRef(NAUIImage* image, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin);
NA_HIAPI void naSetUIImageRef(NAUIImage* image, void* imgref, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin);






NA_IDEF NASizei naGetUIImage1xSize(const NAUIImage* image){
  return image->size1x;
}





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


