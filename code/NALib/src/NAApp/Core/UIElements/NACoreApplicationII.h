
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



#include "../../../NAUtility/NATranslator.h"



// The pointer storing the app if any.
NAApplication* na_App = NA_NULL;



NA_HDEF NABool na_IsApplicationRunning(void) {
  return (NABool)(na_App->flags & NA_APPLICATION_FLAG_RUNNING);
}



NA_HDEF void na_InitApplication(NAApplication* app, void* nativePtr) {
  na_App = app;

  naInitList(&app->windows);
  naInitList(&app->uiElements);

  app->translator = NA_NULL;
  naStartTranslator();
  
  // todo: make this a singleton.
  app->systemFont = NA_NULL;

  app->mouseStatus = na_AllocMouseStatus();
  app->keyStroke = naNewKeyStroke(NA_KEYCODE_ESCAPE, 0); // 0 is not defined, just use esc.
  
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
  na_InitUIElement(&app->uiElement, NA_UI_APPLICATION, nativePtr);
}



NA_HDEF void na_ClearApplication(NAApplication* app) {
  #if NA_DEBUG
    if(!na_App)
      naCrash("No Application running");
  #endif

  naForeachListMutable(&na_App->windows, (NAMutator)naDelete);
  naClearList(&na_App->windows);

  naStopTranslator();
  na_ClearUIElement(&app->uiElement);

  na_DeallocMouseStatus(app->mouseStatus);
  if(app->keyStroke) { naDelete(app->keyStroke); }

  if(app->systemFont)
    naRelease(app->systemFont);

  // This must be at the very end as the uiElements are used up until the last
  // ClearUIElement operation.
  // todo test if all uiElements are gone.
  naClearList(&na_App->uiElements);
}



NA_DEF void naSetApplicationName(const NAUTF8Char* name) {
  NAApplication* app = naGetApplication();
  app->appName = name;
}
NA_DEF void naSetApplicationCompanyName(const NAUTF8Char* name) {
  NAApplication* app = naGetApplication();
  app->companyName = name;
}
NA_DEF void naSetApplicationVersionString(const NAUTF8Char* string) {
  NAApplication* app = naGetApplication();
  app->versionString = string;
}
NA_DEF void naSetApplicationBuildString(const NAUTF8Char* string) {
  NAApplication* app = naGetApplication();
  app->buildString = string;
}
NA_DEF void naSetApplicationResourceBasePath(const NAUTF8Char* path) {
  NAApplication* app = naGetApplication();
  app->resourceBasePath = path;
}
NA_DEF void naSetApplicationIconPath(const NAUTF8Char* path) {
  NAApplication* app = naGetApplication();
  app->iconPath = path;
  na_SetApplicationIconPath(path);
}




NA_HDEF NARect na_GetApplicationRect(const NAApplication* app) {
  NA_UNUSED(app);
  NARect rect = {{0., 0.}, {1., 1.}};
  return rect;
}

NA_HDEF void na_SetApplicationRect(const NAApplication* app, NARect rect) {
  NA_UNUSED(app);
  NA_UNUSED(rect);
  #if NA_DEBUG
    naError("Application rect can not be set.");
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
  #if NA_DEBUG
    if(!na_App)
      naCrash("Application not started. Use naStartApplication");
  #endif
  return na_App->mouseStatus;
}



NA_HDEF NAMouseStatus* na_GetApplicationMouseStatus(NAApplication* app) {
  return app->mouseStatus;
}



NA_HDEF const NAKeyStroke* naGetCurrentKeyStroke() {
  #if NA_DEBUG
    if(!na_App)
      naCrash("Application not started. Use naStartApplication");
  #endif
  return na_App->keyStroke;
}



NA_HDEF void na_SetApplicationKeyStroke(NAApplication* app, NAKeyStroke* keyStroke) {
  if(app->keyStroke) { naDelete(app->keyStroke); }
  app->keyStroke = keyStroke;
}



NA_DEF void naStopApplication(void) {
  naSetFlagu32(&na_App->flags, NA_APPLICATION_FLAG_RUNNING, NA_FALSE);
}



NA_DEF NAApplication* naGetApplication(void) {
  #if NA_DEBUG
    if(!na_App)
      naError("Application not started. Use naStartApplication");
  #endif
  return na_App;
}



NA_HDEF NABool na_GetApplicationMouseVisible() {
  #if NA_DEBUG
    if(!na_App)
      naCrash("Application not started. Use naStartApplication");
  #endif
  return naGetFlagu32(na_App->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE);
}


NA_HDEF void na_SetApplicationMouseVisible(NABool visible) {
  #if NA_DEBUG
  if(!na_App)
    naCrash("Application not started. Use naStartApplication");
  #endif
  naSetFlagu32(&na_App->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE, visible);
}



NA_DEF NABool naGetDefaultWindowSystemKeyHandling() {
  #if NA_DEBUG
    if(!na_App)
      naCrash("Application not started. Use naStartApplication");
  #endif
  return naGetFlagu32(na_App->flags, NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING);
}

NA_DEF void naSetDefaultWindowSystemKeyHandling(NABool enable) {
  #if NA_DEBUG
    if(!na_App)
      naCrash("Application not started. Use naStartApplication");
  #endif
  naSetFlagu32(&na_App->flags, NA_APPLICATION_FLAG_DEFAULT_SYSKEY_HANDLING, enable);
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
