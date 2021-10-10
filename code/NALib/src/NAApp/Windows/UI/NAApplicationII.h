
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



#include "../../../NAString.h"
#include "../../../NAURL.h"

#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  #include <commctrl.h>
#endif



WNDPROC na_GetApplicationOldButtonWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldButtonWindowProc;
}
WNDPROC na_GetApplicationOldCheckBoxWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldCheckBoxWindowProc;
}
WNDPROC na_GetApplicationOldLabelWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldLabelWindowProc;
}
WNDPROC na_GetApplicationOldRadioWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldRadioWindowProc;
}
WNDPROC na_GetApplicationOldSliderWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldSliderWindowProc;
}
WNDPROC na_GetApplicationOldTextFieldWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldTextFieldWindowProc;
}



NAWINAPICallbackInfo naApplicationWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  default:
    //printf("Uncaught Application message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF void naStartApplication(NAMutator preStartup, NAMutator postStartup, void* arg){
  NAWINAPIApplication* app;
  WNDCLASS wndclass;
  MSG message;

  // Uncommented for future use.
  //SetProcessDPIAware();
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
  if(preStartup){preStartup(arg);}

  // Set the language of the translator
  naResetApplicationPreferredTranslatorLanguages();

  // Call postStartup if desired.
  if(postStartup){postStartup(arg);}

  // Start the event loop.
  while(na_IsApplicationRunning()){
    BOOL response = GetMessage(&message, 0, 0, 0);
    naCollectGarbage();
    if(response == 0){break;}
    if (response == -1){
      // handle the error and possibly exit
      break;
    }else{
      if(message.message == WM_QUIT){
        break;
      }
      //if(!IsDialogMessage(message.hwnd, &message)){
      // Capture any keyboard shortcuts overridden by the NALib user
      // Note: Usually, the IsDialogMessage function is responsible for
      // capturing TAB events but it has proven to be difficult to handle
      // and therefore, everything is captured in na_InterceptKeyboardShortcut.
      if(!na_InterceptKeyboardShortcut(&message)){
        // Do the normal message dispatch.
        TranslateMessage(&message);
        DispatchMessage(&message);
      }
      //}
    }
  }

  // When reaching here, the application had been stopped.
  naDelete(app);
}


NA_DEF void naResetApplicationPreferredTranslatorLanguages(void){
  int numchars;
  NAUTF8Char* languageBuf; 
  NALanguageCode3 languageCode;
  LCID userLocalId;

  userLocalId = GetUserDefaultLCID();
  numchars = GetLocaleInfoA(userLocalId, LOCALE_SISO639LANGNAME2, NA_NULL, 0);
  languageBuf = naMalloc((numchars + 1) * sizeof(NAUTF8Char));
  GetLocaleInfoA(userLocalId, LOCALE_SISO639LANGNAME2, languageBuf, numchars);
  languageCode = naGetLanguageCode(languageBuf);

  // Tried to do this with GetUserPreferredUILanguages but it does not
  // work. Not reliably, not at all, not no nada.

  naSetTranslatorLanguagePreference(languageCode);

  naFree(languageBuf);
}



NA_HDEF NAApplication* na_NewApplication(void){

  NAWINAPIApplication* winapiApplication = naNew(NAWINAPIApplication);

  na_InitApplication(&(winapiApplication->application), GetModuleHandle(NULL));

  naInitList(&(winapiApplication->timers));

	winapiApplication->offscreenWindow = CreateWindow(
		TEXT("NAOffscreenWindow"), TEXT("Offscreen window"), WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL, NULL, GetModuleHandle(NULL), NULL);

  winapiApplication->nonClientMetrics.cbSize = sizeof(NONCLIENTMETRICS);
  SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &(winapiApplication->nonClientMetrics), 0);

  winapiApplication->appIcon = NA_NULL;

  winapiApplication->systemFont = NA_NULL;
  winapiApplication->titleFont = NA_NULL;
  winapiApplication->monospaceFont = NA_NULL;
  winapiApplication->paragraphFont = NA_NULL;
  winapiApplication->mathFont = NA_NULL;

  winapiApplication->mouseHoverElement = NA_NULL;
  winapiApplication->lastOpenedMenu = NA_NULL;
  winapiApplication->nextMenuItemId = 1;

  winapiApplication->oldButtonWindowProc = NA_NULL;
  winapiApplication->oldCheckBoxWindowProc = NA_NULL;
  winapiApplication->oldLabelWindowProc = NA_NULL;
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



