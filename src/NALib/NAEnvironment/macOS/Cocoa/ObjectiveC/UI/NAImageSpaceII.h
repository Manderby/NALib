
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


@implementation NANativeImageSpace
- (id) initWithCoreImageSpace:(NACoreImageSpace*)newcoreimagespace frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  coreimagespace = newcoreimagespace;
  return self;
}
- (void) setImageURL:(const NAUTF8Char*)imagePath{
  NSURL* url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:imagePath]];
  NSImage* image = NA_COCOA_AUTORELEASE([[NSImage alloc] initWithContentsOfURL:url]);
  [self setImage:image];
}
@end



NA_DEF NAImageSpace* naNewImageSpace(NAUIImage* uiimage, NASize size){
  NACoreImageSpace* coreImageSpace = naAlloc(NACoreImageSpace);

  NSRect frameRect = NSMakeRect(0., 0., size.width, size.height);
  NANativeImageSpace* nativeImageSpace = [[NANativeImageSpace alloc] initWithCoreImageSpace:coreImageSpace frame:frameRect];  
  naInitCoreImageSpace(coreImageSpace, NA_COCOA_TAKE_OWNERSHIP(nativeImageSpace));
  
  return (NAImageSpace*)coreImageSpace;
}



NA_DEF void naDestructImageSpace(NAImageSpace* imagespace){
  NACoreImageSpace* coreimagespace = (NACoreImageSpace*)imagespace;
  naClearCoreImageSpace(coreimagespace);
}



NA_HDEF NARect naGetImageSpaceAbsoluteInnerRect(NACoreUIElement* imagespace){
  NA_UNUSED(imagespace);
  return naMakeRectS(20, 40, 100, 50);
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
