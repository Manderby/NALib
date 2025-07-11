
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



#include "../../../NAUtility/NATranslator.h"
#include "../../../NAUtility/NANotifier.h"



// The pointer storing the app if any.
NAApplication* na_App = NA_NULL;

NA_DEF NAApplication* naGetApplication(void) {
  #if NA_DEBUG
    if(!na_App)
      naError("Application not started. Use naStartApplication");
  #endif
  return na_App;
}



NA_HDEF NABool na_IsApplicationRunning(void) {
  NAApplication* app = naGetApplication();
  return (NABool)(app->flags & NA_APPLICATION_FLAG_RUNNING);
}



NA_HDEF void na_InitApplication(NAApplication* app, void* nativePtr) {
  #if NA_DEBUG
    if(na_App)
      naError("Application already started.");
  #endif
  na_App = app;

  naInitList(&app->screens);
  naInitList(&app->windows);
  naInitList(&app->uiElements);

  app->totalRect = na_FillScreenList(&app->screens);

  app->translator = NA_NULL;
  naStartTranslator();
  
  app->notifier = naAllocNotifier();
  naSetCurrentNotifier(app->notifier);

  // todo: make this a singleton.
  app->systemFont = NA_NULL;

  app->mouseStatus = na_AllocMouseStatus();
  app->keyStroke = naNewKeyStroke(NA_KEYCODE_ESCAPE, NA_KEY_MODIFIER_NONE); // 0 is not defined, just use esc.
  
  app->flags = 0;
  app->flags |= NA_APPLICATION_FLAG_RUNNING;
  app->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  #if NA_OS == NA_OS_WINDOWS
  app->flags |= NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING;
  #endif

  app->appName = NA_NULL;
  app->companyName = NA_NULL;
  app->versionString = NA_NULL;
  app->buildString = NA_NULL;
  app->resourceBasePath = NA_NULL;
  app->iconPath = NA_NULL;

  // This is done at the very end of the InitApplication function as the
  // application must be fully functional before it can init any UIElements.
  na_InitCoreUIElement(&app->uiElement, NA_UI_APPLICATION, nativePtr);
}



NA_HDEF void na_TerminateApplication(NAMutator cleanup, void* arg) {
  // Before deleting the application, we cleanup whatever the user needs to
  // clean up.
  if(cleanup)
    cleanup(arg);

  // Shutdown the preferences if necessary
  na_ShutdownPreferences();

  // Delete the application object itself.
  naDelete(na_App);
  
  // Finally, stop the runtime.
  naStopRuntime();
}



NA_HDEF void na_ClearApplication(NAApplication* app) {
  #if NA_DEBUG
    if(!na_App)
      naCrash("No Application running");
  #endif

  naDeallocNotifier(na_App->notifier);

  // An NAWindow removes itself from the windows array automatically. So
  // no naForeach is allowed here.
  while(!naIsListEmpty(&na_App->windows)) {
    // The window will remove itself from the list during deletion.
    naDelete(naGetListFirstMutable(&na_App->windows));
  }
  naClearList(&na_App->windows, NA_NULL);

  while(!naIsListEmpty(&na_App->screens)) {
    // The screen will NOT remove itself from the list during deletion.
    naDelete(naRemoveListFirstMutable(&na_App->screens));
  }
  naClearList(&na_App->screens, NA_NULL);

  naStopTranslator();
  na_ClearCoreUIElement(&app->uiElement);

  na_DeallocMouseStatus(app->mouseStatus);
  if(app->keyStroke) { naDelete(app->keyStroke); }

  if(app->systemFont)
    naRelease(app->systemFont);

  // This must be at the very end as the uiElements are used up until the last
  // ClearUIElement operation.
  // todo test if all uiElements are gone.
  naClearList(&na_App->uiElements, NA_NULL);
  
  if(app->appName)
    naDelete(app->appName);
  if(app->companyName)
    naDelete(app->companyName);
  if(app->versionString)
    naDelete(app->versionString);
  if(app->buildString)
    naDelete(app->buildString);
  if(app->resourceBasePath)
    naDelete(app->resourceBasePath);
  if(app->iconPath)
    naDelete(app->iconPath);
}



NA_HDEF NABool naIsScreenMain(const NAScreen* screen) {
  return screen->isMain;
}



NA_DEF size_t naGetApplicationScreenCount(void) {
  return naGetListCount(&na_App->screens);
}