NA_DEF void na_DestructWINAPIApplication(NAWINAPIApplication* winapiApplication){
  DestroyWindow(winapiApplication->offscreenWindow);

  DeleteObject(winapiApplication->fgColor.brush);
  DeleteObject(winapiApplication->fgColorDisabled.brush);
  DeleteObject(winapiApplication->bgColor.brush);
  DeleteObject(winapiApplication->bgColorAlternate.brush);
  DeleteObject(winapiApplication->bgColorAlternate2.brush);

  if(winapiApplication->systemFont){DeleteObject(winapiApplication->systemFont);}
  if(winapiApplication->titleFont){DeleteObject(winapiApplication->titleFont);}
  if(winapiApplication->monospaceFont){DeleteObject(winapiApplication->monospaceFont);}
  if(winapiApplication->paragraphFont){DeleteObject(winapiApplication->paragraphFont);}
  if(winapiApplication->mathFont){DeleteObject(winapiApplication->mathFont);}

  DestroyIcon(winapiApplication->appIcon);

  na_ClearApplication(&(winapiApplication->application));  

  // Now that all windows are destroyed, all dependent timers are deleted. We can
  // safely release the timer structs. todo: Make killing the timers a sport.
  naForeachListMutable(&(winapiApplication->timers), (NAMutator)naFree);
  naClearList(&(winapiApplication->timers));
}



HWND naGetApplicationOffscreenWindow(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->offscreenWindow;
}

NA_UIElement* naGetApplicationMouseHoverElement(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->mouseHoverElement;
}

void naSetApplicationMouseHoverElement(NA_UIElement* element){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->mouseHoverElement = element;
}

const NONCLIENTMETRICS* naGetApplicationMetrics(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return &(app->nonClientMetrics);
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
  NAListIterator iter;

  UINT timerkey = (UINT)idEvent;
  app = (NAWINAPIApplication*)naGetApplication();

  naBeginListModifierIteration(NAWINAPITimerStruct* timerStruct, &(app->timers), iter);
    if(timerStruct->key == timerkey) {
      naRemoveListCurMutable(&iter, NA_FALSE);
      KillTimer(hwnd, idEvent);
      timerStruct->func(timerStruct->arg);
      naFree(timerStruct);
      break;
    }
  naEndListIteration(iter);
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  NAWINAPIApplication* app;
  NAWINAPITimerStruct* timerStruct = naAlloc(NAWINAPITimerStruct);
  timerStruct->func = function;
  timerStruct->arg = arg;
  // todo: Check type
  timerStruct->key = (UINT)SetTimer((HWND)NA_NULL, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), na_TimerCallbackFunction);
  app = (NAWINAPIApplication*)naGetApplication();
  naAddListLastMutable(&(app->timers), timerStruct);
}



#if (NA_ADDRESS_BITS == 64)
  typedef intptr_t NAWINAPIHANDLE;
#elif (NA_ADDRESS_BITS == 32)
  typedef long NAWINAPIHANDLE;
#else
  #error "Undefined system address byteSize"
#endif



#include <io.h>
#include <fcntl.h>
NA_DEF void naOpenConsoleWindow(void){
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



NA_DEF void naSetApplicationName(NAUTF8Char* name){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->application.name = name;
}
NA_DEF void naSetApplicationCompanyName(NAUTF8Char* name){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->application.companyName = name;
}
NA_DEF void naSetApplicationVersionString(NAUTF8Char* string){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->application.versionString = string;
}
NA_DEF void naSetApplicationBuildString(NAUTF8Char* string){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->application.buildString = string;
}
NA_DEF void naSetApplicationIconPath(NAUTF8Char* path){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->application.iconPath = path;

  if(path){
    NABabyImage* iconBabyImage = naCreateBabyImageFromFilePath(path);
    HBITMAP bitmap = naAllocNativeImageWithBabyImage(iconBabyImage);
   
    HBITMAP hbmMask = CreateCompatibleBitmap(
      GetDC(NULL), 
      (int)naGetBabyImageSize(iconBabyImage).width,
      (int)naGetBabyImageSize(iconBabyImage).height);

    ICONINFO ii = {0};
    ii.fIcon    = TRUE;
    ii.hbmColor = bitmap;
    ii.hbmMask  = hbmMask;
    app->appIcon = CreateIconIndirect(&ii);
  }
}



NA_DEF NAString* naNewApplicationName(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->application.name){
    return naNewStringWithFormat("%s", app->application.name);
  }else{
    TCHAR modulePath[MAX_PATH];
    NAString* utf8ModulePath;
    NAURL url;
    NAString* applicationName;
    NAString* applicationbasename;

    GetModuleFileName(NULL, modulePath, MAX_PATH);
    utf8ModulePath = naNewStringFromSystemString(modulePath);

    naInitURLWithUTF8CStringLiteral(&url, naGetStringUTF8Pointer(utf8ModulePath));
    naDelete(utf8ModulePath);
    applicationName = naNewStringWithURLFilename(&url);
    applicationbasename = naNewStringWithBasenameOfPath(applicationName);
    naClearURL(&url);
    naDelete(applicationName);

    return applicationbasename;
  }
}



