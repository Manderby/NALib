
// This file is left intentionally empty until there is a system dependent
// implementation necessary.


#include <objc/objc-runtime.h>

int na_naUIElementKey = 42; // Arbitrarily but carefully chosen number.



NA_HDEF void* na_GetUINALibEquivalent(void* nativePtr) {
  NA_UIElement* obj = NA_COCOA_PTR_OBJC_TO_C(objc_getAssociatedObject(nativePtr, &na_naUIElementKey));
  return obj;
}



NA_HDEF void na_InitSystemUIElement(NA_UIElement* uiElement, void* nativePtr) {
  objc_setAssociatedObject(
    NA_COCOA_PTR_C_TO_OBJC(nativePtr),
    &na_naUIElementKey,
    NA_COCOA_PTR_C_TO_OBJC(uiElement),
    OBJC_ASSOCIATION_ASSIGN);
}



NA_HAPI void na_ClearSystemUIElement(void* nativePtr) {
  // note: do not use objc_removeAssociatedObject as this will not use the key
  // to identify the poper associated object.
  objc_setAssociatedObject(
    NA_COCOA_PTR_C_TO_OBJC(nativePtr),
    &na_naUIElementKey,
    nil,
    OBJC_ASSOCIATION_ASSIGN);
}
