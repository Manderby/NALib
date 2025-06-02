
// This file is left intentionally empty until there is a system dependent
// implementation necessary.



NA_DEF NAScreen* na_NewScreen(
  void* nativePtr)
{
  // A screen is given by its native pointer and not created as a NALib custom
  // subclass. It is though just a reference and is not owned by this struct.

  NAWINAPIScreen* winapiScreen = naNew(NAWINAPIScreen);

  NABool isMainScreen = NA_FALSE;
  const NAUTF8Char* name = "Screen X";
  NARect rect = naMakeRectS(0., 0., 1920., 1080.);
  double uiScale = 1.;

  na_InitScreen(
    (NAScreen*)winapiScreen,
    nativePtr,
    isMainScreen,
    name,
    rect,
    uiScale);

  return (NAScreen*)winapiScreen;
}



NA_DEF void na_DestructWINAPIScreen(NAWINAPIScreen* winapiScreen) {
  na_ClearScreen((NAScreen*)winapiScreen);
}



NA_DEF NARect naGetScreenUsableRect(const NAScreen* screen) {
  return naMakeRectS(0., 0., 1920., 1080.);
  //NSScreen* nsScreen = screen->uiElement.nativePtr;
  //return naMakeRectWithNSRect([nsScreen visibleFrame]);
}



NA_HDEF NARect na_FillScreenList(NAList* screenList) {

  NARect totalRect = naMakeRectZero();

  HMONITOR monitor;
  //MONITORINFO screeninfo = {0};
  POINT origin = {0, 0};  // top left point which surely is on the main screen.
  monitor = MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
  //screeninfo.cbSize = sizeof(MONITORINFO);
  //GetMonitorInfo(screen, &screeninfo);
  //double uiScale = naGetUIElementResolutionScale(NA_NULL);

  NAScreen* screen = na_NewScreen(monitor);

  NARect screenRect = naGetUIElementRect(screen);
  totalRect = naIsRectEmpty(totalRect)
    ? screenRect
    : naMakeRectUnion(totalRect, screenRect);
  naAddListLastMutable(screenList, screen);

  // Update the relative center position.
  NAListIterator it = naMakeListMutator(screenList);
  while(naIterateList(&it)) {
    NAScreen* screen = naGetListCurMutable(&it);
    na_UpdateScreenRelativeCenter(screen, totalRect);
  }
  naClearListIterator(&it);

  return totalRect;
}
