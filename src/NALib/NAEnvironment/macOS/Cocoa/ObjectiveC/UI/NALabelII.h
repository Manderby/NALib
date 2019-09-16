
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



@implementation MDVerticallyCenteredTextFieldCell

- (NSRect)adjustedFrameToVerticallyCenterText:(NSRect)rect {
//  static int blah = 0;
//    CGFloat fontSize = self.font.boundingRectForFont.size.height;
//    NSRect boundRect = [[self font] boundingRectForFont];
//    CGFloat ascender = [[self font] ascender];
//    CGFloat capHeight = [[self font] capHeight];
//    CGFloat descender = [[self font] descender];
//    CGFloat xHeight = [[self font] xHeight];
//    CGFloat test = fontSize - ascender + descender;
//    CGFloat offset = 15 - (fontSize + (fontSize + boundRect.origin.y - ascender + descender));
//    CGFloat offset = 18 - (floor(fontSize) + floor(boundRect.origin.y));
    CGFloat offset = 0;
    return NSMakeRect(rect.origin.x, offset, rect.size.width, rect.size.height - offset);
//    return NSMakeRect(rect.origin.x, 15 - (fontSize + boundRect.origin.y), rect.size.width, fontSize);
}
- (void)editWithFrame:(NSRect)aRect inView:(NSView *)controlView
         editor:(NSText *)editor delegate:(id)delegate event:(NSEvent *)event {
    [super editWithFrame:[self adjustedFrameToVerticallyCenterText:aRect]
          inView:controlView editor:editor delegate:delegate event:event];
}

- (void)selectWithFrame:(NSRect)aRect inView:(NSView *)controlView
                 editor:(NSText *)editor delegate:(id)delegate 
                  start:(NSInteger)start length:(NSInteger)length {

    [super selectWithFrame:[self adjustedFrameToVerticallyCenterText:aRect]
                    inView:controlView editor:editor delegate:delegate
                     start:start length:length];
}

//- (void)drawInteriorWithFrame:(NSRect)frame inView:(NSView *)view {
//    [super drawInteriorWithFrame:[self adjustedFrameToVerticallyCenterText:frame] inView:view];
//}
- (void)drawWithFrame:(NSRect)frame inView:(NSView *)view {
//    CGFontRef cgfont = CTFontCopyGraphicsFont([self font], nil);
//  CGFloat cgAscent = CGFontGetAscent(cgfont);

//  NSLayoutManager* layout = [[NSLayoutManager alloc] init];
//  CGFloat defaultLineHeight = [layout defaultLineHeightForFont:[self font]];
//  CGFloat baselineOffset = [layout defaultBaselineOffsetForFont:[self font]];
//
//    NSRect titleRect = [self titleRectForBounds:frame];
//    CGFloat fontHeight = self.font.boundingRectForFont.size.height;
//    NSRect boundRect = [[self font] boundingRectForFont];
//    CGFloat origin = boundRect.origin.y;
//    CGFloat ascender = ([[self font] ascender]);
//    CGFloat capHeight = [[self font] capHeight];
//    CGFloat descender = ([[self font] descender]);
//    CGFloat xHeight = [[self font] xHeight];
//    CGFloat underlinePos = [[self font] underlinePosition];
//    CGFloat leading = [[self font] leading];
//    NSRect glyphrect = [[self font] boundingRectForCGGlyph:'x'];
//    const CGFloat* matrix = [[self font] matrix];
//CGFloat baseline = ceil(NSMinY(titleRect) + [[self font] ascender]);
//    
//    CGFloat testleading = leading;
//    if(testleading < 0){testleading = 0;}
//    testleading = floor(testleading + .5);
//    
//    CGFloat testlineheight = floor(ascender + .5) - ceil(descender - .5) + leading;
//    CGFloat testDelta = 0;
//    if(leading <= 0){
//      testDelta = floor (0.2 * testlineheight + 0.5);
//    }
//    
//    CGFloat test = defaultLineHeight;
//
//    printf("%f\n", baselineOffset);
//
//    [[NSColor yellowColor] setFill];
//    boundRect.origin.y = 0;
////    boundRect.size.height = defaultLineHeight;
//    boundRect.size.width += 20;
//    NSRectFill(boundRect);
//    boundRect.size.width -= 20;
//
//    [[NSColor orangeColor] setFill];
//    boundRect.origin.y = 0;
//    boundRect.size.height = ascender;
//    NSRectFill(boundRect);
//
//    [[NSColor redColor] setFill];
//    boundRect.origin.y = ascender;
//    boundRect.size.height = -descender;
//    NSRectFill(boundRect);
//
//    [[NSColor orangeColor] setFill];
//    boundRect.origin.y = test;
//    boundRect.size.height = ascender;
//    NSRectFill(boundRect);
//
//    [[NSColor redColor] setFill];
//    boundRect.origin.y = test + ascender;
//    boundRect.size.height = -descender;
//    NSRectFill(boundRect);
    
    [super drawWithFrame:[self adjustedFrameToVerticallyCenterText:frame] inView:view];
}

