
// This file is left intentionally empty until there is a system dependent
// implementation necessary.


#include <ShellScalingApi.h>

NA_DEF NAScreen* na_NewScreen(
  void* nativePtr)
{
  // A screen is given by its native pointer and not created as a NALib custom
  // subclass. It is though just a reference and is not owned by this struct.
  NAWINAPIScreen* winapiScreen = NA_NULL;

  MONITORINFOEX monitorInfo = {0};
  monitorInfo.cbSize = sizeof(monitorInfo);
  if (GetMonitorInfo(nativePtr, (MONITORINFO*)&monitorInfo)) {
    winapiScreen = naNew(NAWINAPIScreen);

    NABool isMainScreen = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY) == MONITORINFOF_PRIMARY;
    const NAUTF8Char* name = "Screen X";
    // Regarding the rect: For now, we store the top but as soon as we have
    // enumerated all screens, we transform that value into a bottom value
    // relative to the absolute coordinate system.
    NARect rect = naMakeRectS(
      monitorInfo.rcMonitor.left,
      monitorInfo.rcMonitor.top,
      monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
      monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top);

    // Now, Windows does some strange things when computing the ui scaling.
    // As far as the author can detect, under windows 11, the HORZRES value
    // is the pixel count divided by the scaling factor. But only for monitors
    // which are not the primary monitor! There, the pixel count is instead
    // multiplied with the scaling factor. So, the best bet might be to test
    // whether HORZRES is smaller or greater than the pixel count and compute
    // the scaling factor accordingly.

    HDC hDC = CreateDC(NULL, monitorInfo.szDevice, NULL, NULL);
    int logpixels = GetDeviceCaps(hDC, LOGPIXELSX);
    int horzres = GetDeviceCaps(hDC, HORZRES);
    double uiScale = (horzres > rect.size.width)
      ? horzres / rect.size.width
      : rect.size.width / horzres;
    ReleaseDC (NULL, hDC);

    // Another often referred to solution is the use of the following function.
    // But that requires Shcore.lib and works like ass. 140% scaling when 150%
    // is chosen. Great job Windows!
    //DEVICE_SCALE_FACTOR dsFactor;
    //GetScaleFactorForMonitor(nativePtr, &dsFactor);

    na_InitScreen(
      (NAScreen*)winapiScreen,
      nativePtr,
      isMainScreen,
      name,
      rect,
      uiScale);
  }

  return (NAScreen*)winapiScreen;
}



NA_DEF void na_DestructWINAPIScreen(NAWINAPIScreen* winapiScreen) {
  na_ClearScreen((NAScreen*)winapiScreen);
}



NA_DEF NARect naGetScreenUsableRect(const NAScreen* screen) {
  MONITORINFO screenInfo = {0};
  screenInfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(naGetUIElementNativePtrConst(screen), &screenInfo);

  return naMakeRectS(
    screenInfo.rcWork.left,
    screenInfo.rcWork.top,
    screenInfo.rcWork.right - screenInfo.rcWork.left,
    screenInfo.rcWork.bottom - screenInfo.rcWork.top);
}


BOOL na_MonitorEnumProc(
  HMONITOR hMonitor,
  HDC dc,
  LPRECT rect,
  LPARAM userData)
{
  NAList* screenList = (NAList*)userData;

  NAScreen* screen = na_NewScreen(hMonitor);
  if(screen) {
    naAddListLastMutable(screenList, screen);
  }

  return TRUE; // true = continue enumeration.
}

NA_HDEF NARect na_FillScreenList(NAList* screenList) {

  NARect totalRect = naMakeRectZero();

  EnumDisplayMonitors(NULL, NULL, na_MonitorEnumProc, (LPARAM)screenList);  

  NARect mainScreenRect;

  NAListIterator it1 = naMakeListMutator(screenList);
  while(naIterateList(&it1)) {
    NAScreen* screen = naGetListCurMutable(&it1);
    NARect screenRect = naGetUIElementRect(screen);
    if(naIsScreenMain(screen)) {
      mainScreenRect = screenRect;
    }
    totalRect = naIsRectEmpty(totalRect)
      ? screenRect
      : naMakeRectUnion(totalRect, screenRect);
  }
  naClearListIterator(&it1);

  const NAScreen* primaryScreen = naGetListFirstConst(screenList);

  // Now, flip the absolute space vertically, such that the main screen
  // will have coordinate (0,0) still in its bottom left conrner.
  totalRect.pos.y = -(totalRect.size.height - (mainScreenRect.size.height) - (mainScreenRect.pos.y - totalRect.pos.y));

  // Update the vertical orientation and the relative center position.
  NAListIterator it2 = naMakeListMutator(screenList);
  while(naIterateList(&it2)) {
    NAScreen* screen = naGetListCurMutable(&it2);
    na_FlipScreenCoordinatesVertically(screen, totalRect);
    na_UpdateScreenRelativeCenter(screen, totalRect);
  }
  naClearListIterator(&it2);

  return totalRect;
}
