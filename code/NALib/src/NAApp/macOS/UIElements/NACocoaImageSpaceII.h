
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



struct NACocoaImageSpace{
  NAImageSpace imageSpace;
};
NA_HAPI void na_DestructCocoaImageSpace(NACocoaImageSpace* _Nonnull cocoaImageSpace);
NA_RUNTIME_TYPE(NACocoaImageSpace, na_DestructCocoaImageSpace, NA_FALSE);



@implementation NACocoaNativeImageSpace

- (id _Nonnull) initWithImageSpace:(NACocoaImageSpace* _Nonnull)newCocoaImageSpace frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  cocoaImageSpace = newCocoaImageSpace;
  return self;
}

- (void) setImageSet:(NAImageSet* _Nullable)imageSet{
  if(imageSet) {
    double uiScale = naGetUIElementUIScale(&cocoaImageSpace->imageSpace);
    NSImage* image = na_CreateResolutionIndependentNativeImage(
      imageSet,
      NA_IMAGE_SET_INTERACTION_NONE,
      NA_FALSE,
      uiScale);
    [self setImage:image];
  }else{
    [self setImage:nil];
  }
}

- (void)mouseMoved:(NSEvent* _Nonnull)event{
  na_SetMouseMovedTo(
    na_GetApplicationMouseStatus(naGetApplication()),
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaImageSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
    [super mouseMoved:event];
  }
}

- (void)mouseDragged:(NSEvent* _Nonnull)event{
  na_SetMouseMovedTo(
    na_GetApplicationMouseStatus(naGetApplication()),
    naMakePosWithNSPoint([NSEvent mouseLocation]));
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaImageSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
    [super mouseDragged:event];
  }
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}
@end



NA_DEF NAImageSpace* _Nonnull naNewImageSpace(NAImageSet* _Nullable imageSet, NASize size) {
  NACocoaImageSpace* cocoaImageSpace = naNew(NACocoaImageSpace);

  NACocoaNativeImageSpace* nativePtr = [[NACocoaNativeImageSpace alloc]
    initWithImageSpace:cocoaImageSpace
    frame:naMakeNSRectWithSize(size)];
  na_InitImageSpace((NAImageSpace*)cocoaImageSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));
  
  if(imageSet) {
    cocoaImageSpace->imageSpace.imageSet = naRetain(imageSet);
  }else{
    cocoaImageSpace->imageSpace.imageSet = NA_NULL;
  }
  [nativePtr setImageSet: imageSet];

  return (NAImageSpace*)cocoaImageSpace;
}



NA_DEF void na_DestructCocoaImageSpace(NACocoaImageSpace* _Nonnull cocoaImageSpace) {
  if(cocoaImageSpace->imageSpace.imageSet) {
    naRelease(cocoaImageSpace->imageSpace.imageSet);
  }
  na_ClearImageSpace((NAImageSpace*)cocoaImageSpace);
}



NA_DEF void naSetImageSpaceImage(NAImageSpace* _Nonnull imageSpace, NAImageSet* _Nullable imageSet) {
  naDefineCocoaObjectConst(NACocoaNativeImageSpace, nativePtr, imageSpace);
  
  if(imageSpace->imageSet) {
    naRelease(imageSpace->imageSet);
  }
  if(imageSet) {
    imageSpace->imageSet = naRetain(imageSet);
  }else{
    imageSpace->imageSet = NA_NULL;
  }
  [nativePtr setImageSet: imageSet];
}



NA_HDEF NARect na_GetImageSpaceRect(const NA_UIElement* _Nonnull imageSpace) {
  naDefineCocoaObjectConst(NACocoaNativeImageSpace, nativePtr, imageSpace);
  return naMakeRectWithNSRect([nativePtr frame]);
}

NA_HDEF void na_SetImageSpaceRect(NA_UIElement* _Nonnull imageSpace, NARect rect) {
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
