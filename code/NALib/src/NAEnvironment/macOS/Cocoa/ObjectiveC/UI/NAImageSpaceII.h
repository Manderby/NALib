
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


@implementation NACocoaImageSpace
- (id) initWithImageSpace:(NAImageSpace*)newImageSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  imageSpace = newImageSpace;
  return self;
}
- (void) setUIImage:(NAUIImage*)newuiImage{
  NSImage* image;
  uiImage = newuiImage;
  image = NA_COCOA_PTR_C_TO_OBJC(naAllocNativeImageWithUIImage(uiImage, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN));
//  NSURL* url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:imagePath]];
//  NSImage* image = NA_COCOA_AUTORELEASE([[NSImage alloc] initWithContentsOfURL:url]);
  [self setImage:image];
}
@end



NA_DEF NAImageSpace* naNewImageSpace(NAUIImage* uiImage, NASize size){
  NAImageSpace* imageSpace = naAlloc(NAImageSpace);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaImageSpace* cocoaImageSpace = [[NACocoaImageSpace alloc] initWithImageSpace:imageSpace frame:frameRect];  
  na_InitImageSpace(imageSpace, NA_COCOA_PTR_OBJC_TO_C(cocoaImageSpace));

  [cocoaImageSpace setUIImage: uiImage];
  
  return imageSpace;
}



NA_DEF void na_DestructImageSpace(NAImageSpace* imageSpace){
  na_ClearImageSpace(imageSpace);
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
