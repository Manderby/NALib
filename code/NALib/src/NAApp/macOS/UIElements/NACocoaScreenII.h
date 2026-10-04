
struct NACocoaScreen{
  NAScreen screen;
};
NA_HAPI void na_DestructCocoaScreen(NACocoaScreen* cocoaScreen);
NA_RUNTIME_TYPE(NACocoaScreen, na_DestructCocoaScreen, NA_FALSE);




NA_DEF NARect naGetMainScreenRect() {
  return naMakeRectWithNSRect([[NSScreen mainScreen] frame]);
}



NA_HDEF NAScreen* na_NewScreen(NSScreen* nativeObj) {
  // asdf
  // A screen is given by its native pointer and not created as a NALib custom
  // subclass. Therefore, we must retain it. It will be stored during a call
  // to na_InitScreen and will be released automatically as soon as
  // na_ClearCoreUIElement will be called.

  NACocoaScreen* cocoaScreen = naNew(NACocoaScreen);

  NABool isMainScreen = nativeObj == [NSScreen mainScreen];
  const NAUTF8Char* name = "Unknown Display";
  if(isAtLeastMacOSVersion(10, 15)) {
    NA_MACOS_AVAILABILITY_GUARD_10_15(
      name = [[nativeObj localizedName] UTF8String];
    )
  }else{
    NSDictionary *screenDictionary = [nativeObj deviceDescription];
    NSNumber *screenID = [screenDictionary objectForKey:@"NSScreenNumber"];
    name = naAllocSprintf(NA_TRUE, "Display %s", [[screenID stringValue] UTF8String]);
  }
  NARect rect = naMakeRectWithNSRect([nativeObj frame]);
  double uiScale = [nativeObj backingScaleFactor];

  na_InitCoreScreen(
    (NAScreen*)cocoaScreen,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    isMainScreen,
    name,
    rect,
    uiScale);
  
  return (NAScreen*)cocoaScreen;
}



NA_DEF void na_DestructCocoaScreen(NACocoaScreen* cocoaScreen) {
  na_ClearCoreScreen((NAScreen*)cocoaScreen);
}



NA_DEF NARect naGetScreenUsableRect(const NAScreen* screen) {
  naDefineUIElementNativeCocoaObjConst(NSScreen, nativeObj, screen);
  return naMakeRectWithNSRect([nativeObj visibleFrame]);
}



NA_HDEF NARect na_FillScreenList(NAList* screenList) {
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
  
  return totalRect;
}



