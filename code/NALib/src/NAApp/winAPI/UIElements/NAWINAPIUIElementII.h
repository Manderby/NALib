
// This file is left intentionally empty until there is a system dependent
// implementation necessary.



// todo: find a faster way. Hash perhaps or something else.
NA_HDEF void* na_GetUINALibEquivalent(void* nativePtr) {
  HINSTANCE appPtr = (HINSTANCE)GetWindowLongPtr(nativePtr, GWLP_HINSTANCE);

  // We need to test for the hinstance. Otherwise HWND of other applications
  // might be used erroneously as NA_UIElement instances, resulting in crashes.
  if(appPtr == (HINSTANCE)naGetUIElementNativePtr(naGetApplication())) {
    return (NA_UIElement*)GetWindowLongPtr(nativePtr, GWLP_USERDATA);
  }
  
  return NA_NULL;
}



NA_HDEF void na_InitSystemUIElement(NA_UIElement* uiElement, void* nativePtr) {
  switch(naGetUIElementType(uiElement)) {
  case NA_UI_APPLICATION:
  case NA_UI_SCREEN:
  case NA_UI_MENU:
  case NA_UI_MENUITEM:
    // These UI elements have a different native pointer than HWND. They are
    // not handeled at this point in time and maybe never have to as the
    // winAPI procedures will only call na_GetUINALibEquivalent for HWND
    // objects anyway.
    break;
  default:
    SetWindowLongPtr(nativePtr, GWLP_USERDATA, (LONG_PTR)uiElement);
    break;
  }
}



NA_HAPI void na_ClearSystemUIElement(void* nativePtr) {
  SetWindowLongPtr(nativePtr, GWLP_USERDATA, (LONG_PTR)NA_NULL);
}
