
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



#include "../../../NAUtility/NAString.h"
#include "../../../NAUtility/NAURL.h"
#include "../../../NAUtility/NATranslator.h"
#include "../../../NAStruct/NAStack.h"
#include "../../../NAVisual/NAImage.h"

#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  #include <commctrl.h>
#endif



WNDPROC na_GetApplicationOldButtonWindowProc() {
  return ((NAWINAPIApplication*)naGetApplication())->oldButtonWindowProc;
}
WNDPROC na_GetApplicationOldCheckBoxWindowProc() {
  return ((NAWINAPIApplication*)naGetApplication())->oldCheckBoxWindowProc;
}
WNDPROC na_GetApplicationOldLabelWindowProc() {
  return ((NAWINAPIApplication*)naGetApplication())->oldLabelWindowProc;
}
WNDPROC na_GetApplicationOldRadioWindowProc() {
  return ((NAWINAPIApplication*)naGetApplication())->oldRadioWindowProc;
}
WNDPROC na_GetApplicationOldSelectWindowProc() {
  return ((NAWINAPIApplication*)naGetApplication())->oldSelectWindowProc;
}
WNDPROC na_GetApplicationOldSliderWindowProc() {
  return ((NAWINAPIApplication*)naGetApplication())->oldSliderWindowProc;
}
WNDPROC na_GetApplicationOldTextFieldWindowProc() {
  return ((NAWINAPIApplication*)naGetApplication())->oldTextFieldWindowProc;
}



NAWINAPICallbackInfo naApplicationWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message) {
  default:
    //printf("Uncaught Application message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF void naStartApplication(NAMutator preStartup, NAMutator postStartup, NAMutator cleanup, void* arg) {
  NAWINAPIApplication* app;
  WNDCLASS wndclass;
  MSG message;

  SetProcessDPIAware();

  // Uncommented for future use.
  //DPI_AWARENESS awareness = DPI_AWARENESS_SYSTEM_AWARE;
  //SetProcessDpiAwarenessContext(&awareness);

  #if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
    InitCommonControls();   // enable visual styles
  #endif
  
  // First, register the window classes. This is required as the
  // offscreen window class must be present upon the call to
  // na_NewApplication.

  // Register the window class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = naWINAPIWindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon( wndclass.hInstance, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NAWindow");
	RegisterClass(&wndclass);

  // Register the offscreen window class
  naZeron(&wndclass, sizeof(WNDCLASS));
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = naWINAPIWindowCallback;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = GetModuleHandle(NULL);
  wndclass.hIcon = NULL;
  wndclass.hCursor = NULL;
  wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = TEXT("NAOffscreenWindow");
  RegisterClass(&wndclass);

  // Register the space class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
	wndclass.lpfnWndProc = naWINAPIWindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NASpace");
	RegisterClass(&wndclass);

    // Register the OpenGL space class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = naWINAPIWindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NAOpenGLSpace");
	RegisterClass(&wndclass);

    // Start the WINAPI application and set the nativePtr of the application.
  app = (NAWINAPIApplication*)na_NewApplication();

  // Call preStartup if desired.
  if(preStartup) {
    preStartup(arg);
  }

  // Set the language of the translator
  naResetApplicationPreferredTranslatorLanguages();

  // Call postStartup if desired.
  if(postStartup) {
    postStartup(arg);
  }

  // Start the event loop.
  while(na_IsApplicationRunning()) {
    BOOL response = FALSE;

    naCollectGarbage();

    // Note about the WM_TIMER and WM_PAINT messages: UI elements like
    // especially OpenGLSpace will take some time to compute and hence block
    // the whole application from being responsive.
    //
    // To overcome this issue, ther are several ways. There is coalescing,
    // done with naAddOpenGLSpaceToRedrawList, currently commented out. And
    // there is naRefreshUIElement which may or may not redraw immediately
    // if the timeDiff is set to zero. Both ways introduce a timer which needs
    // to be sent through the message queue. That leads to a small delay which
    // has been observed to skip one frame on windows most of the time, hence
    // making the UI behave sluggish. Therefore, at this point in time, both
    // methods are turned off and redraws happen immediately when timediff is
    // set to zero.
    //
    // If this part of the code gets revisited one day, be aware: GetMessage
    // will return WM_TIMER and WM_PAINT messages last when no filter is
    // applied. This means when for example new mouse inputs have occured in
    // the meantime between painting, those elements get never painted until
    // the mouse stops.
    //
    // This whole timing and painting thing is a bit tricky but as up to now,
    // the best solution was to just paint immediately and let the application
    // lag. So be it.

    response = PeekMessage(&message, 0, WM_TIMER, WM_TIMER, PM_REMOVE);
    if(response) {
      // Do timer messages immediately.
      TranslateMessage(&message);
      DispatchMessage(&message);
      continue;
    }

    response = PeekMessage(&message, 0, 0, 0, PM_REMOVE | PM_QS_PAINT);
    if(response) {
      // Do paint messages immediately.
      TranslateMessage(&message);
      DispatchMessage(&message);
      continue;
    }

    response = GetMessage(&message, 0, 0, 0);
    
    if(response == 0) {
      break;
    }else if(response == -1) {
      // handle the error and possibly exit
      break;
    }else{
      if(message.message == WM_QUIT) {
        break;
      }
      // Capture any keyboard shortcuts overridden by the NALib user
      // Note: Usually, the IsDialogMessage function is responsible for
      // capturing TAB events but it has proven to be difficult to handle
      // and therefore, everything is captured in na_InterceptKeyboardShortcut.
      if(!na_InterceptKeyboardShortcut(&message)) {
        // Do the normal message dispatch.
        TranslateMessage(&message);
        DispatchMessage(&message);
      }
    }
  }

  // Before deleting the application, we cleanup whatever the user needs to
  // clean up.
  if (cleanup)
    cleanup(arg);

  // Close the preferences handle
  na_ShutdownPreferences();

  // When reaching here, the application had been stopped.
  naDelete(app);
}