NA_DEF const NAScreen* naGetApplicationScreenWithIndex(size_t index) {
  NAListIterator it = naMakeListAccessor(&na_App->screens);
  const NAScreen* retScreen = NA_NULL;
  if(naLocateListIndex(&it, index)) {
    retScreen = naGetListCurConst(&it);
  }
  naClearListIterator(&it);
  return retScreen;
}



NA_DEF const NAScreen* naGetApplicationScreenMain() {
  const NAScreen* mainScreen = NA_NULL;
  NAListIterator it = naMakeListAccessor(&na_App->screens);
  while(naIterateList(&it)) {
    const NAScreen* screen = naGetListCurConst(&it);
    if(naIsScreenMain(screen)) {
      mainScreen = screen;
      break;
    }
  }
  naClearListIterator(&it);
  return mainScreen;
}



NA_DEF const NAScreen* naGetApplicationScreenWithRelativePos(NAPos pos) {
  const NAScreen* desiredScreen = NA_NULL;
  double minDist = NA_INFINITY;
  NAPos minCenter = naMakePosZero();
  NAListIterator it = naMakeListAccessor(&na_App->screens);
  while(naIterateList(&it)) {
    const NAScreen* screen = naGetListCurConst(&it);
    NAPos relativeCenter = naGetScreenRelativeCenter(screen);
    double dist = naGetPosDistance(relativeCenter, pos);
    NABool newCenterFound = NA_FALSE;
    if(dist < minDist) {
      newCenterFound = NA_TRUE;
    } else if (dist == minDist) {
      if(relativeCenter.x < minCenter.x) {
        newCenterFound = NA_TRUE;
      } else if((relativeCenter.x == minCenter.x) && (relativeCenter.y < minCenter.y)) {
        newCenterFound = NA_TRUE;
      } 
    }
    if(newCenterFound) {
      desiredScreen = screen;
      minDist = dist;
      minCenter = relativeCenter;
    }
  }
  naClearListIterator(&it);
  return desiredScreen;
}



NA_DEF const NAScreen* naGetApplicationScreenWithPos(NAPos pos) {
  NAScreen* theScreen = NA_NULL;

  double minDist = NA_INFINITY;

  NAListIterator it = naMakeListMutator(&na_App->screens);
  while(naIterateList(&it)) {
    NAScreen* screen = naGetListCurMutable(&it);
    NARect rect = na_GetScreenRect(&screen->uiElement);
    NAPos center = naGetRectCenter(rect);
    if(naContainsRectPoint(rect, pos)) {
      theScreen = screen;
      break;
    }else{
      double dist = naGetPosDistance(center, pos);
      if(dist < minDist) {
        theScreen = screen;
        minDist = dist;
      }
    }
  }
  naClearListIterator(&it);
  return theScreen;
}



NA_DEF void naCorrectApplicationWindowRect(NARect* contentRect, NABool titleless) {
  NA_UNUSED(contentRect);
  NA_UNUSED(titleless);
//  NAPos topLeft = naMakePos(
//    contentRect.pos.x + 10.,
//    contentRect.pos.y + contentRect.size.height + 10.);
//
}



NA_HDEF NAScreen* na_GetApplicationScreenWithNativePtr(void* nativePtr) {
  NAScreen* theScreen = NA_NULL;
  NAListIterator it = naMakeListMutator(&na_App->screens);
  while(naIterateList(&it)) {
    NAScreen* screen = naGetListCurMutable(&it);
    if(naGetUIElementNativePtrConst(screen) == nativePtr) {
      theScreen = screen;
      break;
    }
  }
  naClearListIterator(&it);
  return theScreen;
}