NA_DEF NAString* naNewApplicationCompanyName(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->application.companyName){
    return naNewStringWithFormat("%s", app->application.companyName);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationVersionString(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->application.versionString){
    return naNewStringWithFormat("%s", app->application.versionString);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationBuildString(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->application.buildString){
    return naNewStringWithFormat("%s", app->application.buildString);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationIconPath(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->application.iconPath){
    return naNewStringWithFormat("%s", app->application.iconPath);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NAString* retString;
  if(dir){
    retString = naNewStringWithFormat("%s/%s.%s", dir, basename, suffix);
  }else{
    retString = naNewStringWithFormat("%s.%s", basename, suffix);
  }
  return retString;
}



NA_DEF HICON naGetWINAPIApplicationIcon(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->appIcon;
}



// This is just a small code snipplet useful for debugging. See call to EnumFontFamilies below.
//int CALLBACK enumFonts(
//  _In_ ENUMLOGFONT   *lpelf,
//  _In_ NEWTEXTMETRIC *lpntm,
//  _In_ DWORD         FontType,
//  _In_ LPARAM        lParam
//){
//  int x = 1234;
//  printf("%s"  NA_NL, lpelf->elfFullName);
//}



NA_DEF NAFont na_GetFontWithKindAndSize(NAFontKind kind, NAFontSize size){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  
  HFONT retfont = NA_NULL;

  ////EnumFontFamilies(GetDC(NA_NULL), NA_NULL, enumFonts, NA_NULL);

  #if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1

  const NONCLIENTMETRICS* metrics = naGetApplicationMetrics();

  LONG baseSize;
  switch(size){
  case NA_FONT_SIZE_SMALL: baseSize = 12; break;
  case NA_FONT_SIZE_DEFAULT: baseSize = 14; break;
  //case NA_FONT_SIZE_DEFAULT: baseSize = metrics->lfMessageFont.lfHeight; break;
  case NA_FONT_SIZE_BIG: baseSize = 18; break;
  case NA_FONT_SIZE_HUGE: baseSize = 24; break;
  default: baseSize = metrics->lfMessageFont.lfHeight; break;
  }

  switch(kind){
    case NA_FONT_KIND_SYSTEM:
      if(!app->systemFont){
        app->systemFont = CreateFont(
          baseSize,
          0,
          0,
          0,
          FW_NORMAL,
          NA_FALSE,
          NA_FALSE,
          NA_FALSE,
          DEFAULT_CHARSET,
          OUT_DEFAULT_PRECIS,
          CLIP_DEFAULT_PRECIS,
          DEFAULT_QUALITY,
          DEFAULT_PITCH | FF_DONTCARE,
          metrics->lfMessageFont.lfFaceName);
      }
      retfont = app->systemFont;
      break;
    case NA_FONT_KIND_TITLE:
      if(!app->titleFont){
        app->titleFont = CreateFont(
        baseSize,
        0,
        0,
        0,
        FW_BOLD,
        NA_FALSE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        metrics->lfMessageFont.lfFaceName);
      }
      retfont = app->titleFont;
      break;
    case NA_FONT_KIND_MONOSPACE:
      if(!app->monospaceFont){
        app->monospaceFont = CreateFont(
        baseSize,
        0,
        0,
        0,
        FW_BOLD,
        NA_FALSE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Courier New"));
      }
      retfont = app->monospaceFont;
      break;
    case NA_FONT_KIND_PARAGRAPH:
      if(!app->paragraphFont){
        app->paragraphFont = CreateFont(
        baseSize,
        0,
        0,
        0,
        FW_BOLD,
        NA_FALSE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Palatino Linotype"));
      }
      retfont = app->paragraphFont;
      break;
    case NA_FONT_KIND_MATH:
      if(!app->mathFont){
        app->mathFont = CreateFont(
        baseSize,
        0,
        0,
        0,
        FW_BOLD,
        NA_TRUE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Times New Roman"));
      }
      retfont = app->mathFont;
      break;
    default:
      #if NA_DEBUG
        naError("Unknown font kind");
      #endif
      break;
  }

  #endif

  return (NAFont)retfont;
}


NA_DEF void naCenterMouse(void* uiElement, NABool includeBorder){
  NARect spacerect;
  NARect screenframe;
  NAPos centerpos;
  spacerect = naGetUIElementRect(uiElement, naGetApplication(), includeBorder);
  // todo: screen not defined
  screenframe = naGetMainScreenRect();
  centerpos.x = spacerect.pos.x + spacerect.size.width * .5f;
  centerpos.y = spacerect.pos.y + spacerect.size.height * .5f;

  na_SetMouseWarpedTo(centerpos);
  SetCursorPos((int)centerpos.x, (int)screenframe.size.height - (int)centerpos.y);
}



NA_DEF void naShowMouse(){
  NAApplication* app = naGetApplication();
  if(!(app->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE)){
    ShowCursor(1);
    app->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}


NA_DEF void naHideMouse(){
  NAApplication* app = naGetApplication();
  if(app->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE){
    ShowCursor(0);
    app->flags &= ~NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}


NA_HDEF UINT na_GetApplicationNextMenuItemId(NAApplication* application)
{
  NAWINAPIApplication* winapiApplication = (NAWINAPIApplication*)application;
  return winapiApplication->nextMenuItemId++;
}

NA_HDEF void na_SetApplicationLastOpenedMenu(NAApplication* application, NAMenu* menu)
{
  NAWINAPIApplication* winapiApplication = (NAWINAPIApplication*)application;
  winapiApplication->lastOpenedMenu = menu;
}

NA_HDEF NAMenu* na_GetApplicationLastOpenedMenu(NAApplication* application)
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
