
// This file is left intentionally empty until there is a system dependent
// implementation necessary.



// todo: find a faster way. Hash perhaps or something else.
NA_HDEF void* na_GetUINALibEquivalent(void* nativePtr) {

  NA_UIElement* retelem = NA_NULL;
  NAListIterator iter = naMakeListMutator(&na_App->uiElements);
  while(naIterateList(&iter)) {
    NA_UIElement* elem = naGetListCurMutable(&iter);
    if(elem->nativePtr == nativePtr) {
      retelem = elem;
      break;
    }
  }
  naClearListIterator(&iter);

  //retelem = (NA_UIElement*)GetWindowLongPtr(nativePtr, GWLP_USERDATA);

  return retelem;
}



NA_HDEF void na_InitSystemUIElement(NA_UIElement* uiElement, void* nativePtr) {
  SetWindowLongPtr(nativePtr, GWLP_USERDATA, (LONG_PTR)uiElement);
}



NA_HAPI void na_ClearSystemUIElement(void* nativePtr) {
  SetWindowLongPtr(nativePtr, GWLP_USERDATA, (LONG_PTR)NA_NULL);
}
