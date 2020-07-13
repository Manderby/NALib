
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



#include "../../../../../NAString.h"
#include "../../../../../NAURL.h"

#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  #include <commctrl.h>
#endif

// The following struct stores all relevant data which will then be stored in
// a list of the running NAWINAPIApplication.
typedef struct NAWINAPITimerStruct NAWINAPITimerStruct;
struct NAWINAPITimerStruct {
  UINT key;
  NAMutator func;
  void* arg;
};

typedef struct NAWINAPIColor NAWINAPIColor;
struct NAWINAPIColor {
  COLORREF color;
  HBRUSH brush;
};

// The struct NAWINAPIApplication stores a list of timers which could otherwise
// not be done.
typedef struct NAWINAPIApplication NAWINAPIApplication;
struct NAWINAPIApplication {
  NAApplication application;
  NAList timers;
  HWND offscreenWindow;
  NONCLIENTMETRICS nonClientMetrics;
  HICON appIcon;

  HFONT systemFont;
  HFONT titleFont;
  HFONT monospaceFont;
  HFONT paragraphFont;
  HFONT mathFont;

  NA_UIElement* mouseHoverElement;

  WNDPROC oldButtonWindowProc;
  WNDPROC oldRadioWindowProc;
  WNDPROC oldCheckBoxWindowProc;
  WNDPROC oldLabelWindowProc;
  WNDPROC oldTextFieldWindowProc;

  NAWINAPIColor fgColor;
  NAWINAPIColor fgColorDisabled;
  NAWINAPIColor bgColor;
  NAWINAPIColor bgColorAlternate;
  NAWINAPIColor bgColorAlternate2;
};



WNDPROC naGetApplicationOldButtonWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldButtonWindowProc;
}
WNDPROC naGetApplicationOldRadioWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldRadioWindowProc;
}
WNDPROC naGetApplicationOldCheckBoxWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldCheckBoxWindowProc;
}
WNDPROC naGetApplicationOldLabelWindowProc(){
  return ((NAWINAPIApplication*)naGetApplication())->oldLabelWindowProc;
}
WNDPROC naGetApplicationOldTextFieldWindowProc(){
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

    // Start the WINAPI application and set the native ID of the application.
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
      // Capture any keyboard shortcuts overwritten by the NALib user
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
  naReleaseUIElement(app);
}


NA_DEF void naResetApplicationPreferredTranslatorLanguages(void){
  int numchars;
  NAUTF8Char* languageBuf; 
  NALanguageCode3 languageCode;
  LCID userLocalId;

  userLocalId = GetUserDefaultLCID();
  numchars = GetLocaleInfoA(userLocalId, LOCALE_SISO639LANGNAME2, NA_NULL, 0);
  languageBuf = naMalloc((numchars + 1) * naSizeof(NAUTF8Char));
  GetLocaleInfoA(userLocalId, LOCALE_SISO639LANGNAME2, languageBuf, numchars);
  languageCode = naGetLanguageCode(languageBuf);

  // Tried to do this with GetUserPreferredUILanguages but it does not
  // work. Not reliably, not at all, not no nada.

  naSetTranslatorLanguagePreference(languageCode);

  naFree(languageBuf);
}



