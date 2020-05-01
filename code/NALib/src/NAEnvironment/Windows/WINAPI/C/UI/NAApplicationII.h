
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



#include "../../../../../NAString.h"
#include "../../../../../NAURL.h"

#if NA_CONFIG_USE_WINDOWS_COMMON_CONTROLS_6 == 1
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
  NACoreApplication coreapplication;
  NAList timers;
  HWND offscreenWindow;
  NONCLIENTMETRICS nonclientmetrics;
  HICON appIcon;

  HFONT systemFont;
  HFONT titleFont;
  HFONT monospaceFont;
  HFONT paragraphFont;
  HFONT mathFont;

  NACoreUIElement* mouseHoverElement;

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



NAWINAPICallbackInfo naApplicationWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  default:
    //printf("Uncaught Application message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  NAWINAPIApplication* app;
  WNDCLASS wndclass;
  MSG message;

  // Uncommented for future use.
  //SetProcessDPIAware();
  //DPI_AWARENESS awareness = DPI_AWARENESS_SYSTEM_AWARE;
  //SetProcessDpiAwarenessContext(&awareness);

  #if NA_CONFIG_USE_WINDOWS_COMMON_CONTROLS_6 == 1
    InitCommonControls();   // enable visual styles
  #endif
  
  // First, register the window classes. This is required as the
  // offscreen window class must be present upon the call to
  // naNewApplication.

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
  app = naNewApplication();

  // Call prestartup if desired.
  if(prestartup){prestartup(arg);}

  // Set the language of the translator
  naResetApplicationPreferredTranslatorLanguages();

  // Call poststartup if desired.
  if(poststartup){poststartup(arg);}

  // Start the event loop.
  while(naIsCoreApplicationRunning()){
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
      // and therefore, everything is captured in naInterceptKeyboardShortcut.
      if(!naInterceptKeyboardShortcut(&message)){
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



NA_HDEF NAApplication* naNewApplication(void){

  NAWINAPIApplication* winapiapplication = naAlloc(NAWINAPIApplication);

  naInitCoreApplication(&(winapiapplication->coreapplication), GetModuleHandle(NULL));

  naInitList(&(winapiapplication->timers));

	winapiapplication->offscreenWindow = CreateWindow(
		TEXT("NAOffscreenWindow"), TEXT("Offscreen window"), WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL, NULL, GetModuleHandle(NULL), NULL);

  winapiapplication->nonclientmetrics.cbSize = sizeof(NONCLIENTMETRICS);
  SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &(winapiapplication->nonclientmetrics), 0);

  winapiapplication->appIcon = NA_NULL;

  winapiapplication->systemFont = NA_NULL;
  winapiapplication->titleFont = NA_NULL;
  winapiapplication->monospaceFont = NA_NULL;
  winapiapplication->paragraphFont = NA_NULL;
  winapiapplication->mathFont = NA_NULL;

  winapiapplication->mouseHoverElement = NA_NULL;

  winapiapplication->oldButtonWindowProc = NA_NULL;
  winapiapplication->oldRadioWindowProc = NA_NULL;
  winapiapplication->oldCheckBoxWindowProc = NA_NULL;
  winapiapplication->oldLabelWindowProc = NA_NULL;
  winapiapplication->oldTextFieldWindowProc = NA_NULL;

  winapiapplication->fgColor.color = GetSysColor(COLOR_WINDOWTEXT);
  winapiapplication->fgColorDisabled.color = GetSysColor(COLOR_GRAYTEXT);
  winapiapplication->bgColor.color = GetSysColor(COLOR_BTNFACE);
  winapiapplication->bgColorAlternate.color = RGB(226, 226, 226);
  winapiapplication->bgColorAlternate2.color = RGB(205, 205, 205);

  winapiapplication->fgColor.brush = CreateSolidBrush(winapiapplication->fgColor.color);
  winapiapplication->fgColorDisabled.brush = CreateSolidBrush(winapiapplication->fgColorDisabled.color);
  winapiapplication->bgColor.brush = CreateSolidBrush(winapiapplication->bgColor.color);
  winapiapplication->bgColorAlternate.brush = CreateSolidBrush(winapiapplication->bgColorAlternate.color);
  winapiapplication->bgColorAlternate2.brush = CreateSolidBrush(winapiapplication->bgColorAlternate2.color);

  return (NAApplication*)winapiapplication;
}



NA_DEF void naDestructApplication(NAApplication* application){
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

  naClearCoreApplication(&(app->coreapplication));  

  // Now that all windows are destroyed, all dependent timers are deleted. We can
  // safely release the timer structs. todo: Make killing the timers a sport.
  naForeachListMutable(&(app->timers), (NAMutator)naFree);
  naClearList(&(app->timers));
}



HWND naGetApplicationOffscreenWindow(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->offscreenWindow;
}

NACoreUIElement* naGetApplicationMouseHoverElement(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->mouseHoverElement;
}

void naSetApplicationMouseHoverElement(NACoreUIElement* element){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->mouseHoverElement = element;
}

const NONCLIENTMETRICS* naGetApplicationMetrics(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return &(app->nonclientmetrics);
}


// ///////////////////////////////////
// TIMER
// ///////////////////////////////////


// This is the native WINAPI callback function. It finds the corresponding
// registered timer struct of the application and executes the function
// stored in that struct with the stored argument.
//
// Definitely not the fastest and best method. But as for now, it's ok. todo.
NA_HDEF static VOID CALLBACK naTimerCallbackFunction(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
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
  timerstruct->key = (UINT)SetTimer((HWND)NA_NULL, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), naTimerCallbackFunction);
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
  app->coreapplication.name = name;
}
NA_DEF void naSetApplicationCompanyName(NAUTF8Char* name){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->coreapplication.companyName = name;
}
NA_DEF void naSetApplicationVersionString(NAUTF8Char* string){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->coreapplication.versionString = string;
}
NA_DEF void naSetApplicationBuildString(NAUTF8Char* string){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->coreapplication.buildString = string;
}
NA_DEF void naSetApplicationIconPath(NAUTF8Char* path){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  app->coreapplication.iconPath = path;

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
  if(app->coreapplication.name){
    return naNewStringWithFormat("%s", app->coreapplication.name);
  }else{
    TCHAR modulepath[MAX_PATH];
    NAString* utf8modulepath;
    NAURL url;
    NAString* applicationname;
    NAString* applicationbasename;

    GetModuleFileName(NULL, modulepath, MAX_PATH);
    utf8modulepath = naNewStringFromSystemString(modulepath);

    naInitURLWithUTF8CStringLiteral(&url, naGetStringUTF8Pointer(utf8modulepath));
    naDelete(utf8modulepath);
    applicationname = naNewStringWithURLFilename(&url);
    applicationbasename = naNewStringWithBasenameOfFilename(applicationname);
    naClearURL(&url);
    naDelete(applicationname);

    return applicationbasename;
  }
}