#include "muiload.h"

NA_DEF void naResetApplicationPreferredTranslatorLanguages(void) {
  // As cryptic as it seems, windows stores the preferred user languages
  // in the windows registry under the following keys, denoted by hex numbers
  // stored in string values, starting with 00000000.
  //
  // The returned values are also string hex values, denoting a LANGID.

  NAStack langStack;
  naInitStack(&langStack, sizeof(NALanguageCode3), 0, 0);

  size_t index = 0;
  while(1) {
    NAUTF8Char* keyString = naAllocSprintf(NA_TRUE, "%08X", (int)index);

    size_t valueSize;
    wchar_t* idString = na_GetWINRegistryVariableEntry(
      HKEY_CURRENT_USER,
      "Software\\Microsoft\\CTF\\SortOrder\\Language",
      keyString,
      &valueSize);

    if(!idString) { break; }

    LANGID langId = 0;
    for(size_t i = 0; i < (valueSize / sizeof(wchar_t) - 1); ++i) {
      if(idString[i] >= L'0' && idString[i] <= L'9') {
        langId = langId * 16 + (idString[i] - L'0');
      }
      if(idString[i] >= L'a' && idString[i] <= L'f') {
        langId = langId * 16 + (idString[i] - L'a' + 10);
      }
      if(idString[i] >= L'A' && idString[i] <= L'F') {
        langId = langId * 16 + (idString[i] - L'F' + 10);
      }
    }

    *(NALanguageCode3*)naPushStack(&langStack) = naGetLanguageCodeWithLANGID(langId);

    naFree(idString);
    index++;
  }

  // Apply the preferred languages from back to front.
  while(naGetStackCount(&langStack)) {
    NALanguageCode3 languageCode = *(NALanguageCode3*)naPopStack(&langStack);
    naSetTranslatorLanguagePreference(languageCode);
  }

  naClearStack(&langStack);
}



