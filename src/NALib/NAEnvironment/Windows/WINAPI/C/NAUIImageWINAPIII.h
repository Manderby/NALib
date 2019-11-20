
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../../NAUIImage.h"
#include "../../../NAPNG.h"
#include <wincodec.h>



NABabyImage* naCreateBabyImageFromNativeImage(const void* nativeimage){
  HDC hdcSource = GetDC(NA_NULL); // the source device context
  HBITMAP hSource = (HBITMAP)nativeimage; // the bitmap selected into the device context

  BITMAPINFO MyBMInfo = {0};
  MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

  // Get the BITMAPINFO structure from the bitmap
  GetDIBits(hdcSource, hSource, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS);

  // create the pixel buffer
  BYTE* lpPixels = naMalloc(MyBMInfo.bmiHeader.biSizeImage);

  MyBMInfo.bmiHeader.biBitCount = 32;
  MyBMInfo.bmiHeader.biCompression = BI_RGB;  // no compression -> easier to use

  // Call GetDIBits a second time, this time to (format and) store the actual
  // bitmap data (the "pixels") in the buffer lpPixels
  GetDIBits(hdcSource, hSource, 0, MyBMInfo.bmiHeader.biHeight, lpPixels, &MyBMInfo, DIB_RGB_COLORS);

  NABabyImage* babyimage = naCreateBabyImage(naMakeSizei(MyBMInfo.bmiHeader.biWidth, MyBMInfo.bmiHeader.biHeight), NA_NULL);
  // Windows does store an alpha component but it is not in use and therefore zero.
  // We therefore ignore it in the following call and receyve a completely opaque image.
  naFillBabyImageWithUInt8(babyimage, lpPixels, NA_FALSE, NA_COLOR_BUFFER_BGR0);
  naFree(lpPixels);

  // clean up: deselect bitmap from device context, close handles, delete buffer
  return babyimage;
}



NABabyImage* naCreateBabyImageFromFilePath(const NAUTF8Char* pathStr){
  // Currently, only png is possible
  NAPNG* png = naNewPNGWithFile(pathStr);
  NABabyImage* babyImage = naCreateBabyImageFromPNG(png);
  return babyImage;
}



NA_DEF void* naAllocNativeImageWithBabyImage(const NABabyImage* image){

  NASizei size = naGetBabyImageSize(image);
  NAByte* buffer = naMalloc(size.width * size.height * 4);
  float* babyptr = naGetBabyImageData(image);

  for(int y = 0; y < size.height; y++){
    NAByte* bufptr = &(buffer[(size.height - y - 1) * (size.width * 4)]);
    for(int x = 0; x < size.width * 4; x += 4){
      bufptr[0] = (NAByte)(babyptr[2] * 255.f);
      bufptr[1] = (NAByte)(babyptr[1] * 255.f);
      bufptr[2] = (NAByte)(babyptr[0] * 255.f);
      bufptr[3] = (NAByte)(babyptr[3] * 255.f);
      babyptr += 4;
      bufptr += 4;
    }
  }

  HBITMAP hNewBitmap = CreateBitmap((int)size.width, (int)size.height, 1, 32, buffer);
  naFree(buffer);

  return hNewBitmap;
}



//void* naAllocNativeImageWithUIImage(NAUIImage* uiimage, NAUIImageKind kind, NAUIImageSkin skin){
//  //NASizei imagesize = naGetUIImage1xSize(uiimage);
//  //NSImage* image = [[NSImage alloc] initWithSize:NSMakeSize(imagesize.width, imagesize.height)];
//
//  //CGImageRef img1x = naGetUIImageRef(uiimage, NA_UIIMAGE_RESOLUTION_1x, kind, skin);
//  //CGImageRef img2x = naGetUIImageRef(uiimage, NA_UIIMAGE_RESOLUTION_2x, kind, skin);
//  //if(img1x){[image addRepresentation:NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img1x])];}
//  //if(img2x){[image addRepresentation:NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img2x])];}
//  //
//  //return NA_COCOA_TAKE_OWNERSHIP(image);
//  return NA_NULL;
//}



void naDeallocNativeImage(void* nativeimage){
  DeleteObject(nativeimage);
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
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
