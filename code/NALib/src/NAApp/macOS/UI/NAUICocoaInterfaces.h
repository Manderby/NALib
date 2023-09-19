
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


typedef struct NACocoaApplication NACocoaApplication;
struct NACocoaApplication {
  NAApplication application;
};
NA_HAPI void na_DestructCocoaApplication(NACocoaApplication* cocoaApplication);
NA_RUNTIME_TYPE(NACocoaApplication, na_DestructCocoaApplication, NA_FALSE);

typedef struct NACocoaButton NACocoaButton;
struct NACocoaButton{
  NAButton button;
};
NA_HAPI void na_DestructCocoaButton(NACocoaButton* cocoaButton);
NA_RUNTIME_TYPE(NACocoaButton, na_DestructCocoaButton, NA_FALSE);

typedef struct NACocoaCheckBox NACocoaCheckBox;
struct NACocoaCheckBox{
  NACheckBox checkBox;
};
NA_HAPI void na_DestructCocoaCheckBox(NACocoaCheckBox* cocoaCheckBox);
NA_RUNTIME_TYPE(NACocoaCheckBox, na_DestructCocoaCheckBox, NA_FALSE);

typedef struct NACocoaImageSpace NACocoaImageSpace;
struct NACocoaImageSpace{
  NAImageSpace imageSpace;
};
NA_HAPI void na_DestructCocoaImageSpace(NACocoaImageSpace* cocoaImageSpace);
NA_RUNTIME_TYPE(NACocoaImageSpace, na_DestructCocoaImageSpace, NA_FALSE);

typedef struct NACocoaLabel NACocoaLabel;
struct NACocoaLabel{
  NALabel label;
};
NA_HAPI void na_DestructCocoaLabel(NACocoaLabel* cocoaLabel);
NA_RUNTIME_TYPE(NACocoaLabel, na_DestructCocoaLabel, NA_FALSE);

typedef struct NACocoaMenu NACocoaMenu;
struct NACocoaMenu{
  NAMenu menu;
};
NA_HAPI void na_DestructCocoaMenu(NACocoaMenu* cocoaMenu);
NA_RUNTIME_TYPE(NACocoaMenu, na_DestructCocoaMenu, NA_FALSE);

typedef struct NACocoaMenuItem NACocoaMenuItem;
struct NACocoaMenuItem{
  NAMenuItem menuItem;
};
NA_HAPI void na_DestructCocoaMenuItem(NACocoaMenuItem* cocoaMenuItem);
NA_RUNTIME_TYPE(NACocoaMenuItem, na_DestructCocoaMenuItem, NA_FALSE);

typedef struct NACocoaMetalSpace NACocoaMetalSpace;
struct NACocoaMetalSpace{
  NAMetalSpace metalSpace;
};
NA_HAPI void na_DestructCocoaMetalSpace(NACocoaMetalSpace* cocoaMetalSpace);
NA_RUNTIME_TYPE(NACocoaMetalSpace, na_DestructCocoaMetalSpace, NA_FALSE);

typedef struct NACocoaOpenGLSpace NACocoaOpenGLSpace;
struct NACocoaOpenGLSpace{
  NAOpenGLSpace openGLSpace;
};
NA_HAPI void na_DestructCocoaOpenGLSpace(NACocoaOpenGLSpace* cocoaOpenGLSpace);
NA_RUNTIME_TYPE(NACocoaOpenGLSpace, na_DestructCocoaOpenGLSpace, NA_FALSE);

typedef struct NACocoaPopupButton NACocoaPopupButton;
struct NACocoaPopupButton{
  NAPopupButton popupButton;
};
NA_HAPI void na_DestructCocoaPopupButton(NACocoaPopupButton* cocoaPopupButton);
NA_RUNTIME_TYPE(NACocoaPopupButton, na_DestructCocoaPopupButton, NA_FALSE);

typedef struct NACocoaRadio NACocoaRadio;
struct NACocoaRadio{
  NARadio radio;
};
NA_HAPI void na_DestructCocoaRadio(NACocoaRadio* cocoaRadio);
NA_RUNTIME_TYPE(NACocoaRadio, na_DestructCocoaRadio, NA_FALSE);

typedef struct NACocoaSlider NACocoaSlider;
struct NACocoaSlider{
  NASlider slider;
};
NA_HAPI void na_DestructCocoaSlider(NACocoaSlider* cocoaSlider);
NA_RUNTIME_TYPE(NACocoaSlider, na_DestructCocoaSlider, NA_FALSE);

typedef struct NACocoaSpace NACocoaSpace;
struct NACocoaSpace{
  NASpace space;
};
NA_HAPI void na_DestructCocoaSpace(NACocoaSpace* cocoaSpace);
NA_RUNTIME_TYPE(NACocoaSpace, na_DestructCocoaSpace, NA_FALSE);

typedef struct NACocoaTextBox NACocoaTextBox;
struct NACocoaTextBox{
  NATextBox textBox;
};
NA_HAPI void na_DestructCocoaTextBox(NACocoaTextBox* cocoaTextBox);
NA_RUNTIME_TYPE(NACocoaTextBox, na_DestructCocoaTextBox, NA_FALSE);

typedef struct NACocoaTextField NACocoaTextField;
struct NACocoaTextField{
  NATextField textField;
};
NA_HAPI void na_DestructCocoaTextField(NACocoaTextField* cocoaTextField);
NA_RUNTIME_TYPE(NACocoaTextField, na_DestructCocoaTextField, NA_FALSE);

typedef struct NACocoaWindow NACocoaWindow;
struct NACocoaWindow{
  NAWindow window;
};
NA_HAPI void na_DestructCocoaWindow(NACocoaWindow* cocoaWindow);
NA_RUNTIME_TYPE(NACocoaWindow, na_DestructCocoaWindow, NA_FALSE);




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
  NSTrackingArea* trackingArea;
  bool isImage;
}
@end

@interface NACocoaNativeCheckBox : NSButton{
  NACocoaCheckBox* cocoaCheckBox;
}
@end

@interface NACocoaNativeImageSpace : NSImageView{
  NACocoaImageSpace* cocoaImageSpace;
  NSTrackingArea* trackingArea;
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

#if (NA_COMPILE_METAL == 1)
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

#if (NA_COMPILE_OPENGL == 1)

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

@interface NACocoaNativePopupButton : NSPopUpButton{
  NACocoaPopupButton* cocoaPopupButton;
}
@end

@interface NACocoaNativeRadio : NSButton <NACocoaNativeEncapsulatedElement>{
  NACocoaRadio* cocoaRadio;
  NSView*       containingView;
}
- (NSView*) getEncapsulatingView;
@end

@interface NACocoaNativeSlider : NSSlider{
  NACocoaSlider* cocoaSlider;
}
@end

@interface NACocoaNativeSpace : NSView{
  NACocoaSpace*   cocoaSpace;
  NSTrackingArea* trackingArea;
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
  size_t trackingCount;
  NSTrackingArea* trackingArea;
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