NA_DEF void naSetApplicationName(const NAUTF8Char* name) {
  NAApplication* app = naGetApplication();
  if(app->appName)
    naDelete(app->appName);

  app->appName = name
    ? naNewStringWithFormat("%s", name)
    : NA_NULL;
}
NA_DEF void naSetApplicationCompanyName(const NAUTF8Char* name) {
  NAApplication* app = naGetApplication();
  if(app->companyName)
    naDelete(app->companyName);

  app->companyName = name
    ? naNewStringWithFormat("%s", name)
    : NA_NULL;
}
NA_DEF void naSetApplicationVersionString(const NAUTF8Char* string) {
  NAApplication* app = naGetApplication();
  if(app->versionString)
    naDelete(app->versionString);

  app->versionString = string
    ? naNewStringWithFormat("%s", string)
    : NA_NULL;
}
NA_DEF void naSetApplicationBuildString(const NAUTF8Char* string) {
  NAApplication* app = naGetApplication();
  if(app->buildString)
    naDelete(app->buildString);

  app->buildString = string
    ? naNewStringWithFormat("%s", string)
    : NA_NULL;
}
NA_DEF void naSetApplicationResourceBasePath(const NAUTF8Char* path) {
  NAApplication* app = naGetApplication();
  if(app->resourceBasePath)
    naDelete(app->resourceBasePath);

  app->resourceBasePath = path
    ? naNewStringWithFormat("%s", path)
    : NA_NULL;
}
NA_DEF void naSetApplicationIconPath(const NAUTF8Char* path) {
  NAApplication* app = naGetApplication();
  if(app->iconPath)
    naDelete(app->iconPath);

  app->iconPath = path
    ? naNewStringWithFormat("%s", path)
    : NA_NULL;

  na_UpdateApplicationIconPath();
}




NA_HDEF void na_RenewApplicationScreens() {
  // Create a copy of the old screen setup
  NAList oldScreenList;
  naInitListWithCopy(&oldScreenList, &na_App->screens);
//  NARect oldTotalRect = na_GetApplicationRect(na_App);
  
  // Clear the old list.
  naClearList(&na_App->screens, NA_NULL);
  
  // Gather the information of the new list
  na_App->totalRect = na_FillScreenList(&na_App->screens);
  
  // Go through all windows, readjust their position if necessary and reattach
  // them to a new screen.
  NAListIterator it = naMakeListMutator(&na_App->windows);
  while(naIterateList(&it)) {
    NAWindow* window = naGetListCurMutable(&it);
    NARect rect = na_GetWindowRect(&window->uiElement);
    NAPos center = naGetRectCenter(rect);
    na_UpdateWindowScreen(window, (NAScreen*)naGetApplicationScreenWithPos(center)); // evil cast
  }
  naClearListIterator(&it);
  
  // Delete the old screen objects.
  naClearList(&oldScreenList, naDelete);
  
  // Spread the information about the change.
  na_DispatchUIElementCommand(&na_App->uiElement, NA_UI_COMMAND_RESHAPE);
}




NA_HDEF NARect na_GetApplicationRect(const NAApplication* app) {
  const NAApplication* application = (const NAApplication*)app;
  return application->totalRect;
}

NA_HDEF void na_SetApplicationRect(const NAApplication* app, NARect rect) {
  NA_UNUSED(app);
  NA_UNUSED(rect);
  #if NA_DEBUG
    naError("Application rect can not be set but is defined by the screens available.");
  #endif
}




NA_HDEF const NAFont* na_GetApplicationSystemFont(const NAApplication* app) {
  if(!app->systemFont) {
    NAApplication* mutableApp = (NAApplication*)app;
    mutableApp->systemFont = naCreateSystemFont();
  }
  return app->systemFont;
}



NA_DEF const NAMouseStatus* naGetCurrentMouseStatus() {
  NAApplication* app = naGetApplication();
  return app->mouseStatus;
}



NA_HDEF NAMouseStatus* na_GetApplicationMouseStatus(NAApplication* app) {
  return app->mouseStatus;
}



NA_HDEF const NAKeyStroke* naGetCurrentKeyStroke() {
  NAApplication* app = naGetApplication();
  return app->keyStroke;
}



NA_HDEF void na_SetApplicationKeyStroke(NAApplication* app, NAKeyStroke* keyStroke) {
  if(app->keyStroke) { naDelete(app->keyStroke); }
  app->keyStroke = keyStroke;
}



NA_DEF void naStopApplication(void) {
  NAApplication* app = naGetApplication();
  naSetFlagu32(&app->flags, NA_APPLICATION_FLAG_RUNNING, NA_FALSE);
}



NA_HDEF NABool na_GetApplicationMouseVisible() {
  NAApplication* app = naGetApplication();
  return naGetFlagu32(app->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE);
}


NA_HDEF void na_SetApplicationMouseVisible(NABool visible) {
  NAApplication* app = naGetApplication();
  naSetFlagu32(&app->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE, visible);
}



NA_DEF NABool naGetDefaultWindowSystemKeyHandling() {
  NAApplication* app = naGetApplication();
  return naGetFlagu32(app->flags, NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING);
}

NA_DEF void naSetDefaultWindowSystemKeyHandling(NABool enable) {
  NAApplication* app = naGetApplication();
  naSetFlagu32(&app->flags, NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING, enable);
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