NA_HDEF NAApplication* na_NewApplication(void){

  NAWINAPIApplication* winapiApplication = naAlloc(NAWINAPIApplication);

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

  winapiApplication->oldButtonWindowProc = NA_NULL;
  winapiApplication->oldRadioWindowProc = NA_NULL;
  winapiApplication->oldCheckBoxWindowProc = NA_NULL;
  winapiApplication->oldLabelWindowProc = NA_NULL;
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



NA_DEF void na_DestructApplication(NAApplication* application){
  NAWINAPIApplication* app = (NAWINAPIApplication*)application;

  DestroyWindow(app->offscreenWindow);

  DeleteObject(app->fgColor.brush);
  DeleteObject(app->fgColorDisabled.brush);
  DeleteObject(app->bgColor.brush);
  DeleteObject(app->bgColorAlternate.brush);
  DeleteObject(app->bgColorAlternate2.brush);

  if(app->systemFont){DeleteObject(app->systemFont);}
  if(app->titleFont){DeleteObject(app->titleFont);}
  if(app->monospaceFont){DeleteObject(app->monospaceFont);}
  if(app->paragraphFont){DeleteObject(app->paragraphFont);}
  if(app->mathFont){DeleteObject(app->mathFont);}

  DestroyIcon(app->appIcon);

  na_ClearApplication(&(app->application));  

  // Now that all windows are destroyed, all dependent timers are deleted. We can
  // safely release the timer structs. todo: Make killing the timers a sport.
  naForeachListMutable(&(app->timers), (NAMutator)naFree);
  naClearList(&(app->timers));
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

  naBeginListModifierIteration(NAWINAPITimerStruct* timerstruct, &(app->timers), iter);
    if(timerstruct->key == timerkey) {
      naRemoveListCurMutable(&iter, NA_FALSE);
      KillTimer(hwnd, idEvent);
      timerstruct->func(timerstruct->arg);
      naFree(timerstruct);
      break;
    }
  naEndListIteration(iter);
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  NAWINAPIApplication* app;
  NAWINAPITimerStruct* timerstruct = naAlloc(NAWINAPITimerStruct);
  timerstruct->func = function;
  timerstruct->arg = arg;
  // todo: Check type
  timerstruct->key = (UINT)SetTimer((HWND)NA_NULL, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), na_TimerCallbackFunction);
  app = (NAWINAPIApplication*)naGetApplication();
  naAddListLastMutable(&(app->timers), timerstruct);
}



#if (NA_ADDRESS_BITS == 64)
  typedef intptr_t NAWINAPIHANDLE;
#elif (NA_ADDRESS_BITS == 32)
  typedef long NAWINAPIHANDLE;
#else
  #error "Undefined system address bytesize"
#endif



#include <io.h>
#include <fcntl.h>
NA_DEF void naOpenConsoleWindow(void){
  FILE *outFile;
  FILE *errFile;
  FILE *inFile;
  TCHAR* systemtitle;

  AllocConsole();

  systemtitle = TEXT("Debug Console");
  SetConsoleTitle(systemtitle);

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
    TCHAR modulepath[MAX_PATH];
    NAString* utf8modulepath;
    NAURL url;
    NAString* applicationName;
    NAString* applicationbasename;

    GetModuleFileName(NULL, modulepath, MAX_PATH);
    utf8modulepath = naNewStringFromSystemString(modulepath);

    naInitURLWithUTF8CStringLiteral(&url, naGetStringUTF8Pointer(utf8modulepath));
    naDelete(utf8modulepath);
    applicationName = naNewStringWithURLFilename(&url);
    applicationbasename = naNewStringWithBasenameOfFilename(applicationName);
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



NA_DEF NAFont na_GetFontWithKind(NAFontKind kind){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  HFONT retfont = NA_NULL;

  ////EnumFontFamilies(GetDC(NA_NULL), NA_NULL, enumFonts, NA_NULL);

  #if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1

  const NONCLIENTMETRICS* metrics = naGetApplicationMetrics();

  switch(kind){
    case NA_FONT_KIND_SYSTEM:
      if(!app->systemFont){
        app->systemFont = CreateFont(
          metrics->lfMessageFont.lfHeight,
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
        metrics->lfMessageFont.lfHeight,
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
        metrics->lfMessageFont.lfHeight,
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
        metrics->lfMessageFont.lfHeight,
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
        metrics->lfMessageFont.lfHeight,
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
      #ifndef NDEBUG
        naError("Unknown font kind");
      #endif
      break;
  }

  #endif

  return (NAFont)retfont;
}


NA_DEF void naCenterMouse(void* uiElement, NABool includebounds, NABool sendmovemessage){
  NARect spacerect;
  NARect screenframe;
  NAPos centerpos;
  spacerect = naGetUIElementRect(uiElement, naGetApplication(), includebounds);
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
