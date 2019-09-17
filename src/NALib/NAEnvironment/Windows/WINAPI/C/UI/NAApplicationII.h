
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


//@implementation NANativeApplicationDelegate
//- (id) initWithCoreApplication:(NACoreApplication*)newcoreapplication{
//  self = [super init];
//  coreapplication = newcoreapplication;
//  return self;
//}
//- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
//  NA_UNUSED(sender);
//  naStopApplication();
//  return NSTerminateCancel;
//}
//- (void)applicationDidFinishLaunching:(NSNotification *)notification{
//  [NSApp applicationDidFinishLaunching:notification];
//}
//@end



NA_DEF void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  NAWINAPIApplication* app;
  WNDCLASS wndclass;
  MSG message;

  // Start the WINAPI application and set the native ID of the application.
  app = naNewApplication();

  // Init the timer list.
  naInitList(&(app->timers));

  // Call prestartup if desired.
  if(prestartup){prestartup(arg);}

  // Set the language of the translator
  naResetApplicationPreferredTranslatorLanguages();

  // Register the window class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = naGetUIElementNativeID(app);
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NAWindow");
	RegisterClass(&wndclass);

  // Register the space class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = naGetUIElementNativeID(app);
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NASpace");
	RegisterClass(&wndclass);

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
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  // When reaching here, the application had been stopped.
//  naClearCoreApplication();
//  naReleaseUIElement(app);
}




NA_DEF void naResetApplicationPreferredTranslatorLanguages(void){
//  NAInt lang = (NAInt)[[NSLocale preferredLanguages] count] - 1;
//  while(lang >= 0){
//    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)lang];
//    NALanguageCode3 langcode = naGetLanguageCode([language UTF8String]);
//    naSetTranslatorLanguagePreference(langcode);
//    lang--;
//  }
}



NA_HDEF NAApplication* naNewApplication(void){
  NAWINAPIApplication* winapiapplication = naAlloc(NAWINAPIApplication);

  NANativeID nativeApp = GetModuleHandle(NULL);

  naInitCoreApplication(&(winapiapplication->coreapplication), nativeApp);

  return (NAApplication*)winapiapplication;
}


NA_DEF void naDestructApplication(NAApplication* application){
//  NACoreApplication* coreapplication = (NACoreApplication*)application;
//  naClearCoreApplication(coreapplication);
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  NAWINAPIApplication* app;
  NATimerStruct* timerstruct = naAlloc(NATimerStruct);
  timerstruct->func = function;
  timerstruct->arg = arg;
  // todo: Check type
  timerstruct->key = (UINT)SetTimer((HWND)NA_NULL, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), naTimerCallbackFunction);
  app = (NAWINAPIApplication*)naGetApplication();
  naAddListLastMutable(&(app->timers), timerstruct);
}



#include <io.h>
#include <fcntl.h>
NA_DEF void naOpenConsoleWindow(const char* windowtitle){
//  int outHandle, errHandle, inHandle;
  FILE *outFile;
  FILE *errFile;
  FILE *inFile;
//  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  AllocConsole();
  SetConsoleTitle(windowtitle);
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
//    coninfo.dwSize.Y = 9999;
//    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

  freopen_s(&inFile, "CONIN$", "r", stdin);
  freopen_s(&outFile, "CONOUT$", "w", stdout);
  freopen_s(&errFile, "CONERR$", "w", stderr);

  //outHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
  //errHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
  //inHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );

  //outFile = _fdopen(outHandle, "w" );
  //errFile = _fdopen(errHandle, "w");
  //inFile =  _fdopen(inHandle, "r");

  //*stdout = *outFile;
  //*stderr = *errFile;
  //*stdin = *inFile;

  //setvbuf( stdout, NULL, _IONBF, 0 );
  //setvbuf( stderr, NULL, _IONBF, 0 );
  //setvbuf( stdin, NULL, _IONBF, 0 );

}


//#define NA_COCOA_BUNDLE_PLIST @"InfoPlist"
//#define NA_COCOA_BUNDLE_APPLICATION_NAME @"CFBundleDisplayName"
//#define NA_COCOA_BUNDLE_VERSION_SHORT_KEY @"CFBundleShortVersionString"
//#define NA_COCOA_BUNDLE_VERSION_KEY @"CFBundleVersion"
//#define NA_COCOA_BUNDLE_ICON_FILE_KEY @"CFBundleIconFile"

NA_DEF NAString* naNewApplicationName(void){
//  NSString* applicationname = [[NSBundle mainBundle] localizedStringForKey:NA_COCOA_BUNDLE_APPLICATION_NAME value:nil table:NA_COCOA_BUNDLE_PLIST];
//  if(!applicationname){
//    applicationname = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_APPLICATION_NAME];
//  }
//  NAString* retstring = naNewStringWithFormat("%s", [applicationname UTF8String]);
//  return retstring;
  return NA_NULL;
}

NA_DEF NAString* naNewApplicationVersionString(void){
//  NSString* versionstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_SHORT_KEY];
//  NAString* retstring = naNewStringWithFormat("%s", [versionstring UTF8String]);
//  return retstring;
  return NA_NULL;
}

NA_DEF NAString* naNewApplicationBuildString(void){
//  NSString* buildstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_KEY];
//  NAString* retstring = naNewStringWithFormat("%s", [buildstring UTF8String]);
//  return retstring;
  return NA_NULL;
}

NA_DEF NAString* naNewApplicationIconPath(void){
//  NSString* iconfilename = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_ICON_FILE_KEY];
//  NSString* iconbasename = [iconfilename stringByDeletingPathExtension];
//  NSURL* url = [[NSBundle mainBundle] URLForResource:iconbasename withExtension:@"icns"];
//  NAString* retstring = naNewStringWithFormat("%s", [[url path] UTF8String]);
//  return retstring;
  return NA_NULL;
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
//  NSURL* url;
//  if(dir){
//    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix] subdirectory:[NSString stringWithUTF8String:dir]];
//  }else{
//    url = [[NSBundle mainBundle] URLForResource:[NSString stringWithUTF8String:basename] withExtension:[NSString stringWithUTF8String:suffix]];
//  }
//  NAString* retstring = naNewStringWithFormat("%s", [[url path] UTF8String]);
//  return retstring;
  return NA_NULL;
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