NA_HDEF NAApplication* na_NewApplication(void) {

  NAWINAPIApplication* winapiApplication = naNew(NAWINAPIApplication);

  winapiApplication->nonClientMetrics.cbSize = sizeof(NONCLIENTMETRICS);
  SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &winapiApplication->nonClientMetrics, 0);

  na_InitApplication(&winapiApplication->application, GetModuleHandle(NULL));

  naInitList(&winapiApplication->timers);
  naInitList(&winapiApplication->openGLRedrawList);

  winapiApplication->offscreenWindow = CreateWindow(
    TEXT("NAOffscreenWindow"), TEXT("Offscreen window"), WS_OVERLAPPEDWINDOW,
    0, 0, 0, 0,
    NULL, NULL, GetModuleHandle(NULL), NULL);

  winapiApplication->appIcon = NA_NULL;

  winapiApplication->mouseHoverElement = NA_NULL;
  winapiApplication->lastOpenedMenu = NA_NULL;
  winapiApplication->nextMenuItemId = 1;

  winapiApplication->oldButtonWindowProc = NA_NULL;
  winapiApplication->oldCheckBoxWindowProc = NA_NULL;
  winapiApplication->oldLabelWindowProc = NA_NULL;
  winapiApplication->oldSelectWindowProc = NA_NULL;
  winapiApplication->oldRadioWindowProc = NA_NULL;
  winapiApplication->oldSliderWindowProc = NA_NULL;
  winapiApplication->oldTextFieldWindowProc = NA_NULL;

  winapiApplication->fgColor.color = GetSysColor(COLOR_WINDOWTEXT);
  winapiApplication->fgColorDisabled.color = GetSysColor(COLOR_GRAYTEXT);
  winapiApplication->bgColor.color = GetSysColor(COLOR_BTNFACE);
  winapiApplication->bgColorAlternate.color = RGB(226, 226, 226);
  winapiApplication->bgColorAlternate2.color = RGB(205, 205, 205);

  winapiApplication->fgColor.brush = CreateSolidBrush(winapiApplication->fgColor.color);
  winapiApplication->fgColorDisabled.brush = CreateSolidBrush(winapiApplication->fgColorDisabled.color);
  winapiApplication->bgColor.brush = CreateSolidBrush(winapiApplication->bgColor.color);
  winapiApplication->bgColorAlternate.brush = CreateSolidBrush(winapiApplication->bgColorAlternate.color);
  winapiApplication->bgColorAlternate2.brush = CreateSolidBrush(winapiApplication->bgColorAlternate2.color);

  return (NAApplication*)winapiApplication;
}



NA_DEF void na_DestructWINAPIApplication(NAWINAPIApplication* winapiApplication) {
  DestroyWindow(winapiApplication->offscreenWindow);

  DeleteObject(winapiApplication->fgColor.brush);
  DeleteObject(winapiApplication->fgColorDisabled.brush);
  DeleteObject(winapiApplication->bgColor.brush);
  DeleteObject(winapiApplication->bgColorAlternate.brush);
  DeleteObject(winapiApplication->bgColorAlternate2.brush);

  DestroyIcon(winapiApplication->appIcon);

  na_ClearApplication(&winapiApplication->application);  

  // Now that all windows are destroyed, all dependent timers are deleted. We can
  // safely release the timer structs. todo: Make killing the timers a sport.
  naForeachListMutable(&winapiApplication->timers, (NAMutator)naFree);
  naClearList(&winapiApplication->timers);

  naClearList(&winapiApplication->openGLRedrawList);
}



HWND naGetApplicationOffscreenWindow(void) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->offscreenWindow;
}

NA_UIElement* naGetApplicationMouseHoverElement(void) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->mouseHoverElement;
}

void naSetApplicationMouseHoverElement(NA_UIElement* element) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->mouseHoverElement = element;
}

const NONCLIENTMETRICS* naGetApplicationMetrics(void) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return &app->nonClientMetrics;
}


// ///////////////////////////////////
// TIMER
// ///////////////////////////////////


