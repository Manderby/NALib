
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



#include "../../NAUtility/NAString.h"

NSString* naBufferSnippletType = @"nalib.nabuffer.snipplet";

@interface NABufferSnipplet : NSObject <NSPasteboardWriting, NSPasteboardReading> {
  NABuffer* buffer;
}
@end



@implementation NABufferSnipplet
// Yes, it is complicated. Praise the OOP enthusiasts of the early 2000s!

- (id)initWithBuffer:(const NABuffer*)newBuffer{
  self = [super init];
  buffer = naCreateBufferCopy(newBuffer, naGetBufferRange(newBuffer), NA_FALSE);
  return self;
}

- (void)dealloc{
  naRelease(buffer);
  NA_COCOA_SUPER_DEALLOC();
}

- (NABuffer*)getBuffer{
  return buffer;
}

// Copy TO pasteboard. First is definite, others are promised.
- (NSArray *)writableTypesForPasteboard:(NSPasteboard *)pasteboard{
  NA_UNUSED(pasteboard);
  return [NSArray arrayWithObjects:naBufferSnippletType, nil];
}

// Paste FROM pasteboard. What is accepted as a constructor input
+ (NSArray *)readableTypesForPasteboard:(NSPasteboard *)pasteboard{
  NA_UNUSED(pasteboard);
  return [NSArray arrayWithObjects:naBufferSnippletType, NSPasteboardTypeString, nil];
}

// Defines, how the pasteboarddata parameter of initWithPasteboardPropertyList
// is encoded
+ (NSPasteboardReadingOptions)readingOptionsForType:(NSString *)type pasteboard:(NSPasteboard *)pasteboard{
  NA_UNUSED(pasteboard);
  if([type isEqualToString:naBufferSnippletType]) {
    return NSPasteboardReadingAsData;
  }else{
    return NSPasteboardReadingAsPropertyList;
  }
}

- (id)initWithPasteboardPropertyList:(id)pasteboarddata ofType:(NSString *)type{
  if([type isEqualToString:naBufferSnippletType]) {
    const NAByte* buf = [(NSData*)pasteboarddata bytes];
    buffer = naCreateBufferWithConstData(buf, (size_t)[(NSData*)pasteboarddata length]);
    return self;
  }
  return nil;
}

- (id)pasteboardPropertyListForType:(NSString *)type{
  if([type isEqualToString:naBufferSnippletType]) {
    size_t bufSize = (size_t)naGetBufferRange(buffer).length;
    NAByte* buf = naMalloc(bufSize);
    naWriteBufferToData(buffer, buf);
    
    NSData* thedata = [NSData dataWithBytesNoCopy:buf length:(NSUInteger)bufSize freeWhenDone:YES];
    return thedata;
  }
  return nil;
}

@end



void naBeginClipboard() {
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];
}

void naEndClipboard() {
  // nothing to do.
}



NA_DEF void naPutStringToClipboard(const NAString* string) {
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];

  [pasteboard writeObjects:
    [NSArray arrayWithObject:
      [NSString stringWithUTF8String:naGetStringUTF8Pointer(string)]]];
}

// Puts the given buffer on the systems clipboard as raw data.
NA_DEF void naPutBufferToClipboard(const NABuffer* buffer) {
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];

  NABufferSnipplet* snipplet = [[NABufferSnipplet alloc] initWithBuffer:buffer];
  [pasteboard writeObjects:[NSArray arrayWithObject:snipplet]];
}



NA_DEF NAString* naNewStringFromClipboard() {
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
  NAString* string = NA_NULL;
  NSArray *classes = [[NSArray alloc] initWithObjects:[NSString class], nil];
  NSDictionary *options = [NSDictionary dictionary];
  NSArray *copiedItems = [pasteboard readObjectsForClasses:classes options:options];
  if ([copiedItems count]) {
    NSString* nsString = [copiedItems objectAtIndex:0];
    string = naNewStringWithFormat("%s", [nsString UTF8String]);
  }else{
    string = naNewString();
  }
  NA_COCOA_RELEASE(classes);
  return string;
}

NA_DEF NABuffer* naCreateBufferFromClipboard() {
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
  NSArray *classes = [[NSArray alloc] initWithObjects:[NABufferSnipplet class], [NSString class], nil];
  NSDictionary *options = [NSDictionary dictionary];
  NSArray *copiedItems = [pasteboard readObjectsForClasses:classes options:options];
  NA_COCOA_RELEASE(classes);

  if ([copiedItems count]) {
    NABufferSnipplet* pastedSnipplet = (NABufferSnipplet*)[copiedItems objectAtIndex:0];
    return naCreateBufferCopy([pastedSnipplet getBuffer], naGetBufferRange([pastedSnipplet getBuffer]), NA_FALSE);
  }
  return NA_NULL;
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