NA_DEF NAString* naNewApplicationCompanyName(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->coreapplication.companyName){
    return naNewStringWithFormat("%s", app->coreapplication.companyName);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationVersionString(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->coreapplication.versionString){
    return naNewStringWithFormat("%s", app->coreapplication.versionString);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationBuildString(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->coreapplication.buildString){
    return naNewStringWithFormat("%s", app->coreapplication.buildString);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationIconPath(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  if(app->coreapplication.iconPath){
    return naNewStringWithFormat("%s", app->coreapplication.iconPath);
  }else{
    return NA_NULL;
  }
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NAString* retstring;
  if(dir){
    retstring = naNewStringWithFormat("%s/%s.%s", dir, basename, suffix);
  }else{
    retstring = naNewStringWithFormat("%s.%s", basename, suffix);
  }
  return retstring;
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



NA_DEF NAFont getFontWithKind(NAFontKind kind){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  HFONT retfont = NA_NULL;

  ////EnumFontFamilies(GetDC(NA_NULL), NA_NULL, enumFonts, NA_NULL);

  #if NA_CONFIG_USE_WINDOWS_COMMON_CONTROLS_6 == 1

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


NA_DEF void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage){
  NARect spacerect;
  NARect screenframe;
  NAPos centerpos;
  spacerect = naGetUIElementRect(uielement, naGetApplication(), includebounds);
  // todo: screen not defined
  screenframe = naGetMainScreenRect();
  centerpos.x = spacerect.pos.x + spacerect.size.width * .5f;
  centerpos.y = spacerect.pos.y + spacerect.size.height * .5f;

  naSetMouseWarpedTo(centerpos);
  SetCursorPos((int)centerpos.x, (int)screenframe.size.height - (int)centerpos.y);
}



NA_DEF void naShowMouse(){
  NACoreApplication* coreapp = (NACoreApplication*)naGetApplication();
  if(!(coreapp->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE)){
    ShowCursor(1);
    coreapp->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}


NA_DEF void naHideMouse(){
  NACoreApplication* coreapp = (NACoreApplication*)naGetApplication();
  if(coreapp->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE){
    ShowCursor(0);
    coreapp->flags &= ~NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}


// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