// This is the native WINAPI callback function. It finds the corresponding
// registered timer struct of the application and executes the function
// stored in that struct with the stored argument.
//
// Definitely not the fastest and best method. But as for now, it's ok. todo.
NA_HDEF static VOID CALLBACK na_TimerCallbackFunction(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
  //todo something is wrong here with the type.
  NAWINAPIApplication* app;

  UINT timerkey = (UINT)idEvent;
  app = (NAWINAPIApplication*)naGetApplication();

  NAListIterator iter = naMakeListModifier(&app->timers);
  while(naIterateList(&iter)) {
    NAWINAPITimerStruct* timerStruct = naGetListCurMutable(&iter);
    if(timerStruct->key == timerkey) {
      naRemoveListCurMutable(&iter, NA_FALSE);
      KillTimer(hwnd, idEvent);
      timerStruct->func(timerStruct->arg);
      naFree(timerStruct);
      break;
    }
  }
  naClearListIterator(&iter);
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff) {
  NAWINAPIApplication* app;
  NAWINAPITimerStruct* timerStruct = naAlloc(NAWINAPITimerStruct);
  timerStruct->func = function;
  timerStruct->arg = arg;
  // todo: Check type
  timerStruct->key = (UINT)SetTimer((HWND)NA_NULL, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), na_TimerCallbackFunction);
  app = (NAWINAPIApplication*)naGetApplication();
  naAddListLastMutable(&app->timers, timerStruct);
}



#if NA_COMPILE_OPENGL == 1

NA_HDEF na_redrawOpenGLSpaces(void* data) {
  NA_UNUSED(data);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAListIterator it = naMakeListMutator(&app->openGLRedrawList);

  while(naIterateList(&it)) {
    NAWINAPIOpenGLSpace* openGLSpace = naGetListCurMutable(&it);
    wglMakeCurrent(GetDC(naGetUIElementNativePtr(openGLSpace)), openGLSpace->hRC);
    if(!na_DispatchUIElementCommand(&openGLSpace->openGLSpace.uiElement, NA_UI_COMMAND_REDRAW)) {
      // don't know what to do.
    }
  }
  naClearListIterator(&it);
  naEmptyList(&app->openGLRedrawList);
}

NA_HDEF void naAddOpenGLSpaceToRedrawList(NAWINAPIOpenGLSpace* openGLSpace) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAListIterator it = naMakeListAccessor(&app->openGLRedrawList);
  if(!naLocateListData(&it, openGLSpace)) {
    if(naIsListEmpty(&app->openGLRedrawList)) {
      naCallApplicationFunctionInSeconds(na_redrawOpenGLSpaces, NA_NULL, 0.);
    }

    naAddListLastMutable(&app->openGLRedrawList, openGLSpace);
  }
  naClearListIterator(&it);
}

#endif // NA_COMPILE_OPENGL == 1



#if(NA_ADDRESS_BITS == 64)
  typedef intptr_t NAWINAPIHANDLE;
#elif(NA_ADDRESS_BITS == 32)
  typedef long NAWINAPIHANDLE;
#else
  #error "Undefined system address byteSize"
#endif



#include <io.h>
#include <fcntl.h>
NA_DEF void naOpenConsoleWindow(void) {
  FILE *outFile;
  FILE *errFile;
  FILE *inFile;
  TCHAR* systemTitle;

  AllocConsole();

  systemTitle = TEXT("Debug Console");
  SetConsoleTitle(systemTitle);

  freopen_s(&inFile, "CONIN$", "r", stdin);
  freopen_s(&outFile, "CONOUT$", "w", stdout);
  freopen_s(&errFile, "CONERR$", "w", stderr);
}




NA_HDEF void na_SetApplicationIconPath(const NAUTF8Char* path) {  
  if(path) {
    NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
    NAImage* iconImage = naCreateImageWithFilePath(path);
    HBITMAP bitmap = naAllocNativeImageWithImage(iconImage);
   
    HBITMAP hbmMask = CreateCompatibleBitmap(
      GetDC(NULL), 
      (int)naGetImageSize(iconImage).width,
      (int)naGetImageSize(iconImage).height);

    ICONINFO ii = {0};
    ii.fIcon    = TRUE;
    ii.hbmColor = bitmap;
    ii.hbmMask  = hbmMask;
    app->appIcon = CreateIconIndirect(&ii);
  }
}