@end

@implementation NANativeLabel
- (id) initWithCoreLabel:(NACoreLabel*)newcorelabel frame:(NSRect)frame{
  self = [super initWithFrame:frame];
//  [self setCell:[[MDVerticallyCenteredTextFieldCell alloc] initTextCell:@"Wurst"]];
  [self setSelectable:YES];
  [self setEditable:NO];
  [self setBordered:NO];
  [self setBackgroundColor:[NSColor colorWithCalibratedRed:0. green:0. blue:1. alpha:.1]];
  [self setDrawsBackground:NO];
  [self setLineBreakMode:NSLineBreakByWordWrapping];
  [self setFont:[NSFont labelFontOfSize:[NSFont systemFontSize]]];
  corelabel = newcorelabel;
  return self;
}
- (void) setText:(const NAUTF8Char*)text{
  [self setStringValue:[NSString stringWithUTF8String:text]];
}
- (void) setLink:(const NAUTF8Char*)url{
  [self setAllowsEditingTextAttributes: YES];
  [self setSelectable: YES];

  NSMutableAttributedString* attrString = NA_COCOA_AUTORELEASE([[NSMutableAttributedString alloc] initWithString: [self stringValue]]);
  NSRange range = NSMakeRange(0, [attrString length]);

  NSURL* nsurl = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
  [attrString beginEditing];
  [attrString addAttribute:NSLinkAttributeName value:[nsurl absoluteString] range:range];
  NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
  [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
  paragraphStyle.alignment = [self alignment];
  [attrString addAttribute:NSParagraphStyleAttributeName value:paragraphStyle range:range];
  [attrString addAttribute:NSUnderlineStyleAttributeName value:[NSNumber numberWithInt:NSUnderlineStyleSingle] range:range];
  [attrString addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:range];
  [attrString endEditing];
  
  [self setAttributedStringValue: attrString];
}
- (void) setLabelEnabled:(NABool)enabled{
  [self setAlphaValue:enabled ? 1. : .35];
}
- (void) setTextAlignment:(NATextAlignment) alignment{
  [self setAlignment:getNSTextAlignmentWithAlignment(alignment)];
}
- (void) setFontKind:(NAFontKind)kind{
  [self setFont:getNSFontWithKind(kind)];
}
@end



NA_DEF NALabel* naNewLabel(const NAUTF8Char* text, NARect rect){
  NACoreLabel* corelabel = naAlloc(NACoreLabel);

  NANativeLabel* nativeLabel = [[NANativeLabel alloc] initWithCoreLabel:corelabel frame:naMakeNSRectWithRect(rect)];
  naInitCoreLabel(corelabel, NA_COCOA_TAKE_OWNERSHIP(nativeLabel));
  naSetLabelText(corelabel, text);
  
  return (NALabel*)corelabel;
}



NA_DEF void naDestructLabel(NALabel* label){
  NACoreLabel* corelabel = (NACoreLabel*)label;
  naClearCoreLabel(corelabel);
}



NA_DEF void naSetLabelText(NALabel* label, const NAUTF8Char* text){
  naDefineNativeCocoaObject(NANativeLabel, nativelabel, label);
  [nativelabel setText:text];
}



NA_DEF void naSetLabelLink(NALabel* label, const NAUTF8Char* url){
  naDefineNativeCocoaObject(NANativeLabel, nativelabel, label);
  [nativelabel setLink: url];
}



NA_DEF void naSetLabelEnabled(NALabel* label, NABool enabled){
  naDefineNativeCocoaObject(NANativeLabel, nativelabel, label);
  [nativelabel setLabelEnabled:enabled];
}



NA_DEF void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment){
  naDefineNativeCocoaObject(NANativeLabel, nativelabel, label);
  [nativelabel setTextAlignment:alignment];
}



NA_DEF void naSetLabelFontKind(NALabel* label, NAFontKind kind){
  naDefineNativeCocoaObject(NANativeLabel, nativelabel, label);
  [nativelabel setFontKind:kind];
}



NA_HDEF NARect naGetLabelAbsoluteInnerRect(NACoreUIElement* label){
  NA_UNUSED(label);
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
