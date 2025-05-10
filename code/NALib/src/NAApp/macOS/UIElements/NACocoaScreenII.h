

struct NACocoaScreen{
  NAScreen screen;
};
NA_HAPI void na_DestructCocoaScreen(NACocoaScreen* cocoaScreen);
NA_RUNTIME_TYPE(NACocoaScreen, na_DestructCocoaScreen, NA_FALSE);




NA_DEF NARect naGetMainScreenRect() {
  return naMakeRectWithNSRect([[NSScreen mainScreen] frame]);
}



NA_DEF NAScreen* na_NewScreen(
  void* nativePtr)
{
  // A screen is given by its native pointer and not created as a NALib custom
  // subclass. Therefore, we must retain it. It will be stored during a call
  // to na_InitScreen and will be released automatically as soon as
  // na_ClearCoreUIElement will be called.
  NSScreen* nsScreen = (NSScreen*)NA_COCOA_RETAIN(nativePtr);
  
  NACocoaScreen* cocoaScreen = naNew(NACocoaScreen);
  
  na_InitScreen(
    (NAScreen*)cocoaScreen,
    nativePtr,
    nativePtr == [NSScreen mainScreen],
    [[nsScreen localizedName] UTF8String],
    naMakeRectWithNSRect([nsScreen frame]),
    [nsScreen backingScaleFactor]);
  
  return (NAScreen*)cocoaScreen;
}



NA_DEF void na_DestructCocoaScreen(NACocoaScreen* cocoaScreen) {
  na_ClearScreen((NAScreen*)cocoaScreen);
}



NA_HDEF NABool naIsScreenMain(const NAScreen* screen) {
  return screen->isMain;
}



NA_HDEF void na_FillScreenList(NAList* screenList) {
  NSArray<NSScreen*>* nsScreens = [NSScreen screens];
  NARect totalRect = naMakeRectZero();
  for (size_t i = 0; i < [nsScreens count]; ++i) {
    NSScreen* nsScreen = [nsScreens objectAtIndex:i];
    NAScreen* screen = na_NewScreen(nsScreen);
    NARect screenRect = naGetUIElementRect(screen);
    totalRect = naIsRectEmpty(totalRect)
      ? screenRect
      : naMakeRectUnion(totalRect, screenRect);
    naAddListLastMutable(screenList, screen);
  }
  
  // Update the relative center position.
  NAListIterator it = naMakeListMutator(screenList);
  while(naIterateList(&it)) {
    NAScreen* screen = naGetListCurMutable(&it);
    na_UpdateScreenRelativeCenter(screen, totalRect);
  }
  naClearListIterator(&it);
}