NA_DEF NAString* naNewApplicationName(void) {
  NAApplication* app = naGetApplication();
  if(app->appName) {
    return naNewStringWithFormat("%s", app->appName);
  }else{
    TCHAR modulePath[MAX_PATH];
    NAString* utf8ModulePath;
    NAURL url;
    NAString* applicationName;
    NAString* applicationbasename;

    GetModuleFileName(NULL, modulePath, MAX_PATH);
    utf8ModulePath = naNewStringWithSystemString(modulePath);

    naInitURLWithUTF8CStringLiteral(&url, naGetStringUTF8Pointer(utf8ModulePath));
    naDelete(utf8ModulePath);
    applicationName = naNewStringWithURLFilename(&url);
    applicationbasename = naNewStringWithBasenameOfPath(applicationName);
    naClearURL(&url);
    naDelete(applicationName);

    return applicationbasename;
  }
}



NA_DEF NAString* naNewApplicationCompanyName(void) {
  NAApplication* app = naGetApplication();
  if(app->companyName) {
    return naNewStringWithFormat("%s", app->companyName);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationVersionString(void) {
  NAApplication* app = naGetApplication();
  if(app->versionString) {
    return naNewStringWithFormat("%s", app->versionString);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationBuildString(void) {
  NAApplication* app = naGetApplication();
  if(app->buildString) {
    return naNewStringWithFormat("%s", app->buildString);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationIconPath(void) {
  NAApplication* app = naGetApplication();
  if(app->iconPath) {
    return naNewStringWithFormat("%s", app->iconPath);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix) {
  NAApplication* app = naGetApplication();
  NAString* retString;
  if(dir) {
    if(app->resourceBasePath) {
      retString = naNewStringWithFormat("%s%c%s%c%s%c%s", app->resourceBasePath, NA_PATH_DELIMITER_WIN, dir, NA_PATH_DELIMITER_WIN, basename, NA_SUFFIX_DELIMITER, suffix);
    }else{
      retString = naNewStringWithFormat("%s%c%s%c%s", dir, NA_PATH_DELIMITER_WIN, basename, NA_SUFFIX_DELIMITER, suffix);
    }
  }else{
    if(app->resourceBasePath) {
      retString = naNewStringWithFormat("%s%c%s%c%s", app->resourceBasePath, NA_PATH_DELIMITER_WIN, basename, NA_SUFFIX_DELIMITER, suffix);
    }else{
      retString = naNewStringWithFormat("%s%c%s", basename, NA_SUFFIX_DELIMITER, suffix);
    }
  }
  return retString;
}



NA_DEF NAString* naNewExecutablePath(void) {
  TCHAR modulePath[MAX_PATH];
  NAString* utf8ModulePath;
  NAString* utf8ModuleBasePath;

  GetModuleFileName(NULL, modulePath, MAX_PATH);
  utf8ModulePath = naNewStringWithSystemString(modulePath);
  utf8ModuleBasePath = naNewStringWithParentOfPath(utf8ModulePath);

  naDelete(utf8ModulePath);

  return utf8ModuleBasePath;
}



NA_DEF HICON na_GetWINAPIApplicationIcon(void) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->appIcon;
}



//// This is just a small code snipplet useful for debugging. See call to EnumFontFamilies below.
//int CALLBACK enumFonts(
//  _In_ ENUMLOGFONT   *lpelf,
//  _In_ NEWTEXTMETRIC *lpntm,
//  _In_ DWORD         FontType,
//  _In_ LPARAM        lParam
//) {
//  int x = 1234;
//  printf("%ls" NA_NL, lpelf->elfFullName);
//}

NA_DEF NAFont* naCreateFontWithPreset(NAFontKind kind, NAFontSize fontSize) {
  NAFont* retFont = NA_NULL;

  //EnumFontFamilies(GetDC(NA_NULL), NA_NULL, enumFonts, NA_NULL);

  const NONCLIENTMETRICS* metrics = naGetApplicationMetrics();

  LONG baseSize;
  switch(fontSize) {
  case NA_FONT_SIZE_SMALL: baseSize = 14; break;
  case NA_FONT_SIZE_DEFAULT: baseSize = 16; break;
    //case NA_FONT_SIZE_DEFAULT: baseSize = metrics->lfMessageFont.lfHeight; break;
  case NA_FONT_SIZE_BIG: baseSize = 20; break;
  case NA_FONT_SIZE_HUGE: baseSize = 30; break;
  default: baseSize = metrics->lfMessageFont.lfHeight; break;
  }

  #if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1

  NAString* fontFamilyName;

  switch(kind) {
    case NA_FONT_KIND_SYSTEM:
      fontFamilyName = naNewStringWithSystemString(metrics->lfMessageFont.lfFaceName);
      retFont = naCreateFont(
        naGetStringUTF8Pointer(fontFamilyName),
        NA_FONT_FLAG_REGULAR,
        baseSize);
      break;
    case NA_FONT_KIND_TITLE:
      fontFamilyName = naNewStringWithSystemString(metrics->lfMessageFont.lfFaceName);
      retFont = naCreateFont(
        naGetStringUTF8Pointer(fontFamilyName),
        NA_FONT_FLAG_BOLD,
        baseSize);
      break;
    case NA_FONT_KIND_MONOSPACE:
      fontFamilyName = naNewStringWithSystemString(TEXT("Courier New"));
      retFont = naCreateFont(
        naGetStringUTF8Pointer(fontFamilyName),
        NA_FONT_FLAG_REGULAR,
        baseSize);
      break;
    case NA_FONT_KIND_PARAGRAPH:
      fontFamilyName = naNewStringWithSystemString(TEXT("Palatino Linotype"));
      retFont = naCreateFont(
        naGetStringUTF8Pointer(fontFamilyName),
        NA_FONT_FLAG_REGULAR,
        baseSize);
      break;
    case NA_FONT_KIND_MATH:
      fontFamilyName = naNewStringWithWideCharString(TEXT("Palatino Linotype"));
      retFont = naCreateFont(
        naGetStringUTF8Pointer(fontFamilyName),
        NA_FONT_FLAG_REGULAR,
        baseSize);
      break;
    default:
      #if NA_DEBUG
        naError("Unknown font kind");
      #endif
      fontFamilyName = naNewString();
      break;
  }

  naDelete(fontFamilyName);

  #else

  retFont = naCreateFont(
    "default",
    NA_FONT_FLAG_REGULAR,
    baseSize);

  #endif

  return retFont;
}



NA_DEF void naCenterMouse(void* uiElement) {
  double uiScale = naGetUIElementResolutionScale(NA_NULL);
  NARect rect = naGetUIElementRectAbsolute(uiElement);

  // todo: screen not defined
  NARect screenFrame = naGetMainScreenRect();
  NAPos centerPos = {
    rect.pos.x + rect.size.width * .5f,
    rect.pos.y + rect.size.height * .5f};

  NAApplication* app = naGetApplication();
  na_SetMouseWarpedTo(na_GetApplicationMouseStatus(app), centerPos);
  SetCursorPos(
    (int)(centerPos.x * uiScale),
    (int)((screenFrame.size.height - centerPos.y) * uiScale));
}



NA_DEF void naShowMouse() {
  if(!na_GetApplicationMouseVisible()) {
    ShowCursor(1);
    na_SetApplicationMouseVisible(NA_TRUE);
  }
}

NA_DEF void naHideMouse() {
  if(na_GetApplicationMouseVisible()) {
    ShowCursor(0);
    na_SetApplicationMouseVisible(NA_FALSE);
  }
}



NA_DEF void naHideMouseUntilMovement(NABool hide) {
  // todo
}

NA_HDEF UINT na_GetApplicationNextMenuItemId(NAApplication* application)
{
  NAWINAPIApplication* winapiApplication = (NAWINAPIApplication*)application;
  return winapiApplication->nextMenuItemId++;
}

NA_HDEF void na_SetApplicationLastOpenedMenu(NAApplication* application, const NAMenu* menu)
{
  NAWINAPIApplication* winapiApplication = (NAWINAPIApplication*)application;
  winapiApplication->lastOpenedMenu = menu;
}

NA_HDEF const NAMenu* na_GetApplicationLastOpenedMenu(NAApplication* application)
{
  NAWINAPIApplication* winapiApplication = (NAWINAPIApplication*)application;
  return winapiApplication->lastOpenedMenu;
}



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
