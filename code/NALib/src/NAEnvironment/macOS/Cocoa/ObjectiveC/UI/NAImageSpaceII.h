
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NACocoaImageSpace NACocoaImageSpace;
struct NACocoaImageSpace{
  NAImageSpace   imageSpace;
};

NA_HAPI void na_DestructCocoaImageSpace(NACocoaImageSpace* cocoaImageSpace);
NA_RUNTIME_TYPE(NACocoaImageSpace, na_DestructCocoaImageSpace, NA_FALSE);

@interface NACocoaNativeImageSpace : NSImageView{
  NACocoaImageSpace* cocoaImageSpace;
  NAUIImage* uiImage;
}
@end



@implementation NACocoaNativeImageSpace

- (id) initWithImageSpace:(NACocoaImageSpace*)newCocoaImageSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  cocoaImageSpace = newCocoaImageSpace;
  return self;
}

- (void) setUIImage:(NAUIImage*)newuiImage{
  uiImage = newuiImage;
  NSImage* image = NA_COCOA_PTR_C_TO_OBJC(naAllocNativeImageWithUIImage(
    uiImage,
    NA_UIIMAGE_KIND_MAIN,
    NA_UIIMAGE_SKIN_PLAIN));
  [self setImage:image];
}

@end



NA_DEF NAImageSpace* naNewImageSpace(NAUIImage* uiImage, NASize size){
  NACocoaImageSpace* cocoaImageSpace = naNew(NACocoaImageSpace);

  NACocoaNativeImageSpace* nativePtr = [[NACocoaNativeImageSpace alloc]
    initWithImageSpace:cocoaImageSpace
    frame:naMakeNSRectWithSize(size)];
  na_InitImageSpace((NAImageSpace*)cocoaImageSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

  [nativePtr setUIImage: uiImage];
  
  return (NAImageSpace*)cocoaImageSpace;
}



NA_DEF void na_DestructCocoaImageSpace(NACocoaImageSpace* cocoaImageSpace){
  na_ClearImageSpace((NAImageSpace*)cocoaImageSpace);
}



NA_HDEF NARect na_GetImageSpaceAbsoluteInnerRect(NA_UIElement* imageSpace){
  NA_UNUSED(imageSpace);
  return naMakeRectS(20, 40, 100, 50);
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
