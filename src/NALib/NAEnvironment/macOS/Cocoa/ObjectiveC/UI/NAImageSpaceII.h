
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


@implementation NANativeImageSpace
- (id) initWithCocoaImageSpace:(NACocoaImageSpace*)newcocoaimagespace frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  cocoaimagespace = newcocoaimagespace;
  return self;
}
- (void) setImageURL:(const NAUTF8Char*)imagePath{
  NSURL* url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:imagePath]];
  NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
  [self setImage:image];
  [image release];
}
@end



NA_DEF NAImageSpace* naNewImageSpace(NARect rect){
  NACocoaImageSpace* cocoaImageSpace = naAlloc(NACocoaImageSpace);

  NSRect contentRect = naMakeNSRectWithRect(rect);
  NANativeImageSpace* nativeImageSpace = [[NANativeImageSpace alloc] initWithCocoaImageSpace:cocoaImageSpace frame:contentRect];  
  naRegisterCoreUIElement(&(cocoaImageSpace->coreimagespace.uielement), NA_UI_IMAGESPACE, (void*)NA_COCOA_RETAIN(nativeImageSpace));
  
  return (NAImageSpace*)cocoaImageSpace;
}



NA_DEF void naDestructImageSpace(NAImageSpace* imagespace){
  NACocoaImageSpace* cocoaimagespace = (NACocoaImageSpace*)imagespace;
  NA_COCOA_RELEASE(naUnregisterCoreUIElement(&(cocoaimagespace->coreimagespace.uielement)));
}



NA_DEF void naSetImageSpacePath(NAImageSpace* imagespace, const NAUTF8Char* imagePath){
  [((NA_COCOA_BRIDGE NANativeImageSpace*)naGetUIElementNativeID(imagespace)) setImageURL:imagePath];
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
