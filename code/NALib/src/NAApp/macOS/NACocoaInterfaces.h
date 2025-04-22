
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


NA_PROTOTYPE(NACocoaApplication);
NA_EXTERN_RUNTIME_TYPE(NACocoaApplication);

NA_PROTOTYPE(NACocoaButton);
NA_EXTERN_RUNTIME_TYPE(NACocoaButton);

NA_PROTOTYPE(NACocoaCheckBox);
NA_EXTERN_RUNTIME_TYPE(NACocoaCheckBox);

NA_PROTOTYPE(NACocoaImageSpace);
NA_EXTERN_RUNTIME_TYPE(NACocoaImageSpace);

NA_PROTOTYPE(NACocoaLabel);
NA_EXTERN_RUNTIME_TYPE(NACocoaLabel);

NA_PROTOTYPE(NACocoaMenu);
NA_EXTERN_RUNTIME_TYPE(NACocoaMenu);

NA_PROTOTYPE(NACocoaMenuItem);
NA_EXTERN_RUNTIME_TYPE(NACocoaMenuItem);

NA_PROTOTYPE(NACocoaMetalSpace);
NA_EXTERN_RUNTIME_TYPE(NACocoaMetalSpace);

NA_PROTOTYPE(NACocoaOpenGLSpace);
NA_EXTERN_RUNTIME_TYPE(NACocoaOpenGLSpace);

NA_PROTOTYPE(NACocoaRadio);
NA_EXTERN_RUNTIME_TYPE(NACocoaRadio);

NA_PROTOTYPE(NACocoaSelect);
NA_EXTERN_RUNTIME_TYPE(NACocoaSelect);

NA_PROTOTYPE(NACocoaSlider);
NA_EXTERN_RUNTIME_TYPE(NACocoaSlider);

NA_PROTOTYPE(NACocoaSpace);
NA_EXTERN_RUNTIME_TYPE(NACocoaSpace);

NA_PROTOTYPE(NACocoaTextBox);
NA_EXTERN_RUNTIME_TYPE(NACocoaTextBox);

NA_PROTOTYPE(NACocoaTextField);
NA_EXTERN_RUNTIME_TYPE(NACocoaTextField);

NA_PROTOTYPE(NACocoaWindow);
NA_EXTERN_RUNTIME_TYPE(NACocoaWindow);




// Cocoa thinks it's smart by doing things automatically. Unfortunately, we
// have to encapsulate some ui elements like NARadio or NATextBox into its own
// view to get the behaviour we need.
@protocol NACocoaNativeEncapsulatedElement
@required
- (NSView*) getEncapsulatingView;
@end

@interface NACocoaNativeApplicationDelegate : NSObject <NSApplicationDelegate>{
  NACocoaApplication* cocoaApplication;
  NSObject <NSApplicationDelegate>* oldDelegate;
  NAMutator postStartupFunction;
  void* postStartupArg;
  NABool atStartup;
}
- (void)setOldDelegate:(NSObject <NSApplicationDelegate>*)delegate;
- (void)setPostStartupFunction:(NAMutator)postUpdate;
- (void)setPostStartupArg:(void*)arg;
@end

@interface NACocoaNativeButton : NSButton{
  NACocoaButton* cocoaButton;
  bool isImage;
}
@end

@interface NACocoaNativeCheckBox : NSButton{
  NACocoaCheckBox* cocoaCheckBox;
}
@end

@interface NACocoaNativeImageSpace : NSImageView{
  NACocoaImageSpace* cocoaImageSpace;
}
@end

@interface NACocoaNativeLabel : NSTextField{
  NACocoaLabel* cocoaLabel;
}
@end

@interface NACocoaNativeMenu : NSMenu{
  NACocoaMenu* cocoaMenu;
}
@end

@interface NACocoaNativeMenuItem : NSMenuItem{
  NACocoaMenuItem* cocoaMenuItem;
}
@end

#if(NA_COMPILE_METAL == 1)
#ifdef __MAC_10_12  // Metal is only available since macOS 10.11, CALayerDelegate since 10.12

  #import <QuartzCore/CAMetalLayer.h>
  #include <Metal/Metal.h>

  @interface NACocoaNativeMetalSpace : NSView<CALayerDelegate>{
    NACocoaMetalSpace* cocoaMetalSpace;
    NSTrackingArea*     trackingArea;
  }
  @end  

#endif
#endif

#if(NA_COMPILE_OPENGL == 1)

  #pragma GCC diagnostic push 
  #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  
  @interface NACocoaNativeOpenGLSpace : NSOpenGLView{
    NACocoaOpenGLSpace* cocoaOpenGLSpace;
    NSTrackingArea*     trackingArea;
    NAMutator           initFunc;
    void*               initData;
  }
  @end

  #pragma GCC diagnostic pop

#endif

@interface NACocoaNativeRadio : NSButton <NACocoaNativeEncapsulatedElement>{
  NACocoaRadio* cocoaRadio;
  NSView*       containingView;
}
- (NSView*) getEncapsulatingView;
@end

@interface NACocoaNativeSelect : NSPopUpButton{
  NACocoaSelect* cocoaSelect;
}
@end

@interface NACocoaNativeSlider : NSSlider{
  NACocoaSlider* cocoaSlider;
}
@end

@interface NACocoaNativeSpace : NSView{
  NACocoaSpace*   cocoaSpace;
  NABool isMoving;
  NAPos originMousePos;
}
@end

@interface NACocoaNativeTextBox : NSTextView <NACocoaNativeEncapsulatedElement, NSTextViewDelegate>{
  NACocoaTextBox* cocoaTextBox;
  NSScrollView*   scrollView;
}
- (NSView*) getEncapsulatingView;
@end

@interface NACocoaNativeTextField : NSTextField <NSTextFieldDelegate>{
  NACocoaTextField* cocoaTextField;
}
@end

@interface NACocoaNativeWindow : NSWindow <NSWindowDelegate>{
  NACocoaWindow* cocoaWindow;
}
@end



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
