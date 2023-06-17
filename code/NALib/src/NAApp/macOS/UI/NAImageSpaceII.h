
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation NACocoaNativeImageSpace

- (id) initWithImageSpace:(NACocoaImageSpace*)newCocoaImageSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  cocoaImageSpace = newCocoaImageSpace;
  return self;
}

- (void) setUIImage:(NAUIImage*)uiImage{
  NSImage* image = naCreateResolutionIndependentNativeImage(
    self,
    uiImage,
    NA_UIIMAGE_INTERACTION_NONE);
  [self setImage:image];
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}
@end



NA_DEF NAImageSpace* naNewImageSpace(NAUIImage* uiImage, NASize size){
  NACocoaImageSpace* cocoaImageSpace = naNew(NACocoaImageSpace);

  NACocoaNativeImageSpace* nativePtr = [[NACocoaNativeImageSpace alloc]
    initWithImageSpace:cocoaImageSpace
    frame:naMakeNSRectWithSize(size)];
  na_InitImageSpace((NAImageSpace*)cocoaImageSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));
  
  cocoaImageSpace->imageSpace.uiImage = naRetain(uiImage);
  [nativePtr setUIImage: uiImage];
  
  return (NAImageSpace*)cocoaImageSpace;
}



NA_DEF void na_DestructCocoaImageSpace(NACocoaImageSpace* cocoaImageSpace){
  naRelease(cocoaImageSpace->imageSpace.uiImage);
  na_ClearImageSpace((NAImageSpace*)cocoaImageSpace);
}



NA_DEF void naSetImageSpaceImage(NAImageSpace* imageSpace, NAUIImage* uiImage){
  naDefineCocoaObjectConst(NACocoaNativeImageSpace, nativePtr, imageSpace);
  
  naRelease(imageSpace->uiImage);
  imageSpace->uiImage = naRetain(uiImage);
  [nativePtr setUIImage: uiImage];
}



NA_HDEF NARect na_GetImageSpaceRect(const NA_UIElement* imageSpace){
  naDefineCocoaObjectConst(NACocoaNativeImageSpace, nativePtr, imageSpace);
  return naMakeRectWithNSRect([nativePtr frame]);
}

NA_HDEF void na_SetImageSpaceRect(NA_UIElement* imageSpace, NARect rect){
  naDefineCocoaObjectConst(NACocoaNativeImageSpace, nativePtr, imageSpace);
  [nativePtr setFrame:naMakeNSRectWithRect(rect)];
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
